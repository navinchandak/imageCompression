all: main.o JPEG_Compression.o entropy.o BinaryTree.o FileConvert.o
	g++ main.o JPEG_Compression.o entropy.o BinaryTree.o FileConvert.o -o JPEG -lX11 -lpthread

main.o: main.cpp
	g++ -c main.cpp 

JPEG_Compression.o: JPEG_Compression.cpp
	g++ -c JPEG_Compression.cpp

entropy.o: entropy.cpp
	g++ -c entropy.cpp

BinaryTree.o: BinaryTree.cpp
	g++ -c BinaryTree.cpp

FileConvert.o: FileConvert.cpp
	g++ -c FileConvert.cpp
