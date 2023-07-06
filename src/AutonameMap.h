#pragma once

#include <iostream>
#include <map>

template <class T>
class AutonameMap : public std::map<std::string, T> {
public:
    AutonameMap(std::initializer_list<T> init) {
        for (auto val : init) { std::map<std::string, T>::operator[](val.name) = val; }
    }
    AutonameMap() : std::map<std::string, T>::map() {}

    void insert(T& val){
        std::map<std::string, T>::operator[](val.name) = val;
    }

    void insert(const T&& val){
        std::map<std::string, T>::operator[](val.name) = val;
    }
};