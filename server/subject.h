#ifndef PROXY_SUBJECT_H
#define PROXY_SUBJECT_H

#include <unordered_set>
#include "observer.h"

template<class T>
class Subject {
public:
    void Attach(Observer<T>* observer) {
        observers_.insert(observer);
    }

    void Detach(Observer<T>* observer) {
        observers_.erase(observer);
    }

    void Notify(const T& something) {
        for (auto& observer_ : observers_) {
            observer_->Update(something);
        }
    }

private:
    std::unordered_set<Observer<T>*> observers_;
};

#endif //PROXY_SUBJECT_H
