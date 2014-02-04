
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

#include <iostream>
#include <cstdlib>
#include <string>
#include <vector>
#include <list>
#include <stdio.h>

using namespace std;

#include"local-environment.hh"
#include"error-display.hh"
#include"user-options.hh"

#include"symbol-table.hh"

#include"ast.hh"
#include"basic-block.hh"
#include"procedure.hh"
#include"program.hh"

#include "parser.h"

int main(int argc, char * argv[]) 
{
	string input_file_name = command_options.process_user_command_options(argc, argv);

	Parser cfglp_parser(input_file_name);

	if(cfglp_parser.parse())
		report_error("Cannot parse the input program", cfglp_parser.get_line_number());

	if (command_options.is_do_eval_selected())
	{
		Eval_Result & result = program_object.evaluate();
		delete &result;
	}

	if (command_options.is_show_ast_selected())
		program_object.print_ast();

	program_object.delete_all();

	return 0;
}
