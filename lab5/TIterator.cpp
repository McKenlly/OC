#ifndef LAB6_TITERATOR_CPP
#define LAB6_TITERATOR_CPP
#include "./TIterator.h"

template <class T>
TIterator<T>::TIterator(size_t position, T *value) {
    _index = position;
    _value = value;
}

template <class T>
T & TIterator<T>::operator*() {
    return _value[_index];
}

template <class T>
T & TIterator<T>::operator->() {
    return _value[_index];
}

template <class T>
void TIterator<T>::operator++() {
    _index++;
}

template <class T>
void TIterator<T>::operator--() {
    _index--;
}
template <class T>
TIterator<T> TIterator<T>::operator++(int) {
    TIterator<T> tmp = *this;
    ++(*this);
    return tmp;
}

template <class T>
TIterator<T> TIterator<T>::operator--(int) {
    TIterator<T> *tmp = this;
    --(*this);
    return *tmp;
}

template <class T>
bool TIterator<T>::operator==(const TIterator &i) const{
    return _index == i._index &&
           _value == i._value;
}

template <class T>
bool TIterator<T>::operator!=(const TIterator &i) const{
    return !(this->_index == i._index);
}
#endif