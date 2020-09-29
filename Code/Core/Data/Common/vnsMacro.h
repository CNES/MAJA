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
 * VERSION : 1.0.0 : FA : LAIG-FA-MAC-148399-CS : 24 octobre 2016 : Audit code - Correction qualite         *
 * VERSION : 1.0.0 : FA : LAIG-FA-MAC-1988-CNES : 18 octobre 2016 : Correction qualite (code mort)          *
 * VERSION : 5-1-0 : FA : LAIG-FA-MAC-145739-CS : 28 juin 2016 : Audit code - Correction du code / clang    *
 * VERSION : 5-0-0 : DM : LAIG-DM-MAC-982-CNES : 8 avril 2016 : Modification de l’algorithme de detection   *
 * VERSION : 4-4-0 : FA : LAIG-FA-MAC-133882-CS : 3 juin 2015 : Correction code pour la qualite             *
 * VERSION : 4-0-0 : FA : LAIG-FA-MAC-117040-CS : 13 mars 2014 : Modifications mineures                     *
 * VERSION : 3-2-0 : FA : LAIG-FA-MAC-116169-CS : 2 decembre 2013 : Configuration pour la compression JPEG  *
 * VERSION : 1-0-0 : <TypeFT> : <NumFT> : 9 avr. 2010 : Creation
 *                                                                                                          *
 * FIN-HISTORIQUE                                                                                           *
 *                                                                                                          *
 * $Id$
 *                                                                                                          *
 ************************************************************************************************************/
#ifndef __vnsMacro_h
#define __vnsMacro_h

#include <vector>
#include <string>
#include <iostream>

#include "itkLightObject.h"
#include "itkMacro.h"
#include "itkSize.h"
#include "itkImageBase.h"
#include "itkImageToImageFilterCommon.h"
#include "itkMetaDataObject.h"

#include "otbImage.h"
#include "otbVectorImage.h"
#include "otbImageFileReader.h"
#include "otbMacro.h"

#include "vnsLoggers.h"

/****************************************************************************
 Venus internal type
 ****************************************************************************/

namespace vns
{
    // No_Data type
    typedef short NoDataType;
    // Real No Data type (internal)
    typedef double RealNoDataType;

    // Scientifics data
    typedef double VNSRealPixelType;
    typedef otb::Image<VNSRealPixelType> VNSRealImageType;
    typedef otb::VectorImage<VNSRealPixelType> VNSRealVectorImageType;

    // Caching typedefs: use for optimize real data (ex: reflectance) writing
    typedef float VNSCachingRealPixelType;
    typedef otb::Image<VNSCachingRealPixelType> VNSCachingRealImageType;
    typedef otb::VectorImage<VNSCachingRealPixelType> VNSCachingRealVectorImageType;

    // Integer data
    typedef otb::Image<short> VNSIntegerImageType;
    typedef otb::VectorImage<short> VNSIntegerVectorImageType;

    // Integer data
    typedef otb::Image<unsigned short> VNSUnsignedIntegerImageType;
    typedef otb::VectorImage<unsigned short> VNSUnsignedIntegerVectorImageType;

    // Mask data
    typedef otb::Image<unsigned char> VNSMaskImageType;
    typedef otb::VectorImage<unsigned char> VNSMaskVectorImageType;

    // DTM data
    //typedef otb::Image<float> VNSDTMImageType;
    typedef otb::Image<double> VNSDTMImageType;

    typedef itk::ImageBase<2>::PointType VNSOriginType;
    typedef itk::ImageBase<2>::SpacingType VNSSpacingType;
    typedef itk::ImageBase<2>::SizeType VNSSizeType;

    typedef class Area
    {
    public:
        // Footprint / Are for images
        typedef VNSOriginType OriginType;
        typedef VNSSpacingType SpacingType;
        typedef VNSSizeType SizeType;
        OriginType Origin;
        SpacingType Spacing;
        SizeType Size;
        void
        PrintSelf(std::ostream& os, itk::Indent /*indent*/) const
        {
            os << "Area :  Origin   : [ " << Origin[0] << " ; " << Origin[1] << " ]" << std::endl;
            os << "        Spacing  : [ " << Spacing[0] << " ; " << Spacing[1] << " ]" << std::endl;
            os << "        Size     : [ " << Size[0] << " ; " << Size[1] << " ]" << std::endl;
        }
    } AreaType;
    typedef std::vector<AreaType> ListOfAreasType;

    typedef class Areas
    {
    public:
        ListOfAreasType FullResolution;
        AreaType CoarseResolution;

        void
        PrintSelf(std::ostream& os, itk::Indent indent) const
        {
            const unsigned int nbResol = FullResolution.size();
            os << "Areas :  Full Resolution (nb=" << nbResol << ")" << std::endl;
            for (unsigned int i = 0; i < nbResol; i++)
            {
                os << indent << "[" << i << "] : ";
                FullResolution[i].PrintSelf(os, indent.GetNextIndent());
            }
            os << indent << "         Coarse Resolution" << std::endl;
            CoarseResolution.PrintSelf(os, indent.GetNextIndent().GetNextIndent());
        }

    } AreasType;

    /** Configuration of the streaming writer : modes */
    typedef enum
    {
        NUMBER_OF_LINES_STRIPPED_STREAMING = 0,
        NUMBER_OF_DIVISIONS_STRIPPED_STREAMING,
        NUMBER_OF_DIVISIONS_TILED_STREAMING,
        AUTOMATIC_STRIPPED_STREAMING,
        AUTOMATIC_TILED_STREAMING,
        AUTOMATIC_ADAPTATIVE_STREAMING,
        INVALID_WRITER_STREAMING_MODE
    } WriterStreamingModeType;

    /** L1 reading configuration mode */
    typedef enum
    {
        READ_L1_MODE_FOR_CHECKTOOL = 0, READ_L1_MODE_FOR_ALGORITHMS, INVALID_READ_L1_MODE
    } ReadL1ModeType;

