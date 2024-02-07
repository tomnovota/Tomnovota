using IntervalArithmetic
using Documenter

DocMeta.setdocmeta!(IntervalArithmetic, :DocTestSetup, :(using IntervalArithmetic); recursive=true)

makedocs(;
    modules=[IntervalArithmetic],
    authors="Tomáš Novotný <novott37@fit.cvut.cz> and contributors",
    repo="https://gitlab.fit.cvut.cz/novott37/IntervalArithmetic.jl/blob/{commit}{path}#{line}",
    sitename="IntervalArithmetic.jl",
    format=Documenter.HTML(;
        prettyurls=get(ENV, "CI", "false") == "true",
        canonical="https://novott37.gitlab.io/IntervalArithmetic.jl",
        edit_link="main",
        assets=String[],
    ),
    pages=[
        "Home" => "index.md",
    ],
)
