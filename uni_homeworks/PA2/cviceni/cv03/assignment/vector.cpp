#include "vector.h"

// TODO: Implement Vector
Vector::Vector()
: len(0), capacity(100)
{
    arr = new int [capacity];
}

Vector::Vector(const size_t initial_capacity)
: len(0), capacity(initial_capacity)
{
    arr = new int [capacity];
};

Vector::~Vector()
{
    delete [] arr;
}

size_t Vector::size() const
{
    return len;
};

bool Vector::empty() const
{
    if ( len == 0 )
        return true;
    return false; 
};

bool Vector::full_capacity () const
{
    if ( len == capacity )
        return true;
    return false;
};

void Vector::extend_array ()
{
    capacity *= 2;
    int * tmp = new int [capacity];
    for ( size_t i = 0; i < len; i ++ )
        tmp [i] = arr [i];
    delete [] arr;
};

void Vector::push_back(const int& value)
{
    if ( full_capacity() )
        extend_array();
    arr [len] = value;
    len ++; 
};

void Vector::pop_back()
{
    if ( ! empty() )
        len --;
};

const int& Vector::at(const size_t idx) const
{
    assert ( idx < len );
    return arr [idx];
};

int& Vector::at(const size_t idx)
{
    assert ( idx < len );
    return arr [idx];
};

size_t Vector::count(const int& value) const
{
    size_t tmp = 0;
    for ( size_t i = 0; i < len; i ++ )
        if ( arr [i] == value )
            tmp ++;
    return tmp;
};

void Vector::insert(const size_t idx, const int& value)
{
    if ( full_capacity() )
        extend_array();
    for ( size_t i = len; i > idx; i -- )
        arr [i] = arr [i - 1];
    arr [idx] = value;
    len ++;
};

void Vector::erase(const size_t idx)
{
    len --;
    for ( size_t i = idx; i < len; i ++ )
        arr [i] = arr [i + 1];
};

int* Vector::data()
{
    return arr;
};

const int* Vector::data() const
{
    return arr;
};

size_t find_index(const Vector& vector, const int& value)
{
    
    return 0;
}

