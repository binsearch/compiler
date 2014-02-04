
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

#ifndef USER_OPTIONS_HH
#define USER_OPTIONS_HH

#include <iostream>
#include <stdio.h>
#include <fstream>
#include <ostream>

using namespace std;

class User_Options;

class User_Options
{
	string source_file;

	bool show_tokens_selected;
	bool show_ast_selected;
	bool do_eval_selected;
	bool demo_mode_selected;

	ostream * tokens_buffer;
	ostream * ast_buffer;
	ostream * output_buffer;

	ofstream tokens_file_buffer;
	ofstream ast_file_buffer;
	ofstream output_file_buffer;

	FILE * file_buffer;

public:
	User_Options(); 
	~User_Options();

	bool is_show_tokens_selected();
	bool is_show_ast_selected();
	bool is_do_eval_selected();
	bool is_demo_mode_selected();

	string process_user_command_options(int argc, char * argv[]);
	string get_file_name();

	void create_tokens_buffer();
	void create_ast_buffer();
	void create_output_buffer();

	ostream & get_tokens_buffer();
	ostream & get_ast_buffer();
	ostream & get_output_buffer();
};

extern User_Options command_options;

#endif
