#ifndef LAB5_TVECTOR_CPP
#define LAB5_TVECTOR_CPP
#include "vector.h"


template <class T>
TVector<T>::TVector() {
    _data = new TVectorItem<T>*[DEFAULT_CAPACITY];
    _capacity = DEFAULT_CAPACITY;
    _size = 0;
}
template <class T>
TVector<T>::TVector(const size_t &sizeArr) {
    _data = new TVectorItem<T>*[sizeArr];
    for (int i = 0; i < sizeArr; i++) {
        _data[i] = nullptr;
    }
    _capacity = sizeArr;
    _size = 0;
}

template <class T>
TVector<T>::TVector(TVector<T>& orig) {
    _data = new TVectorItem<T>*[orig._capacity];
    this->_size = orig._size;
    this->_capacity = orig._capacity;
    for (size_t index = 0; index < _size; index++) {
        _data[index] = orig._data[index];
    }
}

template <class T>
bool TVector<T>::Empty() {
    return _size == 0;
}

template <class T>
void TVector<T>::Push_back(T &temp) {
    if (_size == _capacity) {
        _capacity *= 2;
        TVectorItem<T> **copyArr = new TVectorItem<T>*[_capacity];
        for (size_t index = 0; index < _size; ++index) {
            copyArr[index] = this->_data[index];
        }
        delete [] _data;
        _data = copyArr;
    }
    this->_data[_size++] = new TVectorItem<T>(temp);
}

template <class T>
bool TVector<T>::Delete(const size_t index) {
    TVectorItem<T> **tCopy = new TVectorItem<T>*[_capacity];
    int j = 0;
    bool flag = false;
    for (int i = 0; i < _size; i++) {
        if (i!=index) {
            tCopy[j++] = _data[i];
        }
        else {
            TVectorItem<T> *tmp = _data[i];
            delete tmp;
            flag = true;
        }
    }
    _size--;
    delete [] _data;
    _data = tCopy;
    return flag;
}

template <class T>
T& TVector<T>::operator[](size_t index) {
    return **_data[index];
}
template <class T>
size_t TVector<T>::Size(){
    return this->_size;
}

template <class T>
size_t TVector<T>::Capacity() const {
    return this->_capacity;
}

template <class T>
TVector<T>::~TVector() {
    for (int i = 0; i < _size; i++)
        if (_data[i]!= nullptr)
            delete _data[i];
    delete [] _data;
}

template <class T>
TIterator<TVectorItem<T>*> TVector<T>::begin() {
    return TIterator<TVectorItem<T>*>(0, _data);
}

template <class T>
TIterator<TVectorItem<T>*> TVector<T>::end() {
    return TIterator<TVectorItem<T>*>(_size, _data);
}

#endif