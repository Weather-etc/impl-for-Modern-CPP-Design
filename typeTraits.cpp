// implementation of section 2.10 Type Traits
#include <vector>
#include <iostream>
#include "isConvertible.h"

struct NullType {};
struct EmptyType {};

// Int2Type
template <int v>
struct Int2Type
{
  enum {value = v};
};

// type select
template <bool flag, typename T, typename U>
struct Select
{
  typedef T Result;
};
template <typename T, typename U>
struct Select<false, T, U>
{
  typedef U Result;
};

template <typename T>
class TypeTraits 
{
private:
  // test pointer type
  // We use partial template specialization to strip qualifiers or pointer/reference
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

  // test volatile qualifier
  template <class U> struct VolatileTraits
  {
    enum {result = false};
    typedef U NonVolatileType;
  };
  template <class U> struct VolatileTraits<volatile U>
  {
    enum {result = true};
    typedef U NonVolatileType;
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

  // type traits using typelists, skip details for now
  typedef TYPELIST_4(
      unsigned char, unsigned short int, unsigned int, unsigned long int)
    UnsignedInts;
  typedef TYPELIST_4(
      signed char, short int, int, long int) SignedInts;
  typedef TYPELIST_3(bool, char, wchar_t) OtherInts;
  typedef TYPELIST_3(float, double, long double) Floats;

  enum {isStdUnsignedInt = TL::IndexOf<UnsignedInts, T>::value >= 0};
  enum {isStdSignedInt = TL::IndexOf<SignedInts, T>::value >= 0};
  enum {isStdIntegral = isStdUnsignedInt || isStdSignedInt ||
    TL::IndexOf<Others, T>::value >= 0};
  enum {isStdFloat = TL::IndexOf<Floats, T>::value >= 0};
  enum {isStdArith = isStdIntegral || isStdFloat};
  enum {isStdFundamental = isStdArith || Conversion<T, void>::sameType};

  typedef typename PointerTraits<T>::PointeeType PointeeType;
  typedef typename ReferTraits<T>::ReferType ReferencedType;
  typedef typename ConstTraits<T>::NonConstType NonConstType;
  typedef typename Select<isStdArith || isPointerType || isMemberPointer, 
          T, ReferencedType&>::Result;
};


// A patch for POD(plain old data)
template <typename T>
struct SupportBitwiseCopy
{
  enum {result = false};
};
template <>
struct SupportBitwiseCopy<POD>
{
  enum {result = true};
};

enum CopyAlgoSelector {Conservative, Fase};

// Conservative routine works for any type. So we use it as primary template
template <typename InIt, typename OutIt>
OutIt CopyImpl(InIt first, InIt last, OutIt result, Int2Type<Conservative>)
{
  for (; first != last; ++first, ++result)
    // If *result is uninitialized, then copy constructor is called, otherwise 
    // assignment operator will be used.
    *result = *first;   
  return result;
}
// Fast routine works only for pointers to raw data
template <typename InIt, typename OutIt>
OutIt CopyImpl (InIt first, InIt last, OutIt result, Int2Type<Fast>)
{
  const size_t n = last - first;
  // another impl for copy
  return result + n;
}
template <typename InIt, typename OutIt>
OutIt Copy(InIt first, InIt last, OutIt result)
{
  typedef typename TypeTraits<InIt>::PointeeType SrcPointee;
  typedef typename TypeTraits<OutIt>::PointeeType DestPointee;
  // Constrains to use fast routine: same type and bitwise copyable.
  enum {copyAlgo = 
    TypeTraits<InIt>::isPointer &&
    TypeTraits<OutIt>::isPointer &&
    // Patch for user-define type.
    SupportsBitwiseCopy<SrcPointee>::result &&
    SupportBitwiseCopy<DestPointee>::result &&
    Conversion<SrcPointee, DestPointee>::sameType || (
      TypeTraits<SrcPointee>::isStdFundamental &&
      TypeTraits<DestPointee>::isStdFundamental &&
      Typetraits<SrcPointee>::isStdFloat == TypeTraits<DestPointee>::isStdFloat &&
      sizeof(SrcPointee) == sizeof(DestPointee)) ? Fast : Conservative};
  return CopyImpl(first, last, result, Int2Type<copyAlgo>());
}



// test code
struct POD
{
  int a;
};

int main() {
  const bool iterIsPtr = TypeTraits<std::vector<int>::iterator>::isPointer;
  if (iterIsPtr) 
    std::cout << "iter is ptr\n";
  else
    std::cout << "iter is not ptr\n";

  return 0;
}
