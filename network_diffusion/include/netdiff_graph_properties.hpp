/*======================================================================
                        "BGLgeom library"
        Course on Advanced Programming for Scientific Computing
                      Politecnico di Milano
                          A.Y. 2015-2016
                  
         Copyright (C) 2017 Ilaria Speranza & Mattia Tantardini
======================================================================*/
/*!
	@file	netdiff_graph_properties.hpp
	@author Ilaria Speranza & Mattia Tantardini
	@date 	Jan, 2017
	@brief	Properties that the graph for diffusion on network appliocation will have
	
	All the data structures have to be provided with at least 
	the default constructor.
*/

#ifndef HH_NETDIFF_GRAPH_PROPERTIES_HH
#define HH_NETDIFF_GRAPH_PROPERTIES_HH

#include "data_structure.hpp"
#include "linear_geometry.hpp"

namespace NetDiff{

//! Properties that vertices will have
using Vertex_prop = BGLgeom::Vertex_base_property<3>;

//! Properties that vertices will have
struct Edge_prop : public BGLgeom::Edge_base_property<BGLgeom::linear_geometry<3>,3>{
	//! Diameter
	double diam;
	
	//! Default constructor
	Edge_prop() : 	BGLgeom::Edge_base_property<BGLgeom::linear_geometry<3>,3>(),
					diam() {};
	//! Constructor
	Edge_prop(double const& _diam) : BGLgeom::Edge_base_property<BGLgeom::linear_geometry<3>,3>(),
									diam(_diam) {};
	//! Explicit declaration of default assignment operator
	Edge_prop & operator=(Edge_prop const&) = default;
	
};	//Edge_prop 

/*!
	@brief Topological properties of the graph
	
	The input file gives us explicit infos about the structure of the
	graph. We can use this infos to index the vertex descriptors like
	the given data.
*/
struct Topological_prop{
	/*! 
		@brief Vertex_descriptor for the source
		@remark We use an unsigned int as vertex descriptor since we know 
				(from BGL) that the type of adjacency_list we choose to 
				represent the graph uses unsigned int as vertex descriptor
	*/
	unsigned int src;
	/*! 
		@brief Vertex_descriptor for the source
		@remark We use an unsigned int as vertex descriptor since we know 
				(from BGL) that the type of adjacency_list we choose to 
				represent the graph uses unsigned int as vertex descriptor
	*/
	unsigned int tgt;
	
	//! Default constructor
	Topological_prop() : src(), tgt() {};
	
	//! Constructor
	Topological_prop(unsigned int const& _src, unsigned int const& _tgt) : src(_src), tgt(_tgt) {};
};	//Topological_prop

}	//NetDiff

#endif	//HH_NETDIFF_GRAPH_PROPERTIES_HH
