all:
	g++ main.cpp src/builtins.cpp src/tokenizer.cpp src/parser.cpp src/executor.cpp src/shell.cpp -o shell
