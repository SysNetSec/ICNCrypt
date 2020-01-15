#include "huffman.h"

static void setkey_helper(huffman_t *h, uint8_t node, uint8_t *id,
						  const bitarray_t *code)
{

	// if this is a leaf node, do nothing
	if (node < ALPHABET_SIZE) {
		return;
	}

	// get 'internal node' ID
	uint8_t this_id = (*id)++;

	// do DFS recursion
	setkey_helper(h, h->nodes[node].l, id, code);
	setkey_helper(h, h->nodes[node].r, id, code);

	// extract code from bit array
	uint8_t val;
	if (bitarray_read(code, this_id, &val, 1) != 1) {
#ifdef DEBUG
		fprintf(stderr,
				"error: bitarray passed to huffman_setkey too short.\n");
#endif
		return;
	}

	// if code for this node is 1, swap children
	if (val != 0) {
		uint8_t tmp = h->nodes[node].l;
		h->nodes[node].l = h->nodes[node].r;
		h->nodes[node].r = tmp;
	}
}

void huffman_setkey(huffman_t *h, const bitarray_t *code)
{
	uint8_t id = 0;
	setkey_helper(h, MAX_NODES - 1, &id, code);
}
