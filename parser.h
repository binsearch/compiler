
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

#ifndef Parser_h_included
#define Parser_h_included

#include "parserbase.h"
#include "scanner.h"

//////////////////////////// Parser Initial Code ///////////////////////////

#include<iostream>
#include<sstream>
#include<string>
#include<vector>
#include<list>
#include<map>

using namespace std;

#include <string.h>

#include"error-display.hh"
#include"user-options.hh"
#include"local-environment.hh"

#include"symbol-table.hh"
#include"ast.hh"
#include"basic-block.hh"
#include"procedure.hh"
#include"program.hh"

////////////////////////////////////////////////////////////////////////////

#undef Parser
class Parser: public ParserBase
{
	Scanner d_scanner;

    public:
	Parser(string input_file_name)
	{
		d_scanner.switchStreams(input_file_name, "");
		d_scanner.setSval(&d_val__);
	}

        int parse();
        void print();

	int get_line_number();					// Used for errors

    private:
        void error(char const *msg);
        int lex();

	bool return_statement_used_flag;				// Keeps track that atleast a procedure has atleast 1 return statement
	void bb_strictly_increasing_order_check(list<Basic_Block *> * bb_list, int bb_number); 
        
	void executeAction(int ruleNr);
        void errorRecovery();
        int lookup(bool recovery);
        void nextToken();
        void print__();
};


#endif

/* Structure of parser

program: 			declaration_statement_list procedure_name procedure_body
				| procedure_name procedure_body

procedure_name: 		NAME '(' ')'

procedure_body:			'{' declaration_statement_list basic_block_list '}'
				| '{' basic_block_list '}'

declaration_statement_list: 	declaration_statement
				| declaration_statement_list 	declaration_statement

declaration_statement: 		INTEGER NAME ';'

basic_block_list: 		basic_block_list 	basic_block
				| basic_block

basic_block: 			'<' NAME INTEGER_NUMBER '>' ':' executable_statement_list

executable_statement_list: 	assignment_statement_list
				| assignment_statement_list RETURN

assignment_statement_list: 	// empty
				| assignment_statement_list assignment_statement

assignment_statement: 		assignment_variable '='	assignment_variable ';'
				| assignment_variable '=' constant ';'

assignment_variable:		NAME

constant:			INTEGER_NUMBER

*/
