/************************************************************************
 *
 * Copyright (C) 2009-2016 IRCAD France
 * Copyright (C) 2012-2016 IHU Strasbourg
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

#include "fwDicomIOFilter/composite/CTImageStorageDefaultComposite.hpp"

#include "fwDicomIOFilter/modifier/SliceThicknessModifier.hpp"
#include "fwDicomIOFilter/sorter/ImagePositionPatientSorter.hpp"
#include "fwDicomIOFilter/sorter/InstanceNumberSorter.hpp"
#include "fwDicomIOFilter/splitter/AcquisitionNumberSplitter.hpp"
#include "fwDicomIOFilter/splitter/ImageDimensionsSplitter.hpp"
#include "fwDicomIOFilter/splitter/ImagePositionPatientSplitter.hpp"
#include "fwDicomIOFilter/splitter/ImageTypeSplitter.hpp"
#include "fwDicomIOFilter/splitter/SOPClassUIDSplitter.hpp"
#include "fwDicomIOFilter/splitter/TemporalPositionSplitter.hpp"
#include "fwDicomIOFilter/registry/macros.hpp"

fwDicomIOFilterRegisterMacro( ::fwDicomIOFilter::composite::CTImageStorageDefaultComposite );

namespace fwDicomIOFilter
{
namespace composite
{

const std::string CTImageStorageDefaultComposite::s_FILTER_NAME        = "CTImageStorage default composite";
const std::string CTImageStorageDefaultComposite::s_FILTER_DESCRIPTION =
    "CTImageStorage default composite.";

//-----------------------------------------------------------------------------

CTImageStorageDefaultComposite::CTImageStorageDefaultComposite(::fwDicomIOFilter::IFilter::Key key) : IComposite()
{
    // Split according to image type
    this->addChild(::fwDicomIOFilter::splitter::ImageTypeSplitter::New());
    // Split according to acquisition number
    this->addChild(::fwDicomIOFilter::splitter::AcquisitionNumberSplitter::New());
    // Split according to temporal position (MR)
    this->addChild(::fwDicomIOFilter::splitter::TemporalPositionSplitter::New());
    // Split according to image dimensions
    this->addChild(::fwDicomIOFilter::splitter::ImageDimensionsSplitter::New());
    // Sort according to instance number
    this->addChild(::fwDicomIOFilter::sorter::InstanceNumberSorter::New());
    // Sort according to image position
    this->addChild(::fwDicomIOFilter::sorter::ImagePositionPatientSorter::New());
    // Split if gaps are found in image positions
    this->addChild(::fwDicomIOFilter::splitter::ImagePositionPatientSplitter::New());
    // Sort according to image position to get correct orientation
    this->addChild(::fwDicomIOFilter::sorter::ImagePositionPatientSorter::New());
    // Correct slice thickness if needed
    this->addChild(::fwDicomIOFilter::modifier::SliceThicknessModifier::New());
}

//-----------------------------------------------------------------------------

CTImageStorageDefaultComposite::~CTImageStorageDefaultComposite()
{
}

//-----------------------------------------------------------------------------

std::string CTImageStorageDefaultComposite::getName() const
{
    return CTImageStorageDefaultComposite::s_FILTER_NAME;
}

//-----------------------------------------------------------------------------

std::string CTImageStorageDefaultComposite::getDescription() const
{
    return CTImageStorageDefaultComposite::s_FILTER_DESCRIPTION;
}

} // namespace composite
} // namespace fwDicomIOFilter
