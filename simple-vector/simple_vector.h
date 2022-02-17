#pragma once

#include "array_ptr.h"

#include <cassert>
#include <initializer_list>
#include <stdexcept>

#include <utility>
#include <algorithm>

class ReserveProxyObj
{
public:
    ReserveProxyObj(size_t capacity_to_reserve)
        : capacity_to_reserve_(capacity_to_reserve)
    {
    }
    size_t GetCapscityToReserve()
    {
        return capacity_to_reserve_;
    }

private:
    size_t capacity_to_reserve_;
};

ReserveProxyObj Reserve(size_t capacity_to_reserve) {
    return ReserveProxyObj(capacity_to_reserve);
}


template <typename Type>
class SimpleVector {
public:
    using Iterator = Type*;
    using ConstIterator = const Type*;

    SimpleVector() noexcept = default;

    // Создаёт вектор из size элементов, инициализированных значением по умолчанию
    explicit SimpleVector(size_t size);

    // Создаёт вектор из size элементов, инициализированных значением value
    SimpleVector(size_t size, const Type& value);

    // Создаёт вектор из std::initializer_list
    SimpleVector(std::initializer_list<Type> init);

    SimpleVector(ReserveProxyObj reserve);

    SimpleVector(const SimpleVector& other);

    SimpleVector(SimpleVector&& other) noexcept;

    SimpleVector& operator=(const SimpleVector& rhs);

    SimpleVector& operator=(SimpleVector&& rhs) noexcept;

    // Возвращает количество элементов в массиве
    size_t GetSize() const noexcept;

    // Возвращает вместимость массива
    size_t GetCapacity() const noexcept;

    // Сообщает, пустой ли массив
    bool IsEmpty() const noexcept;

    // Возвращает ссылку на элемент с индексом index
    Type& operator[](size_t index) noexcept;

    // Возвращает константную ссылку на элемент с индексом index
    const Type& operator[](size_t index) const noexcept;

    // Возвращает константную ссылку на элемент с индексом index
    // Выбрасывает исключение std::out_of_range, если index >= size
    Type& At(size_t index);

    // Возвращает константную ссылку на элемент с индексом index
    // Выбрасывает исключение std::out_of_range, если index >= size
    const Type& At(size_t index) const;

    // Обнуляет размер массива, не изменяя его вместимость
    void Clear() noexcept;

    // Изменяет размер массива.
    // При увеличении размера новые элементы получают значение по умолчанию для типа Type
    void Resize(size_t new_size);

    //void MoveResize(size_t new_size);

    // Добавляет элемент в конец вектора
    // При нехватке места увеличивает вдвое вместимость вектора
    void PushBack(const Type& item);

    void PushBack(Type&& item);

    // Вставляет значение value в позицию pos.
    // Возвращает итератор на вставленное значение
    // Если перед вставкой значения вектор был заполнен полностью,
    // вместимость вектора должна увеличиться вдвое, а для вектора вместимостью 0 стать равной 1
    Iterator Insert(ConstIterator pos, const Type& value);

    Iterator Insert(ConstIterator pos, Type&& value);

    // "Удаляет" последний элемент вектора. Вектор не должен быть пустым
    void PopBack() noexcept;

    // Удаляет элемент вектора в указанной позиции
    Iterator Erase(ConstIterator pos);

    void Reserve(size_t new_capacity);

    // Обменивает значение с другим вектором
    void swap(SimpleVector& other) noexcept;

    // Возвращает итератор на начало массива
    // Для пустого массива может быть равен (или не равен) nullptr
    Iterator begin() noexcept;

    // Возвращает итератор на элемент, следующий за последним
    // Для пустого массива может быть равен (или не равен) nullptr
    Iterator end() noexcept;

    // Возвращает константный итератор на начало массива
    // Для пустого массива может быть равен (или не равен) nullptr
    ConstIterator begin() const noexcept;

    // Возвращает итератор на элемент, следующий за последним
    // Для пустого массива может быть равен (или не равен) nullptr
    ConstIterator end() const noexcept;

    // Возвращает константный итератор на начало массива
    // Для пустого массива может быть равен (или не равен) nullptr
    ConstIterator cbegin() const noexcept;

    // Возвращает итератор на элемент, следующий за последним
    // Для пустого массива может быть равен (или не равен) nullptr
    ConstIterator cend() const noexcept;

private:
    ArrayPtr<Type> simple_vector_{};
    size_t size_ = 0;
    size_t capacity_ = 0;
};

template<typename Type>
SimpleVector<Type>::SimpleVector(size_t size)
    : simple_vector_(ArrayPtr<Type>(size)), size_(size), capacity_(size)
{
    std::for_each(begin(), end(), [](auto& n) { n = Type(); });
}

