import random
import math
import copy


RAND_LOW = 10e10
RAND_HIGH = 10e15
THRESHOLD = 300
COOLING_COEF = 0.99

def swap (arr:list, x:int, y:int):
    tmp = arr[x]
    arr[x] = arr[y]
    arr[y] = tmp

def getTemp():
    return random.randint(RAND_LOW, RAND_HIGH)

def randomNeighbor(x:list, C:list):
    rand_num1 = random.randint(1, len(x) - 3)
    y = copy.deepcopy(x)
    swap(y, rand_num1, rand_num1 + 1)
    return y
    
def propFunc(fcurr:list, fnew:list, temp:int):
    prop_val = math.exp( ( fcurr - fnew ) / temp )
    rand_val = random.random()
    return prop_val >= rand_val

def objFunc(x:list, C:list):
    res = 0
    for i in range(len(x) - 1):
        res += C[x[i]] [x[i+1]]
    return res

def printRoute(x:list, C:list, cities_dict:dict):
    print(cities_dict[x[0]], end="")
    for i in range(len(x) - 1):
        print(" -", C[x[i]][x[i+1]], "- ", end="")
        print(cities_dict[x[i+1]], end="")
    print("\n  ", "Celková vzdálenost:", objFunc(x, C))

def decreaseTemp(temp):
    return temp * COOLING_COEF