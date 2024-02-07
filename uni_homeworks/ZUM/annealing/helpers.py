import random
import numpy as np
import copy

MAX_ITER = 0
GOOD_ENOUGH = 0
k = 0 # number of neighbor iteration

RAND_LOW = 10e10
RAND_HIGH = 10e15
COOLING_COEF = 0.99
MINIMAL_TEMP = 1e-100

def objFunc(state, C):
    res = 0
    for i in range(len(state) - 1):
        res += C[state[i]] [state[i+1]]
    return - res

# prohození dvou měst vedle sebe
def swap(arr:list, p1:int, p2:int):
    arr[p1], arr[p2] = arr[p2], arr[p1]
    return arr

""" ONLY 1 neigbor
    rand_num = random.randint(1, len(state) - 3)
    y = copy.deepcopy(state)
    swap(y, rand_num, rand_num + 1)
    # neighbor[index_to_change] += random.uniform(-1, 1)
    return y
"""
def getNeighbors(state):
    return [ swap(copy.deepcopy(state), i, i + 1) for i in range(len(state) - 1) ]

def printRoute(x:list, C:list, cities_dict:dict):
    print(cities_dict[x[0]], end="")
    for i in range(len(x) - 1):
        print(" -", C[x[i]][x[i+1]], "- ", end="")
        print(cities_dict[x[i+1]], end="")
    print("\n  ", "Celková vzdálenost:", - objFunc(x, C),end="\n\n")

