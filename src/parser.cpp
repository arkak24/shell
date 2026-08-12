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

std::vector<Command> CommandParser::parse(const std::vector<std::string>& tokens){
      std::vector<Command> parsed_command;
      Command sub_cmd;
      std::string temp = "";

      for(int i = 0; i < tokens.size(); i++){
            if(!is_operator(tokens[i])){
                  sub_cmd.args.push_back(tokens[i]);
            }
            else{
                  // ERROR CASES:
                  // operator in front or end
                  // one operator after another
                  if(
                        i == tokens.size()-1 || tokens[i+1] == "|" || i == 0 || tokens[i-1] == "|" ||
                        operators.find(tokens[i+1]) != operators.end()
                  ){
                        throw std::runtime_error("Invalid command\n");
                  }

                  if(tokens[i] == "|"){
                        parsed_command.push_back(sub_cmd);
                        sub_cmd = {};
                  }
                  else{
                        // if it is not the last token
                        // then it should redirect to any file
                        // *empty input/output string means it is stdin/stdout
                        if(tokens[i] == ">"){
                              sub_cmd.input_file = "";
                              sub_cmd.output_file = tokens[i+1];
                              sub_cmd.append_output = false;
                              i++;
                        }
                        else if(tokens[i] == "<"){
                              sub_cmd.input_file = tokens[i+1];
                              sub_cmd.output_file = "";
                              sub_cmd.append_output = false;
                              i++;
                        }
                        else if(tokens[i] == ">>"){
                              sub_cmd.input_file = "";
                              sub_cmd.output_file = tokens[i+1];
                              sub_cmd.append_output = true;
                              i++;
                        }
                  }
            }
      }
      parsed_command.push_back(sub_cmd);
      return parsed_command;
}