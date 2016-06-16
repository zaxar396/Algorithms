#include "iostream"
#include <fstream>
#include <string>

using namespace std;

class providers
{
protected:
	char name[100];
	char country[100]; //страна поставщика
	char product[100];

public:
	providers()
	{

	}

	virtual void set()
	{
		cout << "Название компании" << endl;
		cin >> name;
		cout << "Страна компании" << endl;
		cin >> country;
		cout << "Название продукта" << endl;
		cin >> product;

	}

	virtual void write()
	{
		ofstream f1("shop.txt", ios_base::app);
		f1 << "Название компании " << name << "Страна компании " << country << "Название продукта " << product << endl;
	}

	virtual void info()
	{
		cout << "Название компании " << name << "Страна компании " << country << "Название продукта " << product << endl;
	}

	virtual ~providers()
	{

	}
};

class products : public providers
{
private:
	char date[100];
	char price[100];
	char shelflife[100];
public:
	products()
	{

	}

	void set()
	{
		providers::set();
		cout << "Дата изготовления" << endl;
		cin >> date;
		cout << "Цена" << endl;
		cin >> price;
		cout << "Срок годности" << endl;
		cin >> shelflife;
	}

	void write()
	{
		providers::write();
		ofstream f1("shop.txt", ios_base::app);
		f1 << "Дата изготовления " << date << "Цена " << price << "Срок годности " << shelflife << endl;
	}

	void info()
	{
		providers::info();
		cout << "Дата изготовления " << date << "Цена " << price << "Срок годности " << shelflife << endl;
	}

	~products()
	{

	}
};

class staff
{
   private:
	   char fio[100];
	   char cash[100];

   public:
	   staff()
	   {

	   }

	   void set()
	   {
		   cout << "Ф. И. О." << endl;
		   cin >> fio;
		   cout << "Зарплата" << endl;
		   cin >> cash;
	   }

	   void write()
	   {
		   ofstream f1("shop.txt", ios_base::app);
		   f1 << "Ф. И. О. " << fio << "Зарплата " << cash << endl;
	   }

	   void info()
	   {
		   cout << "Ф. И. О. " << fio << "Зарплата " << cash << endl;
	   }

	   ~staff()
	   {

	   }
};

void search(char a[100])
{
	ifstream f1("shop.txt", ios_base::app);
	char buff[100];
	string s;
	while (!f1.eof())
	{
		f1 >> buff;
		if (strcmp(a, buff) == 0)
		{
			getline(f1, s);
			cout << s << endl;
		}
		else
		{
			cout << "Не найдено" << endl;
		}
	}
}

int main()
{
	setlocale(LC_ALL, "Russian");
	ofstream f1("shop.txt", ios_base::app);
	char znach;
	char sear[100];
	products A;
	staff B;
	cout << "1 - добавить продукт(поставщика) " << endl << " 2 - добавить работника " << endl << " 3 поиск в базе" << endl
		<<" 4 - информация о продукте(поставщике) " << endl <<"5 - информация о сотруднике "<< endl;
	cin >> znach;

	switch (znach)
	{

	case '1':

		A.set();
		A.write();
		break;

	case '2':

		B.set();
		B.write();
		break;

	case '3' :
		cin >> sear;
		search(sear);

	case '4' :

		A.info();
		break;

	case '5' :

		B.info();
		break;

	}

	system("pause");
	return 0;
}
