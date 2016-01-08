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

void ComputerIndex::remove(int p_index){
  //If there's a lot of slowdown here,
  //consider a different structure (not vector)
  if(p_index > houses){ return; }
  computerVector.erase(computerVector.begin()+p_index);
}
  
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


uint64_t ComputerIndex::time(int p_index)
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
  //return tmp.jumpCount;
}


bool ComputerIndex::online(int p_index)
{
  if(p_index > houses)
    return false;
  
  return computerVector[p_index].online;
}

void ComputerIndex::append(computer p_comp)
{
  computerVector.push_back(p_comp);
}

int ComputerIndex::count(){
  return houses;
}

jsoncons::json ComputerIndex::json(){
  return complist;
}

computer& ComputerIndex::operator[](const int p_index){
  
  //Make sure the read isn't out-of-bounds.
  if(p_index < 0 || p_index > houses){
    computer errComp;
    errComp.name = "ERROR!";
    errComp.host = "666.666.666.666";
    errComp.message = "ComputerIndex subscript overrun";
    errComp.online = false;
    return errComp;
  }
  else{
    return computerVector[p_index];
  }
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
  
  //Count elements
  houses = complist.size();
  
  jsoncons::json activeComputerJson;
  jsoncons::json::array_iterator complistIterator = complist.begin_elements();
  computer tmpcomp;
  
  //TODO: Put all of these json constants in json.h
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
  
  ComputerIndex::gen_from_vector();
  return 0;
}


void ComputerIndex::gen_from_vector(){
  complist.clear();
  complist = jsoncons::json::an_array;
  jsoncons::json computerJson;
  
  //We could address the vector directly, but this makes
  //vector access easier to secure.
  for(int i = 0; i < computerVector.size(); i++){
    computerJson["name"] = ComputerIndex::name(i);
    computerJson["host"] = ComputerIndex::host(i);
    computerJson["message"] = ComputerIndex::msg(i);
    computerJson["update_time"] = ComputerIndex::time(i);
    computerJson["uptime"] = ComputerIndex::uptime(i);
    computerJson["jump_count"] = ComputerIndex::jcount(i);
    computerJson["online"] = ComputerIndex::online(i);
    complist.add(computerJson);
  }
  
}

void ComputerIndex::import_index(ComputerIndex &p_ci){
  int found;
  
  //Supposed to avoid duplicates.
  //Don't import duplicates, and keep the most recent one.
  for(int i = 0; i < p_ci.count(); i++){
    found = 0;
    for(int j = 0; (j <= computerVector.size()) && (found == 0); j++){
      if(p_ci.name(i) == ComputerIndex::name(j)){
        found = j;
      }
    }
    if(found != 0){
      //Pick the most recent one and keep it.
      if(p_ci.time(i) < ComputerIndex::time(found)){
        ComputerIndex::remove(found);
        computerVector.push_back(p_ci.computerVector[i]);
      }
      //If they're at the same time, pick the one with the least jumps.
      else if(p_ci.time(i) == ComputerIndex::time(found)){
        if(p_ci.jcount(i) < ComputerIndex::jcount(found)){
          ComputerIndex::remove(found);
          computerVector.push_back(p_ci.computerVector[i]);
        }
      }
      //Otherwise do nothing, since we want to keep the version in the vector already.
    }
    else{
      computerVector.push_back(p_ci.computerVector[i]);
      houses++;
    }
  }
  
  //Update the number of houses in the 'hood
  houses = computerVector.size();
  
}
