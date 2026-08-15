#include <unistd.h> // for fork(), execvp(), write(), close()
#include <sys/wait.h> // for waitpid()
#include <fcntl.h> // for open()
#include <unordered_set>

#include "../include/executor.hpp"
#include "../include/builtins.hpp"
#include "../include/parser.hpp"

std::vector<char*> Executor::convert_to_cType(const std::vector<std::string>& args){
      std::vector<char*> ans;
      for(int i = 0; i < args.size(); i++){
            ans.push_back(const_cast<char*>(args[i].c_str()));
            // c.str() gives the const char* but we need char*
            // so use typecast as const_cast<char*>
            // what's in the angular bracket gets converted into
      }
      ans.push_back(nullptr);
      return ans;
}

bool is_builtin(const std::string& program){
      std::unordered_set<std::string> builtins_st = {
            "echo",
            "type",
            "cd",
            "exit"
      };
      if(builtins_st.find(program) != builtins_st.end()) return true;
      else return false;
}

void exec_builtin(const std::vector<std::string>& args){
      Builtins builtins;
      if(args[0] == "echo"){
            builtins.echo(args);
            return;
      }
      if(args[0] == "type"){
            for(int i = 1; i < args.size(); i++){
                  builtins.type(args[i]);
            }
            return;
      }
      if(args[0] == "cd"){
            builtins.cd(args);
            return;
      }
      if(args[0] == "exit"){
            builtins.exit(0);
      }
}

void Executor::execute(const std::vector<Command>& commands){
      // this shell don't support '&&' and all other command
      // so 1 command can contain only 1 program
      // others will be considered as arguments to that program
      // except the redirection filenames

      for(size_t i = 0; i < commands.size(); i++){
            Command cur_cmd = commands[i];

            if(cur_cmd.args[0] == "cd" || cur_cmd.args[0] == "exit"){
                  exec_builtin(cur_cmd.args);
            }
            else{
                  pid_t process_id = fork();
                  if(process_id < 0){
                        // child not created successfully
                        throw std::runtime_error("trash: command can't be executed\n");
                  }

                  // child
                  else if(process_id == 0){
                        // always exit the child process explicitly
                        // like exit(0)/exit(1)

                        std::vector<int> fds(cur_cmd.redirections.size());    // to deal with the file opeanings
                        for(size_t j = 0; j < cur_cmd.redirections.size(); j++){
                              Redirection cur_redir = cur_cmd.redirections[j];

                              int flag;
                              if(cur_redir.rd_type == Redirection_type::Input){
                                    fds[j] = open(cur_redir.filename.c_str(), O_RDONLY);
                                    flag = dup2(fds[j], 0);
                              }
                              else if(cur_redir.rd_type == Redirection_type::Output){
                                    fds[j] = open(cur_redir.filename.c_str(), O_WRONLY | O_CREAT | O_TRUNC, 0644);
                                    flag = dup2(fds[j], 1);
                              }
                              else if(cur_redir.rd_type == Redirection_type::Append){
                                    fds[j] = open(cur_redir.filename.c_str(), O_WRONLY | O_CREAT | O_APPEND, 0644);
                                    flag = dup2(fds[j], 1);
                              }

                              // if file opeaning or redirecting fails
                              if(fds[j] == -1){
                                    perror(cur_redir.filename.c_str());
                                    exit(1);
                              }
                              if(flag == -1){
                                    perror("dup2");
                                    exit(1);
                              }
                              close(fds[j]);
                        }
                        // input and the op is set at this point, pass it to the program

                        if(is_builtin(cur_cmd.args[0])){
                              exec_builtin(cur_cmd.args);
                              exit(0);
                        }
                        else{
                              std::vector<char*> argv = convert_to_cType(cur_cmd.args);
                              execvp(cur_cmd.args[0].c_str(), argv.data());

                              std::cout << cur_cmd.args[0] << ": command not found\n";
                              exit(1); // exiting the child process
                        } 
                  }
                  
                  // parent
                  else{
                        waitpid(process_id, nullptr, 0);
                  }
            }
      }
}