template<typename Type>
SimpleVector<Type>::SimpleVector(size_t size, const Type& value)
    : simple_vector_(ArrayPtr<Type>(size)), size_(size), capacity_(size)
{
    std::for_each(begin(), end(), [value](auto& n) { n = value; });
}

template<typename Type>
SimpleVector<Type>::SimpleVector(std::initializer_list<Type> init)
    : simple_vector_(ArrayPtr<Type>(init.size())), size_(init.size()), capacity_(init.size())
{
    for (size_t i = 0; i < size_; i++)
    {
        simple_vector_[i] = *(init.begin() + i);
    }
}

template<typename Type>
SimpleVector<Type>::SimpleVector(ReserveProxyObj reserve)
    : simple_vector_(ArrayPtr<Type>(reserve.GetCapscityToReserve())), capacity_(reserve.GetCapscityToReserve())
{
}

template<typename Type>
SimpleVector<Type>::SimpleVector(const SimpleVector& other)
    : simple_vector_(ArrayPtr<Type>(other.size_)), size_(other.size_), capacity_(other.capacity_)
{
    for (size_t i = 0; i < size_; i++)
    {
        simple_vector_[i] = other[i];
    }
}

template<typename Type>
SimpleVector<Type>::SimpleVector(SimpleVector&& other) noexcept
    : size_(std::exchange(other.size_, 0)), capacity_(std::exchange(other.capacity_, 0))
{
    simple_vector_.swap(other.simple_vector_);
}

template<typename Type>
SimpleVector<Type>& SimpleVector<Type>::operator=(const SimpleVector& rhs)
{
    if (this != &rhs)
    {
        SimpleVector rhs_copy(rhs);
        swap(rhs_copy);
    }
    return *this;
}

template<typename Type>
SimpleVector<Type>& SimpleVector<Type>::operator=(SimpleVector&& rhs) noexcept
{
    if (this != &rhs)
    {
        SimpleVector rhs_move(rhs);
        swap(rhs_move);
    }
    return *this;
}

template<typename Type>
size_t SimpleVector<Type>::GetSize() const noexcept
{
    return size_;
}

template<typename Type>
size_t SimpleVector<Type>::GetCapacity() const noexcept
{
    return capacity_;
}

template<typename Type>
bool SimpleVector<Type>::IsEmpty() const noexcept
{
    return !size_;
}

template<typename Type>
Type& SimpleVector<Type>::operator[](size_t index) noexcept
{
    return simple_vector_[index];
}

template<typename Type>
const Type& SimpleVector<Type>::operator[](size_t index) const noexcept
{
    return simple_vector_[index];
}

template<typename Type>
Type& SimpleVector<Type>::At(size_t index)
{
    if (index >= size_)
    {
        throw std::out_of_range("Bad index");
    }
    return simple_vector_[index];
}

template<typename Type>
const Type& SimpleVector<Type>::At(size_t index) const
{
    if (index >= size_)
    {
        throw std::out_of_range("Bad index");
    }
    return simple_vector_[index];
}

template<typename Type>
void SimpleVector<Type>::Clear() noexcept
{
    size_ = 0;
}

template<typename Type>
void SimpleVector<Type>::Resize(size_t new_size)
{
    if (new_size <= size_)
    {
        size_ = new_size;
    }
    else if (new_size <= capacity_)
    {
        std::for_each(begin() + size_, begin() + new_size, [](auto& n) { n = Type(); });
        size_ = new_size;
    }
    else
    {
        size_t new_capacity = std::max(new_size, capacity_ * 2);

        ArrayPtr<Type> new_simple_vector(new_capacity);
        std::move(begin(), end(), new_simple_vector.Get());
        std::for_each(new_simple_vector.Get() + size_, new_simple_vector.Get() + new_capacity, [](auto& n) { n = Type(); });
        simple_vector_.swap(new_simple_vector);
        size_ = new_size;
        capacity_ = new_capacity;
    }
}

template<typename Type>
void SimpleVector<Type>::PushBack(const Type& item)
{
    if (size_ < capacity_)
    {
        simple_vector_[size_] = item;
        ++size_;
    }
    else
    {
        Resize(size_ + 1);
        simple_vector_[size_ - 1] = item;
    }
}

template<typename Type>
inline void SimpleVector<Type>::PushBack(Type&& item)
{
    if (size_ < capacity_)
    {
        simple_vector_[size_] = std::move(item);
        ++size_;
    }
    else
    {
        Resize(size_ + 1);
        simple_vector_[size_ - 1] = std::move(item);
    }
}

