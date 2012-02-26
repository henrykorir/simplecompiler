
struct Program : public type
{
	enum{ sid = 0 };
	Program()
		:type(sid)
	{}
};


struct AProgramItem : public type
{
	enum{ sid = 1 };
	AProgramItem()
		:type(sid)
	{}
};


struct ValueDeclear : public type
{
	enum{ sid = 2 };
	ValueDeclear()
		:type(sid)
	{}
};


struct Function : public type
{
	enum{ sid = 3 };
	Function()
		:type(sid)
	{}
};


struct Expression : public type
{
	enum{ sid = 4 };
	Expression()
		:type(sid)
	{}
};


struct valuetype : public type
{
	enum{ sid = 5 };
	valuetype()
		:type(sid)
	{}
};


struct symbol : public type
{
	enum{ sid = 6 };
	symbol()
		:type(sid)
	{}
};


struct AExpression : public type
{
	enum{ sid = 7 };
	AExpression()
		:type(sid)
	{}
};


struct AValue : public type
{
	enum{ sid = 8 };
	AValue()
		:type(sid)
	{}
};


struct number : public type
{
	enum{ sid = 9 };
	number()
		:type(sid)
	{}
};


struct FuncCall : public type
{
	enum{ sid = 10 };
	FuncCall()
		:type(sid)
	{}
};


struct op : public type
{
	enum{ sid = 11 };
	op()
		:type(sid)
	{}
};


struct Print : public type
{
	enum{ sid = 12 };
	Print()
		:type(sid)
	{}
};


struct printF : public type
{
	enum{ sid = 13 };
	printF()
		:type(sid)
	{}
};


struct FunctionContent : public type
{
	enum{ sid = 14 };
	FunctionContent()
		:type(sid)
	{}
};


struct ReturnExp : public type
{
	enum{ sid = 15 };
	ReturnExp()
		:type(sid)
	{}
};


struct Return : public type
{
	enum{ sid = 16 };
	Return()
		:type(sid)
	{}
};


struct Int : public type
{
	enum{ sid = 17 };
	Int()
		:type(sid)
	{}
};


struct Float : public type
{
	enum{ sid = 18 };
	Float()
		:type(sid)
	{}
};


struct { : public type
{
	enum{ sid = 20 };
	{()
		:type(sid)
	{}
};


struct } : public type
{
	enum{ sid = 21 };
	}()
		:type(sid)
	{}
};


struct ( : public type
{
	enum{ sid = 22 };
	(()
		:type(sid)
	{}
};


struct ) : public type
{
	enum{ sid = 23 };
	)()
		:type(sid)
	{}
};


struct = : public type
{
	enum{ sid = 24 };
	=()
		:type(sid)
	{}
};


struct * : public type
{
	enum{ sid = 25 };
	*()
		:type(sid)
	{}
};


struct + : public type
{
	enum{ sid = 26 };
	+()
		:type(sid)
	{}
};


struct - : public type
{
	enum{ sid = 27 };
	-()
		:type(sid)
	{}
};


struct / : public type
{
	enum{ sid = 28 };
	/()
		:type(sid)
	{}
};


struct ; : public type
{
	enum{ sid = 29 };
	;()
		:type(sid)
	{}
};


struct , : public type
{
	enum{ sid = 30 };
	,()
		:type(sid)
	{}
};


