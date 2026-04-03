#pragma once

#include <cstddef>
#include <cstdint>
#include <cinttypes>
#include <cstring>
#include <cstdio>
#include <type_traits>
#include <limits>
#include <utility>
#include <memory>
#include <string>

namespace efmt {

template <typename R>
class Formatter;

// A class you can specialize in order to effectively implement `Format` for types you don't own.
// (primitives, `std` collections, types from other libraries etc.)  
// If you want to be able to format a class you do own, implement `Format` or `DynFormat` for it instead.
template <typename R, typename T, typename Enable = void>
class FormatSpecializer {
public:
    void write(Formatter<R> &f, const T &value) {
        value.format(f);
    }
};

// Provided specializations:

class DynRef;
class DynFormat;

template<typename R, typename T>
class FormatSpecializer<R, T, typename std::enable_if<
    !std::is_same<R, DynRef>::value &&
    std::is_base_of<DynFormat, T>::value
>::type> {
public:
    void write(Formatter<R> &f, const T &value) {
        Formatter<DynRef> dyn = f.as_dyn_ref();
        value.format(dyn);
    }
};

template <typename R>
class FormatSpecializer<R, char> {
public:
    void write(Formatter<R> &f, const char &value) {
        f.write_char(value);
    }
};

template <typename R>
class FormatSpecializer<R, std::nullptr_t> {
public:
    void write(Formatter<R> &f, const std::nullptr_t &) {
        f.write("<NULLPTR>");
    }
};

template <typename R>
class FormatSpecializer<R, const char *> {
public:
    void write(Formatter<R> &f, const char * const &value) {
        if (value == nullptr) {
            f.write(nullptr);
        } else {
            f.write_multiple_chars(value, std::strlen(value));
        }
    }
};

template <typename R, std::size_t LEN>
class FormatSpecializer<R, char [LEN]> {
public:
    void write(Formatter<R> &f, const char (&value)[LEN]) {
        std::size_t len = value[LEN - 1] == '\0'? LEN - 1 : LEN;

        f.write_multiple_chars(value, len);
    }
};

template <typename R, typename Traits, typename Allocator>
class FormatSpecializer<R, std::basic_string<char, Traits, Allocator>> {
public:
    void write(Formatter<R> &f, const std::basic_string<char, Traits, Allocator> &value) {
        f.write_multiple_chars(value.c_str(), value.size());
    }
};

template <typename R>
class FormatSpecializer<R, bool> {
public:
    void write(Formatter<R> &f, const bool &value) {
        if (value) {
            f.write("true");
        } else {
            f.write("false");
        }
    }
};

template<typename R, typename T>
class FormatSpecializer<R, T, typename std::enable_if<
    std::is_integral<T>::value && 
    !std::is_same<T, bool>::value && 
    !std::is_same<T, char>::value &&
    sizeof(T) <= sizeof(std::uint64_t)
>::type> {
public:
    void write(Formatter<R> &f, const T &value) {
        char str[21]; // Either (20 digits + null terminator) or (minus sign + 19 digits + null terminator)
        
        int len;
        if (std::is_signed<T>::value) {
            len = std::snprintf(str, sizeof(str), "%" PRId64, static_cast<std::int64_t>(value));
        } else {
            len = std::snprintf(str, sizeof(str), "%" PRIu64, static_cast<std::uint64_t>(value));
        }

        if (len < 0) {
            f.write("<SNPRINTF ERROR>");
            return;
        }
        
        if (static_cast<std::size_t>(len) > sizeof(str) - 1) {
            len = sizeof(str) - 1;
        }
        
        f.write_multiple_chars(static_cast<const char *>(str), len);
    }
};

template <typename R>
class FormatSpecializer<R, double> {
public:
    void write(Formatter<R> &f, const double &value) {
        char str[512];

#ifdef EFMT_PRECISE_FLOAT
        int precision = std::numeric_limits<double>::max_digits10;
        int len = std::snprintf(str, sizeof(str), "%.*lF", precision, value);
#else
        int len = std::snprintf(str, sizeof(str), "%.5lF", value);
#endif

        if (len < 0) {
            f.write("<SNPRINTF ERROR>");
            return;
        }

        if (static_cast<std::size_t>(len) > sizeof(str) - 1) {
            len = sizeof(str) - 1;
        }

        char *dot = nullptr;
        for (char* p = str; *p; ++p) {
            if (*p == '.') {
                dot = p;
                break;
            }
        }

        if (dot != nullptr) {
            char *end = str + len - 1;
            while (end > dot && *end == '0') {
                *end-- = '\0';
                len--;
            }

            if (*end == '.') {
                *end = '\0';
                len--;
            }
        }

        f.write_multiple_chars(static_cast<const char *>(str), len);
    }
};

template <typename R>
class FormatSpecializer<R, float> {
public:
    void write(Formatter<R> &f, const float &value) {
        f.write(static_cast<double>(value));
    }
};

template <typename R, typename T, std::size_t LEN>
class FormatSpecializer<R, T [LEN], typename std::enable_if<
    !std::is_same<T, char>::value
>::type> {
public:
    void write(Formatter<R> &f, const T (&value)[LEN]) {
        f.write('[');

        bool first = true;
        for (const T &item : value) {
            if (first) {
                first = false;
            } else {
                f.write(", ");
            }

            f.write(item);
        }

        f.write(']');
    }
};

template <typename R, typename T, typename Deleter>
class FormatSpecializer<R, std::unique_ptr<T, Deleter>> {
public:
    void write(Formatter<R> &f, const std::unique_ptr<T, Deleter> &value) {
        if (value) {
            f.write(*value);
        } else {
            f.write("<EMPTY UNIQUE_PTR>");
        }
    }
};

template <typename R, typename T>
class FormatSpecializer<R, std::shared_ptr<T>> {
public:
    void write(Formatter<R> &f, const std::shared_ptr<T> &value) {
        if (value.use_count() != 0) {
            f.write(*value);
        } else {
            f.write("<EMPTY SHARED_PTR>");
        }
    }
};

template <typename R, typename T>
class FormatSpecializer<R, std::weak_ptr<T>> {
public:
    void write(Formatter<R> &f, const std::weak_ptr<T> &value) {
        std::shared_ptr<T> shared = value.lock();
        
        if (shared.use_count() != 0) {
            f.write(std::move(shared));
        } else {
            f.write("<EXPIRED WEAK_PTR>");
        }
    }
};

}
