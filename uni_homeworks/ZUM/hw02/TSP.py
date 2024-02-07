"""
  --------------------------------
  -- Traveling salesman problem --
  --------------------------------
 
  -- Solved by sumulated annealing --

    -- Problem encoding --

Množina přípustných řešení: posloupnost míst k navštívení obsahující všechny místa, začínající a končící startem
    x = R ^ n, n - počet míst k navštívení, R - místo k navštívení

  -- Objective function --

Součet vzdáleností mezi sousedními místy

  -- Pravděpodobnostní funkce se kterou dojde k přechodu k horšímu stavu --
  
pFunc ( fcurr , fnew , t ) = e ^ ( (fnew - fcurr) / t )

  -- Temperature mechanism --

temp = getTemp()
decreaseTemp(temp)

W ... set of citis
C ... matrix of distances

1: x = random state()
2: temp.init(high number)

3: while temp >= 0:
    y = W.randomNeighbor(x)
    if f(y) > f(x):
        x = y
    elif pFunc(f(x), f(y), t) ≥ random(0, 1):
       x = y

    temp.decrease()

12: return x
"""

MINIMAL_TEMP = 1e-100

import sys
import pickle
from helpers import getTemp, randomNeighbor, propFunc, objFunc, printRoute, decreaseTemp

with open(sys.argv[1], 'rb') as f:
    W, C, cities_dict = pickle.load(f)

x = W

printRoute(x, C, cities_dict)

def annealing():
    x = W
    temp = getTemp()
    while temp > MINIMAL_TEMP:
        y = randomNeighbor(x, C)
        if objFunc(y, C) <= objFunc(x, C):
            x = y
        elif propFunc(objFunc(x, C), objFunc(y, C), temp):
            x = y
        temp = decreaseTemp(temp)
    return x

res = annealing()
printRoute(res, C, cities_dict)
