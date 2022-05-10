#include "graphics.h"

#include <iostream>
#include <math.h>
#include <stdlib.h>

#define M_PI 3.1415926535897932384626433832795

using namespace std;

class Vec2
{
public:
	float x = 0.f;
	float y = 0.f;

	Vec2()
	{
		Vec2(0.f, 0.f);
	}

	Vec2(float x, float y) 
	{
		this->x = x;
		this->y = y;
	}

	float distance() {
		return sqrt(x*x + y*y);
	}

	Vec2 normalize() {
		float dist = distance();
		return Vec2(this->x / dist, this->y / dist);
	}

	void rotate(double deg) {
		double theta = deg / 180.0 * M_PI;
		double c = cos(theta);
		double s = sin(theta);
		double tx = x * c - y * s;
		double ty = x * s + y * c;
		x = tx;
		y = ty;
	}

	void operator*=(float s) {
		this->x *= s;
		this->y *= s;
	}

	void operator/=(float s) {
		this->x /= s;
		this->y /= s;
	}

	void operator+=(Vec2 vec) {
		this->x += vec.x;
		this->y += vec.y;
	}
	
	void operator-=(Vec2 vec) {
		this->x -= vec.x;
		this->y -= vec.y;
	}

	Vec2 operator+(Vec2 vec) {
		return Vec2(this->x + vec.x, this->y + vec.y);
	}

	Vec2 operator-(Vec2 vec) {
		return Vec2(this->x - vec.x, this->y - vec.y);
	}
	
private:


};


class Line
{
public:
	int color = RED;

	Vec2 A;
	Vec2 B;

	Line(Vec2 A, Vec2 B)
	{
		this->A = A;
		this->B = B;
	}

	void render()
	{
		setcolor(this->color);
		setlinestyle(SOLID_LINE, 1, THICK_WIDTH);

		moveto(this->A.x, this->A.y);
		lineto(this->B.x, this->B.y);

	}

	void setColor(int color)
	{
		this->color = color;
	}

private:

};


class Triangle
{
public:

	Triangle(Vec2 A, Vec2 B, Vec2 C) {
		this->A = A;
		this->B = B;
		this->C = C;
	}

	void move(Vec2 delta)
	{
		this->A += delta;
		this->B += delta;
		this->C += delta;
	}

	void rotate(int deg)
	{
		Vec2 center = this->getCenter();

		this->A -= center;
		this->B -= center;
		this->C -= center;

		this->A.rotate(deg);
		this->B.rotate(deg);
		this->C.rotate(deg);

		this->A += center;
		this->B += center;
		this->C += center;
	}

	Vec2 getCenter() {
		Vec2 center = this->A + this->B + this->C;
		center /= 3;

		return center;
	}

	void scale(float ratio) {
		Vec2 center = this->getCenter();

		this->A -= center;
		this->B -= center;
		this->C -= center;

		this->A *= ratio;
		this->B *= ratio;
		this->C *= ratio;

		this->A += center;
		this->B += center;
		this->C += center;
	}

	void render() {
		Line line1(this->A, this->B);
		Line line2(this->B, this->C);
		Line line3(this->C, this->A);

		line1.render();
		line2.render();
		line3.render();

		setcolor(WHITE);

		outtextxy(this->A.x, this->A.y, "A");
		outtextxy(this->B.x, this->B.y, "B");
		outtextxy(this->C.x, this->C.y, "C");

	}

private:
	Vec2 A;
	Vec2 B;
	Vec2 C;
};



void check_keys();
void render();


Triangle triangle(Vec2(100.f, 0.f), Vec2(100.f, 100.f), Vec2(200.f, 100.f));


void check_keys()
{
	int command;
	float delta = 5.f;

	if (kbhit() == true)
	{
		command = toupper(getch());

		if (char(command) == 'Q')
		{
			triangle.rotate(-delta);
		}
		else if (char(command) == 'E')
		{
			triangle.rotate(delta);
		}

		else if (char(command) == 'W')
		{
			triangle.move(Vec2(0, -delta));
		}

		else if (char(command) == 'S')
		{
			triangle.move(Vec2(0, delta));
		}

		else if (char(command) == 'A')
		{
			triangle.move(Vec2(-delta, 0));
		}

		else if (char(command) == 'D')
		{
			triangle.move(Vec2(delta, 0));
		}

		else if (command == 'C')
		{
			triangle.scale(0.5);
		}

		else if (command == 'V')
		{
			triangle.scale(2);
		}

		render();

	}
	
}

void render() {
	setactivepage(0);
	clearviewport();

	triangle.render();

	setvisualpage(0);
}

int APIENTRY WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nCmdShow)
{

	initwindow(800, 500);
	render();

	while (true) 
	{
		check_keys();
		delay(10);
	}

}
