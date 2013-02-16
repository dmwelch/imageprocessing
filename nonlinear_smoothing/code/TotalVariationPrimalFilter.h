/*========================================================================= 
 * 
 *  Copyright (c) 2013 Daniel Perry 
 * 
 *  MIT license 
 * 
 *=========================================================================*/ 
#ifndef TotalVariationPrimalFilter_h 
#define TotalVariationPrimalFilter_h 

// itk includes
#include "itkImage.h"
#include "itkImageToImageFilter.h"
#include "itkSmartPointer.h"

namespace imageprocessing
{
/**
 * TotalVariationPrimalFilter
 *  Implementation of total variation.  Initially done for 
 *  CS 7640 - Advanced Image Processing, Spring 2013, University of Utah
 */
template< class TInputImage,
          class TOutputImage=TInputImage
        >
class TotalVariationPrimalFilter:
  public itk::ImageToImageFilter<TInputImage,TOutputImage>
{
public:
  /**
   * Standard class typedefs
   */
  typedef TotalVariationPrimalFilter Self;
  typedef itk::ImageToImageFilter< TInputImage, TOutputImage > Superclass;
  typedef itk::SmartPointer< Self > Pointer;
  typedef itk::SmartPointer< const Self > ConstPointer;

  itkStaticConstMacro(ImageDimension, unsigned int,
                      TInputImage::ImageDimension);

  /** Method for creation through the object factory. */
  itkNewMacro(Self);

  /** Run-time type information (and related methods). */
  itkTypeMacro(TotalVariationPrimalFilter, ImageToImageFilter);

  /** Image type typedef support. */
  typedef TInputImage InputImageType;
  typedef typename InputImageType::ConstPointer InputImageConstPointer;
  typedef TOutputImage OutputImageType;
  typedef typename OutputImageType::RegionType OutputImageRegionType;
  typedef typename InputImageType::SizeType ImageSizeType;
  typedef itk::Image<itk::Vector<float,InputImageType::ImageDimension>,InputImageType::ImageDimension> VectorImageType;
  typedef typename VectorImageType::Pointer VectorImagePointer;
  typedef typename VectorImageType::ConstPointer VectorImageConstPointer;

  /** 
   * Chambolle - whether to compute the TV using the
   *   Chambolle method, instead of primal/dual (default).
   */
  itkSetMacro(Chambolle, bool);
  itkGetConstMacro(Chambolle, bool);
  /**
   * Step size for primal solution.
   */
  itkSetMacro(PrimalStepSize, float);
  itkGetConstMacro(PrimalStepSize, float);
  /**
   * Normalizing parameter - makes it less sensitive to step sizes.
   */
  itkSetMacro(Lambda, float);
  itkGetConstMacro(Lambda, float);
  /**
   * Delta - the change in solution after running.
   */
  itkSetMacro(Delta, float);
  itkGetConstMacro(Delta, float);

  /**
   * X - the unit vector dual image
   */
  itkSetMacro(X, VectorImagePointer)
  itkGetMacro(X, VectorImagePointer)

  /**
   * OriginalImage - the original image.
   */
  itkSetMacro(OriginalImage, InputImageConstPointer)
  itkGetMacro(OriginalImage, InputImageConstPointer)


protected:
  TotalVariationPrimalFilter()
  :m_Chambolle(false),
  m_PrimalStepSize(1),
  m_Lambda(1),
  m_Delta(10e10),
  m_X(),
  m_OriginalImage()
  {}

  virtual ~TotalVariationPrimalFilter(){}
  void PrintSelf(std::ostream & os, itk::Indent indent) const;

  /**
   * Standard parallel pipeline method
   */
  void BeforeThreadedGenerateData();
  void ThreadedGenerateData( const OutputImageRegionType & outputRegionForThread,
                              itk::ThreadIdType threadId);

private:
  TotalVariationPrimalFilter(const Self &); // not allowed
  void operator=(const Self &); // not allowed 

  bool m_Chambolle;
  float m_PrimalStepSize;
  float m_Lambda;
  float m_Delta;

  std::vector<float> m_Deltas; // per thread

  VectorImagePointer m_X; // dual unit gradient image.
  InputImageConstPointer m_OriginalImage;
};
} // end namespace itk

#include "TotalVariationPrimalFilter.hxx"

#endif