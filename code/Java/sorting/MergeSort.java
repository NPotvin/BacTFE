package sorting;

import java.util.Arrays;

public class MergeSort <T extends Comparable<T>> {
    /*
        Class responsible of sorting the array
    */

    private static final int MAX_THREAD = 1; // default maximum thread allowed
    private T[] _array; // the array containing the integers to be sorted

    public MergeSort(final T[] array) {
        // Constructor, copies an array in _array
        _array = Arrays.copyOf(array, array.length);
    }

    public T[] sort() {
        // Public default sort() method
        return sort(MAX_THREAD);
    }

    public T[] sort(int maxThread) {
        // Public sort method calling the actual sorting method and returning the sorted array
        multiMerge(maxThread, 0, _array.length);
        return _array;
    }

    private void multiMerge(int maxThread, int begin, int end) {
        /*
            Multi threading sorting method, it splits the array in two to give half the work
            to two new threads until there is no more threads allowed, then it switches in 
            monothread mode. Once the "divide" part is finished it merges the halfs together
            and the sorting is finished (at least for the part of the array given to this thread)
        */
        if (maxThread > 1 || maxThread <= 0) {
            // 0 and negative values are used to allow the program to create an unlimited number of thread
            // otherwise, it stops creating new thread when the value of maxThread reaches 1
            int len = end-begin;
            Thread left, right;
            // creating new thread "on the fly"
            left = new Thread(
                    new Runnable() {
                        @Override
                        public void run() {
                            multiMerge(maxThread/2, begin, begin+len/2);
                        }
                    }
            );
            right = new Thread(
                    new Runnable() {
                        @Override
                        public void run() {
                            multiMerge(maxThread/2, begin+len/2, end);
                        }
                    }
            );

            // launching them and waiting for their results
            left.start();
            right.start();

            try {
                left.join();
            } catch (InterruptedException e) {
                System.err.println(String.format("Exception caught : %s\nResult may be inconsistent", e.toString()));
                monoMerge(begin, begin+len/2);
            }
            try {
                right.join();
            } catch (InterruptedException e) {
                System.err.println(String.format("Exception caught : %s\nResult may be inconsistent", e.toString()));
                monoMerge(begin+len/2, end);
            }

            // before merging the results together
            merge(begin, end, len);
        }
        else
            // single thread mode, used when this thread is not allowed to create new ones
            monoMerge(begin, end);
    }

    private void monoMerge(int begin, int end) {
        // Recursive method sorting a part of the array in single thread mode
        int len = end-begin;
        if (len != 1) {
            monoMerge(begin, begin+len/2);
            monoMerge(begin+len/2, end);
        }
        // the two halfs of the array are either sorted or only one element, they can be merged
        merge(begin, end, len);
    }

    //synchronized
    private void merge(int begin, int end, int len) {
        /*
            Merging method, it takes the two sorted halfs and merge them into a single sorted part
        */
        T[] left = Arrays.copyOfRange(_array, begin, begin+len/2);
        int i = begin, j = 0, k = begin+len/2;
        while (j<len/2 || k<end)
            if (k==end || j<len/2 && left[j].compareTo(_array[k])<0)
                _array[i++] = left[j++];
            else
                _array[i++] = _array[k++];
    }
}
