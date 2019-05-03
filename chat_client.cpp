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

#include "asio.hpp"
#include "chat_message.hpp"
#include <ncurses.h>
#include "ncurse_gui.hpp"
//#include <readline/readline.h>
//#include <readline/history.h>

//extern char *msg_win_str;

using boost::asio::ip::tcp;

//globals
char new_line[chat_message::max_body_length+1+25];
std::string nick;
bool return_to_menu = FALSE;
std::string input_command;
std::string rest_of_message;
std::vector<std::string> blocked_users;

typedef std::deque<chat_message> chat_message_queue;

bool check_mute(char *user);
void add_mute(char *user);
void unmute(char *user);
void self_mute(char *cp_user);

class chat_client: public menu{
public:
  chat_client(boost::asio::io_context& io_context,
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
    boost::asio::post(io_context_,
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
    boost::asio::post(io_context_, [this]() { socket_.close(); });
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
    boost::asio::async_connect(socket_, endpoints,
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
    boost::asio::async_read(socket_,
        boost::asio::buffer(read_msg_.data(), chat_message::header_length),
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
    boost::asio::async_read(socket_,
        boost::asio::buffer(read_msg_.body(), read_msg_.body_length()),
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
    boost::asio::async_write(socket_,
        boost::asio::buffer(write_msgs_.front().data(),
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
  boost::asio::io_context& io_context_;
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

void print_commands(){
  printf("The current commands supported are: \n");
  printf("1.) /nick <nickname> \t (set your display name) \n");
  printf("2.) /mute <username> \t (mutes the user with display name user <username>)\n");
  printf("3.) /unmute <username> \t (unmutes the user with display name user <username>)\n");
  printf("4.) /help \t (prints commands and usage)\n");
  printf("5.) /quit \t (return to menu)\n");
  printf("6.) /exit \t (exit the program)\n");

}

bool check_command(std::string c){
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
}


bool check_mute(char *user){
  std::string temp(user);
  for(auto str : blocked_users){
    if(temp.compare(str)==0){
      //std::cout << "CHECK: " << temp << std::endl;
      return TRUE;
    }
  }
  return FALSE;
}

void unmute(char *user){
  std::string temp(user);
  for(int i =0 ; i < blocked_users.size();i++){
    if(temp.compare(blocked_users.at(i))==0){
      //std::cout << "UNMUTE: " << temp << std::endl;
      blocked_users.erase(blocked_users.begin()+i);//blocked_users
    }
  }
}

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
  ///*
  try{
    boost::asio::io_service io_service;

    tcp::resolver resolver(io_service);
    auto endpoint_iterator = resolver.resolve({LOCAL_HOST, m.get_port()});
    chat_client c(io_service, endpoint_iterator);
    std::thread t([&io_service](){ io_service.run(); });
    char line[chat_message::max_body_length + 1];

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
      if(return_to_menu==TRUE){
        c.close();
        t.join();
        goto menu;
      }
    }

    c.close();
    t.join();

  }catch (std::exception& e){
    std::cerr << "Exception: " << e.what() << "\n";
  }//*/





  /*
  try
  {
    Mymenu Mymenu;

    boost::asio::io_service io_service;
    std::string port_num = "9000";
  	std::string LOCAL_HOST = "127.0.0.1";
    tcp::resolver resolver(io_service);
    auto endpoint_iterator = resolver.resolve(LOCAL_HOST, port_num);
    chat_client c(io_service, endpoint_iterator);

    std::thread t([&io_service](){ io_service.run(); });

    char line[chat_message::max_body_length + 1];
    while (std::cin.getline(line, chat_message::max_body_length + 1))
    {
      chat_message msg;
      msg.body_length(std::strlen(line));
      std::memcpy(msg.body(), line, msg.body_length());
      msg.encode_header();
      c.write(msg);
    }

    c.close();
    t.join();


    boost::boost::asio::io_context io_context;

	       InternetProtocol::endpoint endpoint_type

		endpoint object; type defined in "basic_resolver.hpp"




		the resolver object has been created

		so the resolver object should reach out to function inside
			the resolver class to the "resolve" function

	"resolve()"
* This function is used to resolve a query into a list of endpoint entries.
   *
   * @param q A query object that determines what endpoints will be returned.
   *
   * @returns A range object representing the list of endpoint entries. A
   * successful call to this function is guaranteed to return a non-empty
   * range.
   *
   * @throws boost::system::system_error Thrown on failure.

  results_type resolve(const query& q)
  {
    boost::system::error_code ec;
    results_type r = this->get_service().resolve(
        this->get_implementation(), q, ec);
    boost::boost::asio::detail::throw_error(ec, "resolve");
    return r;
  }

results_type resolve(const query& q, boost::system::error_code& ec)
  {
    return this->get_service().resolve(this->get_implementation(), q, ec);
  }

results_type resolve(BOOST_boost::asio_STRING_VIEW_PARAM host,
      BOOST_boost::asio_STRING_VIEW_PARAM service)
  {
    return resolve(host, service, resolver_base::flags());
  }



	 we will make this starting port number
	std::string user_chatroom = "9000";
  std::string user_created_room = "9000";
	std::string LOCAL_HOST = "127.0.0.1";


	tcp::resolver resolver(io_context);


		WHEN THE PROGRAM NEEDS TO RUN A NEW SECTION OF THIS CODE (i.e. USER CREATES A NEW CHATROOM)
		REMEMBER THE RESOLVE FUNCTION TAKES 2 STRING LITERALS AS ARGUMENTS; EXPLANATION FOR
		RESOLVE() IS STATED ABOVE




    ./chat_client 127.0.0.1 9000
    Hello
    Hello



	 WE MIGHT BE ABLE TO DISCARD THE AUTO KEYWORD HERE TO POSSIBLY
	 DO SOME SORT OF SMART POINTER WORK HERE AND STORE EACH THREAD
	 AND CHAT_CLIENT OBJECT IN SOME SORT OF CONTAINER AND PULL
	 THEM OUT ONLY WHEN NEEDED BY USER

    auto endpoints = resolver.resolve(LOCAL_HOST, port_num);
    chat_client c(io_context, endpoints);
    std::thread t([&io_context](){ io_context.run(); });
    char line[chat_message::max_body_length + 1];



		Objectives of Friday:

			=> Get a super basic program up and running.

			=> this includes something to the effect of the code below

			=> these 3 options should be focused on first

			=>

    char compare_string[100];
  std::cout << "WELCOME TO SUPERCHAT!\n";
    //char input[50];
    int user_choice; // will be a number between 1-3

    int quit_loop = 0;
    std::cout << "Hello, welcome to SUPERCHAT! What would you like to do:\n";
    while ( !quit_loop )
    {
      Mymenu.print_menu();
      while ( user_choice != 4)
      {
        std::cout << "\n\n<sUpErChAt> ";
        std::cin >> user_choice;
        if (user_choice==1)//Enter Lobby
        {
          user_created_room = "9000";
          std::cout<<"****WELCOME TO LOBBY****";
        }
        if(user_choice==2)//enter chatroom
        {
          //show available chatrooms
          int room_available = Mymenu.enter_rooms(user_created_room);//if enter chatroom send 1;if create chatroom send 0;
          //if 1 check if matches avaiable chatrooms and print available chatrooms;
          while(room_available==0)
          {
          std::cout<<"No such chatroom exist";
          room_available = Mymenu.enter_rooms(user_created_room);
          }
          if(room_available>0)
          {
          std::cout<<"Room"<<user_created_room;
          }
          }
        if (user_choice==3)//create chatroom
        {
          int room_available =Mymenu.create_rooms(user_created_room );
          if (room_available==1)//successful
          {
            std::cout<<"Chatroom"<<user_created_room<<"created\n";
          }
        while(room_available!=1)
        {   std::cout<<"Chatroom Already exist:\n";
            std::cout<<"Enter a chatroom number again:\n";
            std::cin>>user_created_room;
            room_available = Mymenu.create_rooms(user_created_room );
          }
        }
          user_chatroom=user_created_room;
          auto endpoints = resolver.resolve(LOCAL_HOST, user_chatroom);
          chat_client c(io_context, endpoints);
          std::thread t([&io_context](){ io_context.run(); });
          // "line" user input BEFORE it is sent to other user
          char line[chat_message::max_body_length + 1];
          while (std::cin.getline(line, chat_message::max_body_length + 1))
    	    {
    			    chat_message msg;
    	      	msg.body_length(std::strlen(line));
    	      	std::memcpy(msg.body(), line, msg.body_length());

              std::strcpy(compare_string, line);
    	      	msg.encode_header();
    			//msg.message_to_server();
              if ( strcmp(compare_string,"/exit")==0)
              {
                break;
              }
    			c.write(msg);
          //std::string compare_string = c.write(msg);
    	    }

          c.close();
          // waits for thread to finish
          t.join();
          // ************************* //
        }
        continue;
      }

      quit_loop = 0;

    }


  catch (std::exception& e)
    {
      std::cerr << "Exception: " << e.what() << "\n";
    }
  */
  return 0;
}
