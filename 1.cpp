/* Graphics - Assignment 1 - A set of rays that bounce of mirrors and are absorbed by blocks
 * Author   - Ankush Jain
 * Modified - 10/01/13
 */

/* Basic template borrowed from 
 * www.videotutorialsrock.com
 */



#include <iostream>
#include <stdlib.h> 
#include <cmath>
#include <GL/glut.h>
#include "1.h"
#include "vectorClass.h"

using namespace std;

Vector mirrors[] = {Vector(-20.0f, 10.0f, -10.0f, 2.0f), Vector(20.0f, 35.0f, 20.0f, -1.0f)};
Vector mirrorsTemp[] = {Vector(-20.0f, 10.0f, -10.0f, 2.0f), Vector(20.0f, 35.0f, 20.0f, -1.0f)};
int mirrorLen = 2;
Vector projectors[] = {Vector(-10.0f, -20.0f, 10.0f, -20.0f)};
Vector projectorsTemp[] = {Vector(-10.0f, -20.0f, 10.0f, -20.0f)};
int pointDistance[] = {5.0};
int projRays[] = {21};
int projLen = 1;


//Globals
int width = 120;
int height = 100;
float ratio = width*1.0/height;
int windowHeight = 700;
int windowWidth = ratio*windowHeight;
float _angle = -30.0f;
int mult = 1;
float posX = 0.0f;
float posY= 0.0f;
float posZ = 0.0f;

Vector blocks[] = {Vector(-5.0f, 3.0f, 4.0f, 2.0f), Vector(-width/2.0, height/2.0, width/2.0, height/2.0), Vector(width/2.0, height/2.0, width/2.0, -height/2.0), Vector(-width/2.0, -height/2.0, width/2.0, -height/2.0), Vector(-width/2.0, height/2.0, -width/2.0, -height/2.0)};
Vector blocksTemp[] = {Vector(-5.0f, 3.0f, 4.0f, 2.0f), Vector(-width/2.0, height/2.0, width/2.0, height/2.0), Vector(width/2.0, height/2.0, width/2.0, -height/2.0), Vector(-width/2.0, -height/2.0, width/2.0, -height/2.0), Vector(-width/2.0, height/2.0, -width/2.0, -height/2.0)};
//Vector blocksTemp[] = {Vector(-5.0f, 3.0f, 4.0f, 2.0f), Vector(-width/2.0, height/2.0, width/2.0, height/2.0), Vector(width/2.0, height/2.0, width/2.0, -height/2.0), Vector(-width/2.0, -height/2.0, width/2.0, -height/2.0), Vector(-width/2.0, height/2.0, -width/2.0, -height/2.0)};
int blockLen =  6;
//End of Globals