    /****************************************************************************
     Venus specifics Exception class'
     ****************************************************************************/
    class ExceptionBase : public itk::ExceptionObject
    {
    public:
        /** Default constructor.  Needed to ensure the exception object can be
         * copied. */
        ExceptionBase() :
                itk::ExceptionObject()
        {
        }

        /** Constructor. Needed to ensure the exception object can be copied. */
        ExceptionBase(const char *file, unsigned int lineNumber, const char *desc = "None", const char *loc = "Unknown") :
                itk::ExceptionObject(file, lineNumber, desc, loc)
        {
        }

        /** Constructor. Needed to ensure the exception object can be copied. */
        ExceptionBase(const std::string& file, unsigned int lineNumber, const std::string& desc = "None",
                const std::string& loc = "Unknown") :
                itk::ExceptionObject(file, lineNumber, desc, loc)
        {
        }

        /** Virtual destructor needed for subclasses. Has to have empty throw(). */
        virtual
        ~ExceptionBase() throw ()
        {
        }

        virtual const char *
        GetNameOfClass() const
        {
            return "ExceptionBase";
        }
    };
    class ExceptionChain : public ExceptionBase
    {
    public:
        /** Default constructor.  Needed to ensure the exception object can be
         * copied. */
        ExceptionChain() :
                ExceptionBase()
        {
        }

        /** Constructor. Needed to ensure the exception object can be copied. */
        ExceptionChain(const char *file, unsigned int lineNumber, const char *desc = "None", const char *loc = "Unknown") :
                ExceptionBase(file, lineNumber, desc, loc)
        {
        }

        /** Constructor. Needed to ensure the exception object can be copied. */
        ExceptionChain(const std::string& file, unsigned int lineNumber, const std::string& desc = "None", const std::string& loc =
                "Unknown") :
                ExceptionBase(file, lineNumber, desc, loc)
        {
        }

        /** Virtual destructor needed for subclasses. Has to have empty throw(). */
        virtual
        ~ExceptionChain() throw ()
        {
        }

        virtual const char *
        GetNameOfClass() const
        {
            return "ExceptionChain";
        }
    };
    class ExceptionData : public ExceptionBase
    {
    public:
        /** Default constructor.  Needed to ensure the exception object can be
         * copied. */
        ExceptionData() :
                ExceptionBase()
        {
        }

        /** Constructor. Needed to ensure the exception object can be copied. */
        ExceptionData(const char *file, unsigned int lineNumber, const char *desc = "None", const char *loc = "Unknown") :
                ExceptionBase(file, lineNumber, desc, loc)
        {
        }

        /** Constructor. Needed to ensure the exception object can be copied. */
        ExceptionData(const std::string& file, unsigned int lineNumber, const std::string& desc = "None",
                const std::string& loc = "Unknown") :
                ExceptionBase(file, lineNumber, desc, loc)
        {
        }

        /** Virtual destructor needed for subclasses. Has to have empty throw(). */
        virtual
        ~ExceptionData() throw ()
        {
        }

        virtual const char *
        GetNameOfClass() const
        {
            return "ExceptionData";
        }
    };
    class ExceptionBusiness : public ExceptionBase
    {
    public:
        /** Default constructor.  Needed to ensure the exception object can be
         * copied. */
        ExceptionBusiness() :
                ExceptionBase()
        {
        }

        /** Constructor. Needed to ensure the exception object can be copied. */
        ExceptionBusiness(const char *file, unsigned int lineNumber, const char *desc = "None", const char *loc = "Unknown") :
                ExceptionBase(file, lineNumber, desc, loc)
        {
        }

        /** Constructor. Needed to ensure the exception object can be copied. */
        ExceptionBusiness(const std::string& file, unsigned int lineNumber, const std::string& desc = "None", const std::string& loc =
                "Unknown") :
                ExceptionBase(file, lineNumber, desc, loc)
        {
        }

        /** Virtual destructor needed for subclasses. Has to have empty throw(). */
        virtual
        ~ExceptionBusiness() throw ()
        {
        }

        virtual const char *
        GetNameOfClass() const
        {
            return "ExceptionBusiness";
        }
    };
    class ExceptionPluginBase : public ExceptionBase
    {
    public:
        /** Default constructor.  Needed to ensure the exception object can be
         * copied. */
        ExceptionPluginBase() :
                ExceptionBase()
        {
        }

        /** Constructor. Needed to ensure the exception object can be copied. */
        ExceptionPluginBase(const char *file, unsigned int lineNumber, const char *desc = "None", const char *loc = "Unknown") :
                ExceptionBase(file, lineNumber, desc, loc)
        {
        }

        /** Constructor. Needed to ensure the exception object can be copied. */
        ExceptionPluginBase(const std::string& file, unsigned int lineNumber, const std::string& desc = "None", const std::string& loc =
                "Unknown") :
                ExceptionBase(file, lineNumber, desc, loc)
        {
        }

        /** Virtual destructor needed for subclasses. Has to have empty throw(). */
        virtual
        ~ExceptionPluginBase() throw ()
        {
        }

        virtual const char *
        GetNameOfClass() const
        {
            return "ExceptionPluginBase";
        }
    };

    /** Class use to stop Algorithms processing (call from ScientificProcessing() method) */
    class ExceptionAlgorithmsProcessing : public ExceptionBase
    {
    public:
        /** Default constructor.  Needed to ensure the exception object can be
         * copied. */
        ExceptionAlgorithmsProcessing() :
                ExceptionBase()
        {
        }

        /** Constructor. Needed to ensure the exception object can be copied. */
        ExceptionAlgorithmsProcessing(const char *file, unsigned int lineNumber, const char *desc = "None", const char *loc = "Unknown") :
                ExceptionBase(file, lineNumber, desc, loc)
        {
        }

