import matplotlib.pyplot as plt
import numpy as np
from scipy.stats import norm
import math
from pathlib import Path
    
def graph_distr(mu,sigma,x1,x2,name,title):
    z1 = (x1-mu) / sigma
    z2 = (x2-mu) / sigma
    x = np.arange(z1,z2,0.001)
    x_all = np.arange(-10,10,0.001)
    y = norm.pdf(x,0,1)
    y2 = norm.pdf(x_all,0,1)

    fig, ax = plt.subplots(figsize=(9,6))
    plt.style.use('fivethirtyeight')
    ax.plot(x_all,y2)

    ax.fill_between(x,y,0, alpha=0.3, color='b')
    ax.fill_between(x_all,y2,0, alpha=0.1)
    ax.set_xlim([-2,2])
    ax.set_yticklabels([])
    ax.set_title(title)

    plt.savefig(name,dpi=72,bbox_inches='tight')
    #plt.show()

    # end graph_distr

def graph_bar():

    algo =\
    ['aes\nenc','aes\ndec','aesni\nenc','aesni\ndec','huffman\nenc','huffman\ndec','huffman\nencode\nno hmac','huffman\ndecode\nno hmac']
    x_pos = np.arange(len(algo))
    means = [11.79, 11.29, 11.97, 11.49, 6.46, 4.03, 4.73, 2.49]
    error = [4.64, 4.29, 4.58, 4.27, 2.80, 2.18, 1.83, 2.5]
    fig, ax = plt.subplots()
    ax.bar(x_pos, means, yerr=error,
            align='center',alpha=0.5,ecolor='black',capsize=10)
    ax.set_ylabel('Time ( milliseconds )')
    ax.set_xticks(x_pos)
    ax.set_xticklabels(algo,rotation=0)
    ax.set_title('AES vs Huffman vs Huffman w/o HMAC')
    ax.yaxis.grid(True)

    plt.tight_layout()
    plt.savefig('TimingCompare.png')
    plt.show()


    # end graph_bar



if __name__ == "__main__":
    graph_bar()
    #graph_distr(11.79,4.64,8,94,"aes_enc.png","AES ENC")
    #graph_distr(11.97,4.58,9,72,"aesni_enc.png","AESNI ENC")
    #graph_distr(11.29,4.29,7,42,"aes_dec.png","AES DEC")
    #graph_distr(11.49,4.27,5,37,"aesni_dec.png","AESNI DEC")
    #graph_distr(6.46,2.80,4,28,"huffman_enc.png","Huffman Encode")
    #graph_distr(4.73,1.83,4,28,"huffman_enc_nohash.png","Huffman Encode No HMAC")
    #graph_distr(4.03,2.18,3,35,"huffman_dec.png","Huffman Decode")
    #graph_distr(2.49,2.5,2,45,"huffman_dec_nohash.png","Huffman Decode No HMAC")


