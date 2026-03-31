#include <string>
#include <iostream>

#include "efmt/efmt.hpp"

struct Dog: public efmt::Format {
    int age;

    Dog(int age) : age(age) {}

    template <typename R>
    void format(efmt::Formatter<R> &f) const {
        f.write("Dog { age: ", age, " }");
    }
};

enum class Color {
    ORANGE,
    GRAY
};

namespace efmt {
    template <typename R>
    class FormatSpecializer<R, Color> {
    public:
        void write(Formatter<R> &f, const Color &value) {
            switch (value) {
                case Color::ORANGE:
                    f.write("ORANGE");
                break;
                
                case Color::GRAY:
                    f.write("GRAY");
                break;
            }
        }
    };
}

int main() {
    efmt::Formatter<efmt::Print> a;

    char const *meap = " meap ";
    a.write('A', " moop ", 'C', meap, 'D', '\n');

    char abc[] = { 'A', 'B', 'C' };
    const char nums[] = { '1', '2', '3', '4' };

    a.write("LOL", '\n');

    a.write(abc, '\n');

    a.write(nums, '\n');

    std::string s = "MOOOOOOOOP";
    a.write(s, '\n');

    a.write(true, " ", false, ' ', "lol", '\n');

    a.write(57, '\n');

    a.write(Dog(-84), '\n');

    a.write(5.7f, '\n');
    a.write(16.8651651, '\n');
    a.write(1.0 / 3, '\n');
    a.write(7.0, '\n');
    a.write(7.0f, '\n');
    a.write(7.125f, '\n');
    a.write(std::numeric_limits<double>::infinity(), '\n');
    a.write(-std::numeric_limits<double>::infinity(), '\n');
    a.write(std::numeric_limits<double>::quiet_NaN(), '\n');
    a.write(std::numeric_limits<double>::signaling_NaN(), '\n');

    const char * lololololol[] = { "ava", "zimyeru", "kimm", nullptr, "77" };
    a.write(lololololol, '\n');

    Dog dogs[] = { Dog(5), Dog(6), Dog(5) };
    a.write(dogs, '\n');

    a.write(nullptr, '\n');

    int iii[3][3] = { { 10, 11, 12 }, { 20, 21, 22 }, { 30, 31, 32 } };
    a.write(iii, '\n');

    return 0;
}
