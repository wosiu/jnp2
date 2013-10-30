#include "network.h"
#include <map>
#include <unordered_map>
#include <unordered_set>
#include <tuple>
// TODO: usunac includy, jak bedzie skonczone i debug bedzie wyrzucony:
#include <iostream> //zawiera ios_base::Init
#include <assert.h>

using namespace std;

#ifndef NDEBUG
    const bool debug = true;
#else
    const bool debug = false;
#endif //i do sprawdzania czy debug mode uzywaj if( debug [& ...] ) {[...]}

typedef unsigned long net_id;
typedef const char* node_label;
typedef unordered_set <node_label> nodes;
#define get_in_links first
#define get_out_links second
typedef pair <nodes, nodes> links;
#define get_node_label first
#define get_node_links second
typedef unordered_map <node_label, links> graph;
//0: is_growing, 1: total_links_number, 2: graph
typedef tuple <bool, int, graph> net;
#define get_net_id first
#define get_net second
typedef map <net_id, net> network;
network::iterator network_it;

// makra do debugu - TODO usunac przed wyslaniem
#define debon 1
#define deb(burak) if(debon) {cout<<"DEB-> "<<#burak<<": "<<burak<<endl;}
#define debv(burak) if(debon) {cout<<"DEB-> "<<#burak<<": \t"; for(unsigned int zyx=0;zyx<burak.size();zyx++) cout<<burak[zyx]<<" "; cout<<endl;}
#define debt(burak,SIzE) if(debon) {cout<<"DEB-> "<<#burak<<": \t"; for(unsigned int zyx=0;zyx<SIzE;zyx++) cout<<burak[zyx]<<" "; cout<<endl;}
#define debend if(debon) {cout<<"_____________________"<<endl;}
#define memcheck if(debon) {FILE *fp = fopen("/proc/self/status","r");while( !feof(fp) ) putchar(fgetc(fp));}


// funckje pomocnicze (zakladamy poprawnosc argumentow)
network& get_network()
{
	static network nw;
	return nw;
}

