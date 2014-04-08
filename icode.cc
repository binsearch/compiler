
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
#include <iomanip>
#include <iostream>
#include <sstream>
#include <string>
#include <vector>
#include <list>
#include <map>

using namespace std;

#include "common-classes.hh"
#include "error-display.hh"
#include "local-environment.hh"
#include "icode.hh"
#include "reg-alloc.hh"
#include "symbol-table.hh"
#include "ast.hh"
#include "program.hh"

/****************************** Class Ics_Opd *****************************/

Opd_Category Ics_Opd::get_opd_category() 
{ 
	return type;
} 

Register_Descriptor * Ics_Opd::get_reg()
{
	CHECK_INVARIANT(CONTROL_SHOULD_NOT_REACH, 
		"The get_Reg method should not be called for a non-reg operand");
}

Symbol_Table_Entry & Ics_Opd::get_symbol_entry() 
{
	CHECK_INVARIANT(CONTROL_SHOULD_NOT_REACH, 
		"The get_Sym_Entry method should not be called for a non-address operand");
}

/****************************** Class Mem_Addr_Opd *****************************/

Mem_Addr_Opd::Mem_Addr_Opd(Symbol_Table_Entry & se) 
{
	symbol_entry = &se;
	type = memory_addr;
}

Mem_Addr_Opd & Mem_Addr_Opd::operator=(const Mem_Addr_Opd & rhs)
{
	type = rhs.type;
	symbol_entry = rhs.symbol_entry;

	return *this;
}

void Mem_Addr_Opd::print_ics_opd(ostream & file_buffer) 
{
	string name = symbol_entry->get_variable_name();

	file_buffer << name;
}

void Mem_Addr_Opd::print_asm_opd(ostream & file_buffer) 
{
	Table_Scope symbol_scope = symbol_entry->get_symbol_scope();

	CHECK_INVARIANT(((symbol_scope == local) || (symbol_scope == global)), 
			"Wrong scope value");

	if (symbol_scope == local)
	{
		int offset = symbol_entry->get_start_offset();
		file_buffer << offset << "($fp)";
	}
	else
		file_buffer << symbol_entry->get_variable_name();
}

/****************************** Class Register_Addr_Opd *****************************/

Register_Addr_Opd::Register_Addr_Opd(Register_Descriptor * reg) 
{
	type = register_addr;
	register_description = reg;
}

Register_Descriptor * Register_Addr_Opd::get_reg()    { return register_description; }

Register_Addr_Opd& Register_Addr_Opd::operator=(const Register_Addr_Opd& rhs)
{
	type = rhs.type;     
	register_description = rhs.register_description ;

	return *this;
}

void Register_Addr_Opd::print_ics_opd(ostream & file_buffer) 
{
	CHECK_INVARIANT((register_description != NULL), "Register cannot be null");

	string name = register_description->get_name();

	file_buffer << name;
}

void Register_Addr_Opd::print_asm_opd(ostream & file_buffer) 
{
	CHECK_INVARIANT((register_description != NULL), "Register cannot be null");

	string name = register_description->get_name();

	file_buffer << "$" << name;
}

/****************************** Class Const_Opd *****************************/

template <class DATA_TYPE>
Const_Opd<DATA_TYPE>::Const_Opd(DATA_TYPE n) 
{
	type = constant_addr;
	num = n;
}

template <class DATA_TYPE>
Const_Opd<DATA_TYPE> & Const_Opd<DATA_TYPE>::operator=(const Const_Opd<DATA_TYPE> & rhs)
{
	type = rhs.type;
	num = rhs.num;

	return *this;
}

template <class DATA_TYPE>
void Const_Opd<DATA_TYPE>::print_ics_opd(ostream & file_buffer) 
{
	file_buffer << fixed << setprecision(2);
	file_buffer << num;
}

template <class DATA_TYPE>
void Const_Opd<DATA_TYPE>::print_asm_opd(ostream & file_buffer) 
{
	file_buffer << fixed << setprecision(2);

	file_buffer << num;
}

/****************************** Class Icode_Stmt *****************************/

Instruction_Descriptor & Icode_Stmt::get_op()
{ 
	return op_desc; 
}

Ics_Opd * Icode_Stmt::get_opd1()
{
	CHECK_INVARIANT(CONTROL_SHOULD_NOT_REACH, "virtual method get_Opd1 not implemented");
}

Ics_Opd * Icode_Stmt::get_opd2()
{
	CHECK_INVARIANT(CONTROL_SHOULD_NOT_REACH, "virtual method get_Opd2 not implemented");
}

Ics_Opd * Icode_Stmt::get_result()
{
	CHECK_INVARIANT(CONTROL_SHOULD_NOT_REACH, "virtual method get_Result not implemented");
}

