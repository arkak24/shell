#pragma once

#include <vector>
#include <string>

bool check_just_spaces(const std::string& str);

class CommandParser{
      public:
            std::vector<std::string> slice_arguments(const std::string& command);
};