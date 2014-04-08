
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

#include<iostream>
#include<fstream>
#include<typeinfo>

using namespace std;

#include"common-classes.hh"
#include"error-display.hh"
#include"user-options.hh"
#include"local-environment.hh"
#include"icode.hh"
#include"reg-alloc.hh"
#include"symbol-table.hh"
#include"ast.hh"
#include"basic-block.hh"
#include"procedure.hh"
#include"program.hh"

Ast::Ast()
{}

Ast::~Ast()
{}

bool Ast::check_ast()
{
	stringstream msg;
	msg << "No check_ast() function for " << typeid(*this).name();
	CHECK_INVARIANT(CONTROL_SHOULD_NOT_REACH, msg.str());
}

Data_Type Ast::get_data_type()
{
	stringstream msg;
	msg << "No get_data_type() function for " << typeid(*this).name();
	CHECK_INVARIANT(CONTROL_SHOULD_NOT_REACH, msg.str());
}

Symbol_Table_Entry & Ast::get_symbol_entry()
{
	stringstream msg;
	msg << "No get_symbol_entry() function for " << typeid(*this).name();
	CHECK_INVARIANT(CONTROL_SHOULD_NOT_REACH, msg.str());
}

void Ast::print_value(Local_Environment & eval_env, ostream & file_buffer)
{
	stringstream msg;
	msg << "No print_value() function for " << typeid(*this).name();
	CHECK_INVARIANT(CONTROL_SHOULD_NOT_REACH, msg.str());
}

Eval_Result & Ast::get_value_of_evaluation(Local_Environment & eval_env)
{
	stringstream msg;
	msg << "No get_value_of_evaluation() function for " << typeid(*this).name();
	CHECK_INVARIANT(CONTROL_SHOULD_NOT_REACH, msg.str());
}

void Ast::set_value_of_evaluation(Local_Environment & eval_env, Eval_Result & result)
{
	stringstream msg;
	msg << "No set_value_of_evaluation() function for " << typeid(*this).name();
	CHECK_INVARIANT(CONTROL_SHOULD_NOT_REACH, msg.str());
}

Code_For_Ast & Ast::create_store_stmt(Register_Descriptor * store_register)
{
	stringstream msg;
	msg << "No create_store_stmt() function for " << typeid(*this).name();
	CHECK_INVARIANT(CONTROL_SHOULD_NOT_REACH, msg.str());
}

////////////////////////////////////////////////////////////////

Assignment_Ast::Assignment_Ast(Ast * temp_lhs, Ast * temp_rhs, int line)
{
	lhs = temp_lhs;
	rhs = temp_rhs;

	ast_num_child = binary_arity;
	node_data_type = void_data_type;

	lineno = line;
}

Assignment_Ast::~Assignment_Ast()
{
	delete lhs;
	delete rhs;
}

bool Assignment_Ast::check_ast()
{
	CHECK_INVARIANT((rhs != NULL), "Rhs of Assignment_Ast cannot be null");
	CHECK_INVARIANT((lhs != NULL), "Lhs of Assignment_Ast cannot be null");

	if (lhs->get_data_type() == rhs->get_data_type())
	{
		node_data_type = lhs->get_data_type();
		return true;
	}

	CHECK_INVARIANT(CONTROL_SHOULD_NOT_REACH, 
		"Assignment statement data type not compatible");
}

void Assignment_Ast::print(ostream & file_buffer)
{
	file_buffer << "\n" << AST_SPACE << "Asgn:";

	file_buffer << "\n" << AST_NODE_SPACE << "LHS (";
	lhs->print(file_buffer);
	file_buffer << ")";

	file_buffer << "\n" << AST_NODE_SPACE << "RHS (";
	rhs->print(file_buffer);
	file_buffer << ")";
}

Eval_Result & Assignment_Ast::evaluate(Local_Environment & eval_env, ostream & file_buffer)
{
	CHECK_INVARIANT((rhs != NULL), "Rhs of Assignment_Ast cannot be null");
	Eval_Result & result = rhs->evaluate(eval_env, file_buffer);

	CHECK_INPUT_AND_ABORT(result.is_variable_defined(), "Variable should be defined to be on rhs of Assignment_Ast", lineno);

	CHECK_INVARIANT((lhs != NULL), "Lhs of Assignment_Ast cannot be null");

	lhs->set_value_of_evaluation(eval_env, result);

	// Print the result

	print(file_buffer);

	lhs->print_value(eval_env, file_buffer);

	return result;
}

Code_For_Ast & Assignment_Ast::compile()
{
	/* 
		An assignment x = y where y is a variable is 
		compiled as a combination of load and store statements:
		(load) R <- y 
		(store) x <- R
		If y is a constant, the statement is compiled as:
		(imm_Load) R <- y 
		(store) x <- R
		where imm_Load denotes the load immediate operation.
	*/

	CHECK_INVARIANT((lhs != NULL), "Lhs cannot be null");
	CHECK_INVARIANT((rhs != NULL), "Rhs cannot be null");

	Code_For_Ast & load_stmt = rhs->compile();

	Register_Descriptor * load_register = load_stmt.get_reg();

	Code_For_Ast store_stmt = lhs->create_store_stmt(load_register);

	// Store the statement in ic_list

	list<Icode_Stmt *> & ic_list = *new list<Icode_Stmt *>;

	if (load_stmt.get_icode_list().empty() == false)
		ic_list = load_stmt.get_icode_list();

	if (store_stmt.get_icode_list().empty() == false)
		ic_list.splice(ic_list.end(), store_stmt.get_icode_list());

	Code_For_Ast * assign_stmt;
	if (ic_list.empty() == false)
		assign_stmt = new Code_For_Ast(ic_list, load_register);

	return *assign_stmt;
}

