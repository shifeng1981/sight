#pragma once

# include "fwGuiQt/config.hpp"
# include "fwGuiQt/QtQmlObject.hxx"
# include "fwGuiQt/QtQmlInstancier.hpp"

# include <vector>
# include <memory>

namespace fwGuiQt
{

template<typename T>
class QtQmlRegistar
{
public:
	QtQmlRegistar(std::string const& cType)
	{
		std::shared_ptr<QtQmlObject<T>>	qtObject(new QtQmlObject<T>(cType));

		QtQmlInstancier::addClass(qtObject);
	}

	~QtQmlRegistar()
	{
	}
};

} // fwGuiQt