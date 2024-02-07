"""

Úžasný modul počítající regresi pomocí Vážených Nejmenších Čtverců (WLS).

"""
module Helpers

using LinearAlgebra

export fitWLS, getMonth

"""

Výpočet koeficientů lineární kombinace funkcí ve vektoru `fs` pro popis
závislosti `ys` na `xs` s váhami `ws`.

"""
function fitWLS(xs::Vector{T}, ys::Vector{T}, ws::Vector{T}, fs::Vector{Function}) where { T <: Real }
  if length(xs) != length(ys)
    error("Délka `xs` a `ys` musí být stejná!")
  end

  if length(ws) != length(xs)
    error("Délka `ws` a `xs` a `ys` musí být stejná!")
  end

  # Příprava

  n = length(xs)
  m = length(fs)
  𝔽 = T[ fs[k](xs[i]) for k = 1:m, i = 1:n ]

  𝕎 = diagm(ws)

  # Výpočet

  α = (𝔽 * 𝕎 * transpose(𝔽)) \ (𝔽 * 𝕎 * ys)

  # Konec!

  return α
end

function getMonth(x::Int)
    arr = ["Leden", "Únor", "Březen", "Duben", "Květen", "Červen", "Červenec", "Srpen", "Září", "Říjen", "Listopad", "Prosinec"]
    return arr[x]
end
    
end # module
