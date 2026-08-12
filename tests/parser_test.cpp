#include <iostream>
#include <vector>
#include <string>

#include "../include/tokenizer.hpp"
#include "../include/parser.hpp"

int main(){
      CommandTokenizer ct;
      CommandParser cp;

      std::string cmd;
      std::cout << "Enter the command: ";
      std::getline(std::cin, cmd);

      std::vector<std::string> tokens = ct.tokenize(cmd);
      std::vector<Command> parsed = cp.parse(tokens);

      std::cout << "The tokens are:\n\n";
      for(auto it: tokens){
            std::cout << "[" << it << "]\n";
      }
      std::cout << "-------------------------------\n\n";

      std::cout << "Parsed tokens is:\n\n";

      // args
      // input file
      // output file
      // bool append

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

            std::cout << "\tinput file: \"" << temp.input_file << "\"\n";
            std::cout << "\toutput file: \"" << temp.output_file << "\"\n";

            std::cout << std::boolalpha;
            std::cout << "\tappend: \"" << temp.append_output << "\"\n";
            std::cout << std::noboolalpha;

            std::cout << "}\n\n";
      }
      std::cout << "-------------------------------\n\n";

      return 0;
}