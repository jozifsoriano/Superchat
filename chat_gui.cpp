#include <ncurses.h>
#include <string>
#include <vector>
#include "chat_gui.hpp"
int Mygui::ncurse_mainmenu()
{
  std::string menuArray[10];
  menuArray[0]="1.Enter Lobby";
  menuArray[1]="2.Login";
  menuArray[2]="3.quit";
  initscr();
  cbreak();
  noecho();
  int ymax, xmax;
  getmaxyx(stdscr,ymax,xmax);

  //window
  WINDOW * l = newwin(6,xmax-12,ymax-8,5);
  refresh();
  wrefresh(l);

  keypad(l,true);

  int choice;
  int highlight = 0;

  while(1){
    //for loop write options with background color highlight
    for(int i=0; i<4; i++){
      if(i==highlight){
        wattron(l, A_REVERSE);
      }
      mvwprintw(l,i+1,1, menuArray[i].c_str());
      wattroff(l, A_REVERSE);
    }
    choice = wgetch(l);
    switch(choice){
      case KEY_UP:
        highlight--;
        if(highlight==-1){
          highlight=3;
        }
        break;
      case KEY_DOWN:
        highlight++;
        if(highlight==4){
          highlight=0;
        }
        break;
      case 10:
      refresh();
      wrefresh(l);
      endwin();
      return highlight+1;
    }
    if(choice==27){
      break;
    }
  }
}

int Mygui::ncurse_loginmenu()
{
  std::string loginMenuArray[10];
  loginMenuArray[0]="1.Create Account";
  loginMenuArray[1]="2.Login";
  initscr();
  cbreak();
  noecho();
  int ymax, xmax;
  getmaxyx(stdscr,ymax,xmax);

  //window
  WINDOW * l = newwin(6,xmax-12,ymax-8,5);
  refresh();
  wrefresh(l);

  keypad(l,true);

  int choice;
  int highlight = 0;

  while(1){
    //for loop write options with background color highlight
    for(int i=0; i<3; i++){
      if(i==highlight){
        wattron(l, A_REVERSE);
      }
      mvwprintw(l,i+1,1, loginMenuArray[i].c_str());
      wattroff(l, A_REVERSE);
    }
    choice = wgetch(l);
    switch(choice){
      case KEY_UP:
        highlight--;
        if(highlight==-1){
          highlight=2;
        }
        break;
      case KEY_DOWN:
        highlight++;
        if(highlight==3){
          highlight=0;
        }
        break;
      case 10:
      refresh();
      wrefresh(l);
      endwin();
      return highlight+1;
    }
    if(choice==27){
      break;
    }
  }
}

int Mygui::ncurse_chatroommenu()
{
  std::string ChatroomMenu[10];
  ChatroomMenu[0]= "1.Enter CHATROOM";
  ChatroomMenu[1]= "2.Create CHATROOM";
  ChatroomMenu[2]= "3.Delete CHATROOM";
  ChatroomMenu[3]= "4.Quit";
  initscr();
  cbreak();
  noecho();
  int ymax, xmax;
  getmaxyx(stdscr,ymax,xmax);

  //window
  WINDOW * l = newwin(6,xmax-12,ymax-8,5);
  refresh();
  wrefresh(l);

  keypad(l,true);

  int choice;
  int highlight = 0;

  while(1){
    //for loop write options with background color highlight
    for(int i=0; i<4; i++){
      if(i==highlight){
        wattron(l, A_REVERSE);
      }
      mvwprintw(l,i+1,1, ChatroomMenu[i].c_str());
      wattroff(l, A_REVERSE);
    }
    choice = wgetch(l);
    switch(choice){
      case KEY_UP:
        highlight--;
        if(highlight==-1){
          highlight=3;
        }
        break;
      case KEY_DOWN:
        highlight++;
        if(highlight==4){
          highlight=0;
        }
        break;
      case 10:
      refresh();
      wrefresh(l);
      endwin();
      return highlight+1;
    }
    if(choice==27){
      break;
    }
  }

}