Code_For_Ast & Assignment_Ast::compile_and_optimize_ast(Lra_Outcome & lra)
{
	CHECK_INVARIANT((lhs != NULL), "Lhs cannot be null");
	CHECK_INVARIANT((rhs != NULL), "Rhs cannot be null");

	// cout << "came to ast c&o\n";
	if(typeid(*rhs) == typeid(Name_Ast) || typeid(*rhs) == typeid(Number_Ast<int>) || typeid(*rhs) == typeid(Number_Ast<float>))
		lra.optimize_lra(mc_2m, lhs, rhs);
	
	Code_For_Ast load_stmt = rhs->compile_and_optimize_ast(lra);
	Register_Descriptor * result_register = load_stmt.get_reg();

	if(typeid(*rhs) == typeid(Relational_Expr_Ast) || typeid(*rhs) == typeid(Arithmetic_Expr_Ast)){
		Symbol_Table_Entry * destination_symbol_entry;
		// cout << "came here" << endl;
		destination_symbol_entry = &(lhs->get_symbol_entry());
		destination_symbol_entry->update_register(result_register);
		// cout << "crossed it" << endl;
	}



	Code_For_Ast store_stmt = lhs->create_store_stmt(result_register);

	list<Icode_Stmt *> ic_list;

	if (load_stmt.get_icode_list().empty() == false)
		ic_list = load_stmt.get_icode_list();

	if (store_stmt.get_icode_list().empty() == false)
		ic_list.splice(ic_list.end(), store_stmt.get_icode_list());

	Code_For_Ast * assign_stmt;
	if (ic_list.empty() == false)
		assign_stmt = new Code_For_Ast(ic_list, result_register);
	

	return *assign_stmt;
}

/////////////////////////////////////////////////////////////////

Name_Ast::Name_Ast(string & name, Symbol_Table_Entry & var_entry, int line)
{
	variable_symbol_entry = &var_entry;

	CHECK_INVARIANT((variable_symbol_entry->get_variable_name() == name),
		"Variable's symbol entry is not matching its name");

	ast_num_child = zero_arity;
	node_data_type = variable_symbol_entry->get_data_type();
	lineno = line;
}

Name_Ast::~Name_Ast()
{}

Data_Type Name_Ast::get_data_type()
{
	return variable_symbol_entry->get_data_type();
}

Symbol_Table_Entry & Name_Ast::get_symbol_entry()
{
	return *variable_symbol_entry;
}

void Name_Ast::print(ostream & file_buffer)
{
	file_buffer << "Name : " << variable_symbol_entry->get_variable_name();
}

void Name_Ast::print_value(Local_Environment & eval_env, ostream & file_buffer)
{
	string variable_name = variable_symbol_entry->get_variable_name();

	Eval_Result * loc_var_val = eval_env.get_variable_value(variable_name);
	Eval_Result * glob_var_val = interpreter_global_table.get_variable_value(variable_name);

	file_buffer << "\n" << AST_SPACE << variable_name << " : ";

	if (!eval_env.is_variable_defined(variable_name) && !interpreter_global_table.is_variable_defined(variable_name))
		file_buffer << "undefined";

	else if (eval_env.is_variable_defined(variable_name) && loc_var_val != NULL)
	{
		CHECK_INVARIANT(loc_var_val->get_result_enum() == int_result, "Result type can only be int");
		file_buffer << loc_var_val->get_int_value() << "\n";
	}

	else
	{
		CHECK_INVARIANT(glob_var_val->get_result_enum() == int_result, 
			"Result type can only be int and float");

		if (glob_var_val == NULL)
			file_buffer << "0\n";
		else
			file_buffer << glob_var_val->get_int_value() << "\n";
	}
	file_buffer << "\n";
}

Eval_Result & Name_Ast::get_value_of_evaluation(Local_Environment & eval_env)
{
	string variable_name = variable_symbol_entry->get_variable_name();

	if (eval_env.does_variable_exist(variable_name))
	{
		CHECK_INPUT_AND_ABORT((eval_env.is_variable_defined(variable_name) == true), 
					"Variable should be defined before its use", lineno);

		Eval_Result * result = eval_env.get_variable_value(variable_name);
		return *result;
	}

	CHECK_INPUT_AND_ABORT((interpreter_global_table.is_variable_defined(variable_name) == true), 
				"Variable should be defined before its use", lineno);

	Eval_Result * result = interpreter_global_table.get_variable_value(variable_name);
	return *result;
}

void Name_Ast::set_value_of_evaluation(Local_Environment & eval_env, Eval_Result & result)
{
	Eval_Result * i;
	string variable_name = variable_symbol_entry->get_variable_name();

	if (variable_symbol_entry->get_data_type() == int_data_type)
		i = new Eval_Result_Value_Int();
	else
		CHECK_INPUT_AND_ABORT(CONTROL_SHOULD_NOT_REACH, "Type of a name can be int/float only", lineno);

	if (result.get_result_enum() == int_result)
	 	i->set_value(result.get_int_value());
	else
		CHECK_INPUT_AND_ABORT(CONTROL_SHOULD_NOT_REACH, "Type of a name can be int/float only", lineno);

	if (eval_env.does_variable_exist(variable_name))
		eval_env.put_variable_value(*i, variable_name);
	else
		interpreter_global_table.put_variable_value(*i, variable_name);
}

Eval_Result & Name_Ast::evaluate(Local_Environment & eval_env, ostream & file_buffer)
{
	return get_value_of_evaluation(eval_env);
}

