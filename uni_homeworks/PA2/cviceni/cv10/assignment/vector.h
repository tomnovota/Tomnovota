#pragma once
#include <algorithm>
#include <cstdlib>
#include <functional>
#include <ostream>
#include <string>
#include <vector>

const size_t CAPACITY = 100;

template <typename T>
class Vector {
public:
	Vector ( void )
     : m_count ( 0 ), m_capacity ( CAPACITY ), m_data ( new T [m_capacity] )
    {}
	Vector(std::initializer_list<T> init)
     : m_count ( init . size() ), m_capacity ( init . size() ), m_data ( new T [m_capacity] )
    {
        for ( auto i = 0; i < init . size(); i ++ )
            m_data [i] = move ( init [i] );
    }
	Vector(const size_t initial_capacity)
     : m_count ( 0 ), m_capacity ( initial_capacity ), m_data ( new T [m_capacity] )
    {}
	~Vector() { delete m_data; }

	// returns the number of elements
	size_t size() const { return m_count; }

	// checks whether the container is empty
	bool empty() const { return m_count == 0; }

	// adds an element to the end
	void push_back(const T& value)
    {

    }

	// removes the last element
	void pop_back() { m_count > 0 ? -- m_count : 0; }

	// access specified element with bounds checking
	const T& at(const size_t idx) const
    {
        assert ( idx < m_count );
        return m_data [idx];
    }

	// access specified element with bounds checking
	T& at(const size_t idx)
    {
        assert ( idx < m_count );
        return m_data [idx];
    }

	// access specified element with bounds checking
	const T& operator[](const size_t idx) const
    {
        assert ( idx < m_count );
        return m_data [idx];
    }

	// access specified element with bounds checking
	T& operator[](const size_t idx)
    {
        assert ( idx < m_count );
        return m_data [idx];
    }

	void clear() { m_count = 0; }

	// insert an element at the specified position
	void insert(const size_t idx, const T& value)
    {

    }

	// erases an element at the specified position
	void erase(const size_t idx)
    {

    }

	// direct access to the underlying array
	T* data() { return m_data; }

	// direct access to the underlying array
	const T* data() const { return m_data; }

	// copy constructor
	Vector(const Vector<T>& other)
    {

    }

	// assignment operator
	Vector<T>& operator=(const Vector<T>& other)
    {

    }

	// comparison operator
	bool operator==(const Vector<T>& other)
    {
        
    }

	// TEST 2: Implement this method that returns a vector with all the elements
	// for which the provided function returns true.
	Vector<T> filter(const std::function<bool(const T&)>& predicate) const;

	// TEST 3: Implement this method that applies a given function to each element
	// in a Vector and returns a new Vector containing the results.
	template <typename Result>
	Vector<Result> map(const std::function<Result(const T&)>& fn) const;

private:
	size_t m_count;
	size_t m_capacity;
	T* m_data;

	void reserve(const size_t new_capacity);
};

// print out vector as "[ m_data[0] m_data[1] ... ]"
template <typename T>
std::ostream& operator<<(std::ostream& os, const Vector<T>& vec)
{
	os << "[ ";
	for (size_t i = 0; i < vec.size(); i++)
	{
		os << vec[i] << ' ';
	}
	return os << ']';
}

// TEST 4: Implement this template specialization which will make a Vector<char>
// print the text "string" instead of "[ s t r i n g ]"
std::ostream& operator<<(std::ostream& os, const Vector<char>& vec);

#include "vector.tpp"