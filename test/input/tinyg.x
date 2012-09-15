
int f(int a, int b)
{
	int c;
	c = a + b;
	return c;
}

float g(float x, float y)
{
	float z;
	z = x - y;
	return z;
}

float test(int m, int n)
{
	float e;
	float d;
	e = 1.0;
	d = 2.0;
	d = g(e, d);
	return d;
}

int v1;
v1 = 4;
int v2;
v2 = 5;
int c;
c = v1 * v2;
print(c);
int m;
m = f(v1, v2);
print(m);
float t;
t = test(v1, v2);
print(t);
