#pragma once

#include <vector>
#include <string>

#include "./parser.hpp"

class Executor{
      public:
            void execute(const std::vector<Command>& commands);
            std::vector<char*> convert_to_cType(const std::vector<std::string>& cmd_line_args);
};