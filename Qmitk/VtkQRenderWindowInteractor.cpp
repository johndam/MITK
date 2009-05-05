/*=========================================================================

Program:   Medical Imaging & Interaction Toolkit
Language:  C++
Date:      $Date$
Version:   $Revision$

Copyright (c) German Cancer Research Center, Division of Medical and
Biological Informatics. All rights reserved.
See MITKCopyright.txt or http://www.mitk.org/copyright.html for details.

This software is distributed WITHOUT ANY WARRANTY; without even
the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR
PURPOSE.  See the above copyright notices for more information.

=========================================================================*/

//#include <stdio.h>
//#include <stdlib.h>
//#include <string.h>
//#include <math.h>
//#include "VtkQRenderWindowInteractor.h"
//
//#include "mitkRenderingManager.h"
//
//#include "vtkInteractorStyle.h"
//#include "vtkActor.h"
//#include "vtkObjectFactory.h"
//
//#include "vtkObjectFactory.h"
//#include "vtkCommand.h"
//#include "vtkRenderWindow.h"
//#include "vtkVersion.h"
//
////assume vtk-version 4.0, if VTK_VERSION is undefined
//#ifndef VTK_VERSION
//#define VTK_MAJOR_VERSION 4
//#define VTK_MINOR_VERSION 0
//#endif
//
////----------------------------------------------------------------------------
//VtkQRenderWindowInteractor* VtkQRenderWindowInteractor::New()
//{
//    // First try to create the object from the vtkObjectFactory
//    vtkObject* ret = vtkObjectFactory::CreateInstance("VtkQRenderWindowInteractor");
//    if(ret)
//    {
//        return (VtkQRenderWindowInteractor*)ret;
//    }
//    // If the factory was unable to create the object, then create it here.
//    return new VtkQRenderWindowInteractor;
//}
//
//void (*VtkQRenderWindowInteractor::ClassExitMethod)(void *) = (void (*)(void *))NULL;
//void *VtkQRenderWindowInteractor::ClassExitMethodArg = (void *)NULL;
//void (*VtkQRenderWindowInteractor::ClassExitMethodArgDelete)(void *) = (void (*)(void *))NULL;
//
////// Construct object so that light follows camera motion.
//VtkQRenderWindowInteractor::VtkQRenderWindowInteractor() 
//{
//    //static int timerId           = 1;
//    //this->WindowId           = 0;
//    //this->TimerId            = timerId++;
//    this->MouseInWindow = 0;
//}
//
//VtkQRenderWindowInteractor::~VtkQRenderWindowInteractor()
//{
//    
//}
//
//// Begin processing keyboard strokes.
//void VtkQRenderWindowInteractor::Initialize()
//{
// int *size;
//  
//  // make sure we have a RenderWindow and camera
// if (this->Initialized)
//  {
//      return;
//  }
//  this->Initialized = 1;
//  
//  if (GetRenderWindow() != NULL) 
//  {    
//    size = GetRenderWindow()->GetSize();
//    
//    this->Enable();
//    this->Size[0] = size[0];
//    this->Size[1] = size[1];
//  }
//
//}
//
//void VtkQRenderWindowInteractor::Enable()
//{
//    if (this->GetEnabled())
//    {
//        return;
//    }
//    int *size;
//    size = GetRenderWindow()->GetSize();
//    this->Size[0] = size[0];
//    this->Size[1] = size[1];
//    this->Enabled = 1;
//    this->Modified();
//}
//
//
//void VtkQRenderWindowInteractor::Disable()
//{
//    if (!this->GetEnabled())
//    {
//        return;
//    }
//    
//    this->Enabled = 0;
//    this->Modified();
//}
//
//void VtkQRenderWindowInteractor::TerminateApp(void)
//{
//}
//
//void VtkQRenderWindowInteractor::Render()
//{
//  if (this->RenderWindow && this->Enabled)
//  {
//    /*
//    currently this is faster (for navigation applications for instance but without volume rendering.
//    Then the rendering inside one window initially takes some milliseconds and then is fast.
//    */
//    //this->RenderWindow->Render(); 
//    
//    //only way for VolumeRendering
//    mitk::RenderingManager::GetInstance()->RequestUpdate(this->RenderWindow);
//
//  }
//  // outside the above test so that third-party code can redirect
//  // the render to the appropriate class
//#if (VTK_MAJOR_VERSION >= 5)
//  this->InvokeEvent(vtkCommand::RenderEvent, NULL);
//#endif
//}
//
//
//int VtkQRenderWindowInteractor::CreateTimer(int timertype)
//{
//    if (timertype==VTKI_TIMER_FIRST)
//    {
//        return TimerId=startTimer(10);
//    }
//    return 1;
//}
//
//int VtkQRenderWindowInteractor::DestroyTimer(void)
//{
//    killTimer(TimerId);
//    return 1;
//}
//
//
//void VtkQRenderWindowInteractor::SetSize(int w, int h)
//{
//    Resize(w, h);
//}
//
//void VtkQRenderWindowInteractor::SetSize(int size[2])
//{
//    Resize(size[0], size[1]);
//}
//
//void VtkQRenderWindowInteractor::Resize(int w, int h)
//{
//    UpdateSize(w, h);
//    if(InteractorStyle==NULL) return;
//    if(InteractorStyle->IsA("vtkInteractorStyle"))
//    {
//        SetEventSize(w, h);
//        SetEventPosition(0, Size[1] - 0 - 1);
//        if (this->GetEnabled())
//        {
//#if ((VTK_MAJOR_VERSION>4) || ((VTK_MAJOR_VERSION==4) && (VTK_MINOR_VERSION>=2)))
//            InvokeEvent(vtkCommand::ConfigureEvent,NULL);
//#else
//            vtkInteractorStyle* vtkinteractorstyle=(vtkInteractorStyle*)InteractorStyle;
//            vtkinteractorstyle->OnConfigure(w,h);
//#endif
//        }
//    }
//}
//
//// Specify the default function to be called when an interactor needs to exit.
//// This callback is overridden by an instance ExitMethod that is defined.
//void
//VtkQRenderWindowInteractor::SetClassExitMethod(void (*f)(void *),void *arg)
//{
//    if ( f != VtkQRenderWindowInteractor::ClassExitMethod
//        || arg != VtkQRenderWindowInteractor::ClassExitMethodArg)
//    {
//        // delete the current arg if there is a delete method
//        if ((VtkQRenderWindowInteractor::ClassExitMethodArg)
//            && (VtkQRenderWindowInteractor::ClassExitMethodArgDelete))
//        {
//            (*VtkQRenderWindowInteractor::ClassExitMethodArgDelete)
//                (VtkQRenderWindowInteractor::ClassExitMethodArg);
//        }
//        VtkQRenderWindowInteractor::ClassExitMethod = f;
//        VtkQRenderWindowInteractor::ClassExitMethodArg = arg;
//        
//        // no call to this->Modified() since this is a class member function
//    }
//}
//
//
//// Set the arg delete method.  This is used to free user memory.
//void
//VtkQRenderWindowInteractor::SetClassExitMethodArgDelete(void (*f)(void *))
//{
//    if (f != VtkQRenderWindowInteractor::ClassExitMethodArgDelete)
//    {
//        VtkQRenderWindowInteractor::ClassExitMethodArgDelete = f;
//        
//        // no call to this->Modified() since this is a class member function
//    }
//}
//
//void VtkQRenderWindowInteractor::PrintSelf(ostream& os, vtkIndent indent)
//{
//    vtkRenderWindowInteractor::PrintSelf(os,indent);
//}
//
//void VtkQRenderWindowInteractor::ExitCallback()
//{
//    if (this->HasObserver(vtkCommand::ExitEvent))
//    {
//        this->InvokeEvent(vtkCommand::ExitEvent,NULL);
//    }
//    else if (this->ClassExitMethod)
//    {
//        (*this->ClassExitMethod)(this->ClassExitMethodArg);
//    }
//    else
//    {
//        this->TerminateApp();
//    }
//}
//
//
//void VtkQRenderWindowInteractor::MousePressEvent(mitk::MouseEvent *)
//{
//}
//
//void VtkQRenderWindowInteractor::MouseReleaseEvent(mitk::MouseEvent *)
//{
//}
//
//void VtkQRenderWindowInteractor::MouseMoveEvent(mitk::MouseEvent *)
//{
//}
//
//void VtkQRenderWindowInteractor::KeyPressEvent(mitk::KeyEvent *)
//{
//}
//
//void VtkQRenderWindowInteractor::timerEvent ( QTimerEvent * )
//{
//    if (!this->GetEnabled())
//    {
//        return;
//    }
//#if ((VTK_MAJOR_VERSION>4) || ((VTK_MAJOR_VERSION==4) && (VTK_MINOR_VERSION>=2)))
//    InvokeEvent(vtkCommand::TimerEvent,NULL);
//#else
//    this->InteractorStyle->OnTimer();
//#endif
//}
//
////##
//VTK_CREATE_CREATE_FUNCTION(VtkQRenderWindowInteractor);
//
//vtkStandardNewMacro(VtkQRenderWindowInteractorFactory);
//
//VtkQRenderWindowInteractorFactory*  VtkQRenderWindowInteractorFactory::m_Factory = NULL;
//
////##
//VtkQRenderWindowInteractorFactory::VtkQRenderWindowInteractorFactory( )
//: vtkObjectFactory() {
//    
//  if ( !m_Factory ) {
//
//    vtkObjectFactory::RegisterFactory( this );
//    RegisterOverride( "vtkRenderWindowInteractor", "VtkQRenderWindowInteractor", "RenderWindowInteractor for mitk and QT", 1, vtkObjectFactoryCreateVtkQRenderWindowInteractor );
//  }  
//  
//  m_Factory = this;
//}
//
//VtkQRenderWindowInteractorFactory::~VtkQRenderWindowInteractorFactory( )
//
//{
//  if (vtkObjectFactory::GetRegisteredFactories() != NULL) {
//    vtkObjectFactory::UnRegisterFactory( this );
//  }
//  SetReferenceCount( GetReferenceCount() - 1 ); // referenz m_Factory = NULL
//  m_Factory = NULL;
//}
//
////##
//const char* VtkQRenderWindowInteractorFactory::GetVTKSourceVersion()
//{
//  return VTK_SOURCE_VERSION;
//}
//
////##
//const char* VtkQRenderWindowInteractorFactory::GetDescription()
//{
//  return "Factory for VtkQRenderWindowInteractor";
//}
//
//VTK_FACTORY_INTERFACE_IMPLEMENT(VtkQRenderWindowInteractorFactory)
//
