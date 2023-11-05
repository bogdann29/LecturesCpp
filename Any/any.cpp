#include <iostream>
#include <memory>

class BaseHolder{
public:
    virtual ~BaseHolder() = default;
};

template <typename T>
class Holder : public BaseHolder {
public:
    Holder(T value) : value(value) {};

    T get() const {
        return value;
    }
private:
    T value;
};


class Any{
public:
    template<typename T>
    Any(T value) : holder(new Holder<T>(value)){};

    template<typename T>
    T get(){
        auto casted = dynamic_cast<Holder<T>*>(holder.get());
        if (casted)
            return casted->get();
        else
            throw std::exception();
    }
private:
    std::unique_ptr<BaseHolder> holder;
};

int main(){
    Any a(5);
    auto value1 = a.get<int>(); // 5
    std::cout << value1 << std::endl;
    try{
        auto value2 = a.get<std::string>(); // error
    }
    catch(std::exception& ex){
        std::cout << ex.what() << std::endl;
    }
    return 0;
}