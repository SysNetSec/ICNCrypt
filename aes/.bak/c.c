#include <sys/time.h>
#include <openssl/aes.h>
#include <openssl/evp.h>
#include <openssl/rand.h>

int encrypt(unsigned char *pt, int pt_len, unsigned char *key, unsigned char *iv, unsigned char *ct)
{
	int ct_len = 0;
	int len = 0;
	EVP_CIPHER_CTX *ctx;
	if(!(ctx = EVP_CIPHER_CTX_new())) exit(1);
	if(1 != EVP_EncryptInit_ex(ctx, EVP_aes_256_cbc(),NULL,key,iv)) exit(1);
	if(1 != EVP_EncryptUpdate(ctx,ct,&len,pt,pt_len)) exit(1);
	ct_len = len;
	if(1 != EVP_EncryptFinal_ex(ctx,ct + len, &len)) exit(1);
	ct_len += len;
	return ct_len;
}

int decrypt(unsigned char *ct, int ct_len, unsigned char *key, unsigned char *iv, unsigned char *pt)
{
	int pt_len = 0;
	int len = 0;
	EVP_CIPHER_CTX *ctx;
	if(!(ctx = EVP_CIPHER_CTX_new())) exit(1);
	if(1 != EVP_DecryptInit_ex(ctx, EVP_aes_256_cbc(),NULL,key,iv)) exit(1);
	if(1 != EVP_DecryptUpdate(ctx,pt,&len,ct,ct_len)) exit(1);
	pt_len = len;
	if(1 != EVP_DecryptFinal_ex(ctx,pt + len, &len)) exit(1);
	pt_len += len;
	return pt_len;
}


