#include "huffman.h"

static inline void encode_helper(const huffman_t *h, uint8_t byte,
								 bitarray_t *revword)
{
	uint8_t node = byte & ((1 << SYMBOL_BITS) - 1);
	uint8_t parent = h->nodes[node].p;

	bitarray_clear(revword);

	while (parent) {

		if (h->nodes[parent].l == node) {
			bitarray_append(revword, 0);
		}
		else {
			bitarray_append(revword, 1);
		}

		node = parent;
		parent = h->nodes[node].p;
	}
}

inline bool huffman_encode(const huffman_t *h, const bitarray_t *input,
						   bitarray_t *output)
{

	BITARRAY_DECL(revword, ALPHABET_SIZE); // worst-case path from root to leaf
										   // is O(n) in huffman tree

	// read one symbol at a time
	int i;
	for (i = 0; i < bitarray_size(input); i += SYMBOL_BITS) {

		int j;
		uint8_t nextbyte; // symbol goes here
		unsigned readlen = bitarray_read(input, i, &nextbyte, SYMBOL_BITS);

		encode_helper(h, nextbyte, revword); // get codeword for symbol

		if (readlen != SYMBOL_BITS) {
#ifdef DEBUG
			fprintf(stderr, "huffman_encode: input length is not a multiple of "
							"symbol length\n");
#endif
			return false;
		}

		// reverse the leaf->root path to get the codeword
		for (j = revword->size - 1; j >= 0; j--) {
			uint8_t bit;
			bitarray_read(revword, j, &bit, 1);
			if (!bitarray_append(output, bit)) {
				return false; // output array has ran out of room
			}
		}
	}

	return true;
}
