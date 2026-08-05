#pragma once

#include <iostream>
#include <vector>
#include <string>
#include <filesystem>
#include <cstdlib>
#include <unistd.h>

class Builtins{
      public:
            void type(const std::string& argument);
            void echo(const std::vector<std::string>& cmd_line_args);
            void cd(const std::vector<std::string>& cmd_line_args);
            void pwd();
            void exit(int n);
};