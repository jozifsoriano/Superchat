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

//return the userID
std::string interface::get_user(){
  return userID;
}

//setup the window with the 3 boxes
void interface::setup(){
  //ncurse start
  initscr();
  cbreak();
  noecho();

  //whole window
  set_rsize();
  create_main_box();
  create_members_box();
  create_input_box();
  refresh();
  wrefresh(main);
  wrefresh(members);
  wrefresh(input);

  //enable arrow keys
  keypad(main,TRUE);
}

//https://gist.github.com/reagent/9819045
//draw the border for a window
void interface::draw_borders(WINDOW *screen) {
  int x, y, i;

  getmaxyx(screen, y, x);

  // 4 corners
  mvwprintw(screen, 0, 0, "+");
  mvwprintw(screen, y - 1, 0, "+");
  mvwprintw(screen, 0, x - 1, "+");
  mvwprintw(screen, y - 1, x - 1, "+");

  // sides
  for (i = 1; i < (y - 1); i++) {
    mvwprintw(screen, i, 0, "|");
    mvwprintw(screen, i, x - 1, "|");
  }

  // top and bottom
  for (i = 1; i < (x - 1); i++) {
    mvwprintw(screen, 0, i, "-");
    mvwprintw(screen, y - 1, i, "-");
  }
}

//create the main window (top left) and draws the border
void interface::create_main_box(){
  int ymax, xmax;
  getmaxyx(stdscr,ymax,xmax);
  main = newwin(ymax-isize,xmax-rsize,0,0);
  draw_borders(main);
}

//create the members window (top right) and draw border
void interface::create_members_box(){
  int ymax, xmax;
  getmaxyx(stdscr,ymax,xmax);
  members = newwin(ymax-isize,rsize,0,xmax-rsize);
  draw_borders(members);
}

//create the input window (bottom) and draw border
void interface::create_input_box(){
  int ymax, xmax;
  getmaxyx(stdscr,ymax,xmax);
  input = newwin(isize, xmax, ymax - isize, 0);
  draw_borders(input);
}

//sets the size of the right window to be 1/4th of the screen
void interface::set_rsize(){
  int ymax, xmax;
  getmaxyx(stdscr,ymax,xmax);
  rsize = xmax/4;
}

//for a window, get the input
std::string interface::get_input(WINDOW *w, char *name){
  wclear(w);
  wrefresh(w);
  create_input_box();
  bool flag = TRUE;
  char *in_str = (char*)malloc(150*sizeof(char));
  int i=0;
  refresh();
  wrefresh(w);
  mvwprintw(w,1,1,"%s: ",name);
  refresh();
  wrefresh(w);
  do{
    char temp;
    temp = getch();
    if(temp == 10){
      flag = FALSE;
    }else if(temp == 47){//chars that cant be used

    }else if((temp>=32)&&(temp<=127)){
      in_str[i] = temp;
      mvwprintw(w,1,strlen(name)+2,"%s",in_str);
      wrefresh(w);
      i++;
    }else if (temp == 27){
      exit(1);
    }
  }while(flag);
  std::string str(in_str);
  free(in_str);
  wclear(w);
  wrefresh(w);
  return str;
}

//main login screen when login is declared in client
login::login(){
  //flag for movement keys
  bool flag = TRUE;
  setup();

  //options
  std::string options[2] = {"Login", "Create account"};
  int choice;
  int highlight = 0;

  while(flag){
    //empty input_ID and input_password
    //for loop write options with background color highlight
    input_ID = input_password = "";
    for(int i=0; i<2; i++){
      if(i==highlight){
        wattron(main, A_REVERSE);
      }
      mvwprintw(main,(i*3)+15,10, options[i].c_str());
      wattroff(main, A_REVERSE);
    }

    choice = wgetch(main);
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
          clear();
          flag = run_login();
          setup();
        }
        if(highlight==1){
          clear();
          create_account();
          //flag = false;
          setup();
        }
        break;
      case 27:
          flag = false;
          break;
      default:
        break;

      }
  }
  endwin();
}

bool login::run_login(){
  setup();
  char id_str[] = "ID";
  char pw_str[] = "Password";
  //user input
  mvwprintw(main,2,2,"ENTER LOGIN INFO: ");
  mvwprintw(main,15,10,"USER ID:   ");
  mvwprintw(main,20,10,"PASSWORD:  ");
  wrefresh(main);

  input_ID = get_input(input, id_str);
  mvwprintw(main,15,10,"USER ID:   %s",input_ID.c_str());
  wclear(input);
  wrefresh(main);
  wrefresh(input);

  input_password = get_input(input, pw_str);
  mvwprintw(main,20,10,"PASSWORD: (hidden)");
  refresh();
  wclear(input);
  wrefresh(main);
  wrefresh(input);
  clear();
  if(validate_credentials()==FALSE){
    return TRUE;
  }else{
    return FALSE;
  }
}

//compares the id and password
bool login::validate_credentials(){
  std::string checkid, checkpw, fstring;
  std::ifstream flogin("~Superchat");
  while(std::getline(flogin,fstring)){
    std::string delimiter = "/";
    size_t pos = 0;
    pos = fstring.find(delimiter);
    checkid = fstring.substr(0, pos);
    checkpw = fstring.substr(pos+1,fstring.length());
    if((input_ID == checkid)&&(input_password == checkpw)){
      return TRUE;
    }
  }
  return FALSE;
}

//input id and password, store to file
void login::create_account(){
  setup();
  char id_str[] = "ID";
  char pw_str[] = "Password";
  std::ofstream login_info;
  mvwprintw(main,2,2,"CREATE AN ACCOUNT: ");
  login_info.open("~Superchat", std::ios::app);
  mvwprintw(main,15,10,"USER ID:   ");
  mvwprintw(main,20,10,"PASSWORD:  ");
  wrefresh(main);
  input_ID = get_input(input, id_str);
  mvwprintw(main,15,10,"USER ID:   %s",input_ID.c_str());
  wclear(input);
  create_input_box();
  wrefresh(main);
  wrefresh(input);
  input_password = get_input(input, pw_str);
  mvwprintw(main,20,10,"PASSWORD: (hidden)");
  refresh();
  wclear(input);
  create_input_box();
  wrefresh(main);
  wrefresh(input);
  login_info << input_ID << "/" << input_password <<"\n";
  login_info.close();
  clear();
}

//destructor
login::~login(){
  endwin();
}

/*
menu::menu(){
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
  keypad(l,TRUE);

  getch();
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
      case 10: //KEY_ENTER
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
}*/

int menu::join_room(){
  return 9000;
}

void menu::create_room(){

}

menu::~menu(){
  endwin();
}
