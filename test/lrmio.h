// lrmio.h: lr machine io
//

#ifndef _LRMIO_H_TEST_
#define _LRMIO_H_TEST_

#include <iostream>
#include <string>
#include <deque>
#include <list>
#include <stringX.h>
#include <lrmachine.h>

class lrmreader
{
};

class lrmwriter
{
public:
	lrmwriter(std::ostream& os)
	: os_(os)
	{}

public:
	lrmwriter& operator<<(const compile::lrmachine& m)
	{
		os_<<"lr state~:\n";
		output_state(os_, m);
		os_<<"lr table~:\n";
		output_table(os_, m);
		return *this;
	}

	std::ostream& output_state(std::ostream& os, const compile::lrmachine& m)
	{
		const compile::symholder& sholder = m.tinyg().symbols();
		const compile::tinygrammar::vecprods& pholder = m.tinyg().productions();

		const compile::automachine::sparsesheet& sheet = m.sheet();
		for (size_t i = 0; i < sheet.size(); ++ i)
		{
			os<<"\nI"<<i<<"\n";
			if(!sheet[i].more()) continue;
			const compile::lrmachine::pinfoarray& plist = *(const compile::lrmachine::pinfoarray*)sheet[i].more();
			for (size_t j = 0; j < plist.size(); ++ j)
			{
				const compile::production& p = pholder[plist[j].pid];
				const compile::int32 dot = plist[j].dot;
		
				os<<sholder[p.left()].name<<" -> ";
				
				for(size_t j = 0; j < p.right_size(); ++ j)
				{
					if(dot == j) os<<".";
					os<<sholder[p[j]].name<<" ";
				}
				if(dot == p.right_size()) os<<".";
				os<<"\n";
			}
		}
		return os<<"\n";
	}

	std::ostream& output_table(std::ostream& os, const compile::lrmachine& m)
	{
		const compile::symholder& sholder = m.tinyg().symbols();
		const compile::automachine::sparsesheet& sheet = m.sheet();
		for (size_t i = 0; i < sheet.size(); ++ i)
		{
			for (compile::automachine::sheetrow::const_iterator iter = sheet[i].begin(); iter != sheet[i].end(); ++ iter)
			{
				const char* name = iter->first < 0 ? "#" : sholder[iter->first].name;
				const char* action = iter->second < 0 ? "r" : "G";
				os<<"(I"<<i<<", "<<name<<") "<<action<<abs(iter->second)<<"\n";
			}
			os<<"\n";
		}
		return os<<"\n";
	}
private:
	std::ostream& os_;
};

#endif