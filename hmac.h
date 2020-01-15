#ifndef hmac_h
#define hmac_h

#include "bitarray.h"
#include <openssl/hmac.h>
#include <stdint.h>
#include <string.h>

#ifdef DEBUG
#include <stdio.h>
#endif

/* will modify the data passed in through digest pointer
 * any
 */
void xor_op(unsigned char *digest, size_t digestLen,
			const unsigned char *hmacDigest, size_t hmacDigestLen);

#endif
