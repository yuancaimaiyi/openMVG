// This file is part of OpenMVG, an Open Multiple View Geometry C++ library.

// Copyright (c) 2017 Romuald PERROT.

// This Source Code Form is subject to the terms of the Mozilla Public
// License, v. 2.0. If a copy of the MPL was not distributed with this
// file, You can obtain one at http://mozilla.org/MPL/2.0/.

#include "FeatureParams.hh"

#include "openMVG/features/akaze/image_describer_akaze.hpp"
#include "openMVG/features/sift/SIFT_Anatomy_Image_Describer.hpp"
#include "nonFree/sift/SIFT_describer.hpp"

using namespace openMVG ;
using namespace openMVG::features ;


namespace openMVG_gui
{

/**
* @brief ctr
* @param type The feature type to use
* @param preset The feature preset to use
* @param upright Indicate if feature are extracted using upright orientation
*/
FeatureParams::FeatureParams( const FeatureType & type ,
                              const FeaturePreset & preset ,
                              const bool upright  )
  : m_feat_type( type ) ,
    m_feat_preset( preset ) ,
    m_upright( upright ) ,
    m_describer( nullptr )
{
  createDescriber() ;
}

/**
* @brief Get feature type
*/
FeatureType FeatureParams::type( void ) const
{
  return m_feat_type ;
}

/**
* @brief Set feature type
*/
void FeatureParams::setType( const FeatureType & type )
{
  if( type != m_feat_type )
  {
    createDescriber() ;
  }
}


/**
* @brief Get preset mode
*/
FeaturePreset FeatureParams::preset( void ) const
{
  return m_feat_preset ;
}

/**
* @brief set preset mode
*/
void FeatureParams::setPreset( const FeaturePreset & preset )
{
  if( preset != m_feat_preset )
  {
    m_feat_preset = preset ;
    createDescriber() ;
  }
}


/**
* @brief indicate if the describer is in upright orientation
*/
bool FeatureParams::upright( void ) const
{
  return m_upright ;
}

/**
* @brief set upright mode
*/
void FeatureParams::setUpright( const bool ur )
{
  if( ur != m_upright )
  {
    m_upright = ur ;
    createDescriber() ;
  }
}

/**
* @brief Get describer
* @return the describer corresponding to the current parameters
*/
std::shared_ptr<openMVG::features::Image_describer> FeatureParams::describer( void ) const
{
  return m_describer ;
}

/**
* @brief Create internal describer
*/
void FeatureParams::createDescriber( void )
{
  m_describer = nullptr ;
  // Create the desired Image_describer method.
  switch( m_feat_type )
  {
    case FEATURE_TYPE_SIFT :
    {
      m_describer = std::make_shared< SIFT_Image_describer >( SIFT_Image_describer::Params() , ! m_upright ) ;
      break ;
    }
    case FEATURE_TYPE_SIFT_ANATOMY:
    {
      m_describer = std::make_shared< SIFT_Anatomy_Image_describer >( SIFT_Anatomy_Image_describer::Params() ) ;
      break ;
    }
    case FEATURE_TYPE_AKAZE_FLOAT :
    {
      m_describer = AKAZE_Image_describer::create( AKAZE_Image_describer::Params( AKAZE::Params(), AKAZE_MSURF ), !m_upright );
      break ;
    }
    case FEATURE_TYPE_AKAZE_MLDB :
    {
      m_describer = AKAZE_Image_describer::create( AKAZE_Image_describer::Params( AKAZE::Params(), AKAZE_MLDB ), !m_upright );
      break ;
    }
  }
  if( m_describer )
  {
    switch( m_feat_preset )
    {
      case FEATURE_PRESET_NORMAL:
      {
        m_describer->Set_configuration_preset( NORMAL_PRESET ) ;
        break ;
      }
      case FEATURE_PRESET_HIGH :
      {
        m_describer->Set_configuration_preset( HIGH_PRESET ) ;
        break ;
      }
      case FEATURE_PRESET_ULTRA :
      {
        m_describer->Set_configuration_preset( ULTRA_PRESET ) ;
        break ;
      }
    }
  }
}

/**
 * @brief Get all kinds of combinations (features_type/feature_presets)
 * @return All valid (usable) combinations
 */
std::vector< FeatureParams > FeatureParams::allFeatures( void )
{
  std::vector< FeatureParams > res ;

  // SIFT 
  res.emplace_back( FEATURE_TYPE_SIFT , FEATURE_PRESET_NORMAL ) ;
  res.emplace_back( FEATURE_TYPE_SIFT , FEATURE_PRESET_HIGH ) ;
  res.emplace_back( FEATURE_TYPE_SIFT , FEATURE_PRESET_ULTRA ) ;

  // SIFT ANATOMY 
  res.emplace_back( FEATURE_TYPE_SIFT_ANATOMY , FEATURE_PRESET_NORMAL ) ;
  res.emplace_back( FEATURE_TYPE_SIFT_ANATOMY , FEATURE_PRESET_HIGH ) ;
  res.emplace_back( FEATURE_TYPE_SIFT_ANATOMY , FEATURE_PRESET_ULTRA ) ;

  // AKAZE FLOAT (SURF)
  res.emplace_back( FEATURE_TYPE_AKAZE_FLOAT , FEATURE_PRESET_NORMAL ) ;
  res.emplace_back( FEATURE_TYPE_AKAZE_FLOAT , FEATURE_PRESET_HIGH ) ;
  res.emplace_back( FEATURE_TYPE_AKAZE_FLOAT , FEATURE_PRESET_ULTRA ) ;

  // AKAZE MLDB 
  res.emplace_back( FEATURE_TYPE_AKAZE_MLDB , FEATURE_PRESET_NORMAL ) ;
  res.emplace_back( FEATURE_TYPE_AKAZE_MLDB , FEATURE_PRESET_HIGH ) ;
  res.emplace_back( FEATURE_TYPE_AKAZE_MLDB , FEATURE_PRESET_ULTRA ) ;

  return res ;
}


} // namespace openMVG_gui
