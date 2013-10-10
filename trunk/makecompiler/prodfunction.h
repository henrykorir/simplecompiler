// prodfunction.h : parse production function content
//

#ifndef _PROD_FUNCTION_H_SC_
#define _PROD_FUNCTION_H_SC_

#include "compilersyntax.h"

NAMESPACE_BEGIN(compile);
NAMESPACE_BEGIN(syntax);

class compiler_action_parser
{
public:
	compiler_action_parser(funcinfo_t& otput);
public:
	bool newline(const tstring& line);
	bool operator()(const tstring& line);
private:
	bool _is_declear(const tstring& line, std::string& var, std::string& type) const;
	bool _is_exp(const tstring& line, std::vector<tstring>& content);
private:
	funcinfo_t* otput_;
	bool get_left_start_;
};

NAMESPACE_END(syntax);
NAMESPACE_END(compile);

#endif
