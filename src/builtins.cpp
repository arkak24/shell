#include "../include/builtins.hpp"

#include <unistd.h>

void Builtins::type(const std::string& argument){
      if(argument == "echo" || argument == "exit" || argument == "type"){
            std::cout << argument << " is a shell builtin\n";
            return;
      }

      const char* pathStr = std::getenv("PATH");      // return a c style string, so dont use normal string
      if(pathStr == nullptr) return;
      else{
            std::string path_stream = pathStr;
            std::string sub_path = "";
            for(int i = 0; i < path_stream.size(); i++){
                  if(path_stream[i] != ':') sub_path += path_stream[i];
                  if(path_stream[i] == ':' || i == path_stream.size()-1){
                        std::filesystem::path cmd_path = std::filesystem::path(sub_path) / argument;
                        if(std::filesystem::exists(cmd_path) && access(cmd_path.c_str(), X_OK) == 0){
                              std::cout << argument << " is " << cmd_path.string() << std::endl;
                              // filesystem uses the str with "..."
                              // we dont want that, we want just the string
                              // so use .string() to convert the filesystem obj to string obj
                              return;
                        }
                        sub_path = "";
                  }
            }
            std::cout << argument << ": not found\n";
      }
      return;
}

void Builtins::echo(const std::vector<std::string>& cmd_line_args){
      // wrong, more modifications needed
      std::cout << cmd_line_args[1] << std::endl;
      return;
}

void Builtins::cd(const std::vector<std::string>& cmd_line_args){
      if(cmd_line_args.size() > 2){
            std::cout << "cd: too many arguments\n";
            return;
      }
      // chdir() returns 0 if changing dir is succeddful else returns -1
      else if(cmd_line_args.size() == 1 || cmd_line_args[1] == "~"){
            // to home
            chdir(getenv("HOME"));
            return;
      }
      else{
            if(chdir(cmd_line_args[1].c_str()) != 0){
                  perror("cd");
            }
            return;
      }
      // [cd -] not working check out ***
}

void Builtins::pwd(){
      char path[1024];
      // if successful getcwd() returns the same pointer(as path)
      // else it returns nullptr
      if(getcwd(path, sizeof(path)) != nullptr){
            std::cout << path << "\n";
      }
      else{
            perror("pwd");
      }
      // with filesystem we can do like std::filesystem::current_path()
      // but getpwd() is the standard POSIX API
}

void Builtins::exit(int n){
      std::exit(n);
}