network::iterator get_net( net_id id )
{
	return get_network().find( id );
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

// O( log( rozmiar podsieci nt ) )
void remove_link_between_nodes( net* nt, graph::iterator node_s, graph::iterator node_t )
{
	if(debug)
		cerr << "remove_link_between_nodes\n"
		<< "\tnetwork: " << get<1>(*nt) << "\n"
		<< "\tnode_s: " << node_s -> get_node_label << '\n'
		<< "\tnode_t: " << node_t -> get_node_label << '\n';
	// pobieramy nazwy wierzcholkow
	const char* slabel = node_s->get_node_label;
	const char* tlabel = node_t->get_node_label;
	if(debug)
		if(slabel == NULL || tlabel == NULL) {
			if(slabel == NULL)
				cerr << "slabel is null\n";
			if(tlabel == NULL)
				cerr << "tlabel is null\n";
			return;
		}
			
	// pobieramy polaczenia wychodzace z node_s
	nodes* out = &(node_s->get_node_links.get_out_links);
	// pobieramy polaczenia wchodzace do node_t
	nodes* in = &(node_t->get_node_links.get_in_links);
	// probujemy usunac tlabel z out links node_s i sprawdzamy czy usunal
	// = czy slabel mial polaczenie do tlabel
	if ( out->erase( tlabel ) == 0 ) {
		// jesli nie mial polaczenia, to w tlabel nie ma tez wchodzacego
		// z zalozenia ze jest to sytuacja symetryczna
		return;
	}
	// analogicznie w druga strone (slabel z in links tlabel),
	// przy czym tu mamy pewnosc, ze tlabel ma w in links slabel
	// TODO: usunac assert
	assert( in->erase( slabel ) != 0 );
	//zmniejszamy ilosc polaczen w tej sieci
	net_links_number( *nt )--;
}

// O( log( rozmiar podsieci nt ) )
void net_remove_link( net* nt, const char* slabel, const char* tlabel )
{
	if(debug)
		cerr << "net_remove_link\n"
		<< "\tnetwork: " << get<1>(*nt) << '\n'
		<< "\tslabel: " << slabel << '\n'
		<< "\ttlabel: " << tlabel << '\n';
	graph* g = net_graph( *nt );
	graph::iterator graph_it_s, graph_it_t;
	// znajdujemy w grafie wierzcholki o etykietach slabel i tlabel
	// O( 2 log (rozmiar sieci nt) )
	graph_it_s = g->find( slabel );
	graph_it_t = g->find( tlabel );
	// jesli ktoregos nie znaleziono, to nie ma co usuwac
	if ( graph_it_s == g->end() || graph_it_t == g->end() ) {
		return;
	}
	// O( log( rozmiar podsieci nt ) )
	remove_link_between_nodes( nt, graph_it_s, graph_it_t );
}

// funkcje podstawowe

// O( log( liczba podsieci ) )
net_id network_new( int growing )
{
	if(debug)
	{
		cerr << "network_new(" << growing << ")\n";
		if(growing != 0 && growing != 1)
			cerr << "ERROR: first argument of network_new needs to be 0 or 1\n";
	}
	
	int id = 1;
	if ( !get_network().empty() ) {
		// pobranie najwiekszego dotychczasowego id i zwiekszenie o 1
		network_it = get_network().end();
		network_it--;
		id = network_it->get_net_id + 1;
	}
	// wkladanie nowej sieci
	get_network()[ id ] = net( ( growing != 0 ), 0, graph() );
	if(debug)
		cerr << "network " << id << " created\n";
	return id;
}

// O( log( liczba podsieci ) )
void network_delete( net_id id )
{
	if(debug)
	{
		cerr << "network_delete(" << id << ")\n";
		if(get_network().find(id) == get_network().end())
			cerr << "network " << id << " doesn't exist, nothing to do here\n";
	}
	get_network().erase( id );
}

// O( log( liczba podsieci ) )
size_t network_nodes_number( net_id id )
{
	if(debug)
		cerr << "network_nodes_number(" << id << ")\n";
	network_it = get_net( id );
	if ( network_it == get_network().end() ) {
		if(debug)
			cerr << "network " << id << " doesn't exist, returning 0\n";
		return 0;
	}
	return net_graph( network_it->get_net )->size();
}

// O( log( liczba podsieci ) )
size_t network_links_number(net_id id)
{
	if(debug)
		cerr << "network_links_number(" << id << ")\n";
	network_it = get_net( id );
	if ( network_it == get_network().end() ) {
		if(debug)
			cerr << "network " << id << " doesn't exist, returning 0\n";
		return 0;
	}
	return net_links_number( network_it->get_net );
}


// O( log( liczba podsieci ) + log( rozmiar podsieci o id ) )
void network_add_node(net_id id, const char* label)
{
	if(debug)
		cerr << "network_add_node(" << id << ", " << label << ")\n";
	network_it = get_net( id );
	if ( network_it == get_network().end() ) {
		if(debug)
			cerr << "network doesn't exist, nothing to do here\n";
		return;
	}
	if ( label == NULL ) {
		if(debug)
			cerr << "label is empty\n";
		return;
	}
	graph* g = net_graph( network_it->get_net );
	// operator [] doda node do grafu jesli nie istnial
	(*g)[ label ];
}

// O( log( liczba podsieci ) + log( rozmiar podsieci o id ) )
void network_add_link(net_id id, const char* slabel, const char* tlabel)
{
	if(debug)
		cerr << "network_add_link(" << id << ", " << slabel << ", " << tlabel << ")\n";
	if ( slabel == NULL || tlabel == NULL ) {
		if(debug) {
			if(slabel == NULL)
				cerr << "slabel is null, nothing to do here\n";
			if(tlabel == NULL)
				cerr << "tlabel is null, nothing to do here\n";
		}
		return;
	}
	network_it = get_net( id );
	if ( network_it == get_network().end() ) {
		if(debug)
			cerr << "network " << id << " doesn't exist, nothing to do here\n";
		return;
	}
	graph* g = net_graph( network_it->get_net );
	// jesli nie istnial wierzcholek o etykiecie slabel to operator [] tworzy go
	nodes* out = &( (*g)[ slabel ].get_out_links );
	// sprawdzamy czy krawedz istniala wczesniej
	if ( out->find( tlabel ) != out->end() ) {
		if(debug)
			cerr << "link " << slabel << " -> " << tlabel << " in network " << id << "already exists, nothing to do here\n";
		return;
	}
	net_links_number( network_it->get_net )++;
	nodes* in = &( (*g)[ tlabel ].get_in_links );
	out->insert( tlabel );
	in->insert( slabel );
}

// O ( log( liczba podsieci ) + l log n ), gdzie
// n = rozmiar podsieci o id,
// l = ilosc krawedzi wchodzacych i wychodzacych do wierzcholka o label
void network_remove_node(net_id id, const char* label)
{
	if(debug)
		cerr << "network_remove_node(" << id << ", " << label << ")\n";
	// O ( log( liczba podsieci ) )
	network_it = get_net( id );
	if ( network_it == get_network().end() ) {
		if(debug)
			cerr << "network " << id << " doesn't exist, nothing to do here\n";
		return;
	}
	net* nt = &network_it->get_net;
	if ( is_growing( *nt ) ) {
		if(debug)
			cerr << id << " is growing, can't remove node\n";
		return;
	}
	graph* g = net_graph( *nt );
	graph::iterator graph_it_s, graph_it_t;
	// O ( log n ), gdzie n = rozmiar podsieci o id
	// szukamy w grafie wierzcholka o label
	graph_it_s = g->find( label );
	if ( graph_it_s == g->end() ) {
		if(debug)
			cerr << "node " << label << "doesn't exist in network " << id << ", nothing to do here\n";
		return;
	}
	nodes* out = &( graph_it_s->get_node_links.get_out_links );
	nodes* in =  &( graph_it_s->get_node_links.get_in_links );
	// usuwamy polaczenia wychodzace z usuwanego wierzcholka -> (musimy
	// zaktualizowac polaczenia wchodzace w sasiadach wierzcholka o label)
	while ( !out->empty() ) {
		node_label t_label = *(out->begin());
		graph_it_t = g->find( t_label );
		remove_link_between_nodes( nt, graph_it_s, graph_it_t );
	}
	// oraz usuwamy polaczenia odwrotne: wchodzace do wierzcholka o label
	graph_it_t = graph_it_s;
	while ( !in->empty() ) {
		node_label s_label = *(in->begin());
		graph_it_s = g->find( s_label );
		//TODO usunac assert
		assert( graph_it_s != g->end() );
		remove_link_between_nodes( nt, graph_it_s, graph_it_t );
	}
	// po usunieciu polaczen usuwamy wierzcholek
	g->erase( graph_it_t );
}


void network_remove_link(net_id id, const char* slabel, const char* tlabel)
{
	if(debug)
		cerr << "network_remove_link(" << id << ", " << slabel << ", " << tlabel << ")\n";
	network_it = get_net( id );
	if ( network_it == get_network().end() ) {
		if(debug)
			cerr << "network " << id << " doesn't exist, nothing to do here\n";
		return;
	}
	net* nt = &network_it->get_net;
	if ( is_growing( *nt ) ) {
		if(debug)
			cerr << "network " << id << " is growing, can't remove link, returning\n";
		return;
	}
	net_remove_link( nt, slabel, tlabel );
	cerr << "link " << slabel << " -> " << tlabel << " in network " << id << " removed\n";
}

// O( log( liczba podsieci ) )
void network_clear(net_id id)
{
	network_it = get_net( id );
	if ( network_it == get_network().end() ) {
		return;
	}
	if ( is_growing( network_it->get_net ) ) {
		return;
	}
	network_it->get_net = net( 0, 0, graph() );
}

// O( log( liczba podsieci ) + log( rozmiar podsieci o id ) )
size_t network_out_degree(net_id id, const char* label)
{
	if ( label == NULL ) {
		return 0;
	}
	network_it = get_net( id );
	if ( network_it == get_network().end() ) {
		return 0;
	}
	graph* g = net_graph( network_it->get_net );
	graph::iterator graph_it_s, graph_it_t;
	graph_it_s = g->find( label );
	if ( graph_it_s == g->end() ) {
		return 0;
	}
	nodes* out = &( graph_it_s->get_node_links.get_out_links );
	return out->size();
}

// O( log( liczba podsieci ) + log( rozmiar podsieci o id ) )
size_t network_in_degree(net_id id, const char* label)
{
	if ( label == NULL ) {
		return 0;
	}
	network_it = get_net( id );
	if ( network_it == get_network().end() ) {
		return 0;
	}
	graph* g = net_graph( network_it->get_net );
	graph::iterator graph_it_s, graph_it_t;
	graph_it_s = g->find( label );
	if ( graph_it_s == g->end() ) {
		return 0;
	}
	nodes* in = &( graph_it_s->get_node_links.get_in_links );
	return in->size();
}

// wypisuje caly network - TODO usunac przed wyslaniem
void showtime()
{
	cout << "========== WHOLE NETWORK: =============" << endl;
	for( network_it = get_network().begin(); network_it != get_network().end(); network_it++ ) {
		cout << "net id: " << network_it->get_net_id << endl;
		net* nt = &( network_it->get_net );
		cout << "\t is growing: " << is_growing( *nt ) << endl;
		cout << "\t links number: " << net_links_number( *nt ) << endl;
		graph* g = net_graph( *nt );
		cout << "\t nodes number: " << g->size() << endl;
		cout << "\t nodes list: " << endl;

		graph::iterator graph_it_s;
		unordered_set <node_label>::iterator nodes_it;
		for ( graph_it_s = g->begin(); graph_it_s != g->end(); graph_it_s++ ) {
			cout << "\t\t " << graph_it_s->get_node_label << ": " << endl;
			nodes* out = &( graph_it_s->get_node_links.get_out_links );
			nodes* in = &( graph_it_s->get_node_links.get_in_links );
			cout << "\t\t\t out: ";
			for ( nodes_it = out->begin(); nodes_it != out->end(); nodes_it++ ) {
				cout << (*nodes_it) << " ";
			}
			cout << endl << "\t\t\t in: ";
			for ( nodes_it = in->begin(); nodes_it != in->end(); nodes_it++ ) {
				cout << (*nodes_it) << " ";
			}
			cout << endl;
		}
		cout << "-------------------------------------" << endl;
	}

}
