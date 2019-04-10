#ifndef NC_GUI_HPP
#define NC_GUI_HPP

#include <ncurses.h>
#include <string>

class interface{
  WINDOW *win;
  int *userID;
public:
  void create_room_box();
  void create_roomMembers_box();
  void create_text_box();
protected:
  int get_user();
  vector<chat_room> get_room_list();
  void change_room(int room_id);

};

class login: public interface{
  int input_ID;
  std::string input_password;
private:
  bool validate_credentials();
  void create_account();
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
  ~room();
  delete_room(std::string n);
  ~room();
};

class menu: public interface{
public:
  menu();
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
