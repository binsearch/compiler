
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

#include"common-classes.hh"
#include"local-environment.hh"
#include"user-options.hh"
#include"error-display.hh"
#include"user-options.hh"
#include"icode.hh"
#include"reg-alloc.hh"
#include"symbol-table.hh"
#include"ast.hh"
#include"basic-block.hh"
#include"procedure.hh"
#include"program.hh"

Symbol_Table::Symbol_Table()
{
	start_offset_of_first_symbol = 0;
	size_in_bytes = 0;
}

Symbol_Table::~Symbol_Table()
{}

void Symbol_Table::set_table_scope(Table_Scope list_scope)
{
	scope = list_scope;

	list<Symbol_Table_Entry *>::iterator i;
	for(i = variable_table.begin(); i != variable_table.end(); i++)
		(*i)->set_symbol_scope(list_scope);
}

Table_Scope Symbol_Table::get_table_scope()
{
	return scope;
}

bool Symbol_Table::is_empty()
{
	return variable_table.empty();
}

void Symbol_Table::push_symbol(Symbol_Table_Entry * variable)
{
	variable_table.push_back(variable);
}

void Symbol_Table::global_list_in_proc_map_check()
{
	list<Symbol_Table_Entry *>::iterator i;
	for (i = variable_table.begin(); i != variable_table.end(); i++)
	{
		string name = (*i)->get_variable_name();
		CHECK_INPUT((program_object.variable_in_proc_map_check(name) == false),
			"Global variable should not match procedure name", NO_FILE_LINE);
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

	CHECK_INVARIANT(CONTROL_SHOULD_NOT_REACH, "The variable symbol entry doesn't exist");
}

void Symbol_Table::create(Local_Environment & local_global_variables_table)
{
	list<Symbol_Table_Entry *>::iterator i;

	for (i = variable_table.begin(); i != variable_table.end(); i++)
	{
		string name = (*i)->get_variable_name();
		if ((*i)->get_data_type() == int_data_type)
		{
			Eval_Result * j = new Eval_Result_Value_Int();
			if (scope == global)
			{
				j->set_variable_status(true);
				j->set_value(0);
			}

			local_global_variables_table.put_variable_value(*j, name);
		}
	}
}

void Symbol_Table::print(ostream & file_buffer)
{
	list<Symbol_Table_Entry *>::iterator i;

	for(i = variable_table.begin(); i != variable_table.end(); i++)
	{
		string name = (*i)->get_variable_name();
		Data_Type dt = (*i)->get_data_type();
		int start_off = (*i)->get_start_offset();
		int end_off = (*i)->get_end_offset();

		file_buffer << "   Name: " << name;

		switch(dt)
		{
		case int_data_type: file_buffer << " Type: INT"; break;
		defualt: CHECK_INVARIANT(CONTROL_SHOULD_NOT_REACH, "Variable data type can only be int");
		} 

		file_buffer << " Entity Type: VAR";

		if (start_off == end_off)
			file_buffer << " (No offset assigned yet)\n";
		else
			file_buffer << " Start Offset: " << start_off << " End Offset: " << end_off << "\n";
	}
}

// Compile

void Symbol_Table::set_start_offset_of_first_symbol(int n)
{
	start_offset_of_first_symbol = n;
}

int Symbol_Table::get_start_offset_of_first_symbol()
{
	return start_offset_of_first_symbol;
}

void Symbol_Table::set_size(int n)
{
	size_in_bytes;
}

int Symbol_Table::get_size()
{
	return size_in_bytes;
}

void Symbol_Table::assign_offsets()
{
	list<Symbol_Table_Entry *>::iterator i;
	for (i = variable_table.begin(); i != variable_table.end(); i++)
	{
		int size = get_size_of_value_type((*i)->get_data_type());
		(*i)->set_start_offset(size_in_bytes);
		size_in_bytes += size;
		(*i)->set_end_offset(size_in_bytes);
	}
}

int Symbol_Table::get_size_of_value_type(Data_Type dt)
{
	switch(dt)
	{
	case int_data_type: return 4; break;
	case void_data_type: CHECK_INVARIANT(CONTROL_SHOULD_NOT_REACH, "Attempt to seek size of type void");
	defualt: CHECK_INVARIANT(CONTROL_SHOULD_NOT_REACH, "Data type not supperted");
	}
}

void Symbol_Table::print_assembly(ostream & file_buffer)
{
	list<Symbol_Table_Entry *>::iterator i;
	for (i = variable_table.begin(); i != variable_table.end(); i++)
	{
		if (scope == global)
			file_buffer << (*i)->get_variable_name() << ":\t.word 0\n";
	}
}

/////////////////////////////////////////////////////////////

Symbol_Table_Entry::Symbol_Table_Entry()
{
	variable_data_type = void_data_type;
	start_offset = end_offset = 0;
	register_description = NULL;
}

Symbol_Table_Entry::Symbol_Table_Entry(string & name, Data_Type new_data_type, int line)
{
	variable_name = name;
	variable_data_type = new_data_type;

	start_offset = end_offset = 0;
	register_description = NULL;

	lineno = line;
}

Symbol_Table_Entry::~Symbol_Table_Entry()
{}

bool Symbol_Table_Entry::operator==(Symbol_Table_Entry & entry)
{
	if (variable_name != entry.get_variable_name())
		return false;
	else if (variable_data_type != entry.get_data_type())
		return false;
	else if (scope != entry.get_symbol_scope())
		return false;

	return true;
}

void Symbol_Table_Entry::set_symbol_scope(Table_Scope sp)
{
	scope = sp;
}

Table_Scope Symbol_Table_Entry::get_symbol_scope()
{
	return scope;
}

int Symbol_Table_Entry::get_lineno()
{
	return lineno;
}

Data_Type Symbol_Table_Entry::get_data_type()
{
	return variable_data_type;
}

string Symbol_Table_Entry::get_variable_name()
{
	return variable_name;
}

// Compile

int Symbol_Table_Entry::get_start_offset()	{ return start_offset; }
int Symbol_Table_Entry::get_end_offset()	{ return end_offset; }
void Symbol_Table_Entry::set_start_offset(int n) { start_offset = n; }
void Symbol_Table_Entry::set_end_offset(int n)	{ end_offset = n; }

void Symbol_Table_Entry::set_register(Register_Descriptor * reg)
{
	register_description = reg;
}

Register_Descriptor * Symbol_Table_Entry::get_register()
{
	return register_description;
}

void Symbol_Table_Entry::free_register(Register_Descriptor * destination_reg_descr)
{
	CHECK_INVARIANT((destination_reg_descr != NULL), 
			"The register descriptor of the destination should not be NULL while freeing a register");

	/* Remove the destination from its register descriptor */
	destination_reg_descr->remove_symbol_entry_from_list(*this);

	/* Remove the register descriptor from the destination */
	set_register(NULL);
}

void Symbol_Table_Entry::update_register(Register_Descriptor * result_reg_descr)
{
	CHECK_INVARIANT((result_reg_descr != NULL), 
			"The register descriptor of the result should not be NULL while updating register information");

	Register_Descriptor * destination_reg_descr = get_register();

	if(destination_reg_descr != NULL)
		free_register(destination_reg_descr);

	/* set the register of the destination as the new register */
	set_register(result_reg_descr);

	/* add the destination to the list of sym of the register */
	result_reg_descr->update_symbol_information(*this);
}
