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
// Can hold a reference to any receiver inside it, and formatted text is forwarded to it through dynamic dispatch.
class DynRef: public Receive {
private:
    template <typename R>
    class Impl {
    public:
        static void receive_char(void *receiver, char c) {
            static_cast<R *>(receiver)->receive_char(c);
        };
        
        static void receive_multiple_chars(void *receiver, const char *chars, std::size_t count) {
            static_cast<R *>(receiver)->receive_multiple_chars(chars, count);
        };
    };

    void *m_inner;
    void (*m_receive_char)(void *, char);
    void (*m_receive_multiple_chars)(void *, const char *, std::size_t);

public:
    // Lifetime consideration!  
    // `DynRef` stores a reference to a receiver, and therefore the receiver must outlive this instance of `DynRef`!
    template <typename R>
    DynRef(R &receiver) :
        m_inner(static_cast<void *>(&receiver)),
        m_receive_char(Impl<R>::receive_char),
        m_receive_multiple_chars(Impl<R>::receive_multiple_chars)
    {}

    void receive_char(char c) {
        m_receive_char(m_inner, c);
    }

    void receive_multiple_chars(const char *chars, std::size_t count) {
        m_receive_multiple_chars(m_inner, chars, count);
    }
};

}
