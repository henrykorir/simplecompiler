#include "statemachine.h"
#include "grammar.h"

using namespace sc;
using namespace compile;

state_machine::state_machine()
{
}

state_machine::state_machine(const shared_sheet& asheet)
: automachine(asheet)
{
}

state_machine::~state_machine()
{
}

bool state_machine::eta(machine_meta* meta)
{
	if(automachine::eta(meta))
	{
		str_ += (sc::tchar)meta->sid;
		return true;
	}
	return false;
}

void state_machine::init()
{
	automachine::init();
	str_.clear();
}

