
struct AExpression : public type
{
	enum{ sid = 1001 };
	AExpression()
		:type(1001)
	{}
};


struct AProgramItem : public type
{
	enum{ sid = 1002 };
	AProgramItem()
		:type(1002)
	{}
};


struct AValue : public type
{
	enum{ sid = 1003 };
	AValue()
		:type(1003)
	{}
};


struct Expression : public type
{
	enum{ sid = 1004 };
	Expression()
		:type(1004)
	{}
};


struct FuncCall : public type
{
	enum{ sid = 1005 };
	FuncCall()
		:type(1005)
	{}
};


struct Function : public type
{
	enum{ sid = 1006 };
	Function()
		:type(1006)
	{}
};


struct FunctionContent : public type
{
	enum{ sid = 1007 };
	FunctionContent()
		:type(1007)
	{}
};


struct Print : public type
{
	enum{ sid = 1008 };
	Print()
		:type(1008)
	{}
};


struct Program : public type
{
	enum{ sid = 1009 };
	Program()
		:type(1009)
	{}
};


struct ReturnExp : public type
{
	enum{ sid = 1010 };
	ReturnExp()
		:type(1010)
	{}
};


struct ValueDeclear : public type
{
	enum{ sid = 1011 };
	ValueDeclear()
		:type(1011)
	{}
};


struct float : public type
{
	enum{ sid = 1012 };
	float()
		:type(1012)
	{}
};


struct int : public type
{
	enum{ sid = 1013 };
	int()
		:type(1013)
	{}
};


struct number : public type
{
	enum{ sid = 1014 };
	number()
		:type(1014)
	{}
};


struct op : public type
{
	enum{ sid = 1015 };
	op()
		:type(1015)
	{}
};


struct print : public type
{
	enum{ sid = 1016 };
	print()
		:type(1016)
	{}
};


struct return : public type
{
	enum{ sid = 1017 };
	return()
		:type(1017)
	{}
};


struct seperator0 : public type
{
	enum{ sid = 1018 };
	seperator0()
		:type(1018)
	{}
};


struct seperator1 : public type
{
	enum{ sid = 1019 };
	seperator1()
		:type(1019)
	{}
};


struct seperator10 : public type
{
	enum{ sid = 1020 };
	seperator10()
		:type(1020)
	{}
};


struct seperator2 : public type
{
	enum{ sid = 1021 };
	seperator2()
		:type(1021)
	{}
};


struct seperator3 : public type
{
	enum{ sid = 1022 };
	seperator3()
		:type(1022)
	{}
};


struct seperator4 : public type
{
	enum{ sid = 1023 };
	seperator4()
		:type(1023)
	{}
};


struct seperator5 : public type
{
	enum{ sid = 1024 };
	seperator5()
		:type(1024)
	{}
};


struct seperator6 : public type
{
	enum{ sid = 1025 };
	seperator6()
		:type(1025)
	{}
};


struct seperator7 : public type
{
	enum{ sid = 1026 };
	seperator7()
		:type(1026)
	{}
};


struct seperator8 : public type
{
	enum{ sid = 1027 };
	seperator8()
		:type(1027)
	{}
};


struct seperator9 : public type
{
	enum{ sid = 1028 };
	seperator9()
		:type(1028)
	{}
};


struct symbol : public type
{
	enum{ sid = 1029 };
	symbol()
		:type(1029)
	{}
};


struct valuetype : public type
{
	enum{ sid = 1030 };
	valuetype()
		:type(1030)
	{}
};