template<typename Type>
typename SimpleVector<Type>::Iterator SimpleVector<Type>::Insert(ConstIterator pos, const Type& value)
{
    size_t insert_index = pos - begin();
    if (size_ < capacity_)
    {
        std::copy_backward(begin() + insert_index, end(), end() + 1);
        simple_vector_[insert_index] = value;
        ++size_;
        return begin() + insert_index;
    }
    else
    {
        size_t new_capacity = std::max(size_ + 1, capacity_ * 2);
        SimpleVector new_simple_vector(new_capacity);
        new_simple_vector.size_ = insert_index;
        std::copy(begin(), begin() + insert_index, new_simple_vector.begin());
        new_simple_vector.PushBack(value);
        std::copy(begin() + insert_index, end(), new_simple_vector.begin() + insert_index + 1);
        new_simple_vector.size_ = size_ + 1;
        swap(new_simple_vector);
        return begin() + insert_index;
    }
}

template<typename Type>
typename SimpleVector<Type>::Iterator SimpleVector<Type>::Insert(ConstIterator pos, Type&& value)
{
    size_t insert_index = pos - begin();
    if (size_ < capacity_)
    {
        std::copy_backward(std::make_move_iterator(begin()) + insert_index, std::make_move_iterator(end()), end() + 1);
        simple_vector_[insert_index] = std::move(value);
        ++size_;
        return begin() + insert_index;
    }
    else
    {
        size_t new_capacity = std::max(size_ + 1, capacity_ * 2);
        SimpleVector new_simple_vector(new_capacity);
        new_simple_vector.size_ = insert_index;
        std::move(begin(), begin() + insert_index, new_simple_vector.begin());
        new_simple_vector.PushBack(std::move(value));
        std::move(begin() + insert_index, end(), new_simple_vector.begin() + insert_index + 1);
        new_simple_vector.size_ = size_ + 1;
        swap(new_simple_vector);
        return begin() + insert_index;
    }
}

template<typename Type>
void SimpleVector<Type>::PopBack() noexcept
{
    --size_;
}

template<typename Type>
typename SimpleVector<Type>::Iterator SimpleVector<Type>::Erase(ConstIterator pos)
{
    size_t erase_index = pos - begin();
    std::move(begin() + erase_index + 1, end(), begin() + erase_index);
    --size_;
    return begin() + erase_index;
}

template<typename Type>
void SimpleVector<Type>::Reserve(size_t new_capacity)
{
    if (new_capacity > capacity_)
    {
        SimpleVector<Type> new_simple_vector(new_capacity);
        std::copy(begin(), end(), new_simple_vector.begin());
        swap(new_simple_vector);
        size_ = new_simple_vector.GetSize();
    }
}

template<typename Type>
void SimpleVector<Type>::swap(SimpleVector& other) noexcept
{
    simple_vector_.swap(other.simple_vector_);
    std::swap(size_, other.size_);
    std::swap(capacity_, other.capacity_);
}

template <typename Type>
typename SimpleVector<Type>::Iterator SimpleVector<Type>::begin() noexcept
{
    return simple_vector_.Get();
}

template <typename Type>
typename SimpleVector<Type>::Iterator SimpleVector<Type>::end() noexcept
{
    return simple_vector_.Get() + size_;
}

template <typename Type>
typename SimpleVector<Type>::ConstIterator SimpleVector<Type>::begin() const noexcept
{
    return static_cast<const Type*>(simple_vector_.Get());
}

template <typename Type>
typename SimpleVector<Type>::ConstIterator SimpleVector<Type>::end() const noexcept
{
    return static_cast<const Type*>(simple_vector_.Get() + size_);
}

template <typename Type>
typename SimpleVector<Type>::ConstIterator SimpleVector<Type>::cbegin() const noexcept
{
    return begin();
}

template <typename Type>
typename SimpleVector<Type>::ConstIterator SimpleVector<Type>::cend() const noexcept
{
    return end();
}

template <typename Type>
bool operator==(const SimpleVector<Type>& lhs, const SimpleVector<Type>& rhs)
{
    return std::equal(lhs.begin(), lhs.end(), rhs.begin());
}

template <typename Type>
bool operator!=(const SimpleVector<Type>& lhs, const SimpleVector<Type>& rhs)
{
    return !(rhs == lhs);
}

template <typename Type>
bool operator<(const SimpleVector<Type>& lhs, const SimpleVector<Type>& rhs)
{
    return std::lexicographical_compare(lhs.begin(), lhs.end(), rhs.begin(), rhs.end());
}

template <typename Type>
bool operator<=(const SimpleVector<Type>& lhs, const SimpleVector<Type>& rhs)
{
    return !(rhs < lhs);
}

template <typename Type>
bool operator>(const SimpleVector<Type>& lhs, const SimpleVector<Type>& rhs)
{
    return rhs < lhs;
}

template <typename Type>
bool operator>=(const SimpleVector<Type>& lhs, const SimpleVector<Type>& rhs)
{
    return !(lhs < rhs);
}
