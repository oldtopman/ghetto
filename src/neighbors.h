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
#include <string>
#include "jsoncons/json.hpp"
#include "json.h"
#include "computer.h"

#ifndef NEIGHBORS_H_DEFINED
#define NEIGHBORS_H_DEFINED

struct contact{
  std::string host;
  std::string name;
};

void callNeighbors(ComputerIndex &p_ci, std::string tmp_path);

#endif