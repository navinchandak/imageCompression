all: program
program: .encrypt.o .decrypt.o .index.o .vectorf.o .neuron.o .file.o .huffman.o .tree.o
		g++ -std=c++0x -g .encrypt.o .decrypt.o .index.o .vectorf.o .neuron.o .file.o .huffman.o .tree.o -lpthread -lX11 -o neural
.encrypt.o : encrypt.cpp
		g++ -std=c++0x -c encrypt.cpp -g -o .encrypt.o
.decrypt.o : decrypt.cpp
		g++ -std=c++0x -c decrypt.cpp -g -o .decrypt.o
.index.o : index.cpp
		g++ -std=c++0x -c index.cpp -g -o .index.o
.file.o : file.cpp
		g++ -std=c++0x -c file.cpp -g -o .file.o
.neuron.o : neuron.cpp
		g++ -std=c++0x -c neuron.cpp -g -o .neuron.o
.vectorf.o : vectorf.cpp
		g++ -std=c++0x -c vectorf.cpp -g -o .vectorf.o
.huffman.o : huffman.cpp
		g++ -std=c++0x -c huffman.cpp -g -o .huffman.o
.tree.o : tree.cpp
		g++ -std=c++0x -c tree.cpp -g -o .tree.o
