// #include "RocketDebug.h"
#include <iostream>
#include <string>

#include <functional>

void foo(std::string const &rhs) {
	std::cout << "foo called with " << rhs << std::endl;
}

struct boo {
	void bar(std::string const &rhs) {
		std::cout << "boo::bar called with " << rhs << std::endl;
	}
};

struct goo {
	
	template <typename T, typename ... Args>
	using bind_type = decltype(std::bind(std::declval<T>(), ));
	
	template <typename T, typename ... Args>
	static auto get_func(decltype(std::bind(std::declval<T>(), std::declval<Args>()...)) fn)->std::function<T>{
		return std::function<T>{fn};
	}
};

int main() {
	boo b;
	auto i = goo::get_func(std::bind(&boo::bar, &b, std::string{}));
	// i(std::string{"hello"});
	return 0;
}
