#include <utility>
#include <memory>
#include <vector>
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

class Animal: public efmt::DynFormat {
public:
    Animal() = default;
    virtual ~Animal() = default;
};

class Dog: public Animal {
public:
    virtual void format(efmt::Formatter<efmt::DynRef> &f) const override {
        f.write("Dog");
    }
};

class Cat: public Animal {
public:
    virtual void format(efmt::Formatter<efmt::DynRef> &f) const override {
        f.write("Cat");
    }
};

int main() {
    efmt::Formatter<efmt::Print> f;
    efmt::Formatter<efmt::DynRef> dyn = f.as_dyn_ref();

    std::vector<std::unique_ptr<Animal>> v;
    v.emplace_back(new Dog());
    v.emplace_back(new Cat());
    v.emplace_back(new Dog());

    dyn.writeln(*v[0]);
    dyn.writeln(*v[1]);
    dyn.writeln(*v[2]);

    Person person { "Shmulik", 27 };
    efmt::println("My favourite person is: ", person);

    dyn.writeln(Person("abc", 123), " --- ", Person("def", 456));

    return 0;
}
