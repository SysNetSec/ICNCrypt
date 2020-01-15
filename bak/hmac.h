#ifndef hmac_h
#define hmac_h

#include <stdint.h>
#include <openssl/hmac.h>
#include <string.h>
#include "bitarray.h"

#ifdef DEBUG
#include <stdio.h>
#endif


/** hmac:
 *  generating hmac with the master key and a pseudonym.
 **/
    // generating hamc
bool hmac_generate(bitarray_t *digest)
{
    char hmacKey[] = "key";
    char hmacPseudonym [] = "123456789";
    unsigned char * hmacDigest;

    hmacDigest = HMAC(EVP_sha512(), hmacKey, strlen(hmacKey), (unsigned char*)hmacPseudonym, strlen(hmacPseudonym), NULL, NULL);
    if (hmacDigest == NULL){
        fprintf(stderr, "HMAC has reported an error\n");
        return 1;
    }

    for (int j = 0; j < 64; j++){
        for (int i = 0; i < 8; ++i) {
		if(!bitarray_append(digest,((hmacDigest[j] >> i) & 1))){
			return false;
		}
		//fprintf(stderr, "%d", ((hmacDigest[j] >> i) & 1));
        }
    }
    return true;
}


#endif
