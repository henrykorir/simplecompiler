struct Test
{
};

struct Test2
{
	int a;
};

struct Test3
{
	int a : 3;
};

struct Test4
{
	int a[20];
};

struct Test5
{
	int a[20];
}x;

struct Test6
{
	int a[20];
}*y, z[10];

struct Test3
{
	struct inner1
	{
		int b;
	};

	struct inner2
	{
		int c;
	}x;
};