        /** Constructor. Needed to ensure the exception object can be copied. */
        ExceptionAlgorithmsProcessing(const std::string& file, unsigned int lineNumber, const std::string& desc = "None",
                const std::string& loc = "Unknown") :
                ExceptionBase(file, lineNumber, desc, loc)
        {
        }

        /** Virtual destructor needed for subclasses. Has to have empty throw(). */
        virtual
        ~ExceptionAlgorithmsProcessing() throw ()
        {
        }

        virtual const char *
        GetNameOfClass() const
        {
            return "ExceptionAlgorithmsProcessing";
        }
    };

    /** Class use to stop chain from CommandLine parser exit true (--help, ...) */
    class ExceptionParseCommandLineSuccess : public ExceptionBase
    {
    public:
        /** Default constructor.  Needed to ensure the exception object can be
         * copied. */
        ExceptionParseCommandLineSuccess() :
                ExceptionBase()
        {
        }

        /** Constructor. Needed to ensure the exception object can be copied. */
        ExceptionParseCommandLineSuccess(const char *file, unsigned int lineNumber, const char *desc = "None", const char *loc = "Unknown") :
                ExceptionBase(file, lineNumber, desc, loc)
        {
        }

        /** Constructor. Needed to ensure the exception object can be copied. */
        ExceptionParseCommandLineSuccess(const std::string& file, unsigned int lineNumber, const std::string& desc = "None",
                const std::string& loc = "Unknown") :
                ExceptionBase(file, lineNumber, desc, loc)
        {
        }

        /** Virtual destructor needed for subclasses. Has to have empty throw(). */
        virtual
        ~ExceptionParseCommandLineSuccess() throw ()
        {
        }

        virtual const char *
        GetNameOfClass() const
        {
            return "ExceptionParseCommandLineSuccess";
        }
    };

    /** Class use to stop chain from CommandLine parser exit false (--help, ...) */
    class ExceptionParseCommandLineFailure : public ExceptionBase
    {
    public:
        /** Default constructor.  Needed to ensure the exception object can be
         * copied. */
        ExceptionParseCommandLineFailure() :
                ExceptionBase()
        {
        }

        /** Constructor. Needed to ensure the exception object can be copied. */
        ExceptionParseCommandLineFailure(const char *file, unsigned int lineNumber, const char *desc = "None", const char *loc = "Unknown") :
                ExceptionBase(file, lineNumber, desc, loc)
        {
        }

        /** Constructor. Needed to ensure the exception object can be copied. */
        ExceptionParseCommandLineFailure(const std::string& file, unsigned int lineNumber, const std::string& desc = "None",
                const std::string& loc = "Unknown") :
                ExceptionBase(file, lineNumber, desc, loc)
        {
        }

        /** Virtual destructor needed for subclasses. Has to have empty throw(). */
        virtual
        ~ExceptionParseCommandLineFailure() throw ()
        {
        }

