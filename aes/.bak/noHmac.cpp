#include <openssl/hmac.h>
#include <openssl/sha.h>
#include "huffman.h"
 
#define DGSTSIZE 512
using namespace std;
using namespace boost;
 
 
//void strings_xor(string & s, string & t, string & res) 
string strings_xor(string & s, string & t) 
{
    string res = "";
    int j;
    for(int i = 0; i < s.size(); i++) 
    {
        j = i % DGSTSIZE;
        if(s[i] == t[j])
            res += '0';
        else
            res += '1';
    }
    return res;
}
 
void string2Binary(unsigned char * x, string & output)
{
    int _size = sizeof(char) * 8;
     
    for(int i = 0; i < SHA512_DIGEST_LENGTH; i++)
    {
        for(unsigned _s = 0; _s < _size; ++_s)
        {
              int bin = ((x[i] >> (_size - 1 - _s)) & 1);
              output.append(to_string(bin));
        }
    }
}
 
 
int main(int argc, char* argv[])
{  
    struct timeval start , end, start1, end1, start2, end2;
    long mtime, seconds, useconds_encoding, useconds_openfile, useconds_tree;
    string input_symbol_file, input_binary_file, input, binary, phase, digestStr, final_cipher;
    unsigned char * digest;
 
    // HMAC key and pseudonym
    char key[] = "key";
    char pseudonym [] = "123456789";
 
     
    if (argc != 3)
    {
        cout << "The usage of this file is as : ./huffman -enc(-dec) input-file-name" << endl;
    }
     
    phase = argv[1];
    gettimeofday(&start2, NULL);  
    getHuffmanCode();
    gettimeofday(&end2, NULL);  
         
    if(phase.compare("-enc") == 0)
    {
         
        input_symbol_file = argv[2];
 
        ifstream source;
 
        gettimeofday(&start1, NULL);  
        source.open(input_symbol_file.c_str(), ios::in);
        gettimeofday(&end1, NULL);  
 
        ofstream encoded;
        string output = input_symbol_file.append(".enc");
        encoded.open(output.c_str());
 
        while(getline(source, input))
        {
            gettimeofday(&start, NULL);  
             
        //  //Calculate the HMAC of the master key and pseudonym
        //  digest = HMAC(EVP_sha512(), key, strlen(key), (unsigned char*)pseudonym, strlen(pseudonym), NULL, NULL);    
            //  if (digest == NULL){
            //      fprintf(stderr, "HMAC has reported an error\n");
            //      return 1;
            //  }
        //  
        //  //Convert digest to binary string
        //  string2Binary(digest, digestStr); 
 
            //Huffman encode the message
            encoding(input);
 
            //Xor the Huffman encoded message with the digest
        //  encoded << strings_xor(result, digestStr) << endl;
                 
            gettimeofday(&end, NULL); 
            encoded << encoding(input) << endl;
                 
        }
         
    }
 
    else if (phase.compare("-dec") == 0)
    {
        input_binary_file = argv[2];
        ifstream destination;
        gettimeofday(&start1, NULL);  
        destination.open(input_binary_file.c_str(), ios::in);
        gettimeofday(&end1, NULL);  
         
        while(getline(destination, final_cipher))
        {
            gettimeofday(&start, NULL);  
 
            //Calculate the HMAC of the master key and pseudonym
        //  digest = HMAC(EVP_sha512(), key, strlen(key), (unsigned char*)pseudonym, strlen(pseudonym), NULL, NULL);    
            //  digest  if (digest == NULL){
            //  digest      fprintf(stderr, "HMAC has reported an error\n");
            //  digest      return 1;
            //  digest  }
        //  digest  
        //  digest  //Convert the digest to a binary string
        //  digest  string2Binary(digest, digestStr); 
 
        //  digest  //Remove the HMAC from the ciphertext
        //  digest  binary = strings_xor(final_cipher, digestStr);
 
            //Huffman decode the message
            decoding(&original_root, binary);
            gettimeofday(&end, NULL);  
        }
            //system("rm url1.txt.enc");    
    }
     
    seconds = end.tv_sec - start.tv_sec;
     
    useconds_tree = end2.tv_usec - start2.tv_usec;
    useconds_openfile = end1.tv_usec - start1.tv_usec;
    useconds_encoding = end.tv_usec - start.tv_usec;
 
    printf("microseconds, %ld\n", useconds_encoding);
    return 0;
}
