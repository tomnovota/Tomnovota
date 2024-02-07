using IntervalArithmetic
using Test

@testset "IntervalArithmetic.jl" begin
    x = Interval(2.0, 5.0)
    y = Interval(-3., -1.)

    # Plus
    res = x + y
    @test res.x1 == -1. && res.x2 == 4.

    # Minus
    res = x - y
    @test res.x1 == 3. && res.x2 == 8.

    # Multiple
    res = x * y
    @test res.x1 == -15. && res.x2 == -2.

    # Divide
    res = x / y
    @test res.x1 == -5. && res.x2 == -0.6666666666666666

    # Not valid interval
    @test_throws ArgumentError Interval(5.0, -2.)

    # Divide by zero
    @test_throws ArgumentError 5. / Interval(-5., 1.)
end
