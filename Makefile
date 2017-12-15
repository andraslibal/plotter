
granular: granular.c
	gcc granular.c -o granular -lm -O3
plot: plot.c
	gcc plot.c -o plot -I /usr/include/X11 -L /usr/X11/lib -lX11 -lm
