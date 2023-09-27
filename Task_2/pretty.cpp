#include <iostream>
#include <set>
#include <tuple>
#include <vector>
#include <sstream>


struct TPrettyPrinter {
    std::ostringstream os;

    std::string Str() const { return os.str(); }

    template <class P1, class P2>
    TPrettyPrinter& Format(const std::pair<P1, P2>& p) {
        os << "(" << TPrettyPrinter().Format(p.first).Str() << ", " << TPrettyPrinter().Format(p.second).Str() << ")";
        return *this;
    }

    template <class V>
    TPrettyPrinter& Format(V v){
        os << v;
        return *this;
    }

    template <class V>
    TPrettyPrinter& Format(const std::vector<V>& v) {
        os << "[";
        bool first = true;
        for (const auto& x : v) {
            if (!first) {
                os << ", ";
            }
            first = false;
            os << TPrettyPrinter().Format(x).Str();
        }
        os << "]";
        return *this;
    }

    template <class T>
    void printElem(const T& x) {
        std::cout << x << ',';
    };

    template <typename... Args>
    TPrettyPrinter& Format(const std::tuple<Args...>& t) {
        os << "(";
        std::apply([this](auto... el) {
            bool first = true;
            auto printElem = [&first, this](const auto& x) {
                if (!first) {
                    this->os << ", ";
                }
                first = false;
                this->os << TPrettyPrinter().Format(x).Str();
            };
            (printElem(el), ...);
        }, t);

        os << ")";
        return *this;
    }

    template <class S>
    TPrettyPrinter& Format(const std::set<S>& s) {
        os << "{";
        bool first = true;
        for (const auto& x : s) {
            if (!first) {
                os << ", ";
            }
            first = false;
            os << TPrettyPrinter().Format(x).Str();
        }
        os << "}";
        return *this;
    }
};

template<typename T>
std::string Format(const T& t) {
    return TPrettyPrinter().Format(t).Str();
}


int main(){
    std::tuple<std::string, int, int> t = {"xyz", 1, 2};

    std::tuple<std::string, std::vector<int>, std::pair<std::tuple<float, std::set<int>>,std::string>> t1 = {"xyz", {1,4,5,6,-7}, {{1.2f, {7,9,11,12,7,9}}, "aboba"}};

    std::vector<std::pair<int, int> > v = {{1, 4}, {5, 6}};

    std::string s1 = TPrettyPrinter().Format("vector: ").Format(v).Str();
    std::cout << s1 << std::endl;

    std::string s2 = TPrettyPrinter().Format(t).Format(" ! ").Format(0).Str();
    std::cout << s2 << std::endl;

    std::string s3 = TPrettyPrinter().Format("mnogo: ").Format(t1).Str();
    std::cout << s3 << std::endl;
}