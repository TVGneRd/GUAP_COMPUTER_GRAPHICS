#include "graphics.h"

#include <iostream>
#include <math.h>

#define M_PI 3.1415926535897932384626433832795

using namespace std;

template <int ROWS, int COLUMNS>
class Matrix {

public:

	float **matrix;
	int rows;
	int columns;

	Matrix() {};

	Matrix(float arr[ROWS][COLUMNS]) {
		matrix = new float *[ROWS];

		rows = ROWS;
		columns = COLUMNS;

		for (int i = 0; i < ROWS; i++) {
			matrix[i] = new float[COLUMNS];

			for (int j = 0; j < COLUMNS; j++) {
				matrix[i][j] = arr[i][j];
			}
		}
	}

	float* operator[](int n){
		return matrix[n];
	}

	static Matrix getScale(float sx, float sy, float sz) {
		float a[4][4] = {
			{sx, 0, 0, 0},
			{0, sy, 0, 0},
			{0, 0, sz, 0},
			{0, 0,  0,  1.f},
		};

		return Matrix<4,4>(a);
	}

	static Matrix getTransition(float dx, float dy, float dz) {
		float a[4][4] = {
			{1, 0, 0, dx},
			{0, 1, 0, dy},
			{0, 0, 1, dz},
			{0, 0, 0, 1},
		};

		return Matrix<4, 4>(a);
	}

	static Matrix getRotationX(int angle) {
		float rad = M_PI / 180 * angle;

		float a[4][4] = {
				{1, 0, 0, 0},
				{0, cos(rad), -sin(rad), 0},
				{0, sin(rad), cos(rad), 0},
				{0, 0, 0, 1},
		};

		return Matrix<4, 4>(a);
	}

	static Matrix getRotationY(int angle) {
		float rad = M_PI / 180 * angle;

		float a[4][4] = {
				{cos(rad), 0, sin(rad), 0},
				{0, 1, 0, 0},
				{-sin(rad), 0, cos(rad), 0},
				{0, 0, 0, 1},
		};

		return Matrix<4, 4>(a);
	}

	static Matrix getRotationZ(int angle) {
		float rad = M_PI / 180 * angle;

		float a[4][4] = {
				{cos(rad), -sin(rad), 0, 0},
				{sin(rad), cos(rad), 0, 0},
				{0, 0, 1, 0},
				{0, 0, 0, 1},
		};

		return Matrix<4, 4>(a);
	}
private:
};

class Vec3
{
public:
	float x = 0.f;
	float y = 0.f;
	float z = 0.f;
	float w = 0.f;

	Vec3()
	{
		Vec3(0.f, 0.f, 0.f);
	}

	Vec3(float x, float y, float z, float w = 0.f)
	{
		this->x = x;
		this->y = y;
		this->z = z;
		this->w = w;
	}

	void operator+=(Vec3 vec) {
		this->x += vec.x;
		this->y += vec.y;
		this->z += vec.z;
	}

	void operator/=(float s) {
		this->x /= s;
		this->y /= s;
		this->z /= s;
	}

	void operator-=(Vec3 vec) {
		this->x -= vec.x;
		this->y -= vec.y;
		this->z -= vec.z;
	}

	Vec3 operator+(Vec3 vec) {
		
		return Vec3(this->x + vec.x, this->y + vec.y, this->z + vec.z);
	}

	void operator*=(Matrix<4,4> m) {
		float x = this->x;
		float y = this->y;
		float z = this->z;
		float w = this->w;

		this->x = m[0][0] * x + m[0][1] * y + m[0][2] * z + m[0][3] * w;
		this->y = m[1][0] * x + m[1][1] * y + m[1][2] * z + m[1][3] * w;
		this->z = m[2][0] * x + m[2][1] * y + m[2][2] * z + m[2][3] * w;
		this->w = m[3][0] * x + m[3][1] * y + m[3][2] * z + m[3][3] * w;
	}

private:


};


class Line
{
public:
	int color = RED;

	Vec3 A;
	Vec3 B;

	Line(Vec3 A, Vec3 B)
	{
		this->A = A;
		this->B = B;
	}

	void draw()
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


class Pyramid
{
public:
	int verticesLength = 8;
	int edgesLength = 12;
	int controlMode = 0;

	Vec3 center = Vec3(250, 250, 0);
	Vec3 rotateVec = Vec3(0, 0, 0); // usage in status only

	Vec3 vertices[8] = {
		 Vec3(0, 0, 0), // 0 вершина
		 Vec3(1.f, 0, 0), // 1 вершина
		 Vec3(0.5, pow(3, 1 / 2) / 2, 0), // 2 вершина
		 Vec3(0.5, pow(3, 1 / 2) / 6, pow(6, 1 / 2) / 3), // 3 вершина
	};

	int edges[12][2] = {
		{0, 1},
		{1, 2},
		{2, 0},

		{0, 3},
		{1, 3},
		{2, 3},
	};
	

	Pyramid() {
		Vec3 center = getCenter();

		for (int i = 0; i < verticesLength; i++) {
			vertices[i] -= center;
		}

		scale(200);
		rotate(Vec3(45, 45, 45));

	}

