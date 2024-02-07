import matplotlib.pyplot as plt
from matplotlib.colors import ListedColormap, Normalize
import matplotlib.animation as animation
import numpy as np

TRESHOLD = 0

color_map = {
    0: 'blue',
    1: 'red',
    2: 'black',
    3: 'white',
    4: 'green',
    5: 'lightgreen',
    6: 'orange'
}

value_map = {
    'S': 0,
    'E': 1, 
    'X': 2,
    ' ': 3,
    '#': 4,
    '-': 5,
    'o': 6
}

cmap = ListedColormap([c for c in color_map.values()])
norm = Normalize(vmin=0, vmax=6)

def progress(matrix:list, place:tuple, symbol:str, ims:list, save:bool = True):
    matrix[place[0]][place[1]] = value_map[symbol]
    if save or np.random.randint(1,100) > TRESHOLD:
        im = plt.imshow(matrix, cmap=cmap, norm=norm)
        ims.append([im])

def animate(ims:list):
    ani = animation.ArtistAnimation(plt.gcf(), ims, interval=5000/len(ims), blit=True)
    plt.show()

def tranform_labyrint(labyrint:list):
    return np.array([[value_map[val] for val in row] for row in labyrint])

def printMatrix(matrix:list):
    print("Labyrint:")
    for row in matrix:
        print(row)
