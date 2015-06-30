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

#include "jsoncons/json.hpp"
#include "computer.h"
std::string ComputerIndex::name(int p_index)
{
  if(p_index > houses)
    return "out of bounds";
  
  return computerVector[p_index].name;
}


std::string ComputerIndex::host(int p_index)
{
  if(p_index > houses)
    return "out of bounds";
  
  return computerVector[p_index].host;
}


std::string ComputerIndex::msg(int p_index)
{
  if(p_index > houses)
    return "out of bounds";
  
  return computerVector[p_index].message;
}


time_t ComputerIndex::time(int p_index)
{
  if(p_index > houses)
    return -1;
  
  return computerVector[p_index].updateTime;
}


unsigned long long ComputerIndex::uptime(int p_index)
{
  if(p_index > houses)
    return 0;
  
  return computerVector[p_index].uptime;
}


int ComputerIndex::jcount(int p_index)
{
  if(p_index > houses)
    return -1;
  
  return computerVector[p_index].jumpCount;
}


bool ComputerIndex::online(int p_index)
{
  if(p_index > houses)
    return false;
  
  return computerVector[p_index].online;
}


int ComputerIndex::count(){
  return houses;
}

int ComputerIndex::parse(jsoncons::json p_computerList)
{
  
  //TODO: Put all of these json constants in json.h
  try{ complist = p_computerList.get("complist"); }
  catch(const jsoncons::json_parse_exception& e){
    //Handle bad parsing
    //TODO: Some kind of generic json error handling?
    return -1;
  }
  
  jsoncons::json activeComputerJson;
  jsoncons::json::array_iterator complistIterator = complist.begin_elements();
  computer tmpcomp;
  
  for(int i = 0; i < houses; i++){
    activeComputerJson = complistIterator->as<jsoncons::json>();
    tmpcomp.name = activeComputerJson["name"].as_string();
    tmpcomp.host = activeComputerJson["host"].as_string();
    tmpcomp.message = activeComputerJson["message"].as_string();
    tmpcomp.updateTime = activeComputerJson["update_time"].as<unsigned long long>();
    tmpcomp.uptime = activeComputerJson["uptime"].as<unsigned long long>();
    tmpcomp.jumpCount = activeComputerJson["jump_count"].as<int>();
    tmpcomp.online = activeComputerJson["online"].as<bool>();
    computerVector.push_back(tmpcomp);
    complistIterator++;
  }
  
  houses = (complist.end_elements() - complist.begin_elements());
  return 0;
}