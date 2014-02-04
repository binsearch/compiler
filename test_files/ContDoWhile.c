main()
{
	int i = 0, num;

	do
	{
		if (i == 5)
		{
			i = 10;
			continue;
		}
		
		num = i;
		i = 5;
	}while(i > 9);
}
