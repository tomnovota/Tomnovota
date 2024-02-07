"""
_________________________________________________________________________________

Počáteční stav:
    tabulka 9 x 9 
    9 vnitřních bloků 3 x 3
    v každé buňce hodnota buď číslice 1 až 9 nebo prázdná
_________________________________________________________________________________

Úkol:
    vyplnit všechny buňky číslicemi 1 až 9
_________________________________________________________________________________

Omezení:
    různé číslice v řádcích
    různé číslice ve sloupcích
    různé číslice v blocích
_________________________________________________________________________________


Metoda CSP:
    Pro každou buňku omezení vyfiltrují různé možnosti
    Pro některé buňky zbyde jediná možnost -> vyplní se a pokračuje se
    Pokud pro buňky neexistuje jediná možnost -> postupně se zkusí možnosti

X (konečná množina proměnných):
buňky v tabulce x_i_j; i,j = {1..9}

D (možnosti pro proměnné):
hodnoty buněk {1..9}

C (množina omezení):
již vyplněné hodnoty zadavatelem
různé číslice v řádcích AllDifferentConstraint(x_k_1 .. x_k_9); k = {1..9}
různé číslice ve sloupcích AllDifferentConstraint(x_1_k .. x_9_k); k = {1..9}
různé číslice v blocích AllDifferentConstraint( x_3*k+1_3*l+1 ........ x3*k+1,3*l+3
                                        :        .
                                        :               .
                                   x_3*k+3_3*l+1          x_3*k+3_3*l+3 )

AllDifferentConstraint - speciální filtrační algoritmus

Řešení CSP:
    přiřazení číslic buňkám, aby byly všechny omezení dodrženy

Koncepty CSP:
    stav, na který přejdeme nesmí porušovat pravidla

Heuristické prohledávání v CSP:
    Výchozí stav - žádné proměnné nejsou inicializovány
    Akce - přiřazení číslice buňce
    Cílový stav - Všechny proměnné mají přiřazenou číslici (komutativita v pořadí kroků)
    
"""

import matplotlib.pyplot as plt
import numpy as np
from constraint import Problem, AllDifferentConstraint
import math

from sudoku import Sudoku
from random import randint

class SudokuCSP:
    problem:Problem = None
    sudoku:np.array = None
    size:int = None
    colors:list = None
    color_options:list = None

    def __init__(self, sudoku:np.array, color_options = ['lightgrey', 'white'] ):
        self.problem = Problem()
        self.sudoku = sudoku
        self.size = len(sudoku)
        self.colors = np.empty((self.size, self.size), dtype = object)
        self.color_options = color_options
        self.InitVariables()
        self.InitConstraintRow()
        self.InitConstraintCol()
        self.InitConstraintBlock()
        self.GetSolution()

    # Projde všechna políčka sudoku a zavolá na ně inicializační metodu
    def InitVariables(self):
        for row in range(self.size):
            for col in range(self.size):
                self.InitVariable(self.sudoku[row][col], row, col)

    # Inicializuje políčko SUDOKU jako proměnnou CSP
    def InitVariable(self, val:int, row:int, col:int):
        name = f"x_{row}_{col}"
        self.problem.addVariable(name, range(1, self.size + 1)) # políčko má doménu číslice 1 až 9 pro sudoku 9x9
        if val != 0: # vyplněné hodnotě je přiřazena jen vyplněná hodnota
            self.problem.addConstraint(lambda a: a == val, [name])

    # Definice omezení CSP na unikátnost číslic v řádku
    def InitConstraintRow(self):
        for row in range(self.size):
            names = [f"x_{row}_{col}" for col in range(self.size)]
            self.problem.addConstraint(AllDifferentConstraint(), names)

    # Definice omezení CSP na unikátnost číslic ve sloupci
    def InitConstraintCol(self):
        for col in range(self.size):
            names = [f"x_{row}_{col}" for row in range(self.size)]
            self.problem.addConstraint(AllDifferentConstraint(), names)

    # Definice omezení CSP na unikátnost číslic pro bloky
    def InitConstraintBlock(self):
        block_len = int(math.sqrt(self.size))

        flip = True if block_len % 2 == 1 else False
        # iteruje přes všechny bloky
        for block_row in range(block_len):
            if block_len % 2 == 0:
                flip = not flip
            for block_col in range(block_len):
                # pole proměnných v jednom bloku
                block_variables = [] 

                # iteruje přes všechny proměnné v bloku
                for row in range(block_len):
                    for col in range(block_len):
                        row_coo = block_len*block_row + row
                        col_coo = block_len*block_col + col
                        block_variables.append(f"x_{row_coo}_{col_coo}")
                        self.colors[row_coo][col_coo] = self.color_options[0] if flip else self.color_options[1]

                flip = not flip
                self.problem.addConstraint(AllDifferentConstraint(), block_variables)

    def GetSolution(self):
        result = np.zeros((self.size, self.size), int)
        solution = self.problem.getSolution()
        for row in range(self.size):
            for col in range(self.size):
                result[row][col] = solution[f"x_{row}_{col}"]

        self.Plot(result)

    def Plot(self, result):
        # příprava dat pro tabulku
        data1 = [[' ' if self.sudoku[row][col]==0 else str(self.sudoku[row][col]) for col in range(self.size)] for row in range(self.size)]
        data2 = [[' ' if result[row][col]==0 else str(result[row][col]) for col in range(self.size)] for row in range(self.size)]

        # vytvoření tabulky
        figsize = 3 * int(math.sqrt(self.size) - 1)
        fig, ax = plt.subplots(nrows=1, ncols=2, figsize=(2 * figsize, figsize))
        ax[0].axis('off')
        ax[1].axis('off')
        tab1 = ax[0].table(cellText = data1, loc=(0,0), cellLoc = 'center', cellColours = self.colors)
        tab2 = ax[1].table(cellText = data2, loc=(0,0), cellLoc = 'center', cellColours = self.colors)

        # nastavení velikosti tabulky a písma
        tab1.auto_set_font_size(False)
        tab1.set_fontsize(20)
        tab1.scale(1, 3)

        tab2.auto_set_font_size(False)
        tab2.set_fontsize(20)
        tab2.scale(1, 3)
        plt.show()

def generateSudoku(size = 9, difficulty = 0.5):
    sudoku = Sudoku(int(math.sqrt(size)), seed=randint(10,500)).difficulty(difficulty=difficulty)
    board = sudoku.board
    return np.where(np.equal(board, None), 0, board), None# sudoku.solve().board