        virtual const char *
        GetNameOfClass() const
        {
            return "ExceptionParseCommandLineFailure";
        }
    };

/****************************************************************************
 Specifics Set/Get accessor without Debug macro
 ****************************************************************************/

/** Set built-in type.  Creates member Set"name"() (e.g., SetVisibility()); */
#define vnsSetMacro(name,type) \
  virtual void Set##name (const type & _arg) \
  { \
    /* ---------------------------------------------------------------------------------------------*/ \
    /* Check arg value */ \
    if (this->m_##name != _arg) \
      { \
        /* ---------------------------------------------------------------------------------------------*/ \
        /* set the value */ \
        this->m_##name = _arg; \
        /* ---------------------------------------------------------------------------------------------*/ \
        /* Set to the itk callback that object is modified */ \
        this->Modified(); \
      } \
  }

/** Set built-in type.  Creates member Set"name"() (e.g., SetVisibility());
 *  This variant does not perform any test !=
 *  Without this->Modified() */
#define vnsSetRawMacro(name,type) \
  virtual void Set##name (const type &_arg) \
  { \
    /* ---------------------------------------------------------------------------------------------*/ \
    /* set the value */ \
      this->m_##name = _arg; \
  }

#define vnsSetDirectMacro(name,type) \
  virtual void Set##name (const type &_arg) \
  { \
    /* ---------------------------------------------------------------------------------------------*/ \
    /* set the value */ \
      this->m_##name = _arg; \
      /* ---------------------------------------------------------------------------------------------*/ \
      /* Set to the itk callback that object is modified */ \
      this->Modified(); \
  }

/** Set built-in type with set control.  Creates member Set"name"() (e.g., SetVisibility()); */
#define vnsSetMacroWithSetControl(name,type) \
  virtual void Set##name (const type &_arg) \
  { \
    /* ---------------------------------------------------------------------------------------------*/ \
    /* set the value */ \
      this->m_##name = _arg; \
      /* ---------------------------------------------------------------------------------------------*/ \
      /* set the boolean value */ \
      this->m_##name##_isSet = true; \
      /* ---------------------------------------------------------------------------------------------*/ \
      /* Set to the itk callback that object is modified */ \
      this->Modified(); \
  }

/** Set character string with Set control.  Creates member Set"name"()
 * (e.g., SetFilename(char *)). The macro assumes that
 * the class member (name) is declared a type std::string. */
#define vnsSetStringMacroWithSetControl(name) \
  virtual void Set##name (const char* _arg) \
  { \
    /* Check arg value */ \
    if (_arg) \
      { \
        /* ---------------------------------------------------------------------------------------------*/ \
        /* set the value */ \
      this->m_##name = _arg;\
      /* ---------------------------------------------------------------------------------------------*/ \
      /* set the boolean value */ \
      this->m_##name##_isSet = true; \
      } \
     else \
      { \
         /* ---------------------------------------------------------------------------------------------*/ \
         /* set the value */ \
      this->m_##name = ""; \
      /* ---------------------------------------------------------------------------------------------*/ \
      /* set the boolean value */ \
      this->m_##name##_isSet = true; \
      } \
      /* ---------------------------------------------------------------------------------------------*/ \
      /* Set to the itk callback that object is modified */ \
    this->Modified(); \
  } \
  virtual void Set##name (const std::string & _arg) \
  { \
      /* ---------------------------------------------------------------------------------------------*/ \
      /* set the value */ \
    this->Set##name( _arg.c_str() ); \
  }

/** Get built-in type.  Creates member Get"name"() (e.g., GetVisibility()); */
#define vnsGetMacro(name,type) \
  type Get##name () \
  { \
    /* ---------------------------------------------------------------------------------------------*/ \
    /* get the value */ \
    return this->m_##name; \
  }

/** Get built-in type.  Creates member Get"name"() (e.g., GetVisibility());
 * This is the "const" form of the itkGetMacro.  It should be used unless
 * the member can be changed through the "Get" access routine. */
#define vnsGetConstMacro(name,type) \
  type Get##name () const \
  { \
    /* ---------------------------------------------------------------------------------------------*/ \
    /* get the value */ \
    return this->m_##name; \
  }

/** Get built-in type.  Creates member Get"name"() (e.g., GetVisibility());
 * This is the "const" form of the itkGetMacro.  It should be used unless
 * the member can be changed through the "Get" access routine.
 * This versions returns a const reference to the variable. */
#define vnsGetConstReferenceMacro(name,type) \
  const type & Get##name () const \
  { \
    /* ---------------------------------------------------------------------------------------------*/ \
    /* get the value */ \
    return this->m_##name; \
  }

#define vnsGetReferenceConstMacro(name,type) \
  const type & Get##name () \
  { \
    /* ---------------------------------------------------------------------------------------------*/ \
    /* get the value */ \
    return this->m_##name; \
  }

//             itkSetObjectMacro(name, type);

#define vnsSetConstObjectMacro(name,type) \
  virtual void Set##name (const type* _arg) \
  { \
    itkDebugMacro("setting " << #name " to " << _arg ); \
    if (this->m_##name != _arg) \
      { \
        /* ---------------------------------------------------------------------------------------------*/ \
        /* set the value */ \
      this->m_##name = const_cast<type *> (_arg); \
      /* ---------------------------------------------------------------------------------------------*/ \
      /* Set to the itk callback that object is modified */ \
      this->Modified(); \
      } \
  }

#define vnsConstMemberObjectMacro(name, type) \
            itkSetConstObjectMacro(name, type); \
            itkGetConstObjectMacro(name, type); \
        private: \
            /* ---------------------------------------------------------------------------------------------*/ \
            /* declare the member */ \
            typename type ::ConstPointer m_##name; \
        public:

#define vnsMemberSetGetObjectMacro(name, type) \
            /* ---------------------------------------------------------------------------------------------*/ \
            vnsSetConstObjectMacro(name, type); \
            /* ---------------------------------------------------------------------------------------------*/ \
            itkSetObjectMacro(name, type); \
            /* ---------------------------------------------------------------------------------------------*/ \
            itkGetObjectMacro(name, type); \
            /* ---------------------------------------------------------------------------------------------*/ \
            itkGetConstObjectMacro(name, type); \
        private: \
            /* ---------------------------------------------------------------------------------------------*/ \
            /* declare the member */ \
            typename type ::Pointer m_##name; \
        public:

#define vnsCxxMemberSetGetObjectMacro(name, type) \
            /* ---------------------------------------------------------------------------------------------*/ \
            vnsSetConstObjectMacro(name, type); \
            /* ---------------------------------------------------------------------------------------------*/ \
            itkSetObjectMacro(name, type); \
            /* ---------------------------------------------------------------------------------------------*/ \
            itkGetObjectMacro(name, type); \
            /* ---------------------------------------------------------------------------------------------*/ \
            itkGetConstObjectMacro(name, type); \
        private: \
            /* ---------------------------------------------------------------------------------------------*/ \
            /* declare the member */ \
            type ::Pointer m_##name; \
        public:

#define vnsCxxMemberGetObjectMacro(name, type) \
            itkGetObjectMacro(name, type); \
            itkGetConstObjectMacro(name, type); \
        private: \
            /* ---------------------------------------------------------------------------------------------*/ \
            /* declare the member */ \
            type ::Pointer m_##name; \
        public:

#define vnsMemberGetObjectMacro(name, type) \
            itkGetObjectMacro(name, type); \
            itkGetConstObjectMacro(name, type); \
        private: \
            /* ---------------------------------------------------------------------------------------------*/ \
            /* declare the member */ \
            typename type ::Pointer m_##name; \
        public:

#define vnsMemberAndGetConstReferenceMacro(name, type) \
            /* ---------------------------------------------------------------------------------------------*/ \
            vnsGetConstReferenceMacro(name, type); \
            /* ---------------------------------------------------------------------------------------------*/ \
        private: \
            /* ---------------------------------------------------------------------------------------------*/ \
            /* declare the member */ \
            type m_##name; \
            /* ---------------------------------------------------------------------------------------------*/ \
        public:

#define vnsProtectedMemberAndGetConstReferenceMacro(name, type) \
            /* ---------------------------------------------------------------------------------------------*/ \
            vnsGetConstReferenceMacro(name, type); \
            /* ---------------------------------------------------------------------------------------------*/ \
        protected: \
            /* ---------------------------------------------------------------------------------------------*/ \
            /* declare the member */ \
            type m_##name; \
            /* ---------------------------------------------------------------------------------------------*/ \
        public:

#define vnsMemberAndDirectSetAndGetConstReferenceMacro(name, type) \
            /* ---------------------------------------------------------------------------------------------*/ \
            vnsSetDirectMacro(name, type) \
            /* ---------------------------------------------------------------------------------------------*/ \
            vnsGetConstReferenceMacro(name, type); \
            /* ---------------------------------------------------------------------------------------------*/ \
            vnsGetReferenceConstMacro(name, type); \
            /* ---------------------------------------------------------------------------------------------*/ \
        private: \
            /* ---------------------------------------------------------------------------------------------*/ \
            /* declare the member */ \
            type m_##name; \
        public:

#define vnsMemberAndSetAndGetConstReferenceMacro(name, type) \
            /* ---------------------------------------------------------------------------------------------*/ \
            vnsSetMacro(name, type) \
            /* ---------------------------------------------------------------------------------------------*/ \
            vnsGetConstReferenceMacro(name, type); \
            /* ---------------------------------------------------------------------------------------------*/ \
            vnsGetReferenceConstMacro(name, type); \
        private: \
            /* ---------------------------------------------------------------------------------------------*/ \
            /* declare the member */ \
            type m_##name; \
        public:

#define vnsProtectedMemberAndSetAndGetConstReferenceMacro(name, type) \
            /* ---------------------------------------------------------------------------------------------*/ \
            vnsSetMacro(name, type) \
            /* ---------------------------------------------------------------------------------------------*/ \
            vnsGetConstReferenceMacro(name, type); \
            /* ---------------------------------------------------------------------------------------------*/ \
            vnsGetReferenceConstMacro(name, type); \
        protected: \
            /* ---------------------------------------------------------------------------------------------*/ \
            /* declare the member */ \
            type m_##name; \
        public:

#define vnsProtectedMemberAndSetDirectAndGetConstReferenceMacro(name, type) \
            /* ---------------------------------------------------------------------------------------------*/ \
            vnsSetDirectMacro(name, type) \
            /* ---------------------------------------------------------------------------------------------*/ \
            vnsGetConstReferenceMacro(name, type); \
            /* ---------------------------------------------------------------------------------------------*/ \
            vnsGetReferenceConstMacro(name, type); \
        protected: \
            /* ---------------------------------------------------------------------------------------------*/ \
            /* declare the member */ \
            type m_##name; \
        public:

#define vnsStringMemberAndGetConstReferenceMacro(name) \
            /* ---------------------------------------------------------------------------------------------*/ \
            vnsGetConstReferenceMacro(name, std::string); \
        private: \
            /* ---------------------------------------------------------------------------------------------*/ \
            /* declare the member */ \
            std::string m_##name; \
        public:

#define vnsStringMemberAndSetAndGetConstReferenceMacro(name) \
            /* ---------------------------------------------------------------------------------------------*/ \
            vnsSetMacro(name, std::string) \
            /* ---------------------------------------------------------------------------------------------*/ \
            vnsGetConstReferenceMacro(name, std::string); \
            /* ---------------------------------------------------------------------------------------------*/ \
            vnsGetReferenceConstMacro(name, std::string); \
        private: \
            /* ---------------------------------------------------------------------------------------------*/ \
            /* declare the member */ \
            std::string m_##name; \
        public:

#define vnsProtectedStringMemberAndSetAndGetConstReferenceMacro(name) \
            /* ---------------------------------------------------------------------------------------------*/ \
            vnsSetMacro(name, std::string) \
            /* ---------------------------------------------------------------------------------------------*/ \
            vnsGetConstReferenceMacro(name, std::string); \
            /* ---------------------------------------------------------------------------------------------*/ \
            vnsGetReferenceConstMacro(name, std::string); \
        protected: \
            /* ---------------------------------------------------------------------------------------------*/ \
            /* declare the member */ \
            std::string m_##name; \
        public:

#define vnsMemberAndSetMacro(name, type) \
            /* ---------------------------------------------------------------------------------------------*/ \
            vnsSetMacro(name, type) \
        protected: \
            /* ---------------------------------------------------------------------------------------------*/ \
            /* declare the member */ \
            type m_##name; \
        public:

#define vnsProtectedMemberAndGetConstReferenceMacro(name, type) \
            /* ---------------------------------------------------------------------------------------------*/ \
            vnsGetConstReferenceMacro(name, type); \
            /* ---------------------------------------------------------------------------------------------*/ \
        protected: \
            /* ---------------------------------------------------------------------------------------------*/ \
            /* declare the member */ \
            type m_##name; \
        public:

#define vnsProtectedStringMemberAndGetConstReferenceMacro(name) \
            /* ---------------------------------------------------------------------------------------------*/ \
            vnsGetConstReferenceMacro(name, std::string); \
            /* ---------------------------------------------------------------------------------------------*/ \
        protected: \
            /* ---------------------------------------------------------------------------------------------*/ \
            /* declare the member */ \
            std::string m_##name; \
        public:

#define vnsUIntMemberAndGetConstReferenceMacro(name) \
        vnsMemberAndSetAndGetConstReferenceMacro(name, unsigned int) \

#define vnsBooleanMemberAndGetConstReferenceMacro(name) \
        vnsMemberAndSetAndGetConstReferenceMacro(name, bool) \
        itkBooleanMacro(name)

#define vnsProtectedBooleanMemberAndGetConstReferenceMacro(name) \
        vnsProtectedMemberAndSetAndGetConstReferenceMacro(name, bool) \
        itkBooleanMacro(name)

#define vnsBooleanMemberAndSetAndGetConstReferenceMacro(name) \
        vnsMemberAndSetAndGetConstReferenceMacro(name, bool) \
        itkBooleanMacro(name)

#define vnsPublicMemberAndSetAndGetConstReferenceMacro(name, type) \
            /* ---------------------------------------------------------------------------------------------*/ \
            vnsGetConstReferenceMacro(name, type); \
            /* ---------------------------------------------------------------------------------------------*/ \
            vnsSetRawMacro(name, type) \
            /* ---------------------------------------------------------------------------------------------*/ \
            /* declare the member */ \
            type m_##name; \
            /* ---------------------------------------------------------------------------------------------*/ \


/** Macro to define Set/GetImage method */
#define vnsSetGetImageMacro(name,type,index) \
        /* ---------------------------------------------------------------------------------------------*/ \
        /* Set the ##name## Input image */ \
        void Set##name##Image( const type *object) \
        {\
            /* set the pointer */ \
            this->itk::ProcessObject::SetNthInput(index,const_cast<type*>(object)); \
        } \
        /* ---------------------------------------------------------------------------------------------*/ \
        /* Get the ##name## Input image */ \
        type * Get##name##Image() \
        { \
            /* return the pointer */ \
            return itkDynamicCastInDebugMode< type * >( this->itk::ProcessObject::GetInput(index) ); \
        } \
        /* ---------------------------------------------------------------------------------------------*/ \
        /* Get the ##name## Input image */ \
        const type * Get##name##Image() const \
        { \
            /* return the pointer */ \
            return itkDynamicCastInDebugMode< const type * >( this->itk::ProcessObject::GetInput(index) ); \
        }

/** Macro to define Set/GetInput method */
#define vnsSetGetInputMacro(name,type,index) \
        /* ---------------------------------------------------------------------------------------------*/ \
        /* Set the ##name## Input image */ \
        void Set##name##Input( const type *object) \
        {\
            /* set the pointer */ \
            this->itk::ProcessObject::SetNthInput(index,const_cast<type*>(object)); \
        } \
        /* ---------------------------------------------------------------------------------------------*/ \
        /* Get the ##name## Input image */ \
        type * Get##name##Input() \
        { \
            /* return the pointer */ \
            return itkDynamicCastInDebugMode< type * >( this->itk::ProcessObject::GetInput(index) ); \
        } \
        /* ---------------------------------------------------------------------------------------------*/ \
        /* Get the ##name## Input image */ \
        const type * Get##name##Input() const \
        { \
            /* return the pointer */ \
            return itkDynamicCastInDebugMode< const type * >( this->itk::ProcessObject::GetInput(index) ); \
        }

/** Macro to define Set/Get method witout Input in the Set/Get method name
 * TODO : deprecated : modify the code and use only remplace vnsSetGetInputRawMacro by  vnsSetGetInputMacro*/
#define vnsSetGetInputRawMacro(name,type,index) \
        /* ---------------------------------------------------------------------------------------------*/ \
        /* Set the ##name## Input image */ \
        void Set##name( const type *object) \
        {\
            /* set the pointer */ \
            this->itk::ProcessObject::SetNthInput(index,const_cast<type*>(object)); \
        } \
        /* ---------------------------------------------------------------------------------------------*/ \
        /* Get the ##name## Input image */ \
        type * Get##name() \
        { \
            /* return the pointer */ \
            return itkDynamicCastInDebugMode< type * >( this->itk::ProcessObject::GetInput(index) ); \
        } \
        /* ---------------------------------------------------------------------------------------------*/ \
        /* Get the ##name## Input image */ \
        const type * Get##name() const \
        { \
            /* return the pointer */ \
            return itkDynamicCastInDebugMode< const type * >( this->itk::ProcessObject::GetInput(index) ); \
        }

/****************************************************************************
 Macros for throwing exceptions
 ****************************************************************************/

#define vnsStaticExceptionMacro(x) {  \
/* Create Exception */ \
std::ostringstream message_exception;   \
message_exception << "vns::ERROR: " \
                  << ": " << x; \
vnsLogErrorMacro(message_exception.str()); \
::vns::ExceptionBase e_(__FILE__, __LINE__, message_exception.str().c_str(),ITK_LOCATION); \
throw e_;  /* Explicit naming to work around Intel compiler bug.  */ }

#define vnsStaticExceptionDataMacro(x) {  \
/* Create Exception */ \
std::ostringstream message_exception;   \
message_exception << "vns::Data::ERROR: " \
                  << ": " << x; \
vnsLogErrorMacro(message_exception.str()); \
::vns::ExceptionData e_(__FILE__, __LINE__, message_exception.str().c_str(),ITK_LOCATION); \
throw e_;  /* Explicit naming to work around Intel compiler bug.  */ }

#define vnsStaticExceptionBusinessMacro(x) {  \
/* Create Exception */ \
std::ostringstream message_exception;   \
message_exception << "vns::Business::ERROR: " \
                  << ": " << x; \
vnsLogErrorMacro(message_exception.str()); \
::vns::ExceptionBusiness e_(__FILE__, __LINE__, message_exception.str().c_str(),ITK_LOCATION); \
throw e_;  /* Explicit naming to work around Intel compiler bug.  */ }

#define vnsStaticExceptionChainMacro(x) {  \
/* Create Exception */ \
std::ostringstream message_exception;   \
message_exception << "vns::Chain::ERROR: " \
                  << ": " << x; \
/* send the message to the logger */ \
vnsLogErrorMacro(message_exception.str()); \
::vns::ExceptionChain e_(__FILE__, __LINE__, message_exception.str().c_str(),ITK_LOCATION); \
throw e_;  /* Explicit naming to work around Intel compiler bug.  */ }

