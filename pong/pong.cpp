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
#define RESET 'r'
#define SLOWER '-'
#define FASTER '+'
#define NEWBALL ' '



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

// square variables
float x;
float y;
float xspeed;
float yspeed;
float xsize;
float ysize;

// animation speed
int frames;
int volleys;
int rightscore;
int leftscore;


void initvars() {

	//left paddle initial position
	lpx = 70;
	lpy = 500 - 50;
	lpxs = 30;
	lpys = 100;
	//right paddle initial position
	rpx = 900;
	rpy = 500 - 50;
	rpxs = 30;
	rpys = 100;

	paddlestep = 20;

	// ball
	x = 0;
	y = 0;
	xsize = 20;
	ysize = 20;
	xspeed = 10;
	yspeed = 5;
	
	frames = 50;
	volleys = 0;
	leftscore = 0;
	rightscore = 0;
}


// Handler for window-repaint event. Call back when the window first appears and whenever the window needs to be re-painted. 
void display() {
	char vstr[20];
	int i, width, xpos;

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

	// Draw Line 
	glLineStipple(2, 0x00FF);
	glEnable(GL_LINE_STIPPLE);
	glBegin(GL_LINES);
	glVertex2f(SIZEX / 2, 10);
	glVertex2f(SIZEX / 2, SIZEY - 100);
	glEnd();
	
	// Draw volleys
	sprintf_s(vstr, "%02d - %02d", leftscore, rightscore);
	xpos = 500;
	
	glRasterPos2f(xpos, 920); // location to start printing text
	for (i = 0; i < strlen(vstr); i++) // loop until i is greater then l
	{
		glutBitmapCharacter(GLUT_BITMAP_9_BY_15, vstr[i]); // Print a character on the screen
	}
	volleys = glutGet(GLUT_WINDOW_WIDTH);
	sprintf_s(vstr, "%07d", volleys);
	glRasterPos2f(xpos, 950); // location to start printing text
	for (i = 0; i < strlen(vstr); i++) // loop until i is greater then l
	{
		glutBitmapCharacter(GLUT_BITMAP_9_BY_15, vstr[i]); // Print a character on the screen
	}

	glFlush();  // Render now
}


void idleDisplay() {

	// work happens here -
	std::this_thread::sleep_for(std::chrono::milliseconds(frames));
	// move the ball
	x = x + xspeed;
	y = y + yspeed;

	// are we over the edge? keep the ball going it will reset with space
	if (x > SIZEX) {
		leftscore++;
		x = 0;
		y = 0;
		xspeed = 0;
		yspeed = 0;
		volleys = 0;
	}
	else {
		if (x < 0) {
			rightscore++;
			x = 0;
			y = 0;
			xspeed = 0;
			yspeed = 0;
			volleys = 0;
		}
	}
	if (y > SIZEY) {
		y = 0;
	}
	else {
		if (y < 0) {
			y = SIZEY;
		}
	}
	
	// is there a collsion, then invert the speed
	// if squares origin point is within the area of the square[j] invert speeds
	// if we hit the top part of the paddle invert both x and y, else only y etc

	// right paddle
	// is there a collision
	if (x >= rpx && x <= rpx + rpxs && y >= rpy && y <= rpy+ rpys) {
		volleys++;
		// are we upper half, then if the ball is traveling downwards, reflect up, else leave on downwards path
		if ( y >= ( rpy + rpys/2)) {
			if (yspeed < 0) {
				yspeed = yspeed * -1;
				xspeed = xspeed * -1;
			}
			else {
				xspeed = xspeed * -1;
			}
		}
		// we in lower half so if the ball is travleing up, then refelect, else keep the upward path
		else {
			if (yspeed > 0) {
				yspeed = yspeed * -1;
				xspeed = xspeed * -1;
			}
			else {
				xspeed = xspeed * -1;
			}
		}
	}
	// left paddle
	if (x >= lpx && x <= lpx + lpxs && y >= lpy && y <= lpy + lpys) {
		volleys++;
		if (y >= (lpy + lpys / 2)) {
			if (yspeed < 0) {
				yspeed = yspeed * -1;
				xspeed = xspeed * -1;
			}
			else {
				xspeed = xspeed * -1;
			}
		}
		// we in lower half so if the ball is travleing up, then refelect, else keep the upward path
		else {
			if (yspeed > 0) {
				yspeed = yspeed * -1;
				xspeed = xspeed * -1;
			}
			else {
				xspeed = xspeed * -1;
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

	// check for overflow - we wrap, but maybe we should just stop, e. just keep at the value?
	if (lpy > SIZEY) { lpy = 0; }
	if (lpy < 0) { lpy = SIZEY; }
}


void MoveRightPaddle(char upordown) {
	if (upordown == RPUP) {
		rpy = rpy + paddlestep;
	}
	else {
		rpy = rpy - paddlestep;
	}

	// check for overflow - we wrap, but maybe we should just stop, e. just keep at the value?
	if (rpy > SIZEY) { rpy = 0; }
	if (rpy < 0) { rpy = SIZEY; }
}


void processNormalKeys(unsigned char key, int a, int b) {

	// work happens here -
	std::cout << "in Keys\n";
	if (key == LPUP || key == LPDOWN) {
		MoveLeftPaddle(key);
	}
	if (key == RPUP || key == RPDOWN) {
		MoveRightPaddle(key);
	}
	if (key == RESET) {
		initvars();
	}
	if (key == SLOWER) {
		frames++;
	}
	if (key == FASTER) {
		frames--;
	}
	if (key == NEWBALL) {
		xspeed = 10;
		yspeed = rand() % 15;
	}
}



/* Main function: GLUT runs as a console application starting at main()  */
int main(int argc, char** argv) {

	initvars();
	glutInit(&argc, argv);                           // Initialize GLUT

	glutCreateWindow("OpenGL Pong"); // Create a window with the given title
	glutInitWindowSize(SIZEX, SIZEY);                // Set the window's initial width & height
	glutInitWindowPosition(0, 0);                    // Position the window's initial top-left corner
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	gluOrtho2D(0, SIZEX, 0, SIZEY);
	glutKeyboardFunc(processNormalKeys);             // register the callback for keypress


	glutDisplayFunc(display);                       // Register display callback handler for window re-paint
	glutIdleFunc(idleDisplay);                      // Register display callback handler for window re-paint
	glutMainLoop();                                 // Enter the infinitely event-processing loop
	return 0;
}
