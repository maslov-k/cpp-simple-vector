#pragma once

#include <cassert>
#include <cstdlib>
#include <utility>
#include <tuple>

template <typename Type>
class ArrayPtr {
public:
    // Инициализирует ArrayPtr нулевым указателем
    ArrayPtr() = default;

    // Создаёт в куче массив из size элементов типа Type.
    // Если size == 0, поле raw_ptr_ должно быть равно nullptr
    explicit ArrayPtr(size_t size);

    // Конструктор из сырого указателя, хранящего адрес массива в куче либо nullptr
    explicit ArrayPtr(Type* raw_ptr) noexcept;

    // Запрещаем копирование
    ArrayPtr(const ArrayPtr&) = delete;

    ~ArrayPtr();

    // Запрещаем присваивание
    ArrayPtr& operator=(const ArrayPtr&) = delete;

    // Прекращает владением массивом в памяти, возвращает значение адреса массива
    // После вызова метода указатель на массив должен обнулиться
    [[nodiscard]] Type* Release() noexcept;

    // Возвращает ссылку на элемент массива с индексом index
    Type& operator[](size_t index) noexcept;

    // Возвращает константную ссылку на элемент массива с индексом index
    const Type& operator[](size_t index) const noexcept;

    // Возвращает true, если указатель ненулевой, и false в противном случае
    explicit operator bool() const;

    // Возвращает значение сырого указателя, хранящего адрес начала массива
    Type* Get() const noexcept;

    // Обменивается значениям указателя на массив с объектом other
    void swap(ArrayPtr& other) noexcept;

private:
    Type* raw_ptr_ = nullptr;
};

template <typename Type>
ArrayPtr<Type>::ArrayPtr(size_t size)
{
    if (!size)
    {
         std::ignore = ArrayPtr();
    }
    else
    {
        raw_ptr_ = new Type[size];
    }
}

template <typename Type>
ArrayPtr<Type>::ArrayPtr(Type* raw_ptr) noexcept
    : raw_ptr_(raw_ptr)
{
}

template <typename Type>
ArrayPtr<Type>::~ArrayPtr() {
    delete[] raw_ptr_;
}

template <typename Type>
Type* ArrayPtr<Type>::Release() noexcept
{
    Type* p = raw_ptr_;
    raw_ptr_ = nullptr;
    return p;
}

template <typename Type>
Type& ArrayPtr<Type>::operator[](size_t index) noexcept
{
    return *(raw_ptr_ + index);
}

template <typename Type>
const Type& ArrayPtr<Type>::operator[](size_t index) const noexcept
{
    return *(raw_ptr_ + index);
}

template <typename Type>
ArrayPtr<Type>::operator bool() const
{
    return raw_ptr_;
}

template <typename Type>
Type* ArrayPtr<Type>::Get() const noexcept
{
    return raw_ptr_;
}

template <typename Type>
void ArrayPtr<Type>::swap(ArrayPtr& other) noexcept
{
    std::swap(raw_ptr_, other.raw_ptr_);
}