Code_For_Ast & Name_Ast::compile()
{
	Ics_Opd * opd = new Mem_Addr_Opd(*variable_symbol_entry);
	Register_Descriptor * result_register;
	if(node_data_type == float_data_type)
		result_register = machine_dscr_object.get_new_register(float_num);
	else if(node_data_type == int_data_type)
		result_register = machine_dscr_object.get_new_register(int_num);
	Ics_Opd * register_opd = new Register_Addr_Opd(result_register);


	Icode_Stmt * load_stmt = new Move_IC_Stmt(load, opd, register_opd);

	list<Icode_Stmt *> ic_list;
	ic_list.push_back(load_stmt);

	Code_For_Ast & load_code = *new Code_For_Ast(ic_list, result_register);

	return load_code;
}

Code_For_Ast & Name_Ast::create_store_stmt(Register_Descriptor * store_register)
{
	CHECK_INVARIANT((store_register != NULL), "Store register cannot be null");

	Ics_Opd * register_opd = new Register_Addr_Opd(store_register);
	Ics_Opd * opd = new Mem_Addr_Opd(*variable_symbol_entry);

	Icode_Stmt * store_stmt = new Move_IC_Stmt(store, register_opd, opd);

	if (command_options.is_do_lra_selected() == false)
		variable_symbol_entry->free_register(store_register);

	list<Icode_Stmt *> & ic_list = *new list<Icode_Stmt *>;
	ic_list.push_back(store_stmt);

	Code_For_Ast & name_code = *new Code_For_Ast(ic_list, store_register);

	return name_code;
}

Code_For_Ast & Name_Ast::compile_and_optimize_ast(Lra_Outcome & lra)
{
	list<Icode_Stmt *> & ic_list = *new list<Icode_Stmt *>;;

	bool load_needed = lra.is_load_needed();

	Register_Descriptor * result_register = lra.get_register();
	CHECK_INVARIANT((result_register != NULL), "Register cannot be null");
	Ics_Opd * register_opd = new Register_Addr_Opd(result_register);

	Icode_Stmt * load_stmt = NULL;
	if (load_needed)
	{
		Ics_Opd * opd = new Mem_Addr_Opd(*variable_symbol_entry);

		load_stmt = new Move_IC_Stmt(load, opd, register_opd);
			
		ic_list.push_back(load_stmt);
	}

	Code_For_Ast & load_code = *new Code_For_Ast(ic_list, result_register);

	return load_code;
}

///////////////////////////////////////////////////////////////////////////////

template <class DATA_TYPE>
Number_Ast<DATA_TYPE>::Number_Ast(DATA_TYPE number, Data_Type constant_data_type, int line)
{
	constant = number;
	node_data_type = constant_data_type;

	ast_num_child = zero_arity;

	lineno = line;
}

template <class DATA_TYPE>
Number_Ast<DATA_TYPE>::~Number_Ast()
{}

template <class DATA_TYPE>
Data_Type Number_Ast<DATA_TYPE>::get_data_type()
{
	return node_data_type;
}

template <class DATA_TYPE>
void Number_Ast<DATA_TYPE>::print(ostream & file_buffer)
{
	file_buffer << std::fixed;
	file_buffer << std::setprecision(2);

	file_buffer << "Num : " << constant;
}

template <class DATA_TYPE>
Eval_Result & Number_Ast<DATA_TYPE>::evaluate(Local_Environment & eval_env, ostream & file_buffer)
{
	if (node_data_type == int_data_type)
	{
		Eval_Result & result = *new Eval_Result_Value_Int();
		result.set_value(constant);

		return result;
	}
}

template <class DATA_TYPE>
Code_For_Ast & Number_Ast<DATA_TYPE>::compile()
{
	Register_Descriptor * result_register;
	if(node_data_type == float_data_type)
		result_register = machine_dscr_object.get_new_register(float_num);
	else if(node_data_type == int_data_type)
		result_register = machine_dscr_object.get_new_register(int_num);
	CHECK_INVARIANT((result_register != NULL), "Result register cannot be null");
	Ics_Opd * load_register = new Register_Addr_Opd(result_register);
	Ics_Opd * opd = new Const_Opd<DATA_TYPE>(constant);

	Icode_Stmt * load_stmt = new Move_IC_Stmt(imm_load, opd, load_register);

	list<Icode_Stmt *> & ic_list = *new list<Icode_Stmt *>;
	ic_list.push_back(load_stmt);

	Code_For_Ast & num_code = *new Code_For_Ast(ic_list, result_register);

	return num_code;
}

template <class DATA_TYPE>
Code_For_Ast & Number_Ast<DATA_TYPE>::compile_and_optimize_ast(Lra_Outcome & lra)
{
	CHECK_INVARIANT((lra.get_register() != NULL), "Register assigned through optimize_lra cannot be null");
	Ics_Opd * load_register = new Register_Addr_Opd(lra.get_register());
	Ics_Opd * opd = new Const_Opd<DATA_TYPE>(constant);

	Icode_Stmt * load_stmt = new Move_IC_Stmt(imm_load, opd, load_register);

	list<Icode_Stmt *> ic_list;
	ic_list.push_back(load_stmt);

	Code_For_Ast & num_code = *new Code_For_Ast(ic_list, lra.get_register());

	return num_code;
}

///////////////////////////////////////////////////////////////////////////////

Return_Ast::Return_Ast(int line)
{
	lineno = line;
	node_data_type = void_data_type;
	ast_num_child = zero_arity;
}

Return_Ast::~Return_Ast()
{}

void Return_Ast::print(ostream & file_buffer)
{
	file_buffer << "\n" << AST_SPACE << "Return <NOTHING>\n";
}

Eval_Result & Return_Ast::evaluate(Local_Environment & eval_env, ostream & file_buffer)
{
	Eval_Result & result = *new Eval_Result_Value_Int();
	return result;
}

