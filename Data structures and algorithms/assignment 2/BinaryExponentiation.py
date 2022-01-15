import sys


a=float(sys.argv[1])
m=float(sys.argv[2])
n=int(sys.argv[3])
b=format(n,"b")

term = a


if b[-1] == '1':
    product=a
else:
    product=1

for i in range(len(b)-1, 0, -1):
    term = term*term%m
    if b[i-1]=='1':
        product = product*term%m


print(product)