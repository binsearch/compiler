float fn(int a, float b)
{
	int e = 3;
	float c = 2;

	return a + c / e * b;
}

main()
{
	int a = 2;
	float b = 2.3;

	a = a + fn(a, b) / b <= fn(3, 3.2);
}
