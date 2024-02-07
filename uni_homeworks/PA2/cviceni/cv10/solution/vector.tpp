#include "vector.h"

template <typename T>
Vector<T>::Vector(): m_count(0), m_capacity(0), m_data(nullptr) {}
template <typename T>
Vector<T>::Vector(const size_t initial_capacity)
	: m_count(0),
	m_capacity(initial_capacity),
	m_data(initial_capacity == 0 ? nullptr : new T[initial_capacity])
{
}
template <typename T>
Vector<T>::Vector(std::initializer_list<T> init)
	: m_count(0),
	m_capacity(init.size()),
	m_data(m_capacity == 0 ? nullptr : new T[m_capacity])
{
	for (const auto& e : init)
	{
		push_back(e);
	}
}
template <typename T>
Vector<T>::~Vector() { delete[] m_data; }

template <typename T>
size_t Vector<T>::size() const { return m_count; }

template <typename T>
bool Vector<T>::empty() const { return m_count == 0; }

template <typename T>
void Vector<T>::push_back(const T& value)
{
	if (m_count == m_capacity) reserve(m_capacity == 0 ? 1 : m_capacity * 2);
	m_data[m_count++] = value;
}

template <typename T>
void Vector<T>::pop_back()
{
	if (m_count > 0) m_count--;
}

template <typename T>
const T& Vector<T>::at(const size_t idx) const
{
	if (idx >= m_count)
		throw std::out_of_range("Vector::at failed, " + std::to_string(idx) +
			">=" + std::to_string(m_count));
	return m_data[idx];
}

template <typename T>
T& Vector<T>::at(const size_t idx)
{
	if (idx >= m_count)
		throw std::out_of_range("Vector::at failed, " + std::to_string(idx) +
			">=" + std::to_string(m_count));
	return m_data[idx];
}

template <typename T>
const T& Vector<T>::operator[](const size_t idx) const { return m_data[idx]; }

template <typename T>
T& Vector<T>::operator[](const size_t idx) { return m_data[idx]; }

template <typename T>
void Vector<T>::clear() { m_count = 0; }

template <typename T>
void Vector<T>::insert(const size_t idx, const T& value)
{
	if (idx > m_count)
		throw std::out_of_range("Vector::insert failed, " + std::to_string(idx) +
			'>' + std::to_string(m_count));
	if (m_count == m_capacity) reserve(m_capacity == 0 ? 1 : m_capacity * 2);
	for (size_t i = m_count; i > idx; --i) m_data[i] = m_data[i - 1];

	m_data[idx] = value;
	m_count++;
}

template <typename T>
void Vector<T>::erase(const size_t idx)
{
	if (idx >= m_count)
		throw std::out_of_range("Vector::erase failed, " + std::to_string(idx) +
			">=" + std::to_string(m_count));

	for (size_t i = idx + 1; i < m_count; ++i) m_data[i - 1] = m_data[i];
	--m_count;
}

template <typename T>
T* Vector<T>::data() { return m_data; }

template <typename T>
const T* Vector<T>::data() const { return m_data; }

template <typename T>
Vector<T>::Vector(const Vector<T>& other)
	: m_count(other.m_count),
	m_capacity(other.m_capacity),
	m_data(new T[m_capacity])
{
	std::copy(other.m_data, other.m_data + other.m_count, m_data);
}

template <typename T>
Vector<T>& Vector<T>::operator=(const Vector<T>& other)
{
	if (this == &other)
	{
		return *this;
	}

	delete[] m_data;

	m_count = other.m_count;
	m_capacity = other.m_capacity;
	m_data = new T[m_capacity];
	std::copy(other.m_data, other.m_data + other.m_count, m_data);

	return *this;
}

template <typename T>
bool Vector<T>::operator==(const Vector<T>& other)
{
	if (m_count != other.m_count) return false;
	for (size_t i = 0; i < m_count; i++)
	{
		if (m_data[i] != other.m_data[i]) return false;
	}
	return true;
}

template <typename T>
void Vector<T>::reserve(const size_t new_capacity)
{
	T* new_data = new T[new_capacity];
	for (size_t i = 0; i < m_count; ++i) new_data[i] = m_data[i];

	delete[] m_data;
	m_data = new_data;
	m_capacity = new_capacity;
}


template <typename T>
Vector<T> Vector<T>::filter(const std::function<bool(const T&)>& predicate) const
{
	Vector<T> result;
	for (size_t i = 0; i < size(); i++)
		if (predicate(at(i)))
			result.push_back(at(i));
	return result;
}

template <typename T>
template <typename Result>
Vector<Result> Vector<T>::map(const std::function<Result(const T&)>& fn) const
{
	Vector<Result> result;
	for (size_t i = 0; i < size(); i++)
		result.push_back(fn(at(i)));
	return result;
}