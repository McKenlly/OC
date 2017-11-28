#ifndef LAB5_TVECTOR_H
#define LAB5_TVECTOR_H
#include <iostream>
#include "TVectorItem.h"
#include "TIterator.h"
const size_t DEFAULT_CAPACITY = 1;

template <class T>
class TVector {
public:
    TVector();

    explicit TVector(const size_t &);

    TVector(TVector&);

    void Push_back(T &);

    bool Delete(const size_t);

    bool Empty();

    size_t Size();

    size_t Capacity() const;

     T& operator[](size_t);


    TIterator <TVectorItem<T>*> begin ();
    TIterator <TVectorItem<T>*> end ();

    ~TVector();
private:
    TVectorItem<T> **_data;

    size_t _capacity;

    size_t _size;

};
// the types of the class factories
typedef TVector<char>* create_t();

#include "vector.cpp"
#endif  // PROG_TVector_H