void initmachines(compiler& c)
{
	
	{
		state_machine m;
		state_machine::sparsesheet& sheet = m.sheet();
		sheet.reset(2);
		state_machine::sheetrow::value_type* ptr = NULL;
		
		sheet[0].reset(63);
		sheet[0].type(1);
		sheet[0].endings(1);
		ptr = sheet[0].get()
			*(ptr ++) = std::make_pair(47, 0);
			*(ptr ++) = std::make_pair(48, 0);
			*(ptr ++) = std::make_pair(49, 0);
			*(ptr ++) = std::make_pair(50, 0);
			*(ptr ++) = std::make_pair(51, 0);
			*(ptr ++) = std::make_pair(52, 0);
			*(ptr ++) = std::make_pair(53, 0);
			*(ptr ++) = std::make_pair(54, 0);
			*(ptr ++) = std::make_pair(55, 0);
			*(ptr ++) = std::make_pair(56, 0);
			*(ptr ++) = std::make_pair(64, 0);
			*(ptr ++) = std::make_pair(65, 0);
			*(ptr ++) = std::make_pair(66, 0);
			*(ptr ++) = std::make_pair(67, 0);
			*(ptr ++) = std::make_pair(68, 0);
			*(ptr ++) = std::make_pair(69, 0);
			*(ptr ++) = std::make_pair(70, 0);
			*(ptr ++) = std::make_pair(71, 0);
			*(ptr ++) = std::make_pair(72, 0);
			*(ptr ++) = std::make_pair(73, 0);
			*(ptr ++) = std::make_pair(74, 0);
			*(ptr ++) = std::make_pair(75, 0);
			*(ptr ++) = std::make_pair(76, 0);
			*(ptr ++) = std::make_pair(77, 0);
			*(ptr ++) = std::make_pair(78, 0);
			*(ptr ++) = std::make_pair(79, 0);
			*(ptr ++) = std::make_pair(80, 0);
			*(ptr ++) = std::make_pair(81, 0);
			*(ptr ++) = std::make_pair(82, 0);
			*(ptr ++) = std::make_pair(83, 0);
			*(ptr ++) = std::make_pair(84, 0);
			*(ptr ++) = std::make_pair(85, 0);
			*(ptr ++) = std::make_pair(86, 0);
			*(ptr ++) = std::make_pair(87, 0);
			*(ptr ++) = std::make_pair(88, 0);
			*(ptr ++) = std::make_pair(89, 0);
			*(ptr ++) = std::make_pair(94, 0);
			*(ptr ++) = std::make_pair(96, 0);
			*(ptr ++) = std::make_pair(97, 0);
			*(ptr ++) = std::make_pair(98, 0);
			*(ptr ++) = std::make_pair(99, 0);
			*(ptr ++) = std::make_pair(100, 0);
			*(ptr ++) = std::make_pair(101, 0);
			*(ptr ++) = std::make_pair(102, 0);
			*(ptr ++) = std::make_pair(103, 0);
			*(ptr ++) = std::make_pair(104, 0);
			*(ptr ++) = std::make_pair(105, 0);
			*(ptr ++) = std::make_pair(106, 0);
			*(ptr ++) = std::make_pair(107, 0);
			*(ptr ++) = std::make_pair(108, 0);
			*(ptr ++) = std::make_pair(109, 0);
			*(ptr ++) = std::make_pair(110, 0);
			*(ptr ++) = std::make_pair(111, 0);
			*(ptr ++) = std::make_pair(112, 0);
			*(ptr ++) = std::make_pair(113, 0);
			*(ptr ++) = std::make_pair(114, 0);
			*(ptr ++) = std::make_pair(115, 0);
			*(ptr ++) = std::make_pair(116, 0);
			*(ptr ++) = std::make_pair(117, 0);
			*(ptr ++) = std::make_pair(118, 0);
			*(ptr ++) = std::make_pair(119, 0);
			*(ptr ++) = std::make_pair(120, 0);
			*(ptr ++) = std::make_pair(121, 0);
		
		sheet[1].reset(53);
		sheet[1].type(1);
		sheet[1].endings(0);
		ptr = sheet[1].get()
			*(ptr ++) = std::make_pair(64, 0);
			*(ptr ++) = std::make_pair(65, 0);
			*(ptr ++) = std::make_pair(66, 0);
			*(ptr ++) = std::make_pair(67, 0);
			*(ptr ++) = std::make_pair(68, 0);
			*(ptr ++) = std::make_pair(69, 0);
			*(ptr ++) = std::make_pair(70, 0);
			*(ptr ++) = std::make_pair(71, 0);
			*(ptr ++) = std::make_pair(72, 0);
			*(ptr ++) = std::make_pair(73, 0);
			*(ptr ++) = std::make_pair(74, 0);
			*(ptr ++) = std::make_pair(75, 0);
			*(ptr ++) = std::make_pair(76, 0);
			*(ptr ++) = std::make_pair(77, 0);
			*(ptr ++) = std::make_pair(78, 0);
			*(ptr ++) = std::make_pair(79, 0);
			*(ptr ++) = std::make_pair(80, 0);
			*(ptr ++) = std::make_pair(81, 0);
			*(ptr ++) = std::make_pair(82, 0);
			*(ptr ++) = std::make_pair(83, 0);
			*(ptr ++) = std::make_pair(84, 0);
			*(ptr ++) = std::make_pair(85, 0);
			*(ptr ++) = std::make_pair(86, 0);
			*(ptr ++) = std::make_pair(87, 0);
			*(ptr ++) = std::make_pair(88, 0);
			*(ptr ++) = std::make_pair(89, 0);
			*(ptr ++) = std::make_pair(94, 0);
			*(ptr ++) = std::make_pair(96, 0);
			*(ptr ++) = std::make_pair(97, 0);
			*(ptr ++) = std::make_pair(98, 0);
			*(ptr ++) = std::make_pair(99, 0);
			*(ptr ++) = std::make_pair(100, 0);
			*(ptr ++) = std::make_pair(101, 0);
			*(ptr ++) = std::make_pair(102, 0);
			*(ptr ++) = std::make_pair(103, 0);
			*(ptr ++) = std::make_pair(104, 0);
			*(ptr ++) = std::make_pair(105, 0);
			*(ptr ++) = std::make_pair(106, 0);
			*(ptr ++) = std::make_pair(107, 0);
			*(ptr ++) = std::make_pair(108, 0);
			*(ptr ++) = std::make_pair(109, 0);
			*(ptr ++) = std::make_pair(110, 0);
			*(ptr ++) = std::make_pair(111, 0);
			*(ptr ++) = std::make_pair(112, 0);
			*(ptr ++) = std::make_pair(113, 0);
			*(ptr ++) = std::make_pair(114, 0);
			*(ptr ++) = std::make_pair(115, 0);
			*(ptr ++) = std::make_pair(116, 0);
			*(ptr ++) = std::make_pair(117, 0);
			*(ptr ++) = std::make_pair(118, 0);
			*(ptr ++) = std::make_pair(119, 0);
			*(ptr ++) = std::make_pair(120, 0);
			*(ptr ++) = std::make_pair(121, 0);
		
		m.sstate() = 1;
		c["symbol"] = m;
	}
	
	{
		state_machine m;
		state_machine::sparsesheet& sheet = m.sheet();
		sheet.reset(4);
		state_machine::sheetrow::value_type* ptr = NULL;
		
		sheet[0].reset(10);
		sheet[0].type(1);
		sheet[0].endings(1);
		ptr = sheet[0].get()
			*(ptr ++) = std::make_pair(47, 0);
			*(ptr ++) = std::make_pair(48, 0);
			*(ptr ++) = std::make_pair(49, 0);
			*(ptr ++) = std::make_pair(50, 0);
			*(ptr ++) = std::make_pair(51, 0);
			*(ptr ++) = std::make_pair(52, 0);
			*(ptr ++) = std::make_pair(53, 0);
			*(ptr ++) = std::make_pair(54, 0);
			*(ptr ++) = std::make_pair(55, 0);
			*(ptr ++) = std::make_pair(56, 0);
		
		sheet[1].reset(10);
		sheet[1].type(1);
		sheet[1].endings(0);
		ptr = sheet[1].get()
			*(ptr ++) = std::make_pair(47, 0);
			*(ptr ++) = std::make_pair(48, 0);
			*(ptr ++) = std::make_pair(49, 0);
			*(ptr ++) = std::make_pair(50, 0);
			*(ptr ++) = std::make_pair(51, 0);
			*(ptr ++) = std::make_pair(52, 0);
			*(ptr ++) = std::make_pair(53, 0);
			*(ptr ++) = std::make_pair(54, 0);
			*(ptr ++) = std::make_pair(55, 0);
			*(ptr ++) = std::make_pair(56, 0);
		
		sheet[2].reset(11);
		sheet[2].type(1);
		sheet[2].endings(1);
		ptr = sheet[2].get()
			*(ptr ++) = std::make_pair(45, 0);
			*(ptr ++) = std::make_pair(47, 2);
			*(ptr ++) = std::make_pair(48, 2);
			*(ptr ++) = std::make_pair(49, 2);
			*(ptr ++) = std::make_pair(50, 2);
			*(ptr ++) = std::make_pair(51, 2);
			*(ptr ++) = std::make_pair(52, 2);
			*(ptr ++) = std::make_pair(53, 2);
			*(ptr ++) = std::make_pair(54, 2);
			*(ptr ++) = std::make_pair(55, 2);
			*(ptr ++) = std::make_pair(56, 2);
		
		sheet[3].reset(11);
		sheet[3].type(1);
		sheet[3].endings(0);
		ptr = sheet[3].get()
			*(ptr ++) = std::make_pair(45, 1);
			*(ptr ++) = std::make_pair(47, 2);
			*(ptr ++) = std::make_pair(48, 2);
			*(ptr ++) = std::make_pair(49, 2);
			*(ptr ++) = std::make_pair(50, 2);
			*(ptr ++) = std::make_pair(51, 2);
			*(ptr ++) = std::make_pair(52, 2);
			*(ptr ++) = std::make_pair(53, 2);
			*(ptr ++) = std::make_pair(54, 2);
			*(ptr ++) = std::make_pair(55, 2);
			*(ptr ++) = std::make_pair(56, 2);
		
		m.sstate() = 3;
		c["number"] = m;
	}
	
}
