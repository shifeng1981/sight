/************************************************************************
 *
 * Copyright (C) 2018 IRCAD France
 * Copyright (C) 2018 IHU Strasbourg
 *
 * This file is part of Sight.
 *
 * Sight is free software: you can redistribute it and/or modify it under
 * the terms of the GNU Lesser General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * Sight is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU Lesser General Public License for more details.
 *
 * You should have received a copy of the GNU Lesser General Public
 * License along with Sight. If not, see <https://www.gnu.org/licenses/>.
 *
 ***********************************************************************/

#include "fwRenderOgre/compositor/listener/RayExitDepth.hpp"

#include "fwRenderOgre/helper/Technique.hpp"

#include <fwCore/spyLog.hpp>

#include <OGRE/OgrePass.h>
#include <OGRE/OgreTechnique.h>

namespace fwRenderOgre
{
namespace compositor
{
namespace listener
{

//-----------------------------------------------------------------------------

RayExitDepthListener::RayExitDepthListener()
{

}

//------------------------------------------------------------------------------

RayExitDepthListener::~RayExitDepthListener()
{
}

//------------------------------------------------------------------------------

::Ogre::Technique* RayExitDepthListener::handleSchemeNotFound(unsigned short /*_schemeIndex*/,
                                                              const ::Ogre::String& _schemeName,
                                                              ::Ogre::Material* _originalMaterial,
                                                              unsigned short /*_lodIndex*/,
                                                              const ::Ogre::Renderable* /*_renderable*/)
{
    ::Ogre::Technique* newTechnique = nullptr;
    const auto mtlName = _originalMaterial->getName();

    if(_schemeName == "VolumeEntries_FrontFacesMin" && !::Ogre::StringUtil::startsWith(mtlName, "RTV_Mat"))
    {
        auto entryPointsMtl   = ::Ogre::MaterialManager::getSingleton().getByName("RayEntryPoints");
        auto frontFaceMinTech = entryPointsMtl->getTechnique("FrontFacesMin");

        newTechnique = ::fwRenderOgre::helper::Technique::copyToMaterial(frontFaceMinTech, _schemeName,
                                                                         _originalMaterial);
    }

    return newTechnique;
}

//------------------------------------------------------------------------------

} // namespace listener

} // namespace compositor

} // namespace fwRenderOgre
