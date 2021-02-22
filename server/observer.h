#ifndef PROXY_OBSERVER_H
#define PROXY_OBSERVER_H

template<class T>
class Observer {
public:
    virtual void Update(const T&) = 0;
};

#endif //PROXY_OBSERVER_H
