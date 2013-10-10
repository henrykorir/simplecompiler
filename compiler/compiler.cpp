#include "compiler.h"
#include <functional>
#include <algorithm>
#include <logger.h>
#include <fstream>
#include <scerror.h>
#include <typeinfo>
#include <folder.h>
#include <typeinfoname.h>

#include "extract.h"
#include "lalr1machine.h"
#include "asmgenerate.h"
#include "compiler_setup.h"
#include "extract_stream.h"
#include "scope.h"

using namespace sc;
using namespace compile;
using namespace compile::doc;
using namespace compile::runtime;

compiler::compiler()
{
}

compiler::~compiler()
{
}

compile::interlanguage* compiler::get_iml()
{
	return iml_.get();
}

void compiler::setup(kog::shared_ptr<compile::compiler_setup> setup_obj)
{
	setup_ = setup_obj;
	setup_->setup();
}

void compiler::init(const tstring& src_file, const tstring& obj_folder)
{
	std::string folder, filename;
	std::string title, ext;
	if (!kog::filesystem::split_path(src_file, folder, filename) || filename.empty())
	{
		fire("invalidate input source file %s", src_file.c_str());
	}

	if (!kog::filesystem::split_title(filename, title, ext) || ext.empty() || title.empty())
	{
		fire("invalidate input source file %s", src_file.c_str());
	}

	_src_file = src_file;
	if (obj_folder.empty())
	{
		_il_file = kog::filesystem::path_cat(folder, title + ".il");
		_asm_file = kog::filesystem::path_cat(folder, title + ".asm");
	}
	else
	{
		_il_file = kog::filesystem::path_cat(obj_folder, title + ".il");
		_asm_file = kog::filesystem::path_cat(obj_folder, title + ".asm");
	}

	iml_.reset(new compile::interlanguage);

	symbol_machine* symbol_mac = (symbol_machine*)(setup_->machines_["symbol_machine"].mac.get());
	lalr1machine* syntax_mac = (lalr1machine*)(setup_->machines_["syntax_machine"].mac.get());

	if (symbol_mac == NULL)
	{
		fire("not load symbol machine");
	}
	
	if (syntax_mac == NULL)
	{
		fire("not load syntax machine");
	}

	syntax_mac->init();
	symbol_mac->init();

	compile::runtime::global_scope = iml_->current_scope();

	typesystem::instance().init_buildin_types();
}

void compiler::build_src2iml()
{
	const int eof_sid = -1;
	const int space_sid = -2;
	symbol_machine* symbol_mac = (symbol_machine*)(setup_->machines_["symbol_machine"].mac.get());
	lalr1machine* syntax_mac = (lalr1machine*)(setup_->machines_["syntax_machine"].mac.get());
	compiler_grammar* g = setup_->g_.get();
	kog::smart_vector<compiler_setup::veccsconver>& symconvertor = setup_->symconvertor_;
	std::allocator<doc::word> walloc;
	symholder_proxy symproxy = g->symbols();

	std::ifstream ifs(_src_file.c_str());
	if (!ifs.is_open()) fire("can't open file(%s)", _src_file.c_str());
	extrace_stream es(ifs, *symbol_mac, eof_sid, /*g->whitespaces()*/"\r\n\t\v\f ", g->skipspace());
	//logstring("whitespaces is (%s)", g->whitespaces().c_str());

	// read words
	for (streamword word; es>>word;)
	{
		logstring("extrace word '%s', sid is %d, name(%s)", word.txt.c_str(), word.sid, 
			word.sid >= 0 ? symproxy.at(word.sid).name : "$eof");
		automachine::machine_meta* tmp_meta = syntax_mac->new_meta(word.sid);
		lalr1machine::lalr1meta* pm = (lalr1machine::lalr1meta*)tmp_meta;
		pm->content = new doc::word;//walloc.allocate(1);
        //pm->ctype = wtype;
		as<compile::doc::word>(pm->content)->txt = word.txt;

		if (word.sid >= 0)
		{
			foreach (compiler_setup::complex_symbol_convert& convertor, symconvertor[word.sid].begin(), symconvertor[word.sid].end())
			{
				if (convertor != NULL && convertor(tmp_meta)) 
				{
					logstring("convert sid from (%d) to (%d), result symbol is(%s)", 
						word.sid, tmp_meta->sid, symproxy.at(tmp_meta->sid).name);
					break;
				}
			}
		}
		else if (word.sid == space_sid)
		{
		}

		try {
			if (!syntax_mac->eta(tmp_meta))
				fire("invalidate word '%s'!", word.txt.c_str());
		} catch(sc::scerror& sce) {
			fire("%s(%d) : error %s: %s\ncall stack:\n%s", 
					_src_file.c_str(), es.get_line_count(), kog::typeinfo_name(typeid(sce).name()).c_str(), sce.what(), sce.trace_message().c_str());
		} catch (std::exception& ex) {
			fire("%s(%d) : error %s: %s", _src_file.c_str(), es.get_line_count(), kog::typeinfo_name(typeid(ex).name()).c_str(), ex.what());
		} catch (...) {
			fire("%s(%d) : error %s: %s", _src_file.c_str(), es.get_line_count(), "unknown", "unknown error");
		}
	}

	if(syntax_mac->isaccepted()) logstring("accepted!");
	else logstring("error, not accepted!");

	ifs.close();

	// print il file
	logstring("write il file(%s)", _il_file.c_str());
	iml_->print_il(_il_file);
}

void compiler::build_iml2asm()
{
	
}
