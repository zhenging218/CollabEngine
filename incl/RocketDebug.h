#ifndef RDEBUG
#define RDEBUG

#include <iostream>
#include <sstream>
#include <utility>
#include <type_traits>
#include <typeinfo>
#include <list>
#include <memory>
#include <algorithm>

#include "same.h"

class RocketDebug {
	
	struct bool_yes { char c[3]; };
	
	struct s_base {
		virtual void stream(std::string const &) = 0;
		virtual s_base *Clone() const = 0;
		virtual ~s_base() {}
		template <typename T>
		T *Get() {
			streamable<T> *ret = dynamic_cast<streamable<T>*>(this);
			return (ret == nullptr) ? nullptr : ret->target;
		}
		
		template <typename T>
		bool operator==(T const &rhs) {
			T *target = Get<T>();
			return (target == nullptr) ? false : (target == const_cast<T*>(&rhs));
		}
		
		template <typename T>
		bool operator!=(T const &rhs) {
			return !this->operator==(rhs);
		}
	};
	
	template <typename T>
	struct streamable : s_base {
		T *target;
		streamable(T &rhs) : target(&rhs) {}
		streamable *Clone() const { return new streamable{*this}; }
		void stream(std::string const &rhs) override {
			(*target) << rhs.c_str();
		}
	};
	
	RocketDebug(RocketDebug const &) = delete;
	RocketDebug &operator=(RocketDebug const &) = delete;
	
	std::list<std::shared_ptr<s_base>> out;
	
	template <typename T>
	class CanOStream {
		template <typename U>
		static auto check(int)->decltype(std::declval<std::ostream&>()
		<< std::declval<U const &>(), typename std::enable_if<!std::is_same<bool, U>::value>::type(), yes());
		template <typename U>
		static auto check(char)->decltype((std::declval<std::ostream&>()
		<< std::declval<U const &>()), typename std::enable_if<std::is_same<bool, U>::value>::type(), bool_yes());
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
		
		static std::string stringify_impl(bool const &rhs, bool_yes) {
			return std::string{rhs ? "true" : "false"};
		}
		
		std::string operator()(T const &rhs) const {
			using type = typename CanOStream<T>::type;
			return stringify_impl(rhs, type());
		}
	};
	
	template <typename T>
	void lshift_op_impl(T const &rhs) {
		for(std::shared_ptr<s_base> const &os : out) {
			os->stream(Stringify<T>()(rhs));
		}
	}
  
	public:
	
	RocketDebug() : out{std::make_shared<streamable<std::ostream>>(streamable<std::ostream>{std::cout})} {}
	
	template <typename T, typename = decltype((std::declval<T&>() << ""),yes())>
	RocketDebug &operator+=(T &os) {
		static auto comp = [&](std::shared_ptr<s_base> const &rhs)->bool{return (*rhs) == os; };
		if(std::find_if(out.begin(), out.end(), comp) == out.end()) {
			out.push_back(std::make_shared<streamable<typename std::remove_reference<T>::type>>(os));
		}
		return *this;
	}
	
	template <typename T, typename = decltype((std::declval<T&>() << ""),yes())>
	RocketDebug &operator-=(T &os) {
		static auto comp = [&](std::shared_ptr<s_base> const &rhs)->bool{return (*rhs) == os; };
		auto result = std::find_if(out.begin(), out.end(), comp);
		if(result != out.end()) out.erase(result);
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