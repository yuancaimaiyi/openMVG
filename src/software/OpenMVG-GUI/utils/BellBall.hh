#ifndef _OPENMVG_SOFTWARE_OPENMVG_GUI_UTILS_BELL_BALL_HH_
#define _OPENMVG_SOFTWARE_OPENMVG_GUI_UTILS_BELL_BALL_HH_

#include "openMVG/numeric/numeric.h"

namespace openMVG_gui
{
/**
* @brief class holding a bell ball as defined in
* "Virtual Trackballs Revisited"
* @note everything is defined in image plane 
*/
class BellBall
{
  public:

    /**
    * @brief radius of the bell ball
    */
    BellBall( const double radius ) ;

    /**
    * @brief get 3d point on hyperbola given a 2d point 
    */ 
    openMVG::Vec3 get( const double x , const double y ) const ;

  private:

    double m_radius ;
    double m_radius2 ; 
} ;

} // namespace openMVG_gui

#endif