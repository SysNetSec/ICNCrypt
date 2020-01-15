#ifndef huffman_h
#define huffman_h
 
 
#include <iomanip>
#include<sstream>
#include<iostream>
#include<vector>
#include<string>
#include<cstring>
#include<functional>
#include<stdlib.h>
#include<fstream>
#include<sys/time.h>
#include<sys/types.h>
#include<boost/unordered_map.hpp>
#include<boost/functional/hash.hpp>
#include<unistd.h>
#include<ext/hash_map>
 
 
#define BOOST_HAS_HASH
#define SIZE 94 // number of alphabet symbols
 
using namespace std;
using namespace boost;
 
struct node
{
    node * leftChild;
    node * rightChild;
    int frequency;
    string content;
    string code;
};
 
 
node original_root;
 
// Use nodeArray to record all the nodes that may be created in the whole process
vector<node> nodeArray;
 
// Hash map table definition for Huffman code
boost::unordered_map<string, string> huff_tbl_map;
boost::unordered_map<string, string> huff_tbl_conv;
 
 
 
// Return the node that has the least frequency at each iteration
node extractMin()
{ 
    double temp = (double) INT_MAX;
    vector<node>::iterator i1,pos;
    for(i1 = nodeArray.begin();i1!=nodeArray.end();i1++)
    {  
         if(temp>(*i1).frequency)
         {
             pos = i1;
            temp = (*i1).frequency;
        }
    }
 
    node tempNode = (*pos);
    nodeArray.erase(pos); 
 
    return tempNode;
}
                      
 
// Build and insert the new node to array by assigning the two least 
// frequent nodes as its child and eventually return the root node
node getHuffmanTree()
{
    while(!nodeArray.empty())
    { 
        node * tempNode = new node;   
        node * tempNode1 = new node;
        node * tempNode2 = new node;
 
        *tempNode1 = extractMin();
        *tempNode2 = extractMin();
    
        tempNode->leftChild = tempNode1;
        tempNode->rightChild = tempNode2;
        tempNode->frequency = tempNode1->frequency+tempNode2->frequency;
        nodeArray.push_back(*tempNode);
        if(nodeArray.size() == 1)   //only the root node exsits
        {
            break;
        }
    }
    return nodeArray[0];
}
 
 
// Recursive function to make the code from root to the leaves 
void BFS(node * temproot,string s)
{
    node * root1 = new node;
    root1 = temproot;
   
    root1->code = s;
    if(root1 == NULL)
    {
     
    }
    else if(root1->leftChild == NULL && root1->rightChild == NULL)
    {
        //cout << "-----------------" << endl;
        //cout << "| " << root1->content;
        //cout << " | " << root1->code << "\t" << "|" << endl;
        huff_tbl_map.insert(std::make_pair(root1->content, root1->code));
        huff_tbl_conv.insert(std::make_pair(root1->code, root1->content));
        //cout << "-------->" << huff_tbl_map.first() << "," << huff_tbl_map.second() << endl;
        string tmp_code = huff_tbl_map.find(root1->content)->second;
        string tmp_content = huff_tbl_conv.find(root1->code)->second;
        //cout << "------------" << root1->content << " has key " << tmp_code << endl;
        //cout << "------------" << root1->code << " has key " << tmp_content << endl;
    
    }
    else
    {
        root1->leftChild->code = s.append("0");
        s.erase(s.end()-1);
        root1->rightChild->code = s.append("1");
        s.erase(s.end()-1);
 
        BFS(root1->leftChild,s.append("0"));
        s.erase(s.end()-1);
        BFS(root1->rightChild,s.append("1"));
        s.erase(s.end()-1);
    } 
}
 
 
void getHuffmanCode()
{
    int i;
    int tempDouble;
    string tempString; 
 
    ifstream enc;
    enc.open("pmf.txt");
 
    for(i = 0;i<SIZE;i++)
    {
        node tempNode;
        enc >> tempString;
        enc >> tempDouble;
 
        tempNode.frequency = tempDouble;
        tempNode.content = tempString;
        tempNode.leftChild = NULL;
        tempNode.rightChild = NULL;
        nodeArray.push_back(tempNode);
    }
    enc.close();
    node root = getHuffmanTree();
    original_root = root;
    BFS(&root,"");
}
 
 
string encoding(string input)
{
    string encoded, temp;
    string::iterator it;
    for(it = input.begin() ; it != input.end() ; it++)
    {
        temp = *it;
        encoded.append(huff_tbl_map.find(temp)->second);
    }
    return encoded;
}
 
 
string decoding(node * temproot,string s)
{
    node * root = new node;
    root = temproot;
    string decoded;
 
    int i = 0;
    for (i = 0 ; i < s.length() ; i++)
    {   
             
        if (s.at(i) == '1')
        {
            root = root->rightChild;
            if (root->leftChild == NULL && root->rightChild == NULL)
            {
                decoded.append(root->content);
                root = temproot;            
            }
        }
        else
        {
            root = root->leftChild;
            if (root->leftChild == NULL && root->rightChild == NULL)
            {
                decoded.append(root->content);
                root = temproot;            
            }
        }
    }
    return decoded;
}
 
#endif
