#!/usr/bin/env bash

ssh pi@10.0.0.160 cd huffman; ./run.sh 
scp pi@10.0.0.160:huffman/.data/huffmanvsaes.json .
python calc.py --read
