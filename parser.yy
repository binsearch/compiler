
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
	float float_value;
	Data_Type data_type;
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
%token <float_value> FLOAT_NUMBER
%token <string_value> NAME
%token RETURN
%token INTEGER
%token FLOAT 
%token DOUBLE
%token VOID
%token IF
%token ELSE
%token GOTO
%token ASSIGN_OP
%left <string_value> NE EQ
%left <string_value> LT LE GT GE
%left '+' '-'
%left '*' '/'


%type <symbol_table> declaration_statement_list
%type <symbol_table> argument_list
%type <symbol_entry> declaration_statement
%type <symbol_entry> argument
%type <basic_block_list> basic_block_list
%type <basic_block> basic_block
%type <ast_list> executable_statement_list
%type <ast_list> assignment_statement_list
%type <ast_list> pass_variable_list
%type <ast> assignment_statement
%type <ast> comparision_expression
%type <ast> goto_statement
%type <ast> if_block
%type <ast> variable
%type <ast> constant
%type <ast> arith_expression
%type <ast> func_call
%type <ast> atomic_expr
%type <ast> return_value
%type <data_type> type_declaration

%start program

%%

program:
	declaration_statement_list
	procedure_decls
	{
		#if 1
		program_object.set_global_table(*$1);
		#endif
	}
	procedure_list
|
	procedure_decls
	procedure_list
;

procedure_list:

	procedure_name
	{
		#if 1
		return_statement_used_flag = false;		// No return statement in the current procedure till now
		#endif
	}
	procedure_body
	{	
		#if 1

		#endif
	}
 	procedure_list
|

;

procedure_decls:
	type_declaration NAME '(' argument_list ')'  ';' 
	{	
		#if 1
		current_procedure = new Procedure($1, *$2);
		current_procedure->set_argument_list(*$4);
		program_object.set_procedure_map(*current_procedure);
		delete $4;
		#endif
	}
	procedure_decls
|
	type_declaration NAME '(' ')' ';' 	
	{
		#if 1
		current_procedure = new Procedure($1, *$2);
		Symbol_Table temp_list;
		current_procedure->set_argument_list(temp_list);
		program_object.set_procedure_map(*current_procedure);
		#endif
	}
	procedure_decls
|
;



procedure_name:
	NAME '(' argument_list ')'
	{	
		printf("yoyo\n");
		#if 1
		current_procedure = program_object.get_procedure(*$1);
		#endif
	}
|
	NAME '(' ')'
	{
		#if 1
		printf("yo\n");
		current_procedure = program_object.get_procedure(*$1);
		printf("dude\n");	
		#endif
	}
;

argument_list:
	argument_list ',' argument
	{
		#if 1
		// if declaration is local then no need to check in global list
		// if declaration is global then this list is global list

		int line = get_line_number();
		program_object.variable_in_proc_map_check($3->get_variable_name(), line);

		string arg_name = $3->get_variable_name();
		if (current_procedure && current_procedure->get_proc_name() == arg_name)
		{
			int line = get_line_number();
			report_error("Argument name cannot be same as procedure name", line);
		}

		if ($1 != NULL)
		{
			if($1->variable_in_symbol_list_check(arg_name))
			{
				int line = get_line_number();
				report_error("Argument name is used twice", line);
			}

			$$ = $1;
		}

		else
			$$ = new Symbol_Table();

		$$->push_symbol($3);
		#endif
	}


|
	argument
	{
		#if 1
		int line = get_line_number();
		program_object.variable_in_proc_map_check($1->get_variable_name(), line);

		string var_name = $1->get_variable_name();
		if (current_procedure && current_procedure->get_proc_name() == var_name)
		{
			int line = get_line_number();
			report_error("Argument name cannot be same as procedure name", line);
		}

		$$ = new Symbol_Table();
		$$->push_symbol($1);
		#endif
	}

;

argument:
	type_declaration NAME
	{
		#if 1
		$$ = new Symbol_Table_Entry(*$2, $1);

		delete $2;
		#endif
	}
