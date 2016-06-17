include <iostream>
#include "windows.h"
#include "conio.h"
using namespace std;
HANDLE Event;

int main()
{
	setlocale(LC_ALL, "rus");
	Event = OpenEvent(EVENT_ALL_ACCESS, TRUE, L"Event");

	cout << "Для запуска потоков нажмите любую клавишу" << endl;
	_getch();
	SetEvent(Event);

	cout << "Для завершения потоков нажмите любую клавишу" << endl;
	_getch();
	ResetEvent(Event);
	cout << "Для выхода из приложения нажмите любую клавишу" << endl;
	_getch();
	return 0;
}
