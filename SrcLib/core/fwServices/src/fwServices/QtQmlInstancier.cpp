#include "fwServices/QtQmlInstancier.hxx"

#include <algorithm>

namespace fwServices
{

std::vector<std::shared_ptr<IQtQmlObject> >	QtQmlInstancier::m_classList;

QtQmlInstancier::QtQmlInstancier()
{
}

QtQmlInstancier::~QtQmlInstancier()
{
}

void	QtQmlInstancier::addClass(std::shared_ptr<IQtQmlObject> const& object)
{
	m_classList.push_back(object);
}

QObject 	*QtQmlInstancier::instanciate(std::string const& cType)
{
	auto	iterator = m_classList.begin();

	while (iterator != m_classList.end())
	{
		if ((*iterator)->getClassType().compare(cType) == 0)
		{
			return (*iterator)->instanciate();
		}
		++iterator;
	}
	return nullptr;
}

} // fwGuiQt
