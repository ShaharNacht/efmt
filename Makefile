bin := efmt-example

src := $(wildcard src/*.cpp)

flags := -std=c++11 -pedantic -Wall -Wextra -Wconversion -Wswitch-enum

.PHONY: example
example:
	g++ $(src) -o $(bin) $(flags)
