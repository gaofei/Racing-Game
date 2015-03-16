#include <typeinfo.h>
#include <iostream.h>

class gay
{
};

class WoCaozheshiLeima: public gay
{
};

main()
{
	gay* myclass = new  WoCaozheshiLeima;
	cout<<"Processing Class: "<<typeid(*myclass).name();
}
