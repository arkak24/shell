#pragma once

#include <vector>
#include <string>

bool check_just_spaces(const std::string& str);

class CommandTokenizer{
      public:
            std::vector<std::string> tokenize(const std::string& command_line);
};