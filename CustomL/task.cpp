#include <iostream>
#include <string>

using namespace std;

template <char... c> 
struct TString{
    static constexpr char val[sizeof...(c) + 1] = {c..., '\0'};
    static constexpr int size = sizeof...(c);

    template <char... T>
    constexpr bool operator==(const TString<T...>& other) const {
        return true;
    }
};

template <char... c, char... T>
constexpr TString<c..., T...> operator+(const TString<c...>& f, const TString<T...>& s){
    return TString<c..., T...>();
}

template <typename T, T... c>
constexpr TString<c...> operator "" _s(){
    return TString<c...>();
}

int main(){
    constexpr auto hello = "hello"_s + "world"_s;
    static_assert(hello == "hello world"_s);
    return 0;
}