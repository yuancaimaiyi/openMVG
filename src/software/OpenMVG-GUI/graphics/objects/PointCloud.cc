#include "PointCloud.hh"

namespace openMVG_gui
{

/**
* @brief Point cloud
* @param pgm The shader
* @param pts The point list
* @param col The colors associated with the points (if no color provide an empty array)
*/
PointCloud::PointCloud( std::shared_ptr<ShaderProgram> pgm ,
                        const std::vector< openMVG::Vec3 > & pts ,
                        const std::vector< openMVG::Vec3 > & col ,
                        const openMVG::Vec3 defaultColor )
  : RenderableObject( pgm ) ,
    m_pts( pts ) ,
    m_col( col ) ,
    m_default_color( defaultColor )
{

}

/**
* @brief Dtr
*/
PointCloud::~PointCloud()
{
  glDeleteVertexArrays( 1 , &m_vao ) ;
  glDeleteBuffers( 1 , &m_vbo ) ;
}


/**
* @brief Prepare object before rendering (ie: create buffers, prepare data)
* @note openGL context must be active when calling this function
*/
void PointCloud::prepare( void )
{
  if( m_prepared )
  {
    return ;
  }

  const uint32_t nb_vert = m_pts.size() ;
  m_nb_vert = nb_vert ;
  const uint32_t nb_component_per_vert = 6 ;
  const uint32_t nb_total_elt = nb_vert * nb_component_per_vert ;

  GLfloat * data = new GLfloat[ nb_total_elt ] ;

  for( uint32_t id_vert = 0 ; id_vert < nb_vert ; ++id_vert )
  {
    // Position
    data[ nb_component_per_vert * id_vert ] = m_pts[id_vert][ 0 ] ;
    data[ nb_component_per_vert * id_vert + 1 ] = m_pts[id_vert][ 1 ] ;
    data[ nb_component_per_vert * id_vert + 2 ] = m_pts[id_vert][ 2 ] ;

    // Color
    if( m_col.size() > 0 )
    {
      data[ nb_component_per_vert * id_vert + 3 ] = m_col[id_vert][0] ;
      data[ nb_component_per_vert * id_vert + 4 ] = m_col[id_vert][1] ;
      data[ nb_component_per_vert * id_vert + 5 ] = m_col[id_vert][2] ;
    }
    else
    {
      data[ nb_component_per_vert * id_vert + 3 ] = m_default_color[0] ;
      data[ nb_component_per_vert * id_vert + 4 ] = m_default_color[1] ;
      data[ nb_component_per_vert * id_vert + 5 ] = m_default_color[2] ;
    }
  }

  // set vertex data
  glGenVertexArrays( 1 , &m_vao ) ;
  glBindVertexArray( m_vao ) ;
  glGenBuffers( 1 , &m_vbo ) ;
  glBindBuffer( GL_ARRAY_BUFFER , m_vbo ) ;
  glBufferData( GL_ARRAY_BUFFER , nb_total_elt * sizeof( GLfloat ) , data , GL_STATIC_DRAW ) ;

  GLint pos = m_shader->attribLocation( "inPos" ) ;
  GLint col = m_shader->attribLocation( "inCol" ) ;

  if( pos == -1 || col == -1 )
  {
    std::cerr << "Object wont be drawn correcly" << std::endl ;

    if( pos == -1 )
    {
      std::cerr << "shader does not have an active \"inPos\" attrib" << std::endl ;
    }
    if( col == -1 )
    {
      std::cerr << "shader does not have an active \"inCol\" attrib" << std::endl;
    }
  }
  if( pos != -1 )
  {
    glEnableVertexAttribArray( pos ) ;
    glVertexAttribPointer( pos , 3 , GL_FLOAT , GL_FALSE , 6 * sizeof( GLfloat ) , ( GLvoid * ) 0 ) ;
  }
  if( col != -1 )
  {
    glEnableVertexAttribArray( col ) ;
    glVertexAttribPointer( col , 3 , GL_FLOAT , GL_FALSE , 6 * sizeof( GLfloat ) , ( GLvoid* ) ( 3 * sizeof( GLfloat ) ) ) ;
  }

  glBindBuffer( GL_ARRAY_BUFFER , 0 ) ;
  glBindVertexArray( 0 );

  delete[] data ;
  m_prepared = true ;
}

/**
* @brief Draw code for the object
*/
void PointCloud::draw( void ) const
{
  glBindVertexArray( m_vao ) ;
  glDrawArrays( GL_POINTS , 0 , m_nb_vert ) ;
  glBindVertexArray( 0 ) ;
}


} // namespace openMVG_gui