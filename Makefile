all: day11 day14 day15 day16 day17 day19 day20 day21 day22 day23 day25

day11: day11.c
	cc day11.c -o day11

day14: day14.c
	cc day14.c -o day14

day15: day15.c
	cc day15.c -ggdb -o day15

day16: day16.c
	cc day16.c -ggdb -lm -o day16

day17: day17.c
	cc day17.c -O3 -o day17

day19-gen: day19-gen.c
	cc day19-gen.c -ggdb -lm -O3 -o day19-gen

day19: day19-gen

day20: day20.c
	cc day20.c -ggdb -o day20

day21: day21.c
	cc day21.c -ggdb -O3 -o day21

day23: day23.c
	cc day23.c -ggdb -O3 -o day23

day22: day22.c
	cc day22.c -O3 -o day22

day25: day25.c
	cc day25.c -ggdb -O3 -o day25
