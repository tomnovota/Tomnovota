###
#
# BI-JUL: Domácí úkol č. 2
#
# Tomáš Kalvoda, 2022
#

"""
Monte Carlo simulace Isingova modelu.
"""
module Ising

# Balíčky používané vaším modulem:
using Images, ProgressMeter, Colors

# Veřejné metody, přístupné uživateli.
export IsingPeriodic, IsingFixed
export plot, evolve!, randomize!, energy


#
# Typy
#

abstract type IsingModel end

"""
Isingův model s periodickými okrajovými podmínkami.

Konstruktor očekává rozměry obdélníku `height` a `width` a inicializuje všechny spiny do stavu `+1`.

Dále lze předat `3x3` matici `J` popisující interakci mezi sousedy.
Výchozí volba je matice plná jedniček, což odpovídá feromagnetické interakci a žádný ze sousedů není preferován.
Matice by měla být "symetrická" vůči zrcadlení vzhledem ke všem osám symetrie čtverce, jinak model nebude mít dobrý fyzikální význam (interakce by závisela na pořadí, v kterém spiny interagují, což nedává příliš smysl).

Konečně lze předat inverzní teplotu (`β`, výchozí hodnota `1.0`) nebo vnější magnetické pole (`h`, ve výchozím stavu vypnuto, tedy všechny jeho prvky jsou nulové).
"""
struct IsingPeriodic <: IsingModel
  height::Int64
  width::Int64
  J::Matrix{Float64}
  β::Float64
  sites::Matrix{Float64} # Is this optimal?
  h::Matrix{Float64}

  function IsingPeriodic(height, width, J=ones(3, 3); β=1.0, h=zeros(height, width))
    if β <= 0
      throw(ArgumentError("β has to be positive!"))
    end

    if size(J) != (3, 3)
      throw(ArgumentError("J has to be 3x3 matrix!"))
    end

    sites = ones(height, width)

    if size(sites) != size(h)
      throw(ArgumentError("Sizes of sites and h has to match!"))
    end

    new(height, width, J, β, sites, h)
  end
end

"""
Isingův model s pevnými okrajovými podmínkami.

Konstruktor očekává matici s rozměry `height × width`, kde spiny na okraji chápe jako hraniční a neměnné.
Během vývoje se mění jenom ty, co nejsou v prvním a posledním řádku a sloupci matice.

Dále lze předat `3x3` matici `J` popisující interakci mezi sousedy.
Výchozí volba je matice plná jedniček, což odpovídá feromagnetické interakci a žádný ze sousedů není preferován.
Matice by měla být "symetrická" vůči zrcadlení vzhledem ke všem osám symetrie čtverce, jinak model nebude mít dobrý fyzikální význam (interakce by závisela na pořadí).

Konečně lze předat inverzní teplotu (`β`, výchozí hodnota `1.0`) nebo vnější magnetické pole (`h`, ve výchozím stavu vypnuto, tedy všechny jeho prvky jsou nulové).

"""
struct IsingFixed <: IsingModel
  height::Int64
  width::Int64
  J::Matrix{Float64}
  β::Float64
  sites::Matrix{Float64} # Is this optimal?
  h::Matrix{Float64}

  function IsingFixed(sites, J=ones(3, 3); β=1.0, h=zeros(size(sites)))
    height, width = size(sites)

    if β <= 0
      throw(ArgumentError("β has to be positive!"))
    end

    if size(J) != (3, 3)
      throw(ArgumentError("J has to be 3x3 matrix!"))
    end

    if size(sites) != size(h)
      throw(ArgumentError("Sizes of sites and h has to match!"))
    end

    new(height, width, J, β, sites, h)
  end
end


#
# Časový vývoj
#

"""
Spustí simulaci Isingova modelu.
Na vstupu popořadě očekává:

  * `model`: Isingův model.
  * `max_steps`: maximální počet kroků (ve smyslu úspěšně změněných spinů), výchozí hodnota `1_000`.
  * `max_tries`: maximální počet neúspěšných pokusů o změnu hodnoty spinu, výchozí hodnota `1_00`.

Simulace probíhá podle Metropolisova algoritmu popsaného v `README.adoc`.
Pokusme se `max_steps`-krát změnit hodnotu některého ze spinů.
Pokud se náme `max_tries`-krát nepodaří hodnotu spinu změnit, tak jsme pravděpodobně blízko rovnovážného stavu a simulaci ukončujeme.
"""

