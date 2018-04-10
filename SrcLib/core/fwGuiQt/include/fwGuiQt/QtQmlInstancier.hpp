#pragma once

# include "fwGuiQt/config.hpp"
# include "fwGuiQt/IQtQmlObject.hpp"

# include <QObject>

# include <vector>
# include <memory>

namespace fwGuiQt
{

/**
 * @brief: This class is a manager for holding registered class and instanciate new objects
 */
class FWGUIQT_CLASS_API QtQmlInstancier
{
private:
	// Cannot be instancied
	FWGUIQT_API QtQmlInstancier();
	FWGUIQT_API ~QtQmlInstancier();
	
public:
	/**
	 *	@brief: This method add IQmlObject to the current list of object (m_classList)
	 */
	static void	FWGUIQT_API addClass(std::shared_ptr<IQtQmlObject> const&);
	/**
	 *	@brief: This method take a classType as parameters (stored in IQtQmlObject::m_cType) 
	 *		and create an instance the related object.
	 */
	static QObject FWGUIQT_API *instanciate(std::string const& cType);


private:
	/**
	 *	@brief: this vector store all qml registered class
	 */
	static std::vector<std::shared_ptr<IQtQmlObject> >	m_classList;
};

} // fwGuiQt