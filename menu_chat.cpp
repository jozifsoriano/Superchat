#include <iostream>
#include <cstring>
#include "menu_chat.hpp"
#include <fstream>



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
         ChatroomArray[j]=line;
         j++;
     }
     myfile.close();
   }
   chatroomcounter=j;
}

std::string Mymenu::create_rooms(std::string user_created_room)
{

  int x=0;
  int j =0;
  std::string line;
  std::cout<<"Available Chatrooms:\n";
  for( j =0; j <chatroomcounter+1;j++)
  {
    std::cout<<ChatroomArray[j]<< "\n";
  }
  std::cout<<"Enter a room number to create: ";
  std::cin>>user_created_room;
  for( j =0; j < chatroomcounter+1;j++)
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
    std::ofstream myfile ("chatroom.txt");
 if (myfile.is_open())
 {
   for( j =0; j <chatroomcounter+1;j++)
   {
   myfile << ChatroomArray[j]<<"\n";
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
  int j = 0;
  std::string line;
  std::cout<<"Available chatrooms\n";
  std::ifstream myfile("chatroom.txt");
  if (myfile.is_open())
   {
     while ( getline (myfile,line) )
     {
       std::cout << line << '\n';
       for(j =0; j <chatroomcounter+1;j++)
       {
         ChatroomArray[j]=line;
       }
     }
     myfile.close();
   }
  std::cout<<"Enter chatroom number from available room to enter:";
  std::cin>> user_created_room;
  for( j=0; j < chatroomcounter+1;j++)
  {
    if (user_created_room==ChatroomArray[j])
    {
      x++;
    }
  }
  while(x==0)//chatroom doesnot exist
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
  int j = 0;
  std::string line;
  std::string newchatroom[20];
//  std::string oline;
std::cout<<"Available Chatrooms:\n";
for( j =0; j <chatroomcounter+1;j++)
{
  std::cout<<ChatroomArray[j]<< "\n";
}
std::cout<<"Enter a chatroom number to delete";
std::cin>>user_created_room;
std::ofstream myfile("chatroom.txt");
if (myfile.is_open())
{
for( j=0; j <chatroomcounter+1;j++)
{
  if(user_created_room==ChatroomArray[j])
  {
    x++;
    ChatroomArray[j]=ChatroomArray[j+1];
    ChatroomArray[j+1]="";
  }
  else
  {
     myfile << ChatroomArray[j] <<"\n";
     newchatroom[j]=ChatroomArray[j];
  }
}
myfile.close();
}
if (x>0)
{
  std::cout<<"Delete successful\n";
  chatroomcounter--;
}
else
{
  std::cout<<"No such chatroom exist\n";
  delete_rooms(user_created_room);
}
return user_created_room;
}
