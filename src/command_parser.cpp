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

// this don't handle the cases where the user inputs any
// string with invalid quotations
// like opeaning and closing quote errors

# define OPEN true
# define CLOSED false

std::vector<std::string> CommandTokenizer::tokenize(const std::string& command_line){
      std::vector<std::string> tokens;
      if(check_just_spaces(command_line) || command_line.size() == 0) return tokens;

      // add operators here if implemented
      std::unordered_set<char> operators = {'>', '<', '|'};
      std::unordered_set<std::string> d_operators = {">>"};

      bool double_q_flag = CLOSED;
      bool single_q_flag = CLOSED;
      bool word_started = false;

      std::string word = "";
      for(int i = 0; i < command_line.size(); i++){
            // 3 cases: normal, inside single quote, inside double quote

            if(double_q_flag == CLOSED && single_q_flag == CLOSED){
                  // normal case
                  if(command_line[i] == '"'){
                        double_q_flag = OPEN;
                        word_started = true;
                        continue;
                  }
                  if(command_line[i] == '\''){
                        single_q_flag = OPEN;
                        word_started = true;
                        continue;
                  }

                  if(command_line[i] == ' ' || operators.find(command_line[i]) != operators.end()){
                        if(word_started == true){
                              tokens.push_back(word);
                              word.clear();
                              word_started = false;
                        }
                  }
               
                  if(command_line[i] != ' ' && operators.find(command_line[i]) == operators.end()){
                        word += command_line[i];
                        word_started = true;
                  }

                  // if it is a operator
                  if(operators.find(command_line[i]) != operators.end()){
                        // check the double operator or single opertor
                        if(i != command_line.size()-1){
                              std::string temp = convert_char_to_str(command_line[i]) + convert_char_to_str(command_line[i+1]);

                              // valid double operator
                              if(d_operators.find(temp) != d_operators.end()){
                                    tokens.push_back(temp);
                                    i++;
                                    continue;
                              }

                              // it's a single character operator, push as it is
                              else{
                                    tokens.push_back(convert_char_to_str(command_line[i]));
                              }
                        }
                        else{
                              tokens.push_back(convert_char_to_str(command_line[i]));
                        }
                  }
            }
            else if(double_q_flag == CLOSED && single_q_flag == OPEN){
                  // inside single quotes
                  // everything adds to the word as it is until a [']
                  if(command_line[i] == '\''){
                        single_q_flag = CLOSED;
                        continue;
                  }
                  else{
                        word += command_line[i];
                  }
            }
            else if(double_q_flag == OPEN && single_q_flag == CLOSED){
                  // inside the double quotes
                  // everything adds to the word as it is until a ["]
                  if(command_line[i] == '"'){
                        double_q_flag = CLOSED;
                        continue;
                  }
                  else{
                        word += command_line[i];
                  }
            }
      }
      
      if(!word.empty() || word_started){
            tokens.push_back(word);
      }

      return tokens;
}

# undef OPEN
# undef CLOSED
