import sys
import pickle
import os
import random
from helpers import reconstructPath
from helpers import getNeighbors
from visualization import progress, animate


with open(sys.argv[1], 'rb') as f:
    start, end, labyrint = pickle.load(f)

ims = []
progress(labyrint, start, 'S', ims, True)

def random_search():
    open = []
    closed = []
    prev = {start:None}

    open.append(start)

    while len(open) != 0:
        x = random.choice(open)
        if x == end:
            return reconstructPath(x, prev)
        for y in getNeighbors(x, labyrint):
            if y not in open and y not in closed:
                open.append(y)
                progress(labyrint, y, '#', ims, False)
                prev[y] = x
        open.remove(x)
        closed.append(x)
        if x != start:
            progress(labyrint, x, '-', ims, False)

result = random_search()
for item in result:
    progress(labyrint, item, 'o', ims)

ims.pop(0)
animate(ims)

# cleaning
os.remove(sys.argv[1])