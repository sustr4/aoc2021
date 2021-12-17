all: day11 day14 day15 day16 day17

day11: day11.c
	cc day11.c -o day11

day14: day14.c
	cc day14.c -o day14

day15: day15.c
	cc day15.c -ggdb -o day15

day16: day16.c
	cc day16.c -ggdb -lm -o day16

day17: day17.c
	cc day17.c -o3 -o day17
