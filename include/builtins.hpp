#pragma once

#include <iostream>
#include <vector>
#include <string>
#include <filesystem>
#include <cstdlib>
#include <unistd.h>

#include "./shell.hpp"

class Builtins{
      public:
            void type(const std::string& argument);
            void echo(const std::vector<std::string>& cmd_line_args);
            void cd(const std::vector<std::string>& cmd_line_args);
            void pwd();
            void history(int n = curr_session_histories.size());
            void exit(int exit_code = 0);
};