#pragma once

#include <iostream>
#include <vector>
#include <string>

extern std::vector<std::string> curr_session_histories;
class Shell{
      public:
            void run();
};

std::string read_command();