Code_For_Ast & Return_Ast::compile()
{

	Code_For_Ast & ret_code = *new Code_For_Ast();
	return ret_code;
}

Code_For_Ast & Return_Ast::compile_and_optimize_ast(Lra_Outcome & lra)
{
	Code_For_Ast & ret_code = *new Code_For_Ast();
	return ret_code;
}

template class Number_Ast<int>;
template class Number_Ast<float>;

///////////////////////////////////////////////////////////////////////////////
Relational_Expr_Ast::Relational_Expr_Ast(Ast * temp_lhs, Ast * temp_rhs,string temp_op)
{
	lhs = temp_lhs;
	rhs = temp_rhs;
	op=temp_op;
}

Relational_Expr_Ast::~Relational_Expr_Ast()
{
	delete lhs;
	delete rhs;
	delete &op;
}

Data_Type Relational_Expr_Ast::get_data_type()
{
	return int_data_type;
}

void Relational_Expr_Ast::print(ostream & file_buffer)
{
	file_buffer <<"\n"<< COND_SPACE << "Condition: "<<op<<"\n";

	file_buffer << COND_NODE_SPACE<<"LHS (";
	lhs->print(file_buffer);
	file_buffer << ")\n";

	file_buffer << COND_NODE_SPACE << "RHS (";
	rhs->print(file_buffer);
	file_buffer << ")";
}

Eval_Result & Relational_Expr_Ast::evaluate(Local_Environment & eval_env, ostream & file_buffer)
{
	// Eval_Result & result1 = lhs->evaluate(eval_env, file_buffer);
	// Eval_Result & result2 = rhs->evaluate(eval_env, file_buffer);


	// if(op.compare("GT")==0) {
	// 	result.set_value(result1.get_int_value()>result2.get_int_value());
	// }
	// else if(op.compare("GE")==0) {
	// 	result.set_value(result1.get_int_value()>=result2.get_int_value());
	// }
	// else if(op.compare("EQ")==0) {
	// 	result.set_value(result1.get_int_value()==result2.get_int_value());
	// }
	// else if(op.compare("NE")==0) {
	// 	result.set_value(result1.get_int_value()!=result2.get_int_value());
	// }
	// else if(op.compare("LT")==0) {
	// 	result.set_value(result1.get_int_value()<result2.get_int_value());
	// }
	// else if(op.compare("LE")==0) {
	// 	result.set_value(result1.get_int_value()<=result2.get_int_value());
	// }
	// else {
	// 	;
	// }
	// lhs->set_value_of_evaluation(eval_env, result);

	// // Print the result

	// print(file_buffer);

	// lhs->print_value(eval_env, file_buffer);

	Eval_Result & result = *new Eval_Result_Value_Int();
	return result;
}

Code_For_Ast & Relational_Expr_Ast::compile()
{
	// cout << "in rel compile" << endl;
	CHECK_INVARIANT((lhs != NULL), "Lhs cannot be null");
	CHECK_INVARIANT((rhs != NULL), "Rhs cannot be null");
	
	//code for lhs computed and result register locked. 
	Code_For_Ast & lhs_stmt = lhs->compile();
	Register_Descriptor * lhs_reg = lhs_stmt.get_reg();
	lhs_reg->set_used();

	//rhs compiled and result register locked.
	Code_For_Ast & rhs_stmt = rhs->compile();
	Register_Descriptor * rhs_reg = rhs_stmt.get_reg();
	rhs_reg->set_used();

	//Ics_opd for both sides of statement.
	Ics_Opd * rhs_opd = new Register_Addr_Opd(rhs_reg);
	Ics_Opd * lhs_opd = new Register_Addr_Opd(lhs_reg);

	//target register
	Register_Descriptor * result_register = machine_dscr_object.get_new_register(int_num);
	Ics_Opd * result_opd = new Register_Addr_Opd(result_register);	

	//creating Comp_IC_stmt for the respective comp operator.
	Icode_Stmt * comp_stmt;
	if(op == "GT"){
		comp_stmt = new Comp_IC_Stmt(sgt, lhs_opd, rhs_opd, result_opd);
	}

	if(op == "LT"){
		comp_stmt = new Comp_IC_Stmt(slt, lhs_opd, rhs_opd, result_opd);
	}

	if(op == "GE"){
		comp_stmt = new Comp_IC_Stmt(sge, lhs_opd, rhs_opd, result_opd);
	}

	if(op == "LE"){
		comp_stmt = new Comp_IC_Stmt(sle, lhs_opd, rhs_opd, result_opd);
	}

	if(op == "NE"){
		comp_stmt = new Comp_IC_Stmt(sne, lhs_opd, rhs_opd, result_opd);
	}

	if(op == "EQ"){
		comp_stmt = new Comp_IC_Stmt(seq, lhs_opd, rhs_opd, result_opd);
	}

	// Store the statement in ic_list

	list<Icode_Stmt *> & ic_list = *new list<Icode_Stmt *>;

	if (lhs_stmt.get_icode_list().empty() == false)
		ic_list = lhs_stmt.get_icode_list();

	if (rhs_stmt.get_icode_list().empty() == false)
		ic_list.splice(ic_list.end(), rhs_stmt.get_icode_list());
	ic_list.push_back(comp_stmt);

	Code_For_Ast * comp_icode;
	if (ic_list.empty() == false)
		comp_icode = new Code_For_Ast(ic_list, result_register);

	rhs_reg->reset_used();
	lhs_reg->reset_used();


	return *comp_icode;

}

