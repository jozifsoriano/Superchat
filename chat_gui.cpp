#include <ncurses.h>
#include <string.h>
#include <cstring>
#include <vector>
#include <stdio.h>
#include <stdlib.h>
//#include <readline/readline.h> //rl
//#include <readline/history.h> //rl
#include "ncurse_gui.hpp"
#include <fstream>

/*
static size_t strnwidth(const char *s, size_t n, size_t offset){
  mbstate_t shift_state;
  wchar_t wc;
  size_t wc_len;
  size_t width = 0;
  memset(&shift_state, '\0', sizeof shift_state);
  for (size_t i = 0; i < n; i += wc_len) {
    wc_len = mbrtowc(&wc, s + i, MB_CUR_MAX, &shift_state);
    switch (wc_len) {
    case 0:
      goto done;
    case (size_t)-1: case (size_t)-2:
      width += strnlen(s + i, n - i);
      goto done;
    }
    if (wc == '\t'){
      width = ((width + offset + 8) & ~7) - offset;
    }else{
      width += iswcntrl(wc) ? 2 : std::max(0, wcwidth(wc));
    }
  }

done:
  return width;
}

static size_t strwidth(const char *s, size_t offset){
  return strnwidth(s, SIZE_MAX, offset);
}

int readline_input_avail(void){
  return input_avail;
}

int readline_getc(FILE *dummy){
  input_avail=FALSE;
  return cinput;
}

void got_command(char *line){
  if (!line){
    // Ctrl-D pressed on empty line
    should_exit = TRUE;
  }else{
    if (*line){
      add_history(line);
    }
    free(msg_win_str);
    msg_win_str = line;
    //msg_win_redisplay(false);
  }
}

void forward_to_readline(char c){
  cinput = c;
  input_avail = TRUE;
  rl_callback_read_char();
}

void init_readline(char *name){
  rl_catch_signals = 0;
  rl_catch_sigwinch = 0;
  rl_deprep_term_function = NULL;
  rl_prep_term_function = NULL;
  rl_change_environment = 0;
  rl_getc_function = readline_getc;
  rl_input_available_hook = readline_input_avail;
  //rl_callback_handler_install(name, got_command);
  rl_callback_handler_install("> ", got_command);
}

static void input_redisplay(WINDOW *w, bool for_resize){
  size_t prompt_width = strwidth(rl_display_prompt, 0);
  size_t cursor_col = prompt_width + strnwidth(rl_line_buffer, rl_point, prompt_width);

  werase(w);
  mvwprintw(w, 1, 1, "%s%s", rl_display_prompt, rl_line_buffer);
  if(cursor_col>= COLS){
    curs_set(0);
  }else{
    wmove(w,1,cursor_col);
    curs_set(2);
  }
  if (for_resize){
    wnoutrefresh(w);
  }else{
    wrefresh(w);
  }
}

static void readline_redisplay(WINDOW *w){
  input_redisplay(w, FALSE);
}

static void resize(WINDOW *w){
  input_redisplay(w, TRUE);
  doupdate();
}
std::string get_rlinput(WINDOW *w, char *name){
  init_readline(name);

  //refresh();
  //wrefresh(w);
  do{
    int c = wgetch(w);
    switch(c){
    case 10:
      //should_exit=TRUE;
    case KEY_RESIZE:
      resize(w);
      break;
    case '\f':
      clearok(curscr, TRUE);
      resize(w);
      break;
    default:
      forward_to_readline(c);

    }
  }while(!should_exit);
  std::string s(msg_win_str);
  return s;
}
*/

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
  curs_set(0);

  //whole window
  set_rsize();
  create_main_box();
  create_members_box();
  create_input_box();
  keypad(main,TRUE);
  refresh();
  wrefresh(main);
  wrefresh(members);
  wrefresh(input);

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
  mvwprintw(w,2,1,"%s: ",name);
  refresh();
  wrefresh(w);
  do{
    char temp;
    temp = wgetch(w);
    if(temp == 10){
      flag = FALSE;
    }else if(temp == 47){//chars that cant be used

    }else if((temp>=32)&&(temp<=126)){
      wclear(w);
      in_str[i] = temp;
      mvwprintw(w,1,strlen(name)+2,"%s",in_str);
      wrefresh(w);
      i++;
    }else if (temp == 127){

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

void interface::set_userID(std::string s){
  userID = s;
}

WINDOW *interface::get_inputw(){
  return input;
}

//main login screen when login is declared in client
login::login(){
  //flag for movement keys
  bool flag = TRUE;
  setup();

  //options
  std::string options[3] = {"Login", "Create account","Exit"};
  int choice;
  int highlight = 0;

  while(flag==TRUE){
    //empty input_ID and input_password
    //for loop write options with background color highlight
    input_ID = input_password = "";
    for(int i=0; i<3; i++){
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
        if(highlight==3){
          highlight=2;
        }
        break;
      case 10:
        if(highlight==0){
          clear();
          flag = run_login();
          setup();
        }else if(highlight==1){
          clear();
          create_account();
          setup();
        }else if (highlight==2){
          flag = FALSE;
          quit_flag = FALSE;
        }
        break;
      case 27:
          flag = FALSE;
          quit_flag = FALSE;
          break;
      default:
        break;

      }
  }
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

  //input_ID = get_rlinput(input,id_str);
  input_ID = get_input(input, id_str);
  mvwprintw(main,15,10,"USER ID:   %s",input_ID.c_str());
  wclear(input);
  wrefresh(main);
  wrefresh(input);

  input_password = get_input(input, pw_str);
  //input_password = get_rlinput(input,pw_str);
  mvwprintw(main,20,10,"PASSWORD: (hidden)");
  refresh();
  wclear(input);
  wrefresh(main);
  wrefresh(input);
  clear();
  if(validate_credentials()==FALSE){
    return TRUE;//return true to keep loop running if NOT valid login
  }else{
    set_userID(input_ID);
    return FALSE;//end loop if valid login
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
  //input_ID = get_rlinput(input,id_str);
  mvwprintw(main,15,10,"USER ID:   %s",input_ID.c_str());
  wclear(input);
  create_input_box();
  wrefresh(main);
  wrefresh(input);
  input_password = get_input(input, pw_str);
  //input_password = get_rlinput(input,pw_str);
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
  clear();
  refresh();
  endwin();
}


void menu::init_menu(std::string id){
  //flag for movement keys
  bool flag = TRUE;
  setup();
  set_userID(id);
  //options
  std::string options[4] = {"Join Lobby","Join Room","Create Room","Exit"};
  int choice;
  int highlight = 0;

  while(flag){
    //for loop write options with background color highlight
    for(int i=0; i<4; i++){
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
        if(highlight==4){
          highlight=3;
        }
        break;
      case 10: //KEY_ENTER
        if(highlight==0){
          clear();
          join_lobby();
          flag = FALSE;
        }else if(highlight==1){
          clear();
          join_room();
          flag = FALSE;
        }else if(highlight==2){
          clear();
          create_room();
          flag = FALSE;
        }else if(highlight==3){
          endwin();
          flag = FALSE;
          quit_flag = FALSE;
          continue_flag = FALSE;
        }
    }
    if(choice==27){
      quit_flag=FALSE;
      break;
    }
  }
  endwin();
}

void menu::join_lobby(){
  port_num = "9001";
}

void menu::join_room(){
  setup();
  char jr[] = "Enter room num: ";
  port_num = get_input(input,jr);
  //port_num = get_rlinput(input,jr);

}

void menu::create_room(){
  setup();

}

std::string menu::get_port(){
  return port_num;
}
menu::~menu(){
  endwin();
}
