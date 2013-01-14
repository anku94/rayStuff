#include <stdio.h>
#include <iostream>
#include "tp.h"
using namespace std;
Point * Vector::interPoint(Vector v2){
	//Returns pointer to point if actually intersect, else returns NULL
	float x3 = v2.x1, x4 = v2.x2;
	float y3 = v2.y1, y4 = v2.y2;

	float d = (x1 - x2) * (y3 - y4) - (y1 - y2) * (x3 - x4);
	// If d is zero, there is no intersection
	if (d == 0) return NULL;

	// Get the x and y
	float pre = (x1*y2 - y1*x2), post = (x3*y4 - y3*x4);
	float x = ( pre * (x3 - x4) - (x1 - x2) * post ) / d;
	float y = ( pre * (y3 - y4) - (y1 - y2) * post ) / d;

	// Check if the x and y coordinates are within both lines
	if ( x < min(x1, x2) || x > max(x1, x2) ||
			x < min(x3, x4) || x > max(x3, x4) ) return NULL;
	if ( y < min(y1, y2) || y > max(y1, y2) ||
			y < min(y3, y4) || y > max(y3, y4) ) return NULL;

	// Return the point of intersection
	Point* ret = new Point();
	ret->x = x;
	ret->y = y;
	return ret;
}
Point * Vector::interRayPoint(Vector v2){
	//Returns pointer to point if ray v2 intersects with line segment v1, else returns NULL
	float x3 = v2.x1, x4 = v2.x2;
	float y3 = v2.y1, y4 = v2.y2;

	float d = (x1 - x2) * (y3 - y4) - (y1 - y2) * (x3 - x4);
	// If d is zero, there is no intersection
	if (d == 0) return NULL;

	// Get the x and y
	float pre = (x1*y2 - y1*x2), post = (x3*y4 - y3*x4);
	float x = ( pre * (x3 - x4) - (x1 - x2) * post ) / d;
	float y = ( pre * (y3 - y4) - (y1 - y2) * post ) / d;

	// Check if the x and y coordinates are within both lines
	/*if ( x < min(x1, x2) || x > max(x1, x2) ||
			x < min(x3, x4) || x > max(x3, x4) ) return NULL;
	if ( y < min(y1, y2) || y > max(y1, y2) ||
			y < min(y3, y4) || y > max(y3, y4) ) return NULL;*/

	if ( x < (min(x1, x2) - FLOAT_MARGIN) || x > (max(x1, x2) + FLOAT_MARGIN)) 
		return NULL;
	if ( y < (min(y1, y2) - FLOAT_MARGIN) || y > (max(y1, y2) + FLOAT_MARGIN) ){
		return NULL;
	}
	if ( x3 < x4 - FLOAT_MARGIN){
		if(x < x4 - FLOAT_MARGIN)
			return NULL;
	}
	if (x3 > x4 + FLOAT_MARGIN){
		if(x > x4 + FLOAT_MARGIN)
			return NULL;
	}
	if ( y3 < y4 - FLOAT_MARGIN){
		if(y < y4 - FLOAT_MARGIN)
			return NULL;
	}
	if (y3 > y4 + FLOAT_MARGIN){
		if(y > y4 + FLOAT_MARGIN)
			return NULL;
	}

	// Return the point of intersection
	Point* ret = new Point();
	ret->x = x;
	ret->y = y;
	return ret;
}
Point * Vector::interRayPoint2(Vector v2){
	float m1 = this->slope(), m2=v2.slope();
	float c1 = this->intercept(), c2=v2.intercept();
	float x = (c2 - c1)/(m1 - m2);
	float y=(c2*m1 - c1*m2)/(m1 - m2);

	float x3 = v2.x1, x4 = v2.x2;
	float y3 = v2.y1, y4 = v2.y2;

	//Point obtained, but should not lie outside v1 and one side of v2
	if ( x < min(x1, x2) || x > max(x1, x2) ) return NULL;
	if ( y < min(y1, y2) || y > max(y1, y2) ) return NULL;
	if ( x3 < x4 ){
		if(x < x4)
			return NULL;
	}
	if (x3 > x4){
		if(x > x4)
			return NULL;
	}
	if ( y3 < y4 ){
		if(y < y4)
			return NULL;
	}
	if (y3 > y4){
		if(y > y4)
			return NULL;
	}

	Point *ret = new Point();
	ret->x = x;
	ret->y = y;
	return ret;
}
float Vector::slope(){
	float slope = (y2 - y1)/(x2 - x1);
	return slope;
}
float Vector::intercept(){
	float c = (x1*y2 - x2*y1)/(x1 - x2);
	return c;
}
Point * Vector::reflection(float x, float y){
	float m=this->slope();

	//Handling NaN
	if((m > 999) || (m < -999)){
		Point * r2 = new Point();
		r2->y = y;
		r2->x = 2*x1 - x;
		return r2;
	}

	float c=this->intercept();
	float d= (x + (y - c)*m)/(1 + m*m);
	cout<<m<<c<<d<<"\n";
	Point * ret = new Point();
	ret->x = (2*d - x);
	ret->y = (2*d*m - y + 2*c);
	return ret;
}
int main(){
	Vector v1(-1.0, 3.0, 0.0, 4.0), v2(-1.0, 1.0, -3.0, 3.0);
	Vector v3(-60.0, -50.0, 60.0, -50.0);
	Vector v4(-7.33147, 24.1063, -9.6087, 1.42391);
	Vector v5(20.0, 35.0, 20.0, -1.0);


	//Point * p = v1.interRayPoint(v2);
	//p = v1.reflection(1.0, 2.0);
	Point * p = v5.reflection(2.72727, -20.0);
	
	if(p == NULL){
		cout<<(p == NULL)<<" Oops\n";
	}
	else
	{
		cout<<p->x<<p->y;
	}
}