Code_For_Ast & Relational_Expr_Ast::compile_and_optimize_ast(Lra_Outcome & lra)
{
	CHECK_INVARIANT((lhs != NULL), "Lhs cannot be null");
	CHECK_INVARIANT((rhs != NULL), "Rhs cannot be null");
	// cout << "in rel c&o" << endl;
	//code for lhs computed and result register locked. 
	if(typeid(*lhs) == typeid(Number_Ast<int>) || typeid(*lhs) == typeid(Number_Ast<float>) || typeid(*lhs) == typeid(Name_Ast)){
		lra.optimize_lra(mc_2r, NULL, lhs);
	}
	Code_For_Ast & lhs_stmt = lhs->compile_and_optimize_ast(lra);
	Register_Descriptor * lhs_reg = lhs_stmt.get_reg();
	lhs_reg->set_used();

	//rhs compiled and result register locked.
	if(typeid(*rhs) == typeid(Number_Ast<int>) || typeid(*rhs) == typeid(Number_Ast<float>) || typeid(*rhs) == typeid(Name_Ast)){
		lra.optimize_lra(mc_2r, NULL, rhs);
	}
	Code_For_Ast & rhs_stmt = rhs->compile_and_optimize_ast(lra);
	Register_Descriptor * rhs_reg = rhs_stmt.get_reg();
	rhs_reg->set_used();

	//Ics_opd for both sides of statement.
	Ics_Opd * rhs_opd = new Register_Addr_Opd(rhs_reg);
	Ics_Opd * lhs_opd = new Register_Addr_Opd(lhs_reg);

	//target register
	Register_Descriptor * result_register = machine_dscr_object.get_new_register(int_num);
	Ics_Opd * result_opd = new Register_Addr_Opd(result_register);	

	//creating Comp_IC_stmt for the respective comp operator.
	Icode_Stmt * comp_stmt;
	if(op == "GT"){
		comp_stmt = new Comp_IC_Stmt(sgt, lhs_opd, rhs_opd, result_opd);
	}

	if(op == "LT"){
		comp_stmt = new Comp_IC_Stmt(slt, lhs_opd, rhs_opd, result_opd);
	}

	if(op == "GE"){
		comp_stmt = new Comp_IC_Stmt(sge, lhs_opd, rhs_opd, result_opd);
	}

	if(op == "LE"){
		comp_stmt = new Comp_IC_Stmt(sle, lhs_opd, rhs_opd, result_opd);
	}

	if(op == "NE"){
		comp_stmt = new Comp_IC_Stmt(sne, lhs_opd, rhs_opd, result_opd);
	}

	if(op == "EQ"){
		comp_stmt = new Comp_IC_Stmt(seq, lhs_opd, rhs_opd, result_opd);
	}

	// Store the statement in ic_list

	list<Icode_Stmt *> & ic_list = *new list<Icode_Stmt *>;

	if (lhs_stmt.get_icode_list().empty() == false)
		ic_list = lhs_stmt.get_icode_list();

	if (rhs_stmt.get_icode_list().empty() == false)
		ic_list.splice(ic_list.end(), rhs_stmt.get_icode_list());
	ic_list.push_back(comp_stmt);

	Code_For_Ast * comp_icode;
	if (ic_list.empty() == false)
		comp_icode = new Code_For_Ast(ic_list, result_register);

	rhs_reg->reset_used();
	lhs_reg->reset_used();


	return *comp_icode;

}

///////////////////////////////////////////////////////////////////////////////

Goto_Ast::Goto_Ast(int num)
{
	bb_num=num;
}

Goto_Ast::~Goto_Ast()
{
	delete &bb_num;
}


void Goto_Ast::print(ostream & file_buffer)
{
	file_buffer<< AST_SPACE << "Goto statement:\n";

	file_buffer << AST_NODE_SPACE<<"Successor: "<<bb_num<<endl;
}


Eval_Result & Goto_Ast::evaluate(Local_Environment & eval_env, ostream & file_buffer)
{
	Eval_Result & result = *new Eval_Result_Value_Int();
	return result;

}

Code_For_Ast & Goto_Ast::compile()
{
	Register_Descriptor * comp_reg;
	Icode_Stmt * go_to_code = new Cflow_IC_Stmt(go_to,bb_num);
	list<Icode_Stmt *> & ic_list = *new list<Icode_Stmt *>;
	ic_list.push_back(go_to_code);

	Code_For_Ast * goto_icode;
	if (ic_list.empty() == false)
		goto_icode = new Code_For_Ast(ic_list, comp_reg);


	return * goto_icode;


}

Code_For_Ast & Goto_Ast::compile_and_optimize_ast(Lra_Outcome & lra)
{
	Register_Descriptor * comp_reg;
	Icode_Stmt * go_to_code = new Cflow_IC_Stmt(go_to,bb_num);
	list<Icode_Stmt *> & ic_list = *new list<Icode_Stmt *>;
	ic_list.push_back(go_to_code);

	Code_For_Ast * goto_icode;
	if (ic_list.empty() == false)
		goto_icode = new Code_For_Ast(ic_list, comp_reg);


	return * goto_icode;
}

///////////////////////////////////////////////////////////////////////////////

If_Ast::If_Ast(Ast* temp_comp,int temp_true,int temp_false)
{
	comp_exp=temp_comp;
	true_bb=temp_true;
	false_bb=temp_false;
}

If_Ast::~If_Ast()
{
	delete comp_exp;
	delete &true_bb;
	delete &false_bb;
}


void If_Ast::print(ostream & file_buffer)
{
	file_buffer<< AST_SPACE << "If_Else statement:";
	comp_exp->print(file_buffer);
	file_buffer <<endl<< AST_NODE_SPACE<<"True Successor: "<<true_bb<<endl;
	file_buffer << AST_NODE_SPACE<<"False Successor: "<<false_bb<<endl;
}

