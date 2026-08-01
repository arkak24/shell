#include "../include/shell.hpp"
#include "../include/command_parser.hpp"
#include "../include/builtins.hpp"
#include "../include/executor.hpp"

void Shell::run(){
      // Flush after every std::cout / std:cerr
      std::cout << std::unitbuf;
      std::cerr << std::unitbuf;

      Executor executor;
      CommandParser parser;

      std::string command;
      while(true){
            std::cout << "$ ";
            std::getline(std::cin, command);
            if(command == "exit") exit(0);

            std::vector<std::string> cmd_line_args = parser.slice_arguments(command);
            executor.execute(cmd_line_args);
      }
}