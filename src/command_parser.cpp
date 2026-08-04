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

      std::unordered_set<char> operators = {'>', '<', '|', '&', ';'};
      std::unordered_set<std::string> d_operators = {"&&", "||", "<<", ">>", ";;", "<&", ">&"};

      bool double_q_flag = CLOSED;
      bool single_q_flag = CLOSED;

      std::string word = "";
      for(int i = 0; i < command_line.size(); i++){
            // 3 cases: normal, inside single quote, inside double quote

            if(double_q_flag == CLOSED && single_q_flag == CLOSED){
                  // normal case
                  if(command_line[i] == '"'){
                        double_q_flag = OPEN;
                        continue;
                  }
                  if(command_line[i] == '\''){
                        single_q_flag = OPEN;
                        continue;
                  }

                  if(command_line[i] == ' ' || operators.find(command_line[i]) != operators.end()){
                        if(word.size() != 0){
                              tokens.push_back(word);
                              word.clear();
                        }
                  }
               
                  if(command_line[i] != ' ' && operators.find(command_line[i]) == operators.end()){
                        word += command_line[i];
                  }

                  // check if it is a operator
                  if(operators.find(command_line[i]) != operators.end()){
                        // if operator is the first element then the command is invalid
                        if(tokens.empty()){
                              return {"-1"};
                        }

                        // check the double operator or single opertor
                        if(i != command_line.size()-1){
                              std::string temp = convert_char_to_str(command_line[i]) + convert_char_to_str(command_line[i+1]);

                              // valid double operator
                              if(d_operators.find(temp) != d_operators.end()){
                                    tokens.push_back(temp);
                                    i++;
                                    continue;
                              }

                              // invalid double operator
                              else if(operators.find(command_line[i+1]) != operators.end()){
                                    return {"-1"};
                              }

                              // it's a single character operator, push as it is
                              else{
                                    tokens.push_back(convert_char_to_str(command_line[i]));
                              }
                        }
                        else{
                              // operator can't be at last
                              // but we have to handle double operator at last anyhow in the parser
                              // so dedicate that work to parser only
                              // okay for now, do that if necessary

                              return {"-1"};
                              // check if there is any double character operator in the end of the command
                        }


                  }
            }
            else if(double_q_flag == CLOSED && single_q_flag == OPEN){
                  // inside single quotes
                  // everything adds to the word as it is until a [']
                  if(command_line[i] == '\''){
                        single_q_flag = CLOSED;
                        if(i == command_line.size()-1){
                              tokens.push_back(word);
                              word.clear();
                        }
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
                        if(i == command_line.size()-1){
                              tokens.push_back(word);
                              word.clear();
                        }
                        continue;
                  }
                  else{
                        word += command_line[i];
                  }
            }
            // else if(double_q_flag == OPEN && single_q_flag == OPEN){
                  // weired case, have to see which is opened at first,
                  // whichever it is gets the priority
                  // this case never comes to play***
            // }
      }
      if(!word.empty()){
            tokens.push_back(word);
      }

      // handle this case later
      // bash don't work like this
      // bash keeps on asking until you make the shole string right
      if(single_q_flag == OPEN || double_q_flag == OPEN){
            return {"-1"};
      }

      return tokens;
}

# undef OPEN
# undef CLOSED
