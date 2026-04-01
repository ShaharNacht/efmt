#include <utility>
#include <string>

#include "efmt/efmt.hpp"

class Person: public efmt::Format {
private:
    std::string m_name;
    int m_age;

public:
    Person(std::string name, int age) :
        m_name(std::move(name)),
        m_age(age)
    {}

    template <typename R>
    void format(efmt::Formatter<R> &f) const {
        f.write("Person { name: \"", m_name, "\", age: ", m_age, " }");
    }
};

int main() {
    Person person { "Shmulik", 27 };
    
    // Prints: My favourite person is: Person { name: "Shmulik", age: 27 }
    efmt::println("My favourite person is: ", person);

    return 0;
}
