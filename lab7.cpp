#include <windows.h>
#include <iostream>
#include <string>
const double PI = 3.1415926;

class Shape 
{
public:
	Shape(double x0 = 100, double y0 = 150, double rotate = 0, double rotateAngle = 0,
		double shapeSpeed = 0, std::string way = "right", COLORREF color = WHITE_PEN)
		:_x0(x0), _y0(y0), _rotate(rotate), _rotateAngle(rotateAngle),
		_shapeSpeed(shapeSpeed), _way(way), _color(color) {}

	virtual void getRotateOfShape(double, double) = 0;

	virtual void move(RECT) = 0;

	virtual void paint(HDC) = 0;

	virtual void clear(HWND, HDC, RECT) = 0;

	virtual ~Shape() = default;

protected:
	virtual void rotate() = 0;

	virtual void draw(HDC, POINT) = 0;

	double _x0, _y0;
	double _rotate; //вращение	
	double _rotateAngle;//угол поворота
	double _shapeSpeed; //скорость фигуры
	std::string _way;
	POINT _point;
	COLORREF _color;
};

class Circle : public Shape 
{
public:
	Circle(double x0 = 100, double y0 = 150, double rotate = 0, double rotateAngle = 0,
		double shapeSpeed = 0, std::string way = "right", double radius = 10, COLORREF color = WHITE_PEN)
		:Shape(x0, y0, rotate, rotateAngle, shapeSpeed, way, color), _radius(radius) {}

	void getRotateOfShape(double rotateAngle, double rotate) 
	{
		_rotate = rotate;
		_rotateAngle = rotateAngle;
	}

	void move(RECT rect) 
	{
		if (_way == "right")
			_x0 += _shapeSpeed;
		else if (_way == "left")
			_x0 -= _shapeSpeed;
		else if (_way == "top")
			_y0 -= _shapeSpeed;
		else if (_way == "bottom")
			_y0 += _shapeSpeed;
		if (_x0 > rect.right - _radius ||
			_x0 > rect.right + _radius ||
			_x0 < rect.left - _radius ||
			_x0 < rect.left + _radius ||
			_y0 < rect.top - _radius ||
			_y0 < rect.top + _radius ||
			_y0 > rect.bottom - _radius ||
			_y0 > rect.bottom + _radius) {
			_shapeSpeed = -_shapeSpeed;
			_rotateAngle = -_rotateAngle;
		}
		rotate();
	}

	void paint(HDC hdc) 
	{
		_brush = CreateSolidBrush(_color);
		SelectObject(hdc, _brush);
		draw(hdc, _point);
		DeleteObject(_brush);
	}

	void clear(HWND hWindow, HDC hdc, RECT rect) 
	{
		_brush = CreateSolidBrush(RGB(0, 0, 0));
		SelectObject(hdc, _brush);
		draw(hdc, _point);
		DeleteObject(_brush);
	}

	~Circle() = default;

private:
	void rotate() 
	{
		_rotate += _rotateAngle;
		_angle = _rotate * PI / 180;
	}

	void draw(HDC hdc, POINT point) 
	{
		MoveToEx(hdc, _x0, _y0, &point);

		Pie(hdc, _x0 - _radius, _y0 - _radius, _x0 + _radius, _y0 + _radius,
			_x0 + _radius * cos(_angle), _y0 - _radius * sin(_angle),
			_x0 + _radius * sin(_angle), _y0 + _radius * cos(_angle));
	}

	double _radius;
	double _angle;
	HBRUSH _brush;
};

class Poly : public Shape 
{
public:
	Poly(double x0 = 100, double y0 = 150, double rotate = 0, double rotateAngle = 0,
		double shapeSpeed = 0, std::string way = "right", double radius = 100, int n = 3, COLORREF color = WHITE_PEN)
		:Shape(x0, y0, rotate, rotateAngle, shapeSpeed, way, color), _radius(radius), n(n) {}

	void getRotateOfShape(double rotate, double rotateAngle) {
		_rotate = rotate;
		_rotateAngle = rotateAngle;
	}

	void move(RECT rect) {
		if (_way == "right")
			_x0 += _shapeSpeed;
		else if (_way == "left")
			_x0 -= _shapeSpeed;
		else if (_way == "top")
			_y0 -= _shapeSpeed;
		else if (_way == "bottom")
			_y0 += _shapeSpeed;
		if (_x0 > rect.right - _radius * cos(_rotate * PI / 180) / 2 ||
			_x0 > rect.right + _radius * cos(_rotate * PI / 180) / 2 ||
			_x0 < rect.left - _radius * cos(_rotate * PI / 180) / 2 ||
			_x0 < rect.left + _radius * cos(_rotate * PI / 180) / 2 ||
			_x0 < rect.left - _radius * sin(_rotate * PI / 180) / 2 ||
			_x0 < rect.left + _radius * sin(_rotate * PI / 180) / 2 ||
			_y0 < rect.top - _radius * sin(_rotate * PI / 180) / 2 ||
			_y0 < rect.top + _radius * sin(_rotate * PI / 180) / 2 ||
			_y0 > rect.bottom - _radius * sin(_rotate * PI / 180) / 2 ||
			_y0 > rect.bottom + _radius * sin(_rotate * PI / 180) / 2) {
			_shapeSpeed = -_shapeSpeed;
			_rotateAngle = -_rotateAngle;
		}
		rotate();
	}