float distance(float x1, float y1, float x2, float y2){
	float a = pow((x2 - x1), 2), b = pow((y2 - y1), 2);
	return sqrt(a+b);
}
Point * rotatePoint(float x1, float y1, float x, float y, float theta){
	float s = sin(DEG2RAD(theta));	
	float c = cos(DEG2RAD(theta));
	//cout<<"InInYo"<<x1<<" "<<y1<<"\n";
	x1 -= x;
	y1 -= y;
	float xnew = x1*c - y1*s;
	float ynew = y1*c + x1*s;
	xnew += x;
	ynew += y;
	Point * ret = new Point();
	ret->x = xnew;
	ret->y = ynew;
	//cout<<"InInYo"<<xnew<<" "<<ynew<<"\n";

	return ret;
	
}
void rotateVector(Vector v1, float theta, Vector * result){
	Point * p1 = new Point();
	Point * p2 = new Point();
	/*float x = (v1->x1 + v1->x2)/2.0;
	float y = (v1->y1 + v1->y2)/2.0;
	cout<<"InYo: "<<v1->x1<<" "<<v1->y1<<"\n";
	p1 = rotatePoint(v1->x1, v1->y1, x, y, theta);
	cout<<"InYo: "<<v1->x1<<" "<<v1->y1<<"\n";
	p2 = rotatePoint(v1->x2, v1->y2, x, y, theta);

	v1->x1 = p1->x;
	v1->y1 = p1->y;
	v1->x2 = p2->x;
	v1->y2 = p2->y;*/
	
	float x = (v1.x1 + v1.x2)/2.0;
	float y = (v1.y1 + v1.y2)/2.0;
	//cout<<"InYo: "<<v1.x1<<" "<<v1.y1<<"\n";
	p1 = rotatePoint(v1.x1, v1.y1, x, y, theta);
	//cout<<"InYo: "<<v1.x1<<" "<<v1.y1<<"\n";
	p2 = rotatePoint(v1.x2, v1.y2, x, y, theta);

	result->x1 = p1->x;
	result->y1 = p1->y;
	result->x2 = p2->x;
	result->y2 = p2->y;
}
void resetVector(Vector * v1){
	v1->x1 = 0.00;
	v1->y1 = 0.00;
}
int isOnRight(Vector v1, float x, float y){
	Point * rP = rotatePoint(v1.x2, v1.y2, v1.x1, v1.y1, -15.0f);
	//cout<<"Test: "<<rP->x<<", "<<rP->y<<"\n";
	//rightPoint and (x,y) should be on the same side.
	float m = v1.slope();
	float c = v1.intercept();

	float o1 = y - m*x - c;
	float o2 = rP->y - m*(rP->x) - c;

	int flag1, flag2;

	if(o1 < 0 - FLOAT_MARGIN)
		flag1 = -1;
	else if (-FLOAT_MARGIN < o1  && o1 < FLOAT_MARGIN)
		flag1 = 0;
	else
		flag1 = 1;

	if(o2 < 0 - FLOAT_MARGIN)
		flag2 = -1;
	else if (-FLOAT_MARGIN < o2 && o2 < FLOAT_MARGIN)
		flag2 = 0;
	else
		flag2 = 1;
	
	if(flag1 == flag2)
		return 1;
	return 0;
}
//Called when a key is pressed
void handleKeypress(unsigned char key, //The key that was pressed
					int x, int y) {    //The current mouse coordinates
	switch (key) {
		case 27: //Escape key
			exit(0); //Exit the program
		case 97: //A
			projectors[0].x1 -= 0.5f;
			projectors[0].x2 -= 0.5f;
			break;
		case 100: //D
			projectors[0].x1 += 0.5f;
			projectors[0].x2 += 0.5f;
			break;
		case 119: //W
			projectors[0].y1 += 0.5f;
			projectors[0].y2 += 0.5f;
			break;
		case 115: //S
			projectors[0].y1 -= 0.5f;
			projectors[0].y2 -= 0.5f;
			break;
	}
	glutPostRedisplay();
}
void handleSpecial(int key, int x, int y){
	switch(key){
		case GLUT_KEY_UP:
			_angle += 5.0f;
			break;
		case GLUT_KEY_DOWN:
			_angle -= 5.0f;
			break;
	}
	glutPostRedisplay();
}

//Initializes 3D rendering
void initRendering() {
	//Makes 3D drawing work when something is in front of something else
	//glEnable(GL_DEPTH_TEST);
	glEnable(GL_COLOR_MATERIAL);
	glClearColor(1.0f, 1.0f, 1.0f, 1.0f);
	gluOrtho2D(-60.0, 60.0, -45.0, 45.0);
}

//Called when the window is resized
void handleResize(int w, int h) {
	//Tell OpenGL how to convert from coordinates to pixel values
	glViewport(0, 0, w, h);
	
	glMatrixMode(GL_PROJECTION); //Switch to setting the camera perspective
	
	//Set the camera perspective
	glLoadIdentity(); //Reset the camera
	gluOrtho2D(-width/2, width/2, -height/2, height/2);
	/* gluPerspective(45.0,                  
				   (double)w / (double)h, 
				   1.0,                   
				   200.0);               */ 
}

