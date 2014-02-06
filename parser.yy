
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

%scanner ../scanner.h
%scanner-token-function d_scanner.lex()
%filenames parser
%parsefun-source parser.cc

%union 
{
	int integer_value;
	std::string * string_value;
	list<Ast *> * ast_list;
	Ast * ast;
	Symbol_Table * symbol_table;
	Symbol_Table_Entry * symbol_entry;
	Basic_Block * basic_block;
	list<Basic_Block *> * basic_block_list;
	Procedure * procedure;
};

%token <integer_value> INTEGER_NUMBER
%token <integer_value> BASIC_BLOCK
%token <string_value> NAME
%token RETURN
%token INTEGER
%token IF
%token ELSE
%token GOTO
%token ASSIGN_OP
%token <string_value> NE
%token <string_value> EQ
%token <string_value> LT
%token <string_value> LE
%token <string_value> GT
%token <string_value> GE 

%type <symbol_table> declaration_statement_list
%type <symbol_entry> declaration_statement
%type <basic_block_list> basic_block_list
%type <basic_block> basic_block
%type <ast_list> executable_statement_list
%type <ast_list> assignment_statement_list
%type <ast> assignment_statement
%type <ast> comparision_expression
%type <ast> variable
%type <ast> constant

%start program

%%

program:
	declaration_statement_list procedure_name
	{
		#if 1
		program_object.set_global_table(*$1);
		return_statement_used_flag = false;				// No return statement in the current procedure till now
		#endif
	}
	procedure_body
	{
		#if 1
		program_object.set_procedure_map(*current_procedure);

		if ($1)
			$1->global_list_in_proc_map_check(get_line_number());

		delete $1;
		#endif
	}
|
	procedure_name
	{
		#if 1
		return_statement_used_flag = false;				// No return statement in the current procedure till now
		#endif
	}
	procedure_body
	{	
		#if 1
		
		program_object.set_procedure_map(*current_procedure);
		#endif
	}
;

procedure_name:
	NAME '(' ')'
	{	
		#if 1
		current_procedure = new Procedure(void_data_type, *$1);
		#endif
	}
;

procedure_body:
	'{' declaration_statement_list
	{	
		#if 1
		current_procedure->set_local_list(*$2);
		delete $2;
		#endif
	}
	basic_block_list '}'
	{
		#if 1
		
		if (return_statement_used_flag == false)
		{
			int line = get_line_number();
			report_error("Atleast 1 basic block should have a return statement", line);
		}

		current_procedure->set_basic_block_list(*$4);

		delete $4;
		#endif
	}
|
	'{' basic_block_list '}'
	{
		#if 1
		if (return_statement_used_flag == false)
		{
			int line = get_line_number();
			report_error("Atleast 1 basic block should have a return statement", line);
		}

		current_procedure->set_basic_block_list(*$2);

		delete $2;
		#endif
	}
;

declaration_statement_list:
	declaration_statement
	{
		#if 1
		int line = get_line_number();
		program_object.variable_in_proc_map_check($1->get_variable_name(), line);

		string var_name = $1->get_variable_name();
		if (current_procedure && current_procedure->get_proc_name() == var_name)
		{
			int line = get_line_number();
			report_error("Variable name cannot be same as procedure name", line);
		}

		$$ = new Symbol_Table();
		$$->push_symbol($1);
		#endif
	}
|
	declaration_statement_list declaration_statement
	{
		#if 1
		// if declaration is local then no need to check in global list
		// if declaration is global then this list is global list

		int line = get_line_number();
		program_object.variable_in_proc_map_check($2->get_variable_name(), line);

		string var_name = $2->get_variable_name();
		if (current_procedure && current_procedure->get_proc_name() == var_name)
		{
			int line = get_line_number();
			report_error("Variable name cannot be same as procedure name", line);
		}

		if ($1 != NULL)
		{
			if($1->variable_in_symbol_list_check(var_name))
			{
				int line = get_line_number();
				report_error("Variable is declared twice", line);
			}

			$$ = $1;
		}

		else
			$$ = new Symbol_Table();

		$$->push_symbol($2);
		#endif
	}
