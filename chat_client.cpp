//
// chat_client.cpp
// ~~~~~~~~~~~~~~~
//
// Copyright (c) 2003-2018 Christopher M. Kohlhoff (chris at kohlhoff dot com)
//
// Distributed under the Boost Software License, Version 1.0. (See accompanying
// file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
//
#include <string.h>
#include <cstdlib>
#include <deque>
#include <iostream>
#include <thread>
#include "asio.hpp"
#include <ncurses.h>
#include "chat_message.hpp"
#include "ncurse_gui.hpp"

using asio::ip::tcp;

typedef std::deque<chat_message> chat_message_queue;

class chat_client
{
public:
  chat_client(asio::io_context& io_context,
      const tcp::resolver::results_type& endpoints)
    : io_context_(io_context),
      socket_(io_context)
  {
    do_connect(endpoints);
  }

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
        [this](std::error_code ec, std::size_t /*length*/)
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
            std::cout.write(read_msg_.body(), read_msg_.body_length());
            std::cout << "\n";
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
        [this](std::error_code ec, std::size_t /*length*/)
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



int main(int argc, char* argv[])
{
  //login l;
  menu m;
  /*
  try
  {

    asio::io_service io_service;
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


    boost::asio::io_context io_context;

	/*       InternetProtocol::endpoint endpoint_type           */
	/*
		endpoint object; type defined in "basic_resolver.hpp"
	*/

	/*

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
    boost::asio::detail::throw_error(ec, "resolve");
    return r;
  }

results_type resolve(const query& q, boost::system::error_code& ec)
  {
    return this->get_service().resolve(this->get_implementation(), q, ec);
  }

results_type resolve(BOOST_ASIO_STRING_VIEW_PARAM host,
      BOOST_ASIO_STRING_VIEW_PARAM service)
  {
    return resolve(host, service, resolver_base::flags());
  }

	*/

	/* we will make this starting port number */
	std::string user_chatroom = "9000";
  std::string user_create_room = "9000";
	std::string LOCAL_HOST = "127.0.0.1";

	tcp::resolver resolver(io_context);
	/*

		WHEN THE PROGRAM NEEDS TO RUN A NEW SECTION OF THIS CODE (i.e. USER CREATES A NEW CHATROOM)
		REMEMBER THE RESOLVE FUNCTION TAKES 2 STRING LITERALS AS ARGUMENTS; EXPLANATION FOR
		RESOLVE() IS STATED ABOVE

	*/

  /*
    ./chat_client 127.0.0.1 9000
    Hello
    Hello

  */

	/* WE MIGHT BE ABLE TO DISCARD THE AUTO KEYWORD HERE TO POSSIBLY */
	/* DO SOME SORT OF SMART POINTER WORK HERE AND STORE EACH THREAD */
	/* AND CHAT_CLIENT OBJECT IN SOME SORT OF CONTAINER AND PULL     */
	/* THEM OUT ONLY WHEN NEEDED BY USER                             */
    /*
    auto endpoints = resolver.resolve(LOCAL_HOST, port_num);
    chat_client c(io_context, endpoints);
    std::thread t([&io_context](){ io_context.run(); });
    char line[chat_message::max_body_length + 1];
    */
	/*

		Objectives of Friday:

			=> Get a super basic program up and running.

			=> this includes something to the effect of the code below

			=> these 3 options should be focused on first

			=>
	*/
    char compare_string[100];
  std::cout << "WELCOME TO SUPERCHAT!\n";
    //char input[50];
    int user_choice; // will be a number between 1-3

    int quit_loop = 1;
    while ( quit_loop )
    {
      std::cout << "Hello, welcome to SUPERCHAT! What would you like to do:\n";
      std::cout << "1.) Create a chat\n2.) Enter a room\n3.) Leave(quit) SUPERCHAT\n";
      while ( user_choice != 3)
      {
        std::cout << "\n\n<sUpErChAt> ";
        std::cin >> user_choice;

        if ( user_choice == 1)
        {
          //for now lets make only 2 chatrooms lobby 9000 and another user_chatroom
          std::cout<<"Enter a room number to create new chatroom";
          std::cin>>user_create_room;

          auto endpoints = resolver.resolve(LOCAL_HOST, user_create_room);
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


        }

        else if ( user_choice == 2)
        {
          std::cout << "\nEnter room number: ";
          std::cin >> user_chatroom;
          //for now lets suppose there are only 2 chatrooms lobby and another
          if(user_chatroom != "9000" || user_chatroom != user_create_room)
          {
            std::cout<<"No such chatroom exist\n";
            std::cout<<"available chatrooms:\n"<<"9000(lobby)\n"<<user_create_room<<"\n";
            std::cout<<"Enter room number from available rooms:";
            std::cin>>user_chatroom;
          }
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
      }
      continue;
      quit_loop = 0;

    }

  }
  catch (std::exception& e)
    {
      std::cerr << "Exception: " << e.what() << "\n";
    }
  */
  return 0;
}
