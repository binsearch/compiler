int fun()
{
	float a=2.3,b=3.2;
	a=b/2.1-a;
	return a;
}
main()
{
	int a=fun()==3?fun()-2:1;
}
