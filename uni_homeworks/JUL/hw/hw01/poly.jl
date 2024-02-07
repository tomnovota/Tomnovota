###
#
# BI-JUL: 1. Domácí úkol v B221
#
# Podrobný popis zadání naleznete na Course pages nebo ve svém repozitáři.
#


#
# Typ `Polynomial{T}`
#


"""
Parametrický typ `Polynomial{T}`.
K vytvoření polynomu konstruktoru předáme pole:

    Polynomial([1, 2, 3])
q
Prvky v poly odpovídají koeficientům polynomu od nejnižšího stupně k největšímu.
Tj. volbě výše odpovídá polynom ``3x^2 + 2x + 1``.

Pole musí obsahovat alespoň jeden prvek, jinak nastane chyba.
Konstruktor správně určí stupeň zadaného polynomu (stupeň nulového polynomu
bereme jako ``-1``) a případně zahodí nepotřebné nulové koeficienty.
"""
struct Polynomial{ T <: Number }
  coefficients::Vector{T}
  degree::Int64

  function Polynomial(coefficients::Vector{T}) where { T <: Number }
    degree = length(coefficients) - 1

    if degree == -1
      throw(ArgumentError("You have to provide at least one coefficient!"))
    end

    while last(coefficients) == zero(T) && degree > 0
      pop!(coefficients)
      degree -= 1
    end

    if length(coefficients) == 1 && first(coefficients) == zero(T)
      degree = -1
    end

    return new{T}(coefficients, degree)
  end
end


#
# Porovnávání polynomů podle zvoleného datového typu.
#

import Base.==

function ==(p::Polynomial{T}, q::Polynomial{T}) where { T <: Number }
  # exact comparison
  p.coefficients == q.coefficients
end

function ==(p::Polynomial{T}, q::Polynomial{T}) where { T <: AbstractFloat }
  # approximate comparison
  p.coefficients ≈ q.coefficients
end

function ==(p::Polynomial{T}, q::Polynomial{S}) where { T <: Number, S <: Number }
  p.coefficients ≈ q.coefficients
end

function ==(p::Polynomial{T}, q::Polynomial{T}) where { T <: Rational }
  p.coefficients == q.coefficients
end



#
# Pěkná textová reprezentace polynomu.
#

"""
Ukázka výstupu:

    julia> Polynomial([1, 2, 3])
    + 3*x^2 + 2*x^1 + 1

    julia> Polynomial([1, 0, 3//2])
    + 3//2*x^2 + 1//1

    julia> Polynomial([1])
    1

    julia> Polynomial([0])
    0

"""
function Base.show(io::IO, p::Polynomial{T}) where { T <: Number }
  if p.degree <= 0
    return print(io, last(p.coefficients))
  end

  tokens = []

  for j in p.degree:-1:0
    c = p.coefficients[j + 1]

    if c < zero(T)
      append!(tokens, "- ", -c)
    elseif c > zero(T)
      append!(tokens, "+ ", c)
    end

    if j > 0 && c != zero(T)
      append!(tokens, "*x^", j, " ")
    end
  end

  print(io, tokens...)
end


#
# Algebraické operace `+` a `*`
#

import Base.*, Base.+

"""
Součin dvou polynomů.
"""
function *(p::Polynomial{T}, q::Polynomial{S}) where { T <: Number, S <: Number }
  deg = 0 # degree of element of p which is iterated
  res = Polynomial(zeros(Number, 1)) # initialize result
  for i in p.coefficients
    tmp = zeros(Number, deg) # fills 0's to positions which can't be accessed by the particular multiplication
    res += Polynomial(append!(tmp, [i*j for j in q.coefficients])) # multiply iterated element of p with all elements of q
    deg += 1
  end
  return res
end

