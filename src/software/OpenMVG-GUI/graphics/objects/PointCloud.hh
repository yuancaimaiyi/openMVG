#ifndef _OPENMVG_SOFTWARE_OPENMVG_GUI_GRAPHICS_OBJECTS_GRID_HH_
#define _OPENMVG_SOFTWARE_OPENMVG_GUI_GRAPHICS_OBJECTS_GRID_HH_

#include "RenderableObject.hh"

namespace openMVG_gui
{

/**
* @brief Class holding a point cloud
*/
class PointCloud : public RenderableObject
{
  public:

    /**
    * @brief Point cloud
    * @param pgm The shader
    * @param pts The point list
    * @param col The colors associated with the points (if no color provide an empty array)
    */
    PointCloud( std::shared_ptr<ShaderProgram> pgm ,
                const std::vector< openMVG::Vec3 > & pts ,
                const std::vector< openMVG::Vec3 > & col ,
                const openMVG::Vec3 defaultColor = openMVG::Vec3( 0.9 , 0.9 , 0.9 ) ) ;

    /**
    * @brief Dtr
    */
    ~PointCloud();

    /**
    * @brief Prepare object before rendering (ie: create buffers, prepare data)
    * @note openGL context must be active when calling this function
    */
    void prepare( void ) override ;

    /**
    * @brief Draw code for the object
    */
    void draw( void ) const override;

  private:
    std::vector< openMVG::Vec3 > m_pts ;
    std::vector< openMVG::Vec3 > m_col ;
    openMVG::Vec3 m_default_color ;

    // OpenGL
    GLsizei m_nb_vert ;
    GLuint m_vao ;
    GLuint m_vbo ;

  public:
    EIGEN_MAKE_ALIGNED_OPERATOR_NEW
} ;

} // namespace openMVG_gui

#endif