	void paint(HDC hdc) {
		_brush = CreateSolidBrush(_color);
		draw(hdc, _point);
		DeleteObject(_brush);
	}

	void clear(HWND hWindow, HDC hdc, RECT rect) {
		_brush = CreateSolidBrush(RGB(0, 0, 0));
		draw(hdc, _point);
		DeleteObject(_brush);
	}

	~Poly() {
		delete _dots;
	};

private:
	void rotate() 
	{
		_rotate += _rotateAngle;
		_angle = _rotate * PI / 180;

		for (int i = 0; i < n; i++) {
			_dots[i].x = _x0 + _radius*cos((_angle + 360 * (i + 1) / n) * PI / 180);
			_dots[i].y = _y0 + _radius*sin((_angle + 360 * (i + 1) / n) * PI / 180);
		}
	}

	void draw(HDC hdc, POINT _point) 
	{
		BeginPath(hdc);
		Polyline(hdc, _dots, n);
		CloseFigure(hdc);
		EndPath(hdc);
		SelectObject(hdc, _brush);
		SetPolyFillMode(hdc, WINDING);
		FillPath(hdc);
	}

	int n;
	double _radius;
	double _angle;
	POINT *_dots = new POINT[n];
	HBRUSH _brush;
};

class Rectan : public Shape 
{
public:
	Rectan(double x0 = 100, double y0 = 150, double rotate = 0, double rotateAngle = 0,
		double shapeSpeed = 0, std::string way = "right", double length = 100, double width = 30, COLORREF color = WHITE_PEN)
		:Shape(x0, y0, rotate, rotateAngle, shapeSpeed, way, color), _length(length), _width(width) {}

	void getRotateOfShape(double rotate, double rotateAngle) 
	{
		_rotate = rotate;
		_rotateAngle = rotateAngle;
	}

	void move(RECT rect) 
	{
		if (_way == "right")
			_x0 += _shapeSpeed;
		else if (_way == "left")
			_x0 -= _shapeSpeed;
		else if (_way == "top")
			_y0 -= _shapeSpeed;
		else if (_way == "bottom")
			_y0 += _shapeSpeed;
		if (_x0 > rect.right - _length * cos(_rotate * PI / 180) / 2 ||
			_x0 > rect.right + _length * cos(_rotate * PI / 180) / 2 ||
			_x0 < rect.left - _length * cos(_rotate * PI / 180) / 2 ||
			_x0 < rect.left + _length * cos(_rotate * PI / 180) / 2 ||
			_x0 < rect.left - _length * sin(_rotate * PI / 180) / 2 ||
			_x0 < rect.left + _length * sin(_rotate * PI / 180) / 2 ||
			_y0 < rect.top - _width * sin(_rotate * PI / 180) / 2 ||
			_y0 < rect.top + _width * sin(_rotate * PI / 180) / 2 ||
			_y0 > rect.bottom - _width * sin(_rotate * PI / 180) / 2 ||
			_y0 > rect.bottom + _width * sin(_rotate * PI / 180) / 2) {
			_shapeSpeed = -_shapeSpeed;
			_rotateAngle = -_rotateAngle;
		}
		rotate();
	}

	void paint(HDC hdc) 
	{
		_brush = CreateSolidBrush(_color);
		draw(hdc, _point);
		DeleteObject(_brush);
	}

	void clear(HWND hWindow, HDC hdc, RECT rect) 
	{
		_brush = CreateSolidBrush(RGB(0, 0, 0));
		draw(hdc, _point);
		DeleteObject(_brush);
	}

	~Rectan() = default;

private:
	void rotate() 
	{
		_rotate += _rotateAngle;
		_angle = _rotate * PI / 180;

		_dots[0].x = _x0 + sqrt(_length * _length + _width * _width)*cos(_angle) / 2;
		_dots[0].y = _y0 + sqrt(_length * _length + _width * _width)*sin(_angle) / 2;
		_dots[1].x = _x0 + sqrt(_length * _length + _width * _width)*cos(_angle + 2 * atan(_width / _length)) / 2;
		_dots[1].y = _y0 + sqrt(_length * _length + _width * _width)*sin(_angle + 2 * atan(_width / _length)) / 2;
		_dots[2].x = _x0 - sqrt(_length * _length + _width * _width)*cos(_angle) / 2;
		_dots[2].y = _y0 - sqrt(_length * _length + _width * _width)*sin(_angle) / 2;
		_dots[3].x = _x0 - sqrt(_length * _length + _width * _width)*cos(_angle + 2 * atan(_width / _length)) / 2;
		_dots[3].y = _y0 - sqrt(_length * _length + _width * _width)*sin(_angle + 2 * atan(_width / _length)) / 2;
	}

