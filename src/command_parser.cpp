#include "../include/command_parser.hpp"

#include <unordered_set>

bool check_just_spaces(const std::string& str){
      for(int i = 0; i < str.size(); i++){
            if(str[i] != ' ') return false;
      }
      return true;
}

std::string convert_char_to_str(const char& c){
      std::string str = "";
      str += c;
      return str;
}

std::vector<std::string> CommandParser::slice_arguments(const std::string& command){
      std::vector<std::string> args;
      if(check_just_spaces(command) || command.size() == 0) return args;

      std::unordered_set<char> operators = {'>', '<', '|'};

      // std::unordered_set<std::string> d_operators = {};
      // for now just handle the ">>" case

      // bool double_q_flag = false;
      // bool single_q_flag = false;

      std::string word = "";
      for(int i = 0; i < command.size(); i++){
            if(command[i] != ' ' && operators.find(command[i]) == operators.end()){
                  word += command[i];
            }
            if(command[i] == ' ' || i == command.size()-1 || operators.find(command[i]) != operators.end()){
                  if(word.size() != 0){
                        args.push_back(word);
                        word = "";
                  }
            }
            // >> case
            // if >> at the end then also error, handle after tokenize the whole thing
            if(command[i] == '>' && (i != command.size()-1 && command[i+1] == '>')){
                  args.push_back(">>");
                  i++;
            }
            else if(operators.find(command[i]) != operators.end()){
                  if (args.empty()) {
                        // error: operator cannot be the first token
                        args = {"-1"};
                        return args;

                  }
                  else if (args.back() == ">>" || i == command.size() - 1 || operators.find(args.back().back()) != operators.end()) {
                        // error case
                        // return from here
                        args = {"-1"};
                        return args;
                  }
                  else {
                        args.push_back(convert_char_to_str(command[i]));
                  }
            }
            // " ", ' ', " ', ' ", " ' ' ", ' " " '
      }
      return args;
}
