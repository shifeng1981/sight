#pragma once

# include "fwGuiQt/config.hpp"
# include "fwGuiQt/QtQmlObject.hxx"
# include "fwGuiQt/QtQmlInstancier.hpp"

# include <vector>
# include <memory>

namespace fwGuiQt
{

/**
 *	@brief: Registration class
 *		This class a new type based on typename T and cType.
 *		Then store it in QtQmlInstancier
 */
template<typename T>
class QtQmlRegistar
{
public:
	/**
	 * @brief: Constructor
	 */
	QtQmlRegistar(std::string const& cType)
	{
		std::shared_ptr<QtQmlObject<T>>	qtObject(new QtQmlObject<T>(cType));

		QtQmlInstancier::addClass(qtObject);
	}

	/**
	 *	@brief: Destructor
	 */
	~QtQmlRegistar()
	{
	}
};

} // fwGuiQt