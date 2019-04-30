#ifndef NC_GUI_HPP
#define NC_GUI_HPP

#include <ncurses.h>
#include <string>
//#include "gui_input.hpp"

class interface{
  std::string userID;
protected:
  WINDOW *main;
  WINDOW *input;
  WINDOW *members;
  int isize = 3;
  int rsize;
  void setup();//j
  void draw_borders(WINDOW *w);//js
  void create_main_box();//js
  void create_members_box();//js
  void create_input_box();//js
  void set_rsize();//js
  void set_userID(std::string name);//js
  bool check_command();
public:
  WINDOW *get_inputw();
  std::string get_input(WINDOW *w, char *name);//replace with rlncurses
  std::string get_user();
  bool quit_flag = TRUE; // when false, quit

};

class login: public interface{//js
  std::string input_ID;
  std::string input_password;
private:
  bool validate_credentials();//js
  void create_account();//js
  bool run_login();//js
public:
  login();//js
  ~login();//js
};

class menu: public interface{
public:
  void init_menu(std::string id);//js
  void join_room();
  void join_lobby();
  void create_room();
  std::string get_port();//js
  bool continue_flag = TRUE;
  ~menu();
protected:
  void list_users();
  void display_room_list();
  void delete_room(std::string n);
  std::string port_num;
};

class manager: public interface{
public:
  manager();
  ~manager();

};

class command: public interface{
  std::string input;
public:
  command(std::string c);
private:
  void help();
  void mute();
  void create_room();
  void display_room_list();
  void exit_client();
  void spellcheck();
  void broadcast();
  void transfer();
  void get_uptime();
  void error();
  ~command();
};

#endif // NC_GUI.hpp
