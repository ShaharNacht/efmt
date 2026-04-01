#pragma once

#include <utility>
#include <iostream>

#include "formatter.hpp"
#include "receive.hpp"

namespace efmt {

// Print formatted arguments to `std::cout`. (No added linebreak at the end)
template <typename... Args>
void print(Args &&... args) {
    Formatter<Print> f;

    f.write(std::forward<Args>(args)...);
}

// Print formatted arguments + linebreak to `std::cout`.
template <typename... Args>
void println(Args &&... args) {
    print(std::forward<Args>(args)..., '\n');
}

// Print formatted arguments to `std::cerr`. (No added linebreak at the end)
template <typename... Args>
void eprint(Args &&... args) {
    Formatter<Print> f { std::cerr };

    f.write(std::forward<Args>(args)...);
}

// Print formatted arguments + linebreak to `std::cerr`.
template <typename... Args>
void eprintln(Args &&... args) {
    eprint(std::forward<Args>(args)..., '\n');
}

}
