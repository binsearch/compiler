
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

%filenames="scanner"
%lex-source="scanner.cc"

%%

int		{
			store_token_name("INTEGER");
			return Parser::INTEGER; 
		}

return		{ 
			store_token_name("RETURN");
			return Parser::RETURN; 
		}

if		{
			store_token_name("IF");
			return Parser::IF; 
		}

else	{
			store_token_name("ELSE");
			return Parser::ELSE; 
		}

goto	{
			store_token_name("GOTO");
			return Parser::GOTO; 
		}
"==" {
	store_token_name("EQ");
	ParserBase::STYPE__ * val = getSval();
	val->string_value = new std::string("EQ");

	return Parser::EQ;
}

"!=" {
	store_token_name("NE");
	ParserBase::STYPE__ * val = getSval();
	val->string_value = new std::string("NE");

	return Parser::NE;
}

[=]		{
			store_token_name("ASSIGN_OP");
			return Parser::ASSIGN_OP;	
		}

[>] {
	store_token_name("GT");
	ParserBase::STYPE__ * val = getSval();
	val->string_value = new::string("GT");

	return Parser::GT;
}

[<] {
	store_token_name("LT");
	ParserBase::STYPE__ * val = getSval();
	val->string_value = new std::string("LT");
	return Parser::LT;
}

">=" {
	store_token_name("GE");
	ParserBase::STYPE__ * val = getSval();
	val->string_value = new::string("GE");
	return Parser::GE;
}

"<=" {
	store_token_name("LE");
	ParserBase::STYPE__ * val = getSval();
	val->string_value = new std::string("LE");
	return Parser::LE;
}



"<bb "[[:digit:]]+">" {
	store_token_name("BASIC BLOCK");
	
	ParserBase::STYPE__ * val = getSval();
	string s = matched();
	
	s = s.substr(4, s.size()-5);
	val->integer_value = atoi(s.c_str());
	return Parser::BASIC_BLOCK;
}


[<>:{}();!]	{
			store_token_name("META CHAR");
			return matched()[0];
		}

[-]?[[:digit:]]+ 	{ 
				store_token_name("NUM");

				ParserBase::STYPE__ * val = getSval();
				val->integer_value = atoi(matched().c_str());

				return Parser::INTEGER_NUMBER; 
			}

[[:alpha:]][[:alpha:][:digit:]]* {
					store_token_name("NAME");

					ParserBase::STYPE__ * val = getSval();
					val->string_value = new std::string(matched());

					return Parser::NAME; 
				}

\n		{ 
			if (command_options.is_show_tokens_selected())
				ignore_token();
		}    

";;".*  	|
[ \t]		{
			if (command_options.is_show_tokens_selected())
				ignore_token();
		}

.		{ 
			string error_message;
			error_message =  "Illegal character `" + matched();
			error_message += "' on line " + lineNr();
			
			int line_number = lineNr();
			report_error(error_message, line_number);
		}
