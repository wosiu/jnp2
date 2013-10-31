//Test na zle dane
#include <assert.h>
#include <stdio.h>
#include <string>
#include "network.h"
#include "growingnet.h"

int main()
{
		int growingnet = network_new(1);
		
		network_add_link(-1, "0", "0");
		assert(network_nodes_number(growingnet) == 0);
		assert(network_links_number(growingnet) == 0);
		assert(network_out_degree(growingnet, "0") == 0);
		assert(network_in_degree(growingnet, "0") == 0);
		
		network_add_link(growingnet, "0", "0");
		assert(network_nodes_number(growingnet) == 1);
		assert(network_links_number(growingnet) == 1);
		assert(network_out_degree(growingnet, "0") == 1);
		assert(network_in_degree(growingnet, "0") == 1);
		
		network_add_link(growingnet, NULL, "0");
		assert(network_nodes_number(growingnet) == 1);
		assert(network_links_number(growingnet) == 1);
		assert(network_out_degree(growingnet, "0") == 1);
		assert(network_in_degree(growingnet, "0") == 1);
		
		network_add_link(growingnet, "0", NULL);
		assert(network_nodes_number(growingnet) == 1);
		assert(network_links_number(growingnet) == 1);
		assert(network_out_degree(growingnet, "0") == 1);
		assert(network_in_degree(growingnet, "0") == 1);
		
		network_add_link(growingnet, NULL, NULL);
		assert(network_nodes_number(growingnet) == 1);
		assert(network_links_number(growingnet) == 1);
		assert(network_out_degree(growingnet, "0") == 1);
		assert(network_in_degree(growingnet, "0") == 1);
		
		network_remove_link(growingnet, "0", "0");
		assert(network_nodes_number(growingnet) == 1);
		assert(network_links_number(growingnet) == 1);
		assert(network_out_degree(growingnet, "0") == 1);
		assert(network_in_degree(growingnet, "0") == 1);
		
		network_remove_link(growingnet, "1", "1");
		assert(network_nodes_number(growingnet) == 1);
		assert(network_links_number(growingnet) == 1);
		assert(network_out_degree(growingnet, "0") == 1);
		assert(network_in_degree(growingnet, "0") == 1);
		
		network_remove_link(growingnet, NULL, "0");
		assert(network_nodes_number(growingnet) == 1);
		assert(network_links_number(growingnet) == 1);
		assert(network_out_degree(growingnet, "0") == 1);
		assert(network_in_degree(growingnet, "0") == 1);
		
		network_remove_link(growingnet, "0", NULL);
		assert(network_nodes_number(growingnet) == 1);
		assert(network_links_number(growingnet) == 1);
		assert(network_out_degree(growingnet, "0") == 1);
		assert(network_in_degree(growingnet, "0") == 1);
		
		network_remove_link(growingnet, NULL, NULL);
		assert(network_nodes_number(growingnet) == 1);
		assert(network_links_number(growingnet) == 1);
		assert(network_out_degree(growingnet, "0") == 1);
		assert(network_in_degree(growingnet, "0") == 1);

		network_delete(growingnet);
		assert(network_nodes_number(growingnet) == 0);
		assert(network_links_number(growingnet) == 0);
		assert(network_out_degree(growingnet, "0") == 0);
		assert(network_in_degree(growingnet, "0") == 0);
		
		growingnet = network_new(0);

		network_remove_node(growingnet, "0");
		assert(network_nodes_number(growingnet) == 0);
		assert(network_links_number(growingnet) == 0);
		assert(network_out_degree(growingnet, "0") == 0);
		assert(network_in_degree(growingnet, "0") == 0);
		
		network_remove_node(-1, "0");
		assert(network_nodes_number(growingnet) == 0);
		assert(network_links_number(growingnet) == 0);
		assert(network_out_degree(growingnet, "0") == 0);
		assert(network_in_degree(growingnet, "0") == 0);
		
		network_remove_node(growingnet, NULL);
		assert(network_nodes_number(growingnet) == 0);
		assert(network_links_number(growingnet) == 0);
		assert(network_out_degree(growingnet, "0") == 0);
		assert(network_in_degree(growingnet, "0") == 0);
		
		network_add_node(-1, "kupa");
		assert(network_nodes_number(growingnet) == 0);
		assert(network_links_number(growingnet) == 0);
		assert(network_out_degree(growingnet, "0") == 0);
		assert(network_in_degree(growingnet, "0") == 0);
		
		network_add_node(growingnet, NULL);
		assert(network_nodes_number(growingnet) == 0);
		assert(network_links_number(growingnet) == 0);
		assert(network_out_degree(growingnet, "0") == 0);
		assert(network_in_degree(growingnet, "0") == 0);
		
		network_clear(-1);
		assert(network_nodes_number(growingnet) == 0);
		assert(network_links_number(growingnet) == 0);
		assert(network_out_degree(growingnet, "0") == 0);
		assert(network_in_degree(growingnet, "0") == 0);
		
		assert(network_nodes_number(-1) == 0);
		assert(network_links_number(-1) == 0);
		assert(network_out_degree(-1, "0") == 0);
		assert(network_in_degree(-1, "0") == 0);
		
    return 0;
}
