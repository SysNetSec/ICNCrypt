#!/usr/bin/env bash

if [ $# -ne 1 ]; then
	echo "Need data size for tests!"
	exit
fi

rm results/*

NUM_RUN=10

# do not set over 512bit (32 byte) if using sha512 in huffman
# is number of bytes
DATA_SIZE=$1

F1=encryptTimeAESNI.txt
F2=decryptTimeAESNI.txt
F3=encryptTime.txt
F4=decryptTime.txt

# run test
for ((i=0; i<$NUM_RUN; i++));
do
	cd aes
	OPENSSL_ia32cap="~0x200000200000000" ./aes -e $DATA_SIZE >> ../results/$F3
	OPENSSL_ia32cap="~0x200000200000000" ./aes -d $DATA_SIZE >> ../results/$F4
	./aes -e $DATA_SIZE >> ../results/$F1
	./aes -d $DATA_SIZE >> ../results/$F2
	cd ..
	./huffman $DATA_SIZE
	./huffman_nohmac $DATA_SIZE
	echo -ne "[status] $i / $NUM_RUN\r" # display status
	sleep 0.2
done


python calc.py $DATA_SIZE



