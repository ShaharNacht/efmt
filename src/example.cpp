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
    // efmt::println(Dog());
    // efmt::println(Cat());

    // Dog d {};
    // Animal &a = d;
    // efmt::println(d);
    // efmt::println(a);

    // std::vector<std::unique_ptr<Animal>> v;
    // v.emplace_back(new Dog());
    // v.emplace_back(new Cat());
    // v.emplace_back(new Dog());
    // v.emplace_back();

    // efmt::println(v[0]);
    // efmt::println(v[1]);
    // efmt::println(v[2]);
    // efmt::println(v[3]);

    std::weak_ptr<Animal> outer;
    efmt::println("outer: ", outer);

    {
        std::shared_ptr<Animal> cat = std::make_shared<Cat>();
        std::shared_ptr<Animal> cat2 = cat;
        std::weak_ptr<Animal> cat3 = cat;

        efmt::println("cat: ", cat);
        efmt::println("cat2: ", cat2);
        efmt::println("cat3: ", cat3);

        outer = cat2;
        efmt::println("outer: ", outer);
    }

    efmt::println("outer: ", outer);

    return 0;
}
