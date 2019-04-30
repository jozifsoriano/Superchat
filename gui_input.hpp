#ifndef GUI_INPUT_HPP
#define GUI_INPUT_HPP

#include <ncurses.h>
#include <readline/history.h>
#include <readline/readline.h>
#include <string>
#include "ncurse_gui.hpp"
/*
class interface{
  std::string userID;
protected:
  WINDOW *main;
  WINDOW *members;
  WINDOW *input;

  int isize = 3;
  int rsize;
  void setup();//js
  void draw_borders(WINDOW *w);//js
  void create_main_box();//js
  void create_members_box();//js
  void create_input_box();//js
  void set_rsize();//js
  std::string get_input(WINDOW *w, char *name);
  bool check_command();
public:
  std::string get_user();
  bool quit_flag = TRUE; // when false, quit

};*/

class rln_input:public interface{

//variables
public:

private:

protected:


//||~~~~||

//functions
public:

private:

protected:

};

#endif //GUI_INPUT_HPP
