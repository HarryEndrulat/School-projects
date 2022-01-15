import heapq
from heapq import heapify, heappush, heappop
import sys


class TreeNode:
    def __init__(self):
        self.data=None
        self.char=None
        self.leftChild = None
        self.rightChild = None
        self.parent = None


def readFile(fileName):

    file = open(fileName)
    Dict={}
    totalChar =0
    for line in file.readlines():
        for i in line:
            totalChar+=1
            if i in Dict.keys():
                Dict[i] +=1
            else:
                Dict[i] = 1

    return Dict, totalChar



def buildTree(Dict, totalChar):
    heapList=[]
    # for k, v in Dict.items():
    #     heapList.append((k,v/totalChar*100))
    heapify(heapList)

    for k, v in Dict.items():
        node = TreeNode()
        node.char = k
        print(node.char)
        node.data = v/totalChar*100
        print(node.data)
        heappush(heapList, node.data)

    print(heapList)
    # heapq.heapify(heapList)
    # print(heapList)


    while len(heapList)>1:
        node_p = TreeNode()
        p_Left = heapq.heappop(heapList)
        p_Right = heapq.heappop(heapList)
        node_p.leftChild = p_Left
        node_p.rightChild = p_Right
        node_p.data =p_Left+p_Right
        heappush(heapList, node_p)

    return heappop(heapList)

Dict, totalChar = readFile(sys.argv[1])
print(buildTree(Dict,totalChar))

"""
read in the file and determine the number of times a character appears to determine freq.
use a dictionary or map to store characters w/ freq.
in order to determine the freq. take character/whole file
create a binary tree using a minHeap to grab the lowest freq.

"""