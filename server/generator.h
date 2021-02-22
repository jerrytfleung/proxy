#ifndef PROXY_GENERATOR_H
#define PROXY_GENERATOR_H

#include "subject.h"

template<class T>
class Generator :  public Subject<T> {
public:
    virtual void Run() = 0;
};

#endif //PROXY_GENERATOR_H
