
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

int v1 = 4;
int v2 = 5;
int c = v1 * v2;
print(c);
print(f(v1, v2));
float t;
t = test(v1, v2);
print(t);