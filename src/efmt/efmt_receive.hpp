#pragma once

#include <cstddef>
#include <iostream>

namespace efmt {

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

}
