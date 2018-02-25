package sorting;

import java.util.Arrays;

public class MergeSort <T extends Comparable<T>> {

    private static final int MAX_THREAD = 1;
    private T[] _array;

    public MergeSort(final T[] array) {
        _array = Arrays.copyOf(array, array.length);
    }

    public T[] sort() {
        return sort(MAX_THREAD);
    }

    public T[] sort(int maxThread) {
        multiMerge(maxThread, 0, _array.length);
        return _array;
    }

    private void multiMerge(int maxThread, int begin, int end) {
        if (maxThread > 1 || maxThread <= 0) {
            int len = end-begin;
            Thread left, right;
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

            merge(begin, end, len);
        }
        else
            monoMerge(begin, end);
    }

    private void monoMerge(int begin, int end) {
        int len = end-begin;
        if (len != 1) {
            monoMerge(begin, begin+len/2);
            monoMerge(begin+len/2, end);
        }
        merge(begin, end, len);
    }

    //synchronized
    private void merge(int begin, int end, int len) {
        T[] left = Arrays.copyOfRange(_array, begin, begin+len/2);
        int i = begin, j = 0, k = begin+len/2;
        while (j<len/2 || k<end)
            if (k==end || j<len/2 && left[j].compareTo(_array[k])<0)
                _array[i++] = left[j++];
            else
                _array[i++] = _array[k++];
    }
}
