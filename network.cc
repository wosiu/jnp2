#include "network.h"
#include <map>
#include <unordered_map>
#include <unordered_set>
#include <tuple>
#include <iostream> //( ios_base::Init )

using namespace std;

#ifndef NDEBUG
    const bool debug = true;
#else
    const bool debug = false;
#endif

typedef unsigned long net_id;
typedef string node_label;
typedef unordered_set < node_label > nodes;
// first: in_links, second: out_links
typedef pair <nodes, nodes> links;
// first: node_label, second: node_links
typedef unordered_map <node_label, links> graph;
// 0: is_growing, 1: total_links_number, 2: graph
typedef tuple <bool, int, graph> net;
// first: net_id, second: net
typedef map <net_id, net> network;
//network::iterator network_it;


// funckje pomocnicze
network& get_network()
{
	static network nw;
	return nw;
}

net_id get_net_id( network::iterator network_it )
{
	return network_it->first;
}

net& get_net( network::iterator network_it )
{
	return network_it->second;
}

network::iterator find_net( net_id id )
{
	return get_network().find( id );
}

node_label get_node_label( graph::iterator graph_it )
{
	return graph_it->first;
}

nodes* get_in_links( links& lks )
{
	return &(lks.first);
}

nodes* get_in_links( graph::iterator graph_it )
{
	return get_in_links( graph_it->second );
}

nodes* get_out_links( links& lks )
{
	return &(lks.second);
}

nodes* get_out_links( graph::iterator graph_it )
{
	return get_out_links( graph_it->second );
}

bool is_growing( net& nt )
{
	return get<0>(nt);
}

int& net_links_number( net& nt )
{
	return get<1>(nt);
}

graph* net_graph( net& nt )
{
	return &get<2>(nt);
}

graph* net_graph( network::iterator network_it )
{
	return net_graph( network_it->second );
}


// O( log( rozmiar podsieci nt ) )
bool remove_link_between_nodes( net* nt, graph::iterator node_s, graph::iterator node_t )
{
	// pobieramy nazwy wierzcholkow
	node_label slabel = get_node_label( node_s );
	node_label tlabel = get_node_label( node_t );
	if ( slabel.empty() ) {
		if ( debug )
			cerr << "remove_link_between_nodes: slabel is null\n";
		return false;
	}
	if ( tlabel.empty() ) {
		if ( debug )
			cerr << "remove_link_between_nodes: tlabel is null\n";
		return false;
	}

	// pobieramy polaczenia wychodzace z node_s
	nodes* out = get_out_links( node_s );
	// pobieramy polaczenia wchodzace do node_t
	nodes* in = get_in_links( node_t );
	// probujemy usunac tlabel z out links node_s i sprawdzamy czy usunal
	// = czy slabel mial polaczenie do tlabel
	if ( out->erase( tlabel ) == 0 ) {
		// jesli nie mial polaczenia, to w tlabel nie ma tez wchodzacego
		// z zalozenia ze jest to sytuacja symetryczna
		if ( debug )
			cerr << "remove_link_between_nodes: link does not exist\n";
		return false;
	}
	// analogicznie w druga strone (slabel z in links tlabel),
	// przy czym tu mamy pewnosc, ze tlabel ma w in links slabel
	in->erase( slabel );
	//zmniejszamy ilosc polaczen w tej sieci
	net_links_number( *nt )--;

	return true;
}

// funkcje podstawowe

// O( log( liczba podsieci ) )
net_id network_new( int growing )
{
	if ( debug )
		cerr << "network_new(" << growing << ")\n";

	int id = 1;
	network::iterator network_it;
	if ( !get_network().empty() ) {
		// pobranie najwiekszego dotychczasowego id i zwiekszenie o 1
		network_it = get_network().end();
		network_it--;
		id = get_net_id( network_it ) + 1;
	}
	// wkladanie nowej sieci
	get_network()[ id ] = net( ( growing != 0 ), 0, graph() );
	if ( debug )
		cerr << "network_new: network " << id << " created\n";
	return id;
}

