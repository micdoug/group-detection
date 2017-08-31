#ifndef EXTENSIONS_H
#define EXTENSIONS_H

#include <set>
#include <algorithm>
#include <sstream>

namespace ext
{
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
    std::set<T> set_intersection(const std::set<T> &setA, const std::set<T> &setB)
    {
        std::set<T> result;
        std::set_intersection(setA.begin(), setA.end(), setB.begin(), setB.end(), std::inserter(result, result.begin()));
        return std::move(result);
    }

    template<typename T>
    std::set<T> set_difference(const std::set<T> &setA, const std::set<T> &setB)
    {
        std::set<T> result;
        std::set_difference(setA.begin(), setA.end(), setB.begin(), setB.end(), std::inserter(result, result.begin()));
        return std::move(result);
    }

    template<typename T>
    std::set<T> set_union(const std::set<T> &setA, const std::set<T> &setB)
    {
        std::set<T> result;
        std::set_union(setA.begin(), setA.end(), setB.begin(), setB.end(), std::inserter(result, result.begin()));
        return std::move(result);
    }

    template<typename MAP>
    bool containsKey(const MAP &map, typename MAP::key_type key)
    {
        return map.find(key) != map.end();
    }

    template<typename MAP>
    std::string to_string(const MAP &map)
    {
        std::ostringstream stream;
        stream << "[";
        for(const auto &value: map)
        {
            stream << " " << value << ",";
        }
        stream.seekp(-1, std::ios_base::cur);
        stream << " ]";
        return stream.str();
    }

    
    

}

#endif