/* ***** BEGIN LICENSE BLOCK *****
 * FW4SPL - Copyright (C) IRCAD, 2017.
 * Distributed under the terms of the GNU Lesser General Public License (LGPL) as
 * published by the Free Software Foundation.
 * ****** END LICENSE BLOCK ****** */

#ifndef __FWDATA_DIFFUSETEXTURE_HPP__
#define __FWDATA_DIFFUSETEXTURE_HPP__

#include "fwData/factory/new.hpp"
#include "fwData/Image.hpp"
#include "fwData/Object.hpp"

fwCampAutoDeclareDataMacro((fwData)(DiffuseTexture), FWDATA_API);

namespace fwData
{

/**
 * @brief This class manages the parameters and image associated to a single texture.
 */
class FWDATA_CLASS_API DiffuseTexture : public Object
{
public:
    fwCoreClassDefinitionsWithFactoryMacro( (DiffuseTexture)(::fwData::Object), (()),
                                            ::fwData::factory::New< DiffuseTexture >);
    fwCampMakeFriendDataMacro((fwData)(DiffuseTexture));

    /**
     * @brief Constructor
     * @param key Private construction key
     */
    FWDATA_API DiffuseTexture(::fwData::Object::Key key);

    /// Destructor
    FWDATA_API virtual ~DiffuseTexture();

    /// Defines shallow copy
    FWDATA_API void shallowCopy( const Object::csptr& _source );

    /// Defines deep copy
    FWDATA_API void cachedDeepCopy(const Object::csptr& _source, DeepCopyCacheType& cache);

    /**
     * @brief Texture filtering types
     */
    enum FilteringType : unsigned char
    {
        NEAREST,
        LINEAR
    };

    /**
     * @brief Texture wrapping types
     */
    enum WrappingType : unsigned char
    {
        CLAMP,
        REPEAT
    };

    /**
     * @brief Texture blending mode
     */
    enum BlendingType : unsigned char
    {
        NONE = 0,
        REPLACE,
        MODULATE,
        ADD,
        ADD_SIGNED,
        INTERPOLATE,
        SUBTRACT
    };

    /// Getter/Setters
    /**
     * @brief returns editable diffuse texture
     */
    FWDATA_API Image::sptr getImage() const;

    /**
     * @brief Setter for diffuse texture
     *
     * @param diffuseTexture texture
     */
    FWDATA_API void setImage(const Image::sptr& diffuseTexture);

    /** @{
     *  @brief get/set the texture filtering
     */
    FWDATA_API const ::fwData::DiffuseTexture::FilteringType getFiltering() const;
    FWDATA_API void setFiltering(::fwData::DiffuseTexture::FilteringType);
    /// @}

    /** @{
     *  @brief get/set the texture wrapping
     */
    FWDATA_API const ::fwData::DiffuseTexture::WrappingType getWrapping() const;
    FWDATA_API void setWrapping(::fwData::DiffuseTexture::WrappingType);
    /// @}

    /** @{
     *  @brief get/set the texture blending
     */
    FWDATA_API ::fwData::DiffuseTexture::BlendingType getBlending() const;
    FWDATA_API void setBlending(::fwData::DiffuseTexture::BlendingType);
    /// @}

private:

    // The image of the texture
    Image::sptr m_image;

    /// Filtering for diffuse texture
    FilteringType m_filtering;

    /// Wrapping for diffuse texture
    WrappingType m_wrapping;

    /// Wrapping for diffuse texture
    BlendingType m_blending;
};

} //namespace fwData

#endif /* __FWDATA_DIFFUSETEXTURE_HPP__ */

