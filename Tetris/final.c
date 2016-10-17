// Grace Bushong and Erin Turley
// December 13, 2015
// Final Project: Tetris

#include <stdio.h>
#include "gfx4.h"
#include <math.h>
#include <stdlib.h>
#include <unistd.h>
#include <time.h>
#define XSIZE 700
#define YSIZE 700
#define SIDE 50
#define PI 3.141592654 //M_PI

typedef struct Shape_s {
	int color[3];
	int rotations[16][3];
	int ymax[4];
	int xmax[4];
}	Shape;

void drawGrid();
void redraw(int squares[][11], int colors[][11], int allcolors[][4]);
void arrayCopy(int sourcecolor[3], int sourcerot[4][3], int destcolor[3], int destrot[4][3]);
int checkBorders(Shape *figure, int x, int y, int rot, int squares[][11]);
void checkRows(int squares[][11], int colors[][11], Shape *pline, int allcolors[][4]);
void arrArr(int squares[][11], int colors[][11], int row);
int drawShape(Shape *figure, int x, int y, int rot, int squares[][11]);
void drawBlank(Shape *figure, int x, int y, int rot);
int moveShape(Shape *figure, int x, int y, int rot, int ymax, int squares[][11], int color, int colors[][11]);
void getRotations(Shape *figure);
int calcRotation(int rot);
int changeX(int x, int y);
int changeY(int x, int y);
void snowflake(int, int, int);

int main()
{
	Shape line, cube, tshape, lshape, rshape, zshape, sshape;
	Shape *pline = &line, *pcube = &cube, *ptshape = &tshape,
	 *plshape = &lshape, *prshape = &rshape, *pzshape = &zshape, *psshape = &sshape;

	int linerotations[4][3] = {{0, 0}, {-1, 0}, {-2, 0}, {1, 0}};
	int cuberotations[4][3] = {{0, 0}, {-1, 0}, {-1, 1}, {0, 1}};
	int rshaperotations[4][3] = {{0, 0}, {-1, 0}, {-1, 1}, {-1, 2}};
	int lshaperotations[4][3] = {{0, 0}, {-1, 0}, {0, 1}, {0, 2}};
	int zshaperotations[4][3] = {{0, 0}, {-1, 0}, {0, 1}, {1, 1}};
	int sshaperotations[4][3] = {{0, 0}, {1, 0}, {0, 1}, {-1, 1}};
	int tshaperotations[4][3] = {{0, 0}, {-1, 0}, {1, 0}, {0, 1}};

	int allcolors[7][4] = {{0, 255, 255}, {255, 255, 0}, {255, 0, 0}, {0, 51, 0}, {204, 0, 0}, {0, 102, 0}, {192, 192, 192}};

	arrayCopy(allcolors[0], linerotations, pline->color, pline->rotations);
	arrayCopy(allcolors[1], cuberotations, pcube->color, pcube->rotations);
	arrayCopy(allcolors[2], rshaperotations, prshape->color, prshape->rotations);
	arrayCopy(allcolors[3], lshaperotations, plshape->color, plshape->rotations);
	arrayCopy(allcolors[4], zshaperotations, pzshape->color, pzshape->rotations);
	arrayCopy(allcolors[5], sshaperotations, psshape->color, psshape->rotations);
	arrayCopy(allcolors[6], tshaperotations, ptshape->color, ptshape->rotations);

	srand(time(NULL));
	char key = 'a';
	int c, y, ymax, x, rot, squares[12][11] = {0}, colors[12][11] = {0};
	int g, h, row=0, end = 0, deltat = 1000000;
	gfx_open(XSIZE, YSIZE, "Tetrismas");
	drawGrid();
	ymax = 12;
	getRotations(pline);
	getRotations(pcube);
	getRotations(prshape);
	getRotations(plshape);
	getRotations(pzshape);
	getRotations(psshape);
	getRotations(ptshape);

	while(key != 'q'){
		checkRows(squares, colors, pline, allcolors);
		c = gfx_wait();
		y = 0;
		x = 5;
		rot = 1;

		switch(c){
			case '0':
				end = moveShape(pline, x, y, 1, ymax, squares, 0, colors);
				break;
			case '1':
				end = moveShape(pcube, x, y, 1, ymax, squares, 1, colors);
				break;
			case '2':
				end = moveShape(prshape, x, y, 1, ymax, squares, 2, colors);
				break;
			case '3':
				end = moveShape(plshape, x, y, 1, ymax, squares, 3, colors);
				break;
			case '4':
				end = moveShape(pzshape, x, y, 1, ymax, squares, 4, colors);
				break;
			case '5':
				end = moveShape(psshape, x, y, 1, ymax, squares, 5, colors);
				break;
			case '6':
				end = moveShape(ptshape, x, y, 1, ymax, squares, 6, colors);
				break;
		}
		usleep(deltat);
		if (end){
			gfx_color(255, 255, 255);
			gfx_text(550, 400, "Game Over!");
			break;
		}
	}

}

