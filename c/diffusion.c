/*
	Simple model of diffusion meant to mimic, as close as possible,
	to a commonly used NetLogo model. 

	2009 - Andrew Fitz Gibbon
*/
#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <time.h>
#include "diffusion.h"

int main (int argc, char ** argv) {
	if (argc == 2)
		ITERATIONS = atoi(argv[1]);
		
	srandom(time(NULL));

	setup();
	go();
}

void setup () {
	clear_all();

	setup_quadrants();
	setup_particles();
}

void go () {
	for (int i = 0; i < ITERATIONS; ++i) {
		do_plot();
		move();
		check();
	}
}

void setup_quadrants() {
	for (int pxcor = min_pxcor; pxcor <= max_pxcor; ++pxcor) {
		for (int pycor = min_pycor; pycor <= max_pycor; ++pycor) {
			if ((pxcor <= 0) && (pycor <= 0)) set_pcolor(RED, pxcor, pycor);
			if ((pxcor >  0) && (pycor <= 0)) set_pcolor(GREEN, pxcor, pycor);
			if ((pxcor <= 0) && (pycor >  0)) set_pcolor(CYAN, pxcor, pycor);
			if ((pxcor >  0) && (pycor >  0)) set_pcolor(YELLOW, pxcor, pycor);
		}
	}
}

void setup_particles() {
	for (int pxcor = min_pxcor; pxcor < 0; ++pxcor) {
		for (int pycor = max_pycor; pycor > 0; --pycor) {
			if (abs(min_pxcor - pxcor) > 1
				&& abs(min_pxcor - pxcor) < 2 + floor(sqrt(particle_number))
				&& abs(max_pycor - pycor) > 1
				&& abs(max_pycor - pycor) < 2 + floor(sqrt(particle_number)))
			{
				sprout_particle(pxcor, pycor);
			}
		}
	}
}

void move() {
	for (int i = particle_number-1; i >= 0; --i) {
		forward(particles[i]);
	}
}

void do_plot() {
	int count_red    = 0;
	int count_green  = 0;
	int count_cyan   = 0;
	int count_yellow = 0;

	for (int x = min_pxcor; x <= max_pxcor; ++x) {
		for (int y = min_pycor; y <= max_pycor; ++y) {
			if (particle_color_here(x, y, RED))    count_red++;
			if (particle_color_here(x, y, GREEN))  count_green++;
			if (particle_color_here(x, y, CYAN))   count_cyan++;
			if (particle_color_here(x, y, YELLOW)) count_yellow++;
		}
	}

	printf("%d,%d,%d,%d\n", count_cyan, count_green, count_red, count_yellow);
}
