#include <concepts>
#include <type_traits>

template <int ... Ints>
struct IntList{};

static_assert(std::is_same<IntList<1,2>, IntList<1,2>>::value);

template <class V, int Number>
struct PushBack
{};

template<int ... Nums, int Number>
struct PushBack<IntList<Nums...>, Number>
{
    using type =  IntList<Nums..., Number>;
};

static_assert(std::is_same<IntList<1,2>, PushBack<IntList<1>, 2>::type>::value);

template <class V, int Number>
struct PushFront
{};

template<int ... Nums, int Number>
struct PushFront<IntList<Nums...>, Number>
{
    using type =  IntList<Number, Nums...>;
};

static_assert(std::is_same<IntList<1,2>, PushFront<IntList<2>, 1>::type>::value);

template <class V>
struct Size
{};

template <int Head, int ... Tail>
struct Size<IntList<Head, Tail...>>
{
    constexpr static int value = 1 + Size<IntList<Tail...>>::value;
};

template <>
struct Size<IntList<>>
{
    constexpr static int value = 0;
};

static_assert(Size<IntList<>>::value == 0);
static_assert(Size<IntList<1,2,3>>::value == 3);

template <class V>
struct Index
{};

template <int ...Nums>
struct Index<IntList<Nums...>>
{
    static constexpr int value = Size<IntList<Nums...>>::value;
};

template <typename V, int Position>
struct Get;

template <int Position, int Index, int ... Ints>
struct GetImpl;

template <int Position, int Index, int Head, int ... Tail>
struct GetImpl<Position, Index, Head, Tail...>
{
    constexpr static int index = Index;
    constexpr static int value = (Index == Position) ? Head : GetImpl<Position, Index+1, Tail...>::value;

};

template <int Position, int Index, int Last>
struct GetImpl<Position, Index, Last>
{
    constexpr static int index = Index;
    constexpr static int value = Last;

};

template<int ...Ints, int Position>
struct Get<IntList<Ints...>, Position>
{
    constexpr static int index = GetImpl<Position, 0, Ints...>::index;
    constexpr static int value = GetImpl<Position, 0, Ints...>::value;
};

static_assert(Get<IntList<4,6,8,2>, 1>::value == 6);


using values = IntList<1,2,3,4,5>;

template <int ... Ints>
struct SumImpl;

template <int Head, int ... Tail>
struct SumImpl<Head, Tail...>
{
    constexpr static int value = Head + SumImpl<Tail...>::value;
};

template <>
struct SumImpl<>
{
    constexpr static int value = 0;
};

template <class L>
struct Sum{};

template <int ... Ints>
struct Sum<IntList<Ints...>>
{
    constexpr static int value = SumImpl<Ints...>::value;
};

static_assert(Sum<values>::value == 15);

template <int ... Ints>
struct MinImpl;

template <int Head, int ... Tail>
struct MinImpl<Head, Tail...>
{
    constexpr static int tail_value = MinImpl<Tail...>::value;
    constexpr static int value = tail_value < Head ? tail_value : Head;
};

template <int Single>
struct MinImpl<Single>
{
    constexpr static int value = Single;
};

template <class L>
struct Min{};

template <int ... Ints>
struct Min<IntList<Ints...>>
{
    constexpr static int value = MinImpl<Ints...>::value;
};

static_assert(Min<IntList<3,6,1,9,2>>::value == 1);

template <int ... Ints>
struct MaxImpl;

template <int Head, int ... Tail>
struct MaxImpl<Head, Tail...>
{
    constexpr static int tail_value =  MaxImpl<Tail...>::value;
    constexpr static int value = tail_value < Head ? Head : tail_value;
};

template <int Single>
struct MaxImpl<Single>
{
    constexpr static int value = Single;
};

template <class L>
struct Max;

template <int ... Ints>
struct Max<IntList<Ints...>>
{
    static constexpr int value = MaxImpl<Ints...>::value;
};

static_assert(Max<IntList<3,6,1,9,2>>::value == 9);

template <int ToMatch, class L>
struct IsAny;

template <int ToMatch, int ... Ints>
struct IsAnyImpl;

template <int ToMatch, int Head, int ... Tail>
struct IsAnyImpl<ToMatch, Head, Tail...>
{
    static constexpr bool value = (Head == ToMatch) | IsAnyImpl<ToMatch, Tail...>::value;
};

template <int ToMatch, int Single>
struct IsAnyImpl<ToMatch, Single>
{
    static constexpr bool value = Single == ToMatch;
};

template <int ToMatch, int ... Ints>
struct IsAny<ToMatch, IntList<Ints...>>
{
    static constexpr bool value = IsAnyImpl<ToMatch, Ints...>::value;
};

static_assert(IsAny<9, IntList<3,6,1,9,2>>::value);
static_assert(IsAny<31, IntList<3,6,1,9,2>>::value == false);

template <class L>
struct Size;

template <int ... Ints>
struct SizeImpl;

template <int Head, int ... Tail>
struct SizeImpl<Head, Tail...>
{
    constexpr static int value = 1 + SizeImpl<Tail...>::value;
};

template <>
struct SizeImpl<>
{
    constexpr static int value = 0;
};

template <int ... Ints>
struct Size<IntList<Ints...>>
{
    constexpr static int value = SizeImpl<Ints...>::value;
};

static_assert(Size<IntList<3,6,1,9,2>>::value == 5);

template <class L>
struct Average;

template <int ... Ints>
struct Average<IntList<Ints...>>
{
private:
    static constexpr int sum = Sum<IntList<Ints...>>::value;
    static constexpr int size = Size<IntList<Ints...>>::value;

public:
    static constexpr int value = sum/size;
};

static_assert(Average<IntList<2,2,3,4,4>>::value == 3);

template <int ValueToRemove, typename List>
struct RemoveValue;

template <int ValueToRemove, int ...Ints>
struct RemoveValueImpl;

template <int ValueToRemove, int Head, int ...Tail>
struct RemoveValueImpl<ValueToRemove, Head, Tail...>
{
    using listWithoutHead = RemoveValueImpl<ValueToRemove, Tail...>::type;

    using type = std::conditional<(Head == ValueToRemove),
        listWithoutHead,
        typename PushFront<listWithoutHead, Head>::type>::type;
};

template <int ValueToRemove, int Single>
struct RemoveValueImpl<ValueToRemove, Single>
{
    using type = std::conditional<(Single == ValueToRemove),
        IntList<>,
        IntList<Single>>::type;
};


template <int ValueToRemove, int ...Ints>
struct RemoveValue<ValueToRemove, IntList<Ints...>>
{
    using type = RemoveValueImpl<ValueToRemove,Ints...>::type;
};

auto a = IntList<1,2,3,4,5,6,7,8>();
auto b = RemoveValue<99,IntList<1,2,3,4,99,5,6,7,8>>::type();

static_assert(std::is_same<decltype(a), decltype(b)>::value);


int main(int argc, char* argv[])
{
    a = b;
    return 0;
}
