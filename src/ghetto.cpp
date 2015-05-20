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

//General includes
#include <unistd.h> //gethostname
#include <pwd.h> //user's home directory
#include <errno.h> //error handling
#include <stdio.h>
#include <fstream>
#include <stdlib.h>
#include <unistd.h>
#include <iostream>
#include <string>
#include <cstring>
#include <ctime>
#include <cstdint>
#include <stdexcept>

//For mkdir
#include <sys/types.h>
#include <sys/stat.h>

//autotools includes
#include "config.h"

//ncurses includes
#include <ncurses.h>

//ncurses-menu includes
#include "submodules/ncurses-menu/dialogBox.h"
#include "submodules/ncurses-menu/menu.h"

//json includes
#include "jsoncons/json.hpp"

//Defines
#define GHETTO_PORT 6770 //G(HE)TTO
 
int main(int argc, char* argv[]){
  //using jsoncons::json;
  
  /***********************
  * Variable declaration *
  ***********************/
  
  //None yet
  
  /**************************
   * Variable initialization *
   **************************/
  
  //Get hostname
  char hostname[65];
  if(gethostname(hostname, 65)){
    std::cout << "Could not get hostname. Exiting." << std::endl;
    return 1;
  }
  
  //Get homePath
  std::string homePath("");
  if ((homePath = getenv("HOME")) == "") { //Use $HOME before looking it up ourselves.
    homePath = getpwuid(getuid())->pw_dir;
  }
  if(homePath == ""){
    std::cout << "Could not get homePath. Exiting." << std::endl;
    return 1;
  }
  
  //Set other paths
  std::string ghettoPath(homePath + "/.ghetto");
  std::string settingsPath(ghettoPath + "/config");
  std::string netInfoPath(ghettoPath + "/netinfo.json");
  
  /*********************
  * Initialize ncurses *
  *********************/ 
  
  initscr();
  raw();
  noecho();
  cbreak();
  keypad(stdscr, true);
  curs_set(0);
  
  //Configurate the error dbox.
  DialogBox errorDbox;
  errorDbox.options(0,0,0,true);
  
  //Turn color on if supported.
  if(has_colors() == true){ start_color(); }
  else{
    errorDbox.make("No color support here._You should really upgrade your terminal.\n:(");
  }
  
  /****************************************************
  *****************************************************
  ******************* BEGIN PROGRAM *******************
  *****************************************************
  ****************************************************/
  
  clear();
  refresh();
  
  DialogBox statusDbox;
  statusDbox.options(0,0,0,false);
  statusDbox.make("Loading settings...");
  
  //Load settings.
  std::ifstream ifSettingsFile(settingsPath.c_str());
  
  //If it failed, try to create directory.
  if(!ifSettingsFile){
    if(mkdir(ghettoPath.c_str(), S_IRWXU | S_IRWXG | S_IROTH | S_IXOTH)){
      //If mkdir failed, find out why and tell.
      statusDbox.clean();
      if(errno == EROFS){
        errorDbox.make("Setting initialization failed:\nRead only filesystem.");
        return 1;
      }
      if(errno == EEXIST){
        //This means that ghettod has initialized the folder structure
        //But that ghetto hasn't run before.
      }
      else{
        std::string errorString("Unknown error encountered calling mkdir()\nSee man mkdir() for details\n errno=");
        errorString += std::to_string( errno );
        errorDbox.make(errorString.c_str());
        return 1;
      }
    }
    else{
      //This means that neither ghettod or ghetto has run before.
      //We're going to be in trouble, since ghetto needs ghettod to be running already/before.
    }
  }
  
  //Here we load the config file.
  //TODO: Read config file
  
  //Done loading settings, so:
  if(ifSettingsFile){ ifSettingsFile.close(); }
  
  statusDbox.clean();
  statusDbox.make("Loading network info...");
  
  //Load the json file that has all the info about the network.
  std::ifstream ifNetInfoFile(netInfoPath.c_str());
  
  //Errors on loading are going to indicate a problem with ghettod not running.
  //We're assuming that all errors are file not found.
  //TODO: Handle other load file errors.
  if(!ifNetInfoFile){
    //Loop until the file loads
    while(!ifNetInfoFile){
      //Retry every 5 seconds.
      for(std::time_t timeCounter = std::time(nullptr); (std::time(nullptr) - timeCounter) < 6;){
        
        //Build a little countdown message.
        std::string checkMessage("netinfo.json not found. Has ghettod run?\nRetrying in ");
        checkMessage += std::to_string( 5 - ((std::time(nullptr) - timeCounter)) );
        checkMessage += "s";
        statusDbox.clean();
        statusDbox.make(checkMessage.c_str());
      }
      
      //Try loading the file again.
      ifNetInfoFile.open(netInfoPath.c_str());
    }
  }
  //Parse the json file.
  try{ jsoncons::json netInfoJson = jsoncons::json::parse(ifNetInfoFile); }
  catch(const jsoncons::json_parse_exception& e){
    //Handle bad parsing
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
  }
  
  
 
  /**********
  * Cleanup *
  **********/
  
  //TODO: Move most of these to atexits?
  endwin();
 
  return 0;
}
