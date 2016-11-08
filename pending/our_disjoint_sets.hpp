/*======================================================================
                        "[nome_progetto]"
        Course on Advanced Programming for Scientific Computing
                      Politecnico di Milano
                          A.Y. 2015-2016
                  
         Copyright (C) 2016 Ilaria Speranza & Mattia Tantardini
======================================================================*/
/*! 
	@file   our_disjoint_sets.hpp
	@author Ilaria Speranza & Mattia Tantardini
	@date   Sep 2016.
	@brief  Class to handle disjoint sets
	@detail
*/

#ifndef HH_OUR_DISJOINT_SETS_HH
#define HH_OUR_DISJOINT_SETS_HH

#include <iostream>
#include <map>
#include <tuple>
#include <list>
#include <boost/graph/graph_traits.hpp>

/*!
	@brief Template class to handle disjoint sets
	@detail The template parameters are:\n
			Label_map_t: the type of a std::map which key is a vertex descriptor and the value is an unsigned
				int which has the meaning of the current label of the component to which that vertex belongs to.\n
			Components_map_t: the type of a std::map which key is an unsigned int used as label for the group
				and the value is a std::set containing all the vertex descriptor of the vertices
				that have that label, i.e that belong to the same component.
*/
template <typename Graph>
class our_disjoint_sets{
	public:
		//qua dovremmo fare un bel po' di typedefs con tipo Label_map::key_type = lbel_key_type, Label_map::value_type = label_value_type, ecc.
		typedef typename boost::graph_traits<Graph>::vertex_iterator Vertex_iter;
		typedef typename boost::graph_traits<Graph>::vertex_descriptor Vertex_desc;
		
		typedef typename std::map<Vertex_desc, unsigned int> Label_map_t;
		typedef typename std::map<unsigned int, std::list<Vertex_desc> > Components_map_t;		//Possibilità di considerare anche il numero di elementi nella lista e aggiornarlo man mano per velocizzare inserzioni di pochi vertici in gruppi da tanti vertici
		
		typedef typename Label_map_t::key_type Label_key_t;
		typedef typename Label_map_t::mapped_type Label_mapped_t;
		typedef typename Components_map_t::key_type Components_key_t;
		typedef typename Components_map_t::mapped_type Components_mapped_t;	//è il tipo della lista;
		typedef typename Components_mapped_t::value_type Comp_mapped_vertex_t;		//è Vertex_desc in sostanza, cioè il template di std::list
		
		//CI VUOLE UN ASSERT SE Label_map_t E Components_key_t SONO LO STESSO TIPO!!!		
		
		//! Default constructor
		our_disjoint_sets(Graph & _G) : G(_G), label_map(), components_map() {};
		
		//! Copy constructor
		our_disjoint_sets (our_disjoint_sets const&) = default;
		
		//! Assignment operator
		our_disjoint_sets & operator=(our_disjoint_sets const&) = default;
		
		//! Destructor
		~our_disjoint_sets(){};
		
		/*!
			@brief It creates the label map starting form the Graph
			@detail The label_map is set up by associating to each vertex descriptor 
					a progressive unsigned int as label, that indicates to which component
					the vertex belongs to. In other words, label_map is set up by assuming
					that each vertex is a separated component.		
		*/
		void make_label_map(){
			unsigned int label_value = 0;
			Vertex_iter v_it, v_end;
			for(std::tie(v_it, v_end) = boost::vertices(G); v_it != v_end; ++v_it){
				label_map[*v_it] = label_value;
				label_value++;
			}			
		};
		
		//! It returns, from the label_map, the label of the component which the vertex belongs to
		Label_mapped_t get_label(Label_key_t const& vertex){
			return label_map.at(vertex);	//meglio at() perché dà un exception se quella chiave non esiste, sennò con [] avrebbe creato un nuovo elemento
		};
		
		//! It allows to set the label of that vertex in label map
		void set_label(Label_key_t const& vertex, Label_mapped_t const& label){
			label_map[vertex] = label;
		};
		
		//! Checks if a particular component (i.e its label) is already present in the components_map
		bool is_present_component(Components_key_t const& label_of_the_component){
			if(components_map.find(label_of_the_component) == components_map.end())
				return false;	//find restituisce l'iterator map:end() se l'elemento con quella chiave non c'è
			else
				return true;
		};
		
		//! It returns a pair containing the iterator to begin and end of the list that contains all the verteices of the given component
		std::pair<typename Components_mapped_t::iterator, typename Components_mapped_t::iterator>
		get_iterator(Components_key_t const& label_of_the_component){
			return std::make_pair	(components_map[label_of_the_component].begin(),
									components_map[label_of_the_component].end() );
		};		
		
		//! It creates a new component with the given label value as key in components_map
		void new_component(Components_key_t const& label_value){
			components_map[label_value];		//creao lo slot con quella label e lista default constructor
		};
		
		// new_component_with_insertion?
		
		//! It add the given vertex descriptor to the component with that label
		void insert_vertex_in_component(Comp_mapped_vertex_t const& vertex, Components_key_t const& label_value){
			//Usiamo at() e non [] così ci dà un'exception se non esiste quella chiave. Che però l'exception va raccolta
			components_map.at(label_value).push_back(vertex);		//carica la lista.
		};
		
		//! It insert the target component in the source component
		void insert_tgt_comp_in_src_comp(Components_key_t const& tgt_label_value, Components_key_t const& src_label_value){
			typedef typename our_disjoint_sets<Graph>::Components_mapped_t::iterator Comp_iter;
			Comp_iter tgt_comp_begin, tgt_comp_end;
			Comp_iter src_comp_begin, src_comp_end;
			std::tie(tgt_comp_begin, tgt_comp_end) = this->get_iterator(tgt_label_value);
			std::tie(src_comp_begin, src_comp_end) = this->get_iterator(src_label_value);
			components_map[src_label_value].insert(src_comp_end, tgt_comp_begin, tgt_comp_end);
		};
		
		//! It removes from components_map the component with the given key (=label of the component)
		void erase_component(Components_key_t const& label_value){
			components_map.erase(label_value);
		};
	
		//! It returns the components_map outside the class
		Components_map_t get_components_map(){
			return components_map;
		};
		
		//! Overloading of operator<< to view components_map
		friend std::ostream & operator<< (std::ostream & out, our_disjoint_sets & dsets){
			auto&& map = dsets.get_components_map();
			typename Components_mapped_t::iterator list_it, list_end;
			out << "Size of components_map: " << (dsets.get_components_map()).size() << std::endl;
			
			for(auto map_it = map.begin(); map_it != map.end(); ++map_it){
				std::cout << "begin" << std::endl; 
				dsets.get_components_map().at(map_it->first);
				out << "Componente con etichetta " << map_it->first << " contiene i vertici: ";
				for(std::tie(list_it, list_end) = dsets.get_iterator(map_it->first); list_it != list_end; ++list_it){
					out << *list_it << " ";
				}	//for
				out <<  std::endl;
				std::cout << "end" << std::endl; 
			}	//for			
		};
	
	private:
		Graph & G;
		Label_map_t	label_map;
		Components_map_t components_map;
};

#endif	//HH_OUR_DISJOINT_SETS_HH
