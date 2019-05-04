//
// chat_client.cpp
// ~~~~~~~~~~~~~~~
//
// Copyright (c) 2003-2018 Christopher M. Kohlhoff (chris at kohlhoff dot com)
//
// Distributed under the Boost Software License, Version 1.0. (See accompanying
// file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
//
/*
#include <string.h>
#include <cstring>
#include <cstdlib>
#include <deque>
#include <iostream>
#include <thread>
#include "asio.hpp"
#include <ncurses.h>
#include "chat_message.hpp"
#include "ncurse_gui.hpp"
*/
#include <cstdlib>
#include <deque>
#include <iostream>
#include <thread>
#include <ctime>
#include <math.h>
#include "asio.hpp"
#include "chat_message.hpp"
#include <ncurses.h>
#include "ncurse_gui.hpp"
//#include <readline/readline.h>
//#include <readline/history.h>

//extern char *msg_win_str;

using asio::ip::tcp;

//globals
char new_line[chat_message::max_body_length+1+25];
std::string nick;
bool return_to_menu = FALSE;
bool change_room = FALSE;
std::string port_num;
std::string input_command;
std::string rest_of_message;
std::vector<std::string> blocked_users;
time_t start;

//func defs
bool check_mute(char *user);
void add_mute(char *user);
void unmute(char *user);
void self_mute(char *cp_user);
void sec_to_min(int s, bool check);

int min(int x, int y, int z){
	int a = (int)fmin(x,y); //x+y
	int b = (int)fmin(a,z); //x+y+z
	return b;
}

int edit_distance(char * first_string, char * second_string, int print_table){
	//get length of strings

	int fslen = strlen(first_string);
	int sslen = strlen(second_string);

	int table[fslen+1][sslen+1];
	//print table
	if(print_table==1){
		printf("%3c|%3c|",' ', ' ');
		for(int x = 0; x<sslen; x++){
			printf("%3c|",second_string[x]);
		}
		printf("\n");
		for(int xx = 0;xx<sslen+1;xx++){
			printf("------");
		}
		printf("\n");
		for(int i = 0; i<=fslen; i++){
			if(i>0){
				printf("%3c|",first_string[i-1]);
			}else{
				printf("%3c|",' ');
			}
			for(int j = 0; j<=sslen; j++){
				if(i==0){
					table[i][j] = j;
				}else if(j==0){
					table[i][j]= i;
				}else if(first_string[i-1] == second_string[j-1]){
					table[i][j]=table[i-1][j-1];
				}else{
					table[i][j] = 1+min(table[i][j-1], table[i-1][j], table[i-1][j-1]);
				}
				printf("%3d|",table[i][j]);
			}
			printf("\n");
			for(int l = 0;l<sslen+1;l++){
				printf("------");
			}
			printf("\n");
		}
	}else{
		for(int i = 0; i<=fslen; i++){
			for(int j = 0; j<=sslen+1; j++){
				if(i==0){
					table[i][j] = j;
				}else if(j==0){
					table[i][j]= i;
				}else if(first_string[i-1] == second_string[j-1]){
					table[i][j]=table[i-1][j-1];
				}else{
					table[i][j] = 1+min(table[i][j-1], table[i-1][j], table[i-1][j-1]);
				}
			}
		}
	}
	//printf("***ED CALL: %d***\n",table[fslen][sslen]);
	return table[fslen][sslen];  // replace this line with your code
}

void spellcheck(char *teststring){
	FILE *dict;
	char dictstring[100], closestword[100];
	int  temp_ed;

	int  min_ed=100;
	dict = fopen("words.txt","r");
	//printf("\nWORD TO CHECK: %s \n----------------------\n",teststring);
	if(!dict){
		perror("No such dictionary file.");
	}
	while(fgets(dictstring,sizeof(dictstring),dict)){
		int temp_ed;
		//printf("MIN_ED: %d\n",min_ed);
		temp_ed = edit_distance(dictstring,teststring,0);
		//printf("CURRENT: %s\t%s\t***%d***\n",teststring,dictstring,temp_ed);
		if(temp_ed <= min_ed){
			//printf("CLOSEST_WORD: %s\n",closestword);
			min_ed = temp_ed;
			//printf("MIN_ED: %d\n",min_ed);
		}
	}
	printf("MINIMUM EDIT DISTANCE: %d\n----------------------\n",min_ed);
	fclose(dict);
	dict = fopen("words.txt","r");
	while(fgets(dictstring,sizeof(dictstring),dict)){
		temp_ed = edit_distance(dictstring,teststring,0);
		if(min_ed == temp_ed){
			strcpy(closestword, dictstring);
			printf("Did you mean: %s \n",closestword);
		}
	}
	printf("==================================\n\n\n");
	fclose(dict);

}