void drawGrid()
{
	int i, j;
	for (i = 0; i < 10; i++){
		for (j = 0; j < 12; j++){
			gfx_rectangle(50*i, 50*j, SIDE, SIDE);
		}
	}
	gfx_changefont("lucidasans-24");
	gfx_text(550, 100, "TETRIS");

	snowflake(100, 650, 30);
	snowflake(250, 650, 30);
	snowflake(400, 650, 30);
	snowflake(550, 650, 30);
}
void snowflake (int x1, int y1, int len)
{
	if(len<1) return;
	gfx_line(x1, y1, x1 + len*cos(0), y1 + len*sin(0));
	gfx_line(x1, y1, x1 + len*cos(2*PI/5), y1 + len*sin(2*PI/5));
	gfx_line(x1, y1, x1 + len*cos(4*PI/5), y1 + len*sin(4*PI/5));
	gfx_line(x1, y1, x1 + len*cos(6*PI/5), y1 + len*sin(6*PI/5));
	gfx_line(x1, y1, x1 + len*cos(8*PI/5), y1 + len*sin(8*PI/5));

	snowflake(x1+len*cos(0), y1+len*sin(0), len/3);
	snowflake(x1+len*cos(2*PI/5), y1+len*sin(2*PI/5), len/3);
	snowflake(x1+len*cos(4*PI/5), y1+len*sin(4*PI/5), len/3);
	snowflake(x1+len*cos(6*PI/5), y1+len*sin(6*PI/5), len/3);
	snowflake(x1+len*cos(8*PI/5), y1+len*sin(8*PI/5), len/3);
}

void arrayCopy(int sourcecolor[3], int sourcerot[4][3], int destcolor[3], int destrot[4][3])
{
	int i, j;
	for (i= 0; i < 3; i++){
		destcolor[i] = sourcecolor[i];
	}
	for (i = 0; i < 4; i++){
		for (j = 0; j < 3; j++){
			destrot[i][j] = sourcerot[i][j];
		}
	}
}

int calcRotation(int rot)
{
	int i, k;
	if (rot >= 4){
		if (rot%4 == 0){
			i = 3;
		}
		else if(rot%3 == 0 && rot != 6 && rot != 9){
			i = 2;
		}
		else if(rot%2 == 0){
			i = 1;
		}
		else{
			i = 0;
		}
	}
	else i = rot-1;
	i = i * 4;
	return i;
}

void checkRows(int squares[][11], int colors[][11], Shape *figure, int allcolors[][4])
{
	int i, j, count;
	for (i = 0; i < 12; i++){
		count = 0;
		for (j = 0; j < 10; j++){
			if (squares[i][j] == 1){
				count = count + 1;
			}
			else break;
		}
		if (count == 10){
			drawBlank(figure, 2, i, 1);
			drawBlank(figure, 6, i, 1);
			drawBlank(figure, 8, i, 1);
			for (j = 0; j < 10; j++){
				squares[i][j] = 0;
			}
			arrArr(squares, colors, i);
			redraw(squares, colors, allcolors);
		}
	}
}

void redraw(int squares[][11], int colors[][11], int allcolors[][4])
{
	gfx_clear();
	drawGrid();
	int i, j;
	for (i = 0; i < 12; i++){
		for (j = 0; j < 10; j++){
			if (squares[i][j]){
				gfx_color(allcolors[colors[i][j]][0], allcolors[colors[i][j]][1], allcolors[colors[i][j]][2]);
				gfx_fill_rectangle(50*j, 50*i, SIDE, SIDE);
			}
		}
	}
}