#define vnsStaticExceptionPluginBaseMacro(x) {  \
/* Create Exception */ \
std::ostringstream message_exception;   \
message_exception << "vns::Plugin::ERROR: " \
                  << ": " << x; \
/* send the message to the logger */ \
vnsLogErrorMacro(message_exception.str()); \
::vns::ExceptionPluginBase e_(__FILE__, __LINE__, message_exception.str().c_str(),ITK_LOCATION); \
throw e_;  /* Explicit naming to work around Intel compiler bug.  */ }

#define vnsExceptionMacro(x) {   \
/* Create Exception */ \
std::ostringstream message_exception;   \
message_exception << "vns::ERROR: " << this->GetNameOfClass() \
                  << "(" << this << "): " << x; \
/* send the message to the logger */ \
vnsLogErrorMacro(message_exception.str()); \
::vns::ExceptionBase e_(__FILE__, __LINE__, message_exception.str().c_str(),ITK_LOCATION); \
throw e_;  /* Explicit naming to work around Intel compiler bug.  */ }

#define vnsExceptionChainMacro(x) {   \
/* Create Exception */ \
std::ostringstream message_exception;   \
message_exception << "vns::Chain::ERROR: " << this->GetNameOfClass() \
                  << "(" << this << "): " << x; \
/* send the message to the logger */ \
vnsLogErrorMacro(message_exception.str()); \
::vns::ExceptionChain e_(__FILE__, __LINE__, message_exception.str().c_str(),ITK_LOCATION); \
throw e_;  /* Explicit naming to work around Intel compiler bug.  */ }

