#include <assert.h>
#include <stdio.h>
#include "../network.h"
//#include "growingnet.h"

int main()
{
    unsigned long n;

	// TESTOWANIE JEDNEJ PODSIECI W CALEJ SIECI:
    n = network_new(0);

    network_add_link(n, "Kabaty", "Natolin");
    assert(network_nodes_number(n) == 2);
    assert(network_links_number(n) == 1);
    assert(network_out_degree(n, "Kabaty") == 1);
    assert(network_in_degree(n, "Kabaty") == 0);
    assert(network_out_degree(n, "Natolin") == 0);
    assert(network_in_degree(n, "Natolin") == 1);

    network_add_link(n, "Natolin", "Imielin");
    assert(network_nodes_number(n) == 3);
    assert(network_links_number(n) == 2);
    assert(network_out_degree(n, "Natolin") == 1);
    assert(network_in_degree(n, "Natolin") == 1);
    assert(network_out_degree(n, "Imielin") == 0);
    assert(network_in_degree(n, "Imielin") == 1);

    network_add_link(n, "Imielin", "Stokłosy");
    assert(network_nodes_number(n) == 4);
    assert(network_links_number(n) == 3);
    assert(network_out_degree(n, "Imielin") == 1);
    assert(network_in_degree(n, "Imielin") == 1);
    assert(network_out_degree(n, "Stokłosy") == 0);
    assert(network_in_degree(n, "Stokłosy") == 1);

    network_remove_link(n, "Kabaty", "Natolin");
    assert(network_nodes_number(n) == 4);
    assert(network_links_number(n) == 2);
    assert(network_out_degree(n, "Kabaty") == 0);
    assert(network_in_degree(n, "Kabaty") == 0);
    assert(network_out_degree(n, "Natolin") == 1);
    assert(network_in_degree(n, "Natolin") == 0);

    network_remove_node(n, "Imielin");
    assert(network_nodes_number(n) == 3);
    assert(network_links_number(n) == 0);
    assert(network_out_degree(n, "Kabaty") == 0);
    assert(network_in_degree(n, "Kabaty") == 0);
    assert(network_out_degree(n, "Natolin") == 0);
    assert(network_in_degree(n, "Natolin") == 0);
    assert(network_out_degree(n, "Stokłosy") == 0);
    assert(network_in_degree(n, "Stokłosy") == 0);

    network_add_node(n, "Imielin");
    assert(network_nodes_number(n) == 4);
    assert(network_links_number(n) == 0);
    assert(network_out_degree(n, "Imielin") == 0);
    assert(network_in_degree(n, "Imielin") == 0);

    network_add_link(n, "Kabaty", "Kabaty");
    assert(network_nodes_number(n) == 4);
    assert(network_links_number(n) == 1);
    assert(network_out_degree(n, "Kabaty") == 1);
    assert(network_in_degree(n, "Kabaty") == 1);


	//dodanie drugiej sieci
  	unsigned long m;
	m = network_new(1);

	network_add_link(m, "Kabaty", "Natolin");
    assert(network_nodes_number(m) == 2);
    assert(network_links_number(m) == 1);
    assert(network_out_degree(m, "Kabaty") == 1);
    assert(network_in_degree(m, "Kabaty") == 0);
    assert(network_out_degree(m, "Natolin") == 0);
    assert(network_in_degree(m, "Natolin") == 1);

    network_add_link(m, "Natolin", "Imielin");
    assert(network_nodes_number(m) == 3);
    assert(network_links_number(m) == 2);
    assert(network_out_degree(m, "Natolin") == 1);
    assert(network_in_degree(m, "Natolin") == 1);
    assert(network_out_degree(m, "Imielin") == 0);
    assert(network_in_degree(m, "Imielin") == 1);

    network_add_link(m, "Imielin", "Stokłosy");
    assert(network_nodes_number(m) == 4);
    assert(network_links_number(m) == 3);
    assert(network_out_degree(m, "Imielin") == 1);
    assert(network_in_degree(m, "Imielin") == 1);
    assert(network_out_degree(m, "Stokłosy") == 0);
    assert(network_in_degree(m, "Stokłosy") == 1);

    network_remove_link(m, "Kabaty", "Natolin");
    assert(network_nodes_number(m) == 4);
    assert(network_links_number(m) == 3);
    assert(network_out_degree(m, "Imielin") == 1);
    assert(network_in_degree(m, "Imielin") == 1);
    assert(network_out_degree(m, "Stokłosy") == 0);
    assert(network_in_degree(m, "Stokłosy") == 1);

    network_remove_node(m, "Imielin");
    assert(network_nodes_number(m) == 4);
    assert(network_links_number(m) == 3);
    assert(network_out_degree(m, "Kabaty") == 1);
    assert(network_in_degree(m, "Kabaty") == 0);
    assert(network_out_degree(m, "Natolin") == 1);
    assert(network_in_degree(m, "Natolin") == 1);
    assert(network_out_degree(m, "Stokłosy") == 0);
    assert(network_in_degree(m, "Stokłosy") == 1);

    network_add_node(m, "Imielin");
    assert(network_nodes_number(m) == 4);
    assert(network_links_number(m) == 3);
    assert(network_out_degree(m, "Imielin") == 1);
    assert(network_in_degree(m, "Imielin") == 1);

    network_add_link(m, "Kabaty", "Kabaty");
    assert(network_nodes_number(m) == 4);
    assert(network_links_number(m) == 4);
    assert(network_out_degree(m, "Kabaty") == 2);
    assert(network_in_degree(m, "Kabaty") == 1);


	//sprawdzanie wyczyszczonej
    network_clear(n);
    assert(network_nodes_number(n) == 0);
    assert(network_links_number(n) == 0);
    assert(network_out_degree(n, "Kabaty") == 0);
    assert(network_in_degree(n, "Kabaty") == 0);

	network_add_link(n, "Kabaty", "Kabaty");
	network_add_link(n, "Kabaty", "Kabaty");
	network_add_link(n, "Kabaty", "Kabaty");
	network_add_node(n, "Mlociny");
	network_add_node(n, "Mlociny");
	network_add_node(n, "Mlociny");
    assert(network_nodes_number(n) == 2);
    assert(network_links_number(n) == 1);
    assert(network_out_degree(n, "Kabaty") == 1);
    assert(network_in_degree(n, "Kabaty") == 1);

    network_add_link(n, "Kabaty", "Imielin");
    network_add_link(n, "Imielin", "Stoklosy");
    assert(network_nodes_number(n) == 4);
    assert(network_links_number(n) == 3);
    assert(network_out_degree(n, "Imielin") == 1);
    assert(network_in_degree(n, "Imielin") == 1);

    network_add_node(n, "Imielin");
    assert(network_out_degree(n, "Imielin") == 1);
    assert(network_in_degree(n, "Imielin") == 1);

    network_add_link(n, "Mlociny", "Wawrzyszew");
    network_add_link(n, "Wawrzyszew", "Imielin");
    network_add_link(n, "Imielin", "Natolin");

    assert(network_out_degree(n, "Imielin") == 2);
    assert(network_in_degree(n, "Imielin") == 2);

    network_add_node(n, "Imielin");
    assert(network_out_degree(n, "Imielin") == 2);
    assert(network_in_degree(n, "Imielin") == 2);

    network_remove_link(n, "Imielin", "Imielin");
    assert(network_out_degree(n, "Imielin") == 2);
    assert(network_in_degree(n, "Imielin") == 2);

	network_remove_link(n, "Imielin", "Imielin");
	assert(network_out_degree(n, "Imielin") == 2);
    assert(network_in_degree(n, "Imielin") == 2);

    network_remove_link(n, "Imielin", "Kabaty");
	assert(network_out_degree(n, "Imielin") == 2);
    assert(network_in_degree(n, "Imielin") == 2);

    network_add_link(n, "Imielin", "Imielin");
    assert(network_out_degree(n, "Imielin") == 3);
    assert(network_in_degree(n, "Imielin") == 3);
    network_remove_link(n, "Imielin", "Imielin");
    assert(network_out_degree(n, "Imielin") == 2);
    assert(network_in_degree(n, "Imielin") == 2);

    network_remove_link(n, "Kabaty", "Imielin");
	assert(network_out_degree(n, "Imielin") == 2);
    assert(network_in_degree(n, "Imielin") == 1);

	network_remove_node(n, "Imielin");
	assert(network_nodes_number(n) == 5);
    assert(network_links_number(n) == 2);
    assert(network_out_degree(n, "Natolin") == 0);
    assert(network_in_degree(n, "Imielin") == 0);
    assert(network_out_degree(n, "Wawrzyszew") == 0);
    assert(network_in_degree(n, "Wawrzyszew") == 1);
    assert(network_out_degree(n, "Stoklosy") == 0);
    assert(network_in_degree(n, "Stoklosy") == 0);
    assert(network_out_degree(n, "Mlociny") == 1);
    assert(network_in_degree(n, "Mlociny") == 0);
    assert(network_out_degree(n, "Kabaty") == 1);
    assert(network_in_degree(n, "Kabaty") == 1);
    assert(network_out_degree(n, "Pikaczu") == 0);
    assert(network_in_degree(n, "Pikaczu") == 0);
	assert(network_nodes_number(n) == 5);
    assert(network_links_number(n) == 2);


	//sprawdzanie usunietej
	network_delete(n);
	assert(network_out_degree(n, "Imielin") == 0);
    assert(network_in_degree(n, "Imielin") == 0);
    assert(network_nodes_number(n) == 0);
    assert(network_links_number(n) == 0);

	network_add_link(n, "Kabaty", "Kabaty");
	network_add_link(n, "Kabaty", "Imielin");
    network_add_link(n, "Imielin", "Stoklosy");
    network_add_link(n, "Imielin", "Imielin");
	network_add_node(n, "Mlociny");
    assert(network_nodes_number(n) == 0);
    assert(network_links_number(n) == 0);
	assert(network_out_degree(n, "Imielin") == 0);
    assert(network_in_degree(n, "Imielin") == 0);

	network_remove_link(n, "Kabaty", "Imielin");
	network_remove_link(n, "Kabaty", "Kabaty");
	network_remove_node(n, "Mlociny");
	assert(network_nodes_number(n) == 0);
    assert(network_links_number(n) == 0);

    //druga sieć

	return 0;
}
