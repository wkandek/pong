// 
// gapp1.cpp: Draw Squares - Step 7 - 3 squares lower left hand corner, plus keyboard with move, wrap on edge, sizes different, collision
// - = faster, + = slower, r = reset
//
#include <iostream>
#include <windows.h>  // For MS Windows
#include <GL/glut.h>  // GLUT, includes glu.h and gl.h
#include <chrono>
#include <thread>


#define SIZEX 1000  // size of the graphical window to use
#define SIZEY 1000  // define constants rather than use the numbers in the code directly


#define SQUARES 3

// keys for up and down
#define LPUP 'w'
#define LPDOWN 's'

// left paddle variables
float lpx;
float lpy;
float lpxs;
float lpys;
float paddlestep;

// squares variables
float x[SQUARES];
float y[SQUARES];
float xspeed[SQUARES];
float yspeed[SQUARES];
float xsize[SQUARES];
float ysize[SQUARES];
int frames = 50;

int initvars() {

	//left paddle initial position
	lpx = -1.0 + 0.05;
	lpy = 0 - 0.05;
	lpxs = 0.01;
	lpys = 0.1;
	paddlestep = 0.02;

	// 3 squares
	x[0] = -1.0f;
	y[0] = -1.0f;
	xsize[0] = 0.02f;
	ysize[0] = 0.02f;
	xspeed[0] = 0.01f;
	yspeed[0] = 0.005f;
	x[1] = 0.99f;
	y[1] = 0.99f;
	xsize[1] = 0.01f;
	ysize[1] = 0.01f;
	xspeed[1] = -0.01f;
	yspeed[1] = -0.01f;
	x[2] = 0.99f;
	y[2] = -0.99f;
	xsize[2] = 0.03f;
	ysize[2] = 0.03f;
	xspeed[2] = -0.01f;
	yspeed[2] = 0.01f;
	frames = 50;

	return 0;
}
// Handler for window-repaint event. Call back when the window first appears and
// whenever the window needs to be re-painted. 
void display() {
	int i;

	glClearColor(0.0f, 0.0f, 0.0f, 1.0f); // Set background color to black and opaque
	glClear(GL_COLOR_BUFFER_BIT);         // Clear the color buffer

	// Draw all squares 
	for (i = 0; i < SQUARES; i++) {
		glBegin(GL_QUADS);              // Each set of 4 vertices form a quad
		glColor3f(1.0f, 0.0f, 0.0f);    // Red
		glVertex2f(x[i], y[i]);         // x, y
		glVertex2f(x[i], y[i] + ysize[i]);
		glVertex2f(x[i] + xsize[i], y[i] + ysize[i]);
		glVertex2f(x[i] + xsize[i], y[i]);
		glEnd();
	}

	// draw left paddle
	glBegin(GL_QUADS);              // Each set of 4 vertices form a quad
	glColor3f(1.0f, 0.0f, 0.0f);    // Red
	glVertex2f(lpx, lpy);         // x, y
	glVertex2f(lpx, lpy + lpys);
	glVertex2f(lpx + lpxs, lpy + lpys);
	glVertex2f(lpx + lpxs, lpy);
	glEnd();

	glFlush();  // Render now
}


void display2() {
	int i, j;

	// work happens here -
	if (1) {
		std::this_thread::sleep_for(std::chrono::milliseconds(frames));
		// move the square to the top and right
		for (i = 0; i < SQUARES; i++) {
			x[i] = x[i] + xspeed[i];
			y[i] = y[i] + yspeed[i];
		}

		// are we over the edge?
		for (i = 0; i < SQUARES; i++) {
			if (x[i] > 1.0) {
				x[i] = -1.0;
			}
			else {
				if (x[i] < -1.0) {
					x[i] = 1.0;
				}
			}
			if (y[i] > 1.0) {
				y[i] = -1.0;
			}
			else {
				if (y[i] < -1.0) {
					y[i] = 1.0;
				}
			}
		}

		// is there a collsion, then invert the speed
		for (i = 0; i < SQUARES; i++) {
			for (j = 0; j < SQUARES; j++) {
				// if it is the same square skip
				if (i != j) {
					// if square i origin point is within the area of the square[j] invert speeds
					// then stop processing j's - one collision is enough
					if (x[i] >= x[j] && x[i] <= x[j] + xsize[j]) {
						if (y[i] >= y[j] && y[i] <= y[j] + ysize[j]) {
							if (rand() % 100 > 50) {
								xspeed[i] = xspeed[i] * -1;
							}
							else {
								yspeed[i] = yspeed[i] * -1;
							}
							if (rand() % 100 < 50) {
								xspeed[j] = xspeed[j] * -1;
							}
							else {
								yspeed[j] = yspeed[j] * -1;
							}
							break;
						}
					}
				}
			}
		}

		display();
	}
}


