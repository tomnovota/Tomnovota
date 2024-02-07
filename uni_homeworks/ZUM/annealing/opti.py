"""
======================================
    ALGORITMICKY ŘEŠITELNÉ
======================================
- rekurzivní (DP)

======================================
    PROHLEDÁVÁNÍ STAVOVÉHO PROSTORU
======================================
- A*
- GeneralProblemSolver - automatické plánování

======================================
    CSP/SAT
======================================
- solvery

======================================
        ITERATIVNÍ OPTIMALIZACE
======================================

    -- PROBLEM ENCODING --

specifikace prostoru, kam patří řešení problému X'
omezující podmínky přípustného řešení X náleží X'

R^n (nejčastěji), {0,1}^n
- R je jedna položka
- n je počet položek

Optimalizační problémy řešené algoritmicky
1, least-squares
2, lineární programování
3, konvexní optimalizační problém

AI techniky
1, Hill Climbing
-> Problémy: lokální extrémy, prokletí dimenzionality (navzorkování okolí bodu) -> simulované žíhání
2, TABU - R^n:euklidovská vzdálenost, {0,1}^n: Hammingova vzdálenost, grafy:Strukturální podobnost
3, paralelní pouštění několika instancí řešení s různými počátky

SOTCHASTICKÉ POPULAČNÍ ITERATIVNÍ OPTIMALIAZCE
4, Evoluční metody - různé instance řešení spolupracují
- křížení, mutace
- genotyp, fenotyp, fitness, individual, population, generation
- populace -> selekce - rodiče -> křížení -> mutace - potomci -> náhrada ->
- nitching - na základě podobnosti genotypů zabraňuje lokálnímu přemnožení
- konkrétní zástupci: genetický algoritmus ({0,1}^n), genetické programování (graf), evoluční programování (stavový automat), evoluční strategie (R^n)

5, Particle Swarm Optimization
6, přírodní...

    -- Objective function --
Funkce nad instancí problému, kterou optimalizujeme

maximalizace/minimalizace je ekvivalentní
arg max(-f(x)) = arg min(f(x))
"""

import random
import numpy as np
from helpers import *

def objective(x):
    return x

def randomElem():
    return random.random()

def terminationCond(P):
    return 0

def selection(P):
    # exploatace - randomly select group of individuals and select argmax(obj(x))
    return 0

def crossover(p1, p2):
    # může se vynechat(když není vhodné pro dané kódování), jinak určit které bity budou ze kterého jedince
    return 0, 0

def mutation(o):
    # explorace - bit-flip - inverze několika náhodných bitů
    return 0

def genetic():
    num = 10
    P = [(x, objective(x)) for x in [randomElem() for _ in range(num)]]
    while not terminationCond(P):
        O = []
        for i in range(num/2):
            O.extend([(o, objective(o)) for o in [mutation(x) for x in [crossover(selection(P), selection(P))]]])

        P = O

