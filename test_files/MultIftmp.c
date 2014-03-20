main()
{
	int iftmp0 = 2;
	int iftmp3 = 10;
	int iftmp10 = 11;

	iftmp0 = iftmp0?iftmp3 > 3:iftmp10 < iftmp0;
	iftmp10 = iftmp3?iftmp0 > iftmp3:iftmp10 == iftmp3;
}