function evolve!(model::IsingFixed, max_steps=1_00, max_tries=1_00) # max tries v kuse
    steps = 0
    tries = 0
    while steps < max_steps && tries < max_tries
        # choose element of sites with propability 1/n
        rand_posx = rand(2:model.height-1)
        rand_posy = rand(2:model.width-1)
        rand_elem = model.sites[rand_posx, rand_posy]

        # energy of old configuration
        pre_energy = energyPart(model, rand_posx, rand_posy)

        # new configuration with minused chosen element
        model.sites[rand_posx, rand_posy] = - rand_elem
        post_energy = energyPart(model, rand_posx, rand_posy)
        
        # if energy of new configuration > old configuration -> use new configuration with propability exp(-model.β * (energy(new)-energy(old)))
        if post_energy > pre_energy
            propability = exp(- model.β * (post_energy - pre_energy))
            rand_number = rand()
            if propability <= rand_number # no flip
                model.sites[rand_posx, rand_posy] = rand_elem
                tries += 1
                continue
            end
        end
        # if energy of new configuration <= old configuration -> use new configuration
        steps += 1
        tries = 0
    end
end

function evolve!(model::IsingPeriodic, max_steps=1_000, max_tries=1_00) # max tries v kuse
    steps = 0
    tries = 0
    while steps < max_steps && tries < max_tries
        # choose element of sites with propability 1/n
        rand_posx = rand(1:model.height)
        rand_posy = rand(1:model.width)
        rand_elem = model.sites[rand_posx, rand_posy]

        # energy of old configuration
        pre_energy = energyPart(model, rand_posx, rand_posy)

        # new configuration with minused chosen element
        model.sites[rand_posx, rand_posy] = - rand_elem
        post_energy = energyPart(model, rand_posx, rand_posy)

        # if energy of new configuration > old configuration -> use new configuration with propability exp(-model.β * (energy(new)-energy(old)))
        if post_energy > pre_energy
            propability = exp(- model.β * (post_energy - pre_energy))
            rand_number = rand()
            if propability <= rand_number # no flip
                model.sites[rand_posx, rand_posy] = rand_elem
                tries += 1
                continue
            end
        end
        # if energy of new configuration <= old configuration -> use new configuration
        # or if model didn't changed with propability
        steps += 1
        tries = 0
    end
end


# Function calculating energy only on 3x3 part of model
function energyPart(model::IsingFixed, pos1::Int, pos2::Int)
    sum1 = 0
    sum1 += model.J[1, 1] * model.sites[pos1, pos2] * model.sites[pos1-1, pos2-1] # left top
    sum1 += model.J[1, 2] * model.sites[pos1, pos2] * model.sites[pos1-1, pos2] # top
    sum1 += model.J[1, 3] * model.sites[pos1, pos2] * model.sites[pos1-1, pos2+1] # top right
    sum1 += model.J[2, 1] * model.sites[pos1, pos2] * model.sites[pos1, pos2-1] # left
    sum1 += model.J[2, 3] * model.sites[pos1, pos2] * model.sites[pos1, pos2+1] # right
    sum1 += model.J[3, 1] * model.sites[pos1, pos2] * model.sites[pos1+1, pos2-1] # left bot
    sum1 += model.J[3, 2] * model.sites[pos1, pos2] * model.sites[pos1+1, pos2] # bot
    sum1 += model.J[3, 3] * model.sites[pos1, pos2] * model.sites[pos1+1, pos2+1] # right bot
    # iterate all elements of sites and sum up: h[i] * sites[i]
    sum2 = 0
    for i in pos1-1:pos1+1, j in pos2-1:pos2+1
        sum2 += model.sites[i, j] * model.h[i, j]
    end
    # result
    return (- sum1) - sum2
end

# Function calculating energy only on 3x3 part of model
function energyPart(model::IsingPeriodic, pos1::Int, pos2::Int)
    # get proper intervals for border spins
    height_range = [pos1-1, pos1, pos1+1]
    width_range = [pos2-1, pos2, pos2+1]
    if pos1 == 1
        height_range = [model.height, pos1, pos1+1]
    end
    if pos1 == model.height
        height_range = [pos1-1, pos1, 1]
    end
    if pos2 == 1
        width_range = [model.width, pos2, pos2+1]
    end
    if pos2 == model.width
        width_range = [pos2-1, pos2, 1]
    end
    # calculation of energy
    sum1 = 0
    sum1 += model.J[1, 1] * model.sites[pos1, pos2] * model.sites[height_range[1], width_range[1]] # left top
    sum1 += model.J[1, 2] * model.sites[pos1, pos2] * model.sites[height_range[1], width_range[2]] # top
    sum1 += model.J[1, 3] * model.sites[pos1, pos2] * model.sites[height_range[1], width_range[3]] # top right
    sum1 += model.J[2, 1] * model.sites[pos1, pos2] * model.sites[height_range[2], width_range[1]] # left
    sum1 += model.J[2, 3] * model.sites[pos1, pos2] * model.sites[height_range[2], width_range[3]] # right
    sum1 += model.J[3, 1] * model.sites[pos1, pos2] * model.sites[height_range[3], width_range[1]] # left bot
    sum1 += model.J[3, 2] * model.sites[pos1, pos2] * model.sites[height_range[3], width_range[2]] # bot
    sum1 += model.J[3, 3] * model.sites[pos1, pos2] * model.sites[height_range[3], width_range[3]] # right bot
    # iterate all elements of sites and sum up: h[i] * sites[i]
    sum2 = 0
    for i in height_range, j in width_range
        sum2 += model.sites[i, j] * model.h[i, j]
    end
    # result
    return (- sum1) - sum2