void arrArr(int squares[][11], int colors[][11], int row)
{
	int i, j;
	for (i = row; i > 0; i--){
		for (j = 0; j < 10; j++){
			squares[i][j] = squares[i-1][j];
			colors[i][j] = colors[i-1][j];
		}
	}
	for (j = 0; j < 10; j++){
		squares[0][j] = 0;
		colors[0][j] = 0;
	}
}

int checkBorders(Shape *figure, int x, int y, int rot, int squares[][11])
{
	int i, k, x1, y1;
	for (k = 0; k < 4; k++){
		x1 = figure->rotations[k+rot][0] + x;
		y1 = figure->rotations[k+rot][1] + y;
		if (x1 >= 10 || x1 < 0) return 0;
		if (y1 >= 12) return 2;
		if (squares[y1][x1] == 1) return 2;
	}
	return 1;
}

int drawShape(Shape *figure, int x, int y, int rot, int squares[][11])
{
	int k, y1, x1;
	gfx_color(figure->color[0], figure->color[1], figure->color[2]);
	
	for (k = 0; k < 4; k++){
		if (checkBorders(figure, x, y, rot, squares) == 1){
			x1 = figure->rotations[k+rot][0] + x;
			y1 = figure->rotations[k+rot][1] + y;
			gfx_fill_rectangle(50*(x1), 50*(y1), SIDE, SIDE);	
		}
		else if (checkBorders(figure, x, y, rot, squares) == 2) return 2;
		else return 0;
	}
	return 1;
}

void drawBlank(Shape *figure, int x, int y, int rot)
{
	int k, x1, y1;
	for (k = 0; k < 4; k++){
		gfx_color(0, 0, 0);
		x1 = figure->rotations[k+rot][0] + x;
		y1 = figure->rotations[k+rot][1] + y;
		gfx_fill_rectangle(50*(x1), 50*(y1), SIDE, SIDE);
	}

	for (k = 0; k < 4; k++){
		gfx_color(255, 255, 255);
		gfx_rectangle(50*(figure->rotations[k+rot][0] + x), 50*(figure->rotations[k+rot][1] + y), SIDE, SIDE);
	}
}

int moveShape(Shape *figure, int x, int y, int rot, int ymax, int squares[][11], int color, int colors[][11])
{
	int lastx = x, lasty = y, drawas, prevx = x, m=0, realtime = 1000000;
	char d;
	drawShape(figure, x, y, rot, squares);
	gfx_flush();
		for (y = 1; y < ymax; y++){
			usleep(realtime);
			drawBlank(figure, lastx, lasty, drawas);
			gfx_flush();
			if (gfx_event_waiting()){
				d = gfx_wait();
				if (d == 81){
					x = x - 1;
					lastx = x;
				}
				else if (d == 83){
					x = x + 1;
					lastx = x;
				}
				else if (d == 82){
					rot = rot + 1;
					drawas = calcRotation(rot);
				}
				else if (d == 84){
					realtime = realtime - 200000;
				}
			}
			m = drawShape(figure, x, y, drawas, squares);
			if(drawShape(figure, x, y, drawas, squares) != 1){
				lastx = prevx;
				x = prevx;
				y = lasty;
				drawShape(figure, x, y, drawas, squares);
				if (m == 2){
					lasty = y;
					prevx = x;
					break;
				}
			}
			gfx_flush();
			lasty = y;
			prevx = x;
		}
	if (y == 1){
		printf("Game Over");
		gfx_color(255, 255, 255);
		gfx_text(550, 400, "Game Over!");
		return 1;
	}
	int k, x1, y1;
	for (k = 0; k < 4; k++){
		x1 = figure->rotations[k+drawas][0] + prevx;
		y1 = figure->rotations[k+drawas][1] + lasty;
		squares[y1][x1] = 1;
		colors[y1][x1] = color;
	}
	return 0;
}

void getRotations(Shape *figure)
{
	int i, j, x, y, x0, y0;
	for (i = 4; i < 16; i++){
		x = figure->rotations[i-4][0];
		y = figure->rotations[i-4][1];
		figure->rotations[i][0] = changeX(x, y);
		figure->rotations[i][1] = changeY(x, y);
		if (i > 7 && i < 16){
			figure->rotations[i][0] = changeX(x, y) - 1;
		}
	}
}

int changeX(int x, int y)
{
	int x1;
	x1 = -y;
	return x1;
}

int changeY(int x, int y)
{
	int y1;
	y1 = x;
	return y1;
}