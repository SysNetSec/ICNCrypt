#include "aes.h"

#pragma GCC diagnostic ignored "-Wpointer-to-int-cast"

int encrypt(unsigned char *pt, int pt_len, unsigned char *key,
			unsigned char *iv, unsigned char *ct)
{
	int ct_len = 0;
	int len = 0;
	EVP_CIPHER_CTX *ctx;
	if (!(ctx = EVP_CIPHER_CTX_new()))
		exit(1);
	if (1 != EVP_EncryptInit_ex(ctx, EVP_aes_256_cbc(), NULL, key, iv))
		exit(1);
	if (1 != EVP_EncryptUpdate(ctx, ct, &len, pt, pt_len))
		exit(1);
	ct_len = len;
	if (1 != EVP_EncryptFinal_ex(ctx, ct + len, &len))
		exit(1);
	ct_len += len;
	return ct_len;
}

int decrypt(unsigned char *ct, int ct_len, unsigned char *key,
			unsigned char *iv, unsigned char *pt)
{
	int pt_len = 0;
	int len = 0;
	EVP_CIPHER_CTX *ctx;
	if (!(ctx = EVP_CIPHER_CTX_new()))
		exit(1);
	if (1 != EVP_DecryptInit_ex(ctx, EVP_aes_256_cbc(), NULL, key, iv))
		exit(1);
	if (1 != EVP_DecryptUpdate(ctx, pt, &len, ct, ct_len))
		exit(1);
	pt_len = len;
	if (1 != EVP_DecryptFinal_ex(ctx, pt + len, &len))
		exit(1);
	pt_len += len;
	return pt_len;
}

char **genListOfStrings(int data_size, int num_in_list)
{
	srand(time(NULL));
	char **list = malloc(num_in_list * sizeof(char *));
	for (int i = 0; i < num_in_list; i++) {
		list[i] = genRandString(data_size, 0);
		list[i][data_size - 1] = '\0';
	}
	return list;
}

char **genEncList(int data_size, int num_in_list)
{
	char **list = malloc(num_in_list * sizeof(char *));
	for (int i = 0; i < num_in_list; i++) {
		list[i] = malloc(data_size);
	}
	return list;
}

char **genDecList(int data_size, int num_in_list)
{
	char **list = malloc(num_in_list * sizeof(char *));
	for (int i = 0; i < num_in_list; i++) {
		list[i] = malloc(data_size + 1);
		list[i][data_size] = '\0';
	}
	return list;
}

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

void writeToFile(const char *file_name, const char *flags, const char *data,
				 int size)
{
	FILE *fp = fopen(file_name, flags);
	if (!fp) {
		fprintf(stderr, "[!] Error opening file '%s'\n", file_name);
		exit(0);
	}
	fwrite(data, size, 1, fp);
	fclose(fp);
}

char *readFromFile(const char *file_name, const char *flags, int size)
{
	FILE *fp = fopen(file_name, flags);
	char *data = malloc(size);
	if (!fp) {
		fprintf(stderr, "[!] Error opening file '%s'\n", file_name);
		exit(0);
	}
	fread(data, size, 1, fp);
	fclose(fp);
	return data;
}

int checkOption(const char *arg)
{
	if (strcmp(arg, "-e") == 0) {
		return 1;
	}
	else if (strcmp(arg, "-d") == 0) {
		return 2;
	}
	else if (strcmp(arg, "-a") == 0) {
		return 3;
	}
	else {
		return -1;
	}

	return -1;
}
