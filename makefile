CXX=g++

CPPFLAGS=-I/home/saurav/Downloads/boost_1_69_0/

CXXFLAGS=-Wall -O0 -g -std=c++11

all: chat_client chat_server

COMMON_HEADER = chat_message.hpp menu_chat.hpp login.hpp chat_gui.hpp

chat_client.o: ${COMMON_HEADER} chat_client.cpp

menu_chat.o: menu_chat.hpp menu_chat.cpp

login.o: login.hpp login.cpp

chat_gui.o: chat_gui.hpp chat_gui.cpp

chat_client: chat_client.o menu_chat.o login.o chat_gui.o
	${CXX} -o chat_client chat_client.o chat_gui.cpp login.cpp menu_chat.cpp -lpthread -lncurses

chat_server.o: ${COMMON_HEADER} chat_message.hpp chat_server.cpp

chat_server:chat_server.o
	${CXX} -o chat_server chat_server.o -lpthread

clean:
	-rm -f chat_client chat_server chat_client.o chat_server.o
