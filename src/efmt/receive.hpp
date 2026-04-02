#pragma once

#include <cstddef>
#include <utility>
#include <memory>
#include <iostream>

namespace efmt {

// A "concept" that represents a sink for formatted text.
class Receive {
public:
    // Implement this
    void receive_char(char c) = delete;

    // Implement this (for performance)  
    // Do NOT assume that `chars` is null-terminated!  
    // 
    // If you don't have a meaningful way to optimize receiving multiple chars, you can implement it as:  
    // ```
    // for (std::size_t i = 0; i < count; i++) {  
    //     receive_char(chars[i]);  
    // }
    // ```
    void receive_multiple_chars(const char *chars, std::size_t count) = delete;
};

// Provided receivers: (Though you can also implement your own)

class Print: public Receive {
private:
    std::ostream &m_output;

public:
    // Lifetime consideration!  
    // `Print` stores a reference to an `std::ostream`, and therefore the `std::ostream` must outlive this instance of `Print`!  
    // (This of course isn't an issue if the stream you use is either `std::cout` or `std::cerr`, since they are global variables)
    Print(std::ostream &output = std::cout) :
        m_output(output)
    {}

    void receive_char(char c) {
        m_output.put(c);
    }

    void receive_multiple_chars(const char *chars, std::size_t count) {
        m_output.write(chars, count);
    }
};

// Type-erased receiver adapter.  
// Can hold any receiver inside it, and formatted text is forwarded to it through dynamic dispatch.
// 
// Construct it using `Dyn::make()`, rather than the constructor.
class Dyn: public Receive {
private:
    class Interface {
    public:
        Interface() = default;
        virtual ~Interface() = default;

        virtual void receive_char(char c) = 0;

        virtual void receive_multiple_chars(const char *chars, std::size_t count) = 0;
    };

    template <typename R>
    class Inner: public Interface {
    public:
        R receiver;

        template <typename... Args>
        Inner(Args &&... args) :
            receiver(std::forward<Args>(args)...)
        {}

        virtual void receive_char(char c) override {
            receiver.receive_char(c);
        }

        virtual void receive_multiple_chars(const char *chars, std::size_t count) override {
            receiver.receive_multiple_chars(chars, count);
        }
    };

    template <typename R>
    class TypeSelector {};

    std::unique_ptr<Interface> m_inner;

    template <typename R, typename... Args>
    Dyn(TypeSelector<R>, Args &&... args) :
        m_inner(
            std::unique_ptr<Inner<R>> {
                new Inner<R> {
                    std::forward<Args>(args)...
                }
            }
        )
    {}

public:
    Dyn(Dyn &&) = default;
    Dyn &operator =(Dyn &&) = default;

    // `R` is the concrete receiver type, `args` are the arguments to construct `R`.
    template <typename R, typename... Args>
    static Dyn make(Args &&... args) {
        return Dyn { TypeSelector<R> {}, std::forward<Args>(args)... };
    }

    void receive_char(char c) {
        m_inner->receive_char(c);
    }

    void receive_multiple_chars(const char *chars, std::size_t count) {
        m_inner->receive_multiple_chars(chars, count);
    }
};

}
