#ifndef bitarray_h
#define bitarray_h

#include <stdbool.h>
#include <stdint.h>

typedef struct {
	unsigned size;
	unsigned alloc;
	uint8_t data[1];
} bitarray_t;

/** BITARRAY_BITS_TO_BYTES:
 *  Simply returns the number of bytes required to store the requested number
 *  of bits (rounded up).
 */
#define BITARRAY_BITS_TO_BYTES(n_bits) ((n_bits + 7) / 8)

/** BITARRAY_BYTES:
 *  Returns the requisite size of a bitarray to store the requested number of
 *  bytes; same as BITARRAY_BITS_TO_BYTES, except if n_bits is 0 we still need
 *  to allocate 1 byte.
 */
#define BITARRAY_BYTES(n_bits)                                                 \
	(n_bits == 0 ? 1 : BITARRAY_BITS_TO_BYTES(n_bits))

/** BITARRAY_SIZE:
 *  Returns the total size of a bitarray_t including internal overhead.
 */
#define BITARRAY_SIZE(n_bits) (2 * sizeof(unsigned) + BITARRAY_BYTES(n_bits))

/** BITARRAY_DECL:
 *  Macro for declaring a bitarray. First allocates a byte array of required
 *  size, then provides an alias (bitarray_t *) to that array. The final
 *  bitarray pointer will have the requested name; the backing byte array will
 *  be named name_raw. The internal fields of the bitarray will automatically
 *  be initialized.
 */
#define BITARRAY_DECL(name, n_bits)                                            \
	uint8_t name##_raw[BITARRAY_SIZE(n_bits)];                                 \
	bitarray_t *const name = (bitarray_t *)&name##_raw;                        \
	do {                                                                       \
		name->alloc = BITARRAY_BYTES(n_bits);                                  \
		name->size = 0;                                                        \
	} while (0);

/** bitarray_clear:
 *  Empties a bitarray; i.e. sets its length to 0.
 */
void bitarray_clear(bitarray_t *a);

/** bitarray_append:
 *  Appends one bit to a bit array. Returns true on success, or false on failure
 *  (indicating that the array is full).
 */
bool bitarray_append(bitarray_t *a, bool b);

/** bitarray_size:
 *  Returns the size of the bitarray.
 */
unsigned bitarray_size(const bitarray_t *a);

/** bitarray_copy:
 *  Copies the bitarray into the given byte buffer. Returns true on success.
 *  Returns false on failure. Failure occurs if the bitarray does not fit in
 *  the given buffer.
 */
bool bitarray_copy(const bitarray_t *a, uint8_t *buf, unsigned size);

/** bitarray_read:
 *  Reads up to the requested number of bits from the array, starting at the
 *  given offset. Returns the number of bytes written to the buffer.
 */
int bitarray_read(const bitarray_t *a, int offset, uint8_t *buf, int size);

#endif
