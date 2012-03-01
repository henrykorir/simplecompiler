
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
	int c;
	c = f(m, n);
	float d;
	d = m;
	float e;
	e = c;
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
