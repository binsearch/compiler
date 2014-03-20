
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

#include"common-classes.hh"
#include"local-environment.hh"
#include"user-options.hh"
#include"error-display.hh"
#include"icode.hh"
#include"reg-alloc.hh"
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

void Program::set_procedure_map(Procedure * proc, int line)
{
	CHECK_INVARIANT((proc != NULL), "Procedure cannot be null");

	procedure_map[proc->get_proc_name()] = proc;
}

bool Program::variable_in_symbol_list_check(string variable)
{
	return global_symbol_table.variable_in_symbol_list_check(variable);
}

Symbol_Table_Entry & Program::get_symbol_table_entry(string variable_name)
{
	return global_symbol_table.get_symbol_table_entry(variable_name);
}

bool Program::variable_in_proc_map_check(string symbol)
{
	if (procedure_map.find(symbol) == procedure_map.end())
		return false;

	return true;
}

void Program::global_list_in_proc_map_check()
{
	global_symbol_table.global_list_in_proc_map_check();
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

void Program::print()
{
	ostream * file_buffer;

	if(command_options.is_show_ast_selected())
	{
		command_options.create_ast_buffer();
		file_buffer = &(command_options.get_ast_buffer());
	}

	else if (command_options.is_show_program_selected())
	{
		command_options.create_program_buffer();
		file_buffer = &(command_options.get_program_buffer());
	}

	else if (command_options.is_show_symtab_selected())
	{
		command_options.create_symtab_buffer();
		file_buffer = &(command_options.get_symtab_buffer());
	}

	*file_buffer << "Program:\n";

	if (command_options.is_show_program_selected() || command_options.is_show_symtab_selected())
	{
		*file_buffer << "\nGlobal Declarations:\n";
		global_symbol_table.print(*file_buffer);
	}

	map<string, Procedure *>::iterator i;
	for(i = procedure_map.begin(); i != procedure_map.end(); i++)
		i->second->print(*file_buffer);
}

Eval_Result & Program::evaluate()
{
	Procedure * main = get_main_procedure(command_options.get_output_buffer());
	CHECK_INPUT_AND_ABORT((main != NULL), 
		"No main function found in the program", NO_FILE_LINE);

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

void Program::compile()
{
	// set up machine details
	machine_dscr_object.initialize_register_table();
	machine_dscr_object.initialize_instruction_table();
	
	// assign offsets to global variables
	global_symbol_table.set_start_offset_of_first_symbol(0);
	global_symbol_table.set_size(0);
	global_symbol_table.assign_offsets();
	
	// compile the program by visiting each procedure
	map<string, Procedure *>::iterator i;
	for(i = procedure_map.begin(); i != procedure_map.end(); i++)
	{
		i->second->compile();
		if(command_options.is_show_ic_selected())
		{
			ostream * file_buffer;
			command_options.create_ic_buffer();
			file_buffer = &(command_options.get_ic_buffer());
		
			i->second->print_icode(*file_buffer);
		}
	}
	// print assembly language
	if (!((command_options.is_do_lra_selected() == true) && 
		(command_options.is_do_compile_selected() == false)))
		print_assembly();
}

void Program::print_assembly()
{
	command_options.create_output_buffer();
	ostream & file_buffer = command_options.get_output_buffer();

	if (!global_symbol_table.is_empty())
		file_buffer << "\n\t.data\n";

	global_symbol_table.print_assembly(file_buffer);

	// print each procedure
	map<string, Procedure *>::iterator i;
	for (i = procedure_map.begin(); i != procedure_map.end(); i++)
		i->second->print_assembly(file_buffer);
}
