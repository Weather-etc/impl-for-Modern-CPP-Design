// Implementation for section 3 "Typelists"


// Linearizing Typelist creation(TODO: we may simplify it more with template parameters 
// package)
#define TYPELIST_1(T1) Typelist<T1, NullType>
#define TYPELIST_2(T1, T2) Typelist<T1, TYPELIST_1(T2)>
#define TYPELIST_3(T1, T2, T3) Typelist<T1, TYPELIST_2(T2, T3)>
#define TYPELIST_4(T1, T2, T3, T4) Typelist<T1, TYPELIST_3(T2, T3, T4)>

struct NullType {};

// Every specialization of Typelist must end with NullType; the benefit is we can use it
// as a border case when we calculate its length
template <class T, class U>
struct Typelist
{
  typedef T Head;
  typedef U Tail;
};

namespace TL
{
  template <class TList> struct Length;
  template <> struct <NullType>
  {
    enum {value = 0};
  };
  template <class T, class U>
  struct Length<Typelist<T, U>>
  {
    enum {value = 1 + Length<U>::value};
  };



}

// Exercise from section 3.4 Calculating Length
namespace Exercise
{
  template <std::type_info*& ele, class TList> struct ArrayHolder;
  template <std::type_info*& ele, class T, class U> 
  struct ArrayHolder<ele, Typelist<T, U>>
  {
  };
}



// Test cases
int main() {
  Typelist<int, NullType> test1; // two types
  Typelist<int, Typelist<float, NullType>> test2; // two types

  return 0;
}