#define vnsExceptionPluginBaseMacro(x) {   \
/* Create Exception */ \
std::ostringstream message_exception;   \
message_exception << "vns::Plugin::ERROR: " << this->GetNameOfClass() \
                  << "(" << this << "): " << x; \
/* send the message to the logger */ \
vnsLogErrorMacro(message_exception.str()); \
::vns::ExceptionPluginBase e_(__FILE__, __LINE__, message_exception.str().c_str(),ITK_LOCATION); \
throw e_;  /* Explicit naming to work around Intel compiler bug.  */ }

#define vnsExceptionBusinessMacro(x) {   \
/* Create Exception */ \
std::ostringstream message_exception;   \
message_exception << "vns::Business::ERROR: " << this->GetNameOfClass() \
                  << "(" << this << "): " << x; \
/* send the message to the logger */ \
vnsLogErrorMacro(message_exception.str()); \
::vns::ExceptionBusiness e_(__FILE__, __LINE__, message_exception.str().c_str(),ITK_LOCATION); \
throw e_;  /* Explicit naming to work around Intel compiler bug.  */ }

#define vnsExceptionDataMacro(x) {   \
/* Create Exception */ \
std::ostringstream message_exception;   \
message_exception << "vns::Data::ERROR: " << this->GetNameOfClass() \
                  << "(" << this << "): " << x; \
