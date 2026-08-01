#include "../include/command_parser.hpp"

bool check_just_spaces(const std::string& str){
      for(int i = 0; i < str.size(); i++){
            if(str[i] != ' ') return false;
      }
      return true;
}

std::vector<std::string> CommandParser::slice_arguments(const std::string& command){
      std::vector<std::string> args;
      if(check_just_spaces(command) || command.size() == 0) return args;

      std::string temp = "";
      for(int i = 0; i < command.size(); i++){
            if(command[i] != ' ') temp += command[i];
            if(command[i] == ' ' || i == command.size()-1){
                  if(temp.size() != 0){
                        args.push_back(temp);
                        temp = "";
                  }
            }
      }
      return args;
}
