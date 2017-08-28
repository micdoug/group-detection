#ifndef EXTENSIONS_H
#define EXTENSIONS_H

#include <set>
#include <algorithm>
#include <iostream>

template<typename MAP>
std::set<typename MAP::key_type> getKeys(const MAP &map)
{
    std::set<typename MAP::key_type> set;
    for(const auto &pair: map)
    {
        set.insert(pair.first);
    }
    return set;
}

template<typename T>
std::set<T> intersection(const std::set<T> &setA, const std::set<T> &setB)
{
    std::set<T> result;
    std::set_intersection(setA.begin(), setA.end(), setB.begin(), setB.end(), std::inserter(result, result.begin()));
    return result;
}

template<typename T>
std::set<T> difference(const std::set<T> &setA, const std::set<T> &setB)
{
    std::set<T> result;
    std::set_difference(setA.begin(), setA.end(), setB.begin(), setB.end(), std::inserter(result, result.begin()));
    return result;
}

template<typename MAP>
bool containsKey(const MAP &map, typename MAP::key_type key)
{
    return map.find(key) != map.end();
}

#endif