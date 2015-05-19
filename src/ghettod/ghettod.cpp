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
#include <string>
#include <iostream>
#include <fstream>
#include <cstring>
#include <cstdint>

//autotools includes
#include "config.h"

//MHD includes
#include <sys/types.h>
#include <sys/select.h>
#include <sys/socket.h>
#include <microhttpd.h>

//json includes
#include "jsoncons/json.hpp"

//Defines
#define GHETTO_PORT 6770 //G(HE)TTO

static std::string pageText;

static int answer_to_connection (void *cls, struct MHD_Connection *connection, const char *url, const char *method, const char *version, const char *upload_data, size_t *upload_data_size, void **con_cls){
  
  //Make all the response bits
  const char *page = pageText.c_str();
  struct MHD_Response *response;
  int ret;
  
  //Make and send the response(s).
  response = MHD_create_response_from_buffer (strlen (page), (void *) page, MHD_RESPMEM_PERSISTENT);
  ret = MHD_queue_response (connection, MHD_HTTP_OK, response);
  MHD_destroy_response (response);

  return ret;
}

int main(int argc, char* argv[]){
  
  /**********************
  * File initialization *
  **********************/
  
  static const char * pagefilename = "pagefile";
  std::ifstream pagefile;
  
  pagefile.open(pagefilename);
  if(!pagefile) {
    return -1;
  }
  
  //Read pagefile into string.
  std::string s((std::istreambuf_iterator<char>(pagefile)), std::istreambuf_iterator<char>());
  pageText = s;
  
  /*********************
  * MHD initialization *
  *********************/
  struct MHD_Daemon *daemon;

  daemon = MHD_start_daemon (MHD_USE_SELECT_INTERNALLY, GHETTO_PORT, NULL, NULL, &answer_to_connection, NULL, MHD_OPTION_END);
  if (NULL == daemon){
    
    //Failed to start webserver.
    return 1;
  }
  
  /****************************************************
  *****************************************************
  ******************* BEGIN PROGRAM *******************
  *****************************************************
  ****************************************************/
  
  //Don't exit immidiately.
  getchar ();
 
  /**********
  * Cleanup *
  **********/
  MHD_stop_daemon (daemon);
  return 0;
}
