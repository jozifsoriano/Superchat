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
  std::string get_user();
  void draw_borders(WINDOW *w);
  void create_main_box();
  void create_members_box();
  void create_input_box();
  void set_rsize();
  void setup();
  //vector<chat_r> get_room_list();
  //void change_room(int room_id);

};

class login: public interface{
  char *input_ID;
  char *input_password;
private:
  bool validate_credentials(char *id,char *pw);
  void create_account();
  void run_login();
public:
  login();
  ~login();
};

class room: public interface{
  int port_num;
public:
  std::string name;
private:
  void add_room();
  int get_room();
public:
  room();
  void list_users();
  void display_room_list();
  void delete_room(std::string n);
  ~room();
};

class menu: public interface{
public:
  menu();
  int join_room();
  void create_room();
  ~menu();

};

class manager: public interface{


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
  ~command();
};

#endif // NC_GUI.hpp
