#!/bin/bash

for ((i=0; i<1; i++));
do
    #./aes 
	OPENSSL_ia32cap="~0x200000200000000" ./aes 100 10
done
