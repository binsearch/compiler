
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

#ifndef PROGRAM_HH
#define PROGRAM_HH

#include<string>
#include<map>
#include<list>

#define GLOB_SPACE "   "

using namespace std;

class Program;

extern Program program_object;

class Program
{
	Symbol_Table global_symbol_table;
	map<string, Procedure *> procedure_map;

public:
	Program();
	~Program();
	void delete_all();

	void set_procedure_map(Procedure * proc, int line);
	void set_global_table(Symbol_Table & new_global_table);

	Symbol_Table_Entry & get_symbol_table_entry(string variable);

	void print();

	Procedure * get_main_procedure(ostream & file_buffer);

	Eval_Result & evaluate();

	bool variable_in_proc_map_check(string symbol);
	bool variable_in_symbol_list_check(string variable);
	void global_list_in_proc_map_check();

	// compile
	void compile();
	void print_assembly();
};

#endif