	void move(Vec3 delta)
	{
		center += delta;
	}

	void rotate(Vec3 rotateVector)
	{
		rotateVec += rotateVector;

		if (rotateVector.x) {
			Matrix rotateX = Matrix<4, 4>::getRotationX(rotateVector.x);

			rotateVec.x = rotateVec.x > 360 ? rotateVec.x - 360 : rotateVec.x;
			rotateVec.x = rotateVec.x < 0   ? 360 + rotateVec.x : rotateVec.x;

			for (int i = 0; i < verticesLength; i++) {
				vertices[i] *= rotateX;
			}
		}

		if (rotateVector.y) {
			rotateVec.y = rotateVec.y > 360 ? rotateVec.y - 360 : rotateVec.y;
			rotateVec.y = rotateVec.y < 0   ? 360 + rotateVec.y : rotateVec.y;

			Matrix rotateY = Matrix<4, 4>::getRotationY(rotateVector.y);
			for (int i = 0; i < verticesLength; i++) {
				vertices[i] *= rotateY;
			}
		}

		if (rotateVector.z) {
			rotateVec.z = rotateVec.z > 360 ? rotateVec.z - 360 : rotateVec.z;
			rotateVec.z = rotateVec.z < 0   ? 360 + rotateVec.z : rotateVec.z;

			Matrix rotateZ = Matrix<4, 4>::getRotationZ(rotateVector.z);
			for (int i = 0; i < verticesLength; i++) {
				vertices[i] *= rotateZ;
			}
		}
		
	}

	Vec3 getCenter() {
		Vec3 center = Vec3(0, 0, 0);

		for (int i = 0; i < verticesLength; i++) {
			center += vertices[i];
		}

		center /= verticesLength;

		return center;
	}

	void scale(float ratio) {
		Matrix scale = Matrix<4, 4>::getScale(ratio, ratio, ratio);

		for (int i = 0; i < verticesLength; i++) {
			vertices[i] *= scale;
		}
	}

	void render() {
		setcolor(RED);

		for (int i = 0; i < edgesLength; i++) {
			Line line(vertices[edges[i][0]] + center, vertices[edges[i][1]] + center);
			line.draw();
		}
		
		setcolor(WHITE);

		string hud = "TRANSLATE: X: ";
		hud += to_string(center.x);
		hud += " Y: ";
		hud += to_string(center.y);
		hud += " Z: ";
		hud += to_string(center.z);
		hud += " ";
		outtextxy(5, 0, hud.data());

		string hud2 = "ROTATE:   X: ";
		hud2 += to_string(rotateVec.x);
		hud2 += " Y: ";
		hud2 += to_string(rotateVec.y);
		hud2 += " Z: ";
		hud2 += to_string(rotateVec.z);
		hud2 += " ";
		outtextxy(5, 18, hud2.data());

		string mode = "Mode: ";

		mode += controlMode ? "MOVING" : "ROTATION";

		outtextxy(780 - textwidth(mode.data()), 0, mode.data());
	}

private:
	
};


void check_keys();
void render();

Pyramid pyramid;


void check_keys()
{
	int command;
	float delta = 5.f;

	if (kbhit() == true)
	{
		command = toupper(getch());
		if (GetKeyState('Q') & 0x8000)
		{
			pyramid.controlMode ? pyramid.move(Vec3(0, 0, -delta)) : pyramid.rotate(Vec3(0, 0, -delta));
		}

		if (GetKeyState('E') & 0x8000)
		{
			pyramid.controlMode ? pyramid.move(Vec3(0, 0, delta)) : pyramid.rotate(Vec3(0, 0, delta));
		}

		if (GetKeyState('W') & 0x8000)
		{
			pyramid.controlMode ? pyramid.move(Vec3(0, -delta, 0)) : pyramid.rotate(Vec3(delta, 0, 0));
		}

		if (GetKeyState('S') & 0x8000)
		{
			pyramid.controlMode ? pyramid.move(Vec3(0, delta, 0)) : pyramid.rotate(Vec3(-delta, 0, 0));
		}

		if (GetKeyState('A') & 0x8000)
		{
			pyramid.controlMode ? pyramid.move(Vec3(-delta, 0, 0)) : pyramid.rotate(Vec3(0, -delta,  0));
		}

		if (GetKeyState('D') & 0x8000)
		{
			pyramid.controlMode ? pyramid.move(Vec3(delta, 0, 0)) : pyramid.rotate(Vec3(0, delta, 0));
		}

		if (GetKeyState('C') & 0x8000)
		{
			pyramid.scale(1 / 1.5);
		}

		if (GetKeyState('V') & 0x8000)
		{
			pyramid.scale(1.5);
		}

		if (GetKeyState(VK_TAB) & 0x8000)
		{
			pyramid.controlMode = !pyramid.controlMode;
		}

		render();

	}
	
}

void render() {
	cleardevice();

	pyramid.render();

	swapbuffers();
}

int APIENTRY WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nCmdShow)
{
	initwindow(800, 500);

	render();

	while (true) 
	{
		check_keys();
		delay(20);
	}

}
