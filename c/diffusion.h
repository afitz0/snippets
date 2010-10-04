#include <math.h>
#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>

#define min_pxcor -29
#define max_pxcor 30
#define min_pycor -29
#define max_pycor 30

#define PI M_PI

#define particle_number 225

int ITERATIONS = 5;

struct particle {
	int xcor;
	int ycor;
};

struct patch {
	int pcolor;
	struct particle *occupant;
};

struct patch *world[max_pxcor*2][max_pycor*2];
struct particle *particles[particle_number];

enum { RED, GREEN, CYAN, YELLOW, ANY };

void sprout_particle(int, int);
void set_pcolor(int, int, int);
void forward(struct particle *);
bool particle_here(int, int);
bool particle_color_here(int, int, int);
void clear_all();
void check();
void setup_quadrants();
void setup_particles();
void go();
void setup();
void move();
void do_plot();

void sprout_particle(int x, int y) {
	x += max_pxcor - 1;
	y += max_pycor - 1;

	for (int i = 0; i < particle_number; ++i) {
		if (particles[i]->xcor == -1) {
			particles[i]->xcor = x;
			particles[i]->ycor = y;
			world[x][y]->occupant = particles[i];
			break;
		}
	}
}

void clear_all() {
	for (int i = 0; i < particle_number; ++i) {
		particles[i] = (struct particle*)malloc(sizeof(struct particle));
		particles[i]->xcor = -1;
	}

	for (int x = 0; x < max_pxcor*2; ++x)
		for (int y = 0; y < max_pycor*2; ++y) {
			world[x][y] = (struct patch*)malloc(sizeof(struct patch));
			world[x][y]->pcolor = ANY;
		}
}

void set_pcolor(int color, int x, int y) {
	x += max_pxcor - 1;
	y += max_pycor - 1;

	world[x][y]->pcolor = color;
}

bool particle_here(int x, int y) {
	return particle_color_here(x, y, ANY);
}

bool particle_color_here(int x, int y, int color) {
	x += max_pxcor - 1;
	y += max_pycor - 1;

	struct particle *part = world[x][y]->occupant;

	if (part != NULL) {
		if (color == ANY)
			return true;
		if (world[x][y]->pcolor == color)
			return true;
	}

	return false;
}

void forward(struct particle *part) {
	int dx, dy, newx, newy, heading;

	int hlist[4] = {0, 90, 180, 270};
	int hlist_rand[4] = {-1, -1, -1, -1};

	for (int i = 0; i < 4; ++i) {
		int r = random() % 4;
		while (hlist_rand[r] != -1)
			r = random() % 4;
		hlist_rand[r] = hlist[i];
	}

	for (int i = 0; i < 4; ++i) {
		heading = hlist_rand[i];

		switch (heading) {
			case 0:
				dx = 0; dy = 1;
				break;
			case 90:
				dx = 1; dy = 0;
				break;
			case 180:
				dx = 0; dy = -1;
				break;
			case 270:
				dx = -1; dy = 0;
				break;
		}

		newx = part->xcor+dx;
		newy = part->ycor+dy;

		if (newx < 0)
			newx = 0;
		else if (newx >= max_pxcor*2)
			newx = max_pxcor*2-1;

		if (newy < 0)
			newy = 0;
		else if (newy >= max_pycor*2)
			newy = max_pycor*2-1;

		if (world[newx][newy]->occupant == NULL) {
			world[part->xcor][part->ycor]->occupant = NULL;
			part->xcor = newx;
			part->ycor = newy;
			world[newx][newy]->occupant = part;
			break;
		}
	}
}

void check() {
	int count=0;

	for (int i = 0; i < particle_number; ++i) {
		if (world[particles[i]->xcor][particles[i]->ycor]->occupant != particles[i]) {
			printf("****** FATAL: Particle %d is not where it thinks it is!!\n", i);
			exit(1);
		} if ((particles[i]->xcor < 0) || (particles[i]->xcor > max_pxcor*2-1) ||
			(particles[i]->ycor < 0) || (particles[i]->ycor > max_pycor*2-1)) {
			printf("****** FATAL: Particle %d is out of bounds!!\n", i);
			exit(1);
		}
	}

	for (int x = 0; x < max_pxcor*2; ++x) {
		for (int y = 0; y < max_pycor*2; ++y) {
			if (world[x][y]->occupant != NULL) count++;
		}
	}

	if (count != particle_number) {
		printf("****** FATAL: Particle count mismatch! (is %d; should be %d)\n", count, particle_number);
		exit(1);
	}
}