typedef std::deque<chat_message> chat_message_queue;

class chat_client: public menu{
public:
  chat_client(asio::io_context& io_context,
      const tcp::resolver::results_type& endpoints)
    : io_context_(io_context),
      socket_(io_context)
  {
    do_connect(endpoints);
  }

/* void init_client(std::string id){
    setup();
    set_userID(id);
  }*/
  void write(const chat_message& msg)
  {
    asio::post(io_context_,
        [this, msg]()
        {
          bool write_in_progress = !write_msgs_.empty();
          write_msgs_.push_back(msg);
          if (!write_in_progress)
          {
            do_write();
          }
        });
  }

  void close()
  {
    asio::post(io_context_, [this]() { socket_.close(); });
  }
/*void print_recent_msgs(){

  }

  void print_users(){

  }

  void init_room(){
    setup();
  }
  void run_client(){
    std::thread t([&io_service](){ io_service.run(); });

    char line[chat_message::max_body_length + 1];

    //while (std::cin.getline(line, chat_message::max_body_length + 1)){
    while(c.continue_flag){
      chat_message msg;
      msg.body_length(std::strlen(line));
      std::string sline;
      sline = c.get_input();
      std::memcpy(msg.body(), line, msg.body_length());
      msg.encode_header();
      c.write(msg);

    }

    c.close();
    t.join();
  }*/

  std::string name;
private:
  void do_connect(const tcp::resolver::results_type& endpoints)
  {
    asio::async_connect(socket_, endpoints,
        [this](std::error_code ec, tcp::endpoint)
        {
          if (!ec)
          {
            do_read_header();
          }
        });
  }

  void do_read_header()
  {
    asio::async_read(socket_,
        asio::buffer(read_msg_.data(), chat_message::header_length),
        [this](std::error_code ec, std::size_t length)
        {
          if (!ec && read_msg_.decode_header())
          {
            do_read_body();
          }
          else
          {
            socket_.close();
          }
        });
  }

  void do_read_body()
  {
    asio::async_read(socket_,
        asio::buffer(read_msg_.body(), read_msg_.body_length()),
        [this](std::error_code ec, std::size_t /*length*/)
        {
          if (!ec)
          {
            // Want to make a very clean transition between
            // the data coming in and the string handed to ncurses.
            // More complicated than it needs to be, but a high
            // assurance that no extra characters get displayed
            char *outline = (char*)malloc(read_msg_.body_length() + 1);
            char *cp_user=(char*)malloc(15*sizeof(char));
            char *cp_rom=(char*)malloc(150*sizeof(char));


            //printf("DEBUG outline: %s \n", outline);
            //printf("DEBUG uid: %s \n",cp_user);

            free(cp_rom);
            memset(outline,'\0',read_msg_.body_length() + 1);
            memcpy(outline,read_msg_.body(),read_msg_.body_length());
            std::string temp(outline);
            //std::cout << "DEBUG STRING: " << temp << std::endl;
            //printf("DEBUG outline: %s \n", outline);
            //printf("DEBUG uid: %s \n",cp_user);
            sscanf(temp.c_str(),"%s %150[^\n]",cp_user, cp_rom);
            //mvwprintw(main,1,1,outline);
            //wrefresh(main);
            //printw(outline);
            free(outline);

            //if user blocked
            if(!check_mute(cp_user)){
              //printf("DEBUG NOT MUTED\n");
              std::cout.write(read_msg_.body(), read_msg_.body_length());
              std::cout << "\n";
            }
            free(cp_user);
            do_read_header();
          }
          else
          {
            socket_.close();
          }
        });
  }

  void do_write()
  {
    asio::async_write(socket_,
        asio::buffer(write_msgs_.front().data(),
          write_msgs_.front().length()),
        [this](std::error_code ec, std::size_t length)
        {
          if (!ec)
          {
            write_msgs_.pop_front();
            if (!write_msgs_.empty())
            {
              do_write();
            }
          }
          else
          {
            socket_.close();
          }
        });
  }

private:
  asio::io_context& io_context_;
  tcp::socket socket_;
  chat_message read_msg_;
  chat_message_queue write_msgs_;
};

