#include <unittest.h>

#include <iostream>
#include <fstream>

#include <compiler.h>
#include <extract.h>
#include <regex2nfa.h>
#include <nfa2dfa.h>
#include <logger.h>
#include <gio.h>
#include <dfa2machine.h>
#include <combinemachines.h>
#include <extract_stream.h>

using namespace sc;
using namespace compile;

class cpl_wordsplit_test : public sc::unittest
{
public:
	/* overwrite */ virtual void init(int argc, const char* argv[])
	{
	}

	/* overwrite */ virtual void run_test()
	{
		//compiler::instance().initialization();
		//compiler::instance().check(input);
		//check_regex2nfa();
		//check_nfa2dfa();
		//combine_check();
		check_splitwords();
	}
private:
	void check_splitwords()
	{
		//compile::tinygrammar grammars[6];
		std::vector<compile::tinygrammar*> grammars;
		std::vector<compile::automachine*> macs;
		grammars.push_back(gettinyg("[a-zA-Z_][a-zA-Z_0-9]*"));
		grammars.push_back(gettinyg("([0-9]+)|(0[xX][0-9A-Fa-f]+)"));
		grammars.push_back(gettinyg("(\\d*\\.\\d+)|(\\d+\\.)|((\\d+|(\\d*\\.\\d+)|(\\d+\\.))[Ee][+\\-]?(\\d+))"));
		grammars.push_back(gettinyg("((\\d*\\.\\d+)|(\\d+\\.)|((\\d+|(\\d*\\.\\d+)|(\\d+\\.))[Ee][+\\-]?(\\d+)))f"));
		grammars.push_back(gettinyg("\"([^\\\\\"]*(\\\\.)*)*\""));
		grammars.push_back(gettinyg("'([^\\\\']*(\\\\.)*)*'"));
		grammars.push_back(gettinyg("\\+"));
		grammars.push_back(gettinyg("\\*"));
		grammars.push_back(gettinyg("-"));
		grammars.push_back(gettinyg("/"));
		grammars.push_back(gettinyg("\\("));
		grammars.push_back(gettinyg("\\)"));
		grammars.push_back(gettinyg(","));
		grammars.push_back(gettinyg(";"));
		grammars.push_back(gettinyg("\\{"));
		grammars.push_back(gettinyg("\\}"));
		grammars.push_back(gettinyg("="));
		grammars.push_back(gettinyg("\\."));
		grammars.push_back(gettinyg("\\+\\+"));
		grammars.push_back(gettinyg("<"));
		grammars.push_back(gettinyg(">"));
		grammars.push_back(gettinyg("!="));
		grammars.push_back(gettinyg("::"));
		grammars.push_back(gettinyg(":"));
		grammars.push_back(gettinyg("<<"));
		grammars.push_back(gettinyg(">>"));
		grammars.push_back(gettinyg("#"));
		grammars.push_back(gettinyg("\\["));
		grammars.push_back(gettinyg("\\]"));
		grammars.push_back(gettinyg("&"));
		grammars.push_back(gettinyg("&&"));
		grammars.push_back(gettinyg("!"));
		grammars.push_back(gettinyg("#include\\s*<[^>]*>"));
		//grammars.push_back(gettinyg("new"));

		grammars.push_back(gettinyg("//(.*\\\\s*[\r\n]+)*(.*)")); // signal line comment
		grammars.push_back(gettinyg("/\\*([^*]|[\r\n]|(\\*[^/]))*\\*/")); // multiline comments

		macs.resize(grammars.size());
		for (size_t i = 0; i < grammars.size(); ++ i)
		{
			macs[i] = new compile::automachine;
			compile::ga::dfa2machine d2m(*grammars[i], *macs[i]);
			d2m.invoke();
		}

		compile::symbol_machine mainmac;
		compile::ga::combine_machines cm(&macs[0], macs.size(), mainmac);
		cm.invoke();
		
		std::ifstream ifs("../../cprogramlanguage/words-split-test.cpp");
		//std::ifstream ifs("../../test/callstacktest.cpp");
		//std::ifstream ifs("../../test/input/tinyg.x");
		if (!ifs.is_open()) fire("open file failed!");
		extrace_stream es(ifs, mainmac, -1, " \t\r\n");
		
		streamword aword;
		while(es>>aword)
		{
			logstring("read word[%d]: %s", aword.sid, aword.txt.c_str());
		}
		if (es.eof()) logstring("done read all words");
		else logstring("some error occure!");
		ifs.close();
	}
private:
	void combine_check()
	{
		compile::tinygrammar grammars[6];
		gettinyg("[a-zA-Z_][a-zA-Z_0-9]*", grammars[0]);
		gettinyg("([0-9]+)|(0[xX][0-9A-Fa-f]+)", grammars[1]);
		gettinyg("(\\d*\\.\\d+)|(\\d+\\.)|((\\d+|(\\d*\\.\\d+)|(\\d+\\.))[Ee][+\\-]?(\\d+))", grammars[2]);
		gettinyg("((\\d*\\.\\d+)|(\\d+\\.)|((\\d+|(\\d*\\.\\d+)|(\\d+\\.))[Ee][+\\-]?(\\d+)))f", grammars[3]);
		gettinyg("\"([^\\\\\"]*(\\\\.)*)*\"", grammars[4]);
		gettinyg("'([^\\\\']*(\\\\.)*)*'", grammars[5]);

		compile::automachine* macs[6];
		for (int i = 0; i < 6; ++ i)
		{
			macs[i] = new compile::automachine;
			compile::ga::dfa2machine d2m(grammars[i], *macs[i]);
			d2m.invoke();
		}

		compile::symbol_machine mainmac;
		compile::ga::combine_machines cm(macs, 6, mainmac);
		cm.invoke();

		std::cout<<"start to check strings"<<std::endl;

		//compile::symbol_machine tmpm;
		//compile::tinygrammar tmpg;
		//gettinyg("[a-zA-Z_][a-zA-Z_0-9]*", tmpg);
		//compile::ga::dfa2machine d2m(tmpg, tmpm);
		//d2m.invoke();
		//check_mac(tmpm, "abcd");
		//check_mac(tmpm, "10.5");

		check_mac(mainmac, "abcd");
		check_mac(mainmac, "10.5");
		check_mac(mainmac, ".53");
	}

