# python3 read_pmf.py filename
#!/usr/bin/env python3

import sys, struct

def main(input_file=None):

    if input_file is None:
        input_file = sys.stdin.buffer
    else:
        input_file = open(input_file, 'rb')

    count = 0
    while True:
        x = input_file.read(1)
        if x == b'':
            break
        x, = struct.unpack('B', x)
        print("%02x %3d" % (count, x))
        count += 1
    
    if input_file is not sys.stdin.buffer:
        input_file.close()
    
    return 0

if __name__ == "__main__":
    sys.exit(main(*sys.argv[1:]))
