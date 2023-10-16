#pragma once

#include <memory>

namespace NBinTree {

/*
    Наследование от std::enable_shared_from_this<TNode<T>>
    для приведения объекта this к shared_ptr или weak_ptr
    Данный вариант безопаснее прямого приведения 
    std::shared_ptr<TNode<T>>(this) так как в таком случае
    this может быть уничтожен несколько раз
*/
template <typename T>
class TNode : public std::enable_shared_from_this<TNode<T>> {
public:
    using TNodePtr = std::shared_ptr<TNode<T>>;
    using TNodeConstPtr = std::shared_ptr<const TNode<T>>;
    using TWeakPtr = std::weak_ptr<TNode<T>>;
    using TWeakConstPtr = std::weak_ptr<const TNode<T>>;

    bool HasLeft() const {
        return Left;
    }

    bool HasRight() const {
        return Right;
    }

    bool HasParent() const {
        return Parent;
    }

    T& GetValue() {
        return Value;
    }

    const T& GetValue() const {
        return Value;
    }

    TNodePtr GetLeft() {
        return Left;
    }

    TNodeConstPtr GetLeft() const {
        return Left;
    }

    TNodePtr GetRight() {
        return Right;
    }

    TNodeConstPtr GetRight() const {
        return Right;
    }

    TNodePtr GetParent() {
         /*
            проверка weak_ptr был ли удален объект,
            на который она ссылкается
        */
        if(!Parent.expired())
            return Parent.lock();    // если нет, то возвращаем shared_ptr
        return nullptr;
        
    }

    TNodeConstPtr GetParent() const {
        /*
            проверка weak_ptr был ли удален объект,
            на который она ссылкается
        */
        if(!Parent.expired())
            return Parent.lock();   // если нет, то возвращаем shared_ptr
        return nullptr;
    }

    static TNodePtr CreateLeaf(T value) {
        return std::make_shared<TNode>(TNode(value));
    }

    static TNodePtr Fork(T value, TNodePtr left, TNodePtr right) {
        TNodePtr ptr = std::make_shared<TNode>(TNode(value, left, right));
        SetParent(ptr->GetLeft(), ptr);
        SetParent(ptr->GetRight(), ptr);
        return ptr;
    }

    TNodePtr ReplaceLeft(TNodePtr left) {
        // Приведение данного объекта к weak_ptr как родителя
        SetParent(left, this->weak_from_this());
        SetParent(Left, TWeakPtr());
        std::swap(left, Left);
        return left;
    }

    TNodePtr ReplaceRight(TNodePtr right) {
        // Приведение данного объекта к weak_ptr как родителя
        SetParent(right, this->weak_from_this());
        SetParent(Right, TWeakPtr());
        std::swap(right, Right);
        return right;
    }

    TNodePtr ReplaceRightWithLeaf(T value) {
        return ReplaceRight(CreateLeaf(value));
    }

    TNodePtr ReplaceLeftWithLeaf(T value) {
        return ReplaceLeft(CreateLeaf(value));
    }

    TNodePtr RemoveLeft() {
        return ReplaceLeft(nullptr);
    }

    TNodePtr RemoveRight() {
        return ReplaceRight(nullptr);
    }

private:
    T Value;
    TNodePtr Left = nullptr;
    TNodePtr Right = nullptr;
    TWeakPtr Parent = TWeakPtr(); 
    /*
        Parent type: TNodePtr -> TWeakPtr
        для прерывания цикла ссылок std::shared_ptr
        Parent->Child и Child->Parent

        Так как, если на данный цикл ничего не укаывает,
        то счетчик ссылок не может достичь нуля и происходит
        утечка памяти
    */

    TNode(T value)
        : Value(value)
    {
    }

    /*  TNode* -> TNodePtr так как left и right - std::shared_ptr */
    TNode(T value, TNodePtr left, TNodePtr right)
        : Value(value)
        , Left(left)
        , Right(right)
    {
    }

    static void SetParent(TNodePtr node, TWeakPtr parent) {
        if (node) {
            node->Parent = parent;
        }
    }
};

} // namespace NBinTree
