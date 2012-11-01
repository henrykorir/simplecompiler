
int add(int a, int b)
{
	return a + b;
}

float fadd(float a, float b)
{
	return a + b;
}

int main(int argc, char* argv[])
{
	printf("easy test!\n");
	int t;
	int a = 4;
	int b = 5;
	int c = add(a, b);
	printf("%d + %d = %d\n", a, b, c);

	float x = 5.5;
	float y = .6;
	float z = fadd(x, y);
	printf("%f + %f = %f\n", x, y, z);
	return 0;
}
