#ifndef SORTING_SYNCARRAY_HPP
#define SORTING_SYNCARRAY_HPP
#include <mutex>
#include <cstring>

// #define USING_MUTEX

#ifdef USING_MUTEX
    #define MUTEX mutable std::mutex _mutex;
    #define LOCK _mutex.lock();
    #define UNLOCK _mutex.unlock();
    #define OTHER_LOCK other._mutex.lock();
    #define OTHER_UNLOCK other._mutex.unlock();
#endif

#ifndef USING_MUTEX
    #define MUTEX
    #define LOCK
    #define UNLOCK
    #define OTHER_LOCK
    #define OTHER_UNLOCK
#endif

template <typename T>
class MergeArray {
    /*
        Class used to guarantee an array to stay consistent through the 
            use of a mutex (is the macro USING_MUTEX is set)
        This class does not implement the dividing part of the mergesort
            algorithm, but propose a merge method once two parts have been
            sorted
        This class allows dynamic operations trhough the copy and deletion
            of the underlying array, though it is not its primary goal and
            is certainly less efficient than a vector for such operations
    */

private:

    T* _array = nullptr; // the underlying array
    MUTEX // mutable std::mutex _mutex;
    std::size_t _length = 0; // the size of the array

    inline T* copyArray(const T* const & src, T* const & dest, const std::size_t& size) {
        // used as a "specialization" of the memcpy function, copy an array in an existing bigger one
        std::memcpy(dest, src, size*sizeof(T));
        return dest;
    }

    inline T* copyArray(const T* const & array, std::size_t size) {
        // same as above, except the bigger array is created here and returned
        return copyArray(array, new T[size], size);
    }

public:

    inline explicit MergeArray(const std::size_t& length=0) {
        // default constructor, allows the reservation of some space
        _array = new T[length];
        _length = length;
    }

    inline explicit MergeArray(const T* const & array, const std::size_t& length) {
        // constructor copying an existing array
        _length = length;
        _array = copyArray(array, length);
    }

    inline MergeArray(const MergeArray<T>& other) {
        // copy constructor
        LOCK // _mutex.lock();
        OTHER_LOCK // other._mutex.lock();
        _length = other._length;
        _array = copyArray(other._array, _length);
        OTHER_UNLOCK // other._mutex.unlock();
        UNLOCK // _mutex.unlock();
    }

    inline MergeArray(MergeArray<T>&& other) noexcept {
        // transfer constructor
        LOCK // _mutex.lock();
        OTHER_LOCK // other._mutex.lock();
        _length = other._length;
        _array = other._array;
        other._array = nullptr;
        other._length = 0;
        OTHER_UNLOCK // other._mutex.unlock();
        UNLOCK // _mutex.unlock();
    }
    
    MergeArray<T>& operator=(const MergeArray<T>& other)  {
        // copy operator
        LOCK // _mutex.lock();
        OTHER_LOCK // other._mutex.lock();
        _length = other._length;
        _array = copyArray(other._array, _length);
        OTHER_UNLOCK // other._mutex.unlock();
        UNLOCK // _mutex.unlock();
    }

    MergeArray<T>& operator=(MergeArray<T>&& other) noexcept {
        // transfer operator
        LOCK // _mutex.lock();
        OTHER_LOCK // other._mutex.lock();
        _length = other._length;
        _array = other._array;
        other._array = nullptr;
        other._length = 0;
        OTHER_UNLOCK // other._mutex.unlock();
        UNLOCK // _mutex.unlock();
    }

    inline MergeArray<T>* fill(const T* const & array, const std::size_t& size) {
        // method used to fill this array, copying values from an other, adapt its size if needed
        if (_length == size) {
            copyArray(array, _array, size);
            return this;
        }
        delete[] _array;
        _length = size;
        _array = copyArray(array, size);
    }
    
    inline const T& operator[](const std::ptrdiff_t& i) const {
        /* 
            subscript operator used while getting the values without changing them
            
            the mutex lock is still necessary in case an other thread has not finished
            modifying the value asked for
        */
        LOCK // _mutex.lock();
        T& res = _array[i];
        UNLOCK // _mutex.unlock();
        return res;
    }

    inline T& operator[](const std::ptrdiff_t& i) {
        /* 
            subscript operator used to get a reference to an element allowing modification
            
            IMPORTANT : once the reference is get, an other thread may modify the element without
                        locking the mutex first

            TODO : move this method in the private part, and implement a method to modify
                    the elements locking the mutex before the change
        */
        LOCK // _mutex.lock();
        T& res = _array[i];
        UNLOCK // _mutex.unlock();
        return res;
    }

    inline void merge(const std::ptrdiff_t& begin, const std::ptrdiff_t& end) {
        /*
            This method is used as a part of the mergesort algorithm. It merges two sorted parts of the array
            into a bigger sorted part.

            args : -begin the id of the first element of the sequence to merge
                   -end the id of the last element of the sequence (exclusive)
        */
        LOCK // _mutex.lock();

        std::size_t length = static_cast<std::size_t>(end-begin);
        // we need a bit more of memory to store part of the sorted values while working on them
        T* left = copyArray(&_array[begin], length/2);

        for (std::ptrdiff_t i=begin, j=0, k=begin+length/2; static_cast<std::size_t>(j)<length/2 || k<end;)
            /*  
                for each slot we choose an element of one of the two arrays (the smallest)
                i is used for the main array (_array) and is incremented at each step
                j is used for the left part of the array and is incremented when we copy an element from it
                k is used for the right part and is incremented when we copy an element from it
            */
            if (k==end || (static_cast<std::size_t>(j)<length/2 && left[j]<_array[k]))
                _array[i++] = left[j++];
            else
                _array[i++] = _array[k++];

        delete[] left;
        left = nullptr;

        UNLOCK // _mutex.unlock();
    }

    inline const std::size_t& size() const {
        // getter
        return _length;
    }

    inline const T* getArray() {
        // getter
        return _array;
    }

    inline ~MergeArray() {
        LOCK // _mutex.lock();
        _length = 0;
        delete[] _array;
        _array = nullptr;
        UNLOCK // _mutex.unlock();
    }

};


#endif //SORTING_SYNCARRAY_HPP
