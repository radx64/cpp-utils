template <typename T>
class HasCoffe
{
    template <typename A> static constexpr std::true_type hasField(decltype(A::Coffee));
    template <typename A> static constexpr std::false_type hasField(...);

public:
    static const bool value = std::is_same<decltype(hasField<T>(0)), std::true_type>::value;
};