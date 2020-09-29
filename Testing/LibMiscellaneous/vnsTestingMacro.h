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
 * VERSION : 5-0-0 : DM : LAIG-DM-MAC-982-CNES : 8 avril 2016 : Modification de l’algorithme de detection   *
 * VERSION : 4-4-0 : FA : LAIG-FA-MAC-133882-CS : 3 juin 2015 : Correction code pour la qualite             *
 * VERSION : 4-0-0 : FA : LAIG-FA-MAC-117040-CS : 13 mars 2014 : Modifications mineures                     *
 * VERSION : 3-2-0 : FA : LAIG-FA-MAC-116169-CS : 2 decembre 2013 : Configuration pour la compression JPEG  *
 * VERSION : 1-0-0 : <TypeFT> : <NumFT> : 9 avr. 2010 : Creation
 *                                                                                                          *
 * FIN-HISTORIQUE                                                                                           *
 *                                                                                                          *
 * $Id: vnsMacro.h 11977 2016-04-08 14:38:50Z tfeuvrie $
 *                                                                                                          *
 ************************************************************************************************************/
#ifndef __vnsTestingMacro_h
#define __vnsTestingMacro_h

#include <vector>

namespace vns
{
    namespace tu
    {
        namespace Functor
        {
            template<class TInput, class TOutput = TInput>
                class myFloorFunctor
                {
                public:
                    typedef typename TOutput::ValueType OutputValueType;
                    myFloorFunctor()
                    {
                    }
                    ;
                    virtual
                    ~myFloorFunctor()
                    {
                    }
                    ;
                    inline TOutput
                    operator()(const TInput & A) const
                    {
                        TOutput outPix;
                        outPix.SetSize(A.Size());
                        for (unsigned int i = 0; i < A.Size(); i++)
                        {
                            double lol = static_cast<double>(A[i]) * 1000 + 0.5;
                            lol = vcl_floor(lol) / 1000.;

                            outPix[i] = static_cast<OutputValueType>(lol);
                        }
                        return outPix;
                    }
                };
        } // namespace Functor
    } // namespace tu

} // namespace vns

#if 0
// Sauvergade exemple de code avec itksysProcess_Se
int
System::CallCommandBasicWithItkSysProcess(const Utilities::ListOfStrings & commandlines)
{
    // ---------------------------------------------------------------------------------------------
    // Disable Log for basic command
    const int ac = commandlines.size();
    vnsLogSuperDebugMacro("Argument nb : <"<<ac<<">")

    // ---------------------------------------------------------------------------------------------
    char** args = new char*[ac + 1];
    // ---------------------------------------------------------------------------------------------
    for (int i = 0; i < ac; i++)
    {
        std::string l_String = commandlines[i];
        args[i] = new char[l_String.size() + 1];
//            strcpy(args[i], l_String.c_str());
        std::copy(l_String.begin(), l_String.end(), args[i]);
        args[i][l_String.size()] = '\0';
//            args[i] = l_String.c_str();
        // ---------------------------------------------------------------------------------------------
        vnsLogSuperDebugMacro("Argument : '"<<args[i]<<"'")
    }

    // ---------------------------------------------------------------------------------------------
    args[ac] = ITK_NULLPTR;

    // ---------------------------------------------------------------------------------------------
    itk::TimeProbe clock;
    clock.Start();
    // ---------------------------------------------------------------------------------------------
    // ---------------------------------------------------------------------------------------------
    // Call process
    itksysProcess * process = itksysProcess_New();
    itksysProcess_SetCommand(process, args);
    itksysProcess_SetOption(process, itksysProcess_Option_Detach, 0);
    itksysProcess_SetPipeShared(process, itksysProcess_Pipe_STDOUT, true);
    itksysProcess_SetPipeShared(process, itksysProcess_Pipe_STDERR, true);
    itksysProcess_Execute(process);
    itksysProcess_WaitForExit(process, ITK_NULLPTR);

    // ---------------------------------------------------------------------------------------------
    delete[] args;
    // ---------------------------------------------------------------------------------------------
    int state = itksysProcess_GetState(process);
    // ---------------------------------------------------------------------------------------------
    vnsLogSuperDebugMacro("Command itk state return '"<<state<<"'.")
    // ---------------------------------------------------------------------------------------------
    switch (state)
        {
    // ---------------------------------------------------------------------------------------------
    case itksysProcess_State_Error:
        {
            vnsLogErrorMacro("Process error: " << itksysProcess_GetErrorString(process))
            itksysProcess_Delete(process);
            return 1;
            break;
        }
        // ---------------------------------------------------------------------------------------------
    case itksysProcess_State_Exception:
        {
            vnsLogErrorMacro("Process exception: " << itksysProcess_GetExceptionString(process))
            itksysProcess_Delete(process);
            return 1;
            break;
        }
        // ---------------------------------------------------------------------------------------------
    case itksysProcess_State_Executing:
        {
            // ---------------------------------------------------------------------------------------------
            // this is not a possible state after itksysProcess_WaitForExit
            vnsLogErrorMacro("Internal error: process can't be in Executing State.")
            itksysProcess_Delete(process);
            return 1;
            break;
        }
        // ---------------------------------------------------------------------------------------------
    case itksysProcess_State_Exited:
        {
            // ---------------------------------------------------------------------------------------------
            // this is the normal case - it is treated later
            break;
        }
        // ---------------------------------------------------------------------------------------------
    case itksysProcess_State_Expired:
        {
            // ---------------------------------------------------------------------------------------------
            // this is not a possible state after itksysProcess_WaitForExit
            vnsLogErrorMacro("Internal error: process can't be in Expired State.")
            itksysProcess_Delete(process);
            return 1;
            break;
        }
        // ---------------------------------------------------------------------------------------------
    case itksysProcess_State_Killed:
        {
            // ---------------------------------------------------------------------------------------------
            vnsLogErrorMacro("The process has been killed.")
            itksysProcess_Delete(process);
            return 1;
            break;
        }
        // ---------------------------------------------------------------------------------------------
    case itksysProcess_State_Disowned:
        {
            // ---------------------------------------------------------------------------------------------
            vnsLogErrorMacro("Process disowned.")
            itksysProcess_Delete(process);
            return 1;
            break;
        }
        // ---------------------------------------------------------------------------------------------
    default:
        {
            // ---------------------------------------------------------------------------------------------
            // this is not a possible state after itksysProcess_WaitForExit
            vnsLogErrorMacro("Internal error: unknown State.")
            itksysProcess_Delete(process);
            return 1;
            break;
        }
        }
    // ---------------------------------------------------------------------------------------------
    clock.Stop();
    // ---------------------------------------------------------------------------------------------
    int result = itksysProcess_GetExitValue(process);
    // ---------------------------------------------------------------------------------------------
    itksysProcess_Kill(process);

    // ---------------------------------------------------------------------------------------------
    vnsLogSuperDebugMacro("Command run in "<< Utilities::GetStringFormatedTime(clock.GetTotal())<<" (code return '"<<result<<"').");

    // ---------------------------------------------------------------------------------------------
    return result;

}
#endif

#endif /* __vnsTestingMacro_h */
