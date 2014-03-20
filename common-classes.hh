
/*********************************************************************************************

                                cfglp : A CFG Language Processor
                                --------------------------------

               About:
               Implemented by Uday Khedker (http://www.cse.iitb.ac.in/~uday) for 
               the courses cs302+cs306: Language Processors (theory and lab) at
               IIT Bombay. Release date Jan 6, 2013. Copyrights reserved by Uday
               Khedker. This implemenation has been made available purely for
               academic purposes without any warranty of any kind.

               Please see the README file for some details. A doxygen generated
               documentation can be found at http://www.cse.iitb.ac.in/~uday/cfglp


***********************************************************************************************/

#include <string>
#include <vector>
#include <list>
#include <map>

class Ast;

class Symbol_Table;
class Symbol_Table_Entry;

class Program;
class Procedure;
class Basic_Block;

class Eval_Result;
class Local_Environment; 

class Ics_Opd;
class Icode_Stmt;
class Code_For_Ast;
class Register_Descriptor;
class Instruction_Descriptor;
class Machine_Descriptor;
class Lra_Outcome;
