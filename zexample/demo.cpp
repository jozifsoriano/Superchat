#include <ncurses.h>
#include <string.h>
#include <cstring>
#include <vector>
#include <stdio.h>
#include <stdlib.h>
#include <readline/readline.h>
#include <readline/history.h>
#include "ncurse_gui.hpp"
#include <fstream>

//g++ -Wall -O0 -g -std=c++11 -I/Users/josephsoriano/Downloads/boost_1_69_0/ -lreadline -lncurses
int main(){
  interface i;
  i.sample();
  return 0;
}
