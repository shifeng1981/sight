#pragma once

#include "fwServices/config.hpp"

namespace fwServices
{

class FWSERVICES_CLASS_API IQmlEngine
{
public:
	virtual FWSERVICES_API ~IQmlEngine();

	virtual void FWSERVICES_API 	loadFile(std::string const&) = 0;
	virtual void FWSERVICES_API 	launch() = 0;
	virtual void FWSERVICES_API 	addCtx(std::string const& uid, std::string const& type) = 0;
};

}