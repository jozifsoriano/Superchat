#include <ncurses.h>

int main(int argc, char** argv)
{
  initscr();
  //get screen size
  int ymax, xmax;
  getmaxyx(stdscr,ymax,xmax);

  WINDOW *win=newwin(0,0,ymax,xmax);
  box(win,1,1);
  refresh();
  wrefresh(win);
  getch();
  endwin();
  return 0;
}
