#include <fwGuiQt/QtQmlRegistar.hxx>
#include <QObject>
#include "TestQML/config.hpp"
#include "TestQML/Test.hpp"


fwGuiQt::QtQmlRegistar<Test>	Test::registar("::test");

