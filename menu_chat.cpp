#include <iostream>
#include <cstring>
#include "menu_chat.hpp"
#include <fstream>
#include <vector>
#include<iterator>


void Mymenu::print_menu()
{
  int j =0;
  std::string line;
  std::string menuArray[10];
  menuArray[0]= "1.Enter CHATROOM";
  menuArray[1]= "2.Create CHATROOM";
  menuArray[2]= "3.Delete CHATROOM";
  menuArray[3]= "4.Quit";
  for(int i=0; i< 4; i++)
  {
    std::cout<<menuArray[i]<<"\n";
  }

  std::ifstream myfile("chatroom.txt");
  if (myfile.is_open())
   {
     while ( getline (myfile,line) )
     {
        ChatroomArray.insert(ChatroomArray.end(),line);
         j++;
     }
     myfile.close();
   }
   chatroomcounter=j;
   std::cout<<"Available Chatrooms: "<< chatroomcounter <<"\n";
   for (std::vector<std::string>::iterator it = ChatroomArray.begin(); it != ChatroomArray.end(); ++it)
   std::cout << *it<<"\n";
}

std::string Mymenu::create_rooms(std::string user_created_room)
{

  int x=0;
  //int j =0;
  std::string line;
  std::cout<<"Enter a room number to create: ";
  std::cin>>user_created_room;
  for (std::vector<std::string>::iterator it = ChatroomArray.begin(); it != ChatroomArray.end(); ++it)
       {
        if (user_created_room== *it)
    {
      x++;//if exist increase x;
    }
  }
  if (x==0)//doesnot already exist
  {
    chatroomcounter++;
    ChatroomArray.push_back(user_created_room);//add chatroom to array
    std::ofstream myfile ("chatroom.txt");
 if (myfile.is_open())
 {
   for (std::vector<std::string>::iterator it = ChatroomArray.begin(); it != ChatroomArray.end(); ++it)
        {
   myfile << *it <<"\n";
    }
   myfile.close();
 }
 else std::cout << "Unable to save chatroom";
 std::cout<<"Chatroom"<< user_created_room <<"created\n";
  //chatroom create successful
  }
  else
  {
  while(x!=1)
    {   std::cout<<"Chatroom Already exist:\n";
      std::cout<<"Enter a chatroom number again:\n";
      std::cin>>user_created_room;
      create_rooms(user_created_room );
    }
  }
  return user_created_room;
}


std::string Mymenu::enter_rooms(std::string user_created_room)
{
  int x =0;
  //int j = 0;
  std::string line;
  std::cout<<"Enter chatroom number from available room to enter:";
  std::cin>> user_created_room;
  for (std::vector<std::string>::iterator it = ChatroomArray.begin(); it != ChatroomArray.end(); ++it)
  {
      if (user_created_room== *it)
      {
        x++;
      }
  }
  if(x==0)//chatroom doesnot exist
  {
    std::cout<<"Chatroom Doeesnot exist\n";
    std::cout<<"Enter from available list";
    std::cin>>user_created_room;
    enter_rooms(user_created_room);
  }
  return user_created_room;

}

std::string Mymenu::delete_rooms(std::string user_created_room)
{
  int x =0;
  //int j = 0;
  std::string line;
  std::string newchatroom[20];
//  std::string oline;
std::cout<<"Enter a chatroom number to delete: ";
std::cin>>user_created_room;
for (std::vector<std::string>::iterator it = ChatroomArray.begin(); it != ChatroomArray.end(); ++it)
     {
       if (user_created_room==*it)
       {
         ChatroomArray.erase(it);
         chatroomcounter--;
         x++;
         std::cout<<"delete successful";
         break;
       }
     }

if (x>0)
{
  std::ofstream myfile("chatroom.txt");
  if (myfile.is_open())
  {
    for (std::vector<std::string>::iterator it = ChatroomArray.begin(); it != ChatroomArray.end(); ++it)
         {
            myfile << *it <<"\n";
         }
  myfile.close();
  }
}
while(x==0)
{
  std::cout<<"No such chatroom exist";
  std::cout<<"Enter from available chatrooms";
  std::cin>>user_created_room;
  delete_rooms(user_created_room);
}
return user_created_room;
}
