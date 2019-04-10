#include <ncurses.h>
#include <string>
#include <vector>
#include <readline/readline.h>
#include <readline/history.h>

//#include "ncurse_gui.hpp"
class interface{
  WINDOW *win;
  int userID;
public:
  void create_room_box();
  void create_roomMembers_box();
  void create_input_box();
protected:
  int get_user(){
    return userID;
  }
  //std::vector<std::string> get_room_list(){}
  //void change_room(int room_id){}
};

class login: public interface{
  int input_ID;
  std::string input_password;
private:
  void run_login(){

  }
  bool validate_credentials(){
    if(1){
      return true;
    }else{
      return false;
    }
  }
  void create_account(){

  }
public:
  login(){
    //ncurse start
    initscr();
    cbreak();
    noecho();

    //get screen size
    int ymax, xmax;
    getmaxyx(stdscr,ymax,xmax);

    //window
    WINDOW * l = newwin(6,xmax-12,ymax-8,5);
    refresh();
    wrefresh(l);

    //for arrow keys
    keypad(l,true);

    //options
    std::string options[2] = {"Login", "Create account"};
    int choice;
    int highlight = 0;

    while(1){
      //for loop write options with background color highlight
      for(int i=0; i<2; i++){
        if(i==highlight){
          wattron(l, A_REVERSE);
        }
        mvwprintw(l,i+1,1, options[i].c_str());
        wattroff(l, A_REVERSE);
      }
      choice = wgetch(l);
      switch(choice){
        case KEY_UP:
          highlight--;
          if(highlight==-1){
            highlight=0;
          }
          break;
        case KEY_DOWN:
          highlight++;
          if(highlight==2){
            highlight=1;
          }
          break;
        case 10:
          if(highlight==0){
            run_login();
          }
          if(highlight==1){
            create_account();
          }
      }
      if(choice==27){
        break;
      }
    }

  }
  void compare_file(){
    
  }
  ~login(){
    endwin();
  }
};
/*
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
*/
class menu: public interface{
public:
  menu(){
    //ncurse start
    initscr();
    cbreak();
    noecho();

    //get screen size
    int ymax, xmax;
    getmaxyx(stdscr,ymax,xmax);

    //window
    WINDOW * l = newwin(6,xmax-12,ymax-8,5);
    refresh();
    wrefresh(l);

    //for arrow keys
    keypad(l,true);

    //options
    std::string options[3] = {"Join Room","Create Room","Exit"};
    int choice;
    int highlight = 0;

    while(1){
      //for loop write options with background color highlight
      for(int i=0; i<3; i++){
        if(i==highlight){
          wattron(l, A_REVERSE);
        }
        mvwprintw(l,i+1,1, options[i].c_str());
        wattroff(l, A_REVERSE);
      }
      choice = wgetch(l);
      switch(choice){
        case KEY_UP:
          highlight--;
          if(highlight==-1){
            highlight=0;
          }
          break;
        case KEY_DOWN:
          highlight++;
          if(highlight==3){
            highlight=2;
          }
          break;
        case 10:
          if(highlight==0){
            join_room();
          }
          if(highlight==1){
            create_room();
          }
          if(highlight==2){
            endwin();
          }
      }
      if(choice==27){
        break;
      }
    }
  }

  int join_room(){
    return 9000;
  }

  void create_room(){

  }

  ~menu(){
    endwin();
  }

};
/*
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
*/


int main(){
  //login l;
  menu m;
  return 0;
}
