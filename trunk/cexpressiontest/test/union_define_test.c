union Test2
{
	int a;
};

union Test3
{
	int a;
	int b;
};

union Test4
{
	int a[20];
};

union Test5
{
	int a[20];
}x;

union Test6
{
	int a[20];
}*y, z[10];

union Test3
{
	union inner1
	{
		int b;
	};

	union inner2
	{
		int c;
	}x;

	struct inner3
	{
	}y;
};

union Test4
{
	struct inner3
	{
		int a;
	}x;

	struct inner3
	{
		int a;
	}y;
};