#include <cstdlib> // size_t
#include <cstddef> // nullptr
#include <assert.h>

class Vector {
public:
    // TODO: TASK 1 - Implement the following methods:
    Vector();
    ~Vector();
    size_t size() const; // returns the number of elements
    bool empty() const; // checks whether the container is empty

    // TODO: TASK 2 - Implement the following methods:
    Vector(const size_t initial_capacity);
    void push_back(const int& value);  // adds an element to the end
    void pop_back();  // removes the last element
    const int& at(const size_t idx) const; // access specified element with bounds checking
    int& at(const size_t idx);

    // TODO: TASK 3 - Implement the following methods:
    size_t count(const int& value) const; // returns the number of occurences of an element
    void insert(const size_t idx, const int& value); // insert an element at the specified position
    void erase(const size_t idx); // erases an element at the specified position

    // TODO: TASK 4 - Implement the function find_index() and the following methods:
    int* data(); // direct access to the underlying array
    const int* data() const;

private:
    // TODO: Add private methods and fields as necessary
    size_t len;
    size_t capacity;
    int * arr;

    bool full_capacity () const;
    void extend_array ();
};

// Returns the position of the first occurence of an element (assume the array is sorted)
// NOTE: use std::lower_bound
size_t find_index(const Vector& vector, const int& value);