function *(p::Polynomial{T}, q::Polynomial{T}) where { T <: Number }
  deg = 0 # degree of element of p which is iterated
  res = Polynomial(zeros(T, 1)) # initialize result
  for i in p.coefficients
    tmp = zeros(T, deg) # fills 0's to positions which can't be accessed by the particular multiplication
    res += Polynomial(append!(tmp, [i*j for j in q.coefficients])) # multiply iterated element of p with all elements of q
    deg += 1
  end
  return res
end

"""
Součet dvou polynomů.
"""
function +(p::Polynomial{T}, q::Polynomial{S}) where { T <: Number, S <: Number }
  x = p.degree >= q.degree ? p.coefficients : q.coefficients # choose maximum size array
  y = x == q.coefficients ? p.coefficients : q.coefficients # the minimum size array
  ly = length(y)
  u = zeros(Number, 0)
  u = append!(u, [x[i]+y[i] for i in 1:ly])
  return Polynomial(append!(u, x[ly+1:end])) # sums common positions and append values of bigger polynom
end

function +(p::Polynomial{T}, q::Polynomial{T}) where { T <: Number }
  x = p.degree >= q.degree ? p.coefficients : q.coefficients # choose maximum size array
  y = x == q.coefficients ? p.coefficients : q.coefficients # the minimum size array
  ly = length(y)
  u = zeros(T, 0)
  u = append!( u, x[i]+y[i] for i in 1:ly )
  println(p, ",", q)
  #u = [ T(x[i])+T(y[i]) for i in 1:ly ]
  return Polynomial(append!(u, x[ly+1:end])) # sums common positions and append values of bigger polynom
end


"""Opačný polynom"""

import Base.-

function -(p::Polynomial{T}) where { T <: Number }
  return Polynomial([ -x for x in p.coefficients ])
end

"""Odečítání polynomů"""
function -(p::Polynomial{T}, q::Polynomial{S} ) where { T <: Number, S <: Number }
  x = -q
  return p + x
end

function -(p::Polynomial{T}, q::Polynomial{T} ) where { T <: Number }
  x = -q
  return p + x
end

#
# Výpočet funkční hodnoty polynomu
#

"""
Díky této metodě můžeme polynomy vyhodnocovat, tedy počítat jejich funkční hodnoty.
Například:

    julia> p = Polynomial([1, 0, 3//2])
    + 3//2*x^2 + 1//1

    julia> p(-4//5)
    49//25

"""
function (p::Polynomial{T})(x::S) where { T <: Number, S <: Number }
  return sum(p.coefficients[j] * ( x ^ (j-1) ) for j in 1:(p.degree + 1))
end

#
# Dělení polynomu polynomem
#

"""
Divide polynomial `p` by polynomial `q` and return the quotient and remainder.
"""
function pdiv(p::Polynomial{T}, q::Polynomial{T}) where { T <: Number }
  if q.degree == -1
    throw(ArgumentError("It is not possible to divide by 0!"))
  end
  
  x = p # copy of numerator
  res = Polynomial(zeros(Number, 1)) # initialize denumerator
  while x.degree >= q.degree 
    v = last(x.coefficients) / last(q.coefficients) # divide highest member of num / highest den
    u = Polynomial(append!(zeros(Number, x.degree - q.degree), v)) # gets another member of result
    res = res + u # update result
    x = x - (u*q) # update numerator
  end

  return res, x
end

function pdiv(p::Polynomial{T}, q::Polynomial{T}) where { T <: Rational }
  if q.degree == -1
    throw(ArgumentError("It is not possible to divide by 0!"))
  end
  
  x = p # copy of numerator
  res = Polynomial(zeros(T, 1)) # initialize denumerator
  while x.degree >= q.degree 
    v = last(x.coefficients) / last(q.coefficients) # divide highest member of num / highest den
    u = Polynomial(append!(zeros(T, x.degree - q.degree), v)) # gets another member of result
    res = res + u # update result
    x = x - (u*q) # update numerator
  end

  return res, x
end

Polynomial([1, 2, 4])