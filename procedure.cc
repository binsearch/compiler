
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
#include<iostream>

using namespace std;

#include"error-display.hh"
#include"local-environment.hh"

#include"symbol-table.hh"
#include"ast.hh"
#include"basic-block.hh"
#include"procedure.hh"
#include"program.hh"

Procedure::Procedure(Data_Type proc_return_type, string proc_name)
{
	return_type = proc_return_type;
	name = proc_name;
}

Procedure::~Procedure()
{
	list<Basic_Block *>::iterator i;
	for (i = basic_block_list.begin(); i != basic_block_list.end(); i++)
		delete (*i);
}

string Procedure::get_proc_name()
{
	return name;
}

void Procedure::set_basic_block_list(list<Basic_Block *> bb_list)
{
	basic_block_list = bb_list;
}

void Procedure::set_local_list(Symbol_Table & new_list)
{
	// printf("nannna\n");
	// // list<Symbol_Table_Entry*> l1=local_symbol_table.variable_table;
	// printf("yo\n");
	// printf("yay %d \n",new_list.variable_table.size());
	// printf("%d",local_symbol_table.variable_table.size());
	// printf("dude\n");

	// // Symbol_Table s=new_list;
	// // printf("ddue\n");
	// local_symbol_table.variable_table=new_list.variable_table;
	// // printf("nayna\n");

	// list<Symbol_Table_Entry *> l=new_list.variable_table;
	// for(list<Symbol_Table_Entry *>::iterator it=l.begin();it!=l.end();it++) {
	// 	if((*it)==NULL)
	// 		cout<<"what\n";
	// 	else
	// 		printf("nayna\n");
	// 	cout<<(*it)->variable_name<<" "<<(*it)->variable_data_type<<endl;
	// }
	// // local_symbol_table.variable_table=new_list.variable_table;
	// printf("man\n");
	// local_symbol_table.
	// printf("ohhh\n");
	// printf("o re piya\n");
	// // for(list<Symbol_Table_Entry *>::iterator it=)
	local_symbol_table = new_list;
	// printf("mann\n");
	local_symbol_table.set_table_scope(local);
}

void Procedure::set_argument_list(Symbol_Table & new_list)
{
	argument_table=new_list;
	argument_table.set_table_scope(local);
}

Data_Type Procedure::get_return_type()
{
	return return_type;
}

bool Procedure::variable_in_symbol_list_check(string variable)
{
	return local_symbol_table.variable_in_symbol_list_check(variable) | argument_table.variable_in_symbol_list_check(variable);
}

Symbol_Table_Entry & Procedure::get_symbol_table_entry(string variable_name)
{
	if(local_symbol_table.variable_in_symbol_list_check(variable_name))
		return local_symbol_table.get_symbol_table_entry(variable_name);
	if(argument_table.variable_in_symbol_list_check(variable_name))
		return argument_table.get_symbol_table_entry(variable_name);

	report_error("variable symbol entry doesn't exist", NOLINE);
}

void Procedure::print_ast(ostream & file_buffer)
{
	file_buffer << PROC_SPACE << "Procedure: " <<name<< "\n";

	list<Basic_Block *>::iterator i;
	for(i = basic_block_list.begin(); i != basic_block_list.end(); i++)
		(*i)->print_bb(file_buffer);
}
	
Basic_Block & Procedure::get_start_basic_block()
{
	list<Basic_Block *>::iterator i;
	i = basic_block_list.begin();
	return **i;
}

Basic_Block * Procedure::get_next_bb(Basic_Block & current_bb)
{
	bool flag = false;

	list<Basic_Block *>::iterator i;
	for(i = basic_block_list.begin(); i != basic_block_list.end(); i++)
	{
		if((*i)->get_bb_number() == current_bb.get_bb_number())
		{
			flag = true;
			continue;
		}
		if (flag)
			return (*i);
	}
	
	return NULL;
}

Symbol_Table & Procedure::get_symbol_table(){
	return local_symbol_table;
}

Eval_Result & Procedure::evaluate(ostream & file_buffer)
{
	Local_Environment & eval_env = *new Local_Environment();
	local_symbol_table.create(eval_env);
	
	Eval_Result * result = NULL;

	file_buffer << PROC_SPACE << "Evaluating Procedure " << name << "\n";
	file_buffer << LOC_VAR_SPACE << "Local Variables (before evaluating):\n";
	eval_env.print(file_buffer);
	file_buffer << "\n";
	
	Basic_Block * current_bb = &(get_start_basic_block());
	while (current_bb)
	{
		result = &(current_bb->evaluate(eval_env, file_buffer));
		int ret = result->get_value();
		// cout<<current_bb->get_bb_number()<<" "<<ret<<endl; 
		if(result->get_result_enum() == bb_result){
			list<Basic_Block *>::iterator i;
			for(i = basic_block_list.begin(); i != basic_block_list.end(); i++)
			{	
				if((*i)->get_bb_number() == ret){
					// cout<<ret<<" yo "<<(*i)->get_bb_number()<<endl;
					current_bb = *i;
					break;
				}
			}
		}
		else if(result->get_result_enum()==return_result) {
			break;
		}
		else{
			current_bb = get_next_bb(*current_bb);
		}

	}

	file_buffer << "\n\n";
	file_buffer << LOC_VAR_SPACE << "Local Variables (after evaluating):\n";
	eval_env.print(file_buffer);

	return *result;
}


