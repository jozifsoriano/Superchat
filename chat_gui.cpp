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

std::string interface::get_user(){
  return userID;
}

void interface::setup(){
  //ncurse start
  initscr();
  cbreak();
  noecho();

  //window
  set_rsize();
  create_main_box();
  create_members_box();
  create_input_box();
  refresh();
  wrefresh(main);
  wrefresh(members);
  wrefresh(input);
  //for arrow keys
  keypad(main,true);
}

//https://gist.github.com/reagent/9819045
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

void interface::create_main_box(){
  int ymax, xmax;
  getmaxyx(stdscr,ymax,xmax);
  main = newwin(ymax-isize,xmax-rsize,0,0);
  draw_borders(main);
}

void interface::create_members_box(){
  int ymax, xmax;
  getmaxyx(stdscr,ymax,xmax);
  members = newwin(ymax-isize,rsize,0,xmax-rsize);
  draw_borders(members);
}

void interface::create_input_box(){
  int ymax, xmax;
  getmaxyx(stdscr,ymax,xmax);
  input = newwin(isize, xmax, ymax - isize, 0);
  draw_borders(input);
}

void interface::set_rsize(){
  int ymax, xmax;
  getmaxyx(stdscr,ymax,xmax);
  rsize = xmax/4;
}

login::login(){
  bool flag = true;
  setup();

  //options
  std::string options[2] = {"Login", "Create account"};
  int choice;
  int highlight = 0;

  while(flag){
    //for loop write options with background color highlight
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
          run_login();
          //flag =false;
        }
        if(highlight==1){
          //create_account();
          //flag = false;
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

void login::run_login(){
  setup();
  echo();
  //user input
  wrefresh(main);
  mvwprintw(main,15,1,"USER ID:   ");
  mvwprintw(input,1,1,"User ID:  ");
  wmove(input,1,14);
  getstr(input_ID);

  //mvwprintw(main,15,1,"USER ID:   %s",input_ID);
  //mvwprintw(main,45,1,"PASSWORD:  ");
  //mvwprintw(input,1,1,"Password:  ");
  wgetstr(input,input_password);
  wrefresh(main);
  wrefresh(input);
  refresh();
  getch();

  //validate_credentials(input_ID,input_password);
}

bool login::validate_credentials(char *id ,char *pw){
  std::string checkid, checkpw, fstring;
  std::ifstream flogin("~Superchat");
  while(std::getline(flogin,fstring)){
    std::string delimiter = "/";
    size_t pos = 0;
    pos = fstring.find(delimiter);
    checkid = fstring.substr(0, pos);
    checkpw = fstring.substr(pos,fstring.length());
    if((id == checkid)&&(pw == checkpw)){
      return true;
    }
  }
  return false;
}

void login::create_account(){

}


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
  keypad(l,true);

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
