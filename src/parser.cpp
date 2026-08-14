#include <vector>
#include <string>
#include <unordered_set>
#include <stdexcept>

#include "../include/parser.hpp"

const std::unordered_set<std::string> operators = {">", "<", ">>", "|"};

bool CommandParser::is_operator(const std::string& token){
      if(operators.find(token) != operators.end()){
            return true;
      }
      else return false;
}

Redirection_type CommandParser::redir_type_token(const std::string& token){
      if(token == "<") return Redirection_type::Input;
      if(token == ">") return Redirection_type::Output;
      if(token == ">>") return Redirection_type::Append;
      throw std::runtime_error("trash: invalid operator\n\n");
}

std::vector<Command> CommandParser::parse(const std::vector<std::string>& tokens){
      std::vector<Command> parsed_command;
      Command sub_cmd;

      for(int i = 0; i < tokens.size(); i++){
            if(!is_operator(tokens[i])){
                  sub_cmd.args.push_back(tokens[i]);
            }
            else{
                  // pipe at end is valid in real bash,
                  // the shell forces to complete the whole command
                  // just like the incomplete qoute case
                  // although handle that case while taking the input function
                  if(tokens[i] == "|"){
                        if(i == 0 || (i != tokens.size()-1 && tokens[i+1] == "|")){
                              // error case, pipes cant be consecutive
                              throw std::runtime_error("trash: syntax error near unexpected token '|'");
                        }

                        // REMOVE THIS CONDITION LATER, WHEN THE INPUT IS REDESIGNED
                        if(i == tokens.size()-1) throw std::runtime_error("Pipe can't be at last\n");

                        parsed_command.push_back(sub_cmd);
                        sub_cmd = {};
                  }

                  /*
                  if the operator is not a pipe
                  one operator after another is invalid
                  redirecting operator at the end is invalid
                  redirecting operator at the front is valid
                  *empty input/output string means it is stdin/stdout
                  */
                  else{
                        if(i == tokens.size()-1){
                              throw std::runtime_error("trash: syntax error near unexpected token 'newline'\n");
                        }
                        if(i != tokens.size()-1 && operators.find(tokens[i+1]) != operators.end()){
                              std::string tmp = "trash: syntax error near unexpected token '" + tokens[i+1] + "'\n";
                              throw std::runtime_error(tmp);
                        }
                        Redirection_type rt = redir_type_token(tokens[i]);
                        std::string fn = tokens[i+1];
                        sub_cmd.redirections.push_back({rt, fn});
                        i++;
                  }
            }
      }
      parsed_command.push_back(sub_cmd);
      return parsed_command;
}