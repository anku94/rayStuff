#ifndef A_H
#define A_H
#define FLOAT_MARGIN 0.0005f
/*Include guards*/
class Point {
public:
	float x, y;
	Point(){};
	Point(float x, float y){
		this->x = x;
		this->y = y;
	}

};
class Vector {
	public: 
	float x1, y1, x2, y2;
	Vector(float x1, float y1, float x2, float y2){
		this->x1 = x1;
		this->x2 = x2;
		this->y1 = y1;
		this->y2 = y2;
	}
	Point * interPoint(Vector);
	Point * interRayPoint(Vector);
	Point * interRayPoint2(Vector);
	float slope();
	float intercept();
	Point * reflection(float, float);


};
#endif
