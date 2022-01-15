import math
import queue
import sys
import numpy as np

import networkx as nx
from networkx import connected_components


def createGraph(fileName):
    i = 1
    g = nx.Graph()
    file = open(fileName)

    for line in file.readlines():
        args = line.split()

        if i != 1:
            if int(args[0]) not in g.nodes():
                g.add_node(int(args[0]))
            if int(args[1]) not in g.nodes():
                g.add_node(int(args[1]))
            g.add_edge(int(args[0]), int(args[1]), weight=float(args[2]))
        i += 1
    nx.set_node_attributes(g, 'white', 'color')
    return g


def BFS(g, startingV):
    travelOrder = []
    q = queue.Queue(maxsize=0)
    q.put(startingV)
    g.nodes[startingV]['color'] = 'gray'
    while not q.empty():
        x = q.get()
        g.nodes[startingV]['color'] = 'black'
        travelOrder.append(x)
        edges = sorted(g.edges(x, data=True), key=lambda t: t[2].get('weight', 1))
        for u, v, wt in edges:
            if g.nodes[v]['color'] == 'white':
                q.put(v)
                g.nodes[v]['color'] = 'gray'
    isdisconnected, node = isDisconnected(g)
    if isdisconnected:
        travelOrder.extend(BFS(g, node))
    return travelOrder


def DFS(g, startingV):
    travelOrder = []
    s = []
    s.append(startingV)
    travelOrder.append(startingV)
    g.nodes[startingV]['color'] = 'gray'
    while len(s) > 0:
        x = s.pop()
        s.append(x)
        localNeighbors = list(g.neighbors(x))
        for i, value in enumerate(localNeighbors):
            if value == min(localNeighbors):
                if g.nodes[value]['color'] == 'white':
                    s.append(value)
                    g.nodes[value]['color'] = 'gray'
                    travelOrder.append(value)
                else:
                    localNeighbors[i] = math.inf
            if localNeighbors[len(localNeighbors) - 1] == math.inf:  # if there are no neighbors
                s.pop()
                g.nodes[x]['color'] = 'black'
    isdisconnected, node = isDisconnected(g)
    if isdisconnected:
        travelOrder.extend(DFS(g, node))
    return travelOrder


def distanceMatrix(g):
    nodeLen = len(list(g.nodes))
    arr = np.zeros((nodeLen, nodeLen))
    allNodes = list(g.nodes)
    allEdges = list(g.edges(data=True))
    for p, node in enumerate(allNodes):
        for u, v, wt in allEdges:
            if node == u:
                arr[p, allNodes.index(v)] = wt['weight']
            if node == v:
                arr[p, allNodes.index(u)] = wt['weight']
    return arr

def FWdistanceMatrix(g):
    nodeLen = len(list(g.nodes))
    arr = np.full((nodeLen, nodeLen), np.inf)
    allNodes = list(g.nodes)
    allEdges = list(g.edges(data=True))
    for p, node in enumerate(allNodes):
        for u, v, wt in allEdges:
            if node == u:
                arr[p, allNodes.index(v)] = wt['weight']
            if node == v:
                arr[p, allNodes.index(u)] = wt['weight']
    for i in range(nodeLen):
        arr[i][i]=0
    return arr


def successorMatrix(g):
    allNodes = list(g.nodes)
    nodeLen = len(list(g.nodes))
    d=FWdistanceMatrix(g)
    s=np.zeros((nodeLen,nodeLen))
    for i in range(0, nodeLen):
        for j in range(0, nodeLen):
            if g.has_edge(allNodes[i], allNodes[j]):
                s[i][j]=j
    for k in range(0, nodeLen):
        for i in range(0, nodeLen):
            for j in range(0, nodeLen):
                if d[i][j]>d[i][k]+d[k][j]:
                    d[i][j]=d[i][k]+d[k][j]
                    s[i][j]=s[i][k]
    return d, s


def matrixMST(g):
    nodeLen = len(list(g.nodes))
    allNodes = list(g.nodes)
    MSTedges=[]
    totalWeight=0.0
    visited =[False for i in range(nodeLen)]
    arr = distanceMatrix(g)
    visited[0]=True
    v=-1
    u=-1
    for h in range(nodeLen-1):
        minWeight = math.inf
        for i, iValue in enumerate(allNodes):
            if not visited[i]:
                for j, jValue in enumerate(allNodes):
                    if visited[j]:
                        if minWeight > arr[i][j] > 0:
                            minWeight=arr[i][j]
                            v=j
                            u=i
        visited[u]=True
        MSTedges.append((allNodes[u],allNodes[v],minWeight))
        totalWeight+=minWeight

    return MSTedges, totalWeight


def F_W_MDT(g):
    d, s = successorMatrix(g)
    allNodes = list(g.nodes)
    nodeLen = len(allNodes)
    print("shortest path:")
    s=s.astype(int)
    for i in allNodes:
        for j in allNodes:
            print(i, "->", j, ":")
            path = [i]
            current = i
            while current != j:
                current = s[current][allNodes.index(j)]
                path.append(current)

            for idx, k in enumerate(path):
                l = idx + 1
                if l < len(path):
                    print("(", k, ",", path[l], ",", g[k][path[l]]['weight'], ")")
            print("Path Weight: ", d[i][j])
    return path

def isDisconnected(g):
    nodeAttributes = nx.get_node_attributes(g, 'color')

    for node, color in nodeAttributes.items():
        if color == 'white':
            return True, node
    return False, None


BFSTravelOrder = []
DFSTravelOrder = []
g = createGraph(sys.argv[1])
travelOrder = BFS(g, list(g)[0])
BFSTravelOrder.extend(travelOrder)

nx.set_node_attributes(g, 'white', 'color')
travelOrder = DFS(g, list(g)[0])
DFSTravelOrder.extend(travelOrder)

nx.set_node_attributes(g, 'white', 'color')

print("BFS traversal order: ", BFSTravelOrder)
print("DFS traversal order: ", DFSTravelOrder)
print("MST / MSF:")
MSTWeight =0
subgraphs = (g.subgraph(c) for c in connected_components(g))

for z in subgraphs:
    MSTedges, subWeight = matrixMST(z)
    MSTWeight+=subWeight
    for T in MSTedges:
        print("(", T[0], T[1], "weight:", T[2], ")")
print("Total Weight: ", MSTWeight)
subgraphs2 = (g.subgraph(c) for c in connected_components(g))
for y in subgraphs2:
    FWlist = F_W_MDT(y)
