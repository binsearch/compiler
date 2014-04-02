main()
{
	int a = 3;
	float b = 2.3;

	if (a == 0)
		a = b;
	else if (b > 0)
		b = a;
	else
	{
		a += b;
		b ++;
	}
}
