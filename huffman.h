#ifndef huffman_h
#define huffman_h

#include "bitarray.h"
#include <stdint.h>

#ifdef DEBUG
#include <stdio.h>
#endif

#define SYMBOL_BITS (7)
#define ALPHABET_SIZE (128)				  // encodes 7 bits at a time
#define MAX_NODES (2 * ALPHABET_SIZE - 1) // tree with n leaves has (2n-1) nodes

/** node_t:
 *  Structure representing a single node. Has pointers to left and right
 *  children, and a pointer to the parent.
 *
 *  Consumes 3 bytes if packed.
 */
typedef struct {
	uint8_t l; // left child, ptr via huffman_t.nodes
	uint8_t r; // right child, ptr via huffman_t.nodes
	uint8_t p; // parent, ptr via huffman_t.nodes
} node_t;

/** huffman_t:
 *  Container for an array of nodes representing the huffman tree.
 *
 *  Consumes 6n-3 bytes packed, where n = ALPHABET_SIZE
 */
typedef struct {
	node_t nodes[MAX_NODES];
} huffman_t;

/** huffman_init:
 *  Initializes the Huffman tree 'h' under the PMF described by the 'freqs'
 *  array.
 *
 *  Time complexity: O(n^2), where n = ALPHABET_SIZE
 *  Memory complexity: O(n). In theory, 2n+10 bytes total working memory.
 */
void huffman_init(huffman_t *h, const uint8_t freqs[ALPHABET_SIZE]);

/** huffman_setkey:
 *  Rekeys the inner nodes of the Huffman tree 'h' using the given 'code' array.
 *
 *  Time complexity: O(n), where n = ALPHABET_SIZE
 *  Memory complexity: O(n) worst case, O(log n) average case
 */
void huffman_setkey(huffman_t *h, const bitarray_t *code);

/** huffman_encode:
 *  Performs encoding with the input bitarray, storing result in the output
 *  bitarray. Returns true to indicate success, false on failure.
 *
 *  Time complexity: O(m log n), where n = ALPHABET_SIZE and m = message length
 *  Memory complexity: O(log n)
 */
bool huffman_encode(const huffman_t *h, const bitarray_t *input,
					bitarray_t *output);

/** huffman_decode:
 *  Performs decoding with the input bitarray, storing the result in the output
 *  bitarray. Returns true to indicate success, false on failure.
 *
 *  Time complexity: O(m log n)
 *  Memory complexity: O(1)
 */
bool huffman_decode(const huffman_t *h, const bitarray_t *input,
					bitarray_t *output);

#endif
