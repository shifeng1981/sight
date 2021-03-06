/************************************************************************
 *
 * Copyright (C) 2009-2018 IRCAD France
 * Copyright (C) 2012-2018 IHU Strasbourg
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

#pragma once

#include <fwDataTools/helper/Array.hpp>
#include <fwDataTools/helper/ImageGetter.hpp>

#include <fwTools/DynamicType.hpp>

#include <itkImage.h>
#include <itkImageRegion.h>

namespace fwItkIO
{

//------------------------------------------------------------------------------

template< class ITKIMAGE>
void dataImageFactory( typename ITKIMAGE::Pointer itkImage, ::fwData::Image::sptr _dataImage,
                       bool bufferManagerIsDataImage )
{
    SLM_ASSERT("_dataImage not instanced", _dataImage);

    // Add by arnaud
    std::uint8_t dim = ITKIMAGE::ImageDimension;
    ::fwData::Image::SpacingType _vSpacing(dim, 1);
    ::fwData::Image::OriginType _vOrigin(dim, 0);
    ::fwData::Image::SizeType _vSize(dim, 0);

    for (std::uint8_t d = 0; d < dim; ++d)
    {
        // _vOrigin[d] = itkImage->GetBufferedRegion().GetIndex()[d];
        _vOrigin[d]  = itkImage->GetOrigin()[d];
        _vSize[d]    = itkImage->GetBufferedRegion().GetSize()[d];
        _vSpacing[d] = itkImage->GetSpacing()[d];
    }

    _dataImage->setSize( _vSize );
    _dataImage->setOrigin( _vOrigin );
    _dataImage->setSpacing( _vSpacing );

    typedef typename ITKIMAGE::PixelType PixelType;
    _dataImage->setType( ::fwTools::Type::create<PixelType>() );
    ::fwData::Array::sptr array = _dataImage->getDataArray();
    ::fwDataTools::helper::Array arrayHelper(array);
    if( bufferManagerIsDataImage )
    {
        SLM_ASSERT("Sorry, this method requires that itkImage manages its buffer.",
                   itkImage->GetPixelContainer()->GetContainerManageMemory() );
        arrayHelper.setBuffer(
            static_cast<void*>(itkImage->GetBufferPointer()), true, _dataImage->getType(), _vSize, 1,
            ::fwMemory::BufferNewPolicy::New());
        /// itk image release its management buffer. dataImage must now deal memory
        itkImage->GetPixelContainer()->SetContainerManageMemory( false );
    }
    else
    {
        arrayHelper.setBuffer( static_cast<void*>(itkImage->GetBufferPointer()), false,
                               _dataImage->getType(), _vSize, 1 );
    }

    // Post Condition correct PixelType
    SLM_ASSERT("Sorry, pixel type is not correct", _dataImage->getType() != fwTools::Type() );
}

//------------------------------------------------------------------------------

template< class ITKIMAGE>
::fwData::Image::sptr dataImageFactory( typename ITKIMAGE::Pointer itkImage, bool bufferManagerIsDataImage )
{
    ::fwData::Image::sptr data = ::fwData::Image::New();
    ::fwItkIO::dataImageFactory< ITKIMAGE >(itkImage, data, bufferManagerIsDataImage);
    return data;
}

//------------------------------------------------------------------------------

template< class ITKIMAGE_PTR >
void itkImageToFwDataImage( ITKIMAGE_PTR itkImage, ::fwData::Image::sptr _dataImage )
{
    dataImageFactory< typename ITKIMAGE_PTR::ObjectType >(itkImage, _dataImage);
}

//------------------------------------------------------------------------------

template< class ITKIMAGE>
typename ITKIMAGE::Pointer fwDataImageToItkImage( ::fwData::Image::csptr imageData, bool bufferManagerIsDataImage )
{
    // Pre Condition
    SLM_ASSERT("Sorry, itk image dimension not correspond to fwData image",
               imageData->getNumberOfDimensions() == ITKIMAGE::ImageDimension );

    ::fwDataTools::helper::ImageGetter imageHelper(imageData);

    typename ITKIMAGE::Pointer itkImage = ITKIMAGE::New();

    // update spacing information ; workaround due to GetSpacing const
    typename ITKIMAGE::SpacingType spacing = itkImage->GetSpacing();
    for (std::uint8_t d = 0; d < ITKIMAGE::ImageDimension; ++d)
    {
        spacing[d] = imageData->getSpacing()[d];
    }
    itkImage->SetSpacing(spacing);

    // update origin information ; workaround due to GetOrigin const
    std::copy(   imageData->getOrigin().begin(),
                 imageData->getOrigin().end(),
                 const_cast< typename ITKIMAGE::PointType* >( &itkImage->GetOrigin())->Begin()
                 );

    itk::ImageRegion< ITKIMAGE::ImageDimension > itkRegion;

    unsigned long nbpixels = 1;
    for (std::uint8_t d = 0; d < ITKIMAGE::ImageDimension; ++d)
    {
        // itkRegion.SetIndex( d,  static_cast<int>(imageData->getOrigin()[d]) );
        itkRegion.SetSize( d,   static_cast<unsigned long>(imageData->getSize()[d]) );
        nbpixels *= itkRegion.GetSize()[d];
    }

    itkImage->SetRegions(itkRegion);

    if( bufferManagerIsDataImage )
    {
        itkImage->GetPixelContainer()->SetImportPointer(static_cast< typename ITKIMAGE::PixelType*>( imageHelper.
                                                                                                     getBuffer() ), nbpixels,
                                                        false );
    }
    else
    {
        SLM_ASSERT("Sorry, this method requires that imageData manages its buffer.",
                   imageData->getDataArray()->getIsBufferOwner() );
        itkImage->GetPixelContainer()->SetImportPointer(static_cast< typename ITKIMAGE::PixelType*>( imageHelper.
                                                                                                     getBuffer() ), nbpixels,
                                                        true );
        imageData->getDataArray()->setIsBufferOwner( false );
    }

    return itkImage;
}

//------------------------------------------------------------------------------

template< class ITKIMAGE>
typename ITKIMAGE::Pointer itkImageFactory( ::fwData::Image::csptr imageData, bool bufferManagerIsDataImage )
{
    return fwDataImageToItkImage<ITKIMAGE>( imageData, bufferManagerIsDataImage );
}

//------------------------------------------------------------------------------

} // end namespace
