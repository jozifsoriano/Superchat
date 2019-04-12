#include <ncurses.h>
#include <fstream>
#include <iostream>
#include <string.h>

int main(int argc, char** argv)
{
  //FILE STUFF
  std::string input_ID, input_password;
  std::cout << "Input ID: ";
  std::cin >> input_ID;
  std::cout << "Input pw: ";
  std::cin >> input_password;
  std::string checkid, checkpw, fstring;
  std::ifstream flogin("~Superchat");
  while(std::getline(flogin,fstring)){
    std::string delimiter = "/";
    std::cout << "delimiter:" << delimiter << std::endl;
    size_t pos = 0;
    pos = fstring.find(delimiter);
    checkid = fstring.substr(0, pos);
    std::cout << "check id: " << checkid << std::endl;
    checkpw = fstring.substr(pos+1,fstring.length());
    std::cout << "check pw: " << checkpw << std::endl;
    if((input_ID == checkid)&&(input_password == checkpw)){
      std::cout << "true" << std::endl;
    }
  }
  std::cout << "false" << std::endl;
  /*std::ofstream out;

  // std::ios::app is the open mode "append" meaning
  // new data will be written to the end of the file.
  out.open("myfile.txt", std::ios::app);

  std::string str = "I am here.\n";
  out << str;*/


  //GUI STUFF
  /*initscr();
  //get screen size
  int ymax, xmax;
  getmaxyx(stdscr,ymax,xmax);

  WINDOW *win=newwin(0,0,ymax,xmax);
  box(win,1,1);
  refresh();
  wrefresh(win);
  getch();
  endwin();*/
  return 0;
}