Eval_Result & If_Ast::evaluate(Local_Environment & eval_env, ostream & file_buffer)
{

	// Eval_Result & result = comp_exp->evaluate(eval_env, file_buffer);

	// int ret = result.get_value();

	// int ans;
	// print(file_buffer);
	
	// if(ret){
	// 	file_buffer<<AST_SPACE<<"Condition True : Goto (BB "<<true_bb<<")\n";
	// 	ans = true_bb;
	// }
	// else{
	// 	file_buffer<<AST_SPACE<<"Condition False : Goto (BB "<<false_bb<<")\n";
	// 	ans = false_bb;
	// }
	
	// Eval_Result & result1 = *new Eval_Result_Value_BB();
	// result1.set_value(ans);


	// Condition True : Goto (BB 3)	
	// return result1;

	// Eval_Result & result = rhs->evaluate(eval_env, file_buffer);

	// if (result.is_variable_defined() == false)
	// 	report_error("Variable should be defined to be on rhs", NOLINE);

	// lhs->set_value_of_evaluation(eval_env, result);

	// // Print the result

	// print(file_buffer);

	// lhs->print_value(eval_env, file_buffer);

	// return result;
	Eval_Result & result = *new Eval_Result_Value_Int();
	return result;

}

Code_For_Ast & If_Ast::compile()
{
	Code_For_Ast & comp_stmt = comp_exp->compile();
	Register_Descriptor * comp_reg = comp_stmt.get_reg();
	Ics_Opd * comp_opd = new Register_Addr_Opd(comp_reg);
	
	Icode_Stmt * if_stmt_code = new Cflow_IC_Stmt(if_stmt,comp_opd, true_bb, false_bb);


	list<Icode_Stmt *> & ic_list = *new list<Icode_Stmt *>;

	if (comp_stmt.get_icode_list().empty() == false)
		ic_list = comp_stmt.get_icode_list();

	ic_list.push_back(if_stmt_code);

	Code_For_Ast * if_icode;
	if (ic_list.empty() == false)
		if_icode = new Code_For_Ast(ic_list, comp_reg);


	return * if_icode;
}

Code_For_Ast & If_Ast::compile_and_optimize_ast(Lra_Outcome & lra)
{
	Code_For_Ast & comp_stmt = comp_exp->compile_and_optimize_ast(lra);
	Register_Descriptor * comp_reg = comp_stmt.get_reg();
	Ics_Opd * comp_opd = new Register_Addr_Opd(comp_reg);
	
	Icode_Stmt * if_stmt_code = new Cflow_IC_Stmt(if_stmt,comp_opd, true_bb, false_bb);


	list<Icode_Stmt *> & ic_list = *new list<Icode_Stmt *>;

	if (comp_stmt.get_icode_list().empty() == false)
		ic_list = comp_stmt.get_icode_list();

	ic_list.push_back(if_stmt_code);

	Code_For_Ast * if_icode;
	if (ic_list.empty() == false)
		if_icode = new Code_For_Ast(ic_list, comp_reg);


	return * if_icode;
}

Arithmetic_Expr_Ast::Arithmetic_Expr_Ast(Ast * temp_lhs, Ast * temp_rhs,int temp_op)
{
	lhs = temp_lhs;
	rhs = temp_rhs;
	// op='*';
	op=temp_op;
}

Arithmetic_Expr_Ast::~Arithmetic_Expr_Ast()
{
	delete lhs;
	delete rhs;
	delete &op;
}

Data_Type Arithmetic_Expr_Ast::get_data_type()
{
	return node_data_type;
}

bool Arithmetic_Expr_Ast::check_ast()
{
	if(rhs==NULL) {
		if(op==0)
			node_data_type=lhs->get_data_type();
		else if(op==1)
			node_data_type=float_data_type;
		else if(op==2)
			node_data_type=int_data_type;
		else
			;
		return true;
	}
	if (lhs->get_data_type() == rhs->get_data_type())
	{
		node_data_type = lhs->get_data_type();
		return true;
	}

	// cout << "Arithmetic Expression statement data type not compatible\n";
	return false;
}

void Arithmetic_Expr_Ast::print(ostream & file_buffer)
{
	// if(rhs==NULL) {
	// 	if(op==0) {
	// 		file_buffer <<"\n"<< ARITH_SPACE << "Arith: UMINUS\n";
	// 		file_buffer << ARITH_NODE_SPACE<<"LHS (";
	// 		lhs->print_ast(file_buffer);
	// 		file_buffer << ")";
	// 	}
	// 	else {
	// 		lhs->print_ast(file_buffer);
	// 	}
	// }
	// else {
	// 	file_buffer <<"\n"<< ARITH_SPACE << "Arith: ";

	// 	if(op==0)
	// 		file_buffer<<"MULT";
	// 	else if(op==2)
	// 		file_buffer<<"PLUS";
	// 	else if(op==3)
	// 		file_buffer<<"MINUS";
	// 	else if(op==1)
	// 		file_buffer<<"DIV";
	// 	else
	// 		;
	// 	file_buffer<<"\n";

	// 	file_buffer << ARITH_NODE_SPACE<<"LHS (";
	// 	lhs->print_ast(file_buffer);
	// 	file_buffer << ")\n";

	// 	file_buffer << ARITH_NODE_SPACE << "RHS (";
	// 	rhs->print_ast(file_buffer);
	// 	file_buffer << ")";
	// }

}

