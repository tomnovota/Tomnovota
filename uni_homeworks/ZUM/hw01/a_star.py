import sys
import pickle
import os
from helpers import getNeighbors, reconstructPath, distance
from visualization import progress, animate

with open(sys.argv[1], 'rb') as f:
    start, end, labyrint = pickle.load(f)

ims = []
progress(labyrint, start, 'S', ims, True)

def insert_for_h(open:list, dist:dict, y):
    for i, item in enumerate(open):
        if dist[item] + distance(item, end) >= dist[y] + distance(y, end):
            open.insert(i, y)
            return
    open.append(y)

def a_star():
    open = []
    closed = []
    prev = {start:None}

    dist = {}
    dist[start] = 0

    open.append(start)

    while len(open) != 0:
        x = open.pop(0)
        if x == end:
            return reconstructPath(x, prev)
        for y in getNeighbors(x, labyrint):
            tmp = distance(y, start)
            if y not in closed or dist[y] > tmp:
                dist[y] = tmp
                if y in open:
                    open.remove(y)
                insert_for_h(open, dist, y)
                progress(labyrint, y, '#', ims, False)
                prev[y] = x
        closed.append(x)
        if x != start:
            progress(labyrint, x, '-', ims, False)

result = a_star()
for item in result:
    progress(labyrint, item, 'o', ims)

ims.pop(0)
animate(ims)

# cleaning
os.remove(sys.argv[1])