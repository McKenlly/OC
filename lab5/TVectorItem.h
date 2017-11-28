#ifndef LAB6_TVECTORITEM_H
#define LAB6_TVECTORITEM_H

#include <memory>
#include <iostream>

template <class T>
class TVectorItem {
public:
    explicit TVectorItem ();

    explicit TVectorItem (T &);


    T& operator * () {
        return _value;
    }

    virtual ~TVectorItem ();

private:

    T _value;
};

#include "TVectorItem.cpp"
#endif //LAB6_TVECTORITEM_H
