#ifndef LAB6_TVECTORITEM_CPP
#define LAB6_TVECTORITEM_CPP
#include "./TVectorItem.h"
template <class T>
TVectorItem <T>::TVectorItem () {
    this->_value = nullptr;
}

template <class T>
TVectorItem <T>::TVectorItem (T &item) {
    this->_value = item;
}

template <class T>
TVectorItem <T>::~TVectorItem () {
    if (_value) {
        delete _value;
    }
}

#endif