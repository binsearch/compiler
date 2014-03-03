int prod;

fact(int a)
{
	if (a == 1)
		return 1;

	prod *= a;
	fact(a-1);
}

main()
{
	int a = 10;
	prod = 1;
	fact(a);
}
