
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
#include <fstream>
#include <ostream>
#include <istream>
#include <string>
#include <string.h>
#include <stdio.h>
#include <stdlib.h>

using namespace std;

#include "user-options.hh"
#include "error-display.hh"

User_Options command_options;

User_Options::User_Options() 
{
	show_tokens_selected = false;
	show_ast_selected = false;
	do_eval_selected = false;
	demo_mode_selected = false;
} 

User_Options::~User_Options()
{
	if (demo_mode_selected == false)
	{
		if (show_tokens_selected)
			tokens_file_buffer.close(); 

		if (show_ast_selected)
			ast_file_buffer.close(); 

		output_file_buffer.close(); 
	}
}

bool User_Options::is_show_tokens_selected()
{
	return show_tokens_selected;
}

bool User_Options::is_show_ast_selected()
{
	return show_ast_selected;
}

bool User_Options::is_do_eval_selected()
{
	return do_eval_selected;
}

bool User_Options::is_demo_mode_selected()
{
	return demo_mode_selected;
}

void User_Options::create_tokens_buffer()
{
	if (!show_tokens_selected)
		report_internal_error("Show tokens option not set");

	if (demo_mode_selected)
		tokens_buffer = &cout;

	else
		tokens_buffer = &tokens_file_buffer; 
}

void User_Options::create_ast_buffer()
{
	if (!show_ast_selected)
		report_internal_error("Show AST option not set");

	if (demo_mode_selected)
		ast_buffer = &cout;

	else
		ast_buffer = &ast_file_buffer; 
}

void User_Options::create_output_buffer()
{ 
	if (demo_mode_selected)
		output_buffer = &cout;
	else
		output_buffer = &output_file_buffer; 
}

ostream & User_Options::get_tokens_buffer()
{
	if (tokens_buffer == NULL)
		tokens_buffer = &cout;

	return *tokens_buffer;
}

ostream & User_Options::get_ast_buffer()
{
	if (ast_buffer == NULL)
		ast_buffer = &cout;

	return *ast_buffer;
}

ostream & User_Options::get_output_buffer()
{
	if (output_buffer == NULL)
		output_buffer = &cout;

	return *output_buffer;
}

string User_Options::get_file_name()
{
	return source_file;
}

string User_Options::process_user_command_options(int argc, char * argv[])
{
	string input_file_name;

	bool user_input_file_entered = false;
	string user_input_file_name, tokens_file_name, ast_file_name, output_file_name;
	char * user_input_file_c_string = NULL;

	const string usage = "\n     Usage: cfglp [options] [file]\n\
			Options:\n\
			-help     Show this help\n\
			-tokens   Show the tokens in file.toks (or out.toks)\n\
			-ast      Show abstract syntax trees in file.ast (or out.ast)\n\
			-eval     Interpret the program and show a trace of the execution in file.eval (or out.eval)\n\
			-d        Demo version. Use stdout for the outputs instead of files\n\n";

	for (int i = 1; i < argc; i++)
	{    
		char * option = strdup(argv[i]);
		if (!strncmp(option,"-",1))
		{
			if (!strcmp(option,"-d"))
				demo_mode_selected = true;

			else if (!strcmp(option,"-tokens")) 
				show_tokens_selected = true;

			else if (!strcmp(option,"-ast")) 
				show_ast_selected = true;
			
			else if (!strcmp(option,"-eval"))
				do_eval_selected = true;
			
			else if (!(strcmp(option,"--help")) || !(strcmp(option,"-help")))
			{        
				cerr << usage;
				exit(0);
			}
			else 
			{
				string error_message = "Unknown option `" + string(option) + "'" + usage;
				report_internal_error(error_message);
			}
		}
		else if (user_input_file_entered)
		{
			string error_message = "Only one input file name can be provided" + usage;
			report_internal_error(error_message);
		}
		else    
		{    
			user_input_file_entered = true;
			user_input_file_c_string = strdup(option);
		}
	}

	if (user_input_file_entered == true)
	{
		if (!user_input_file_c_string)
			report_internal_error ("Input file string cannot be NULL");

		user_input_file_name = string (user_input_file_c_string);
		source_file = user_input_file_name;
		input_file_name = user_input_file_name;
	}
	else
		user_input_file_name = "out";

	if (!user_input_file_name.c_str())
		report_internal_error("Input file name cannot be NULL");

	tokens_file_name = user_input_file_name + ".toks"; 
	ast_file_name = user_input_file_name + ".ast"; 

	if (do_eval_selected)
		output_file_name = user_input_file_name + ".eval"; 

	remove (tokens_file_name.c_str()); 
	remove (ast_file_name.c_str()); 
	remove (output_file_name.c_str()); 
	remove ("out.toks");
	remove ("out.ast");
	remove ("out.eval");

	if (demo_mode_selected == false)
	{
		if (show_tokens_selected)
			tokens_file_buffer.open(tokens_file_name.c_str()); 

		if (show_ast_selected)
			ast_file_buffer.open(ast_file_name.c_str()); 

		output_file_buffer.open(output_file_name.c_str()); 

		if (show_tokens_selected && !tokens_file_buffer)
	        	report_internal_error("Unable to open output file for tokens");

		if (show_ast_selected && !ast_file_buffer)
		        report_internal_error("Unable to open output file for AST");

		if(do_eval_selected && !output_file_buffer)
			report_internal_error("Unable to open output file");
	}

	return input_file_name;
}
