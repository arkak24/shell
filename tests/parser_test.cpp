#include <iostream>
#include <vector>
#include <string>

#include "../include/tokenizer.hpp"
#include "../include/parser.hpp"

std::string helper(Redirection_type rt){
      switch(rt){
            case Redirection_type::Input:
                  return "<";
                  break;
            
            case Redirection_type::Output:
                  return ">";
                  break;

            case Redirection_type::Append:
                  return ">>";
                  break;

            default:
                  return "-1";
      }
}

int main(){
      CommandTokenizer ct;
      CommandParser cp;

      std::string cmd;
      std::cout << "Enter the command: ";
      std::getline(std::cin, cmd);

      std::vector<std::string> tokens = ct.tokenize(cmd);
      if(tokens.size() == 0){
            std::cout << "Empty command\n";
            return 0;
      }

      std::vector<Command> parsed = cp.parse(tokens);

      std::cout << "The tokens are:\n\n";
      for(auto it: tokens){
            std::cout << "[" << it << "]\n";
      }
      std::cout << "-------------------------------\n\n";

      std::cout << "Parsed tokens is:\n\n";

      for(int i = 0; i < parsed.size(); i++){
            Command temp = parsed[i];
            std::cout << "Command [" << i << "]: {\n";

            // args
            std::cout << "\targs: {";
            for(int i = 0; i < temp.args.size(); i++){
                  std::cout << "\"" << temp.args[i] << "\"";
                  if(i < temp.args.size()-1) std::cout << ", ";
                  else std::cout << "}\n";
            }

            std::cout << "\tredirections: {";
            for(int i = 0; i < temp.redirections.size(); i++){
                  std::cout << "{\"" << helper(temp.redirections[i].rd_type) << "\": \"" << temp.redirections[i].filename << "\"}";
                  if(i < temp.redirections.size()-1) std::cout << ", ";
                  else std::cout << "}\n";
            }

            std::cout << "}\n\n";
      }
      std::cout << "-------------------------------\n\n";

      return 0;
}