// O( log( liczba podsieci ) )
void network_delete( net_id id )
{
	if ( debug )
		cerr << "network_delete(" << id << ")\n";

	if ( get_network().erase( id ) == 0 && debug ) {
		cerr << "network_delete: network " << id << " doesn't exist, nothing to do here\n";
		return;
	}
	if ( debug )
		cerr << "network_delete: network " << id << " was deleted\n";
}

// O( log( liczba podsieci ) )
size_t network_nodes_number( net_id id )
{
	if ( debug )
		cerr << "network_nodes_number(" << id << ")\n";
	network::iterator network_it = find_net( id );
	if ( network_it == get_network().end() ) {
		if ( debug )
			cerr << "network_nodes_number: network " << id << " doesn't exist, returning 0\n";
		return 0;
	}
	if ( debug )
		cerr << "network_nodes_number: returning " << net_graph( network_it )->size() << "\n";
	return net_graph( network_it )->size();
}

// O( log( liczba podsieci ) )
size_t network_links_number( net_id id )
{
	if ( debug )
		cerr << "network_links_number(" << id << ")\n";
	network::iterator network_it = find_net( id );
	if ( network_it == get_network().end() ) {
		if ( debug )
			cerr << "network_links_number: network " << id << " doesn't exist, returning 0\n";
		return 0;
	}
	if ( debug )
		cerr << "network_links_number: returning " << net_links_number( get_net( network_it ) ) << "\n";
	return net_links_number( get_net( network_it ) );
}


// O( log( liczba podsieci ) + log( rozmiar podsieci o id ) )
void network_add_node( net_id id, const char* label_ )
{
	if ( debug )
		cerr << "network_add_node(" << id << ", " << label_ << ")\n";

	node_label label = label_ == NULL ? "" : label_;

	network::iterator network_it = find_net( id );
	if ( network_it == get_network().end() ) {
		if ( debug )
			cerr << "network_add_node: network doesn't exist, nothing to do here\n";
		return;
	}
	if ( label.empty() ) {
		if ( debug )
			cerr << "network_add_node: label is empty\n";
		return;
	}
	graph* g = net_graph( network_it );
	// operator [] doda node do grafu jesli nie istnial
	if ( debug ) {
		if(g->find(label) == g-> end())
			cerr << "network_add_node: node " << label << " already exists in network " << id << "\n";
		else
			cerr << "network_add_node: adding node " << label << " to network " << id << "\n";
	}
	(*g)[ label ];
}

// O( log( liczba podsieci ) + log( rozmiar podsieci o id ) )
void network_add_link( net_id id, const char* slabel_, const char* tlabel_ )
{
	if ( debug )
		cerr << "network_add_link(" << id << ", " << slabel_ << ", " << tlabel_ << ")\n";

	node_label slabel = slabel_ == NULL ? "" : slabel_;
	node_label tlabel = tlabel_ == NULL ? "" : tlabel_;

	if ( slabel.empty() || tlabel.empty() ) {
		if ( debug ) {
			if(slabel.empty())
				cerr << "network_add_link: slabel is null, nothing to do here\n";
			if(tlabel.empty())
				cerr << "network_add_link: tlabel is null, nothing to do here\n";
		}
		return;
	}
	network::iterator network_it = find_net( id );
	if ( network_it == get_network().end() ) {
		if ( debug )
			cerr << "network_add_link: network " << id << " doesn't exist, nothing to do here\n";
		return;
	}
	graph* g = (net_graph( network_it ));
	// jesli nie istnial wierzcholek o etykiecie slabel to operator [] tworzy go
	nodes* out = get_out_links( (*g)[ slabel ] );

	// sprawdzamy czy krawedz istniala wczesniej
	if ( out->find( tlabel ) != out->end() ) {
		if ( debug )
			cerr << "network_add_link: link " << slabel << " -> " << tlabel <<
			" in network " << id << " already exists, nothing to do here\n";
		return;
	}
	net_links_number( get_net( network_it ) )++;
	nodes* in = get_in_links( (*g)[ tlabel ] );
	out->insert( tlabel );
	in->insert( slabel );
	if ( debug )
		cerr << "network_add_link: node " << slabel << " -> " << tlabel << " was added to network " << id << "\n";

}