void update(int value){
	_angle += 0.2f * mult;
	if(_angle > 60 || _angle < -60 ){
		mult *= -1;
	}
	glutPostRedisplay();
	glutTimerFunc(15, update, 0);
}
void drawLine(float x1, float y1, float x2, float y2){
	glBegin(GL_LINES);
	  glVertex3f(x1, y1, -0.0f);
	  glVertex3f(x2, y2, -0.0f);
	glEnd();
}
void drawVector(Vector v1){
	drawLine(v1.x1, v1.y1, v1.x2, v1.y2);
}
void drawArray(Vector * array, int len){
	int i=0;
	for(i=0;i<len;i++){
		drawVector(array[i]);
	}
}
Point * getLightSource(Vector projector, float r){
	float m1 = projector.slope();
	float m2 = -(1.0/m1);
	float p = (projector.x1 + projector.x2)/2.0;
	float q = (projector.y1 + projector.y2)/2.0;
	float theta = atan(m2);
	cout<<"------------------   "<<theta<<"\n";
	float c = r*cos(theta), s = r*sin(theta);
	Point *p1 = new Point(), *p2 = new Point();
	p1->x = p + c, p1->y = q+s;
	p2->x = p -c, p2->y = q-s;
	if (isOnRight(projector, p1->x, p1->y))
		return p1;
	return p2;
}
void drawProjector(Vector projector, int n, float pointDistance){
	cout<<"======\n";
	drawVector(projector);

	Point * p1, * p2, * p3;

	int ratL, ratR; //ratios
	float pixelX, pixelY;

	float midX = (projector.x1 + projector.x2)/2.0;
	float midY = (projector.y1 + projector.y2)/2.0;
	
	//Let's assume, for now, that projector will always be perpendicular to y-axis and towards negative Y.
	//midX, midY = getLightSource(Vector projector, float pointDistance)
	p3 = getLightSource(projector, pointDistance);
	midX = p3->x;
	midY = p3->y;
	//midY -= pointDistance;

	float _midX = midX;
	float _midY = midY;	
	//midX and midY are point souce coordinates
	int i = 0;
	for(i=0;i<n;i++){
		//Trace each ray
		ratL = i+1;
		ratR = n-i;

		midX = _midX;
		midY = _midY;
	
		pixelX = (ratL*projector.x1 + ratR*projector.x2)/(ratL+ratR);
		pixelY = (ratL*projector.y1 + ratR*projector.y2)/(ratL+ratR);
		//cout<<"PixelX/Y: "<<pixelX<<" "<<pixelY<<"\n";
		drawLine(midX, midY, pixelX, pixelY);
		int flag = 1;
		int count=0;
		int prevMirror = -1;
		while(flag){
			//cout<<"-count: "<<count<<"\n";
			count++;
			//if(count>10) break; In case of NaN
			//Initial coordinates - (midX, midY) to (pixelX, pixelY)

			//Find next closest object (mirror or block)

			int mirrorOrBlock = 0; //0 means mirror and 1 means block
			float minLen = 10000.00, curLen; //Distance to object
			int index = 0, j;
			//cout<<"Start-MoB: "<<mirrorOrBlock<<" Index: "<<index<<", min distance: "<<minLen<<"\n";

			for(j=0;j<mirrorLen;j++){
				if((j!=prevMirror)&&(p1=mirrorsTemp[j].interRayPoint(Vector(midX, midY, pixelX, pixelY)))){
					//If intersects with mirror, find distance
					//cout<<"--"<<j<<"\n";
					curLen = distance(p1->x, p1->y, pixelX, pixelY);
					if (curLen < minLen){
						minLen = curLen;
						index = j;
						mirrorOrBlock = 0;
					}
				}				
			}
			for(j=0;j<blockLen;j++){
				if((p1 = blocksTemp[j].interRayPoint(Vector(midX, midY, pixelX, pixelY)))){
					curLen = distance(p1->x, p1->y, pixelX, pixelY);
					if(curLen < minLen){
						minLen = curLen;
						index = j;
						mirrorOrBlock = 1;
					}
				}
			}
			if(mirrorOrBlock){
				//It is a block
				p1 = blocksTemp[index].interRayPoint(Vector(midX, midY, pixelX, pixelY));
				drawLine(pixelX, pixelY, p1->x, p1->y);
				flag = 0;
			}
			else{
				//It is a mirror
				p1 = mirrorsTemp[index].interRayPoint(Vector(midX, midY, pixelX, pixelY));
				drawLine(pixelX, pixelY, p1->x, p1->y);
				if(isOnRight(mirrorsTemp[index], pixelX, pixelY)){
					//reflect
					p2 = mirrorsTemp[index].reflection(pixelX, pixelY);
					/*midX = p2->x;
					midY = p2->y;
					pixelX = midX;
					pixelY = midY;*/

					//p2 to p1, p2 is mid, p1 is pixel
					midX = p2->x;
					midY = p2->y;
					pixelX = p1->x;
					pixelY = p1->y;
					//drawLine(p1->x, p1->y, p2->x, p2->y);
					//cout<<"P1: "<<p1->x<<" "<<p1->y<<"\n";
					//cout<<"P2: "<<p2->x<<" "<<p2->y<<"\n";

					prevMirror = index;

					//flag = 0;
				}
				else{
					flag = 0;
				}
			}
			//cout<<"MoB: "<<mirrorOrBlock<<" Index: "<<index<<", min distance: "<<minLen<<"\n";
			
			//If mirror, trace ray to mirror, set next coordinates to be mirror of current origin
			//and next end coordinate to be intersection of mirror and ray
			
			//Else if block or not right side of mirror, hit and stop loop.
			//
			//flag = 0;
		}
		/*if((p1=mirrorsTemp[0].interRayPoint(Vector(midX, midY, pixelX, pixelY)))&&(isOnRight(mirrorsTemp[0], pixelX, pixelY))){
			drawLine(pixelX, pixelY, p1->x, p1->y);
			p2=mirrorsTemp[0].reflection(pixelX, pixelY);
			glColor3f(0.6f, 0.6f, 0.6f);
			//drawLine(p1->x, p1->y, p2->x, p2->y);
			//drawLine(pixelX, pixelY, p2->x, p2->y);
			glColor3f(1.0f, 0.0f, 0.0f);

			if((p3=blocks[4].interRayPoint(Vector(p2->x, p2->y, p1->x, p1->y)))){
				
				drawLine(p3->x, p3->y, p1->x, p1->y);
			}			
			if((p3=blocks[3].interRayPoint(Vector(p2->x, p2->y, p1->x, p1->y)))){
				drawLine(p3->x, p3->y, p1->x, p1->y);

			}
			else{
				//cout<<"Oopsies\n";
				//cout<<"x1: "<<p2->x<<", y1: "<<p2->y<<", x2: "<<p1->x<<", y2: "<<p1->y<<"\n";
			}			
			if((p3=blocks[2].interRayPoint(Vector(p2->x, p2->y, p1->x, p1->y)))){
				drawLine(p3->x, p3->y, p1->x, p1->y);
			}			
			if((p3=blocks[1].interRayPoint(Vector(p2->x, p2->y, p1->x, p1->y)))){
				cout<<"---"<<"p2: "<<p2->x<<", "<<p2->y<<"\n";
				cout<<"---"<<"p1: "<<p1->x<<", "<<p1->y<<"\n";
				cout<<"---"<<"p3: "<<p3->x<<", "<<p3->y<<"\n";
				drawLine(p3->x, p3->y, p1->x, p1->y);
			}			
		}*/
	}


}
void drawProjectors(Vector * array, int len){
	int i=0;
	for(i=0;i<len;i++){
		drawProjector(array[i], projRays[i], pointDistance[i]);
	}
}
//Draws the 3D scene
void drawScene() {
	//Clear information from last draw
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	
	glMatrixMode(GL_MODELVIEW); //Switch to the drawing perspective
	glLoadIdentity(); //Reset the drawing perspective
	glTranslatef(posX, posY, posZ);
	
	glPushMatrix();

	glColor3f(0.5f, 0.5f, 0.5f);
	//cout<<"Yo1: "<<mirrors[0].x1<<" "<<mirrors[0].y1<<"\n";
	rotateVector(projectors[0], _angle, &projectorsTemp[0]);
	//cout<<"Yo2: "<<mirrors[0].x1<<" "<<mirrors[0].y1<<"\n";
	drawArray(mirrorsTemp, mirrorLen);

	glPopMatrix();

	glColor3f(1.0f, 0.7f, 0.0f);
	drawArray(blocks, 1);

	glColor3f(1.0f, 0.0f, 0.0f);
	drawProjectors(projectorsTemp, 1);

	glColor3f(0.6f, 0.2f, 0.9f);
	
	drawLine(0.0f, 0.0f, 1.0f, 0.0f);
	drawLine(0.0f, 0.0f, -1.0f, 0.0f);
	drawLine(0.0f, 0.0f, 0.0f, 1.0f);
	drawLine(0.0f, 0.0f, 0.0f, -1.0f);

	glutSwapBuffers(); //Send the 3D scene to the screen
}

int main(int argc, char** argv) {

	Point * xx = mirrorsTemp[1].reflection(2.72727f, -20.0f);
	cout<<"+++++++++++++++\n";
	cout<<xx->x<<" "<<xx->y<<"\n";
	cout<<"+++++++++++++++\n";

	//Initialize GLUT
	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB | GLUT_DEPTH);
	int w = glutGet(GLUT_SCREEN_WIDTH);
	int h = glutGet(GLUT_SCREEN_HEIGHT);
	glutInitWindowSize(windowWidth, windowHeight); //Set the window size
	glutInitWindowPosition((w - windowWidth) / 2, (h - windowHeight) / 2);
	
	//Create the window
	glutCreateWindow("Mirros and Blocks - Assignment 1 - Ankush Jain - 201101010");
	initRendering(); //Initialize rendering
	
	//Set handler functions for drawing, keypresses, and window resizes
	glutDisplayFunc(drawScene);
	glutKeyboardFunc(handleKeypress);
	glutSpecialFunc(handleSpecial);
	glutReshapeFunc(handleResize);
	
	//glutTimerFunc(26, update, 0);
	glutMainLoop(); //Start the main loop.  glutMainLoop doesn't return.
	return 0; //This line is never reached
}
