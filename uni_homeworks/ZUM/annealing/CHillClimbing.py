import random
import numpy as np

"""
Maximizing
"""
class CHillClimbing:
    def __init__(self, objFunc, getNeighbors, C, initial_temp=10e10, cooling_coeff=0.99, temp_min=1e-100):
        self.objFunc = objFunc
        self.getNeighbors = getNeighbors
        self.initial_temp = initial_temp
        self.temp_min = temp_min
        self.cooling_coeff = cooling_coeff

        self.current_state = None
        self.temp = None

        self.C = C

    def acceptState(self, curr, new):
        return True if curr < new else True if random.random() < np.exp((new - curr) / self.temp) else False

    def annealing(self, initial_state):
        self.current_state = initial_state
        self.temp = random.randint(self.initial_temp / 10, self.initial_temp * 10)

        while self.temp > self.temp_min:
            N = self.getNeighbors(self.current_state)
            y = random.choice(N)

            if self.acceptState(self.objFunc(self.current_state, self.C), self.objFunc(y, self.C)):
                self.current_state = y

            self.temp *= self.cooling_coeff

        return self.current_state, self.objFunc(self.current_state, self.C)

    def steepest(self, initial_state, max_iter=1000, k = 1/4):
        self.current_state = initial_state
        i = 0
        while i < max_iter:
            i += 1
            N = self.getNeighbors(self.current_state)

            y = random.choice(N)
            for _ in range(int(k * len(N))): # searching for steepest neighbor
                z = random.choice(N)
                if self.objFunc(y, self.C) < self.objFunc(z, self.C):
                    y = z

            if self.objFunc(y, self.C) > self.objFunc(self.current_state, self.C):
                self.current_state = y
            elif self.objFunc(max(N, key=lambda x: self.objFunc(x, self.C)), self.C) <= self.objFunc(y, self.C): # local optimum
                break

        return self.current_state, self.objFunc(self.current_state, self.C)
