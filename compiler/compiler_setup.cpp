#include "compiler_setup.h"
#include "extract_stream.h"
#include "type.h"

using namespace compile;

compiler_setup::compiler_setup()
{
}

compiler_setup::~compiler_setup()
{
}

void compiler_setup::setup()
{
	// make new grammar and machines
	g_.reset(new compiler_grammar);
	symconvertor_.reset(0);

	{ // load grammar
		init_grammar(*g_);
		init_whitespaces(g_->skipspace(), g_->whitespaces());
		logstring("init grammar done.");
	}
	{
		std::map<std::string, doc::machine> machines_tmp;
		init_machines(machines_tmp);
		//s.swap(*dynamic_cast<symbol_machine*>(machines["symbol_machine"].mac.get()));
		machines_.swap(machines_tmp);
		logstring("init machines done.");
	}

	{
		init_complex_symbols(symconvertor_);
		logstring("init complex symbols done.");
	}
}