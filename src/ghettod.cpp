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
#include <unistd.h> //sleep()
#include <pwd.h> //user id stuff
#include <time.h>
#include <sys/utsname.h> //uname
#include <sys/sysinfo.h> //uptime, memory, cpu, etc.

//autotools includes
#include "config.h"

//MHD includes
#include <sys/types.h>
#include <sys/select.h>
#include <sys/socket.h>
#include <microhttpd.h>

//libcurl includes
#include <curl/curl.h>

//json includes
#include "jsoncons/json.hpp"

//ghettod includes
#include "json.h"
#include "computer.h"

//Defines
#define GHETTO_PORT 6770 //G(HE)TTO

//TODO: Learn what this does
//TODO: Make write_data write_data_t
static size_t write_data(void *ptr, size_t size, size_t nmemb, void *stream)
{
  int written = fwrite(ptr, size, nmemb, (FILE *)stream);
  return written;
}

//TODO: Un-globalize this
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

//TODO: reformat this as a daemon
int main(int argc, char* argv[]){
  
  /**************************
   * Variable initialization *
   **************************/
  
  //TODO: Move these to functions in a different file
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
  std::string netInfoPath(ghettoPath + "/netinfo.json");
  std::string workingNetInfoPath(ghettoPath + "/workingnetinfo.json");
  
  /**********************
  * File initialization *
  **********************/
  FILE * bodyfile;
  bodyfile = fopen("body.out", "wb");
  if(!bodyfile){
    return -1;
  }
  
  std::ofstream tmp_file;
  tmp_file.open(workingNetInfoPath, std::ios::out);
  if(!tmp_file){
    return -1;
  }
  
  std::ifstream pagefile;
  pagefile.open(netInfoPath);
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
  //Initialize our handle
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
    std::cout << curlError << std::endl;
  }
  
  //Check input for quit hooks.
  while(true){
    
    if(true){
      
      //Declare variables?
      //TODO: Move variable declaration up here
      
      //TODO: All of this needs to be on the stack
      jsoncons::json netinfo;
      jsoncons::json stale_netinfo;
      jsoncons::json tmp_netinfo;
      ComputerIndex computer_index;
      ComputerIndex stale_computer_index;
      
      //Build netinfo outline.
      ni_outline outline;
      outline.ver_ghetto = GHETTOJSON_VERSION;
      outline.ver_oldghetto = GHETTOJSON_OLDVERSION;
      outline.ver_ghettotaint = GHETTOJSON_VERSION;
      outline.time_lastrecieved = 0;
      outline.time_generated = time(NULL); //TODO: Make this safer
      gen_ni_outline(outline, netinfo);
      
      //Build computer list.
      computer wrkcomp;
      
      //Generate info about this computer.
      struct utsname my_uname;
      uname(&my_uname);
      struct sysinfo my_sysinfo;
      sysinfo(&my_sysinfo);
      
      //Make our computer info
      wrkcomp.name = my_uname.nodename;
      wrkcomp.host = "127.0.0.1"; //TODO: Determine this from local settings.
      wrkcomp.message = "blankmessage"; //TODO: Read this from local settings.
      wrkcomp.updateTime = time(NULL); //TODO: Make this safer
      wrkcomp.uptime = my_sysinfo.uptime;
      wrkcomp.jumpCount = 0;
      wrkcomp.online = true;
      computer_index.append(wrkcomp);
      
      //Import local netinfo and add to list
      if(openJsonFile(netInfoPath.c_str(), stale_netinfo)){
        //TODO: Handle different openJsonFile errors.
      }
      stale_computer_index.parse(stale_netinfo);
      computer_index.import_index(stale_computer_index);
      //Construct list of people to contact
      
      //---Attempt to contact other computers in the network, 1 penalty.
      
      //---Copy over the leftover stale computers into the array.
      
      //Write netinfo
      computer_index.gen_from_vector();
      netinfo["complist"] = computer_index.json();
      tmp_file << jsoncons::pretty_print(netinfo);
      tmp_file.flush();
      
      //Does MHD need to know about the update?
      
      //Does ghetto need to know about the update?
      
      
    }
    
    //TODO: Read sleep interval from file
    sleep(300);
  }
  
  //Don't exit immidiately.
  while(true);
 
  /**********
  * Cleanup *
  **********/
  MHD_stop_daemon (daemon);
  curl_easy_cleanup(curlHandle);
  return 0;
}
