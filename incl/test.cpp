#include "RocketDebug.h"
#include <iostream>
#include <vector>
#include <string>


struct foo {
	std::vector<std::string> stuff;
};

foo &operator<<(foo &f, std::string const &rhs) {
	if(f.stuff.empty() || f.stuff.back().back() == '\n') {
		f.stuff.push_back(rhs);
	} else {
		f.stuff[f.stuff.size() - 1] += rhs;
	}
	return f;
}

int main() {
	foo f;
	Debug += f;
	Debug << "hello world" << std::endl;
	Debug << "aaa" << std::endl;
	for(auto const &i : f.stuff) {
		std::cout << i << std::endl;
	}
	Debug -= f;
	Debug << "hello again" << std::endl;
	for(auto const &i : f.stuff) {
		std::cout << i << std::endl;
	}
}