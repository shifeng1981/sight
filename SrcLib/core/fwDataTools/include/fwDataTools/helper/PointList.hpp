/* ***** BEGIN LICENSE BLOCK *****
 * FW4SPL - Copyright (C) IRCAD, 2017-2018.
 * Distributed under the terms of the GNU Lesser General Public License (LGPL) as
 * published by the Free Software Foundation.
 * ****** END LICENSE BLOCK ****** */

#ifndef __FWDATATOOLS_HELPER_POINTLIST_HPP__
#define __FWDATATOOLS_HELPER_POINTLIST_HPP__

#include "fwDataTools/config.hpp"

#include <fwData/Array.hpp>
#include <fwData/PointList.hpp>
#include <fwData/TransformationMatrix3D.hpp>

namespace fwDataTools
{
namespace helper
{

/**
 * @brief Defines a helper to modify a ::fwData::PointList.
 */
class FWDATATOOLS_CLASS_API PointList
{

public:

    /// Constructor. Does nothing.
    FWDATATOOLS_API PointList();

    /// Destructor. Does nothing.
    FWDATATOOLS_API ~PointList();

    /**
     * @brief Computes the point-to-point distance between 2 pointlists
     * @param pointList1 [::fwData::PointList]: first point list
     * @param pointList2 [::fwData::PointList]: second point list
     * @return array of the size of one the pointlists (they must have the same size)
     */
    FWDATATOOLS_API static ::fwData::Array::sptr
    computeDistance(::fwData::PointList::sptr pointList1, ::fwData::PointList::sptr pointList2);

    /**
     * @brief Transform a pointList with a transformation matrix
     * @param pointList [::fwData::PointList]: pointlist to be transformed
     * @param matrix [::fwData::TransformationMatrix3D]: transformation to apply to each points in pointlist
     */
    FWDATATOOLS_API static void transform(::fwData::PointList::sptr& pointList,
                                          const ::fwData::TransformationMatrix3D::csptr& matrix);

    /**
     * @brief Associate 2 pointLists:
     * Take 2 pointLists as input and re-order the second one, so that the points at the
     * same index on both lists are the closest to each other
     * @param pointList1 [::fwData::PointList]: first pointlist
     * @param pointList2 [::fwData::PointList]: pointlist that will be re-ordered
     */
    FWDATATOOLS_API static void associate(const ::fwData::PointList::csptr& pointList1,
                                          ::fwData::PointList::sptr pointList2);

};

} // namespace helper
} // namespace fwDataTools

#endif // __FWDATATOOLS_HELPER_POINTLIST_HPP__