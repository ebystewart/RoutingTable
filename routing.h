#ifndef __ROUTING_H__
#define __ROUTING_H__

#include <stdint.h>

typedef enum bit_type_{
    ZERO,
    ONE,
    DONT_CARE,
    BIT_TYPE_MAX
}bit_type_t;

typedef uint32_t bitmap_t;

typedef struct mtrie_node_{
    bitmap_t prefix;
    bitmap_t wildcard;
    uint16_t prefix_len;
    struct mtrie_node_ *parent;
    struct mtrie_node_ *child[BIT_TYPE_MAX];
    void *data;
}mtrie_node_t;


void bitmap_set_bit(bitmap_t *bm, int index);

void calculate_effective_prefix(char *ip, uint16_t subnet_mask);

void init_routing_table(mtrie_node_t *root_node);

void route_insert(mtrie_node_t *root_node, unsigned int ip_addr, uint16_t subnet_mask);

unsigned int route_search_exactmatch(mtrie_node_t *root_node, unsigned int ip_addr, uint16_t subnet_mask);

unsigned int route_lookup_lpm(mtrie_node_t *root_node, unsigned int ip_addr);

void route_delete(mtrie_node_t *root_node, unsigned int ip_addr, uint16_t subnet_mask);

#endif