#pragma once

#include <vector>
#include <string>
#include <unordered_set>

bool check_just_spaces(const std::string& str);

class CommandTokenizer{
      public:
            static std::unordered_set<std::string> operators;
            static bool is_operator(const std::string& str);

            std::vector<std::string> tokenize(const std::string& command_line);
};