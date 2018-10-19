// 
// pong.cpp: Draw Squares - 2 paddles and a ball
// - = faster, + = slower, r = reset
//
#include <iostream>
#include <windows.h>  // For MS Windows
#include <GL/glut.h>  // GLUT, includes glu.h and gl.h
#include <chrono>
#include <thread>


#define SIZEX 1000  // size of the graphical window to use
#define SIZEY 1000  // define constants rather than use the numbers in the code directly

// keys for up and down
#define LPUP 'w'
#define LPDOWN 's'
#define RPUP 'o'
#define RPDOWN 'l'


// left paddle variables
float lpx;
float lpy;
float lpxs;
float lpys;
// right paddle variables
float rpx;
float rpy;
float rpxs;
float rpys;
float paddlestep;

// squares variables
float x;
float y;
float xspeed;
float yspeed;
float xsize;
float ysize;
int frames = 50;


void initvars() {

	//left paddle initial position
	lpx = -1.0 + 0.05;
	lpy = 0 - 0.05;
	lpxs = 0.02;
	lpys = 0.1;
	//right paddle initial position
	rpx = 1.0 - 0.05;
	rpy = 0 - 0.05;
	rpxs = 0.02;
	rpys = 0.1;

	paddlestep = 0.02;

	// ball
	x = -1.0f;
	y = -1.0f;
	xsize = 0.02f;
	ysize = 0.02f;
	xspeed = 0.01f;
	yspeed = 0.005f;
	frames = 50;

}


// Handler for window-repaint event. Call back when the window first appears and whenever the window needs to be re-painted. 
void display() {
	int i;

	glClearColor(0.0f, 0.0f, 0.0f, 1.0f); // Set background color to black and opaque
	glClear(GL_COLOR_BUFFER_BIT);         // Clear the color buffer

	// Draw ball
	glBegin(GL_QUADS);              // Each set of 4 vertices form a quad
	glColor3f(1.0f, 0.0f, 0.0f);    // Red
	glVertex2f(x, y);         // x, y
	glVertex2f(x, y + ysize);
	glVertex2f(x + xsize, y + ysize);
	glVertex2f(x + xsize, y);
	glEnd();
	
	// draw left paddle
	glBegin(GL_QUADS);              // Each set of 4 vertices form a quad
	glColor3f(1.0f, 0.0f, 0.0f);    // Red
	glVertex2f(lpx, lpy);         // x, y
	glVertex2f(lpx, lpy + lpys);
	glVertex2f(lpx + lpxs, lpy + lpys);
	glVertex2f(lpx + lpxs, lpy);
	glEnd();

	// draw right paddle
	glBegin(GL_QUADS);              // Each set of 4 vertices form a quad
	glColor3f(1.0f, 0.0f, 0.0f);    // Red
	glVertex2f(rpx, rpy);         // x, y
	glVertex2f(rpx, rpy + rpys);
	glVertex2f(rpx + rpxs, rpy + rpys);
	glVertex2f(rpx + rpxs, rpy);
	glEnd();

	glFlush();  // Render now
}


void idleDisplay() {
	int i, j;

	// work happens here -
	std::this_thread::sleep_for(std::chrono::milliseconds(frames));
	// move the ball
	x = x + xspeed;
	y = y + yspeed;

	// are we over the edge?
	if (x > 1.0) {
		x = -1.0;
	}
	else {
		if (x < -1.0) {
			x = 1.0;
		}
	}
	if (y > 1.0) {
		y = -1.0;
	}
	else {
		if (y < -1.0) {
			y = 1.0;
		}
	}
	
	// is there a collsion, then invert the speed
	// if square i origin point is within the area of the square[j] invert speeds
	// right paddle
	if (x >= rpx && x <= rpx + rpxs ) {
		if (y >= rpy && y <= rpy+ rpys) {
			if (rand() % 100 > 50) {
				xspeed = xspeed * -1;
			}
			else {
				yspeed = yspeed * -1;
			}
			if (rand() % 100 < 50) {
				xspeed = xspeed * -1;
			}
			else {
				yspeed = yspeed * -1;
			}
		}
	}
	// left paddle
	if (x >= lpx && x <= lpx + lpxs) {
		if (y >= lpy && y <= lpy + lpys) {
			if (rand() % 100 > 50) {
				xspeed = xspeed * -1;
			}
			else {
				yspeed = yspeed * -1;
			}
			if (rand() % 100 < 50) {
				xspeed = xspeed * -1;
			}
			else {
				yspeed = yspeed * -1;
			}
		}
	}
	display();
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


void MoveRightPaddle(char upordown) {
	if (upordown == RPUP) {
		rpy = rpy + paddlestep;
	}
	else {
		rpy = rpy - paddlestep;
	}

	// check for overflow - we wrap, but maybe we shoudl just stopi, e. just keep at the value?
	if (rpy > 1.0) { rpy = -1.0; }
	if (rpy < -1.0) { rpy = 1.0; }
}


void processNormalKeys(unsigned char key, int a, int b) {
	int i, j;

	// work happens here -
	std::cout << "in Keys\n";
	if (key == LPUP || key == LPDOWN) {
		MoveLeftPaddle(key);
	}
	if (key == RPUP || key == RPDOWN) {
		MoveRightPaddle(key);
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
	glutIdleFunc(idleDisplay);                      // Register display callback handler for window re-paint
	glutMainLoop();                                 // Enter the infinitely event-processing loop
	return 0;
}