void add_user_to_msg(char line[],std::string userID){
  char *uid =&userID[0u];
  //printf("DEBUG: USER ID %s\n",uid);
  int i=0, j=0;
  while(uid[i]!='\0'){
    new_line[i] = uid[i];
    //printf("%d %c %c \n", i, new_line[i],uid[i]);
    i++;
  }
  new_line[i] = 58;//:
  i++;
  new_line[i] = 32;//[space]
  i++;
  while(line[j]!='\0'){
    new_line[i+j]=line[j];
    //printf("%d  %d %c %c", i, i+j, new_line[i+j],line[j]);
    j++;
  }
  //printf("DEBUG: NEWLINE: %s\n",new_line);
}

void print_commands(){ //  /help
  printf("The current commands supported are: \n");
  printf("1.) /nick <nickname> \t (set your display name) \n");
  printf("2.) /mute <username> \t (mutes the user with display name user <username>)\n");
  printf("3.) /unmute <username> \t (unmutes the user with display name user <username>)\n");
  printf("4.) /change <port_num> \t (changes current chatroom)\n");
  printf("5.) /spellcheck <word> \t (compares word to file, recommend similar words \n)");
  printf("6.) /uptime \t (checks how long a user has been in a room)");
  printf("7.) /help \t (prints commands and usage)\n");
  printf("8.) /quit \t (return to menu)\n");
  printf("9.) /exit \t (exit the program)\n");

}


bool check_command(std::string c){ //checks if input line is a command (begins with '/')
  char *cp_command=(char*)malloc(15*sizeof(char));
  char *cp_rom=(char*)malloc(150*sizeof(char));
  if(c[0] == '/'){
    sscanf(c.c_str(),"%s %150[^\n]",cp_command, cp_rom );
    //printf("command: %s \n rom: %s\n",cp_command,cp_rom );
    if(strcmp(cp_command,"/quit")==0){
      printf("RETURNING \n");
      return_to_menu=TRUE;
    }else if(strcmp(cp_command,"/exit")==0){
      exit(0);
    }else if(strcmp(cp_command,"/help")==0){
      print_commands();
    }else if(strcmp(cp_command,"/nick")==0){
      char cp_nick[15];
      sscanf(cp_rom,"%s",cp_nick);
      std::string temp(cp_nick);
      nick = temp;
      printf("Your new display name is %s. \n",cp_nick);
    }else if(strcmp(cp_command,"/mute")==0){
      char cp_user[15];
      char new_user[16];
      sscanf(cp_rom,"%s",cp_user);
      //printf("DEBUG: MUTING %s\n",cp_user);
      int i = 0;
      while (cp_user[i]!= '\0') {
        new_user[i] = cp_user[i];
        i++;
      }
      new_user[i] = 58;//:
      i++;
      //printf("DEBUG: NEW USER -> \t %s",new_user);
      add_mute(new_user);
    }else if(strcmp(cp_command,"/unmute")==0){
      char cp_user[15];
      char new_user[16];
      sscanf(cp_rom,"%s",cp_user);
      //printf("DEBUG: MUTING %s\n",cp_user);
      int i = 0;
      while (cp_user[i]!= '\0') {
        new_user[i] = cp_user[i];
        i++;
      }
      new_user[i] = 58;//:
      i++;
      unmute(new_user);
    }else if(strcmp(cp_command,"/change")==0){
      char port[15];
      bool cant_change = FALSE;
      sscanf(cp_rom,"%s",port);
      for(int i = 0; i < 11; i++){
        std::string options[10] = {"9000","9001","9002","9003","9004","9005","9006","9007","9008","9009"};
        if(strcmp(options[i].c_str(),cp_rom)==0){
          cant_change=TRUE;
          std::string temp(port);
          port_num = temp;
          break;
        }
      }
      if(cant_change==FALSE){
        printf("!!Error!! Cannot change to room %s. \n", cp_rom);
      }else{
        std::cout << "You are now in room " << port_num << std::endl;
        change_room = TRUE;
      }
      //printf("DEBUG: port %s \n",port);

    }else if(strcmp(cp_command,"/spellcheck")==0){
      char word[150];
      sscanf(cp_rom,"%s",word);
      spellcheck(word);
    }else if(strcmp(cp_command,"/uptime")==0){
      double duration = difftime( time(0), start);
      bool min_check = FALSE;
      char min[4];
      sscanf(cp_rom,"%s",min);
      if(strcmp(min,"min")==0){
        min_check = TRUE;
      }
      sec_to_min((int)duration,min_check);
    }else{
      printf("%s !!NOT A RECOGNIZED COMMAND!!\n", cp_command);
      printf("Type '/help' to get command list.\n");
    }
    free(cp_command);
    free(cp_rom);
    return TRUE; //if command do not send
  }else{
    free(cp_command);
    free(cp_rom);
    return FALSE; //if not command, send
  }

}
void add_mute(char *user){
  std::string temp(user);
  for(auto str : blocked_users){
    if(temp.compare(str)==0){
      printf("!!User Already Blocked!! \n");
      std::cout << "ADD: " << temp << std::endl;
      return;
    }
  }
  blocked_users.push_back(temp);
  printf("%s is now blocked.\n", user);
}//js


