
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


struct Expressions : public type
{
	enum{ sid = 4 };
	Expressions()
		:type(sid)
	{}
};


struct Valuetype : public type
{
	enum{ sid = 5 };
	Valuetype()
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


struct Op : public type
{
	enum{ sid = 11 };
	Op()
		:type(sid)
	{}
};


struct PrintFunc : public type
{
	enum{ sid = 12 };
	PrintFunc()
		:type(sid)
	{}
};


struct print : public type
{
	enum{ sid = 13 };
	print()
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


struct return : public type
{
	enum{ sid = 16 };
	return()
		:type(sid)
	{}
};


struct int : public type
{
	enum{ sid = 17 };
	int()
		:type(sid)
	{}
};


struct float : public type
{
	enum{ sid = 18 };
	float()
		:type(sid)
	{}
};


struct Assignment : public type
{
	enum{ sid = 20 };
	Assignment()
		:type(sid)
	{}
};


struct { : public type
{
	enum{ sid = 21 };
	{()
		:type(sid)
	{}
};


struct } : public type
{
	enum{ sid = 22 };
	}()
		:type(sid)
	{}
};


struct ( : public type
{
	enum{ sid = 23 };
	(()
		:type(sid)
	{}
};


struct ) : public type
{
	enum{ sid = 24 };
	)()
		:type(sid)
	{}
};


struct = : public type
{
	enum{ sid = 25 };
	=()
		:type(sid)
	{}
};


struct * : public type
{
	enum{ sid = 26 };
	*()
		:type(sid)
	{}
};


struct + : public type
{
	enum{ sid = 27 };
	+()
		:type(sid)
	{}
};


struct - : public type
{
	enum{ sid = 28 };
	-()
		:type(sid)
	{}
};


struct / : public type
{
	enum{ sid = 29 };
	/()
		:type(sid)
	{}
};


struct ; : public type
{
	enum{ sid = 30 };
	;()
		:type(sid)
	{}
};


struct , : public type
{
	enum{ sid = 31 };
	,()
		:type(sid)
	{}
};


void initmachines(compiler& c)
{
	
	{
		automachine m;
		automachine::sparsesheet& sheet = m.sheet();
		sheet.reset(2);
		automachine::sheetrow::value_type* ptr = NULL;
		
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
			*(ptr ++) = std::make_pair(103