	void draw(HDC hdc, POINT _point) 
	{
		BeginPath(hdc);
		Polyline(hdc, _dots, 4);
		CloseFigure(hdc);
		EndPath(hdc);
		SelectObject(hdc, _brush);
		SetPolyFillMode(hdc, WINDING);
		FillPath(hdc);
	}

	double _width;
	double _length;
	double _angle;
	POINT _dots[4];
	HBRUSH _brush;
};

LRESULT CALLBACK WindowProcess(HWND, UINT, WPARAM, LPARAM);
wchar_t className[] = L"Фигуры";
#define ID_TIMER 1
Circle Circle1(700, 200, 45, 8, 10, "bottom", 50, RGB(255, 250, 250));
Circle Circle2(600, 460, 90, 12, 8, "right", 150, RGB(255, 228, 196));
Circle Circle3(200, 250, 120, 2, 6, "left", 100, RGB(178, 34, 34));
Poly poly1(900, 400, 90, 40, 7, "right", 90, 4, RGB(255, 218, 185));
Poly poly2(500, 200, 30, 60, 6, "top", 60, 5, RGB(238, 130, 238));
Poly poly3(350, 350, 55, 30, 11, "left", 120, 6, RGB(255, 140, 0));
Rectan rec1(600, 300, 90, 2, 9, "right", 190, 60, RGB(238, 213, 210));
Rectan rec2(300, 150, 0, 5, 4, "top", 90, 60, RGB(240, 255, 240));
Rectan rec3(400, 530, 20, 7, 13, "left", 150, 90, RGB(255, 192, 203));
Shape *shapes[] = { &rec1, &rec2, &rec3, &Circle1, &Circle2, &Circle3, &poly1, &poly2, &poly3 };
int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInst, LPSTR pCommandLine, int nCommandShow)
{
	HWND hwnd;
	MSG message;
	WNDCLASSEX windowClass;

	windowClass.hInstance = hInstance;
	windowClass.lpszClassName = className;
	windowClass.lpfnWndProc = WindowProcess;
	windowClass.style = 0;
	windowClass.cbSize = sizeof(WNDCLASSEX);
	windowClass.hIcon = LoadIcon(NULL, IDI_APPLICATION);
	windowClass.hIconSm = LoadIcon(NULL, IDI_WINLOGO);
	windowClass.hCursor = LoadCursor(NULL, IDC_ARROW);
	windowClass.lpszMenuName = NULL;
	windowClass.cbClsExtra = 0;
	windowClass.cbWndExtra = 0;
	windowClass.hbrBackground = (HBRUSH)GetStockObject(BLACK_BRUSH);

	if (!RegisterClassEx(&windowClass)) return 0;
	hwnd = CreateWindow(
		className,
		NULL,
		WS_OVERLAPPEDWINDOW,
		CW_USEDEFAULT,
		CW_USEDEFAULT,
		CW_USEDEFAULT,
		CW_USEDEFAULT,
		HWND_DESKTOP,
		NULL,
		hInstance,
		NULL);

	ShowWindow(hwnd, nCommandShow);
	UpdateWindow(hwnd);
	while (GetMessage(&message, NULL, 0, 0))
	{
		TranslateMessage(&message);
		DispatchMessage(&message);
	}
	return message.wParam;
}

LRESULT CALLBACK WindowProcess(HWND hWindow, UINT uMessage, WPARAM wParameter, LPARAM lParameter) {
	HDC hdc;
	RECT rect;
	GetClientRect(hWindow, &rect);
	switch (uMessage)
	{
	case WM_CREATE:
		SetTimer(hWindow, ID_TIMER, 1, NULL);
		break;
	case WM_DESTROY:
		KillTimer(hWindow, ID_TIMER);
		PostQuitMessage(0);
		break;
	case WM_TIMER:
		hdc = GetDC(hWindow);
		for (std::size_t i = 0; i < 9; ++i) {
			shapes[i]->clear(hWindow, hdc, rect);
			shapes[i]->move(rect);
			shapes[i]->paint(hdc);
		}
		ReleaseDC(hWindow, hdc);
		break;
	default:
		return DefWindowProc(hWindow, uMessage, wParameter, lParameter);
	}
	return 0;
}
