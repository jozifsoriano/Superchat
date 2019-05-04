CXX=g++

CPPFLAGS=-I/Users/josephsoriano/Downloads/boost_1_69_0/

CXXFLAGS=-Wall -O0 -g -std=c++11

all: chat_client chat_server

COMMON_HEADER = chat_message.hpp ncurse_gui.hpp

chat_client.o: $(COMMON_HEADER) chat_client.cpp

chat_gui.o:  $(COMMON_HEADER) chat_gui.cpp

chat_client:chat_client.o chat_gui.o
	$(CXX) -o chat_client chat_client.o chat_gui.o -lncurses -lpthread -lreadline

chat_server.o: ${COMMON_HEADER} chat_message.hpp chat_server.cpp

chat_server:chat_server.o
	$(CXX) -o chat_server chat_server.o -lpthread

runs:
	./chat_server

runc:
	./chat_client

clean:
	-rm -f chat_client chat_server chat_client.o chat_server.o chat_gui.o 
