import sys
import pickle
import os
from helpers import getNeighbors, reconstructPath, distance
from visualization import progress, animate

with open(sys.argv[1], 'rb') as f:
    start, end, labyrint = pickle.load(f)

ims = []
progress(labyrint, start, 'S', ims, True)

def insert_for_h(open:list, y):
    for i, item in enumerate(open):
        if distance(item, end) >= distance(y, end):
            open.insert(i, y)
            return
    open.append(y)

def greedy_search():
    open = []
    closed = []
    prev = {start:None}

    open.append(start)

    while len(open) != 0:
        x = open.pop(0)
        if x == end:
            return reconstructPath(x, prev)
        for y in getNeighbors(x, labyrint):
            if y not in open and y not in closed:
                insert_for_h(open, y)
                progress(labyrint, y, '#', ims, False)
                prev[y] = x
        closed.append(x)
        if x != start:
            progress(labyrint, x, '-', ims, False)

result = greedy_search()
for item in result:
    progress(labyrint, item, 'o', ims)

ims.pop(0)
animate(ims)

# cleaning
os.remove(sys.argv[1])