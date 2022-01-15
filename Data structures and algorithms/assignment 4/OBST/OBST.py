import math
from sys import argv
import numpy as np
import collections

class Node:
    def __init__(self, data):
        self.left = None
        self.right = None
        self.parent = None
        self.data =data

def read_file(fileName):#may need to sort the keys
    file=open(fileName)
    i=1
    propDict ={}
    for line in file.readlines():
        args = line.split()
        if i != 1:
            key =args[0]
            prop =float(args[1])
            propDict[key]=prop
        i+=1
    orderedPropDict =collections.OrderedDict(sorted(propDict.items()))
    return orderedPropDict


def tables(propDict):
    #output the cost table and root table of the OBST
    n=len(list(propDict.keys()))
    costTable=np.zeros((n+2, n+1))
    rootTable=np.empty((n, n), dtype=object)

    propDictValues=list(propDict.values())
    propDictKeys = list(propDict.keys())
    n=len(propDict.keys())
    for i in range(1,n+1):
        costTable[i][i-1]=0
        costTable[i][i]=propDictValues[i-1]
        rootTable[i-1][i-1]=propDictKeys[i-1]
    costTable[n][n-1]=0
    for d in range(1,n+1):
        for i in range(1,(n-d)+1):
            j=d+i
            min = math.inf
            for l in range(i, j+1):
                q=costTable[(i,l-1)]+costTable[(l+1,j)]
                if q<min:
                    min=q
                    rootTable[(i-1,j-1)]=propDictKeys[l-1]
            subTreeSum=0.0
            for s in range(i,j+1):
                subTreeSum+=propDictValues[s-1]
            costTable[(i,j)]=min+subTreeSum
    return costTable, rootTable


def OBST(propDictKeys, rootTable):
    n = len(propDictKeys)
    root = Node(rootTable[(0,n-1)])
    S=[]
    S.append((root, 0, n-1))
    while len(S)>0:
        (u, i, j)=S.pop()
        key=rootTable[(i,j)]
        l=propDictKeys.index(key)
        if(l<j):
            v=Node(rootTable[(l+1,j)])
            v.parent=u
            u.right=v
            S.append((v,l+1,j))
        if(l>i):
            v = Node(rootTable[(i,l-1)])
            u.left=v
            v.parent = u
            S.append((v,i,l-1))
    return root

def printTree(root, propDictValues, propDictKeys):
    if root:
        printTree(root.left, propDictValues, propDictKeys)
        key =root.data
        index=propDictKeys.index(key)
        prop = propDictValues[index]
        if root.parent:
            parent = root.parent.data
        else:
            parent=None
        if root.left:
            left=root.left.data
        else:
            left=None
        if root.right:
            right =root.right.data
        else:
            right = None
        print("\nNode:","\nKey:",key,"\nprobability:",prop,"\nParent:",parent,"\nLeft Child:",left,"\nRight Child:",right)
        printTree(root.right, propDictValues, propDictKeys)
    return None


propDict=read_file(argv[1])
costTable, rootTable=tables(propDict)
root =OBST(list(propDict.keys()), rootTable)
printTree(root, list(propDict.values()), list(propDict.keys()))
