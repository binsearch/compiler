
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

#ifndef BASIC_BLOCK_HH
#define BASIC_BLOCK_HH

#include<string>
#include<map>
#include<list>

#define BB_SPACE "      "

using namespace std;

class Basic_Block;

class Basic_Block
{
	int id_number;
	list<Ast *> statement_list;

	list<Icode_Stmt *> bb_icode_list;

	int lineno;

public:
	Basic_Block(int basic_block_number, int line);
	~Basic_Block();

	int get_bb_number();
	void set_ast_list(list<Ast *> & ast_list);

	void print_bb(ostream & file_buffer);

	Eval_Result & evaluate(Local_Environment & eval_env, ostream & file_buffer);

	// compile
	void compile();
	void print_assembly(ostream & file_buffer);
	void print_icode(ostream & file_buffer);
};

#endif
