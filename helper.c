#include "helper.h"

char *genRandString(int num, int flag)
{
	char *randString = malloc(num + 1);
	if (!randString) {
		fprintf(stderr, "Error with randStrig\n");
		exit(0);
	}
	char char_set[] = "abcdefghijklmonpqrstuvwxyz";

	FILE *fp = fopen("/dev/urandom", "r");
	char x[4];
	fread(x, 4, 1, fp);
	fclose(fp);
	if (flag == 1)
		srand(time(NULL));

	for (int i = 0; i < num; i++) {
		size_t index = (double)rand() / RAND_MAX * (sizeof char_set - 1);
		randString[i] = char_set[index];
	}
	randString[num] = '\0';
	return randString;
}