	void check_mac(compile::symbol_machine mac, const std::string& input)
	{
		logstring("check string %s", input.c_str());
		std::cout<<"check string "<<input<<std::endl;
		mac.init();
		//foreach (char c, input.begin(), input.end())
		for (size_t i = 0; i < input.size(); ++ i)
		{
			char c = input[i];
			logstring("eat meta %c", c);
			compile::automachine::machine_meta meta(c);
			if(!mac.eta(&meta))
			{
				check_status(mac.get_cursid());
				return ;
			}
		}
		logstring("@ end of the input string");
		std::cout<<"@ end of the input string"<<std::endl;
		check_status(mac.get_cursid());
	}

	void check_status(int32 code)
	{
		if (code == -1)
		{
			logstring("invalidate input string!");
		}
		else
		{
			logstring("machine %d accepted!", code);
		}
	}

	void check_regex2nfa()
	{
		// check regex2nfa first
		check("abcd");
		check("a|bfd");
		check("axy|bfd");
		check("a(za)d");
		check("(a(b)c)");
		check("(a|b)fd");
		check("(a|b)fd+");
		check("(a|b)+fd+");
		check("(a|b)+f|d+");
		check("(a|b)+[abc]?fd+");
		check("[a-dx0-4](a|b)+[abc]?fd+");
		check("[a-dx\\[\\]0-4](a|b)+[abc]?fd+");
		check("\"([^\\\\\"]*(\\\\.)*)*\"");
		check("'([^\\\\\"]*(\\\\.)*)*'");
		check("(\\d*\\.\\d+)|(\\d+\\.)|((\\d+|(\\d*\\.\\d+)|(\\d+\\.))[Ee][+\\-]?(\\d+))");
		check("([0-9]+)|(0[xX][0-9A-Fa-f]+)");
		check("[a-zA-Z_][a-zA-Z_0-9]*");
	}

	void check_nfa2dfa()
	{
		check2("[\\w\\d]+");
		check2("[^\\w]+");
		check2("abcd");
		check2("a+b");
		check2("a?b");
		check2("a*b");
		check2("a(b|cd)x");
		check2("a[def](b|cd)x");
		check2("a([def]m(n(b|cd)x))");
		check2("[a-zA-Z_][a-zA-Z_0-9]*");
		check2("([0-9]+)|(0[xX][0-9A-Fa-f]+)");
		check2("(\\d*\\.\\d+)|(\\d+\\.)|((\\d+|(\\d*\\.\\d+)|(\\d+\\.))[Ee][+\\-]?(\\d+))");
		check2("\"([^\\\\\"]*(\\\\.)*)*\"");
		check2("'([^\\\\']*(\\\\.)*)*'");
	}

	void check(const tstring& regex_str)
	{
		logstring("start check %s", regex_str.c_str());
		compile::tinygrammar mout;
		compile::ga::regex2nfa r2n(regex_str, mout, false);
		r2n.invoke();
	}

	void check2(const tstring& regex_str)
	{
		compile::tinygrammar mout;
		compile::ga::regex2nfa r2n(regex_str, mout, false);
		compile::ga::nfa2dfa n2d(mout, mout);
		r2n.invoke();
		gwriter gw(kog::loggermanager::instance().get_logger().getos());
		gw<<mout;
		
		n2d.invoke();
		gw<<mout;
	}

	compile::tinygrammar* gettinyg(const tstring& regex_str)
	{
		compile::tinygrammar* pg = new compile::tinygrammar;
		gettinyg(regex_str, *pg);
		return pg;
	}

	void gettinyg(const tstring& regex_str, compile::tinygrammar& g)
	{
		compile::tinygrammar mout;
		compile::ga::regex2nfa r2n(regex_str, mout, false);
		compile::ga::nfa2dfa n2d(mout, mout);
		r2n.invoke();
		n2d.invoke();

		gwriter gw(kog::loggermanager::instance().get_logger().getos());
		gw<<mout;

		g.swap(mout);
	}
private:
	std::string input;
    std::string asmfile;
};

NEW_UNITTEST(cpl_wordsplit_test);
