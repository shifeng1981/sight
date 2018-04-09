#pragma once

#include "fwGuiQt/config.hpp"
#include "fwGuiQt/IQtQmlObject.hpp"

namespace fwGuiQt
{
	template<typename T>
	class QtQmlObject : public IQtQmlObject
	{
	public:
		QtQmlObject(std::string const& cType) : IQtQmlObject(cType)
		{
		}

		~QtQmlObject()
		{
		}

		QObject 	*instanciate() const
		{
			return new T();
		}
	};
}