#pragma once

#include <utility>

#include "efmt_format_specializer.hpp"

namespace efmt {

template <typename R>
class Formatter {
private:
    R m_receiver;

public:
    template <typename... Args>
    Formatter(Args &&...args) :
        m_receiver(std::forward<Args>(args)...)
    {}

    template <typename T>
    void write(const T &value) {
        FormatSpecializer<R, T> spec;

        spec.write(*this, value);
    }

    template <typename A, typename B, typename... Rest>
    void write(A &&first, B &&second, Rest &&... rest) {
        write(std::forward<A>(first));
        write(std::forward<B>(second), std::forward<Rest>(rest)...);
    }

    void write_char(char c) {
        m_receiver.receive_char(c);
    }

    void write_multiple_chars(const char *chars, std::size_t count) {
        m_receiver.receive_multiple_chars(chars, count);
    }
};

}
