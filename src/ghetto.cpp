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

//Defines
#define GHETTO_PORT 6770 //G(HE)TTO
 
static size_t write_data(void *ptr, size_t size, size_t nmemb, void *stream)
{
  int written = fwrite(ptr, size, nmemb, (FILE *)stream);
  return written;
}
 
int main(int argc, char* argv[]){
  
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
  
  /****************************************************
  *****************************************************
  ******************* BEGIN PROGRAM *******************
  *****************************************************
  ****************************************************/
 
  CURL *curl_handle;
  
  curl_handle = curl_easy_init();
 
  //Configure handle
  curl_easy_setopt(curl_handle, CURLOPT_URL, "http://localhost:6770");
  curl_easy_setopt(curl_handle, CURLOPT_WRITEFUNCTION, write_data);
  curl_easy_setopt(curl_handle, CURLOPT_WRITEDATA, bodyfile);
 
  //Execute handle
  curl_easy_perform(curl_handle);
 
  /**********
  * Cleanup *
  **********/
  
  //TODO: Move most of these to atexits?
  fclose(bodyfile);
  curl_easy_cleanup(curl_handle);
 
  return 0;
}
