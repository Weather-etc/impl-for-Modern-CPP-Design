// implementation of section 2.10 Type Traits
#include <vector>
#include <iostream>

struct NullType {};
struct EmptyType {};

template <typename T>
class TypeTraits 
{
private:
  // test pointer type
  template <class U> struct PointerTraits
  {
    enum {result = false};
    typedef NullType PointeeType;
  };
  template <class U> struct PointerTraits<U*>
  {
    enum {result = true};
    typedef U PointeeType;
  };

  // test referenced type
  template <class U> struct ReferTraits
  {
    enum {result = false};
    typedef U ReferType;
  };
  template <class U> struct ReferTraits<U&>
  {
    enum {result = true};
    typedef U ReferType;
  };

  // test const type
  template <class U> struct ConstTraits
  {
    enum {result = false};
    typedef U NonConstType;
  };
  template <class U> struct ConstTraits<const U>
  {
    enum {resutl = true};
    typedef U NonConstType;
  };

  // test class member pointer
  template <class U> struct PToMTraits
  {
    enum {result = false};
  };
  template <class U, class V> struct PToMTraits
  {
    enum {result = true};
  };
public:
  enum {isPointerType = PointerTraits::result};
  enum {isReferencedType = ReferTraits::result};
  enum {isMemberPointer = PToMTraits::result};
  enum {isConstType = ConstTraits::result};

  typedef typename PointerTraits<T>::PointeeType PointeeType;
  typedef typename ReferTraits<T>::ReferType ReferencedType;
  typedef typename ConstTraits<T>::NonConstType NonConstType;

  // type traits using typelists, skip details for now
  typedef TYPELIST_4(
      unsigned char, unsigned short int, unsigned int, unsigned long int)
    UnsignedInts;
  typedef TYPELIST_4(
      signed char, short int, int, long int)
    SignedInts;
  typedef TYPELIST_3(bool, char, wchar_t) OtherInts;
  typedef TYPELIST_3(float, double, long double) Floats;

  enum {isStdUnsignedInt = TL::IndexOf<UnsignedInts, T>::value >= 0};
  enum {isStdIntegral = isStdUnsignedInt || isStdSignedInt ||
    TL::IndexOf<Others, T>::value >= 0};
  enum {isStdFloat = TL::IndexOf<Floats, T>::value >= 0};
  enum {isStdArith = isStdIntegral || isStdFloat};
  enum {isStdFundamental = isStdArith || Conversion<T, void>::sameType};
};



// test code
int main() {
  const bool iterIsPtr = TypeTraits<std::vector<int>::iterator>::isPointer;
  if (iterIsPtr) 
    std::cout << "iter is ptr\n";
  else
    std::cout << "iter is not ptr\n";

  return 0;
}