;

declaration_statement:
	INTEGER NAME ';'
	{
		#if 1
		$$ = new Symbol_Table_Entry(*$2, int_data_type);

		delete $2;
		#endif
	}
;

basic_block_list:
	basic_block_list basic_block
	{
		#if 1
		if (!$2)
		{
			int line = get_line_number();
			report_error("Basic block doesn't exist", line);
		}

		bb_strictly_increasing_order_check($$, $2->get_bb_number());

		$$ = $1;
		$$->push_back($2);
		#endif
	}
|
	basic_block
	{
		#if 1
		if (!$1)
		{
			int line = get_line_number();
			report_error("Basic block doesn't exist", line);
		}

		$$ = new list<Basic_Block *>;
		$$->push_back($1);
		#endif
	}
	
;

basic_block:
	BASIC_BLOCK ':' executable_statement_list
	{
		#if 1
		if ($3 != NULL)
		$$ = new Basic_Block($1, *$3);
		else
		{
			list<Ast *> * ast_list = new list<Ast *>;
			$$ = new Basic_Block($1, *ast_list);
		}

		delete $3;
				
		#endif
	}
;

executable_statement_list:
	assignment_statement_list
	{
		#if 1
		$$ = $1;
		#endif
	}
|
	assignment_statement_list RETURN ';'
	{
		#if 1
		Ast * ret = new Return_Ast();

		return_statement_used_flag = true;					// Current procedure has an occurrence of return statement

		if ($1 != NULL)
			$$ = $1;

		else
			$$ = new list<Ast *>;

		$$->push_back(ret);
		#endif
	}
;

assignment_statement_list:
	{
		#if 1
		$$ = NULL;
		#endif
	}
|
	assignment_statement_list assignment_statement
	{
		#if 1
		if ($1 == NULL)
			$$ = new list<Ast *>;

		else
			$$ = $1;

		$$->push_back($2);
		#endif
	}
;

/*assignment statements and if clauses*/

assignment_statement:
	variable ASSIGN_OP variable ';'
	{
		#if 1
		$$ = new Assignment_Ast($1, $3);

		int line = get_line_number();
		$$->check_ast(line);
		#endif
	}
|
	variable ASSIGN_OP constant ';'
	{
		#if 1
		$$ = new Assignment_Ast($1, $3);

		int line = get_line_number();
		$$->check_ast(line);
		#endif
	}
	
|
	variable ASSIGN_OP comparision_expression ';'
	{
		$$=new Assignment_Ast($1,$3);

	}
|
	if_block
	{}
|
	goto_statement	
	{}
;

/* if clause */

if_block:

	IF '(' comparision_expression ')'
	goto_statement
	ELSE
	goto_statement
	{}
;

goto_statement:
	
	GOTO BASIC_BLOCK ';'
	{}
;

comparision_expression:

	variable GT comparision_expression
	{
		$$=new Relational_Expr_Ast($1,$3,*$2);
	}
|
	variable LT comparision_expression
	{
		$$=new Relational_Expr_Ast($1,$3,*$2);
	}
|
	variable GE comparision_expression
	{
		$$=new Relational_Expr_Ast($1,$3,*$2);
	}
|
	variable LE comparision_expression
	{
		$$=new Relational_Expr_Ast($1,$3,*$2);
	}
|
	variable EQ comparision_expression
	{
		$$=new Relational_Expr_Ast($1,$3,*$2);
	}
|
	variable NE comparision_expression
	{
		$$=new Relational_Expr_Ast($1,$3,*$2);
	}
|

	variable GT constant
	{
		$$=new Relational_Expr_Ast($1,$3,*$2);
	}
