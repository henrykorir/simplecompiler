// interlanguage.h: interlanguage 
//

#ifndef _INTERMEDIATE_LANGUAGE_H_SC_
#define _INTERMEDIATE_LANGUAGE_H_SC_

#include <map>
#include <list>
#include <singleton.h>
#include <arrayX.h>
#include <shared_ptr.h>
#include <buckethash.h>
#include <tree.h>
#include <basic_types.h>
#include "automachine.h"
#include "statemachine.h"
#include "lalr1machine.h"
#include "scope.h"

//class compiler;
NAMESPACE_BEGIN(compile)

class interlanguage
{
	friend struct split_separators;
    friend class ::compiler;
public:
	typedef compile::automachine automachine;
	typedef compile::state_machine state_machine;
	typedef compile::lalr1machine lalr1machine;
public:
	interlanguage();
    virtual ~interlanguage();
public:
    // create a new scope as sub scope of current scope
    runtime::scope* new_scope();
    // into a special scope
    runtime::scope* into_scope(runtime::scope* s);
    // outof current scope and make parent as current scope
    runtime::scope* otof_scope();
    // get current scope
    runtime::scope* current_scope() const;
public:
	void generate(const std::string& finput_name, const std::string& fotput_name);
public:
    //kog::iterator_wrap_t<std::deque<kog::shared_ptr<runtime::scope> >::iterator > 
private:
    kog::shared_ptr<runtime::scope> main_scope_;
    std::deque<kog::shared_ptr<runtime::scope> > all_scopes_;
    runtime::scope* current_scope_;
};

NAMESPACE_END(compile)

#endif
