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

void callNeighbors(ComputerIndex &p_ci){
  CURL *curlHandle;
  curlHandle = curl_easy_init();
  curl_easy_setopt(curlHandle, CURLOPT_URL, "http://localhost:6770");
  curl_easy_setopt(curlHandle, CURLOPT_WRITEFUNCTION, write_data);
  //curl_easy_setopt(curlHandle, CURLOPT_ERRORBUFFER, curlError);
  //char curlError[CURL_ERROR_SIZE]; //Handle curl errors.
  std::vector<contact> contacts;
  curl_easy_cleanup(curlHandle);
  callNeighbors(p_ci, contacts);
  curl_easy_cleanup(curlHandle);
  return;
}

static void callNeighbors(ComputerIndex &p_ci, std::vector<contact> &p_contacts){
  
  /*
  FILE * outfile;
  outfile = fopen("FILENAME HERE", "wb");
  if(!bodyfile){
    //error here for something
  }
  curl_easy_setopt(curlHandle, CURLOPT_WRITEDATA, outfile);
  */
}