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
#include <stdio.h>
#include <fstream>
#include <stdlib.h>
#include <unistd.h>
#include <iostream>
#include <string>
#include <cstring>
#include "config.h"

//libcurl includes
#include <curl/curl.h>

//ncurses includes
#include <ncurses.h>

//ncurses-menu includes
#include "submodules/ncurses-menu/dialogBox.h"
#include "submodules/ncurses-menu/menu.h"

//Defines
#define GHETTO_PORT 6770 //G(HE)TTO
 
static size_t write_data(void *ptr, size_t size, size_t nmemb, void *stream)
{
  int written = fwrite(ptr, size, nmemb, (FILE *)stream);
  return written;
}
 
int main(int argc, char* argv[]){
  
  /**************************
  * Variable initialization *
  **************************/
  
  //Get hostname
  char hostname[65];
  if(gethostname(hostname, 65)){
    std::cout << "Could not get hostname. Exiting." << std::endl;
    return 1;
  }
  
  //Get homedir
  std::string homedir("");
  if ((homedir = getenv("HOME")) == "") { //Use $HOME before looking it up ourselves.
    homedir = getpwuid(getuid())->pw_dir;
  }
  if(homedir == ""){
    std::cout << "Could not get homedir. Exiting." << std::endl;
    return 1;
  }
  
  
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
    errorDbox.make("No color support here._You should really upgrade your terminal._:(");
  }
  
  /**********************
  * File initialization *
  **********************/
  
  static const char * bodyfilename = "body.out";
  FILE * bodyfile;
  
  bodyfile = fopen(bodyfilename, "wb");
  if(!bodyfile) {
    return -1;
  }
  
  
  /*************************
  * Libcurl initialization *
  *************************/
 
  curl_global_init(CURL_GLOBAL_NOTHING); //Assume libcurl was compiled with minimal features
  
  char curlError[CURL_ERROR_SIZE]; //Handle curl errors.
  
  /****************************************************
  *****************************************************
  ******************* BEGIN PROGRAM *******************
  *****************************************************
  ****************************************************/
  
  /****************
  * Splash screen *
  ****************/
  mvprintw(0,0,"ghetto - A tool for keeping track of your linux-based systems and their uptimes");
  mvprintw(1,0,"Copyright 2015 oldtopman <oldtopman@gmail.com>");
  
  mvprintw(3,0,"This program comes with ABSOLUTELY NO WARRANTY.");
  mvprintw(4,0,"This is free software, and you are welcome to redistribute it");
  mvprintw(5,0,"under certain conditions; see COPYING for details.");
  mvprintw(6,0,"The complete text of the GNU GPLv3, which covers this software,");
  mvprintw(7,0,"should be included in the file \"LICENSE.TXT\".");
  mvprintw(8,0,"If not, see <http://www.gnu.org/licenses/>.");
  
  mvprintw(10,0,"Press any key to start the program.");
  
  getch();
  clear();
  refresh();
 
  //Get the 
  CURL *curlHandle;
  
  curlHandle = curl_easy_init();
 
  //Configure handle
  curl_easy_setopt(curlHandle, CURLOPT_URL, "http://localhost:6770");
  curl_easy_setopt(curlHandle, CURLOPT_WRITEFUNCTION, write_data);
  curl_easy_setopt(curlHandle, CURLOPT_WRITEDATA, bodyfile);
  curl_easy_setopt(curlHandle, CURLOPT_ERRORBUFFER, curlError);
 
  //Execute handle
  if(curl_easy_perform(curlHandle) != CURLE_OK){
    
    //Handle errors.
    errorDbox.make(curlError);
  }
 
  /**********
  * Cleanup *
  **********/
  
  //TODO: Move most of these to atexits?
  endwin();
  fclose(bodyfile);
  curl_easy_cleanup(curlHandle);
 
  return 0;
}
