onbreak {quit -force}
onerror {quit -force}

asim -t 1ps +access +r +m+multiplier -L xil_defaultlib -L secureip -O5 xil_defaultlib.multiplier

do {wave.do}

view wave
view structure

do {multiplier.udo}

run -all

endsim

quit -force