/* send the message to the logger */ \
vnsLogErrorMacro(message_exception.str()); \
::vns::ExceptionData e_(__FILE__, __LINE__, message_exception.str().c_str(),ITK_LOCATION); \
throw e_;  /* Explicit naming to work around Intel compiler bug.  */ }

/** Algorithm exception: use to stop algorithms (ScientifProcessing), but execute Post Processing to generate a complet product */
#define vnsExceptionAlgorithmsProcessingMacro(x) {   \
/* Create Exception */ \
std::ostringstream message_exception;   \
message_exception << x << " (AlgorithmsProcessing " << this->GetNameOfClass() << ")."; \
::vns::ExceptionAlgorithmsProcessing e_(__FILE__, __LINE__, message_exception.str().c_str(),ITK_LOCATION); \
throw e_;  /* Explicit naming to work around Intel compiler bug.  */ }

/** Macros to Check images size (LargestPossibleRegion()) */

#define vnsChecking2SizeMacro(Size1, Size2) \
    /** Check the images size */ \
    if ( Size1 != Size2 ) \
    { \
        vnsExceptionDataMacro("The sizes '"<<#Size1<<"' (size "<<Size1<<") and '"<<#Size2<<"' (size "<<Size2<<") hare not identical!"); \
    }
#define vnsChecking2ImagesSizeMacro(Image1, Image2) { \
        /** Check the images size */ \
        if ( Image1->GetLargestPossibleRegion().GetSize() != Image2->GetLargestPossibleRegion().GetSize() ) \
        { \
            vnsExceptionDataMacro("The images '"<<#Image1<<"' (size "<<Image1->GetLargestPossibleRegion().GetSize()<<") and '"<<#Image2<<"' (size "<<Image2->GetLargestPossibleRegion().GetSize()<<") have not the same size !"); \
        } \
        const double coordinateTol = itk::ImageToImageFilterCommon::GetGlobalDefaultCoordinateTolerance() * Image1->GetSpacing()[0]; \
        /** Check the images origin */ \
        if ( (vcl_abs(Image1->GetOrigin()[0] - Image2->GetOrigin()[0]) >  coordinateTol) || (vcl_abs(Image1->GetOrigin()[1] - Image2->GetOrigin()[1]) > coordinateTol) ) \
        { \
            vnsExceptionDataMacro("The images '"<<#Image1<<"' (origin "<<Image1->GetOrigin() <<") and '"<<#Image2<<"' (origin "<<Image2->GetOrigin()<<") have not the same origin (tolerance=" << coordinateTol << ")!"); \
        } \
        /** Check the images spacing */ \
        if ( (vcl_abs(Image1->GetSpacing()[0] - Image2->GetSpacing()[0]) > coordinateTol ) || (vcl_abs(Image1->GetSpacing()[1] - Image2->GetSpacing()[1]) > coordinateTol) ) \
        { \
            vnsExceptionDataMacro("The images '"<<#Image1<<"' (spacing "<<Image1->GetSpacing()<<") and '"<<#Image2<<"' (spacing "<<Image2->GetSpacing()<<") have not the same spacing (tolerance=" << coordinateTol << ")!"); \
        }\
        }



// Macros used to simplify the interface of the attributes in the XML file handler
#define vnsSetAndGetStringValueMacro(name, xpath) \
        /* set the value for the Xpath */ \
        void Set##name(const std::string & value) \
        { \
            /* set the string value for the Xpath */ \
            this->SetStringValue(xpath, value); \
        } \
        /* get the value for the Xpath */ \
        std::string Get##name() const \
        { \
            /* get the string value for the Xpath */ \
            return this->GetStringValue(xpath); \
        }

#define vnsSetAndGetStringPrivateValueMacro(name, xpath) \
        private : \
        /* set the string value for the Xpath */ \
        void Set##name(const std::string & value) \
        { \
            /* set the string value for the Xpath */ \
            this->SetStringValue(xpath, value); \
        } \
        /* Get the string value for the Xpath */ \
        std::string Get##name() const \
        { \
            /* set the string value for the Xpath */ \
            return this->GetStringValue(xpath); \
        } \
        public :

#define vnsSetAndGetBoolValueMacro(name, xpath) \
        /* set the value for the Xpath */ \
        void Set##name(const bool & value) \
        { \
            /* set the value for the Xpath */ \
            this->SetBoolValue(xpath, value); \
        } \
        /* get the value for the Xpath */ \
        bool Get##name() const \
        { \
            /* get the value for the Xpath */ \
            return this->GetBoolValue(xpath); \
        }
#define vnsSetAndGetDoubleValueMacro(name, xpath) \
        /* set the value for the Xpath */ \
        void Set##name(const double & value) \
        { \
            /* set the value for the Xpath */ \
            this->SetDoubleValue(xpath, value); \
        } \
        /* get the value for the Xpath */ \
        double Get##name() const \
        { \
            /* get the value for the Xpath */ \
            return this->GetDoubleValue(xpath); \
        }
#define vnsSetAndGetUShortValueMacro(name, xpath) \
        /* set the value for the Xpath */ \
        void Set##name(const unsigned short & value) \
        { \
            /* set the value for the Xpath */ \
            this->SetUShortValue(xpath, value); \
        } \
        /* get the value for the Xpath */ \
        unsigned short Get##name() const \
        { \
            /* get the value for the Xpath */ \
            return this->GetUShortValue(xpath); \
        }
#define vnsSetAndGetIntValueMacro(name, xpath) \
        /* set the value for the Xpath */ \
        void Set##name(const int & value) \
        { \
            /* set the value for the Xpath */ \
            this->SetIntValue(xpath, value); \
        } \
        /* get the value for the Xpath */ \
        int Get##name() const \
        { \
            /* get the value for the Xpath */ \
            return this->GetIntValue(xpath); \
        }

#define vnsChecking3ImagesSizeMacro(Image1, Image2, Image3) \
        vnsChecking2ImagesSizeMacro(Image1, Image2) \
        vnsChecking2ImagesSizeMacro(Image1, Image3)

#define vnsChecking4ImagesSizeMacro(Image1, Image2, Image3, Image4) \
        vnsChecking3ImagesSizeMacro(Image1, Image2, Image3) \
        vnsChecking2ImagesSizeMacro(Image1, Image4)

#define vnsChecking5ImagesSizeMacro(Image1, Image2, Image3, Image4, Image5) \
        vnsChecking4ImagesSizeMacro(Image1, Image2, Image3, Image4) \
        vnsChecking2ImagesSizeMacro(Image1, Image5)

#define vnsChecking6ImagesSizeMacro(Image1, Image2, Image3, Image4, Image5, Image6) \
        vnsChecking5ImagesSizeMacro(Image1, Image2, Image3, Image4, Image5) \
        vnsChecking2ImagesSizeMacro(Image1, Image6)

#define vnsChecking7ImagesSizeMacro(Image1, Image2, Image3, Image4, Image5, Image6, Image7) \
        vnsChecking6ImagesSizeMacro(Image1, Image2, Image3, Image4, Image5, Image6) \
        vnsChecking2ImagesSizeMacro(Image1, Image7)

#define vnsChecking8ImagesSizeMacro(Image1, Image2, Image3, Image4, Image5, Image6, Image7, Image8) \
        vnsChecking7ImagesSizeMacro(Image1, Image2, Image3, Image4, Image5, Image6, Image7) \
        vnsChecking2ImagesSizeMacro(Image1, Image8)

#define vnsChecking9ImagesSizeMacro(Image1, Image2, Image3, Image4, Image5, Image6, Image7, Image8, Image9) \
        vnsChecking8ImagesSizeMacro(Image1, Image2, Image3, Image4, Image5, Image6, Image7, Image8) \
        vnsChecking2ImagesSizeMacro(Image1, Image9)

#define vnsChecking10ImagesSizeMacro(Image1, Image2, Image3, Image4, Image5, Image6, Image7, Image8, Image9, Image10) \
        vnsChecking9ImagesSizeMacro(Image1, Image2, Image3, Image4, Image5, Image6, Image7, Image8, Image9) \
        vnsChecking2ImagesSizeMacro(Image1, Image10)

#define vnsChecking11ImagesSizeMacro(Image1, Image2, Image3, Image4, Image5, Image6, Image7, Image8, Image9, Image10, Image11) \
        vnsChecking10ImagesSizeMacro(Image1, Image2, Image3, Image4, Image5, Image6, Image7, Image8, Image9, Image10) \
        vnsChecking2ImagesSizeMacro(Image1, Image11)

#define vnsChecking12ImagesSizeMacro(Image1, Image2, Image3, Image4, Image5, Image6, Image7, Image8, Image9, Image10, Image11, Image12) \
        vnsChecking11ImagesSizeMacro(Image1, Image2, Image3, Image4, Image5, Image6, Image7, Image8, Image9, Image10, Image11) \
        vnsChecking2ImagesSizeMacro(Image1, Image12)

#define vnsChecking13ImagesSizeMacro(Image1, Image2, Image3, Image4, Image5, Image6, Image7, Image8, Image9, Image10, Image11, Image12, Image13) \
        vnsChecking12ImagesSizeMacro(Image1, Image2, Image3, Image4, Image5, Image6, Image7, Image8, Image9, Image10, Image11, Image12) \
        vnsChecking2ImagesSizeMacro(Image1, Image13)

#define vnsChecking14ImagesSizeMacro(Image1, Image2, Image3, Image4, Image5, Image6, Image7, Image8, Image9, Image10, Image11, Image12, Image13, Image14) \
        vnsChecking13ImagesSizeMacro(Image1, Image2, Image3, Image4, Image5, Image6, Image7, Image8, Image9, Image10, Image11, Image12, Image13) \
        vnsChecking2ImagesSizeMacro(Image1, Image14)

} // namespace vns

#endif /* __vnsMacro_h */
