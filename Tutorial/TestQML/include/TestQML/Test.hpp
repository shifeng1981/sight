#include <fwGuiQt/QtQmlRegistar.hxx>
#include <iostream>
#include <QObject>
#include "TestQML/config.hpp"


class Test : public QObject 
{
	Q_OBJECT

public:
	TESTQML_API Test() : QObject(nullptr) {}
	TESTQML_API ~Test() {}
	
	Q_INVOKABLE void	in()
	{
		std::cout << "Controller IN" << std::endl;
	}

	Q_INVOKABLE void	out()
	{
		std::cout << "Controller OUT" << std::endl;
	}

	Q_INVOKABLE void	exiting()
	{
		std::cout << "Controller with exit" << std::endl;
	}

	static fwGuiQt::QtQmlRegistar<Test>	registar;
};
