#include <iostream>
#include <vector>
#include <string>
#include <utility>

#include "../include/command_parser.hpp"

int main(){
      std::vector<std::pair<const std::string, std::vector<std::string>>> tests = {
            {R"(echo hello world)",
            {"echo", "hello", "world"}},

            {R"(echo "hello world")",
            {"echo", "hello world"}},

            {R"(echo 'hello world')",
            {"echo", "hello world"}},

            {R"(echo "hello"'world')",
            {"echo", "helloworld"}},

            {R"(echo hello"world")",
            {"echo", "helloworld"}},

            {R"(echo "hello"world)",
            {"echo", "helloworld"}},

            {R"(echo a"b"c'd'e)",
            {"echo", "abcde"}},

            {R"(echo a'b'c"d"e'f'g)",
            {"echo", "abcdefg"}},

            {R"(echo "")",
            {"echo", ""}},

            {R"(echo '')",
            {"echo", ""}},

            {R"(echo """"a)",
            {"echo", "a"}},

            {R"(echo a""""b)",
            {"echo", "ab"}},

            {R"(echo ''""''"")",
            {"echo", ""}},

            {R"(echo "a>b")",
            {"echo", "a>b"}},

            {R"(echo 'a>>b')",
            {"echo", "a>>b"}},

            {R"(echo ">>|<<&&><")",
            {"echo", ">>|<<&&><"}},

            {R"(echo '><><><')",
            {"echo", "><><><"}},

            {R"(echo hello>out.txt)",
            {"echo", "hello", ">", "out.txt"}},

            {R"(echo hello>>out.txt)",
            {"echo", "hello", ">>", "out.txt"}},

            {R"(cat<input.txt)",
            {"cat", "<", "input.txt"}},

            {R"(ls|wc)",
            {"ls", "|", "wc"}},

            {R"(echo "a>b"|cat)",
            {"echo", "a>b", "|", "cat"}},

            {R"(echo 'a>>b'>>file)",
            {"echo", "a>>b", ">>", "file"}},

            {R"(echo a"b>c"d>>file)",
            {"echo", "ab>cd", ">>", "file"}},

            {R"(cat<input|grep"abc">>out)",
            {"cat", "<", "input", "|", "grepabc", ">>", "out"}},

            {R"(cmd1>>out1|cmd2<input2>>out2|cmd3"a b"'c d'>>final)",
            {"cmd1", ">>", "out1", "|", "cmd2", "<", "input2", ">>", "out2", "|", "cmd3a bc d", ">>", "final"}},

            {R"(echo >>> kela)",
            {"echo", ">>", ">", "kela"}},

            {R"(echo >>>> kela)",
            {"echo", ">>", ">>", "kela"}},

            {R"(echo >>>>> kela)",
            {"echo", ">>", ">>", ">", "kela"}},

            {R"(echo >> > >>> > >>>> kela)",
            {"echo", ">>", ">", ">>", ">", ">", ">>", ">>", "kela"}},

            {R"(echo >> >a >>>a>> >>a>>> kela)",
            {"echo", ">>", ">", "a", ">>", ">", "a", ">>", ">>", "a", ">>", ">", "kela"}},

            {R"(a<b>c>>d|e)",
            {"a", "<", "b", ">", "c", ">>", "d", "|", "e"}},

            {R"(a""b''c"d"e'f'g"h")",
            {"abcdefgh"}},

            {R"("a""b""c""d")",
            {"abcd"}},

            {R"('a''b''c''d')",
            {"abcd"}},

            {R"(a><<>>>|&&||b)",
            {"-1"}},

            {R"(echo a"b"'c'd)",
            {"echo", "abcd"}},

            {R"(echo "a"'b'"c"'d'"e")",
            {"echo", "abcde"}},

            {R"(echo abc<in>out)",
            {"echo", "abc", "<", "in", ">", "out"}},

            {R"(echo abc>>out<<in)",
            {"echo", "abc", ">>", "out", "<<", "in"}},

            {R"(cmd<in|grep abc>>out)",
            {"cmd", "<", "in", "|", "grep", "abc", ">>", "out"}},

            {R"(cmd"a""b"'c'd)",
            {"cmdabcd"}},

            {R"(""abc""def"")",
            {"abcdef"}},

            {R"(''abc''def'')",
            {"abcdef"}},

            {R"(a>>>>b)",
            {"a", ">>", ">>", "b"}},

            {R"(a>>>>>b)",
            {"a", ">>", ">>", ">", "b"}},

            {R"(a<<<<<<<b)",
            {"a", "<<", "<<", "<<", "<", "b"}},

            {R"(a||||b)",
            {"a", "||", "||", "b"}},

            {R"(a&&&&b)",
            {"a", "&&", "&&", "b"}},

            {R"(a|&&||b)",
            {"-1"}},

            {R"(a><b)",
            {"-1"}},

            {R"(a<>b)",
            {"-1"}},

            {R"(a>|b)",
            {"-1"}},

            {R"(a<&|b)",
            {"a", "<&", "|", "b"}},      // if you support <&

            {R"(a>&|b)",
            {"a", ">&", "|", "b"}},      // if you support >&

            {R"(echo ">|<&&||>>")",
            {"echo", ">|<&&||>>"}},

            {R"(echo '>|<&&||>>')",
            {"echo", ">|<&&||>>"}},

            {R"(echo abc""""def)",
            {"echo", "abcdef"}},

            {R"(echo ""abc""def"")",
            {"echo", "abcdef"}},

            {R"(echo 'abc''def')",
            {"echo", "abcdef"}},

            {R"(echo ""''""''abc)",
            {"echo", "abc"}},

            {R"(cmd1<in1<in2)",
            {"cmd1", "<", "in1", "<", "in2"}},

            {R"(cmd1>o1>>o2)",
            {"cmd1", ">", "o1", ">>", "o2"}},

            {R"(cmd1|cmd2|cmd3|cmd4)",
            {"cmd1", "|", "cmd2", "|", "cmd3", "|", "cmd4"}},

            {R"(cmd1>>o|cmd2<<i|cmd3>a)",
            {"cmd1", ">>", "o", "|", "cmd2", "<<", "i", "|", "cmd3", ">", "a"}},

            {R"(echo "abc""def"'ghi'jkl)",
            {"echo", "abcdefghijkl"}},

            {R"(a>>>>><<<<<b)",
            {"-1"}},

            {R"(a><<<b)",
            {"-1"}},

            {R"(a|><b)",
            {"-1"}},

            {R"(a>>|<<b)",
            {"-1"}},
      };
      
      CommandTokenizer tokenizer;

      int passed = 0;
      for(int i = 0; i < tests.size(); i++){
            std::vector<std::string> returned_val = tokenizer.tokenize(tests[i].first);
            if(returned_val != tests[i].second){
                  std::cout << "\nTest no: " << i+1 << "\n";

                  std::cout << "\nString is: \n";
                  std::cout << tests[i].first;
                  std::cout << "\n\n";

                  std::cout << "Returned: \n";
                  for(auto it: returned_val) std::cout << it << "\t";
                  std::cout << "\n\n";

                  std::cout << "Expected: \n";
                  for(auto it: tests[i].second) std::cout << it << "\t";
                  std::cout << "\n\n---------------------------------------------------\n";
            }
            else passed++;
      }
      if(passed == tests.size()) std::cout << "all good\n";

      return 0;
}