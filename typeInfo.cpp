// implementation for section 2.9 A Wrapper Around type_info

#include <iostream>
#include <set>
#include <vector>
#include <typeinfo>

class TypeInfo {
public:
  TypeInfo() = default;
  template <class T> TypeInfo(T t) {pInfo_ = &typeid(t);}
  TypeInfo(const std::type_info& info) {pInfo_ = &info;}

  // We can access private fields of 'info' because access control works on class
  // level, and not on object level.
  // (ref: https://stackoverflow.com/questions/4117002/why-can-i-access-private-variables-in-the-copy-constructor)
  TypeInfo(const TypeInfo& info) {pInfo_ = info.pInfo_;}

  // compatibility with usage of std::type_info
  const char* name() const {return pInfo_->name();}
  bool before(const std::type_info& base) const {return pInfo_->before(base);}

  // overload operator
  friend bool operator== (const TypeInfo&, const TypeInfo&);
  friend bool operator<= (const TypeInfo&, const TypeInfo&);
  friend bool operator< (const TypeInfo&, const TypeInfo&);
  friend bool operator>= (const TypeInfo&, const TypeInfo&);
  friend bool operator> (const TypeInfo&, const TypeInfo&);
  friend bool operator!= (const TypeInfo&, const TypeInfo&);

private:
  const std::type_info *pInfo_;  // std::type_info has static storage so can be referred by pointer.

};

bool operator== (const TypeInfo& lhs, const TypeInfo& rhs) {
  return lhs.pInfo_ == rhs.pInfo_;
}

bool operator!= (const TypeInfo& lhs, const TypeInfo& rhs) {
  return lhs.pInfo_ != rhs.pInfo_;
}

bool operator< (const TypeInfo& lhs, const TypeInfo& rhs) {
  return lhs.pInfo_->before(*rhs.pInfo_);
}

bool operator<= (const TypeInfo& lhs, const TypeInfo& rhs) {
  return lhs.pInfo_->before(*rhs.pInfo_) || lhs.pInfo_->before(*rhs.pInfo_);
}

bool operator> (const TypeInfo& lhs, const TypeInfo& rhs) {
  return rhs.pInfo_->before(*lhs.pInfo_);
}

bool operator>= (const TypeInfo& lhs, const TypeInfo& rhs) {
  return rhs.pInfo_->before(*lhs.pInfo_) || rhs.pInfo_ == lhs.pInfo_;
}




// test code
class A {};

int main() {
  TypeInfo a, a1(0);
  TypeInfo b('c');

  std::set<TypeInfo> container{b};
  // vec.emplace(a); // ERROR! a.pInfo_ is a wild pointer
  container.emplace(a1);

  return 0;
}