;


procedure_body:
	'{' declaration_statement_list
	{	
		if($2==NULL)
			printf("mama\n");
		else
			printf("ouch\n");


		#if 1
		// Symbol_Table s=*$2;
		current_procedure->set_local_list(*$2);
		printf("no\n");
		delete $2;
		#endif
	}
	basic_block_list '}'
	{
		#if 1
		
		// if (return_statement_used_flag == false)
		// {
		// 	int line = get_line_number();
		// 	report_error("Atleast 1 basic block should have a return statement", line);
		// }

		current_procedure->set_basic_block_list(*$4);

		delete $4;
		#endif
	}
|
	'{' basic_block_list '}'
	{
		#if 1
		// if (return_statement_used_flag == false)
		// {
		// 	int line = get_line_number();
		// 	report_error("Atleast 1 basic block should have a return statement", line);
		// }

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
	type_declaration NAME ';'
	{
		#if 1
		$$ = new Symbol_Table_Entry(*$2, $1);

		delete $2;
		#endif
	}

;

type_declaration:
	INTEGER 
	{
		#if 1
		$$=int_data_type;
		#endif
	}
|
	FLOAT
	{
		#if 1
		$$=float_data_type;
		#endif
	}
|
	DOUBLE
	{
		#if 1
		$$=float_data_type;
		#endif
	}
|
	VOID
	{
		#if 1
		$$=void_data_type;
		#endif
	}
;

basic_block_list:
	basic_block_list basic_block
	{
		#if 1
		printf("sai\n");
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
		printf("krish\n");
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
			printf("ayya\n");
			list<Ast *> * a_li= new list<Ast *>;
			$$ = new Basic_Block($1, *a_li);
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
	assignment_statement_list RETURN return_value ';'
	{
		#if 1
		Ast * ret = new Return_Ast($3);

		return_statement_used_flag = true;					// Current procedure has an occurrence of return statement

		if ($1 != NULL)
			$$ = $1;

		else
			$$ = new list<Ast *>;

		$$->push_back(ret);
		#endif
	}
;

return_value:
	comparision_expression
	{
		#if 1
		$$ = $1;
		#endif
	}
|
	{
		#if 1
		$$ = NULL;
		#endif
	}
	
;

assignment_statement_list:
	{
		#if 1
		$$ = new list <Ast *>;
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
	variable ASSIGN_OP comparision_expression ';'
	{
		#if 1
		
		$$=new Assignment_Ast($1,$3);
		$$->check_ast(get_line_number());
		
		#endif

	}
|
	if_block
	{
		#if 1
		$$ = $1;
		#endif
	}
|
	goto_statement	
	{
		#if 1
		$$ = $1;
		#endif
	}
|
	func_call ';'
	{
		#if 1
		$$=$1;
		#endif
	}
;

/* if clause */

if_block:

	IF '(' comparision_expression ')'
	GOTO BASIC_BLOCK ';'
	ELSE
	GOTO BASIC_BLOCK ';'
	{
		#if 1
		$$ = new If_Ast($3, $6, $10);
		#endif
	}
;

goto_statement:
	
	GOTO BASIC_BLOCK ';'
	{	
		#if 1
		$$ = new Goto_Ast($2);
		#endif
	}
;

comparision_expression:
	
	arith_expression
	{
		#if 1
		$$= $1;
		#endif
	}
|	
	
	comparision_expression GT comparision_expression
	{
		#if 1
		$$=new Relational_Expr_Ast($1,$3,*$2);
		$$->check_ast(get_line_number());
		#endif
	}
|
	comparision_expression LT comparision_expression
	{
		#if 1
		$$=new Relational_Expr_Ast($1,$3,*$2);
		$$->check_ast(get_line_number());
		#endif
	}
|
	comparision_expression GE comparision_expression
	{	
		#if 1
		$$=new Relational_Expr_Ast($1,$3,*$2);
		$$->check_ast(get_line_number());
		#endif
	}
|
	comparision_expression LE comparision_expression
	{
		#if 1
		$$=new Relational_Expr_Ast($1,$3,*$2);
		$$->check_ast(get_line_number());
		#endif
	}
|
	comparision_expression EQ comparision_expression
	{
		#if 1
		$$=new Relational_Expr_Ast($1,$3,*$2);
		$$->check_ast(get_line_number());
		#endif
	}
|
	comparision_expression NE comparision_expression
	{
		#if 1
		$$=new Relational_Expr_Ast($1,$3,*$2);
		$$->check_ast(get_line_number());
		#endif
	}
;

arith_expression:
	
	'-' atomic_expr
	{
		#if 1
		$$=new Arithmetic_Expr_Ast($2,NULL,0);
		$$->check_ast(get_line_number());
		#endif
	}
|
	'-' '(' comparision_expression ')'
	{
		#if 1
		$$=new Arithmetic_Expr_Ast($3,NULL,0);
		$$->check_ast(get_line_number());
		#endif
	}
|
	'(' type_declaration ')' atomic_expr 
	{
		#if 1
		if($2==int_data_type)
			$$=new Arithmetic_Expr_Ast($4,NULL,2);
		else if($2==float_data_type)
			$$=new Arithmetic_Expr_Ast($4,NULL,1);
		else
			report_error("Typecasting can be only int or float", get_line_number());;
		
		$$->check_ast(get_line_number());

		#endif
	}
|
	'(' type_declaration ')' '(' comparision_expression ')'
	{
		#if 1
		if($2==int_data_type)
			$$=new Arithmetic_Expr_Ast($5,NULL,2);
		else if($2==float_data_type)
			$$=new Arithmetic_Expr_Ast($5,NULL,1);
		else
			report_error("Typecasting can be only int or float", get_line_number());;
		
		$$->check_ast(get_line_number());

		#endif
	}

|
	'(' comparision_expression ')'
	{
		#if 1
		$$=$2;
		#endif
	}

|
	atomic_expr
	{	
		#if 1
		$$ = $1;
		#endif
	}
|
	arith_expression '*' arith_expression
	{
		#if 1
		$$=new Arithmetic_Expr_Ast($1,$3,0);
		$$->check_ast(get_line_number());
		#endif
	}

| 
	arith_expression '/' arith_expression
	{
		#if 1
		$$=new Arithmetic_Expr_Ast($1,$3,1);
		$$->check_ast(get_line_number());
		#endif
	}

|
	arith_expression '+' arith_expression
	{
		#if 1
		$$=new Arithmetic_Expr_Ast($1,$3,2);
		$$->check_ast(get_line_number());
		#endif
	}

|
	arith_expression '-' arith_expression
	{
		#if 1
		$$=new Arithmetic_Expr_Ast($1,$3, 3);
		$$->check_ast(get_line_number());
		#endif
	}

;

atomic_expr:
	variable
	{
		#if 1
		$$=$1;
		#endif
	}
|
	constant
	{
		#if 1
		$$=$1;
		#endif
	}
|
	func_call
	{
		#if 1
		$$=$1;
		#endif
	}
;

func_call:
	NAME '(' pass_variable_list ')'
	{
		#if 1
		$$=new Function_Ast(*$1,*$3);
		$$->check_ast(get_line_number());
		#endif
	}
|
	NAME '(' ')'
	{
		#if 1
		list<Ast *> * param_list = new list<Ast*>;
		$$ = new Function_Ast(*$1,*param_list);

		$$->check_ast(get_line_number());
		#endif
	}
;

pass_variable_list:
	comparision_expression ',' pass_variable_list
	{
		#if 1
		if($$==NULL)
			$$=new list<Ast *>;

		$$->push_back($1);
		#endif
	}
|
	comparision_expression
	{
		#if 1
		$$ = new list<Ast *>;
		$$->push_back($1);
		#endif
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
|
	FLOAT_NUMBER
	{
		#if 1
		$$= new Number_Ast<float> ($1,float_data_type);
		#endif
	}
;
