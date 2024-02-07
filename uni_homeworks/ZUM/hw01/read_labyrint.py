import os
import glob
import argparse
import sys
import pickle
import subprocess
from visualization import tranform_labyrint

parser = argparse.ArgumentParser()
parser.add_argument("--directory", help="Path to test files", type=str, default="dataset/")
args = parser.parse_args()
directory = args.directory

labyrint = []
start = tuple()
end = tuple()

files = glob.glob(os.path.join(directory, "*"))

print(f"Složka: {directory}")
for i, file in enumerate(files):
    print(f"{i+1}. {file}")

file_num = int(input("Vyberte soubor: "))

if file_num < 1 or file_num > len(files):
    print("Invalid file number")
else:
    chosen_file = files[file_num-1]
    print(f"Vybrán soubor: {chosen_file}")

with open(chosen_file, 'r') as file:
    for line in file:
        if "start" in line:
            start_values = line.split()
            start_values[1] = start_values[1][:-1]
            start = tuple([int(start_values[2]), int(start_values[1])])
        elif "end" in line:
            end_values = line.split()
            end_values[1] = end_values[1][:-1]
            end = tuple([int(end_values[2]), int(end_values[1])])
        else:
            line_chars = [char for char in line.strip()]
            labyrint.append(line_chars)

labyrint[start[0]][start[1]] = 'S'
labyrint[end[0]][end[1]] = 'E'
labyrint_array = tranform_labyrint(labyrint)

with open('data.pkl', 'wb') as f:
    pickle.dump((start, end, labyrint_array), f)

method_dict = {
    1:"random_search.py",
    2:"bfs.py",
    3:"dfs.py",
    4:"greedy_search.py",
    5:"a_star.py"
}

names = ["RANDOM SEARCH",
        "BFS",
        "DFS",
        "GREEDY SEARCH",
        "A* algoritmus"]

print("Nabídka algoritmů:")
for i, name in enumerate(names):
    print(f"{i+1}. {name}")

method = method_dict[int(input("Vyberte algoritmus: "))]

subprocess.call(['python', method, 'data.pkl'])