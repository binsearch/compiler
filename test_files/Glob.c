int a;
float b;
int c;
float d;
float e;
main()
{
	a = c = 2;
	b = d = e = 3.4;

	int f = 3;
	float t = 10;

	a = t * f + e / c;
	t = a * e / t / f;
}
