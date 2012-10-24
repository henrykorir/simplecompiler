#include "cplcompiler.h"
#include "extract_stream.h"

using namespace compile;
using namespace compile::cplc;

cplcompiler::cplcompiler()
{
}

cplcompiler::~cplcompiler()
{
}

void cplcompiler::setup(loadgrammar loader)
{
	// load grammar
	if (loader != NULL)
	{
		// make new grammar and machines
		g_.reset(new compiler_grammar);
		m_.reset(new lalr1machine());
		s_.reset(new symbol_machine);
	
		loader(*g_, *m_, *s_, symconvertor_);
	}
	else fire("no grammar specialed");
}

void cplcompiler::init()
{
	m_->init();
	s_->init();
}

void cplcompiler::build(tistream& cfile)
{
	const int eof_sid = -1;
	extrace_stream es(cfile, *s_, eof_sid, g_->whitespaces(), true);
	std::allocator<doc::word> walloc;

	// read words
	for (streamword word; es>>word;)
	{
		logstring("extrace word '%s', sid is %d", word.txt.c_str(), word.sid);
		automachine::machine_meta* tmp_meta = m_->new_meta(word.sid);
		lalr1machine::lalr1meta* pm = (lalr1machine::lalr1meta*)tmp_meta;
		pm->content = new doc::word;//walloc.allocate(1);
        pm->ctype = typesystem::instance().word_type();
		as<compile::doc::word>(pm->content)->txt = word.txt;

		if (word.sid != eof_sid)
		{
			foreach (complex_symbol_convert& convertor, symconvertor_[word.sid].begin(), symconvertor_[word.sid].end())
			{
				if (convertor != NULL && convertor(tmp_meta)) break;
			}
		}

		if(!m_->eta(tmp_meta))
			fire("invalidate word '%s'!", word.txt.c_str());
	}

	if(m_->isaccepted()) logstring("accepted!");
	else logstring("error, not accepted!");
}
