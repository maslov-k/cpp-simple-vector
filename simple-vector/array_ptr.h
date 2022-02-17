#pragma once

#include <cassert>
#include <cstdlib>
#include <utility>
#include <tuple>

template <typename Type>
class ArrayPtr {
public:
    // �������������� ArrayPtr ������� ����������
    ArrayPtr() = default;

    // ������ � ���� ������ �� size ��������� ���� Type.
    // ���� size == 0, ���� raw_ptr_ ������ ���� ����� nullptr
    explicit ArrayPtr(size_t size);

    // ����������� �� ������ ���������, ��������� ����� ������� � ���� ���� nullptr
    explicit ArrayPtr(Type* raw_ptr) noexcept;

    // ��������� �����������
    ArrayPtr(const ArrayPtr&) = delete;

    ~ArrayPtr();

    // ��������� ������������
    ArrayPtr& operator=(const ArrayPtr&) = delete;

    // ���������� ��������� �������� � ������, ���������� �������� ������ �������
    // ����� ������ ������ ��������� �� ������ ������ ����������
    [[nodiscard]] Type* Release() noexcept;

    // ���������� ������ �� ������� ������� � �������� index
    Type& operator[](size_t index) noexcept;

    // ���������� ����������� ������ �� ������� ������� � �������� index
    const Type& operator[](size_t index) const noexcept;

    // ���������� true, ���� ��������� ���������, � false � ��������� ������
    explicit operator bool() const;

    // ���������� �������� ������ ���������, ��������� ����� ������ �������
    Type* Get() const noexcept;

    // ������������ ��������� ��������� �� ������ � �������� other
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