// gio.h: grammar reader and writer
//

#ifndef _GRAMMAR_IO_H_TEST_
#define _GRAMMAR_IO_H_TEST_

#include <iostream>
#include <string>
#include <deque>
#include <list>
#include <stringX.h>
#include <grammar.h>

class greader
{
public:
	greader(std::istream& is)
	: is_(is)
	{
	}

public:
	greader& operator>>(compile::grammar& g)
	{
		compile::tinygrammar tg;
		(*this)>>tg;
		g.swap_kernel(tg);
		return *this;
	}

	greader& operator>>(compile::tinygrammar& g)
	{
		std::istream& is = is_;
		while(is)
		{
			std::string line;
			std::getline(is, line);
			line = stringX::trim(line);
			if(line.empty()) continue;
			std::string::size_type pos = line.find(':');
			
			if(pos != std::string::npos)
			{
				std::string::size_type pos2 = line.find(',', pos);
				std::string p1 = stringX::trim(line.substr(0, pos));
				std::string p2;
				std::string p3;
				std::string p4;
				if(pos2 == std::string::npos) 
				{
					p2 = stringX::trim(line.substr(pos+1));
				}
				else
				{
					p2 = stringX::trim(line.substr(pos+1, pos2-pos-1));
					std::string::size_type pos3 = line.find(',', pos2 + 1);
					if(pos3 != std::string::npos)
					{
						p3 = stringX::trim(line.substr(pos2 + 1, pos3 - pos2 - 1));
						p4 = stringX::trim(line.substr(pos3 + 1));
					}
					else
					{
						p3 = stringX::trim(line.substr(pos2 + 1));
					}
				}
				if(stringX::tolower(p1) != "grammar" || p2.empty() || !/*std*/::isupper(p2[0]) || p3.size() > 1)
					throw std::runtime_error("invalidate g: " + line);
				read_grammar(is, g, p2, p3, p4);
				return *this;
			}
		}
		return *this;
	}

private:
	void read_grammar(std::istream& is, compile::tinygrammar& g, const std::string& start, const std::string& eplison, const std::string& ending) const
	{
		using namespace compile;
		std::list<std::string> bufs;
		std::deque<symbol> slist;
		std::deque<production> plist;
		int eid = -1;
		int xid = -1;
		if(!eplison.empty()) eid = finds(bufs, slist, eplison); // add eplison as first symbol: sid=0
		if(!ending.empty()) xid = finds(bufs, slist, ending);
		while(is)
		{
			std::string line;
			std::getline(is, line);
			line = stringX::trim(line);
			if(line.empty()) break;
			std::string::size_type pos = line.find("->");
			if(pos == std::string::npos) throw std::runtime_error("invalidate production: " + line);
			
			int32 L = finds(bufs, slist, stringX::trim(line.substr(0, pos)));
			std::vector<std::string> rightstr = stringX::split(line.substr(pos + 2), std::string(" "), true);
			kog::smart_vector<int32> right(rightstr.size());
			for(size_t i = 0; i < right.size(); ++ i)
			{
				right[i] = finds(bufs, slist, rightstr[i]);
			}
			plist.push_back(production(L, right.get(), right.size()));
            
            char ch;
            if(is>>ch)
            {
                is.putback(ch);
                if (ch == '{')
                {
                    std::getline(is, line);
                    plist.back().func() = line;
                }
            }
		}

		int32 ssid = finds(bufs, slist, start);
		if(!eplison.empty()) bufs.front()[0] = '\0';// set eplison symbol content to empty string
		tinygrammar tmp(slist.begin(), slist.end(), plist.begin(), plist.end(), ssid, eid, xid);
	//	tinyg.make_index();
	//	if(!ending.empty())tinyg.endings() = tinyg.index(ending);
	//	if(!eplison.empty()) tinyg.eplisons() = tinyg.index("");
		tmp.swap(g);
	}

	compile::int32 finds(std::list<std::string>& bufs, std::deque<compile::symbol>& slist, const std::string& s) const
	{
		using namespace compile;
		// find special symbol, if not found create new symbol
		for(int i = 0; i < slist.size(); ++ i)
		{
			if(slist[i].name == s) return slist[i].sid;
		}
		symbol asym;
		memset(&asym, 0, sizeof(asym));
		bufs.push_back(s);
		asym.name = &bufs.back()[0];//.c_str();
		asym.Lname = (int)s.size();
		asym.sid = (int32)slist.size();
		asym.ist = /*std::*/isupper(s[0]) ? 0 : 1;
		slist.push_back(asym);
		return asym.sid;
	}
private:
	std::istream& is_;
};

class gwriter
{
public:
	gwriter(std::ostream& os)
	: os_(os)
	{}

public:
	gwriter& operator<<(const compile::grammar& g)
	{
		const compile::tinygrammar& tg = g.gettinyg();
		return (*this)<<tg;
	}

	gwriter& operator<<(const compile::tinygrammar& tg)
	{
		using namespace compile;
		const tinygrammar::vecprods& prods = tg.productions();
		const symholder& sholder = tg.symbols();
		os_<<"grammar:"<<sholder[tg.starts()].name<<"\n";
		for(size_t i = 0; i < prods.size(); ++ i)
		{
			const production& p = prods[i];
			os_<<sholder.at(p.left()).name<<" -> ";
			os_<<sholder.at(p[0]).name;
			for(size_t  j = 1; j < p.right_size(); ++ j)
			{
				os_<<' '<<sholder.at(p[j]).name;
			}
			os_<<"\n";
		}
		return *this;
	}
private:
	std::ostream& os_;
};

#endif
