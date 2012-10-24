#include "symbolmachine.h"
#include <galgorithm.h>
#include <scerror.h>

using namespace sc;
using namespace compile;
using namespace compile::ga;

symbol_machine::symbol_machine()
{
}

symbol_machine::symbol_machine(const syntax_string str_syntaxs[], size_t N)
: sstr_(str_syntaxs, str_syntaxs + N)
{
	// build all symbol machine using regex string
	kog::smart_vector<automachine*> regexmacs(N);
	for (size_t i = 0; i < regexmacs.size(); ++ i)
	{
		if (str_syntaxs[i].third) logstring("start convert string machine[%d] (%s)", i, str_syntaxs[i].first.c_str());
		else logstring("start convert regex string machine[%d] (%s)", i, str_syntaxs[i].first.c_str());

		tinygrammar nfa;
		tinygrammar dfa;
		std::auto_ptr<automachine> mac(new automachine);
		
		regex2nfa r2n(str_syntaxs[i].first, nfa, str_syntaxs[i].third);
		nfa2dfa n2d(nfa, dfa);
		dfa2machine d2m(dfa, *mac);
		r2n.invoke();
		n2d.invoke();
		d2m.invoke();
		regexmacs[i] = mac.release();
	}

	logstring("start combine machines");
	// combine machines
	combine_machines cm(regexmacs.get(), regexmacs.size(), *this);
	cm.invoke();

	logstring("reset output machine's sid");
	// reset output machine's sid
	foreach (automachine::sheetrow& row, sheet().begin(), sheet().end())
	{
		// update row's code
		if (row.code() == -1) continue;
		else if (row.code() >= 0 && row.code() < (int32)regexmacs.size()) 
			row.code(str_syntaxs[row.code()].second);
		else fire("invalidate code %d, outof range", row.code());
	}
}

int32 symbol_machine::get_cursid() const
{
	if (cstate_ < 0 || cstate_ >= (int32)sheet().size())
		return -1;
	return sheet().at(cstate_).code();
}
