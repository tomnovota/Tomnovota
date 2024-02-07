#pragma once
#include <vector>

class SharedPtr {
public:
    using T = int;
    SharedPtr();
    SharedPtr(T*);
    ~SharedPtr();
    SharedPtr(const SharedPtr& x);
    SharedPtr& operator=(const SharedPtr& x);
    T& operator * ();
    T* operator -> ();
    bool operator == (const SharedPtr& other) const;
    operator bool() const;
    size_t refCount() const;

private:
    struct Data {
        T* allocated_data;
        size_t refCount;

        Data();
        Data(T* allocated_data);
        ~Data();
    };

    Data* data;
};

template <class Type, class ...Args>
SharedPtr make_shared(Args&&... args)
{
    return SharedPtr(new Type(std::forward<Args>(args)...)); // C++11 stuff
}
