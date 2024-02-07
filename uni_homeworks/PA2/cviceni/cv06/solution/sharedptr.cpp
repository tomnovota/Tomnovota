#include "sharedptr.h"

SharedPtr::Data::Data()
    :allocated_data(nullptr), refCount(1)
{
}

SharedPtr::Data::Data(T* allocated_data)
    :allocated_data(allocated_data), refCount(1)
{
}

SharedPtr::Data::~Data()
{
    delete allocated_data;
}


SharedPtr::SharedPtr(): data(new Data()) {}

SharedPtr::SharedPtr(T* ptr): data(new Data(ptr)) {};

SharedPtr::~SharedPtr()
{
    data->refCount--;
    if (data->refCount == 0)
        delete data;
}

SharedPtr::SharedPtr(const SharedPtr& other)
{
    data = other.data;
    data->refCount++;
}

SharedPtr& SharedPtr::operator= (const SharedPtr& other)
{
    if (&other == this) return *this;

    data->refCount--;
    if (data->refCount == 0)
        delete data;

    data = other.data;
    data->refCount++;
    return *this;
}

SharedPtr::T& SharedPtr::operator * ()
{
    return *data->allocated_data;
}

SharedPtr::T* SharedPtr::operator -> ()
{
    return data->allocated_data;
}

size_t SharedPtr::refCount() const
{
    return data->refCount;
}

bool SharedPtr::operator == (const SharedPtr& other) const
{
    return data->allocated_data == other.data->allocated_data;
}

SharedPtr::operator bool() const
{
    return data->allocated_data != nullptr;
}
