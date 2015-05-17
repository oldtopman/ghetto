//General includes
#include <stdio.h>
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
 
int main(void)
{
  CURL *curl_handle;
  static const char *headerfilename = "head.out";
  FILE *headerfile;
  static const char *bodyfilename = "body.out";
  FILE *bodyfile;
 
  curl_global_init(CURL_GLOBAL_ALL);
 
  /* init the curl session */ 
  curl_handle = curl_easy_init();
 
  /* set URL to get */ 
  curl_easy_setopt(curl_handle, CURLOPT_URL, "http://localhost:6770");
 
  /* no progress meter please */ 
  curl_easy_setopt(curl_handle, CURLOPT_NOPROGRESS, 1L);
 
  /* send all data to this function  */ 
  curl_easy_setopt(curl_handle, CURLOPT_WRITEFUNCTION, write_data);
 
  /* open the header file */ 
  headerfile = fopen(headerfilename, "wb");
  if(!headerfile) {
    curl_easy_cleanup(curl_handle);
    return -1;
  }
 
  /* open the body file */ 
  bodyfile = fopen(bodyfilename, "wb");
  if(!bodyfile) {
    curl_easy_cleanup(curl_handle);
    fclose(headerfile);
    return -1;
  }
 
  /* we want the headers be written to this file handle */ 
  curl_easy_setopt(curl_handle, CURLOPT_HEADERDATA, headerfile);
 
  /* we want the body be written to this file handle instead of stdout */ 
  curl_easy_setopt(curl_handle, CURLOPT_WRITEDATA, bodyfile);
 
  /* get it! */ 
  curl_easy_perform(curl_handle);
 
  /* close the header file */ 
  fclose(headerfile);
 
  /* close the body file */ 
  fclose(bodyfile);
 
  /* cleanup curl stuff */ 
  curl_easy_cleanup(curl_handle);
 
  return 0;
}