// O ( log( liczba podsieci ) + l log n ), gdzie
// n = rozmiar podsieci o id,
// l = ilosc krawedzi wchodzacych i wychodzacych do wierzcholka o label
void network_remove_node( net_id id, const char* label_ )
{
	if ( debug )
		cerr << "network_remove_node(" << id << ", " << label_ << ")\n";

	node_label label = label_ == NULL ? "" : label_;

	// O ( log( liczba podsieci ) )
	network::iterator network_it = find_net( id );
	if ( network_it == get_network().end() ) {
		if ( debug )
			cerr << "network_remove_node: network " << id << " doesn't exist, nothing to do here\n";
		return;
	}
	net* nt = &get_net( network_it );
	if ( is_growing( *nt ) ) {
		if ( debug )
			cerr << "network_remove_node: network " << id << " is growing, can't remove node\n";
		return;
	}
	graph* g = net_graph( *nt );
	graph::iterator graph_it_s, graph_it_t;
	// O ( log n ), gdzie n = rozmiar podsieci o id
	// szukamy w grafie wierzcholka o label
	graph_it_s = g->find( label );
	if ( graph_it_s == g->end() ) {
		if ( debug )
			cerr << "network_remove_node: node " << label << "doesn't exist in network " << id << ", nothing to do here\n";
		return;
	}
	nodes* out = get_out_links( graph_it_s );
	nodes* in =  get_in_links ( graph_it_s );
	// usuwamy polaczenia wychodzace z usuwanego wierzcholka -> (musimy
	// zaktualizowac polaczenia wchodzace w sasiadach wierzcholka o label)
	while ( !out->empty() ) {
		node_label t_label = *( out->begin() );
		graph_it_t = g->find( t_label );
		if ( debug )
			cerr << "network_remove_node: removing link " << label << " -> " << t_label << "\n";
		if( remove_link_between_nodes( nt, graph_it_s, graph_it_t ) && debug )
			cerr << "network_remove_node: link " << label << " -> " << t_label << " removed\n";
	}
	// oraz usuwamy polaczenia odwrotne: wchodzace do wierzcholka o label
	graph_it_t = graph_it_s;
	while ( !in->empty() ) {
		node_label s_label = *( in->begin() );
		graph_it_s = g->find( s_label );
		if ( debug )
			cerr << "network_remove_node: removing link " << s_label << " -> " << label << "\n";
		if ( remove_link_between_nodes( nt, graph_it_s, graph_it_t ) && debug )
			cerr << "network_remove_node: link " << s_label << " -> " << label << " removed\n";
	}
	// po usunieciu polaczen usuwamy wierzcholek
	g->erase( graph_it_t );
	if ( debug )
		cerr << "network_remove_node: node " << label << " removed from network " << id << "\n";
}