|
	variable LT constant
	{
		$$=new Relational_Expr_Ast($1,$3,*$2);
	}
|
	variable GE constant
	{
		$$=new Relational_Expr_Ast($1,$3,*$2);
	}
|
	variable LE constant
	{
		$$=new Relational_Expr_Ast($1,$3,*$2);
	}
|
	variable EQ constant
	{
		$$=new Relational_Expr_Ast($1,$3,*$2);
	}
|
	variable NE constant
	{
		$$=new Relational_Expr_Ast($1,$3,*$2);
	}
|
	variable GT variable
	{
		$$=new Relational_Expr_Ast($1,$3,*$2);
	}
|
	variable LT variable
	{
		$$=new Relational_Expr_Ast($1,$3,*$2);
	}
|
	variable GE variable
	{
		$$=new Relational_Expr_Ast($1,$3,*$2);
	}
|
	variable LE variable
	{
		$$=new Relational_Expr_Ast($1,$3,*$2);
	}
|
	variable EQ variable
	{
		$$=new Relational_Expr_Ast($1,$3,*$2);
	}
|
	variable NE variable
	{
		$$=new Relational_Expr_Ast($1,$3,*$2);
	}
|
	constant GT constant
	{
		$$=new Relational_Expr_Ast($1,$3,*$2);
	}
|
	constant LT constant
	{
		$$=new Relational_Expr_Ast($1,$3,*$2);
	}
|
	constant GE constant
	{
		$$=new Relational_Expr_Ast($1,$3,*$2);
	}
|
	constant LE constant
	{
		$$=new Relational_Expr_Ast($1,$3,*$2);
	}
|
	constant EQ constant
	{
		$$=new Relational_Expr_Ast($1,$3,*$2);
	}
|
	constant NE constant
	{
		$$=new Relational_Expr_Ast($1,$3,*$2);
	}
|
	constant GT variable
	{
		$$=new Relational_Expr_Ast($1,$3,*$2);
	}
|
	constant LT variable
	{
		$$=new Relational_Expr_Ast($1,$3,*$2);
	}
|
	constant GE variable
	{
		$$=new Relational_Expr_Ast($1,$3,*$2);
	}
|
	constant LE variable
	{
		$$=new Relational_Expr_Ast($1,$3,*$2);
	}
|
	constant EQ variable
	{
		$$=new Relational_Expr_Ast($1,$3,*$2);
	}
|
	constant NE variable
	{
		$$=new Relational_Expr_Ast($1,$3,*$2);
	}
|
	constant GT comparision_expression
	{
		$$=new Relational_Expr_Ast($1,$3,*$2);
	}
|
	constant LT comparision_expression
	{
		$$=new Relational_Expr_Ast($1,$3,*$2);
	}
|
	constant GE comparision_expression
	{
		$$=new Relational_Expr_Ast($1,$3,*$2);
	}
|
	constant LE comparision_expression
	{
		$$=new Relational_Expr_Ast($1,$3,*$2);
	}
|
	constant EQ comparision_expression
	{
		$$=new Relational_Expr_Ast($1,$3,*$2);
	}
|
	constant NE comparision_expression
	{
		$$=new Relational_Expr_Ast($1,$3,*$2);
	}
;

variable:
	NAME
	{
		#if 1
		Symbol_Table_Entry var_table_entry;

		if (current_procedure->variable_in_symbol_list_check(*$1))
			 var_table_entry = current_procedure->get_symbol_table_entry(*$1);

		else if (program_object.variable_in_symbol_list_check(*$1))
			var_table_entry = program_object.get_symbol_table_entry(*$1);

		else
		{
			int line = get_line_number();
			report_error("Variable has not been declared", line);
		}

		$$ = new Name_Ast(*$1, var_table_entry);

		delete $1;
		#endif
	}
;

constant:
	INTEGER_NUMBER
	{
		#if 1
		$$ = new Number_Ast<int>($1, int_data_type);
		#endif
	}
;
