#include "graphics.h"
#include <iostream>
#include <math.h>
#include <stdlib.h>                     // Provides exit

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

private:


};


class Position
{
public:
	float size;
	float angle = 0;

	Vec2 vector;

	Position() {
		Position(Vec2(0, 0), Vec2(1, 1));
	}
	Position(Vec2 start, Vec2 end) {
		this->x = start.x;
		this->y = start.y;
		this->size = Vec2(end.x - start.x, end.y - start.y).distance();
		this->angle = atan((end.x - start.x) / (end.y - start.y )) * 180 / M_PI;

		this->vector = Vec2(end.x - start.x, end.y - start.y).normalize();
	}
	
	Vec2* start() {
		return new Vec2(this->x, this->y);
	}

	Vec2* end() {

		return new Vec2(this->x + this->size * vector.x, this->y + this->size * vector.y);
	}

	void moveTo(int x, int y)
	{
		this->x = x;
		this->y = y;
	}

	void applyRotate(int deg)
	{
		angle += deg;
		angle = angle > 360.f ? angle - 360.f : angle;
		angle = angle < 0.f ? angle + 360.f : angle;

		vector.rotate(deg);
		
	}

	void scale(float ratio) {
		vector *= ratio;
	}

private:
	int x;
	int y;
};

class Line
{
public:
	int color = RED;

	Position position;

	Line(Position position)
	{
		this->position = position;
	}

	void DrawWithFirstMethod()
	{
		setcolor(this->color);
		setlinestyle(SOLID_LINE, 1, THICK_WIDTH);

		moveto((int)position.start()->x, (int)position.start()->y);
		lineto((int)position.end()->x, (int)position.end()->y);

	}

	void DrawWithSecondMethod()
	{
		setcolor(this->color);
		int x1 = position.start()->x,
			y1 = position.start()->y,
			x2 = position.end()->x,
			y2 = position.end()->y;


		const int deltaX = abs(x2 - x1);
		const int deltaY = abs(y2 - y1);
		const int signX = x1 < x2 ? 1 : -1;
		const int signY = y1 < y2 ? 1 : -1;

		int error = deltaX - deltaY;

		putpixel(x2, y2, this->color);

		while (x1 != x2 || y1 != y2)
		{
			putpixel(x1, y1, this->color);
			putpixel(x1-1, y1, this->color);
			putpixel(x1+1, y1, this->color);
			putpixel(x1, y1 - 1, this->color);
			putpixel(x1, y1 + 1, this->color);

			int error2 = error * 2;
			if (error2 > -deltaY)
			{
				error -= deltaY;
				x1 += signX;
			}
			if (error2 < deltaX)
			{
				error += deltaX;
				y1 += signY;
			}
		}

	}

	void setColor(int color)
	{
		this->color = color;
	}

private:

};


void check_keys();
void mouseEvent(int x, int y);
void setActiveLine(Line *line);
void render();

Line line1(Position(Vec2(0.f, 0.f), Vec2(100.f, 100.f)));
Line line2(Position(Vec2(500.f, 250.f), Vec2(450.f, 300.f)));

Line *activeLine = &line1;

void mouseEvent(int x, int y) {
	if (activeLine) activeLine->position.moveTo(x, y);
	render();
}

void setActiveLine(Line *line) {
	if(activeLine)	activeLine->setColor(RED);

	activeLine = line;

	if (activeLine) activeLine->setColor(COLOR(255, 255, 0));

}

void changeActiveLine(int x, int y) {
	if (activeLine == &line1) {
		setActiveLine(&line2);
	} else if(activeLine == &line2) {
		setActiveLine(nullptr);
	} else {
		setActiveLine(&line1);
	}
	mouseEvent(x, y);
}

void check_keys()
{
	int command;

	if (!activeLine) return;

	if (kbhit() == true)
	{
		command = getch();
		if (command == 224)
		{
			command = getch();
		}

		float delta = 5.f;
		
		if (char(command) == KEY_LEFT)
		{
			activeLine->position.applyRotate(-delta);
		} 
		else if(char(command) == KEY_RIGHT)
		{
			activeLine->position.applyRotate(delta);
		} 
		else if(command == KEY_UP)
		{
			activeLine->position.scale(1.5);
		}
		else if (command == KEY_DOWN)
		{
			activeLine->position.scale(1 / 1.5);
		}

		render();

	}
}

void render() {
	setactivepage(0);
	clearviewport();

	line1.DrawWithFirstMethod();
	line2.DrawWithSecondMethod();

	setcolor(WHITE);

	if (activeLine) {
		char msg[512];
		char msg2[512];

		sprintf(msg, "Position (%f, %f) (%f, %f) ", activeLine->position.start()->x, activeLine->position.start()->y, activeLine->position.end()->x, activeLine->position.end()->y);
		sprintf(msg2, "Angle: %d Size: %f VX: %f VY: %f", (int)(activeLine->position.angle), activeLine->position.size, activeLine->position.vector.x, activeLine->position.vector.y);

		outtextxy(0, 0, msg);
		outtextxy(0, 20, msg2);
	}
	

	setvisualpage(0);
}

int APIENTRY WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nCmdShow)
{

	cout << "Press 'Q' to quit" << endl;

	initwindow(800, 500);
	registermousehandler(WM_MOUSEMOVE, mouseEvent);
	registermousehandler(WM_LBUTTONDOWN, changeActiveLine);

	setActiveLine(&line1);
	render();

	while (true) 
	{
		check_keys();
		delay(10);
	}

}

