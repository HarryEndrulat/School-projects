from sys import argv
import numpy as np


def readFile(fileName):
    pal = ''
    file = open(fileName)
    for line in file.readlines():
        args = line.split()
        pal = args[0]
    return pal

# dynamicPalindrome function constructs an array by comparing all indices with each other
# and stores the length in the array
def dynamicPalindrome(s):
    sLength = len(s)
    arr = np.zeros((sLength, sLength))
    for i in range(0, sLength):
        arr[i][i] = 1
    i = sLength - 1
    while i >= 0:
        for j in range(i + 1, sLength):
            if s[i] == s[j]:
                arr[i][j] = arr[i + 1][j - 1] + 2
            else:
                arr[i][j] = max(arr[i + 1][j], arr[i][j - 1])
        i -= 1
    return arr

# the backtrack function is used to iterate over the length array and return the palindrome and its length
def backtrack(arr, s):
    answer = ''
    cord = []
    i = 0
    j = len(s) - 1
    while i < len(arr) - 1 and j >= 0:
        if arr[i][j] > arr[i + 1][j] and arr[i][j] > arr[i][j - 1]:
            cord.append(i)
            cord.append(j)
            i, j = i + 1, j - 1
        elif arr[i + 1][j] > arr[i][j - 1] and arr[i + 1][j] == arr[i][j]:
            i = i + 1
        else:
            j = j - 1
    # set() in the following for loop removes duplicates and sorts the indices
    # the for loop itself builds the largest palindromic substring
    for i in set(cord):
        answer = answer + s[i]
    maxLength = int(arr[0][len(arr) - 1])
    return maxLength, answer


pal = readFile(argv[1])
arr = dynamicPalindrome(pal)
palLength, palString = backtrack(arr, pal)

print("the max palindrome is:", palString, "\nThe palindrome is of length:", palLength)
# the time complexity for this program is O(n^2) where n is the length of the string passed
