#pragma once

#include "formatter.hpp"

namespace efmt {

// A "concept" that represents a type that can be formatted.
class Format {
public:
    // Implement this
    template <typename R>
    void format(Formatter<R> &f) const = delete;
};

}
