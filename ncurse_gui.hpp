#ifndef NC_GUI_HPP
#define NC_GUI_HPP

#include <ncurses.h>
#include <string>

class interface{
  std::string userID;
protected:
  WINDOW *main;
  WINDOW *members;
  WINDOW *input;

  int isize = 3;
  int rsize;
  void setup();
  void draw_borders(WINDOW *w);
  void create_main_box();
  void create_members_box();
  void create_input_box();
  void set_rsize();
  std::string get_input(WINDOW *w, char *name);
  bool check_command();
public:
  std::string get_user();

};

class login: public interface{
  std::string input_ID;
  std::string input_password;
private:
  bool validate_credentials();
  void create_account();
  bool run_login();
public:
  login();
  ~login();
};

class menu: public interface{
public:
  menu();
  void join_room();
  void join_lobby();
  void create_room();
  ~menu();
protected:
  void list_users();
  void display_room_list();
  void delete_room(std::string n);
  std::string port_num;
};

class room: public menu{
public:
  std::string name;
public:
  room();
  void print_recent_msgs();
  ~room();
};

class manager: public interface{
public:
  manager();
  ~manager();

};

class command{
  std::string input;
public:
  command(std::string c);
private:
  void help();
  void mute();
  void create_room();
  void display_room_list();
  void exit_client();
  void broadcast();
  void transfer();
  void error();
  ~command();
};

#endif // NC_GUI.hpp