void MoveLeftPaddle(char upordown) {
	if (upordown == LPUP ) {
		lpy = lpy + paddlestep;
	}
	else {
		lpy = lpy - paddlestep;
	}

	// check for overflow - we wrap, but maybe we shoudl just stopi, e. just keep at the value?
	if (lpy > 1.0) { lpy = -1.0; }
	if (lpy < -1.0) { lpy = 1.0; }
}


void processNormalKeys(unsigned char key, int a, int b) {
	int i, j;

	// work happens here -
	std::cout << "in Keys\n";
	if (key == LPUP || key == LPDOWN) {
		MoveLeftPaddle(key);
	}
	if (key == 'r') {
		initvars();
	}
	if (key == '+') {
		frames++;
	}
	if (key == '-') {
		frames--;
	}
	if (key == 32) {
		std::cout << "32 pressed\n";
		// move the square to the top and right
		for (i = 0; i < 3; i++) {
			x[i] = x[i] + xspeed[i];
			y[i] = y[i] + yspeed[i];
			std::cout << i << "\n";
			std::cout << x[i] << "\n";
			std::cout << xspeed[i] << "\n";
			std::cout << y[i] << "\n";
			std::cout << yspeed[i] << "\n";
		}

		// are we over the edge?
		for (i = 0; i < 4; i++) {
			if (x[i] > 1.0) {
				x[i] = -1.0;
			}
			else {
				if (x[i] < -1.0) {
					x[i] = 1.0;
				}
			}
			if (y[i] > 1.0) {
				y[i] = -1.0;
			}
			else {
				if (y[i] < -1.0) {
					y[i] = 1.0;
				}
			}
		}

		// is there a collsion, then invert the speed
		for (i = 0; i < 4; i++) {
			for (j = 0; j < 4; j++) {
				// if it is the same square skip
				if (i != j) {
					// if square i origin point is within the area of the square[j] invert speeds
					// then stop processing j's - one collision is enough
					if (x[i] >= x[j] && x[i] <= x[j] + xsize[j]) {
						if (y[i] >= y[j] && y[i] <= y[j] + ysize[j]) {
							if (rand() % 100 > 50) {
								xspeed[i] = xspeed[i] * -1;
							}
							else {
								yspeed[i] = yspeed[i] * -1;
							}
							if (rand() % 100 < 50) {
								xspeed[j] = xspeed[j] * -1;
							}
							else {
								yspeed[j] = yspeed[j] * -1;
							}
							break;
						}
					}
				}
			}
		}

		display();
	}
}



/* Main function: GLUT runs as a console application starting at main()  */
int main(int argc, char** argv) {


	initvars();
	glutInit(&argc, argv);                           // Initialize GLUT

	glutCreateWindow("OpenGL Square Demo - Step 5"); // Create a window with the given title
	glutInitWindowSize(SIZEX, SIZEY);                // Set the window's initial width & height
	glutInitWindowPosition(0, 0);                    // Position the window's initial top-left corner
	glutKeyboardFunc(processNormalKeys);             // register the callback for keypress


	glutDisplayFunc(display);                       // Register display callback handler for window re-paint
	glutIdleFunc(display2);                          // Register display callback handler for window re-paint
	glutMainLoop();                                 // Enter the infinitely event-processing loop
	return 0;
}
