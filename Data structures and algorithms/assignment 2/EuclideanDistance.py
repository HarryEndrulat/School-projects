import math
import copy
import sys


def readFile(fileName):
    file = open(fileName)
    i = 1
    points=[]
    for line in file.readlines():
        args = line.split()
        if i == 1:
            n = args[0]
        else:
            points.append((float(args[0]), float(args[1])))
        i += 1
    return points

def getDistance(p, q):
    return math.sqrt((p[0] - q[0]) * (p[0] - q[0]) + (p[1] - q[1]) * (p[1] - q[1]))

def euclideanBruteForce(points):
    minDist = 62582362438415128148148
    n = len(points)
    pointA = 0, 0
    pointB = 0, 0
    for i in range(n - 1):
        for j in range(i + 1, n):
            distance = getDistance(points[i], points[j])
            if distance < minDist:
                minDist = distance
                pointA=points[i]
                pointB=points[j]
    return minDist, pointA, pointB

def euclideanSlice(slice, n, delta):
    min = delta
    for i in range(n):
        j = i + 1
        while j < n and (slice[j].y - slice[i].y < min):
            min = getDistance(slice[i], slice[j])
    return min

def euclideanDivideAndConquer(p):
    p.sort(key=lambda point: point[0])
    q = copy.deepcopy(p)
    q.sort(key=lambda point: point[1])

    return euclideanDivideAndConquerRecursive(p, q, len(p))

def euclideanDivideAndConquerRecursive(p, q, n):
    middle = int(n / 2)
    midPoint = p[middle]
    pLeft = p[:middle]
    pRight = p[middle:]

    if n <= 50:
        return euclideanBruteForce(p)

    deltaLeft = euclideanDivideAndConquerRecursive(pLeft, q, n - middle)
    deltaRight = euclideanDivideAndConquerRecursive(pRight, q, n - middle)

    delta = min(deltaLeft, deltaRight)
    stripLeft = []
    stripRight = []
    strip = pLeft + pRight
    for i in range(n):
        if abs(strip[i].x - midPoint.x) < delta:
            stripLeft.append(strip[i])
        if abs(strip[i].x - midPoint.x) > delta:
            stripRight.append(q[i])

    stripLeft.sort(key=lambda point: point.y)
    a = min(delta, euclideanSlice(stripLeft, len(stripLeft), delta))
    b = min(delta, euclideanSlice(stripRight, len(stripRight), delta))

    return min(a, b)


points = readFile(sys.argv[1])
dist, point1, point2 =euclideanDivideAndConquer(points)

print("The two closest points are: {0}, {1}. The distance between them is {2}".format(point1, point2, dist))

"""
find the mid-point / median w/ respect to x of the array
create 2 sub arrays based on x. recurse until the array is of size <= 50
at which point, use brute force.
compare the minDist of each recurse
create a subset based on the the minDist and median x,
only consider points from y-minDist to y+minDist
"""
