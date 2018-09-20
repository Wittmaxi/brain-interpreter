all:
	g++ ./src/*.cpp -g -o ./brainfuck -std=c++17
	./brainfuck test_file.bf