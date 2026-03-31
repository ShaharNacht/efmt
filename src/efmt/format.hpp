#pragma once

#include "formatter.hpp"

namespace efmt {

class Format {
public:
    // Implement this
    template <typename R>
    void format(Formatter<R> &f) const = delete;
};

}