Eval_Result & Arithmetic_Expr_Ast::evaluate(Local_Environment & eval_env, ostream & file_buffer)
{
	Eval_Result & result = *new Eval_Result_Value_Int();
	return result;
	// if(rhs==NULL) {
	// 	if(op==0) {
	// 		Eval_Result & result=lhs->evaluate(eval_env,file_buffer);

	// 		if(result.get_result_enum()==int_result) {
	// 			Eval_Result & result1=*new Eval_Result_Value_Int();
	// 			result1.set_value(-result.get_value());
	// 			return result1;
	// 		}
	// 		else if(result.get_result_enum()==float_result) {
	// 			Eval_Result & result1=*new Eval_Result_Value_Float();
	// 			result1.float_set_value(-result.float_get_value());
	// 			return result1;
	// 		}
	// 		else
	// 			;
	// 		// return result;
	// 	}
	// 	if(op==1) {
	// 		Eval_Result & result=lhs->evaluate(eval_env,file_buffer);
	// 		if(result.get_result_enum()==float_result) {
	// 			Eval_Result & result1=*new Eval_Result_Value_Float();
	// 			result1.float_set_value(result.float_get_value()); 
	// 			return result1;
	// 		}
	// 		else {
	// 			Eval_Result & result1=* new Eval_Result_Value_Float();
	// 			result1.float_set_value((float)result.get_value());
	// 			return result1;
	// 		}
	// 	}
	// 	if(op==2) {
	// 		Eval_Result & result=lhs->evaluate(eval_env,file_buffer);
			
	// 		if(result.get_result_enum()==int_result) {
	// 			Eval_Result & result1=* new Eval_Result_Value_Int();
	// 			result1.set_value(result.get_value());
	// 			return result1;
	// 		}
	// 		else {
	// 			Eval_Result & result1=* new Eval_Result_Value_Int();
	// 			result1.set_value((int)result.float_get_value());
	// 			return result1;
	// 		}
	// 	}

	// }
	// else {
	// 	Eval_Result & result1 = lhs->evaluate(eval_env, file_buffer);
	// 	Eval_Result & result2 = rhs->evaluate(eval_env, file_buffer);
	// 	Eval_Result & result = *new Eval_Result_Value_Int();
		
	// // 	if(result1.get_result_enum()==int_result && result2.get_result_enum()==int_result) {
	// 		Eval_Result & result = *new Eval_Result_Value_Int();
	// 		if(op==0)
	// 			result.set_value(result1.get_value() * result2.get_value());
	// 		else if(op==1)
	// 			result.set_value(result1.get_value() / result2.get_value());
	// 		else if(op==2)
	// 			result.set_value(result1.get_value() + result2.get_value());
	// 		else if(op==3)
	// 			result.set_value(result1.get_value() - result2.get_value());
	// 		else
	// 			;
	// 		return result;
	// 	}
	// 	else if(result1.get_result_enum()==float_result && result2.get_result_enum()==float_result) {
	// 		Eval_Result & result = *new Eval_Result_Value_Float();
	// 		if(op==0)
	// 			result.float_set_value(result1.float_get_value() * result2.float_get_value());
	// 		else if(op==1)
	// 			result.float_set_value(result1.float_get_value() / result2.float_get_value());
	// 		else if(op==2)
	// 			result.float_set_value(result1.float_get_value() + result2.float_get_value());
	// 		else if(op==3)
	// 			result.float_set_value(result1.float_get_value() - result2.float_get_value());
	// 		else
	// 			;
	// 		return result;
	// 	}
	// 	else {
	// 		report_internal_error("The operands should be of the same type.");		
	// 	}
	// }


	// // Print the result

	// print_ast(file_buffer);

	// lhs->print_value(eval_env, file_buffer);

	// return result;
}

Code_For_Ast & Arithmetic_Expr_Ast::compile()
{

	// cout << "in rel compile" << endl;
	CHECK_INVARIANT((lhs != NULL), "Lhs cannot be null");
	// CHECK_INVARIANT((rhs != NULL), "Rhs cannot be null");
	
	//code for lhs computed and result register locked. 
	Code_For_Ast & lhs_stmt = lhs->compile();
	if(rhs == NULL){
		if(op == 1){
			if(lhs->get_data_type() == float_data_type || lhs->get_data_type() == double_data_type)
				return lhs_stmt;
		}
		else if(op == 2){
			if(lhs->get_data_type() == int_data_type)
				return lhs_stmt;
		}
	}

	Register_Descriptor * lhs_reg = lhs_stmt.get_reg();
	lhs_reg->set_used();

	//rhs compiled and result register locked.
	Code_For_Ast rhs_stmt;
	Ics_Opd * rhs_opd;
	Register_Descriptor * rhs_reg;
	if(rhs != NULL){
		rhs_stmt = rhs->compile();
		rhs_reg = rhs_stmt.get_reg();
		rhs_reg->set_used();
		rhs_opd = new Register_Addr_Opd(rhs_reg);
	}

	//Ics_opd for both sides of statement.
	Ics_Opd * lhs_opd = new Register_Addr_Opd(lhs_reg);

	//target register
	Register_Descriptor * result_register;
	if(node_data_type == float_data_type)
		result_register = machine_dscr_object.get_new_register(float_num);
	else if(node_data_type == int_data_type)
		result_register = machine_dscr_object.get_new_register(int_num);

	Ics_Opd * result_opd = new Register_Addr_Opd(result_register);	

	//creating Comp_IC_stmt for the respective comp operator.
	Icode_Stmt * comp_stmt;
	if(rhs != NULL){
		if(op == 0){
			comp_stmt = new Comp_IC_Stmt(mul_op, lhs_opd, rhs_opd, result_opd);
		}

		if(op == 1){
			comp_stmt = new Comp_IC_Stmt(div_op, lhs_opd, rhs_opd, result_opd);
		}

		if(op == 2){
			comp_stmt = new Comp_IC_Stmt(add_op, lhs_opd, rhs_opd, result_opd);
		}

		if(op == 3){
			comp_stmt = new Comp_IC_Stmt(sub_op, lhs_opd, rhs_opd, result_opd);
		}
	}

	else{
		if(op == 0){
			comp_stmt = new Comp_IC_Stmt(uminus_op, lhs_opd, NULL, result_opd);
		}
		else if(op == 1){
			comp_stmt = new Comp_IC_Stmt(mtc1_op, lhs_opd, NULL, result_opd);			
		}
		else if(op == 2){
			comp_stmt = new Comp_IC_Stmt(mfc1_op, lhs_opd, NULL, result_opd);			
		}

	}
	// Store the statement in ic_list

	list<Icode_Stmt *> & ic_list = *new list<Icode_Stmt *>;

	if (lhs_stmt.get_icode_list().empty() == false)
		ic_list = lhs_stmt.get_icode_list();

	if (rhs_stmt.get_icode_list().empty() == false)
		ic_list.splice(ic_list.end(), rhs_stmt.get_icode_list());
	ic_list.push_back(comp_stmt);

	Code_For_Ast * comp_icode;
	if (ic_list.empty() == false)
		comp_icode = new Code_For_Ast(ic_list, result_register);

	if(rhs != NULL) rhs_reg->reset_used();
	lhs_reg->reset_used();


	return *comp_icode;

}

