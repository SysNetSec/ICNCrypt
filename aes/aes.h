#include <openssl/aes.h>
#include <openssl/evp.h>
#include <openssl/rand.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/time.h>
#include <unistd.h>

int encrypt(unsigned char *pt, int pt_len, unsigned char *key,
			unsigned char *iv, unsigned char *ct);

int decrypt(unsigned char *ct, int ct_len, unsigned char *key,
			unsigned char *iv, unsigned char *pt);

char **genListOfStrings(int data_size, int num_in_list);

char **genEncList(int data_size, int num_in_list);

char **genDecList(int data_size, int num_in_list);

char *genRandString(int num, int flag);

void writeToFile(const char *file_name, const char *flags, const char *data,
				 int size);

char *readFromFile(const char *file_name, const char *flags, int size);

int checkOption(const char *arg);
