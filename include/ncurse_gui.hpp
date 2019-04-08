#ifndef NC_GUI_HPP
#define NC_GUI_HPP

#include <ncurses.h>
#include <string.h>

class interface{
  WINDOW win;
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


};

class room: public interface{


};

class menu: public interface{


};

class manager: public interface{


};

class command{
  std::string input;
public:
  command(std::string c);
  void create_room();
  void display_room_list
  void exit_client();
};

#endif // NC_GUI.hpp