void Icode_Stmt::set_opd1(Ics_Opd * ics_opd)
{
	CHECK_INVARIANT(CONTROL_SHOULD_NOT_REACH, "virtual method set_Opd1 not implemented");
}

void Icode_Stmt::set_opd2(Ics_Opd * ics_opd)
{
	CHECK_INVARIANT(CONTROL_SHOULD_NOT_REACH, "virtual method set_Opd2 not implemented");
}

void Icode_Stmt::set_result(Ics_Opd * ics_opd)
{
	CHECK_INVARIANT(CONTROL_SHOULD_NOT_REACH, "virtual methos set_Result not implemented");
}

/*************************** Class Move_IC_Stmt *****************************/

Move_IC_Stmt::Move_IC_Stmt(Tgt_Op op, Ics_Opd * o1, Ics_Opd * res)
{
	CHECK_INVARIANT((machine_dscr_object.spim_instruction_table[op] != NULL),
			"Instruction description in spim table cannot be null");

	op_desc = *(machine_dscr_object.spim_instruction_table[op]);
	opd1 = o1;   
	result = res; 
}

Ics_Opd * Move_IC_Stmt::get_opd1()          { return opd1; }
Ics_Opd * Move_IC_Stmt::get_result()        { return result; }

void Move_IC_Stmt::set_opd1(Ics_Opd * io)   { opd1 = io; }
void Move_IC_Stmt::set_result(Ics_Opd * io) { result = io; }

Move_IC_Stmt& Move_IC_Stmt::operator=(const Move_IC_Stmt& rhs)
{
	op_desc = rhs.op_desc;
	opd1 = rhs.opd1;
	result = rhs.result; 

	return *this;
}

void Move_IC_Stmt::print_icode(ostream & file_buffer)
{
	cout << "in print move \n";
	CHECK_INVARIANT (opd1, "Opd1 cannot be NULL for a move IC Stmt");
	CHECK_INVARIANT (result, "Result cannot be NULL for a move IC Stmt");

	string operation_name = op_desc.get_name();

	Icode_Format ic_format = op_desc.get_ic_format();

	switch (ic_format)
	{
	case i_r_op_o1: 
			file_buffer << " " << operation_name;
			cout << "before move ic get reg" << endl;
			if(opd1->get_opd_category() == register_addr) {
		 		if((opd1->get_reg())->get_value_type() == float_num){
		 			file_buffer << ".d";
		 		}
		 	}

			if(result->get_opd_category() == register_addr) {
		 		if((result->get_reg())->get_value_type() == float_num){
		 			file_buffer << ".d";
		 		}
		 	}
		 	cout << "after move ic get reg" << endl;
			file_buffer << ":\t";
			result->print_ics_opd(file_buffer);
			file_buffer << " <- ";
			opd1->print_ics_opd(file_buffer);
			file_buffer << "\n";

			break; 

	default: CHECK_INVARIANT(CONTROL_SHOULD_NOT_REACH, 
				"Intermediate code format not supported");
		break;
	}
	cout << "out of print move" << endl;
}

void Move_IC_Stmt::print_assembly(ostream & file_buffer)
{
	CHECK_INVARIANT (opd1, "Opd1 cannot be NULL for a move IC Stmt");
	CHECK_INVARIANT (result, "Result cannot be NULL for a move IC Stmt");
	string op_name = op_desc.get_mnemonic();

	Assembly_Format assem_format = op_desc.get_assembly_format();
	switch (assem_format)
	{
	case a_op_r_o1: 
			file_buffer << "\t" << op_name << " ";
			result->print_asm_opd(file_buffer);
			file_buffer << ", ";
			opd1->print_asm_opd(file_buffer);
			file_buffer << "\n";

			break; 

	case a_op_o1_r: 
			file_buffer << "\t" << op_name << " ";
			opd1->print_asm_opd(file_buffer);
			file_buffer << ", ";
			result->print_asm_opd(file_buffer);
			file_buffer << "\n";

			break; 

	default: CHECK_INVARIANT(CONTROL_SHOULD_NOT_REACH, "Intermediate code format not supported");
		break;
	}
}

/******************************* Class Comp_IC_Stmt *************/
Comp_IC_Stmt::Comp_IC_Stmt(Tgt_Op inst_op, Ics_Opd * temp_lhs, Ics_Opd * temp_rhs, Ics_Opd * temp_result){

	CHECK_INVARIANT((machine_dscr_object.spim_instruction_table[inst_op] != NULL),
			"Instruction description in spim table cannot be null");
	op_desc = *(machine_dscr_object.spim_instruction_table[inst_op]);

	lhs = temp_lhs;
	rhs = temp_rhs;
	result = temp_result;

}

