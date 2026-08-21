#pragma once

#include <iostream>
#include <vector>
#include <string>
#include <filesystem>
#include <cstdlib>
#include <unistd.h>
#include <unordered_set>

#include "./shell.hpp"

class Builtins{
      private:
            static std::unordered_set<std::string> builtins_set;

      public:
            static bool is_builtin(const std::string& program);

            void type(const std::string& argument);
            void echo(const std::vector<std::string>& cmd_line_args);
            void cd(const std::vector<std::string>& cmd_line_args);
            void pwd();
            void history(int n = curr_session_histories.size());
            void exit(int exit_code = 0);
};