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
 * VERSION : 5-1-0 : FA : LAIG-FA-MAC-145739-CS : 27 juin 2016 : Audit code - Supp de la macro ITK_EXPORT   *
 * VERSION : 5-1-0 : FA : LAIG-FA-MAC-144674-CS : 2 mai 2016 : Correction warning, qualite, etc             *
 * VERSION : 5-0-0 : FA : LAIG-FA-MAC-143764-CS : 12 avril 2016 : Mise en forme du code (indentation, etc.) *
 * VERSION : 4-4-0 : FA : LAIG-FA-MAC-133882-CS : 3 juin 2015 : Correction code pour la qualite             *
 * VERSION : 2-0-0 : DM : LAIG-DM-MAC-146-CNES : 13 oct. 2011 : Pas de produit composite en mode init            *
 * VERSION : 1-0-0-3 : DM : 357 : 25 juil. 2011 : The method GetLUT() returns a LUTConstPointerType instead   *
 * 												of a LUTPointerType											*
 * VERSION : 1-0-0 : <TypeFT> : <NumFT> : 7 juin 2010 : Creation                                              *
 *                                                                                                          *
 * FIN-HISTORIQUE                                                                                           *
 *                                                                                                          *
 * $Id$
 *                                                                                                          *
 ************************************************************************************************************/
#ifndef vnsDarkestPixelAOTCalculator_h
#define vnsDarkestPixelAOTCalculator_h

#include "otbPersistentImageFilter.h"
#include "otbPersistentFilterStreamingDecorator.h"
#include "itkSimpleDataObjectDecorator.h"
#include "vnsMacro.h"