void network_remove_link( net_id id, const char* slabel_, const char* tlabel_ )
{
	if ( debug )
		cerr << "network_remove_link(" << id << ", " << slabel_ << ", " << tlabel_ << ")\n";

	node_label slabel = slabel_ == NULL ? "" : slabel_;
	node_label tlabel = tlabel_ == NULL ? "" : tlabel_;

	network::iterator network_it = find_net( id );
	if ( network_it == get_network().end() ) {
		if ( debug )
			cerr << "network_remove_link: network " << id << " doesn't exist, nothing to do here\n";
		return;
	}
	net* nt = &get_net( network_it );
	if ( is_growing( *nt ) ) {
		if ( debug )
			cerr << "network_remove_link: network " << id << " is growing, can't remove link, exiting\n";
		return;
	}

	graph* g = net_graph( *nt );
	graph::iterator graph_it_s, graph_it_t;
	// znajdujemy w grafie wierzcholki o etykietach slabel i tlabel
	// O( 2 log (rozmiar sieci nt) )
	graph_it_s = g->find( slabel );
	graph_it_t = g->find( tlabel );
	// jesli ktoregos nie znaleziono, to nie ma co usuwac
	if ( graph_it_s == g->end() ) {
		if ( debug )
			cerr << "net_remove_link: can't find label " << slabel << "\n";
		return;
	}
	if ( graph_it_t == g->end() ) {
		if ( debug )
			cerr << "net_remove_link: can't find label " << tlabel << "\n";
		return;
	}
	// O( log( rozmiar podsieci nt ) )
	if ( !remove_link_between_nodes( nt, graph_it_s, graph_it_t ) ) {
		return;
	}

	if ( debug )
		cerr << "network_remove_link: link " << slabel << " -> " << tlabel << " in network " << id << " removed\n";
}

// O( log( liczba podsieci ) )
void network_clear( net_id id )
{
	if ( debug )
		cerr << "network_clear(" << id << ")\n";
	network::iterator network_it = find_net( id );
	if ( network_it == get_network().end() ) {
		if ( debug )
			cerr << "network_clear: network " << id << " doesn't exist\n";
		return;
	}
	if ( is_growing( get_net( network_it ) ) ) {
		if ( debug )
			cerr << "network_clear: network " << id << " is growing, can't clear\n";
		return;
	}
	get_net( network_it ) = net( 0, 0, graph() );
	if ( debug )
		cerr << "network_clear: network " << id << " cleared\n";
}

// O( log( liczba podsieci ) + log( rozmiar podsieci o id ) )
size_t network_out_degree( net_id id, const char* label_ )
{
	if ( debug )
		cerr << "network_out_degree(" << id << ", " << label_ << ")\n";

	node_label label = label_ == NULL ? "" : label_;

	if ( label.empty() ) {
		if ( debug )
			cerr << "network_out_degree: label is null, returning 0\n";
		return 0;
	}
	network::iterator network_it = find_net( id );
	if ( network_it == get_network().end() ) {
		if ( debug )
			cerr << "network_out_degree: can't find network " << id << '\n';
		return 0;
	}
	graph* g = net_graph( network_it );
	graph::iterator graph_it_s, graph_it_t;
	graph_it_s = g->find( label );
	if ( graph_it_s == g->end() ) {
		if ( debug )
			cerr << "network_out_degree: can't find node " << label << " in network " << id << ", returning 0\n";
		return 0;
	}
	nodes* out = get_out_links( graph_it_s );
	if ( debug )
		cerr << "network_out_degree: returning " << out->size() << "\n";
	return out->size();
}

// O( log( liczba podsieci ) + log( rozmiar podsieci o id ) )
size_t network_in_degree( net_id id, const char* label_ )
{
	if ( debug )
		cerr << "network_in_degree(" << id << ", " << label_ << ")\n";

	node_label label = label_ == NULL ? "" : label_;

	if ( label.empty() ) {
		if ( debug )
			cerr << "network_in_degree: label is null, returning 0\n";
		return 0;
	}
	network::iterator network_it = find_net( id );
	if ( network_it == get_network().end() ) {
		if ( debug )
			cerr << "network_in_degree: can't find network " << id << ", returning 0\n";
		return 0;
	}
	graph* g = net_graph( network_it );
	graph::iterator graph_it_s, graph_it_t;
	graph_it_s = g->find( label );
	if ( graph_it_s == g->end() ) {
		if ( debug )
			cerr << "network_in_degree: can't find node " << label << " in network " << id << ", returning 0\n";
		return 0;
	}
	nodes* in = get_in_links( graph_it_s );
	if ( debug )
		cerr << "network_in_degree: returning " << in->size() << "\n";
	return in->size();
}
