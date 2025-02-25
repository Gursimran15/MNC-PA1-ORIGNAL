/**
 * @gursimr2_assignment1
 * @author  Gursimran Singh <gursimr2@buffalo.edu>
 * @version 1.0
 *
 * @section LICENSE
 *
 * This program is free software; you can redistribute it and/or
 * modify it under the terms of the GNU General Public License as
 * published by the Free Software Foundation; either version 2 of
 * the License, or (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful, but
 * WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the GNU
 * General Public License for more details at
 * http://www.gnu.org/copyleft/gpl.html
 *
 * @section DESCRIPTION
 *
 * This contains the main function. Add further description here....
 */
#include <iostream>
#include <stdio.h>
#include <string.h>
#include<time.h>
#include <stdarg.h> 
#include "../include/global.h"
#include "../include/logger.h"

using namespace std;

/**
 * main function
 *
 * @param  argc Number of arguments
 * @param  argv The argument list
 * @return 0 EXIT_SUCCESS
 */
// void log_print(char* filename, int line, char *fmt,...);
// #define LOG_PRINT(...) log_print(__FILE__, __LINE__, __VA_ARGS__ )

// FILE *fp ;
// static int SESSION_TRACKER; //Keeps track of session
// std::string logname = "/tmp/gur_1";


int main(int argc, char **argv)
{
	/*Init. Logger*/
	cse4589_init_log(argv[2]);

	/* Clear LOGFILE*/
    fclose(fopen(LOGFILE, "w"));
	// cout<<argv[2]<<argv[1];
	/*Start Here*/
	// char *ip = "127.0.0.1";
	// char u[]={'s','c'};
	
		if(strcmp(argv[1],"s")==0)
		{ 
			server(argv[2]);}
		if(strcmp(argv[1],"c")==0){
			
		client(argv[2]);}
		
	return 0;
}
