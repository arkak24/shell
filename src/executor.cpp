#include "../include/executor.hpp"
#include "../include/builtins.hpp"

void Executor::execute(const std::vector<std::string>& cmd_line_args){
      Builtins builtins;
      if(cmd_line_args[0] == "echo"){
            builtins.echo(cmd_line_args[1]);
            return;
      }
      if(cmd_line_args[0] == "type"){
            builtins.type(cmd_line_args[1]);
            return;
      }
      std::cout << cmd_line_args[0] << ": command not found" << std::endl;
}