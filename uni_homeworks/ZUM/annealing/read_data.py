import csv
import glob
import os
import subprocess
import pickle

cities = []
distances = []

files = glob.glob(os.path.join("data", "*"))

print("Data:")
for i, file in enumerate(files):
    if ( file.endswith("csv") ):
        print(f"{i+1}. {file}")

file_num = int(input("Vyberte soubor: "))

if file_num < 1 or file_num > len(files):
    print("Invalid file number")
else:
    chosen_file = files[file_num-1]
    print(f"Vybrán soubor: {chosen_file}")


with open(chosen_file, 'r') as file:
    reader = csv.reader(file)
    for row in reader:
        city = row[0]
        distance_row = [int(x) for x in row[1:]]
        cities.append(city)
        distances.append(distance_row)

for i, city in enumerate(cities):
    print(f"{i+1}. {city}")

city_num = int(input("Vyberte město: "))
city_num -= 1

cities_dict = {i: cities[i] for i in range(len(cities))}
cities = [i for i in range(len(cities))]
cities.remove(city_num)
cities.insert(0, city_num)
cities.append(city_num)

with open('data.pkl', 'wb') as f:
    pickle.dump((cities, distances, cities_dict), f)


subprocess.call(['python', "hill_climbing.py", 'data.pkl'])