/*
ghetto - A tool for keeping track of your linux-based systems and their uptimes.
Copyright (C) 2015  oldtopman <oldtopman@gmail.com>

This program is free software: you can redistribute it and/or modify
it under the terms of the GNU General Public License as published by
the Free Software Foundation, either version 3 of the License, or
(at your option) any later version.

This program is distributed in the hope that it will be useful,
but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
GNU General Public License for more details.

You should have received a copy of the GNU General Public License
along with this program.  If not, see <http://www.gnu.org/licenses/>.
*/
#include <vector>
#include "jsoncons/json.hpp"

#ifndef COMPUTER_H_DEFINED
#define COMPUTER_H_DEFINED

//TODO: Rename several of these for briefness
struct computer{
  std::string name;
  std::string host;
  std::string message;
  std::time_t updateTime;
  unsigned long long uptime;
  int jumpCount;
  bool online;
};

class ComputerIndex{
  
  private:
  std::vector<computer> computerVector;
  jsoncons::json complist;
  int houses;
  
  public:
  
  //accessors for the struct.
  std::string name(int p_index);
  std::string host(int p_index);
  std::string msg(int p_index);
  std::time_t time(int p_index);
  unsigned long long uptime(int p_index);
  int jcount(int p_index);
  bool online(int p_index);
  
  //generic accessors
  int count();
  
  //actual functions
  int parse(jsoncons::json p_computerList);
  
  ComputerIndex():
  houses(0)
  {
    //constructor
  }
};

#endif