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
 * VERSION : 4-0-0 : FA : LAIG-FA-MAC-120653-CS : 7 mai 2014 : Correction de règles qualité ou commentaires *
 * VERSION : 1-0-0 : <TypeFT> : <NumFT> : 26 avr. 2010 : Creation                                                           
 *                                                                                                          *
 * FIN-HISTORIQUE                                                                                           *
 *                                                                                                          *
 * $Id$
 *                                                                                                          *
 ************************************************************************************************************/
#ifndef __vnsRegionForSubsample_txx
#define __vnsRegionForSubsample_txx

#include "vnsRegionForSubsample.h"
#include "vnsMacro.h"

namespace vns
{

    /** Constructor */
    template<unsigned int VImageDimension>
        RegionForSubsample<VImageDimension>::RegionForSubsample()
        {
            m_SubsampleStep = 1;
        }

    /** Destructor */
    template<unsigned int VImageDimension>
        RegionForSubsample<VImageDimension>::~RegionForSubsample()
        {
        }

    /** Method to change the currrent Region */
    template<unsigned int VImageDimension>
        void
        RegionForSubsample<VImageDimension>::ChangeCurrentRegion(IndexType index, unsigned int radius)
        {
            // ---------------------------------------------------------------------------------------------
            IndexType l_SubIndex;
            // ---------------------------------------------------------------------------------------------
            // Loops on the dimensions
            for (unsigned int i = 0; i < VImageDimension; i++)
            {
                l_SubIndex[i] = index[i] * m_SubsampleStep;
                // ---------------------------------------------------------------------------------------------
                // Upper left corner
                l_SubIndex[i] = l_SubIndex[i] - radius;
            }
            // ---------------------------------------------------------------------------------------------
            SizeType l_Size;
            l_Size.Fill(2 * radius + 1);

            // ---------------------------------------------------------------------------------------------
            // Set the index and size
            this->SetIndex(l_SubIndex);
            this->SetSize(l_Size);

            // ---------------------------------------------------------------------------------------------
            // Checks if the largest possible region is inside
            if (this->IsInside(m_LargestPossibleRegion))
            {
                vnsExceptionBusinessMacro("RegionForSubsample::ChangeCurrentRegion()\n"
                        "l_Region is outside the m_LargestPossibleRegion");
            }
            return;
        }

    /** Method to change the radius of the currrent Region */
    template<unsigned int VImageDimension>
        void
        RegionForSubsample<VImageDimension>::ChangeCurrentRegionRadius(unsigned int radius)
        {

            IndexType l_Index = this->GetIndex();
            SizeType l_Size = this->GetSize();

            for (unsigned int i = 0; i < VImageDimension; i++)
            {
                // Back to center
                // Set the index taking into account new radius
                l_Index[i] = l_Index[i] + (l_Size[i] - 1) / 2 - radius;
            }

            this->SetIndex(l_Index);

            return;

        }

    /** PrintSelf method */
    template<unsigned int VImageDimension>
        void
        RegionForSubsample<VImageDimension>::PrintSelf(std::ostream& os, itk::Indent indent) const
        {
            Superclass::PrintSelf(os, indent);
        }

} // End namespace vns

#endif /* __vnsRegionForSubsample_txx */
