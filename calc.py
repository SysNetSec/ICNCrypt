import matplotlib.pyplot as plt
import numpy as np
from scipy.stats import norm
from pathlib import Path
import json, sys, math
import matplotlib.style as style

# running list of read in numbers
lines = []

# used to hold all data to be graphed
data = {'names':[],
        'averages':[],
        'stddevs':[],
        'maxs':[],
        'mins':[]
       }

def dump_data(name):
    with open(".data/"+name,"w") as f:
        json.dump(data,f)


def read_data(name):
    with open(".data/"+name,"r") as f:
        data = json.load(f)
    return data

def graph_distr(mu,sigma,x1,x2,name,title):
    name = name.replace(' ' ,'_')
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
    print("[*] Made graph: " + name)
    #plt.show()

    # end graph_distr

def graph_bar(d,name,data_size):
    style.use('ggplot')
    algo = d['names']
    #x_pos = np.arange(len(algo))
    x_pos = [1,1.85,3,3.85,5,5.85,7,7.85]
    means = d['averages']
    error = d['stddevs']
    fig, ax = plt.subplots()
    #t = ax.bar(x_pos, means, yerr=error, align='center',alpha=0.5,ecolor='black',capsize=10)
    t = ax.bar(x_pos, means, yerr=error, alpha=0.65,capsize=5)
    
    ax.set_ylabel('Time ( milliseconds )')
    ax.set_xticks(x_pos)
    ax.set_xticklabels(algo,rotation=90)
    ax.set_title('Timing on Data Size: ' + data_size)
    ax.yaxis.grid(True)
    for i in range(0,len(t)):
        if i%2 == 0:
            t[i].set_color('green')
        else:
            t[i].set_color('red')

    # graph bar

    plt.tight_layout()
    name = data_size + "-" + name
    #plt.savefig(name)
    print("[*] Made graph: " + name)
    plt.show()



def calc(filename,name):
    lines.clear()
    with open(filename,"r") as f:
        line = f.readline().strip()
        while line:
            lines.append(int(line))
            line = f.readline().strip()

    summ = 0
    for x in lines:
        summ += x 
    #avg = summ/len(lines)
    avg = np.average(lines)
    std = np.std(lines) 
    ma = max(lines)
    mi = min(lines)
    #print("file: " + filename)
    #print("avg: %.2f"%np.average(lines))
    #print("std: %.2f"%np.std(lines))
    #print("max: " + str(max(lines)))
    #print("min: " + str(min(lines)))
    #print("")
    data['names'].append(name)
    data['averages'].append(avg)
    data['stddevs'].append(std)
    data['maxs'].append(ma)
    data['mins'].append(mi)



if __name__ == "__main__":

    if len(sys.argv) == 2:
        if sys.argv[1] == "--read":
            data = read_data("huffmanvsaes.json")
            graph_bar(data,"TimingCompare.png")
            quit()
        else:
            data_size = sys.argv[1]

    # build  bar graph

    # calc stores data to data dictionary
    calc("results/encryptTime.txt","aes enc")
    calc("results/decryptTime.txt","aes dec")
    calc("results/encryptTimeAESNI.txt","aesni enc")
    calc("results/decryptTimeAESNI.txt","aesni dec")
    calc("results/encodeTimeHuffman.txt","huffman encoding")
    calc("results/decodeTimeHuffman.txt","huffman decoding")
    calc("results/encodeTimeHuffmanNOHASH.txt","huffman encoding no hmac")
    calc("results/decodeTimeHuffmanNOHASH.txt","huffman decoding no hmac")

    #dump_data("huffmanvsaes.json")
    #data = read_data("huffmanvsaes.json")
    # build  bar graph
    graph_bar(data,"TimingCompare.png",data_size)

    """
    # build each distribution graph
    for i in range(0,len(data['names'])):
        graph_distr(data['averages'][i],
                    data['stddevs'][i],
                    data['mins'][i],
                    data['maxs'][i],
                    data['names'][i] + ".png",
                    data['names'][i])
    """

    


