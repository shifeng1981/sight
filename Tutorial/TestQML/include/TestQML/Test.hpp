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
		m_count += 1;
		std::cout << "Controller IN" << std::endl;
		send(m_count);
	}

	Q_INVOKABLE void	out()
	{
		std::cout << "Controller OUT" << std::endl;
		send(m_count);
	}

	Q_INVOKABLE void	exiting()
	{
		std::cout << "Controller with exit" << std::endl;
	}

	Q_SIGNALS:
		void	send(int value);

	private:
		int	m_count;

	static fwGuiQt::QtQmlRegistar<Test>	registar;
};
