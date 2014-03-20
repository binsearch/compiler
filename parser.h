
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
#include <string.h>

using namespace std;

#include"common-classes.hh"
#include"error-display.hh"
#include"user-options.hh"
#include"local-environment.hh"
#include"reg-alloc.hh"
#include"symbol-table.hh"
#include"ast.hh"
#include"basic-block.hh"
#include"procedure.hh"
#include"icode.hh"
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

		NOT_ONLY_PARSE = command_options.not_only_parse;
	}

        int parse();
        void print();

	int get_line_number();					// Used for errors

	bool NOT_ONLY_PARSE;

    private:
        void error(char const *msg);
        int lex();

	void bb_strictly_increasing_order_check(list<Basic_Block *> * bb_list, int bb_number); 
        
	void executeAction(int ruleNr);
        void errorRecovery();
        int lookup(bool recovery);
        void nextToken();
        void print__();
};


#endif

/* Structure of parser

program:
	optional_declaration_list procedure_definition

optional_declaration_list:
|	variable_declaration_list

procedure_definition:
	NAME '(' ')'
	'{' optional_variable_declaration_list
	basic_block_list '}'

optional_variable_declaration_list:
|	variable_declaration_list

variable_declaration_list:
	variable_declaration
|	variable_declaration_list variable_declaration

variable_declaration:
	declaration ';'

declaration:
	INTEGER NAME

basic_block_list:
	basic_block_list basic_block
|	basic_block

basic_block:
	BBNUM ':' executable_statement_list

executable_statement_list:
	assignment_statement_list
|	assignment_statement_list RETURN ';'

assignment_statement_list:
|	assignment_statement_list assignment_statement

assignment_statement:
	variable ASSIGN variable ';'
|	variable ASSIGN constant ';'

variable:
	NAME

constant:
	INTEGER_NUMBER

*/
