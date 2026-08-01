#include "../include/shell.hpp"
#include "../include/command_parser.hpp"
#include "../include/builtins.hpp"

void Shell::run(){
      // Flush after every std::cout / std:cerr
      std::cout << std::unitbuf;
      std::cerr << std::unitbuf;

      std::string command;
      while(true){
            std::cout << "$ ";
            std::getline(std::cin, command);
            if(command == "exit") exit(0);

            CommandParser parser;
            std::vector<std::string> cmd_line_args = parser.slice_arguments(command);

            Builtins builtins;
            if(cmd_line_args[0] == "echo"){
                  builtins.echo(cmd_line_args[1]);
                  continue;
            }
            if(cmd_line_args[0] == "type"){
                  builtins.type(cmd_line_args[1]);
                  continue;
            }

            std::cout << command << ": command not found" << std::endl;
      }
}