
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
#include"error-display.hh"
#include"user-options.hh"
#include"icode.hh"
#include"reg-alloc.hh"
#include"symbol-table.hh"
#include"ast.hh"
#include"basic-block.hh"

Basic_Block::Basic_Block(int basic_block_number, int line)
{
	id_number = basic_block_number;

	lineno = line;
}

Basic_Block::~Basic_Block()
{
	list<Ast *>::iterator i;
	for (i = statement_list.begin(); i != statement_list.end(); i++)
		delete (*i);
}

void Basic_Block::set_ast_list(list<Ast *> & ast_list)
{
	statement_list = ast_list;
}
int Basic_Block::get_bb_number()
{
	return id_number;
}
void Basic_Block::print_bb(ostream & file_buffer)
{
	file_buffer << BB_SPACE << "Basic_Block " << id_number << "\n";

	list<Ast *>::iterator i;
	for(i = statement_list.begin(); i != statement_list.end(); i++)
		(*i)->print(file_buffer);
}

Eval_Result & Basic_Block::evaluate(Local_Environment & eval_env, ostream & file_buffer)
{
	Eval_Result * result = NULL;

	file_buffer << "\n" << BB_SPACE << "Basic Block: " << id_number << "\n";

	list <Ast *>::iterator i;
	for (i = statement_list.begin(); i != statement_list.end(); i++)
	{
		CHECK_INVARIANT(((*i) != NULL), "Ast pointer seems to be NULL into the basic block");
		result = &((*i)->evaluate(eval_env, file_buffer)); 
	}

	return *result;
}

void Basic_Block::compile()
{
	Code_For_Ast ast_code;

	machine_dscr_object.validate_init_local_register_mapping();

	// compile the program by visiting each ast in the block
	list<Ast *>::iterator i;
	for (i = statement_list.begin(); i != statement_list.end(); i++)
	{
		Ast * ast = *i;

		if (typeid(*ast) != typeid(Return_Ast))
		{
			if (command_options.is_do_lra_selected() == true)
			{
				Lra_Outcome lra;
				ast_code = ast->compile_and_optimize_ast(lra);
			}

			else
				ast_code = ast->compile();

			list<Icode_Stmt *> & ast_icode_list = ast_code.get_icode_list();
			if (ast_icode_list.empty() == false)
			{
				if (bb_icode_list.empty())
					bb_icode_list = ast_icode_list;
				else
					bb_icode_list.splice(bb_icode_list.end(), ast_icode_list);
			}
		}
	}

	machine_dscr_object.clear_local_register_mappings();
}

void Basic_Block::print_assembly(ostream & file_buffer)
{
	file_buffer<<"\n";
	file_buffer<< "label" << id_number << ":\n"; 
	list<Icode_Stmt *>::iterator i;
	for (i = bb_icode_list.begin(); i != bb_icode_list.end(); i++)
		(*i)->print_assembly(file_buffer);
}

void Basic_Block::print_icode(ostream & file_buffer)
{
	file_buffer<<"\n";
	file_buffer<< "label" << id_number << ":\n"; 
	list<Icode_Stmt *>::iterator i;
	int j = 0;
	cout << "in basic block print icode" << endl;
	for (i = bb_icode_list.begin(); i != bb_icode_list.end(); i++){
		cout << "printing " << ++j << " instruction\n";
		// if(j == 3) continue;
 		(*i)->print_icode(file_buffer);
	}
	cout << "out of bb icode" << endl;
}
