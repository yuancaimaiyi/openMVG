// Copyright (c) 2012, 2013 openMVG authors.

// This Source Code Form is subject to the terms of the Mozilla Public
// License, v. 2.0. If a copy of the MPL was not distributed with this
// file, You can obtain one at http://mozilla.org/MPL/2.0/.

#ifndef OPENMVG_GRAPH_CONNECTED_COMPONENT_H_
#define OPENMVG_GRAPH_CONNECTED_COMPONENT_H_

#include <openMVG/types.hpp>
#include <openMVG/graph/graph.hpp>
#include <openMVG/tracks/union_find.hpp>
#include <set>

namespace openMVG
{
namespace graph
{


/**
* @brief Export node of each CC (Connected Component) in a map
* @param g Input graph
* @return Connected component of input graph
*/
template <typename GraphT, typename IndexT>
std::map<IndexT, std::set<lemon::ListGraph::Node> >  exportGraphToMapSubgraphs
(
  const GraphT & g
)
{
  typedef lemon::ListGraph::NodeMap<IndexT> IndexMap;
  IndexMap connectedNodeMap( g );
  lemon::connectedComponents( g, connectedNodeMap );

  std::map<IndexT, std::set<lemon::ListGraph::Node> > map_subgraphs;

  // Create subgraphs' map
  typedef lemon::ListGraph::NodeIt NodeIterator;
  NodeIterator itNode( g );
  for ( typename IndexMap::MapIt it( connectedNodeMap );
        it != lemon::INVALID; ++it, ++itNode )
  {
    map_subgraphs[*it].insert( itNode );
  }
  return map_subgraphs;
}

/**
* @brief Computes nodeIds that belongs to the largest bi-edge connected component
* @param edges List of edges
* @return nodeIds that belongs to the largest bi-edge connected component
*/
template<typename EdgesInterface_T, typename IndexT>
std::set<IndexT> CleanGraph_KeepLargestBiEdge_Nodes
(
  const EdgesInterface_T & edges
)
{
  std::set<IndexT> largestBiEdgeCC;

  // Create a graph from pairwise correspondences:
  // - remove not biedge connected component,
  // - keep the largest connected component.

  typedef lemon::ListGraph Graph;
  graph::indexedGraph putativeGraph(edges);

  // Remove not bi-edge connected edges
  typedef Graph::EdgeMap<bool> EdgeMapAlias;
  EdgeMapAlias cutMap( putativeGraph.g );

  if ( lemon::biEdgeConnectedCutEdges( putativeGraph.g, cutMap ) > 0 )
  {
    // Some edges must be removed because they don't follow the biEdge condition.
    typedef Graph::EdgeIt EdgeIterator;
    EdgeIterator itEdge( putativeGraph.g );
    for ( EdgeMapAlias::MapIt it( cutMap ); it != lemon::INVALID; ++it, ++itEdge )
    {
      if ( *it )
      {
        putativeGraph.g.erase( itEdge );  // remove the not bi-edge element
      }
    }
  }

  // Graph is bi-edge connected, but still many connected components can exist
  // Keep only the largest one
  const int connectedComponentCount = lemon::countConnectedComponents( putativeGraph.g );
  std::cout << "\n" << "CleanGraph_KeepLargestBiEdge_Nodes():: => connected Component: "
            << connectedComponentCount << std::endl;
  if ( connectedComponentCount >= 1 )
  {
    // Keep only the largest connected component
    // - list all CC size
    // - if the largest one is meet, keep all the edges that belong to this node

    const std::map<IndexT, std::set<Graph::Node> > map_subgraphs = exportGraphToMapSubgraphs<Graph, IndexT>( putativeGraph.g );
    size_t count = std::numeric_limits<size_t>::min();
    typename std::map<IndexT, std::set<Graph::Node> >::const_iterator iterLargestCC = map_subgraphs.end();
    for( typename std::map<IndexT, std::set<Graph::Node> >::const_iterator iter = map_subgraphs.begin();
         iter != map_subgraphs.end(); ++iter )
    {
      if ( iter->second.size() > count )
      {
        count = iter->second.size();
        iterLargestCC = iter;
      }
      std::cout << "Connected component of size: " << iter->second.size() << std::endl;
    }

    //-- Keep only the nodes that are in the largest CC
    for( typename std::map<IndexT, std::set<lemon::ListGraph::Node> >::const_iterator iter = map_subgraphs.begin();
         iter != map_subgraphs.end(); ++iter )
    {
      if ( iter == iterLargestCC )
      {
        // list all nodes that belong to the current CC and update the Node Ids list
        const std::set<lemon::ListGraph::Node> & ccSet = iter->second;
        for ( std::set<lemon::ListGraph::Node>::const_iterator iter2 = ccSet.begin();
              iter2 != ccSet.end(); ++iter2 )
        {
          const IndexT Id = ( *putativeGraph.map_nodeMapIndex )[*iter2];
          largestBiEdgeCC.insert( Id );
        }
      }
      else
      {
        // remove the edges from the graph
        const std::set<lemon::ListGraph::Node> & ccSet = iter->second;
        for ( std::set<lemon::ListGraph::Node>::const_iterator iter2 = ccSet.begin();
              iter2 != ccSet.end(); ++iter2 )
        {
          typedef Graph::OutArcIt OutArcIt;
          for ( OutArcIt e( putativeGraph.g, *iter2 ); e != lemon::INVALID; ++e )
          {
            putativeGraph.g.erase( e );
          }
        }
      }
    }
  }

  std::cout
    << "\n"
    << "Cardinal of nodes: " << lemon::countNodes( putativeGraph.g ) << "\n"
    << "Cardinal of edges: " << lemon::countEdges( putativeGraph.g ) << std::endl
    << std::endl;

  return largestBiEdgeCC;
}

/**
* @brief Computes nodeIds that belongs to the largest connected component
* @param edges List of edges
* @return nodeIds that belongs to the largest connected component
*/
template<typename EdgesInterface_T, typename IndexT>
std::set<IndexT> KeepLargestCC_Nodes
(
  const EdgesInterface_T & edges
)
{
  // Index nodes as individual components [X->Y] => [0,n]
  std::map<IndexT, unsigned int> node_to_index;
  {
    unsigned int i = 0;
    for (const Pair & pair_it: edges)
    {
      if (node_to_index.count(pair_it.first) == 0)
        node_to_index[pair_it.first] = i++;
      if (node_to_index.count(pair_it.second) == 0)
        node_to_index[pair_it.second] = i++;
    }
  }

  // Establish nodes connections (by using Union Find) on edges
  UnionFind uf;
  {
    // Init singletons
    uf.InitSets(node_to_index.size());
    // Merge containing sets
    for (const Pair & pair_it: edges)
    {
      uf.Union(node_to_index[pair_it.first], node_to_index[pair_it.second]);
    }
  }

  // List the number of parent_id (the number of CCs)
  std::set<unsigned int> parent_id;
  for (const Pair & pair_it: edges)
  {
    parent_id.insert( uf.Find(node_to_index[pair_it.first]) ); // edge first and second go to the same parent_id
  }

  std::set<IndexT> node_ids;
  if (parent_id.size() == 1)
  {
    // There is only one CC, return all ids
    for (const Pair & pair_it: edges)
    {
      node_ids.insert(pair_it.first);
      node_ids.insert(pair_it.second);
    }
  }
  else
  {
    // There is many CC, look the largest one and export containing node ids
    // (if many CC have the same size, export the first that have been seen)
    std::pair<IndexT, unsigned int> max_cc( UndefinedIndexT, std::numeric_limits<unsigned int>::min());
    {
      for (const unsigned int parent_id_it : parent_id)
      {
        if (uf.m_cc_size[parent_id_it] > max_cc.second) // Update the component parent id and size
          max_cc = std::make_pair(parent_id_it, uf.m_cc_size[parent_id_it]);
      }
    }
    if (max_cc.first != UndefinedIndexT)
    {
      // Backup nodes that belong to the largest CC
      const unsigned int parent_id_largest_cc = max_cc.first;
      for (const Pair & pair_it: edges)
      {
        if (uf.Find(node_to_index[pair_it.first]) == parent_id_largest_cc)
        {
          node_ids.insert(pair_it.first);
          node_ids.insert(pair_it.second);
        }
      }
    }
  }
  return node_ids;
}

} // namespace graph
} // namespace openMVG

#endif // OPENMVG_GRAPH_CONNECTED_COMPONENT_H_
