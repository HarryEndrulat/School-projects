from sys import argv
import numpy as np


def readFile(fileName):
    W=0
    n=0
    w=[]
    A=[]

    file = open(fileName)

    for i, line in enumerate(file.readlines()):
        args = line.split()
        if i==0:
            n=int(args[0])
            W=int(args[1])
        elif i==1:
            for arg in args:
                w.append(int(arg))
        elif i==2:
            for arg in args:
                A.append(int(arg))
    return W, n, w, A


def knapSackTable(W, n, w, A):
    V=np.zeros((n+1,W+1))
    for i in range(1, n+1):
        for j in range(1, W+1):
            if ((j-w[i-1])>=0):
                    V[i][j]=max(V[i-1][j],V[i-1][j-w[i-1]]+A[i-1])
            else:
                V[i][j]=V[i-1][j]
    return V


def printKnapSack(V, W, w, n, A):
    S=[]
    i=n-1
    j=W
    while i>=0 and j>=1:
        while i>=1 and V[(i+1, j)]==V[(i, j)]:
            i-=1
        if not V[(i+1, j)]==V[(i, j)]:
            S.append((A[i],w[i], i+1))
            j=j-w[i]
            i -= 1
    totalVal=0.0
    for i, value in enumerate(S):
        print("Item:", value[2], ", weight:", value[1], ", value:", value[0])
        totalVal+=value[0]
    print("Optimal weight:", (W-j))
    print("Optimal value:",totalVal)
    return S


W, n, w, A = readFile(argv[1])
V = knapSackTable(W, n, w, A)
printKnapSack(V, W, w, n, A)
