#ifndef RDEBUG
#define RDEBUG

#include <iostream>
#include <sstream>
#include <utility>
#include <type_traits>
#include <typeinfo>
#include <list>

#include "same.h"

class RocketDebug {

	RocketDebug(RocketDebug const &) = delete;
	RocketDebug &operator=(RocketDebug const &) = delete;
	
	std::list<std::ostream *> out;
	
  template <typename T>
	class CanOStream {
		template <typename U>
		static auto check(int)->decltype(std::declval<std::ostream&>()
		<< std::declval<U const &>(), yes());
		template <typename U>
		static no check(...);
		public:
		using type = decltype(check<T>(0));
	};
	
	template <typename T>
	struct Stringify {
		template <typename U>
		static std::string stringify_impl(T const &rhs, U) {
			std::string ret = typeid(rhs).name();
			return ret;
		}
		
		static std::string stringify_impl(T const &rhs, yes) {
			std::string ret;
			std::stringstream ss;
			ss << rhs;
			ret = ss.str();
			return ret;
		}
		
		static std::string stringify_impl(bool const &rhs, yes) {
			return std::string{rhs ? "true" : "false"};
		}
		
		std::string operator()(T const &rhs) const {
			using type = typename CanOStream<T>::type;
			return stringify_impl(rhs, type());
		}
	};
	
	template <typename T>
	void lshift_op_impl(T const &rhs) {
		for(std::ostream * os : out) {
			*os << Stringify<T>()(rhs);
		}
	}
  
	public:
	
	RocketDebug() : out{&std::cout} {}
	
	template <typename T, typename = decltype((std::declval<T&>() << ""),yes())>
	RocketDebug &operator+=(T &os) {
		out.push_back(&os);
		return *this;
	}
	
	template <typename T, typename = decltype((std::declval<T&>() << ""),yes())>
	RocketDebug &operator-=(T &os) {
		auto result = out.begin();
		while(result != out.end() && *result != &os) ++result;
		out.erase(result);
		return *this;
	}
	
	template <typename T>
	friend RocketDebug &operator<<(RocketDebug &os, T const &rhs) {
		os.lshift_op_impl(rhs);
		return os;
	}
  
  friend RocketDebug &operator<<(RocketDebug &os, std::ostream &(*)(std::ostream&)) {
    os.lshift_op_impl("\n");
    return os;
  }
};

static RocketDebug Debug{};

#endif