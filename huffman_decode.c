#include "huffman.h"

inline bool huffman_decode(const huffman_t *h, const bitarray_t *input,
						   bitarray_t *output)
{

	int i, j;
	uint8_t nextbyte;
	uint8_t cur = MAX_NODES - 1;

	bitarray_clear(output);

	for (i = 0; bitarray_read(input, i, &nextbyte, 1) == 1; i++) {

		if (nextbyte == 0) {
			cur = h->nodes[cur].l;
		}
		else {
			cur = h->nodes[cur].r;
		}

		if (cur < ALPHABET_SIZE) {
			for (j = 0; j < SYMBOL_BITS; j++) {
				if (!bitarray_append(output, cur & (1 << j))) {
#ifdef DEBUG
					fprintf(stderr, "huffman_decode: output array exhausted\n");
#endif
					return false;
				}
			}
			cur = MAX_NODES - 1;
		}
	}

	return cur == MAX_NODES - 1;
}
