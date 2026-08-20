#include "../include/builtins.hpp"
#include "../include/shell.hpp"

#include <fcntl.h>
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
      std::string str = "";
      for(size_t i = 1; i < cmd_line_args.size(); i++){
            str += cmd_line_args[i];
            if(i != cmd_line_args.size()-1) str += " ";
            else str += "\n";
      }
      write(1, str.c_str(), str.size());
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

void Builtins::history(int n){
      // the default val is given in the hpp file, so don't here
      // if successful write the histories vector into the stdout
      // for now don't take any args like the number of lines to print

      char* home = getenv("HOME");
      std::string history_address = std::string(home) + "/.trash_history";

      int hist_fd = open(history_address.c_str(), O_RDONLY, 0644);
      char buffer[4096];      // 4KB buffer
      ssize_t size_read = read(hist_fd, buffer, sizeof(buffer));
      while(size_read > 0){
            write(STDOUT_FILENO, buffer, size_read);
            size_read = read(hist_fd, buffer, sizeof(buffer));
      }
      close(hist_fd);

      for(size_t i = 0; i < curr_session_histories.size(); i++){
            std::cout << curr_session_histories[i] << "\n";
      }
      return;
}

void Builtins::exit(int exit_code){
      if(curr_session_histories.size() == 0) std::exit(exit_code);

      std::string history_str = "";
      for(size_t i = 0; i < curr_session_histories.size(); i++){
            history_str += curr_session_histories[i];
            history_str += "\n";
      }

      char* home = getenv("HOME");
      std::string history_address = std::string(home) + "/.trash_history";

      int hist_fd = open(history_address.c_str(), O_CREAT | O_APPEND | O_WRONLY, 0644);
      write(hist_fd, history_str.c_str(), history_str.size());
      close(hist_fd);

      std::exit(exit_code);
}