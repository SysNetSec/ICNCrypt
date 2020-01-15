
#include "hmac.h"

// goes of the digestLen
void xor_op(unsigned char *digest, size_t digestLen,
			const unsigned char *hmacDigest, size_t hmacDigestLen)
{

	for (int i = 0; i < digestLen; i++) {
		digest[i] = digest[i] ^ hmacDigest[0];
	}
}
