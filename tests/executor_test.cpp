// this don't work
// according to the current flow
// have to modify so that it can take parsed cmd

#include <iostream>
#include <vector>
#include <string>

#include "../include/tokenizer.hpp"
#include "../include/parser.hpp"
#include "../include/executor.hpp"

int main(){
      CommandTokenizer ct;
      CommandParser cp;
      Executor exc;

      std::string cmd;
      std::cout << "Enter the command: ";
      std::getline(std::cin, cmd);

      std::vector<std::string> tokens = ct.tokenize(cmd);
      if(tokens.size() == 0){
            std::cout << "Empty command\n";
            return 0;
      }

      exc.execute(tokens);

      return 0;
}