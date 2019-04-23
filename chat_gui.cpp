#include <ncurses.h>
#include <string>
#include <vector>
#include <readline/readline.h>
#include <readline/history.h>
#include "chat_gui.hpp"

int Mygui::ncurse_mainmenu(std::string menuArray[])
{

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
      endwin();
      return highlight+1;
    }
    if(choice==27){
      break;
    }
  }
}
