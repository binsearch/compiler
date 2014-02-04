
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

#include<string>
#include<fstream>

using namespace std;

#include"local-environment.hh"
#include"user-options.hh"
#include"error-display.hh"

#include"symbol-table.hh"
#include"ast.hh"
#include"basic-block.hh"
#include"procedure.hh"
#include"program.hh"

Program program_object;
Local_Environment interpreter_global_table;

Program::Program()
{}

Program::~Program()
{}

void Program::delete_all()
{
	map<string, Procedure *>::iterator i;
	for (i = procedure_map.begin(); i != procedure_map.end(); i++)
		delete i->second;
}

void Program::set_global_table(Symbol_Table & new_global_table)
{
	global_symbol_table = new_global_table;
	global_symbol_table.set_table_scope(global);
}

void Program::set_procedure_map(Procedure & proc)
{
	procedure_map[proc.get_proc_name()] = &proc;
}

bool Program::variable_in_symbol_list_check(string variable)
{
	return global_symbol_table.variable_in_symbol_list_check(variable);
}

Symbol_Table_Entry & Program::get_symbol_table_entry(string variable_name)
{
	return global_symbol_table.get_symbol_table_entry(variable_name);
}

void Program::variable_in_proc_map_check(string variable, int line)
{
	if(procedure_map[variable] != NULL)
		report_error("Variable name cannot be same as procedure name", line);
}

Procedure * Program::get_main_procedure(ostream & file_buffer)
{
	map<string, Procedure *>::iterator i;
	for(i = procedure_map.begin(); i != procedure_map.end(); i++)
	{
		if (i->second != NULL && i->second->get_proc_name() == "main")
				return i->second;
	}
	
	return NULL;
}

void Program::print_ast()
{
	command_options.create_ast_buffer();
	ostream & ast_buffer = command_options.get_ast_buffer();

	ast_buffer << "Program:\n";

	Procedure * main = get_main_procedure(ast_buffer);
	if (main == NULL)
		report_error("No main function found in the program", NOLINE);

	else
	{
		main->print_ast(ast_buffer);
	}
}

Eval_Result & Program::evaluate()
{
	Procedure * main = get_main_procedure(command_options.get_output_buffer());
	if (main == NULL)
		report_error("No main function found in the program", NOLINE);

	global_symbol_table.create(interpreter_global_table);

	command_options.create_output_buffer();
	ostream & file_buffer = command_options.get_output_buffer();
	file_buffer << "Evaluating Program\n";
	file_buffer << GLOB_SPACE << "Global Variables (before evaluating):\n";
	interpreter_global_table.print(file_buffer);

	Eval_Result & result = main->evaluate(file_buffer);

	file_buffer << GLOB_SPACE << "Global Variables (after evaluating):\n";
	interpreter_global_table.print(file_buffer);

	return result;
}
