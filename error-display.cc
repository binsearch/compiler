
/*********************************************************************************************

                                cfglp : A CFG Language Processor
                                --------------------------------

           About:

           Implemented by Tanu  Kanvar (tanu@cse.iitb.ac.in) and Uday
           Khedker (http://www.cse.iitb.ac.in/~uday)  for the courses
           cs302+cs306: Language  Processors (theory and lab)  at IIT
           Bombay.

           Release  date Jan  15, 2013.  Copyrights reserved  by Uday
           Khedker. This implemenation has been made available purely
           for academic purposes without any warranty of any kind.

           A  doxygen   generated  documentation  can  be   found  at
           http://www.cse.iitb.ac.in/~uday/cfglp


***********************************************************************************************/

#include <iostream>
#include <cstdlib>
#include <stdlib.h>
#include <string>

using namespace std;

#include "common-classes.hh"

#include "user-options.hh"
#include "error-display.hh"

void check_invariant_underlying_function(bool condition, string error_message)
{
	if (!condition)
	{
		cerr << "\ncfglp internal error: " << error_message << "\n";

		command_options.remove_files();
		exit(1);
	}
}

bool global_error_status = false;

void report_violation_of_condition(bool condition, string error_message, int lineno)
{
	string file_name = command_options.get_file_name();
	if (!condition)
	{
		cerr <<  "cfglp error: File: " << file_name << ", Line: " << lineno << ": " << error_message << "\n";
		global_error_status = true;
	}
} 

void report_violation_and_abort(bool condition, string error_message, int lineno)
{
	string file_name = command_options.get_file_name();
	if (!condition)
	{
		cerr <<  "cfglp error: File: " << file_name << ", Line: " << lineno << ": " << error_message << "\n";
		exit(1);
	}
} 

bool error_status()
{
	return global_error_status;
}