void Comp_IC_Stmt::print_icode(ostream & file_buffer)
{
	cout << "in comp print ic \n";
	CHECK_INVARIANT ((lhs != NULL), "lhs cannot be NULL for a Comparision IC Stmt");
	// CHECK_INVARIANT ((rhs != NULL), "rhs cannot be NULL for a Comparision IC Stmt");
	CHECK_INVARIANT((result != NULL), "result cannot be NULL for a Comparision IC Stmt");
	string operation_name = op_desc.get_name();

	Icode_Format ic_format = op_desc.get_ic_format();

	switch (ic_format)
	{
	case i_r_op_o1: 
			file_buffer << " " << operation_name;
			// if(lhs->get_opd_category() == register_addr) {
			// 	if((lhs->get_reg())->get_value_type() == float_num){
			// 		file_buffer << ".d";
			// 	}
			// }
			// else if(rhs != NULL){
			// 	if(rhs->get_opd_category() == register_addr){
			// 		if((rhs->get_reg())->get_value_type() == float_num){
			// 			file_buffer << ".d";
			// 		}
			// 	}
			// }

			// cout << "comp ic stmt get reg" << endl;
			if((result->get_opd_category() == register_addr) && ((result->get_reg())->get_value_type() == float_num)){  
				file_buffer << ".d";
			}
			// cout << "after comp ic stmt" << endl;
			file_buffer << ": ";
			result->print_ics_opd(file_buffer);
			file_buffer << " <- ";
			lhs->print_ics_opd(file_buffer);
			if(operation_name != "uminus"){
				file_buffer << " , ";
				rhs->print_ics_opd(file_buffer);
			}
			file_buffer << "\n";

			break; 

	case i_op_o1:
			file_buffer << " " << operation_name;
			file_buffer << ": ";
			result->print_ics_opd(file_buffer);
			file_buffer << " <- ";
			lhs->print_ics_opd(file_buffer);
			file_buffer << "\n";

			break;		
	default: CHECK_INVARIANT(CONTROL_SHOULD_NOT_REACH, 
				"Intermediate code format not supported");
		break;
	}
	cout << "after comp print ic\n"; 
}

void Comp_IC_Stmt::print_assembly(ostream & file_buffer){
	CHECK_INVARIANT ((lhs != NULL), "lhs cannot be NULL for a Comparision IC Stmt");
	// CHECK_INVARIANT ((rhs != NULL), "rhs cannot be NULL for a Comparision IC Stmt");
	CHECK_INVARIANT((result != NULL), "result cannot be NULL for a Comparision IC Stmt");

	// string op_name = op_desc.get_mnemonic();

	// Assembly_Format assem_format = op_desc.get_assembly_format();
	// switch (assem_format)
	// {
	// case a_op_r_o1: 
	// 		file_buffer << "\t" << op_name << " ";
	// 		result->print_asm_opd(file_buffer);
	// 		file_buffer << ", ";
	// 		opd1->print_asm_opd(file_buffer);
	// 		file_buffer << "\n";

	// 		break; 

	// case a_op_o1_r: 
	// 		file_buffer << "\t" << op_name << " ";
	// 		opd1->print_asm_opd(file_buffer);
	// 		file_buffer << ", ";
	// 		result->print_asm_opd(file_buffer);
	// 		file_buffer << "\n";

	// 		break; 

	// default: CHECK_INVARIANT(CONTROL_SHOULD_NOT_REACH, "Intermediate code format not supported");
	// 	break;
	// }

	// string op_name = op_desc.get_mnemonic();

	// Assembly_Format assem_format = op_desc.get_assembly_format();

	// switch (assem_format)
	// {

	// case a_op_r_o1: 
	// 	file_buffer << "\t" << op_name << " ";
	// 	result->print_asm_opd(file_buffer);
	// 	file_buffer << ", ";
	// 	lhs->print_asm_opd(file_buffer);
	// 	file_buffer << ", ";
	// 	rhs->print_asm_opd(file_buffer);
	// 	file_buffer << "\n";

	// 	break; 

	// // case i_r_op_o1: 
	// // 		file_buffer << " " << operation_name << ": ";
	// // 		result->print_ics_opd(file_buffer);
	// // 		file_buffer << " <- ";
	// // 		lhs->print_ics_opd(file_buffer);
	// // 		file_buffer << " , ";
	// // 		rhs->print_ics_opd(file_buffer);
	// // 		file_buffer << "\n";

	// // 		break; 
	// default: CHECK_INVARIANT(CONTROL_SHOULD_NOT_REACH, "Intermediate code format not supported");

	// break;
	// }

}

/******************************* Class Cflow_IC_Stmt ************/
Cflow_IC_Stmt::Cflow_IC_Stmt(Tgt_Op inst_op, Ics_Opd * temp_cond, int temp_true, int temp_false){
	CHECK_INVARIANT((machine_dscr_object.spim_instruction_table[inst_op] != NULL),
			"Instruction description in spim table cannot be null");
	op_desc = *(machine_dscr_object.spim_instruction_table[inst_op]);
	cond = temp_cond;
	trueid = temp_true;
	falseid = temp_false;
}

