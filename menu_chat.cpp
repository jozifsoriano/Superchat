#include <iostream>
#include <cstring>
#include "menu_chat.hpp"

ChatroomArray[chatroomcounter]="9000";

void Mymenu::print_menu()
{
  std::string menuArray[10];
  menuArray[0]= "1.Enter Lobby";
  menuArray[1]= "2.Enter Chatroom";
  menuArray[2]= "3.Create Chatroom";
  menuArray[3]= "4.Quit";
  for(int i=0; i<4; i++)
  {
    std::cout<<menuArray[i]<<"\n";
  }

}

int Mymenu::create_rooms(std::string user_created_room)
{

  int x=0;
  int j =0;
  std::cout<<"Available Chatrooms:\n";
  //how do we update the chatrooms when new window is opened ????
  for(j =0; j <chatroomcounter+1;j++)//compare if chatroom already exist
  {
    std::cout<<ChatroomArray[j]<<"\n";
  }
  std::cout<<"Enter a room number to create: ";
  std::cin>>user_created_room;
  for( j =0; j <chatroomcounter+1;j++)
  {
        if (user_created_room==ChatroomArray[j])
    {
      x++;//if exist increase x;
    }
  }
  if (x==0)//doesnot already exist
  {
    chatroomcounter++;
    ChatroomArray[chatroomcounter]=user_created_room;//add chatroom to array
    return 1;//chatroom create successful
  }
  else
  {
    return 0;//
  }
}


int Mymenu::enter_rooms(std::string user_created_room)
{
  int x =0;
  int j = 0;
  std::cout<<"Available chatrooms\n";
  for(j =0; j <chatroomcounter+1;j++)
  {
    std::cout<<ChatroomArray[j]<<"\n";
  }
  std::cout<<"Enter chatroom number from available room to enter:";
  std::cin>>user_created_room;
  for( j =0; j <chatroomcounter+1;j++)
  {
    if (user_created_room==ChatroomArray[j])
    {
      x++;
    }
  }
  if (x==0)//chatroom doesnot exist
  {
    std::cout<<"Chatroom Doeesnot exist\n";
    return 0;//enter chatroom unsuccessful;]
  }
  else
  {
    return 1;
  }
}
