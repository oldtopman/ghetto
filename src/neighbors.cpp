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

#include <string>
#include <vector>
#include "jsoncons/json.hpp"
#include "neighbors.h"
#include "computer.h"

void callNeighbors(ComputerIndex &p_ci){
  std::vector<contact> contacts;
  return callNeighbors(p_ci, contacts);
}

void callNeighbors(ComputerIndex &p_ci, std::vector<contact> &p_contacts){
  
}