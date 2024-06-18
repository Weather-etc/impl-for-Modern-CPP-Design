// Implements section 2.7: Detecting Convertibility and Inheritance at Compile Time
//
// Use concepts and constrains(since c++20) and static_cast to prevent compiler 
// report a compile-time error when trying to use isSuperClass(base, inherit1) 
// in test code
//
// GNU ISO C++ uses compiler intrinsic '__is_base_of()':
//    https://gcc.gnu.org/onlinedocs/libstdc++/latest-doxygen/a00227_source.html

#include <iostream>

template <typename T, typename U>
concept convertible = requires(T t, U u){
  u = t;
  // static_cast<U>(t);   ERROR! static_cast allows any implicit conversion and 
  //    its reverse operation
};

template <typename T, typename U>
struct isConvertible
{
public:
  static const bool convertibility = 0;
  static const bool identity = 0;
};

// Use template speciliazation. Because this version is more restrict than 
// primary version, compiler will accept it.
template <typename T, typename U> requires convertible<T, U>
struct isConvertible <T, U>
{
public:
  static const bool convertibility = 1;
  static const bool identity = 0;
};

template <typename T>
struct isConvertible<T, T>
{
  static const bool convertibility = 1;
  static const bool identity = 1;
};

// Cases when const U* can implicitly convert to T*:
// 1. T and U have the same type
// 2. T is an unambiguous public base of U(This kind of ambiguity often happens 
//    in diamond inheritance, refer: 
//    [IBM](https://www.ibm.com/docs/en/zos/2.4.0?topic=only-ambiguous-base-classes-c)
//    for more about ambiguity in class inheritance)
// 3. T is void
//
// duplicate 'const' will be ignored in template deduction
#define isSuperClass(T, U) \
  (::isConvertible<const U*, const T*>::convertibility && \
   !::isConvertible<const T*, const void*>::identity)
#define isSuperClassStrictly(T, U) \
  (::isSuperClass(T, U) && \
   !::isConvertible<const U*, const T*>::identity)



// Test code
class base{};
class inherit1 : private base {};
class inherit2 : public base {};

int main() {
  if (isSuperClass(float, int))
    std::cout << "is\n";
  else
    std::cout << "not\n";

  if (isSuperClass(base, inherit1))
    std::cout << "is\n";
  else
    std::cout << "not\n";
  
  if (isSuperClass(base, inherit2))
    std::cout << "is\n";
  else
    std::cout << "not\n";

  if (isSuperClass(inherit2, base))
    std::cout << "is\n";
  else
    std::cout << "not\n";

  if (isSuperClass(void, int))
    std::cout << "is\n";
  else
    std::cout << "not\n";
  return 0;
}

