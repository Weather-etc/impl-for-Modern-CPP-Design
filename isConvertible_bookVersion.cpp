// Implements chapter 2.7: Detecting Convertibility and Inheritance at Compile Time

#include <iostream>

// isConvertible decide if type T can be converted to type U 
template <typename T, typename U>
struct isConvertible 
{
private:
  typedef char small;
  class big {char dummy[2];}; // local class. Mentioned in 2.3 local classes

  // Use makeT to generate compile-time T type. This patchs for the situation when
  // constructor of T is not public.
  static T makeT();

  // Overload function test() to help decide if T can be converted to U
  static small test(const U&);
  static big test(...);

public:
  // convertibility is true when T is convertible to U
  static constexpr bool convertibility = sizeof(test(makeT())) == sizeof(small);

  // identity is true when T and U are exactly the same type.
  // This is realized by template partial specification
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
  if (isSuperClass(void, int))
    std::cout << "is\n";
  else
    std::cout << "not\n";

  // This will cause a compile-time error
  /*
  if (isSuperClass(base, inherit1))
    std::cout << "is\n";
  else
    std::cout << "not\n";
    */

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

