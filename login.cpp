#include <iostream>
#include <cstring>
#include "login.hpp"
#include <fstream>
#include <vector>
#include<iterator>




int Mylogin::create_account()
{
  int x=0;
  int y=0;
  std::string line;
  while(x==0)
  {
  std::cout<<"Enter a username to create an account: ";
  std::cin>> new_username;
  std::cout<<"Enter a password to create an account: ";
  std::cin>> new_password;
  std::ifstream myfile("login.txt",std::ios_base::app);
  if (myfile.is_open())
   {
     while ( getline (myfile,line) )
     {
      myfile>>username>>password;
      if(username==new_username)
      {
        std::cout<<"username exists:\n"<<"Enter a differnet username: ";
        std::cin>> new_username;
      }
      else
      {
        x=1;
      }

     }
     myfile.close();
   }
   }
   if (x==1)
   {
     std::cout<<"Account successfully created\n";
     int auth_login= login(true);
     if(auth_login==0){return 0;}
     if (auth_login==1){return 1;}
   }
}





int Mylogin::login(bool new_login)
{
  std::string line;
std::cout<<"Login Window\n";
if (new_login==true)
  {
std::ofstream myfile ("login.txt",std::ios_base::app);
  if (myfile.is_open())
    {
      myfile << new_username<<" "<<new_password<<"\n";
    }

myfile.close();
}

std::cout<<"Enter username: ";
std::cin>>new_username;
std::cout<<"Enter password: ";
std::cin>>new_password;
std::ifstream imyfile("login.txt",std::ios_base::app);
if (imyfile.is_open())
 {
   while ( getline (imyfile,line) )
   {
    imyfile>>username>>password;
    if(username==new_username)
    {
      if (password==new_password)
      {
        std::cout<<"login successful\n";
        imyfile.close();
        return 1;
      }
    }
    else
      {
        std::cout<<"incorrect user name or password\n";
        imyfile.close();
        return 0;//password incorrect--unsuccessful
      }



    }

}
}
