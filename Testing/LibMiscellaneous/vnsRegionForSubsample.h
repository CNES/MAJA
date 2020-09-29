/*
* Copyright (C) 2020 Centre National d'Etudes Spatiales (CNES)
*
* Licensed under the Apache License, Version 2.0 (the "License");
* you may not use this file except in compliance with the License.
* You may obtain a copy of the License at
*
*    http://www.apache.org/licenses/LICENSE-2.0
*
* Unless required by applicable law or agreed to in writing, software
* distributed under the License is distributed on an "AS IS" BASIS,
* WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
* See the License for the specific language governing permissions and
* limitations under the License.
*
*/
/************************************************************************************************************ 
 *                                                                                                          *
 *                                ooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooo         *
 *                             o                                                                            *
 *                          o                                                                               *
 *                        o                                                                                 *
 *                      o                                                                                   *
 *                     o       ooooooo       ooooooo    o         o      oo                                 *
 *    o              o       o        o     o       o   o         o     o   o                               *
 *      o           o       o          o   o         o  o         o    o      o                             *
 *        o        o       o           o   o         o  o         o    o        o                           *
 *         o      o        o      oooo     o         o  o         o   o           o                         *
 *          o    o          o              o         o  o         o   o           o                         *
 *           o  o            o             o         o  o o      o   o          o                           *
 *            oo              oooooooo    o         o   o  oooooo   o      oooo                             *
 *                                                     o                                                    *
 *                                                     o                                                    *
 *                                                    o                            o                        *
 *                                                    o            o      oooo     o   o      oooo          *
 *                                                   o             o         o    o    o         o          *
 *                                                   o            o       ooo     o   o       ooo           *
 *                                                               o       o       o   o          o           *
 *                                                               ooooo   oooo    o   ooooo  oooo            *
 *                                                                              o                           *
 *                                                                                                          *
 ************************************************************************************************************
 *                                                                                                          *
 * Author: CS Systemes d'Information  (France)                                                              * 
 *                                                                                                          * 
 ************************************************************************************************************ 
 * HISTORIQUE                                                                                               *
 *                                                                                                          *
 * VERSION : 1-0-0 : <TypeFT> : <NumFT> : 26 avr. 2010 : Creation                                                           
 *                                                                                                          *
 * FIN-HISTORIQUE                                                                                           *
 *                                                                                                          *
 * $Id$
 *                                                                                                          *
 ************************************************************************************************************/
#ifndef __vnsRegionForSubsample_h
#define __vnsRegionForSubsample_h

#include "itkImageRegion.h"

namespace vns
{
/** \class  RegionForSubsample
 * \brief This class aims to modify a region by using an index and/or a size.
 * Two methods are implemented :
 * - ChangeCurrentRegion which needs an index and a radius
 * - ChangeCurrentRegionRadius which just needs a radius
 *
 * 
 * \author CS Systemes d'Information
 *
 * \ingroup Common
 *
 */
template <unsigned int VImageDimension=2>
class RegionForSubsample : public itk::ImageRegion<VImageDimension>
{
public:
    /** Standard class typedefs. */
    typedef RegionForSubsample                                     Self;
    typedef itk::ImageRegion<VImageDimension>                      Superclass;

    typedef typename Superclass::IndexType                         IndexType;
    typedef typename Superclass::SizeType                          SizeType;

    // Parameters accessors
    /** Subsampling Step */
    void SetSubsampleStep(unsigned int step)
    {
        m_SubsampleStep = step;
    }
    unsigned int GetSubsampleStep()
    {
        return m_SubsampleStep;
    }
    /** Largest possible region of the image */
    void SetLargestPossibleRegion(Superclass largestRegion)
    {
        m_LargestPossibleRegion = largestRegion;
    }
    Superclass GetLargestPossibleRegion()
    {
        return m_LargestPossibleRegion;
    }

    /** Constructor */
    RegionForSubsample();
    /** Destructor */
    virtual ~RegionForSubsample();

    /** Change the current region */
    void ChangeCurrentRegion(IndexType index, unsigned int radius);

    /** Change the raidus of the current region */
    void ChangeCurrentRegionRadius(unsigned int radius);

    /** PrintSelf method */
    virtual void PrintSelf(std::ostream& os, itk::Indent indent) const;

private:
    RegionForSubsample(const Self&); //purposely not implemented
    void operator=(const Self&); //purposely not implemented

    /** Subsampling step delcaration */
    unsigned int m_SubsampleStep;
    /** Largest possible region declaration */
    Superclass m_LargestPossibleRegion;

};

} // End namespace vns

#ifndef VNS_MANUAL_INSTANTIATION
#include "vnsRegionForSubsample.txx"
#endif


#endif /* __vnsRegionForSubsample_h */
