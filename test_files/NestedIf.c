main()
{
	int a, b, c;
	int greatest;
	a = 3;
	b = 5;
	c = 7;

	if (a && b && c)
	{
		if (a >= b)
		{
			if (a >= c)
				greatest = a;
			else
				greatest = c;
		}
		else
		{
			if (b >= c)
				greatest = b;
			else
				greatest = c;
		}
	}

	return;
}
