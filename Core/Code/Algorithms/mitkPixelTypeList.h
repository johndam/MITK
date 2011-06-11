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

#include <stdexcept>

namespace mitk {

struct EmptyType {};

template<
  typename T0=EmptyType,
  typename T1=EmptyType,
  typename T2=EmptyType,
  typename T3=EmptyType,
  typename T4=EmptyType,
  typename T5=EmptyType,
  typename T6=EmptyType,
  typename T7=EmptyType,
  typename T8=EmptyType,
  typename T9=EmptyType
> struct PixelTypeList;

template<
  typename T0,
  typename T1,
  typename T2,
  typename T3,
  typename T4,
  typename T5,
  typename T6,
  typename T7,
  typename T8,
  typename T9
> struct PixelTypeList
{
  typedef T0 head;
  typedef PixelTypeList<T1,T2,T3,T4,T5,T6,T7,T8,T9> tail;
  enum
  {
    length = tail::length+1
  };
};

template<>
struct PixelTypeList<EmptyType, EmptyType, EmptyType, EmptyType, EmptyType,
                     EmptyType, EmptyType, EmptyType, EmptyType, EmptyType>
{
  enum
  {
    length = 0
  };
};

template<typename TypeList>
struct PixelTypeLength
{
  enum
  {
    value = TypeList::length
  };
};

template<
  typename TypeList,
  int Index, //requested element index
  int Step = 0, //current recusion step
  bool Stop=(Index==Step), //stop recusion flag
  bool OutOfRange = PixelTypeLength<TypeList>::value==0 //out of range flag
> struct GetPixelType
{
  typedef typename GetPixelType<typename TypeList::tail, Index, Step+1>::type type;
};

//"out of range" specialization
template<
  typename TypeList,
  int Index,
  int Step,
  bool Stop
>
struct GetPixelType<TypeList, Index, Step, Stop, true>
{
  //if OutOfRange is 'true' the 'type' is undefined
  //so we'll get a compile-time error
};

//"element found" specialization
template<
  typename TypeList,
  int Index,
  int Step,
  bool OutOfRange
>
struct GetPixelType<TypeList, Index, Step, true, OutOfRange>
{
  //the index is equal to the recursion step
  //so the result type is the head of the Typlist and stop!
  typedef typename TypeList::head type;
};

////////////////////////////////////////////////////////////
// run-time type switch
template<
  typename TypeList,
  int Index = 0,
  bool Stop=(Index==PixelTypeLength<TypeList>::value)
> struct PixelTypeSwitch;

template<typename TypeList, int Index, bool Stop>
struct PixelTypeSwitch
{
  template<typename F>
  bool operator() (int i, F& f)
  {
    if( i == Index)
    {
      return f.operator()<typename GetPixelType<TypeList, Index>::type>();
    }
    else
    {
      PixelTypeSwitch<TypeList, Index+1> next;
      return next(i, f);
    }
  }
};

template<typename TypeList, int Index>
struct PixelTypeSwitch<TypeList, Index, true>
{
  template<typename F>
  bool operator() (int, F&)
  {
    throw std::out_of_range("Index out of range");
  }
};

template<typename X, int VDimension, typename T1 = EmptyType, typename T2 = EmptyType, typename T3 = EmptyType>
struct AccessItkImageFunctor
{

  AccessItkImageFunctor(X* cl, mitk::Image::Pointer mitkImage, const T1& t1 = T1(), const T2& t2 = T2(), const T3& t3 = T3())
    : cl(cl), mitkImage(mitkImage), pixelType(mitkImage->GetPixelType()),
      t1(t1), t2(t2), t3(t3)
  {
    mitkImage->Update();
    assert(mitkImage->GetDimension() == VDimension);
  }

  template<typename PixelType>
  bool operator() ()
  {
    if (pixelType != typeid(PixelType)) return false;

    typedef itk::Image<PixelType, VDimension> ImageType;
    typedef mitk::ImageToItk<ImageType> ImageToItkType;
    itk::SmartPointer<ImageToItkType> imagetoitk = ImageToItkType::New();
    imagetoitk->SetInput(mitkImage);
    imagetoitk->Update();
    callMember<ImageType, T1, T2, T3>()(imagetoitk->GetOutput(), cl, t1, t2, t3);
    return true;
  }

private:

  template<typename ImageType, typename A, typename B, typename C>
  struct callMember
  {
    void operator()(ImageType* img, X* cl, A& a, B& b, C& c) { cl->AccessItkImage(img, a, b, c); }
  };

  template<typename ImageType, typename A, typename B>
  struct callMember<ImageType, A, B, EmptyType>
  {
    void operator()(ImageType* img, X* cl, A& a, B& b, EmptyType&) { cl->AccessItkImage(img, a, b); }
  };

  template<typename ImageType, typename A>
  struct callMember<ImageType, A, EmptyType, EmptyType>
  {
    void operator()(ImageType* img, X* cl, A& a, EmptyType&, EmptyType&) { cl->AccessItkImage(img, a); }
  };

  template<typename ImageType>
  struct callMember<ImageType, EmptyType, EmptyType, EmptyType>
  {
    void operator()(ImageType* img, X* cl, EmptyType&, EmptyType&, EmptyType&) { cl->AccessItkImage(img); }
  };

  X* cl;
  const mitk::Image::Pointer mitkImage;
  const mitk::PixelType& pixelType;
  T1 t1;
  T2 t2;
  T3 t3;
};


} // namespace mitk

