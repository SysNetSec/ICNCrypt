#python3 generate_pmf.py 128 filename
#!/usr/bin/env python3

import sys, random, struct, numpy

def main(alphabet_size="128", output_file=None):

    alphabet_size = int(alphabet_size)
    
    if output_file is None:
        output_file = sys.stdout.buffer
    else:
        output_file = open(output_file, 'wb')
    
    freqs = numpy.random.zipf(2., alphabet_size)
    freqs = freqs * 128.0 / max(freqs)
    freqs = freqs.astype('uint8')
    
    binstr = b''.join(struct.pack('B', x) for x in freqs)
    output_file.write(binstr)
    
    if output_file is not sys.stdout.buffer:
        output_file.close()
    
    return 0

if __name__ == "__main__":
    sys.exit(main(*sys.argv[1:]))
