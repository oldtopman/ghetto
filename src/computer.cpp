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

int computerCount(jsoncons::json & p_netInfoJson){
  
  //Find the computer list and parse it.
  
  //TODO: Cache the complist so we don't have to parse this more than once.
  jsoncons::json complist;
  try{ complist = p_netInfoJson.get("complist"); }
  catch(const jsoncons::json_parse_exception& e){
    //Handle bad parsing
    //TODO: Some kind of generic json error handling?
    return -1;
  }
  
  return (complist.end_elements() - complist.begin_elements());
}

int populateComputer(computer p_computerList[], jsoncons::json p_computerList){
  
  //Parse a computer at the given offset.
  