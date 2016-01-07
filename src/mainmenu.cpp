/*
 * ghetto - A tool for keeping track of your linux-based systems and their uptimes.
 * Copyright (C) 2015  oldtopman <oldtopman@gmail.com>
 * 
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 * 
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 * 
 * You should have received a copy of the GNU General Public License
 * along with this program.  If not, see <http://www.gnu.org/licenses/>.
 */

#include "mainmenu.h"
#include <string>

std::string gen_comp_menu(ComputerIndex * p_computers)
{
  std::string wrkstring("");
  
  for(int i = 0; i < p_computers->count(); i++){
    
    //Indicate online status
    if(p_computers->online(i)){ wrkstring += "O : "; }
    else{ wrkstring += "X : "; }
    
    //Add computer name.
    wrkstring += p_computers->name(i);
    
    //Newline to prepare for the next computer.
    wrkstring += ",";
  }
  
  return wrkstring;
}

std::string gen_comp_details(ComputerIndex * p_computers, int p_index)
{
  std::string wrkstring("");
  
  wrkstring += p_computers->name(p_index);
  wrkstring += "@";
  wrkstring += p_computers->host(p_index);
  wrkstring += "\n";
  wrkstring += "msg: ";
  wrkstring += p_computers->msg(p_index);
  wrkstring += "\n";
  wrkstring += "upd-upt-jmp\n";
  wrkstring += std::to_string(p_computers->time(p_index));
  wrkstring += "-";
  wrkstring += std::to_string(p_computers->uptime(p_index));
  wrkstring += "-";
  wrkstring += std::to_string(p_computers->jcount(p_index));
  
  return wrkstring;
}