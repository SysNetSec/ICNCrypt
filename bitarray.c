#include "bitarray.h"

inline void bitarray_clear(bitarray_t *a) { a->size = 0; }

inline bool bitarray_append(bitarray_t *a, bool b)
{

	if (a->size == a->alloc * 8) {
		return false;
	}

	unsigned byte_idx = a->size / 8;
	unsigned bit_idx = a->size % 8;

	// if we are starting a new byte, clear it entirely
	if (bit_idx == 0) {
		a->data[byte_idx] = 0;
	}

	// conditionally set bit
	if (b) {
		a->data[byte_idx] |= (1 << bit_idx);
	}

	a->size++;

	return true;
}

inline unsigned bitarray_size(const bitarray_t *a) { return a->size; }

inline bool bitarray_copy(const bitarray_t *a, uint8_t *buf, unsigned size)
{
	unsigned bitarray_size = BITARRAY_BITS_TO_BYTES(a->size);

	if (bitarray_size < size) {
		return false;
	}

	unsigned i;
	for (i = 0; i < bitarray_size; i++) {
		buf[i] = a->data[i];
	}

	return true;
}

inline int bitarray_read(const bitarray_t *a, int offset, uint8_t *buf,
						 int size)
{

	int i;

	for (i = 0; i < size && i + offset < a->size; i++) {
		int src_byte_idx = (offset + i) / 8;
		int src_bit_idx = (offset + i) % 8;
		int dst_byte_idx = i / 8;
		int dst_bit_idx = i % 8;

		// if doing an aligned read, take a whole byte
		if (src_bit_idx == 0 && dst_bit_idx == 0 &&
			(size - (offset + i)) >= 8 && (a->size - (offset + i)) >= 8) {
			buf[dst_byte_idx] = a->data[src_byte_idx];
			i += 7;
		}

		// otherwise, do one bit at a time
		else {

			// if starting a new byte, clear it
			if (dst_bit_idx == 0) {
				buf[dst_byte_idx] = 0;
			}

			// conditionally set bit
			if (a->data[src_byte_idx] & (1 << src_bit_idx)) {
				buf[dst_byte_idx] |= (1 << dst_bit_idx);
			}
		}
	}

	return i;
}
