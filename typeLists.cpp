// Implementation for section 3 "Typelists"


// Linearizing Typelist creation(TODO: we may simplify it more with template parameters 
// package)
#define TYPELIST_1(T1) Typelist<T1, NullType>
#define TYPELIST_2(T1, T2) Typelist<T1, Typelist<T2, NullType>>
#define TYPELIST_3(T1, T2, T3) Typelist<T1, Typelist<T2, Typelist<T3, NullType>>>
#define TYPELIST_4(T1, T2, T3, T4) \
  Typelist<T1, Typelist<T2, Typelist<T3, Typelist<T4, NullType>>>>

struct NullType {};

// Every specialization of Typelist must end with NullType
template <class T, class U>
struct Typelist
{
  typedef T Head;
  typedef U Tail;
};

namespace TL
{
}




// Test cases
int main() {
  Typelist<int, NullType> test1; // two types
  Typelist<int, Typelist<float, NullType>> test2; // two types

  return 0;
}
