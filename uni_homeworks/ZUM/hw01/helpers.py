from visualization import value_map
from math import sqrt

def reconstructPath(elem, prev):
    result = []
    while elem:
        result.append(elem)
        elem = prev[elem]
    return result[::-1]

def getNeighbors(x, labyrint):
    tmp = [(x[0] + i[0], x[1] + i[1]) for i in [(1,0), (0,1), (-1,0), (0,-1)]]
    return [i for i in tmp if labyrint[i[0]][i[1]] != value_map['X']]

def distance(x, end):
    return sqrt((x[0] - end[0])**2 + (x[1] - end[1])**2)