end

#
# Vizualizace
#

"""
Vizualizační metoda pro vykreslení spinů v mřížce (obdélníků).

NOTE: Vaše vizualizace nemusí nutně vypadat přesně tak jako moje v `ising.ipynb`. Nebojte se být kreativní.
"""
function plot(model)
  Gray.(model.sites)
end


#
# Pomocné metody
#

"""
Nastaví spiny zcela náhodně.
"""
function randomize!(model::IsingModel)
    model.sites[:,:] = rand( (-1,1), model.height, model.width )
end

"""
Vrátí celkovou energii modelu v dané konfiguraci.
"""
function energy(model::IsingFixed)
    # iterate through elements in lines and for every one choose only some neigbors(left, left top, top, right top) if they are inside borders of array sites
    # and sum up: J[mutual position of neighbors] * neighbor1 * neighbor2
    sum1 = 0
    for i in 1:model.height, j in 1:model.width
        if i != 1 && j != model.width # right top
            sum1 += model.J[1, 3] * model.sites[i, j] * model.sites[i-1, j+1]
        end
        if i != 1 # top
            sum1 += model.J[1, 2] * model.sites[i, j] * model.sites[i-1, j]
        end
        if i != 1 && j != 1 # left top
            sum1 += model.J[1, 1] * model.sites[i, j] * model.sites[i-1, j-1]
        end
        if j != 1 # left
            sum1 += model.J[2, 1] * model.sites[i, j] * model.sites[i, j-1]
        end
    end
    # iterate all elements of sites and sum up: h[i] * sites[i]
    sum2 = 0
    for i in 1:model.height, j in 1:model.width
        sum2 += model.sites[i, j] * model.h[i, j]
    end
    # result
    return (- sum1) - sum2
end

function energy(model::IsingPeriodic)
    # iterate through elements in lines and for every one choose only some neigbors(left, left top, top, right top)
    # if it is border element, choose periodic neighbors
    # and sum up: J[mutual position of neighbors] * neighbor1 * neighbor2
    sum1 = 0
    for i in 1:model.height, j in 1:model.width
        tmp = 0
        # right top
        if i != 1 && j != model.width # inner left
            tmp = model.sites[i-1, j+1]
        elseif i == 1 && j != model.width # top line
            tmp = model.sites[model.height, j+1]
        elseif i != 1 && j == model.width # right column
            tmp = model.sites[i-1, 1]
        else # right top corner
            tmp = model.sites[model.width, 1]
        end
        sum1 += model.J[1, 3] * model.sites[i, j] * tmp
        
        # top
        if i != 1 # not top line
            tmp = model.sites[i-1, j]
        else # top line
            tmp = model.sites[model.height, j]
        end
        sum1 += model.J[1, 2] * model.sites[i, j] * tmp
        
        # left top
        if i != 1 && j != 1 # inner right
            tmp = model.sites[i-1, j-1]
        elseif i == 1 && j != 1 # top line
            tmp = model.sites[model.height, j-1]
        elseif i != 1 && j == 1 # left column
            tmp = model.sites[i-1, model.width]
        else # left top corner
            tmp = model.sites[model.height, model.width]
        end
        sum1 += model.J[1, 1] * model.sites[i, j] * tmp
        
        # left
        if j != 1 # not left column
            tmp = model.sites[i, j-1]
        else # left column
            tmp = model.sites[i, model.width]
        end
        sum1 += model.J[2, 1] * model.sites[i, j] * tmp
    end
    # iterate all elements of sites and sum up: h[i] * sites[i]
    sum2 = 0
    for i in 1:model.height, j in 1:model.width
        sum2 += model.sites[i, j] * model.h[i, j]
    end
    # result
    return (- sum1) - sum2
end

end # module
