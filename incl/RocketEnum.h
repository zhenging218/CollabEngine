#ifndef RENUM_H
#define RENUM_H

#include <iostream>
#include <string>
#include <vector>
#include <utility>
#include <sstream>
#include <algorithm>
#include <iterator>

#define CONCAT(x,y) x##y

template <char delim = '\n'>
struct Line {
  Line(std::string const & str = std::string{}) : line{ str } {}
  operator std::string() const { return line; }

  friend std::ostream &operator<<(std::ostream &os, Line const &rhs) {
    os << rhs << delim;
    return os;
  }

  friend std::istream &operator>>(std::istream &is, Line &rhs) {
    std::getline(is, rhs.line, delim);
    return is;
  }

private:
  std::string line;
};

template <typename T>
struct RocketEnumeration {
  static std::vector<std::pair<int, std::string>> gettable() {
    return T::returntable();
  }
  virtual operator int() const = 0;
  virtual operator int&() = 0;
  virtual bool operator==(int rhs) const = 0;
  virtual bool operator!=(int rhs) const = 0;
  virtual bool operator==(T const &rhs) const = 0;
  virtual bool operator!=(T const &rhs) const = 0;
  virtual ~RocketEnumeration() {}
};

#define RocketEnum(type,...)                                                   \
struct type : RocketEnumeration<type> {                                        \
                                                                               \
  enum CONCAT(type,_values) : int {__VA_ARGS__};                               \
                                                                               \
  type(int v = table.cbegin()->first) : value{v} {}                            \
  type &operator=(int rhs) {                                                   \
    value = rhs;                                                               \
	return *this;                                                              \
  }                                                                            \
                                                                               \
  bool operator==(type const &rhs) const { return value == rhs.value; }        \
  bool operator==(int rhs) const { return value == rhs; }                      \
                                                                               \
  bool operator!=(type const &rhs) const { return !operator==(rhs); }          \
  bool operator!=(int rhs) const { return !operator==(rhs); }                  \
                                                                               \
  operator int() const { return value; }                                       \
  operator int&() { return value; }                                            \
                                                                               \
  using type_element = std::pair<int,std::string>;                             \
  using type_table = std::vector<type_element>;                                \
                                                                               \
  private:                                                                     \
                                                                               \
  static type_table gentable() {                                               \
    int val = 0;                                                               \
    std::string word, index, str{#__VA_ARGS__};                                \
    std::replace(str.begin(),str.end(), '=', ' ');                             \
    std::stringstream ss(str);                                                 \
    using iterator = std::istream_iterator<Line<','>>;                         \
    std::vector<std::string> tokenised{iterator(ss),iterator()};               \
    type_table ret;                                                            \
    for(auto i = tokenised.begin(); i != tokenised.end(); ++i) {               \
      std::stringstream ss2{*i};                                               \
      ss2 >> word >> index;                                                    \
      val = (index.size() > 0) ? atoi(index.c_str()) : val;                    \
      ret.push_back(std::make_pair(val++,word));                               \
      index.clear();                                                           \
    }                                                                          \
    return ret;                                                                \
  }                                                                            \
                                                                               \
  static std::string genElementList(type_table const &rhs) {                   \
    std::stringstream ss;                                                      \
    ss << "{ ";                                                                \
    for(size_t i = 0; i < rhs.size(); ++i) {                                   \
      ss << rhs[i].second << " = " << rhs[i].first                             \
      << ((i+1 >= rhs.size()) ? " " : ", ");                                   \
    }                                                                          \
    ss << '}';                                                                 \
    return ss.str();                                                           \
  }                                                                            \
                                                                               \
  static const type_table table;                                               \
  static const std::string CONCAT(type,Elements);                              \
  int value;                                                                   \
                                                                               \
  public:                                                                      \
                                                                               \
  static std::string ToString(int rhs) {                                       \
    auto ret = std::find_if(table.cbegin(),table.cend(),                       \
    [&rhs](type_element const &i){                                             \
      return i.first == rhs;                                                   \
    });                                                                        \
    return (ret == table.cend()) ? std::to_string(rhs) : (*ret).second;        \
  }                                                                            \
                                                                               \
  static type FromString(std::string const &rhs) {                             \
    auto ret = std::find_if(table.cbegin(),table.cend(),                       \
    [&rhs](type_element const &i){                                             \
      return i.second == rhs;                                                  \
    });                                                                        \
    return (ret == table.cend()) ? atoi(rhs.c_str()) : (*ret).first;           \
  }                                                                            \
                                                                               \
  static std::string GetElementList() { return type::CONCAT(type,Elements); }  \
                                                                               \
  friend bool operator==(type const &lhs, type const &rhs) {                   \
    return lhs.value == rhs.value;                                             \
  }                                                                            \
                                                                               \
  static type_table returntable() { return type::table; }                      \
};                                                                             \
                                                                               \
const type::type_table type::table = gentable();                               \
const std::string type::CONCAT(type,Elements) = genElementList(table);         \
                                                                               \
std::ostream &operator<<(std::ostream &os, type const &rhs) {                  \
  os <<  type::ToString(rhs);                                                  \
  return os;                                                                   \
}                                                                              \
                                                                               \
std::istream &operator>>(std::istream &is, type &rhs) {                        \
  std::string temp;                                                            \
  is >> temp;                                                                  \
  rhs = type{type::FromString(temp)};                                          \
  return is;                                                                   \
}

#endif