Code_For_Ast & Arithmetic_Expr_Ast::compile_and_optimize_ast(Lra_Outcome & lra)
{
	// cout << "in rel compile " << endl;
	CHECK_INVARIANT((lhs != NULL), "Lhs cannot be null");
	// CHECK_INVARIANT((rhs != NULL), "Rhs cannot be null");
	
	//code for lhs computed and result register locked. 
	Code_For_Ast & lhs_stmt = *new Code_For_Ast();
	if(typeid(*lhs) == typeid(Name_Ast) || typeid(*lhs) == typeid(Number_Ast<int>) || typeid(*lhs) == typeid(Number_Ast<float>))
		lra.optimize_lra(mc_2r, NULL, lhs);

	lhs_stmt = lhs->compile_and_optimize_ast(lra);

	if(rhs == NULL){
		if(op == 1){
			if(lhs->get_data_type() == float_data_type || lhs->get_data_type() == double_data_type)
				return lhs_stmt;
		}
		if(op == 2){
			if(lhs->get_data_type() == int_data_type)
				return lhs_stmt;
		}
	}
	// cout << "before arith expr get reg\n";
	Register_Descriptor * lhs_reg = lhs_stmt.get_reg();
	lhs_reg->set_used();
	// cout << "in arith expression" << endl;
	//Ics_opd for both sides of statement.
	Ics_Opd * lhs_opd = new Register_Addr_Opd(lhs_reg);

	// cout << "after arith expr get reg \n";
	//rhs compiled and result register locked.
	Code_For_Ast rhs_stmt;
	Ics_Opd * rhs_opd;
	Register_Descriptor * rhs_reg;
	if(rhs != NULL){
		if(typeid(*rhs) == typeid(Name_Ast) || typeid(*rhs) == typeid(Number_Ast<int>) || typeid(*rhs) == typeid(Number_Ast<float>))
			lra.optimize_lra(mc_2r, NULL, rhs);

		rhs_stmt = rhs->compile_and_optimize_ast(lra);
		rhs_reg = rhs_stmt.get_reg();
		rhs_reg->set_used();
		rhs_opd = new Register_Addr_Opd(rhs_reg);
	}

	//target register
	Register_Descriptor * result_register;
	if(node_data_type == float_data_type)
		result_register = machine_dscr_object.get_new_register(float_num);
	if(node_data_type == int_data_type)
		result_register = machine_dscr_object.get_new_register(int_num);

	Ics_Opd * result_opd = new Register_Addr_Opd(result_register);	

	//creating Comp_IC_stmt for the respective comp operator.
	Icode_Stmt * comp_stmt;
	if(rhs != NULL)
	{
		if(op == 0){
			comp_stmt = new Comp_IC_Stmt(mul_op, lhs_opd, rhs_opd, result_opd);
		}

		else if(op == 1){
			comp_stmt = new Comp_IC_Stmt(div_op, lhs_opd, rhs_opd, result_opd);
		}

		else if(op == 2){
			comp_stmt = new Comp_IC_Stmt(add_op, lhs_opd, rhs_opd, result_opd);
		}

		else{
			comp_stmt = new Comp_IC_Stmt(sub_op, lhs_opd, rhs_opd, result_opd);
		}
	}

	else
	{
		if(op == 0){
			comp_stmt = new Comp_IC_Stmt(uminus_op, lhs_opd, NULL, result_opd);
		}
		else if(op == 1){
			comp_stmt = new Comp_IC_Stmt(mtc1_op, lhs_opd, NULL, result_opd);			
		}
		else{
			comp_stmt = new Comp_IC_Stmt(mfc1_op, lhs_opd, NULL, result_opd);			
		}

	}
	// Store the statement in ic_list

	list<Icode_Stmt *> & ic_list = *new list<Icode_Stmt *>;

	if (lhs_stmt.get_icode_list().empty() == false)
		ic_list = lhs_stmt.get_icode_list();

	if (rhs_stmt.get_icode_list().empty() == false)
		ic_list.splice(ic_list.end(), rhs_stmt.get_icode_list());
	ic_list.push_back(comp_stmt);

	Code_For_Ast * comp_icode;
	if (ic_list.empty() == false)
		comp_icode = new Code_For_Ast(ic_list, result_register);

	if(rhs != NULL) rhs_reg->reset_used();
	lhs_reg->reset_used();

	// cout << "out of arith expression" << endl;
	return *comp_icode;

}
