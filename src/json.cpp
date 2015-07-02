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
#include "json.h"

int openJsonFile(const char * p_filePath, jsoncons::json& p_json){
  
  //First, open the file.
  std::ifstream fileStream(p_filePath);
  if(!fileStream){
    //error somehow
    //TODO: Error handling.
    return -1;
  }
  
  //Parse the file now.
  try{
    p_json = jsoncons::json::parse(fileStream);
  }
  catch(const jsoncons::json_parse_exception& e){
    //error somehow
    /*
    std::string parseError;
    parseError = "Caught json_parse_exception with category ";
    parseError += e.code().category().name();
    parseError += ", code ";
    parseError += e.code().value();
    parseError += "\nmessage: ";
    parseError += e.what();
    errorDbox.make(parseError.c_str());
    endwin();
    return 1;
    */
    //TODO: Error handling.
    return -1;
  }
  
  //Whee, no errors!
  return 0;
}

int countArrayElements(jsoncons::json & p_json){
  return (p_json.end_elements() - p_json.begin_elements());
}

int gen_ni_outline(ni_outline p_nio, jsoncons::json & p_json){
  p_json["ver_ghetto"] = p_nio.ver_ghetto;
  p_json["ver_oldghetto"] = p_nio.ver_oldghetto;
  p_json["ver_ghettotaint"] = p_nio.ver_ghettotaint;
  p_json["time_lastrecieved"] = p_nio.time_lastrecieved;
  p_json["time_generated"] = p_nio.time_generated;
  return 0;
}