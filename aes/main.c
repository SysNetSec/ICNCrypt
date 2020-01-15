/* IMPORTANT: must run encrypt then exact same decryption right after
 */

#include "aes.h"

#define PT_SIZE atoi(argv[2])			// get the plaintext size
#define CT_SIZE (PT_SIZE / 16 + 1) * 16 // calc the ciphertext size
#define LIST_SIZE atoi(argv[3])			// get the list size if used

void error(const char *msg);
int main(int argc, char *argv[])
{
	struct timeval startENC, endENC, startDEC, endDEC;
	long useconds_encoding = 0, useconds_decoding = 0;
	int opts = 0;

	if (argc < 3)
		error("usage: ./aes {-e/-d} (plaintext_size)\n");

	opts = checkOption(argv[1]);
	if (opts < 0)
		error("Error with option: need -e OR -d as first arg!\n");

	char **list;
	char **enc_list;
	char **dec_list;
	char *pt = genRandString(PT_SIZE, 1);
	char *ct = malloc(CT_SIZE);

	unsigned char *key = (unsigned char *)"11345611245111111118898111878111";
	unsigned char *iv = (unsigned char *)"1111643645117111";
	int len = 0;

	switch (opts) {
	case 1:
		// fprintf(stdout,"[before] %s\n",pt);
		gettimeofday(&startENC, NULL);
		len = encrypt((unsigned char *)pt, PT_SIZE, key, iv, ct);
		gettimeofday(&endENC, NULL);
		writeToFile("test.out", "wb", ct, CT_SIZE);
		useconds_encoding = endENC.tv_usec - startENC.tv_usec;
		fprintf(stdout, "%ld\n", useconds_encoding);
		break;
	case 2:
		ct = readFromFile("test.out", "rb", CT_SIZE);
		gettimeofday(&startDEC, NULL);
		len = decrypt((unsigned char *)ct, CT_SIZE, key, iv, pt);
		gettimeofday(&endDEC, NULL);
		useconds_decoding = endDEC.tv_usec - startDEC.tv_usec;
		fprintf(stdout, "%ld\n", useconds_decoding);
		// fprintf(stdout,"[after] %s\n",pt);
		break;
	case 3:
		if (argc < 4) {
			fprintf(stderr, "[!] Need list size!\n");
			exit(1);
		}
		/* generate the lists */
		list = genListOfStrings(PT_SIZE, LIST_SIZE);
		enc_list = genEncList(CT_SIZE, LIST_SIZE);
		dec_list = genDecList(PT_SIZE, LIST_SIZE);
		for (int i = 0; i < LIST_SIZE; i++) {
			// fprintf(stdout,"[list before] %s\n",list[i]);

			gettimeofday(&startENC, NULL);
			len = encrypt((unsigned char *)list[i], PT_SIZE, key, iv,
						  enc_list[i]);
			gettimeofday(&endENC, NULL);

			gettimeofday(&startDEC, NULL);
			len = decrypt((unsigned char *)enc_list[i], CT_SIZE, key, iv,
						  dec_list[i]);
			gettimeofday(&endDEC, NULL);

			// fprintf(stdout," [list after] %s\n",dec_list[i]);

			useconds_encoding = endENC.tv_usec - startENC.tv_usec;
			useconds_decoding = endDEC.tv_usec - startDEC.tv_usec;

			fprintf(stdout, "%ld\n", useconds_encoding);
			fprintf(stderr, "%ld\n", useconds_decoding);
		} // end loop

		break;
	default:
		fprintf(stdout, "Error with options");
		break;
	}
}

void error(const char *msg)
{
	fprintf(stderr, msg);
	exit(0);
}
