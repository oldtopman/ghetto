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

#ifndef JSON_H_INCLUDED
#define JSON_H_INCLUDED
  
int openJsonFile(const char * p_filePath, jsoncons::json& p_json);
int countArrayElements(jsoncons::json & p_json);

//TODO: Rename several of these for briefness
struct computer{
  std::string name;
  std::string host;
  std::string message;
  std::uint64_t updateTime;
  unsigned long long uptime;
  int jumpCount;
  bool online;
};

struct ni_outline{
  int ver_ghetto;
  int ver_oldghetto;
  int ver_ghettotaint;
  uint64_t time_lastrecieved;
  uint64_t time_generated;
};
int gen_ni_outline(ni_outline p_nio, jsoncons::json & p_json);

#endif