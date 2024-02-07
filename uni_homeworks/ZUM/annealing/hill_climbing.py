import numpy as np
import sys
import pickle
from helpers import *
from CHillClimbing import *

with open(sys.argv[1], 'rb') as f:
    initial, C, cities_dict = pickle.load(f)

printRoute(initial, C, cities_dict)

solver = CHillClimbing(objFunc, getNeighbors, C)

print("     -- ANNEALING --")
final_state, final_value = solver.annealing(initial)
printRoute(final_state, C, cities_dict)

print("     -- STEEPEST --")
final_state, final_value = solver.steepest(initial)
printRoute(final_state, C, cities_dict)
