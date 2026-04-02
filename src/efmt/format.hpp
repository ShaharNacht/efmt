#pragma once

#include "formatter.hpp"
#include "receive.hpp"

namespace efmt {

// A "concept" that represents a type that can be formatted.  
// This should be your default choice for implementing formatting for your types.
class Format {
public:
    // Implement this
    template <typename R>
    void format(Formatter<R> &f) const = delete;
};

// An interface that represents a type that can be formatted through dynamic dispatch.  
// This is useful for implementing formatting for polymorphic types.
// 
// For example, this allows you to have a base class that inherits from `DynFormat`, and have the
// derived classes implement the `format()` method to decide how they want to be formatted.
class DynFormat {
public:
    DynFormat() = default;
    virtual ~DynFormat() = default;

    // Implement this
    virtual void format(Formatter<DynRef> &f) const = 0;
};

}
