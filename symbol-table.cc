
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
#include<iostream>

using namespace std;

#include"local-environment.hh"
#include"user-options.hh"
#include"error-display.hh"

#include"symbol-table.hh"

#include"ast.hh"
#include"basic-block.hh"
#include"procedure.hh"
#include"program.hh"

Symbol_Table::Symbol_Table()
{}

Symbol_Table::~Symbol_Table()
{}

void Symbol_Table::set_table_scope(Table_Scope list_scope)
{
	scope = list_scope;
}

Table_Scope Symbol_Table::get_table_scope()
{
	return scope;
}

void Symbol_Table::push_symbol(Symbol_Table_Entry * variable)
{
	variable_table.push_back(variable);
}

void Symbol_Table::global_list_in_proc_map_check(int line)
{
	list<Symbol_Table_Entry *>::iterator i;
	for (i = variable_table.begin(); i != variable_table.end(); i++)
	{
		string name = (*i)->get_variable_name();
		program_object.variable_in_proc_map_check(name, line);
	}
}

bool Symbol_Table::variable_in_symbol_list_check(string variable)
{
	list<Symbol_Table_Entry *>::iterator i;
	for (i = variable_table.begin(); i != variable_table.end(); i++)
	{
		if ((*i)->get_variable_name() == variable)
			return true;
	}

	return false;
}

Symbol_Table_Entry & Symbol_Table::get_symbol_table_entry(string variable_name)
{
	list<Symbol_Table_Entry *>::iterator i;
	for (i = variable_table.begin(); i != variable_table.end(); i++)
	{
		if ((*i)->get_variable_name() == variable_name)
			return **i;
	}

	report_error("variable symbol entry doesn't exist", NOLINE);
}

void Symbol_Table::create(Local_Environment & local_global_variables_table)
{
	list<Symbol_Table_Entry *>::iterator i;

	for (i = variable_table.begin(); i != variable_table.end(); i++)
	{
		string name = (*i)->get_variable_name();
		Eval_Result_Value_Int * j = new Eval_Result_Value_Int();
		if (scope == global)
		{
			j->set_variable_status(true);
			j->set_value(0);
		}

		local_global_variables_table.put_variable_value(*j, name);
	}
}

/////////////////////////////////////////////////////////////

Symbol_Table_Entry::Symbol_Table_Entry()
{}

Symbol_Table_Entry::Symbol_Table_Entry(string & name, Data_Type new_data_type)
{
	variable_name = name;
	variable_data_type = new_data_type;
}

Symbol_Table_Entry::~Symbol_Table_Entry()
{}

Data_Type Symbol_Table_Entry::get_data_type()
{
	return variable_data_type;
}

string Symbol_Table_Entry::get_variable_name()
{
	return variable_name;
}