namespace vns
{
/** \class  PersistentDarkestPixelAOTCalculator
 * \brief This class implements the method which computes the darkest pixel
 * of a reflectance image. It first find the minimum value of the input image,
 * considering constraints and then convert this value inverting a AOT LUT.
 * 
 * \author CS Systemes d'Information
 *
 * \sa Object
 *
 * \ingroup L2
 * \ingroup Checktool
 *
 */
    template<class TInputImage, class TInputDTM, class TInputMask, class TLUT>
        class PersistentDarkestPixelAOTCalculator :
            public otb::PersistentImageFilter<TInputImage, TInputImage>
        {
public:
    /** Standard class typedefs. */
    typedef PersistentDarkestPixelAOTCalculator                    Self;
    typedef otb::PersistentImageFilter<TInputImage,TInputImage>    Superclass;
    typedef itk::SmartPointer<Self>                                Pointer;
    typedef itk::SmartPointer<const Self>                          ConstPointer;

    /** Type macro */
    itkNewMacro(Self);
    /** Creation through object factory macro */
    itkTypeMacro(PersistentDarkestPixelAOTCalculator, PersistentImageFilter );

    /** Some convenient typedefs. */
    typedef TInputImage                                     InputImageType;
    typedef typename InputImageType::ConstPointer           InputImageConstPointerType;
    typedef typename InputImageType::Pointer                InputImagePointerType;
    typedef typename InputImageType::RegionType             InputImageRegionType;
    typedef typename InputImageType::PixelType              InputImagePixelType;
    typedef typename InputImageType::InternalPixelType      InternalPixelType;
    typedef typename InputImageType::IndexType              IndexType;
    typedef typename InputImageType::SizeType               SizeType;


    typedef TInputMask                                      InputMaskType;
    typedef typename InputMaskType::Pointer                 InputMaskPointerType;
    typedef typename InputMaskType::ConstPointer            InputMaskConstPointer;
    typedef typename InputMaskType::PixelType               InputMaskPixelType;

    typedef TInputDTM                                       InputDTMType;
    typedef typename InputDTMType::Pointer                  InputDTMPointerType;
    typedef typename InputDTMType::ConstPointer             InputDTMConstPointer;
    typedef typename InputDTMType::PixelType                InputDTMPixelType;
    typedef typename InputDTMType::InternalPixelType        InputDTMInternalPixelType;

    /** Look up table template typedefs */
    typedef TLUT                                            LUTType;
    typedef typename LUTType::Pointer                       LUTPointerType;
    typedef typename LUTType::PixelType                     LUTPixelType;
    typedef typename LUTType::IndexType                     LUTIndexType;
    typedef typename LUTType::PointType                     LUTPointType;
    typedef typename LUTPointType::ValueType                LUTPointValueType;
    typedef typename LUTType::ParameterValuesType           LUTParameterValuesType;

    /** Type to use for computations. */
    typedef typename itk::NumericTraits<InternalPixelType>::RealType RealType;
    /** Smart Pointer type to a DataObject. */
    typedef typename itk::DataObject::Pointer               DataObjectPointer;
    typedef itk::SimpleDataObjectDecorator<double>          DoubleObjectType;
    typedef itk::SimpleDataObjectDecorator<bool>            BoolObjectType;

    typedef typename std::vector<double>                    DoubleListType;
    typedef typename std::vector<IndexType>                 IndexListType;
    typedef typename std::vector<InputDTMInternalPixelType>  AltListType;

    // Availability of the composite product parameter accessors
    itkSetMacro(L2inIsAvalaible, bool)

    /** Getters/Setters **/
    vnsSetGetInputRawMacro(IPTOACSub, InputImageType, 0);
    vnsSetGetInputRawMacro(L2CLDSub,  InputMaskType,  1);
    vnsSetGetInputRawMacro(IPSHDSub,  InputMaskType,  2);
    vnsSetGetInputRawMacro(IPEDG,     InputMaskType,  3);
    vnsSetGetInputRawMacro(DTMSub,    InputDTMType,   4);
    vnsSetGetInputRawMacro(L2inEDG,   InputMaskType,  5);
    vnsSetGetInputRawMacro(L2inRTC,   InputImageType, 6);

    itkSetObjectMacro(LUT,LUTType);
    itkGetConstObjectMacro(LUT,LUTType);

    itkSetMacro(DarkestChannelTOAC,unsigned int);
    itkGetConstMacro(DarkestChannelTOAC,unsigned int);

    itkSetMacro(DarkestChannelRTC,unsigned int);
    itkGetConstMacro(DarkestChannelRTC,unsigned int);

    itkSetMacro(DarkSurfaceReflectance,double);
    itkGetConstMacro(DarkSurfaceReflectance,double);

    itkSetMacro(ThresholdDarkPixel,double);
    itkGetConstMacro(ThresholdDarkPixel,double);

    const DoubleObjectType* GetDarkestPixelAOT() const
    {
        return itkDynamicCastInDebugMode<const DoubleObjectType*>(this->itk::ProcessObject::GetOutput(1));
    }

    const BoolObjectType* GetDarkestPixelAOTComputed() const
    {
        return itkDynamicCastInDebugMode<const BoolObjectType*>(this->itk::ProcessObject::GetOutput(2));
    }

    /** Make a DataObject of the correct type to be used as the specified
     * output.
     */
    virtual DataObjectPointer MakeOutput(unsigned int idx);

    /** Pass the input through unmodified. Do this by Grafting in the
     *  AllocateOutputs method.
     */
    virtual void AllocateOutputs(void);
    virtual void GenerateOutputInformation(void);
    virtual void Synthetize(void);
    virtual void Reset(void);

protected:
    /** Constructor */
    PersistentDarkestPixelAOTCalculator();
    /** Destructor */
    virtual ~PersistentDarkestPixelAOTCalculator() {};

    /** Multi-thread version GenerateData. */
    virtual void ThreadedGenerateData (const InputImageRegionType& outputRegionForThread, itk::ThreadIdType threadId);

private:
    PersistentDarkestPixelAOTCalculator(const Self&); //purposely not implemented
    void operator=(const Self&); //purposely not implemented

    /// Availability of the composite image parameter
    bool m_L2inIsAvalaible;

    /// AOT LookUpTable
    LUTPointerType m_LUT;

    unsigned int   m_DarkestChannelTOAC;
    unsigned int   m_DarkestChannelRTC;
    double         m_DarkSurfaceReflectance;
    double         m_ThresholdDarkPixel; // (=0.03)

    DoubleListType m_ThreadMin;
    IndexListType  m_ThreadIndex;
    AltListType    m_ThreadAlt;

    // Outputs
    /*  double m_DarkestPixelAOT          <=> GetOutput(1)
     *  bool   m_DarkestPixelAOTComputed  <=> GetOutput(2)
     */
};  // end of class PersistentConditionalStatisticsVectorImageFilter

/*===========================================================================*/

template<class TInputImage, class TInputDTM, class TInputMask, class TLUT>
class DarkestPixelAOTCalculator : public otb::PersistentFilterStreamingDecorator<
PersistentDarkestPixelAOTCalculator<TInputImage, TInputDTM, TInputMask,TLUT> >
{
public:
    /** Standard Self typedef */
    typedef DarkestPixelAOTCalculator Self;
    typedef otb::PersistentFilterStreamingDecorator
        <PersistentDarkestPixelAOTCalculator<TInputImage,TInputDTM,TInputMask,TLUT> > Superclass;
    typedef itk::SmartPointer<Self> Pointer;
    typedef itk::SmartPointer<const Self> ConstPointer;

    /** Type macro */
    itkNewMacro(Self);

    /** Creation through object factory macro */
    itkTypeMacro(DarkestPixelAOTCalculator,PersistentFilterStreamingDecorator);

    typedef typename Superclass::FilterType StatFilterType;
    typedef typename StatFilterType::RealType RealType;

    /** Some convenient typedefs. */
    typedef TInputImage                           InputImageType;
    typedef typename InputImageType::ConstPointer InputImageConstPointerType;
    typedef typename InputImageType::Pointer      InputImagePointerType;

    typedef TInputMask                            InputMaskType;
    typedef typename InputMaskType::Pointer       InputMaskPointerType;
    typedef typename InputMaskType::ConstPointer  InputMaskConstPointer;
    typedef TInputDTM                             InputDTMType;
    typedef typename InputDTMType::Pointer        InputDTMPointerType;

    /** Look up table template typedefs */
    typedef TLUT                             LUTType;
    typedef typename LUTType::Pointer        LUTPointerType;
    typedef typename LUTType::ConstPointer   LUTConstPointerType;

    /** Availability of the composite product parameter accessors */
    void SetL2inIsAvalaible(const bool pL2inExist)
    {
        this->GetFilter()->SetL2inIsAvalaible(pL2inExist);
    }

    /** Getters/Setters **/
    void SetIPTOACSub(const InputImageType * pIPTOACSub)
    {
        this->GetFilter()->SetIPTOACSub(pIPTOACSub);
    }

    InputImageType* GetIPTOACSub()
    {
        return this->GetFilter()->GetIPTOACSub();
    }

    void SetL2CLDSub(const InputMaskType * pL2CLDSub)
    {
        this->GetFilter()->SetL2CLDSub(pL2CLDSub);
    }

    InputMaskType* GetL2CLDSub()
    {
        return this->GetFilter()->GetL2CLDSub();
    }

    void SetIPSHDSub(const InputMaskType * pIPSHDSub)
    {
        this->GetFilter()->SetIPSHDSub(pIPSHDSub);
    }

    InputMaskType* GetIPSHDSub()
    {
        return this->GetFilter()->GetIPSHDSub();
    }

    void SetIPEDG(const InputMaskType * pIPEDG)
    {
        this->GetFilter()->SetIPEDG(pIPEDG);
    }

    InputMaskType* GetIPEDG()
    {
        return this->GetFilter()->GetIPEDG();
    }

    void SetDTMSub(const InputDTMType * pDTMSub)
    {
        this->GetFilter()->SetDTMSub(pDTMSub);
    }

    InputDTMType* GetDTMSub()
    {
        return this->GetFilter()->GetDTMSub();
    }

    void SetL2inRTC(const InputImageType * pL2inRTC)
    {
        this->GetFilter()->SetL2inRTC(pL2inRTC);
    }

    InputImageType* GetL2inRTC()
    {
        return this->GetFilter()->GetL2inRTC();
    }

    void SetL2inEDG(const InputMaskType * pL2inEDG)
    {
        this->GetFilter()->SetL2inEDG(pL2inEDG);
    }

    InputMaskType* GetL2inEDG()
    {
        return this->GetFilter()->GetL2inEDG();
    }

    // AOT LookUpTable
    void SetLUT(LUTPointerType pParam)
    {
        this->GetFilter()->SetLUT(pParam);
    }

    LUTConstPointerType GetLUT()
    {
        return this->GetFilter()->GetLUT();
    }

    void SetDarkestChannelTOAC(unsigned int pParam)
    {
        this->GetFilter()->SetDarkestChannelTOAC(pParam);
    }

    unsigned int GetDarkestChannelTOAC()
    {
        return this->GetFilter()->GetDarkestChannelTOAC();
    }
    void SetDarkestChannelRTC(unsigned int pParam)
    {
        this->GetFilter()->SetDarkestChannelRTC(pParam);
    }

    unsigned int GetDarkestChannelRTC()
    {
        return this->GetFilter()->GetDarkestChannelRTC();
    }

    void SetDarkSurfaceReflectance(double pParam)
    {
        this->GetFilter()->SetDarkSurfaceReflectance(pParam);
    }

    double GetDarkSurfaceReflectance()
    {
        return this->GetFilter()->GetDarkSurfaceReflectance();
    }

    void SetThresholdDarkPixel(double pParam)
    {
        this->GetFilter()->SetThresholdDarkPixel(pParam);
    }

    double GetThresholdDarkPixel()
    {
        return this->GetFilter()->GetThresholdDarkPixel();
    }

    // Outputs
    double GetDarkestPixelAOT() const
    {
        return this->GetFilter()->GetDarkestPixelAOT()->Get();
    }

    bool GetDarkestPixelAOTComputed() const
    {
        return this->GetFilter()->GetDarkestPixelAOTComputed()->Get();
    }

protected:
    /** Constructor */
    DarkestPixelAOTCalculator()
    {}
    /** Destructor */
    virtual ~DarkestPixelAOTCalculator()
    {}

private:
    DarkestPixelAOTCalculator(const Self&); //purposely not implemented
    void operator=(const Self&); //purposely not implemented
};

} // End namespace vns

#ifndef VNS_MANUAL_INSTANTIATION
#include "vnsDarkestPixelAOTCalculator.txx"
#endif

#endif /* vnsDarkestPixelAOTCalculator_h */
