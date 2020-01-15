#include "huffman.h"
#include <stdbool.h>

/** Returns the frequency of node with given id, using separate arrays for
 *  leaf frequences and inner node frequencies.
 */
static inline uint16_t
get_freq(uint8_t node_id, const uint8_t leaf_freq[ALPHABET_SIZE],
		 const uint16_t inner_freq[MAX_NODES - ALPHABET_SIZE])
{
	if (node_id < ALPHABET_SIZE) {
		return leaf_freq[node_id];
	}
	else {
		return inner_freq[node_id - ALPHABET_SIZE];
	}
}

/** Finds the index of the minimum-frequency subtree in the forest, returns
 *  true on success. If exclude flag is true, exclude excl in the search. Places
 *  the index of the minimum node in *res, and its frequency in *freq.
 *
 *  Runs in O(n) time.
 */
static bool get_min_node(const huffman_t *h,
						 const uint8_t leaf_freq[ALPHABET_SIZE],
						 const uint16_t inner_freq[MAX_NODES - ALPHABET_SIZE],
						 uint8_t num_nodes, uint8_t *res, uint16_t *freq,
						 bool exclude, uint8_t excl)
{
	bool found = false;
	uint8_t v = 0, i;
	uint16_t vfreq;

	// find the first parentless node
	for (i = 0; i < num_nodes; i++) {
		if (h->nodes[i].p == 0 && (!exclude || i != excl)) {
			found = true;
			v = i;
			vfreq = get_freq(i, leaf_freq, inner_freq);
			break;
		}
	}

	// if no node found, fail
	if (!found) {
		return false;
	}

	// find a better node
	for (i = i + 1; i < num_nodes; i++) {
		uint16_t this_freq = get_freq(i, leaf_freq, inner_freq);
		if (h->nodes[i].p == 0 && this_freq < vfreq &&
			(!exclude || i != excl)) {
			v = i;
			vfreq = this_freq;
		}
	}

	*freq = vfreq;
	*res = v;
	return true;
}

void huffman_init(huffman_t *h, const uint8_t freqs[ALPHABET_SIZE])
{

	// temporary array for frequencies of non-leaves
	uint16_t inner_freq[MAX_NODES - ALPHABET_SIZE];

	// create leaf nodes: O(n)
	uint8_t i;
	for (i = 0; i < ALPHABET_SIZE; i++) {
		node_t new = {0};
		h->nodes[i] = new;
	}

	uint8_t num_nodes = ALPHABET_SIZE;

	// create inner nodes: O(n) cost * O(n) iterations = O(n^2)
	uint8_t m1, m2;
	uint16_t f1, f2;
	while (get_min_node(h, freqs, inner_freq, num_nodes, &m1, &f1, false, 0) &&
		   get_min_node(h, freqs, inner_freq, num_nodes, &m2, &f2, true, m1)) {

		// instantiate node
		node_t new = {0};
		new.l = m1;
		new.r = m2;
		inner_freq[num_nodes - ALPHABET_SIZE] = f1 + f2;

		// copy into nodes array
		h->nodes[num_nodes] = new;

		// update parents
		h->nodes[m1].p = num_nodes;
		h->nodes[m2].p = num_nodes;

		// increment counter
		num_nodes++;
	}
}
