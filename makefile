CXX=g++

CPPFLAGS=-I/Users/josephsoriano/Downloads/boost_1_69_0/

CXXFLAGS=-Wall -O0 -g -std=c++11 -lncurses -lpthread

all: chat_client chat_server

COMMON_HEADER = chat_message.hpp ncurse_gui.hpp

chat_client.o: $(COMMON_HEADER) chat_client.cpp

chat_gui.o:  $(COMMON_HEADER) chat_gui.cpp

chat_client:chat_client.o chat_gui.o
	$(CXX) -o chat_client chat_client.o chat_gui.cpp -lncurses -lpthread

chat_server.o: ${COMMON_HEADER} chat_message.hpp chat_server.cpp

chat_server:chat_server.o
	$(CXX) -o chat_server chat_server.o -lpthread

practice:
	$(CXX) chat_gui.cpp -lncurses #-lreadline #ncurse_gui.hpp

runp:
	./a.out

runs:
	./chat_server

runc:
	./chat_client

directories:
	-mkdir bin
	-mkdir src
	-mkdir res
	-mkdir include
	-mkdir obj


clean:
	-rm -f chat_client chat_server chat_client.o chat_server.o a.out
	-rm -rf bin src res include obj
