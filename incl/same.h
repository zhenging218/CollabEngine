#ifndef SAMECHECK_H
#define SAMECHECK_H

struct yes { 
	char c;
};
struct no { 
	char c[2]; 
};

template <bool ... empty>
struct all_same {
	static constexpr bool value = true;
};

template <bool first, bool ... rest>
struct all_same<first, rest...> : public all_same<rest...> {
};

template <bool ... rest>
struct all_same<false, rest...> {
	static constexpr bool value = false;
};

template <bool ... empty>
struct at_least_one_same {
	static constexpr bool value = false;
};

template <bool first, bool ... rest>
struct at_least_one_same<first, rest...> : public at_least_one_same<rest...> {
	
};

#endif