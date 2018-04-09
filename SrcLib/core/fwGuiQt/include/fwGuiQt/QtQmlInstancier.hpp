#pragma once

# include "fwGuiQt/config.hpp"
# include "fwGuiQt/IQtQmlObject.hpp"

# include <QObject>

# include <vector>
# include <memory>

namespace fwGuiQt
{

class FWGUIQT_CLASS_API QtQmlInstancier
{
private:
	// Cannot be instancied
	FWGUIQT_API QtQmlInstancier();
	FWGUIQT_API ~QtQmlInstancier();
	
public:
	static void	FWGUIQT_API addClass(std::shared_ptr<IQtQmlObject> const&);
	static QObject FWGUIQT_API *instanciate(std::string const& cType);


private:
	static std::vector<std::shared_ptr<IQtQmlObject> >	m_classList;
};

} // fwGuiQt