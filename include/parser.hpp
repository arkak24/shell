#pragma once

#include <vector>
#include <string>

enum class Redirection_type{
      Input,      // <
      Output,     // >
      Append      // >>
};

struct Redirection{
      Redirection_type rd_type;
      std::string filename;
};

struct Command{
      std::vector<std::string> args;
      std::vector<Redirection> redirections;
};

class CommandParser{
      public:
            std::vector<Command> parse(const std::vector<std::string>& tokens);

      private:
            Redirection_type redir_type_token(const std::string& token);
};