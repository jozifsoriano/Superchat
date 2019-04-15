//menu.hpp
class Mymenu{
public:
void print_menu();
std::string enter_rooms(std::string user_created_room);
std::string create_rooms(std::string user_created_room);
std::string delete_rooms(std::string user_created_room);
int chatroomcounter;
std::string ChatroomArray[20];
};
