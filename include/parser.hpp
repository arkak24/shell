#pragma once

#include <vector>
#include <string>

struct Command{
      std::vector<std::string> args;

      std::string input_file;
      std::string output_file;

      bool append_output = false;
};

class CommandParser{
      public:
            std::vector<Command> parse(const std::vector<std::string>& tokens);

      private:
            bool is_operator(const std::string& token);
};