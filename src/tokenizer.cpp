#include "../include/tokenizer.hpp"

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

std::unordered_set<std::string> CommandTokenizer::operators = {
      ">",
      ">>",
      "<",
      "|"
};

bool CommandTokenizer::is_operator(const std::string& str){
      if(operators.find(str) != operators.end()){
            return true;
      }
      else return false;
}

# define OPEN true
# define CLOSED false

std::vector<std::string> CommandTokenizer::tokenize(const std::string& command_line){
      std::vector<std::string> tokens;
      if(check_just_spaces(command_line) || command_line.size() == 0) return tokens;

      bool double_q_flag = CLOSED;
      bool single_q_flag = CLOSED;
      bool word_started = false;

      std::string word = "";
      for(int i = 0; i < command_line.size(); i++){
            std::string curr_char = convert_char_to_str(command_line[i]);
            // 3 cases: normal, inside single quote, inside double quote

            if(double_q_flag == CLOSED && single_q_flag == CLOSED){
                  // normal case
                  if(curr_char == "\""){
                        double_q_flag = OPEN;
                        word_started = true;
                        continue;
                  }
                  if(curr_char == "\'"){
                        single_q_flag = OPEN;
                        word_started = true;
                        continue;
                  }

                  if(curr_char == " " || is_operator(curr_char)){
                        if(word_started == true){
                              tokens.push_back(word);
                              word.clear();
                              word_started = false;
                        }
                  }
               
                  if(curr_char != " " && !is_operator(curr_char)){
                        word += curr_char;
                        word_started = true;
                  }

                  // if it is a operator
                  if(is_operator(curr_char)){
                        // check the double operator or single opertor
                        if(i != command_line.size()-1){
                              std::string temp = curr_char + convert_char_to_str(command_line[i+1]);

                              // valid double operator
                              if(is_operator(temp)){
                                    tokens.push_back(temp);
                                    i++;
                                    continue;
                              }

                              // it's a single character operator, push as it is
                              else{
                                    tokens.push_back(curr_char);
                              }
                        }
                        else{
                              tokens.push_back(curr_char);
                        }
                  }
            }
            else if(double_q_flag == CLOSED && single_q_flag == OPEN){
                  // inside single quotes
                  // everything adds to the word as it is until a [']
                  if(curr_char == "\'"){
                        single_q_flag = CLOSED;
                        continue;
                  }
                  else{
                        word += curr_char;
                  }
            }
            else if(double_q_flag == OPEN && single_q_flag == CLOSED){
                  // inside the double quotes
                  // everything adds to the word as it is until a ["]
                  if(curr_char == "\""){
                        double_q_flag = CLOSED;
                        continue;
                  }
                  else{
                        word += curr_char;
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
