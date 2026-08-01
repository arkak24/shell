#include "../include/builtins.hpp"

void Builtins::type(std::string& argument){

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

void Builtins::echo(std::string& argument){
      std::cout << argument << std::endl;
      return;
}
