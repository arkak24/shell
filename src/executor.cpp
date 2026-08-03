#include <unistd.h> // for fork() and execvp()
#include <sys/wait.h> // for waitpid()

#include "../include/executor.hpp"
#include "../include/builtins.hpp"

std::vector<char*> convert_to_cType(const std::vector<std::string>& cmd_line_args){
      std::vector<char*> ans;
      for(int i = 0; i < cmd_line_args.size(); i++){
            ans.push_back(const_cast<char*>(cmd_line_args[i].c_str()));
            // c.str() gives the const char* but we need char*
            // so use typecast as const_cast<char*>
            // what's in the angular bracket gets converted into
      }
      ans.push_back(nullptr);
      return ans;
}

void Executor::execute(const std::vector<std::string>& cmd_line_args){
      Builtins builtins;
      if(cmd_line_args[0] == "echo"){
            builtins.echo(cmd_line_args);
            return;
      }
      if(cmd_line_args[0] == "type"){
            for(int i = 1; i < cmd_line_args.size(); i++){
                  builtins.type(cmd_line_args[i]);
            }
            return;
      }
      if(cmd_line_args[0] == "cd"){
            builtins.cd(cmd_line_args);
            return;
      }

      // for the commands that are not builtin
      pid_t process_id = fork();
      if(process_id < 0){
            std::cout << "failed in creating child" << std::endl;
            return;
      }
      else if(process_id == 0){
            // child process
            // we replace the current shell process with the process user want to execute
            // execvp() takes (char* argv[])
            // .c_str() returns (const char*)
            // Because the string owns its memory. C++ lets you read it, but not modify it through the pointer.

            std::vector<char*> argv = convert_to_cType(cmd_line_args);
            execvp(cmd_line_args[0].c_str(), argv.data());
            // .data() returns the prointer to the first element in the vector

            // If execvp() succeeds, it never returns. Your child process becomes ls, pwd, etc.
            // If it returns, that means it failed.
            // so we give a fail statement

            std::cout << cmd_line_args[0] << ": command not found\n";
            exit(1); // exiting the child process
      }
      else{
            // parent process
            waitpid(process_id, nullptr, 0);
      }
}