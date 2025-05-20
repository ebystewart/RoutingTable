#include "routing.h"
#include <stdint.h>
#include <stdio.h>


mtrie_node_t root_node;

void bitmap_set_bit(bitmap_t *bm, int index)
{
    *bm = *bm | (1U << index);
}

void calculate_effective_prefix(char *ip, uint16_t subnet_mask)
{

}

void init_routing_table(mtrie_node_t *root_node)
{
    root_node->prefix = 0U;
    root_node->prefix_len = 0U;
    root_node->wildcard = 0U;
    root_node->parent = NULL;
    root_node->child[ZERO] = NULL;
    root_node->child[ONE] = NULL;
    root_node->child[DONT_CARE] = NULL;
    root_node->data = NULL;
}

void route_insert(mtrie_node_t *root_node, unsigned int ip_addr, uint16_t subnet_mask)
{
    
}

unsigned int route_search_exactmatch(mtrie_node_t *root_node, unsigned int ip_addr, uint16_t subnet_mask)
{
    unsigned int next_hop_ip;

    return next_hop_ip;
}

unsigned int route_lookup_lpm(mtrie_node_t *root_node, unsigned int ip_addr)
{
    unsigned int next_hop_ip;

    return next_hop_ip;
}

void route_delete(mtrie_node_t *root_node, unsigned int ip_addr, uint16_t subnet_mask)
{

}
