#include "sharedptr.h"
#include <cstddef>

// TODO provide your implementation here

SharedPtr::SharedPtr ()
: data (new Data())
{

}

SharedPtr::SharedPtr ( T* ptr )
: data ( new Data ( ptr ) )
{

}

SharedPtr::~SharedPtr ()
{

}

SharedPtr::SharedPtr ( const SharedPtr& x )
{

}

SharedPtr& SharedPtr::operator = ( const SharedPtr& x)
{

}

SharedPtr::T& SharedPtr::operator * ()
{

}

SharedPtr::T* SharedPtr::operator -> ()
{

}

bool SharedPtr::operator == ( const SharedPtr& other ) const
{

}

SharedPtr::operator bool () const
{

}

size_t SharedPtr::refCount () const
{

}

struct SharedPtr::Data {
    T* allocated_data;
    size_t refCount;

    Data();
    Data(T* allocated_data);
};