#include <cstddef>
#include <thread>
#include "sorting.hpp"
#include "MergeArray.hpp"



template <typename T>
void monoMerge(MergeArray<T>& array, const std::ptrdiff_t& begin, const std::ptrdiff_t& end) {
    std::ptrdiff_t length = end-begin;
    if (length != 1) {
        monoMerge(array, begin, begin+length/2);
        monoMerge(array, begin+length/2, end);
    }
    array.merge(begin, end, static_cast<std::size_t>(length));
}

template <typename T>
void multiMerge(MergeArray<T>& array, const unsigned& maxThread, std::ptrdiff_t begin, std::ptrdiff_t end) {
    if (maxThread > 1 || maxThread <= 0) {
        std::ptrdiff_t length = end-begin;
        std::thread left(multiMerge, array, maxThread/2, begin, begin+length/2);
        std::thread right(multiMerge, array, maxThread/2, begin+length/2, end);

        left.join();
        right.join();

        array.merge(begin, end, static_cast<std::size_t >(length));
    }
    else
        monoMerge(array, begin, end);
}

template <typename T>
T* mergeSort(const T* const array, const std::size_t& length, const unsigned& maxThread) {
    MergeArray<T> tmp(length);
    for (std::ptrdiff_t i=0; static_cast<std::size_t>(i)<length; ++i)
        tmp[i] = array[i];

    multiMerge(tmp, maxThread, 0, static_cast<std::ptrdiff_t>(length));

    T* res = new T[length];
    for (std::ptrdiff_t i=0; static_cast<std::size_t>(i)<length; ++i)
        res[i] = tmp[i];

    return res;
}

template <typename T>
bool checkArray(const T* const array, const std::size_t& length) {
    bool flag = true;
    std::ptrdiff_t i = 1;
    while (flag && static_cast<std::size_t>(i)<length)
        flag = array[i-1] <= array[i++];
    return flag;
}