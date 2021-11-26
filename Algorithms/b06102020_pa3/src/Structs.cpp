#include<iostream>
#include <queue>
#include <vector>
#include <tuple>
using namespace std;
typedef pair<int, int> pint;
typedef tuple<int, int, int> tint;
struct BySecond
{
    constexpr bool operator()(
        pair<int, int> &a,
        pair<int, int> &b)
        const noexcept
    {
        return a.second < b.second;
    }
};
template<int M, template<typename> class F = less>
struct TupleCompare
{
    template<typename T>
    bool operator()(T const &t1, T const &t2)
    {
        return F<typename tuple_element<M, T>::type>()(get<M>(t1), get<M>(t2));
    }
};

