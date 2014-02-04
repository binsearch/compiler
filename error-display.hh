
/*********************************************************************************************

                                cfglp : A CFG Language Processor
                                --------------------------------

           About:

           Implemented   by  Tanu  Kanvar (tanu@cse.iitb.ac.in) and Uday
           Khedker    (http://www.cse.iitb.ac.in/~uday)  for the courses
           cs302+cs306: Language  Processors  (theory and  lab)  at  IIT
           Bombay.

           Release  date  Jan  15, 2013.  Copyrights  reserved  by  Uday
           Khedker. This  implemenation  has been made  available purely
           for academic purposes without any warranty of any kind.

           Documentation (functionality, manual, and design) and related
           tools are  available at http://www.cse.iitb.ac.in/~uday/cfglp


***********************************************************************************************/

#ifndef ERROR_DISPLAY_HH
#define ERROR_DISPLAY_HH

#include <sstream>
#include <string>

#define NOLINE -1
#define NOTEXIT 0

using namespace std;

void report_error(string error_message, int line);
void print_error(string error_message, int exit_flag);

#define report_internal_error(error_message)									\
	{												\
		stringstream message;									\
		message << "(Internal Error) ";								\
		message << __FILE__ << " : line " << __LINE__ << " :: error : " << error_message;	\
		message << "\nTerminating";							\
		print_error(message.str(), 1);								\
	}

#endif
