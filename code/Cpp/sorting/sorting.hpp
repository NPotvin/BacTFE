#ifndef SORTING_HPP
#define SORTING_HPP

#include <cstddef>
#include <thread>
#include "MergeArray.hpp"


template<typename T>
struct threadUtils {
	// The struct used to store the necessary args for a thread
	MergeArray<T>& array; // ref to the array to sort (see MergeArray for more info)
	const unsigned maxThread; // the number of thread this new thread can instantiate
	// the thread has to sort the part of the array determined by [begin , end[
	std::ptrdiff_t begin;
	std::ptrdiff_t end;
};


template <typename T>
inline void monoMerge(MergeArray<T>& array, const std::ptrdiff_t& begin, const std::ptrdiff_t& end) {
	/*
		Recursive function used to sort an array while mono threading
			args : -array : the array to sort
				   -begin : the id of the first element of the part to sort by this function
				   -end : the id of the last element (exclusive) of the part to sort
	*/
    std::ptrdiff_t length = end-begin;
    if (length != 1) { // stop condition
    	// spliting the work in two, first sort the left part, then the left
        monoMerge(array, begin, begin+length/2);
        monoMerge(array, begin+length/2, end);
    }
    // once here, all subparts are either sorted or one element long, it's time to merge them
    array.merge(begin, end);
}

template <typename T>
inline void multiMerge(const threadUtils<T>& utils) {
	/*
		Function used to sort an array while multi threading
			args : -utils a threadUtils struct containing the info necessary to this thread
					(see above for doc on threadUtils)
	*/
    if (utils.maxThread > 1 || utils.maxThread <= 0) {
    	// if the number of thread wanted by the user is a positive number, <=0 is impossible
    	// (it stops dividing once the value reach 1)
    	// for test purpose, null and negative values can be used to allow an unlimited number of threads
        
        std::ptrdiff_t length = utils.end-utils.begin;
    	// preparing to launch two new threads, here are the information they need
    	threadUtils<T> leftUtils = {
    		utils.array,
    		utils.maxThread/2,
    		utils.begin,
    		utils.begin+length/2
    	};
    	threadUtils<T> rightUtils = {
    		utils.array,
    		utils.maxThread/2,
    		utils.begin+length/2,
    		utils.end
    	};

    	// launching them
        std::thread left(multiMerge<T>, leftUtils);
        std::thread right(multiMerge<T>, rightUtils);

        // waiting for them to finish their job
        left.join();
        right.join();

        // once done, the two subarrays are sorted and can be merged
        utils.array.merge(utils.begin, utils.end);
    }
    else
    	// in the case of maxThread being 1, we cannot instantiate new threads and we switch to mono thread mode
        monoMerge<T>(utils.array, utils.begin, utils.end);
}

template <typename T>
inline const T* mergeSort(const T* const array, const std::size_t& length, const unsigned& maxThread) {
	/*
		Main function of this module, used by main. It simply instantiates a threadUtils and pass it to multiMerge().
		IMPORTANT : the number of thread given by maxThread is not the number of threads instantiated, but the number
					of threads running simultaneously (meaning if maxThread is 2, there will be at most 3 threads :
					two running and one waiting; if maxThread is 4, there will be at most 7 : 3 waiting and 4 running;
					and so on)
		args : -array : pointer to an array of random integers
			   -length : size of the array
			   -maxThread : max number of running threads allowed
		returns : a pointer to a sorted array (a copy of the initial one)
	*/
    MergeArray<T> tmp(array, length); // creating the MergeArray here, its reference is used in all threads
	threadUtils<T> utils = {
		tmp,
		maxThread,
		0,
		static_cast<std::ptrdiff_t>(length)
	};

	// calling multiMerge, which will do the actual work of sorting the array
    multiMerge(utils);

    // returning the result, it is an array of const unsigned stored in tmp (the MergeArray object)
    return tmp.getArray();
}

template <typename T>
bool checkArray(const T* const array, const std::size_t& length) {
	/*
		Small simple function used by main to check if the array is sorted
	*/
    bool flag = true;
    std::ptrdiff_t i = 1;
    while (flag && static_cast<std::size_t>(i)<length) {
        flag = array[i-1] <= array[i];
        i++;
    }
    return flag;
}

#endif