#include <stdio.h>
#include <string.h>
#include <sys/time.h>

#include <openssl/hmac.h>
#include <openssl/rand.h>
#include <openssl/sha.h>

#include "helper.h"
#include "hmac.h"
#include "huffman.h"

int main(int argc, char *argv[])
{
	if (argc < 2) {
		printf("Size of data?\n");
		exit(0);
	}
	uint8_t pmf[ALPHABET_SIZE];
	struct timeval start, end, start1, end1;
	struct timeval startHMAC, endHMAC;
	long useconds_encoding = 0, useconds_decoding = 0, useconds_hmac = 0;

	// used for hmac calculation to obtain hmacDigest
	unsigned char *hmacDigest = malloc(EVP_MAX_MD_SIZE);
	char hmacKey[] = "key";
	unsigned char hmacPseudonym[] = "123456789";

	BITARRAY_DECL(key, ALPHABET_SIZE - 1);

	huffman_t h;

	/* read pmf */
	FILE *fh_pmf = fopen("pmf.bin", "rb");

	if (fh_pmf == 0) {
		fprintf(stderr, "could not open pmf.bin\n");
		return 1;
	}

	if (fread(pmf, sizeof(uint8_t), ALPHABET_SIZE, fh_pmf) != ALPHABET_SIZE) {
		fprintf(stderr, "could not read full pmf from pmf.bin\n");
		return 1;
	}

	fclose(fh_pmf);

	/* read key */
	FILE *fh_key = fopen("key.bin", "rb");

	if (fh_key == 0) {
		fprintf(stderr, "could not open key.bin\n");
		return 1;
	}

	if (fread(key->data, sizeof(uint8_t),
			  BITARRAY_BITS_TO_BYTES(ALPHABET_SIZE - 1),
			  fh_key) != BITARRAY_BITS_TO_BYTES(ALPHABET_SIZE - 1)) {
		fprintf(stderr, "could not read full key from key.bin\n");
		return 1;
	}
	fclose(fh_key);
	key->size = 8 * BITARRAY_BITS_TO_BYTES(ALPHABET_SIZE - 1);

	huffman_init(&h, pmf);
	huffman_setkey(&h, key);

	// test encode

#define BYTES_TO_SYMBOLS(x) ((x * 8 + SYMBOL_BITS - 1) / SYMBOL_BITS)
#define PADDED_BYTES(x) (((BYTES_TO_SYMBOLS(x) * SYMBOL_BITS) + 7) / 8)

#define PT_SIZE atoi(argv[1])
#define HASH_ALGO EVP_md4()

	char *plaintext_str = genRandString(PADDED_BYTES(PT_SIZE), 1);
	// fprintf(stdout,"[      ] %s\n",plaintext_str);
	BITARRAY_DECL(plaintext, PADDED_BYTES(PT_SIZE) * 8);
	memcpy(plaintext->data, plaintext_str, PADDED_BYTES(PT_SIZE) + 1);
	plaintext->size = BYTES_TO_SYMBOLS(PT_SIZE) * SYMBOL_BITS;
	// fprintf(stderr,"[before] %s\n",plaintext->data);

	/* longest possible code * size of plaintext, just large enough to fit the
	 * ct */
	BITARRAY_DECL(ciphertext, 128 * PT_SIZE);

	/* the context can be reused, so dont measure this timing */
	HMAC_CTX *ctx = HMAC_CTX_new();
	if (ctx == NULL) {
		printf("Error with new\n");
		exit(0);
	}
	int rc = HMAC_Init_ex(ctx, hmacKey, 3, HASH_ALGO, NULL);
	if (rc != 1) {
		printf("Error with init hmac\n");
		exit(0);
	}
	unsigned int len = 0;

#ifdef HASH
	gettimeofday(&startHMAC, NULL);
	// hmacDigest = HMAC(md, hmacKey, 3, (unsigned char*)hmacPseudonym, 9, NULL,
	// NULL);
	HMAC_Update(ctx, hmacPseudonym,
				9); // 9 is a hardcoded valude of the length of the pseudonym
	HMAC_Final(ctx, hmacDigest, &len);
	gettimeofday(&endHMAC, NULL);
	useconds_hmac = endHMAC.tv_usec - startHMAC.tv_usec;
#endif

	/***START*******ENCODE****************************************************************
	*************************************************************************************/
	gettimeofday(&start, NULL);

	if (!huffman_encode(&h, plaintext, ciphertext)) {
		fprintf(stderr, "huffman_encode has reported an error\n");
		return 1;
	}
#ifdef HASH
	xor_op(ciphertext->data, ciphertext->size, hmacDigest, EVP_MAX_MD_SIZE);
#endif
	gettimeofday(&end, NULL);
	/*****END*****ENCODE*****************************************************************
	*************************************************************************************/

	/*****START***DECODE*****************************************************************
	*************************************************************************************/
	gettimeofday(&start1, NULL);

#ifdef HASH
	xor_op(ciphertext->data, ciphertext->size, hmacDigest, EVP_MAX_MD_SIZE);
#endif

	if (!huffman_decode(&h, ciphertext, plaintext)) {
		fprintf(stderr, "huffman_decode has reported an error\n");
		return 1;
	}

	gettimeofday(&end1, NULL);
	/******END********DECODE*************************************************************
	*************************************************************************************/

	useconds_encoding = end.tv_usec - start.tv_usec;
	useconds_decoding = end1.tv_usec - start1.tv_usec;

	// fprintf(stderr, "hmac timing: %ld\n", useconds_hmac);
	// fprintf(stderr, "encoding timing: %ld\n", useconds_encoding);
	// fprintf(stderr, "decoding timing: %ld\n", useconds_decoding);
	// fprintf(stdout,"[enc timing : %d] %ld\n",PT_SIZE,useconds_encoding);
	// fprintf(stdout,"[dec timing : %d] %ld\n",PT_SIZE,useconds_decoding);

	/* print the data to a file */
#ifdef NOHASH
	FILE *encodeOut = fopen("results/encodeTimeHuffmanNOHASH.txt", "a");
	FILE *decodeOut = fopen("results/decodeTimeHuffmanNOHASH.txt", "a");
	fprintf(encodeOut, "%ld\n", useconds_encoding);
	fprintf(decodeOut, "%ld\n", useconds_decoding);
#endif

#ifdef HASH
	FILE *encodeOut = fopen("results/encodeTimeHuffman.txt", "a");
	FILE *decodeOut = fopen("results/decodeTimeHuffman.txt", "a");
	fprintf(encodeOut, "%ld\n", useconds_hmac + useconds_encoding);
	fprintf(decodeOut, "%ld\n", useconds_hmac + useconds_decoding);
#endif

	fclose(encodeOut);
	fclose(decodeOut);

	return 0;
}
