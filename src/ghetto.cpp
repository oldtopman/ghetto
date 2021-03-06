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
#include <new> //new
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

//Includes for this project.
#include "json.h"
#include "computer.h"
#include "mainmenu.h"

//Defines
#define GHETTO_PORT 6770 //G(HE)TTO
 
int main(int argc, char* argv[]){
  //using jsoncons::json;
  
  /***********************
  * Variable declaration *
  ***********************/
  
  //TODO: Move some up here.
  //TODO: Consistently name variables (json, file, etc.)
  bool latestGhetto;
  
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
  jsoncons::json netInfoJson;
  
  //Errors on loading are going to indicate a problem with ghettod not running.
  //We're assuming that all errors are file not found.
  //TODO: Handle other load file errors.
  while(openJsonFile(netInfoPath.c_str(), netInfoJson) ){
    //Retry every 5 seconds.
    for(std::time_t timeCounter = std::time(nullptr); (std::time(nullptr) - timeCounter) < 6;){
      
      //Build a little countdown message.
      std::string checkMessage("netinfo.json not found (maybe). Has ghettod run?\nRetrying in ");
      checkMessage += std::to_string( 5 - ((std::time(nullptr) - timeCounter)) );
      checkMessage += "s";
      statusDbox.clean();
      statusDbox.make(checkMessage.c_str());
    }
  }
  
  //Check to see if we can parse the file.
  if(GHETTOJSON_VERSION < netInfoJson["ver_oldghetto"].as<int>()){
    
    //We're too old to parse the file.
    errorDbox.make("This version of ghetto(d) is too old to run.\nPlease update.");
    endwin();
    return 2;
  }
  
  //We can also check to see if there are any updates.
  //TODO: Check taint rather than version.
  //version would only differ between ghettod/ghetto versions,
  //while taint spans the whole network.
  if(GHETTOJSON_VERSION < netInfoJson["ver_ghetto"].as<int>()){
    
    //Updates available...somewhere.
    errorDbox.make("Updates are available for ghetto!");
    latestGhetto = false;
  }else{
    
    //We are up to date, at least in our network.
    latestGhetto = true;
  }
  
  //Check to see how stale the results are.
  if((std::time(nullptr) - netInfoJson["time_generated"].as<int>()) > 400){
    
    //Daemon hasn't updated the file in a long time.
    errorDbox.make("Network info file is stale - is ghettod running?");
  }
  
  //If ghettod is updating, see how long since it last networked.
  else if((std::time(nullptr) - netInfoJson["time_lastrecieved"].as<int>()) > 700){
    
    //We're not connected to the network
    errorDbox.make("ghettod can't find the 'hood.\nResults out of date.");
  }
  
  //Since we're able to parse the file, parse it!
  
  //Extract complist from the netinfo.
  statusDbox.clean();
  statusDbox.make("Extracting complist.");
  
  ComputerIndex * computers = new ComputerIndex;
  computers->parse(netInfoJson);
  
  /*******************************************************
  ********************************************************
  ******************** BEGIN INTERFACE *******************
  ********************************************************
  *******************************************************/
  clear();
  refresh();
  
  
  /**********************************************************
  * Quick outline of the plan:                              *
  * Write our computer info on the top of the window (dbox) *
  * Write menu with other computers below our info.         *
  * When computer is selected, display info (somewhere)     *
  * ???                                                     *
  * Get famous for writing ghetto                           *
  **********************************************************/
  //TODO: Move to other file. Getting sick of how large this function is becoming, even for quick work.
  
  //Generate dialogbox with our computer info.
  DialogBox computerInfo;
  computerInfo.options(0,0,0,false);
  
  int ourComputerIndex = -1;
  //First, we have to find our computer - it has a jump of 0.
  for(int i = 0; i < computers->count(); i++){
    
    //If this is our computer
    if(computers->jcount(i) == 0){
      ourComputerIndex = i;
      break;
    }
  }
  
  if(ourComputerIndex < 0){ //Couldn't find our computer in the index
    
    //Is it because ghetto is out of date?
    if(latestGhetto == false){
      computerInfo.make("Can't find information on this system.\nConsider updating ghetto.");
    }
    
    //Nope. Complain anyway.
    else{
      computerInfo.make("Can't find information on this system.\nThis should *not* occur in normal usage.");
    }
  }else{
    
    //Found our computer - build header.
    std::string infoString("This is ");
    infoString += computers->name(ourComputerIndex);
    infoString += " at ";
    infoString += computers->host(ourComputerIndex);
    infoString += " - uptime: ";
    infoString += std::to_string(computers->uptime(ourComputerIndex));
    infoString += "\n";
    
    infoString += computers->msg(ourComputerIndex);
    
    computerInfo.make(infoString.c_str());
  }
  
  //Generate other computer menus.
  Menu computerMenu;
  DialogBox computerDetails;
  computerDetails.options(15,3,0,false);
  
  //Generate menu string.
  //Fortunately, jsoncons sorts stuff alphabetically, so it will appear sorted without any work. (ish)
  while(true){ //TODO: Make this the main loop
    
    //TODO: Clear display
    
    //TODO: Display system info dbox
    
    //TODO: Get menu to stay still when scrolling.
    //BUGFIX: ^^
    //Draw computer select menu.
    std::string computerMenuString(gen_comp_menu(computers));
    
    int selectedComputerIndex = computerMenu.quickMake(computerMenuString.c_str(), 0, 3, 10);
    selectedComputerIndex--; //Menu is count-from-one.
    
    //Display appropiate computer's details.
    computerDetails.clean();
    std::string computerDetailString = gen_comp_details(computers, selectedComputerIndex);
    computerDetails.make(computerDetailString.c_str());
  }
  
  
  /**********
  * Cleanup *
  **********/
  
  //TODO: Move most of these to atexits?
  delete computers;
  endwin();
 
  return 0;
}
