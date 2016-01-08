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

//libcurl includes
#include <curl/curl.h>

#include <string>
#include <vector>
#include "jsoncons/json.hpp"
#include "neighbors.h"
#include "computer.h"

//TODO: Learn how this works
static size_t write_data(void *ptr, size_t size, size_t nmemb, void *stream)
{
  int written = fwrite(ptr, size, nmemb, (FILE *)stream);
  return written;
}

//TODO: Implement this so that it's called recursively.
void callNeighbors(ComputerIndex &p_ci){
  
  //Get CURL going.
  CURL *curlHandle;
  curlHandle = curl_easy_init();
  curl_easy_setopt(curlHandle, CURLOPT_URL, "http://localhost:6770");
  curl_easy_setopt(curlHandle, CURLOPT_WRITEFUNCTION, write_data);
  char curlError[CURL_ERROR_SIZE]; //Handle curl errors.
  curl_easy_setopt(curlHandle, CURLOPT_ERRORBUFFER, curlError);
  
  //Create things the recursive functions need.
  std::vector<contact> contacts;
  int index = 0;
  
  //Start making the calls!
  for(int i = 0; i < p_ci.count(); i++){
    
    //Skip our host.
    if(p_ci.jcount(i) == 0){
      //skip?
    }else{
      std::string address = p_ci.host(i);
      address += ":6770";
      curl_easy_setopt(curlHandle, CURLOPT_URL, address.c_str());
    }
  }
  
  //Cleanup
  curl_easy_cleanup(curlHandle);
  return;
}