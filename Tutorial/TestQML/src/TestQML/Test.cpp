#include "TestQML/config.hpp"
#include "TestQML/Test.hpp"

#include <fwGuiQt/QtQmlRegistar.hxx>

#include <QObject>

fwGuiQt::QtQmlRegistar<Test>	Test::registar("::test");
fwGuiQt::QtQmlType<Test>	Test::type("com.wurmel.io", 1, 0, "Test");