Cflow_IC_Stmt::Cflow_IC_Stmt(Tgt_Op inst_op, int temp_true){
	CHECK_INVARIANT((machine_dscr_object.spim_instruction_table[inst_op] != NULL),
			"Instruction description in spim table cannot be null");
	op_desc = *(machine_dscr_object.spim_instruction_table[inst_op]);
	trueid = temp_true;
}


void Cflow_IC_Stmt::print_icode(ostream & file_buffer){
	cout << "in cflow icode" << endl;
	string operation_name = op_desc.get_name();

	Icode_Format ic_format = op_desc.get_ic_format();

	switch (ic_format)
	{
	case i_r_op_o1: 
			file_buffer << " " << operation_name << ": ";
			cond->print_ics_opd(file_buffer);
			file_buffer << " , zero : goto label" << trueid << endl;
			file_buffer << " goto label" << falseid << endl;
			break; 

	case i_op_o1:
			file_buffer << " goto label" << trueid << endl;
			break;

	default: CHECK_INVARIANT(CONTROL_SHOULD_NOT_REACH, 
				"Intermediate code format not supported");
		break;
	}
	cout << "out of cflow icode " << endl;
}

void Cflow_IC_Stmt::print_assembly(ostream & file_buffer){

	// string op_name = op_desc.get_mnemonic();

	// Assembly_Format assem_format = op_desc.get_assembly_format();

	// switch (assem_format)
	// {

	// case a_op_r_o1: 
	// 	file_buffer << "\t" << op_name << " ";
	// 	result->print_asm_opd(file_buffer);
	// 	file_buffer << ", ";
	// 	lhs->print_asm_opd(file_buffer);
	// 	file_buffer << ", ";
	// 	rhs->print_asm_opd(file_buffer);
	// 	file_buffer << "\n";

	// 	break; 

	string op_name = op_desc.get_mnemonic();

	Assembly_Format assem_format = op_desc.get_assembly_format();	

	switch (assem_format)
	{
	case a_op_r_o1: 
			file_buffer << "\t" << op_name << " ";
			cond->print_asm_opd(file_buffer);
			file_buffer << ", $zero, label" << trueid << endl;
			file_buffer << " j label" << falseid << endl;
			break; 

	case a_op_o1:
			file_buffer << " j label" << trueid << endl;
			break;

	default: CHECK_INVARIANT(CONTROL_SHOULD_NOT_REACH, 
				"Intermediate code format not supported");
		break;
	}

}

/******************************* Class Code_For_Ast ****************************/

Code_For_Ast::Code_For_Ast()
{
	ics_list = *new list<Icode_Stmt *>;
}

Code_For_Ast::Code_For_Ast(list<Icode_Stmt *> & ic_l, Register_Descriptor * reg)
{
	ics_list = ic_l;
	result_register = reg;
}

void Code_For_Ast::append_ics(Icode_Stmt & ic_stmt)
{
	ics_list.push_back(&ic_stmt);
	Ics_Opd * result = ic_stmt.get_result();
	result_register = result->get_reg();
}

list<Icode_Stmt *> & Code_For_Ast::get_icode_list()  
{ 
	return ics_list; 
}

Register_Descriptor * Code_For_Ast::get_reg()
{
	return result_register;
}

Code_For_Ast& Code_For_Ast::operator=(const Code_For_Ast& rhs)
{
	ics_list = rhs.ics_list;
	result_register = rhs.result_register;

	return *this;
}

/************************ class Instruction_Descriptor ********************************/

Tgt_Op Instruction_Descriptor::get_op()                   	{ return inst_op; }
string Instruction_Descriptor::get_name()                       { return name; }
string Instruction_Descriptor::get_mnemonic()                   { return mnemonic; }
string Instruction_Descriptor::get_ic_symbol()                  { return ic_symbol; }
Icode_Format Instruction_Descriptor::get_ic_format()            { return ic_format; }
Assembly_Format Instruction_Descriptor::get_assembly_format()   { return assem_format; }

Instruction_Descriptor::Instruction_Descriptor (Tgt_Op op, string temp_name, string temp_mnemonic, 
						string ic_sym, Icode_Format ic_form, Assembly_Format as_form)
{
	inst_op = op;
	name = temp_name; 
	mnemonic = temp_mnemonic;
	ic_symbol = ic_sym;
	ic_format = ic_form;
	assem_format = as_form;
}

Instruction_Descriptor::Instruction_Descriptor()
{
	inst_op = nop;
	name = "";
	mnemonic = "";
	ic_symbol = "";
	ic_format = i_nsy;
	assem_format = a_nsy;
}

template class Const_Opd<int>;
template class Const_Opd<float>;
