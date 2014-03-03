float add(float,float);
float sub(float,float);
float mul(float,float);
float div(float,float);

main()
{
	float a=2,b=3,c,d,e,f;
	c=add(a,b);
	d=sub(a,b);
	e=mul(a,b);
	f=div(a,b);
	
}

float add (float a1,float b1)
{
	return (a1+b1);
}

float sub (float a1,float b1)
{
	return (a1-b1);
}

float mul (float a1,float b1)
{
	return (a1*b1);
}

float div (float a1,float b1)
{
	return (a1/b1);
}
