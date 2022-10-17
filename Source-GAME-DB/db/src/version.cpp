#include <stdio.h>
#include <stdlib.h>

void WriteVersion()
{
#ifndef __WIN32__
	FILE* fp(fopen("version.txt", "w"));

	if (NULL != fp)
	{
		fprintf(fp, "Owsap Productions\n");
		fprintf(fp, "Game Perforce Revision: %s\n", __VERSION__);
		fclose(fp);
	}
	else
	{
		fprintf(stderr, "Cannot open version.txt\n");
		exit(0);
	}
#endif
}
