FLAGS = -std=c++17 -Wpedantic -Wall -Wextra -Winit-self -Winline -Wconversion -Weffc++ -Wstrict-null-sentinel -Wold-style-cast -Wnoexcept -Wctor-dtor-privacy -Woverloaded-virtual -Wconversion -Wsign-promo -Wzero-as-null-pointer-constant


all: Java Cpp

Java: Java/class/MergeSortMain.class
Cpp: Cpp/out/mergeSort


Java/class/MergeSortMain.class: Java/class/sorting/MergeSort.class
	mkdir -p Java/class
	javac -cp Java/commons-cli-1.4/*:Java/class/ Java/MergeSortMain.java -d Java/class

Java/class/sorting/MergeSort.class:
	mkdir -p Java/class/sorting
	javac Java/sorting/MergeSort.java -d Java/class


Cpp/out/mergeSort: Cpp/out/parsing.o
	mkdir -p Cpp/out
	g++ $(FLAGS) -I Cpp/sorting/ -I Cpp/parsing/ -I Cpp/gopt-10.0/ Cpp/out/* Cpp/main.cpp -pthread -B Cpp/out/ -o Cpp/out/mergeSort

Cpp/out/gopt.o:
	mkdir -p Cpp/out
	g++ $(FLAGS) -c Cpp/gopt-10.0/gopt.c -o Cpp/out/gopt.o

Cpp/out/gopt-errors.o:
	mkdir -p Cpp/out
	g++ $(FLAGS) -c Cpp/gopt-10.0/gopt-errors.c -o Cpp/out/gopt-errors.o

Cpp/out/parsing.o: Cpp/out/gopt.o Cpp/out/gopt-errors.o
	mkdir -p Cpp/out
	g++ $(FLAGS) -c Cpp/parsing/parsing.cpp -I Cpp/gopt-10.0/ -o Cpp/out/parsing.o


clean:
	rm -Rf Java/class
	rm -Rf Cpp/out