bool check_mute(char *user){
  std::string temp(user);
  for(auto str : blocked_users){
    if(temp.compare(str)==0){
      //std::cout << "CHECK: " << temp << std::endl;
      return TRUE;
    }
  }
  return FALSE;
}//js

void unmute(char *user){
  std::string temp(user);
  for(int i =0 ; i < blocked_users.size();i++){
    if(temp.compare(blocked_users.at(i))==0){
      //std::cout << "UNMUTE: " << temp << std::endl;
      blocked_users.erase(blocked_users.begin()+i);//blocked_users
      printf("%s is now unblocked.\n", user);
    }
  }
}//js

void self_mute(std::string nick){
  char new_user[16];
  int i = 0;
  while (nick[i]!= '\0') {
    new_user[i] = nick[i];
    i++;
  }
  new_user[i] = 58;//:
  i++;
  add_mute(new_user);
}//js

void sec_to_min(int s, bool check){
  if(check){
    int ss, mm;
    ss = s%60;
    mm = ss/60;
    printf("Current uptime is %d minutes and %d seconds. \n", mm, ss);
  }else{
    printf("Current uptime is %d(s) \n", s);
    return;
  }
}

//-------------------------------------------------
int main(int argc, char* argv[]){
  //main variables
  bool running = TRUE;
  std::string LOCAL_HOST = "127.0.0.1";
  menu m;
  //run login screen
  //continues when logged in or quits when quit_flag is false
  login l;
  if (l.quit_flag == FALSE){
    return 0;
  }
  if(l.get_user().compare("")==0){
    nick="Guest";
  }else{
    nick = l.get_user();
  }
  self_mute(nick);
menu:
  return_to_menu = FALSE;
  running = TRUE;
  while(running){
    m.init_menu(l.get_user());
    running = m.continue_flag;
    if(m.quit_flag == FALSE){
      return 0;
    }
    erase();
  }
  system("clear");
  while(1){
    //printf("DEBUG START OF BIG LOOP \n");
    start = time(0);
    if(change_room == TRUE){
      change_room = FALSE;
    }else{
      port_num = m.get_port();
    }
    ///*
    try{
      asio::io_service io_service;

      tcp::resolver resolver(io_service);
      auto endpoint_iterator = resolver.resolve({LOCAL_HOST, port_num});
      chat_client c(io_service, endpoint_iterator);


      std::thread t([&io_service](){ io_service.run(); });
      char line[chat_message::max_body_length + 1];
      printf("You are in room: %s \n",port_num.c_str());

      //std::string sline;
      //c.init_client(l.get_user());
      //char *uid =&c.get_user()[0u];
      while (std::cin.getline(line, chat_message::max_body_length + 1)){
      //while(c.continue_flag){
        std::string temp(line);
        bool can_send = check_command(temp);
        if(!can_send){//false for not command
          chat_message msg;
          add_user_to_msg(line,nick);
          //msg.body_length(std::strlen(line));
          msg.body_length(std::strlen(new_line));
          //sline = c.get_input(c.get_inputw(),uid);
          //sline = get_rlinput(c.get_inputw(),uid);
          //msg.body_length(sline.length());
          std::memcpy(msg.body(), new_line, msg.body_length());
          msg.encode_header();
          c.write(msg);
          memset(new_line, 0, sizeof(new_line));
        }
        if(change_room==TRUE){
          c.close();
          t.join();
          //printf("CHANGING ROOM \n");
          break;
        }
        if(return_to_menu==TRUE){
          c.close();
          t.join();
          goto menu;
        }
      }
      if(change_room==FALSE){
        c.close();
        t.join();
      }


    }catch (std::exception& e){
      std::cerr << "Exception: " << e.what() << "\n";
    }//*/

  }

  return 0;
}
