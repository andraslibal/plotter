
granular: granular.c
	gcc granular.c -o granular -lm -O3
plot: plot.cpp
	g++ plot.cpp -o plot -lglut -lGL