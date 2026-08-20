#include <unistd.h> // for fork(), execvp(), write(), close()
#include <sys/wait.h> // for waitpid()
#include <fcntl.h> // for open()
#include <unordered_set>
#include <utility>

#include "../include/executor.hpp"
#include "../include/builtins.hpp"
#include "../include/parser.hpp"
#include "../include/shell.hpp"

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
            "history",
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
      if(args[0] == "history"){
            builtins.history();
            return;
      }
      if(args[0] == "exit"){
            builtins.exit();
      }
}

void Executor::execute(const std::vector<Command>& commands){
      // this shell don't support '&&' and all other command
      // so 1 command can contain only 1 program
      // others will be considered as arguments to that program
      // except the redirection filenames

      std::vector<std::pair<int, int>> pipes(commands.size()-1);
      for(size_t i = 0; i < commands.size()-1; i++){
            int fds[2];
            if(pipe(fds) == -1){
                  // the kernel gurantees that the retured fds are valid and distinct
                  perror("pipe");
                  return;
            }
            pipes[i] = {fds[1], fds[0]};
            // pushing them flipped cause it's easier to visualise
            // fds[0] -> read end
            // fds[1] -> write end
      }

      std::vector<size_t> pids;

      for(size_t i = 0; i < commands.size(); i++){
            Command cur_cmd = commands[i];

            if((cur_cmd.args[0] == "cd" && commands.size() == 1) || cur_cmd.args[0] == "exit"){
                  exec_builtin(cur_cmd.args);
            }
            else{
                  pid_t process_id = fork();
                  if(process_id < 0){
                        // child not created successfully
                        perror("fork");
                        return;
                  }

                  // child
                  else if(process_id == 0){
                        // always exit the child process explicitly
                        // like exit(0)/exit(1)

                        if(i > 0){  // fixing the stdin of the child
                              dup2(pipes[i-1].second, 0);
                        }
                        if(i < commands.size()-1){    // fixing the stdout of the child
                              dup2(pipes[i].first, 1);
                        }

                        // now the full pipes info is of no use to the child
                        // so close the fds
                        for(size_t itr = 0; itr < pipes.size(); itr++){
                              close(pipes[itr].first);
                              close(pipes[itr].second);
                        }

                        // the i/o are set for the child
                        // redirection can overwrite them, if there's any
                        for(size_t j = 0; j < cur_cmd.redirections.size(); j++){
                              Redirection cur_redir = cur_cmd.redirections[j];
                              
                              int fd;
                              int flag;
                              if(cur_redir.rd_type == Redirection_type::Input){
                                    fd = open(cur_redir.filename.c_str(), O_RDONLY);
                                    flag = dup2(fd, 0);
                              }
                              else if(cur_redir.rd_type == Redirection_type::Output){
                                    fd = open(cur_redir.filename.c_str(), O_WRONLY | O_CREAT | O_TRUNC, 0644);
                                    flag = dup2(fd, 1);
                              }
                              else if(cur_redir.rd_type == Redirection_type::Append){
                                    fd = open(cur_redir.filename.c_str(), O_WRONLY | O_CREAT | O_APPEND, 0644);
                                    flag = dup2(fd, 1);
                              }

                              // if file opeaning or redirecting fails
                              if(fd == -1){
                                    perror(cur_redir.filename.c_str());
                                    exit(1);
                              }
                              if(flag == -1){
                                    perror("dup2");
                                    exit(1);
                              }
                              close(fd);
                        }
                        // input and the op is set at this point, pass it to the program

                        if(is_builtin(cur_cmd.args[0])){
                              exec_builtin(cur_cmd.args);
                              exit(0);
                        }
                        else{
                              std::vector<char*> argv = convert_to_cType(cur_cmd.args);
                              execvp(cur_cmd.args[0].c_str(), argv.data());

                              std::cerr << cur_cmd.args[0] << ": command not found\n";
                              // if the command is not found then we want it in fd2
                              // not in the output fd that is set by the code before
                              // thats why use cerr than cout
                              
                              exit(1); // exiting the child process
                        } 
                  }
                  
                  // parent
                  else{
                        pids.push_back(process_id);
                  }
            }
      }

      // inside the child we close the fds, yes
      // but they are still open in the parent
      // so we close them for the parent
      for(size_t itr = 0; itr < pipes.size(); itr++){
            close(pipes[itr].first);
            close(pipes[itr].second);
      }

      // bring a closure to all the childs
      for(pid_t pid: pids){
            waitpid(pid, nullptr, 0);
      }

      return;
}