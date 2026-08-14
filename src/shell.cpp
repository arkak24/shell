#include <stdexcept>

#include "../include/shell.hpp"
#include "../include/tokenizer.hpp"
#include "../include/parser.hpp"
#include "../include/builtins.hpp"
#include "../include/executor.hpp"

#define OPEN true
#define CLOSED false

// *** the incomplete pipe also make you complete the command
// like incomplete quotes
// not implemented yet
std::string read_command(){
      std::string str = "";
      char ch;

      bool sq = CLOSED;
      bool dq = CLOSED;

      while(true){
            ch = getchar();

            if(sq == CLOSED && dq == CLOSED && ch == '\n'){
                  return str;
            }
            else if(sq == CLOSED && dq == CLOSED && ch == '\''){
                  sq = OPEN;
                  str += ch;
                  continue;
            }
            else if(sq == CLOSED && dq == CLOSED && ch == '"'){
                  dq = OPEN;
                  str += ch;
                  continue;
            }
            else if(sq == CLOSED && dq == CLOSED){
                  str += ch;
                  continue;
            }

            if(sq == OPEN && dq == CLOSED){
                  if(ch == '\n'){
                        str += ch;
                        std::cout << "> ";
                  }
                  else if(ch == '\''){
                        str += ch;
                        sq = CLOSED;
                  }
                  else{
                        str += ch;
                  }
            }
            else if(sq == CLOSED && dq == OPEN){
                  if(ch == '\n'){
                        str += ch;
                        std::cout << "> ";
                  }
                  else if(ch == '"'){
                        str += ch;
                        dq = CLOSED;
                  }
                  else{
                        str += ch;
                  }
            }
      }
      return str;
}

void Shell::run(){
      // Flush after every std::cout / std:cerr
      std::cout << std::unitbuf;
      std::cerr << std::unitbuf;

      Executor executor;
      CommandTokenizer tokenizer;
      CommandParser parser;

      std::string command_line;
      while(true){
            std::cout << "$ ";

            // std::getline(std::cin, command_line);
            // just doing this would not enable the
            // inclomplete qoute case

            command_line = read_command();
            
            // tokenize the command line
            std::vector<std::string> tokens = tokenizer.tokenize(command_line);
            if(tokens.size() == 0) continue;
            else if(tokens.back() == "-1"){
                  std::cout << "wrong command format\n";
                  continue;
            }

            // parse the tokens
            try{
                  std::vector<Command> commands = parser.parse(tokens);
            }
            catch (const std::runtime_error& e) {
                  std::cerr << e.what() << '\n';
            }

            // pass the parsed commands to the execute
            // executor.execute(tokens);
      }
}