#include "../include/shell.hpp"
#include "../include/command_parser.hpp"
#include "../include/builtins.hpp"
#include "../include/executor.hpp"

void Shell::run(){
      // Flush after every std::cout / std:cerr
      std::cout << std::unitbuf;
      std::cerr << std::unitbuf;

      Executor executor;
      CommandTokenizer tokenizer;

      std::string command_line;
      while(true){
            std::cout << "$ ";
            std::getline(std::cin, command_line);
            if(command_line == "exit") exit(0);

            std::vector<std::string> cmd_line_args = tokenizer.tokenize(command_line);
            if(cmd_line_args.size() == 0) continue;
            else if(cmd_line_args.back() == "-1"){
                  std::cout << "wrong command format\n";
                  continue;
            }
            
            executor.execute(cmd_line_args);
      }
}