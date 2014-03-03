fn(int a, float b)
{
	a = b;
	return a;
}

main()
{
	int a = 10;
	a = fn(a, 1.4);
}
