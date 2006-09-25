// __BEGIN_LICENSE__
//
// Copyright (C) 2006 United States Government as represented by the
// Administrator of the National Aeronautics and Space Administration
// (NASA).  All Rights Reserved.
// 
// This software is distributed under the NASA Open Source Agreement
// (NOSA), version 1.3.  The NOSA has been approved by the Open Source
// Initiative.  See the file COPYING at the top of the distribution
// directory tree for the complete NOSA document.
// 
// THE SUBJECT SOFTWARE IS PROVIDED "AS IS" WITHOUT ANY WARRANTY OF ANY
// KIND, EITHER EXPRESSED, IMPLIED, OR STATUTORY, INCLUDING, BUT NOT
// LIMITED TO, ANY WARRANTY THAT THE SUBJECT SOFTWARE WILL CONFORM TO
// SPECIFICATIONS, ANY IMPLIED WARRANTIES OF MERCHANTABILITY, FITNESS FOR
// A PARTICULAR PURPOSE, OR FREEDOM FROM INFRINGEMENT, ANY WARRANTY THAT
// THE SUBJECT SOFTWARE WILL BE ERROR FREE, OR ANY WARRANTY THAT
// DOCUMENTATION, IF PROVIDED, WILL CONFORM TO THE SUBJECT SOFTWARE.
//
// __END_LICENSE__

/// \file ImageStatistics.h
/// 
/// These functions compute the min, max, minmax, and
/// mean value of an image on a per-channel basis.  
///

// TODO: How do we deal with alpha channels here?
#ifndef __VW_IMAGE_STATISTICS_H__
#define __VW_IMAGE_STATISTICS_H__

#include <boost/type_traits.hpp>

#include <vw/Image/ImageView.h>
#include <vw/Image/Manipulation.h>

namespace vw {

  /// Compute the minimum value stored in all of the channels of all of the planes of the images.
  template <class ViewT>
  typename CompoundChannelType<typename ViewT::pixel_type>::type
  min_channel_value( const ImageViewBase<ViewT>& view_ ) {
    const ViewT& view = view_.impl();
 
    typedef typename ViewT::pixel_accessor pixel_accessor;
    typedef typename ViewT::pixel_type pixel_type;
    typedef typename CompoundChannelType<typename ViewT::pixel_type>::type channel_type;

    pixel_type pix = view(0,0);
    channel_type min = compound_select_channel<channel_type>(pix,0);

    pixel_accessor plane_acc = view.origin();
    for (unsigned p = 0; p < view.planes(); p++, plane_acc.next_plane()) { 
      pixel_accessor col_acc = plane_acc;
      for (unsigned i = 0; i < view.cols(); i++, col_acc.next_col()) {
        pixel_accessor row_acc = col_acc;
        for (unsigned j = 0; j < view.rows(); j++, row_acc.next_row()) {
          typename ViewT::result_type pix = *row_acc;
          for (unsigned channel = 0; channel < view.channels(); channel++) {
            channel_type channel_value = compound_select_channel<channel_type>(pix,channel);
            if( channel_value < min ) min = channel_value;
          }
        }
      }  
    }
    return min;
  }


  /// Compute the maximum value stored in all of the channels of all of the planes of the images.
  template <class ViewT>
  typename CompoundChannelType<typename ViewT::pixel_type>::type
  max_channel_value( const ImageViewBase<ViewT>& view_ ) {
    const ViewT& view = view_.impl();

    typedef typename ViewT::pixel_accessor pixel_accessor;
    typedef typename ViewT::pixel_type pixel_type;
    typedef typename CompoundChannelType<typename ViewT::pixel_type>::type channel_type;

    pixel_type pix = view(0,0);
    channel_type max = compound_select_channel<channel_type>(pix,0);

    pixel_accessor plane_acc = view.origin();
    for (unsigned p = 0; p < view.planes(); p++, plane_acc.next_plane()) { 
      pixel_accessor col_acc = plane_acc;
      for (unsigned i = 0; i < view.cols(); i++, col_acc.next_col()) {
        pixel_accessor row_acc = col_acc;
        for (unsigned j = 0; j < view.rows(); j++, row_acc.next_row()) {
          typename ViewT::result_type pix = *row_acc;
          for (unsigned channel = 0; channel < view.channels(); channel++) {
            channel_type channel_value = compound_select_channel<channel_type>(pix,channel);
            if( channel_value > max ) max = channel_value;
          }
        }
      } 
    }
    return max;
  }


  /// Simultaneously compute the min and max value in all of the
  /// channels of all of the planes of the image.
  template <class ViewT>
  void min_max_channel_values( const ImageViewBase<ViewT> &view_, 
                               typename CompoundChannelType<typename ViewT::pixel_type>::type &min, 
                               typename CompoundChannelType<typename ViewT::pixel_type>::type &max ) {
    const ViewT& view = view_.impl();

    typedef typename ViewT::pixel_accessor pixel_accessor;
    typedef typename ViewT::pixel_type pixel_type;
    typedef typename CompoundChannelType<typename ViewT::pixel_type>::type channel_type;

    pixel_type pix = view(0,0);
    min = compound_select_channel<channel_type>(pix,0);
    max = compound_select_channel<channel_type>(pix,0);

    pixel_accessor plane_acc = view.origin();
    for (unsigned p = 0; p < view.planes(); p++, plane_acc.next_plane()) { 
      pixel_accessor col_acc = plane_acc;
      for (unsigned i = 0; i < view.cols(); i++, col_acc.next_col()) {
        pixel_accessor row_acc = col_acc;
        for (unsigned j = 0; j < view.rows(); j++, row_acc.next_row()) {
          typename ViewT::result_type pix = *row_acc;
          for (unsigned channel = 0; channel < view.channels(); channel++) {
            channel_type channel_value = compound_select_channel<channel_type>(pix,channel);
            if( channel_value < min ) min = channel_value;
            if( channel_value > max ) max = channel_value;
          }
        }
      }  
    }
  }
	
  /// Compute the mean value stored in all of the channels of all of the planes of the image.
  template <class ViewT>
  typename CompoundChannelType<typename ViewT::pixel_type>::type
  mean_channel_value( const ImageViewBase<ViewT> &view_ ) {
    const ViewT& view = view_.impl();

    typedef typename ViewT::pixel_accessor pixel_accessor;
    typedef typename CompoundChannelType<typename ViewT::pixel_type>::type channel_type;

    double accum = 0;

    pixel_accessor plane_acc = view.origin();
    for (unsigned p = 0; p < view.planes(); p++, plane_acc.next_plane()) { 
      pixel_accessor col_acc = plane_acc;
      for (unsigned i = 0; i < view.cols(); i++, col_acc.next_col()) {
        pixel_accessor row_acc = col_acc;
        for (unsigned j = 0; j < view.rows(); j++, row_acc.next_row()) {
          typename ViewT::result_type pix = *row_acc;
          for (unsigned channel = 0; channel < view.channels(); channel++) {
            channel_type channel_value = compound_select_channel<channel_type>(pix,channel);
            accum += channel_value;
          }
        }
      }  
    }
    return channel_type(accum / (view.planes() * view.rows() * view.cols() * view.channels()));
  }

}  // namespace vw


#endif // __VW_IMAGE_STATISTICS_H__
