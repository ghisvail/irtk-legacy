/*=========================================================================

  Library   : Image Registration Toolkit (IRTK)
  Module    : $Id$
  Copyright : Imperial College, Department of Computing
              Visual Information Processing (VIP), 2008 onwards
  Date      : $Date$
  Version   : $Revision$
  Changes   : $Author$

=========================================================================*/

#ifndef _IRTKIMAGETRANSFORMATION_H

#define _IRTKIMAGETRANSFORMATION_H

#include <irtkImage.h>

#include <irtkImageFunction.h>

#include <irtkTransformation.h>

/**
 * Filter for image transformations.
 *
 * This class implements an image filter which takes an input image and a
 * transformation and computes the corresponding output image. The output
 * image is calculated by looping over the voxel locations and calculating
 * the corresponding voxel locations in the input image. The intensity of
 * the voxels of the output image is by interpolation from the input image.
 * Note, that the transformation is assumed to map the input image into the
 * ouput image and is therefore inverted during the execution of the filter.
 * All calculations are carried out using world coordinates rather than image
 * coordinates.
 *
 */

template <class VoxelType> class irtkImageTransformation
{

public:

  /// Input for the image to image filter
  irtkGenericImage<VoxelType> *_input;

  /// Output for the image to image filter
  irtkGenericImage<VoxelType> *_output;

  /// Transformation
  irtkTransformation *_transformation;

  /// Interpolation
  irtkImageFunction<VoxelType> *_interpolator;

  /// Padding value in target (voxels in the target image with this
  /// value will be ignored)
  VoxelType _TargetPaddingValue;

  /// Padding value in source (voxels outside the source image will
  /// be set to this value)
  VoxelType _SourcePaddingValue;

  /// Flag whether to invert transformation
  int _Invert;

public:

  /** Constructor. This constructs an transformation filter with a given
   *  interpolation mode and padding value. By default the interpolation
   *  mode is set to trilinear and invert is off
   */
  irtkImageTransformation();

  /** Static constructor. This constructs an transformation filter for a
   *  given transformation. If there is a fast transformation filter, it
   *  will use this fast transformation filter, otherwise the standard
   *  transformation filter is used.
   */
  static irtkImageTransformation<VoxelType> *New(irtkTransformation *);

  /// Destructor
  virtual ~irtkImageTransformation();

  /// Sets input image
  virtual void SetInput (irtkGenericImage<VoxelType> *);

  /// Sets input image and transformation
  virtual void SetInput (irtkGenericImage<VoxelType> *, irtkTransformation *);

  /// Sets output image
  virtual void SetOutput(irtkGenericImage<VoxelType> *);

  /// Sets transformation
  virtual void SetTransformation(irtkTransformation *);

  /// Gets the target padding value
  virtual VoxelType GetTargetPaddingValue(void);

  /// Puts the target padding value
  virtual void PutTargetPaddingValue(VoxelType);

  /// Gets the source padding value
  virtual VoxelType GetSourcePaddingValue(void);

  /// Puts the source padding value
  virtual void PutSourcePaddingValue(VoxelType);

  /// Gets the interpolator
  virtual irtkImageFunction<VoxelType> *GetInterpolator(void);

  /// Sets the interpolator
  virtual void PutInterpolator(irtkImageFunction<VoxelType> *);

  /// Invert on
  virtual void InvertOn(void);

  /// Sets the interpolator
  virtual void InvertOff(void);

  /// Runs the filter
  virtual void Run();

};

template <class VoxelType> inline VoxelType irtkImageTransformation<VoxelType>::GetTargetPaddingValue()
{
  return _TargetPaddingValue;
}

template <class VoxelType> inline void irtkImageTransformation<VoxelType>::PutTargetPaddingValue(VoxelType PaddingValue)
{
  _TargetPaddingValue = PaddingValue;
}

template <class VoxelType> inline VoxelType irtkImageTransformation<VoxelType>::GetSourcePaddingValue()
{
  return _SourcePaddingValue;
}

template <class VoxelType> inline void irtkImageTransformation<VoxelType>::PutSourcePaddingValue(VoxelType PaddingValue)
{
  _SourcePaddingValue = PaddingValue;
}

template <class VoxelType> inline irtkImageFunction<VoxelType> *irtkImageTransformation<VoxelType>::GetInterpolator()
{
  return _interpolator;
}

template <class VoxelType> inline void irtkImageTransformation<VoxelType>::PutInterpolator(irtkImageFunction<VoxelType> *interpolator)
{
  _interpolator = interpolator;
}

template <class VoxelType> inline void irtkImageTransformation<VoxelType>::InvertOn()
{
  _Invert = True;
}

template <class VoxelType> inline void irtkImageTransformation<VoxelType>::InvertOff()
{
  _Invert = False;
}

#endif
