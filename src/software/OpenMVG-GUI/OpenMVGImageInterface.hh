#ifndef _OPENMVG_SOFTWARE_OPENMVG_GUI_OPENMVG_IMAGE_INTERFACE_HH_
#define _OPENMVG_SOFTWARE_OPENMVG_GUI_OPENMVG_IMAGE_INTERFACE_HH_

#include "openMVG/image/image_container.hpp"
#include "openMVG/image/pixel_types.hpp"

#include <QImage>

namespace openMVG_gui
{

/**
* @brief Build a QImage from an openMVG (RGB) image
* @param img Input image
* @return a QImage corresponding to the input image
* @note this makes a deep copy of the image
*/
QImage openMVGImageToQImage( const openMVG::image::Image<openMVG::image::RGBColor> &img );

/**
  * @brief Build a QImage from an openMVG (grayscale) image
  * @param img Input image
  * @return a QImage corresponding to the input image
  * @note this makes a deep copy of the image
  */
QImage openMVGImageToQImage( const openMVG::image::Image<unsigned char> &img );

/**
  * @brief Convert a QImage to an openMVG image (RGB)
  * @param img Input image
  * @return openMVG image corresponding to this image
  * @note this makes a deep copy
  */
openMVG::image::Image<openMVG::image::RGBColor> QImageToOpenMVGImage( const QImage &img );

/**
  * @brief Convert a QImage to an openMVG image (grayscale)
  * @param img Input image
  * @return openMVG image corresponding to this image
  * @note this makes a deep copy
  */
openMVG::image::Image<unsigned char> QImageToOpenMVGImageGrayscale( const QImage & img ) ;

} // namespace openMVG_gui

#endif