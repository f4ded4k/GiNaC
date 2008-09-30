/// @file parser_a_b.cpp Check for some silly bugs in the parser.
#include "ginac.h"
#include <string>
#include <iostream>
#include <stdexcept>
#include <sstream>
using namespace GiNaC;

// - a - b was misparsed as -a + b due to a bug in parser::parse_unary_expr()
static int check1(std::ostream& err_str)
{
	const std::string srep("-a-b");
	parser reader;
	ex e = reader(srep);
	ex a = reader.get_syms()["a"];
	ex b = reader.get_syms()["b"];
	ex g = - a - b;
	ex d = (e - g).expand();
	if (!d.is_zero()) {
		err_str << "\"" << srep << "\" was misparsed as \""
			<< e << "\"" << std::endl;
		return 1;
	}
	return 0;
}

/// Parser was rejecting the valid expression '5 - (3*x)/10'.
static int check2(std::ostream& err_str)
{
	const std::string srep("5-(3*x)/10");
	parser reader;
	ex e = reader(srep);
	ex x = reader.get_syms()["x"];
	ex g = 5 - (3*x)/10;
	ex d = (e - g).expand();
	if (!d.is_zero()) {
		err_str << "\"" << srep << "\" was misparsed as \""
			<< e << "\"" << std::endl;
		return 1;
	}
	return 0;
}

/// parse_literal_expr forget to consume the token, so parser get
/// very confused.
static int check3(std::ostream& err_str)
{
	const std::string srep("5-(2*I)/3");
	parser reader;
	ex e = reader(srep);
	ex g = numeric(5) - (numeric(2)*I)/3;
	ex d = (e - g).expand();
	if (!d.is_zero()) {
		err_str << "\"" << srep << "\" was misparsed as \""
			<< e << "\"" << std::endl;
		return 1;
	}
	return 0;
}

/// parser happily accepted various junk like 'x^2()+1'
static int check4(std::ostream& err_str)
{
	const std::string junk("x^2()+1");
	parser reader;
	ex e;
	try {
		e = reader(junk);
		err_str << "parser accepts junk: \"" << junk << "\"" << std::endl;
		return 1;
	} catch (parse_error& err) {
		// Ok, parser rejects the nonsense.
		return 0;
	}
}

int main(int argc, char** argv)
{
	std::cout << "checking for parser bugs. " << std::flush;
	std::ostringstream err_str;
	int errors = 0;
	errors += check1(err_str);
	errors += check2(err_str);
	errors += check3(err_str);
	errors += check4(err_str);
	if (errors) {
		std::cout << "Yes, unfortunately:" << std::endl;
		std::cout << err_str.str();
	} else {
		std::cout << "Not found. ";
	}
	return errors;
}

