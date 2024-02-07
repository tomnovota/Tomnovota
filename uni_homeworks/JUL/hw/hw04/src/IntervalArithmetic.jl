
module IntervalArithmetic

export Interval

""" 
Class Interval
""" 
struct Interval{T <: Real} <: Real
    x1::T
    x2::T

    function Interval(x::T, y::T) where {T <: Real}
        if x > y
            throw(ArgumentError("Left value is greater than right value!"))
        end

        return new{T}(x, y)
    end

    function Interval(x::T) where {T <: Real}
        return new{T}(x, x)
    end

    function Interval{T}(x::S) where {S <: Real, T <: Real}
        return new{T}(T(x), T(x))
    end
end

import Base.promote_rule, Base.convert

""" 
Interaction between implemented and native real numbers
""" 
promote_rule(::Type{Interval{T}}, ::Type{Interval{S}}) where { T <: Real, S <: Real } = Interval{promote_type(S, T)}

promote_rule(::Type{Interval{T}}, ::Type{S}) where { T <: Real, S <: Real } = Interval{promote_type(S, T)}

function convert(::Type{Interval{T}}, x::Interval{S}) where { T <: Real, S <: Real }
    return Interval(T(x.x1), T(x.x2))
end

function convert(::Type{Interval{T}}, x::S) where { T <: Real, S <: Real }
    return Interval(T(x), T(x))
end

import Base.+, Base.-, Base.*, Base./

"""
    +(::Interval{T}, ::Interval{T})

Adds up two Intervals and returns result as Interval.
""" 
function +(a::Interval{T}, b::Interval{T}) where { T <: Real }
    return Interval(a.x1 + b.x1, a.x2 + b.x2)
end

"""
    -(::Interval{T}, ::Interval{T})

Subtracts two Intervals and returns result as Interval.
""" 
function -(a::Interval{T}, b::Interval{T}) where { T <: Real }
    return Interval(a.x1 - b.x2, a.x2 - b.x1)
end

"""
    *(::Interval{T}, ::Interval{T})

Multiplicate two Intervals and returns result as Interval.
""" 
function *(a::Interval{T}, b::Interval{T}) where { T <: Real }
    result = [a.x1 * b.x1, a.x1 * b.x2, a.x2 * b.x1, a.x2 * b.x2]
    return Interval(minimum(result), maximum(result))
end

"""
    /(::Interval{T}, ::Interval{T})

Divide two Intervals and returns result as Interval.
""" 
function /(a::Interval{T}, b::Interval{T}) where { T <: Real }
    if b.x1 * b.x2 <= 0
        throw(ArgumentError("You can't divide by zero!"))
    end

    return a * Interval(1 / b.x2, 1 / b.x1)
end

""" 
Output operation
"""
function Base.show(io::IO, a::Interval{T}) where { T <: Real }
    if a.x1 != a.x2
        print(io, "𝛂 ∈ ", "⟨", a.x1, ", ", a.x2, "⟩")
    else
        print(io, "𝛂 = ", a.x1)
    end
end

import Base.in



"""
    in(::Interval{T}, ::Interval{T})

This function returns fact if given value is in interval or is not.
"""
function in(item::T, interval::Interval{T}) where T <: Real
    if interval.x1 < item < interval.x2
        println("Čísla se rovnají.")
    else
        println("Čísla se nerovnají.")

    end
end

end # module
