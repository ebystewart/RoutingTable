#ifndef __ROUTING_H__
#define __ROUTING_H__

#include <stdint.h>


typedef enum bit_type_{
    ZERO,
    ONE,
    DONT_CARE,
    BIT_TYPE_MAX
}bit_type_t;

typedef struct mtrie_node_{
    uint32_t prefix;
    uint32_t wildcard;
    uint16_t prefix_len;
    struct mtrie_node_ *parent;
    struct mtrie_node_ *child[BIT_TYPE_MAX];
    void *data;
}mtrie_node_t;


void bitmap_set_bit(uint32_t *bm, uint32_t index);

void bitmap_clr_bit(uint32_t *bm, uint32_t index);

void bitmap_set_bits(uint32_t *bm, uint32_t start, uint32_t end);

void bitmap_clr_bits(uint32_t *bm, uint32_t start, uint32_t end);

uint32_t calculate_effective_prefix(uint32_t ip, uint16_t subnet_mask);

uint16_t match_effective_prefix(uint32_t dst_ip_int, uint32_t effective_prefix, uint16_t prefix_len);

void split_mtrie_node(mtrie_node_t *current_node, uint16_t match_len, uint32_t dst_ip, uint16_t dst_ip_mask, char *next_hop_ip, \
                    mtrie_node_t *child_node1, mtrie_node_t *child_node2);

mtrie_node_t *init_routing_table(void);

void route_insert(mtrie_node_t *root_node, char *dest_ip_addr, uint16_t subnet_mask, char *next_hop_ip);

/* if a route is found, returns next hop IP in network format; else returns 0 */
uint32_t route_search_exactmatch(mtrie_node_t *root_node, uint32_t ip_addr, uint16_t subnet_mask);

/* if a route is found, returns next hop ip in network format; else returns 0 */
uint32_t route_lookup_lpm(mtrie_node_t *root_node, uint32_t ip_addr, uint16_t subnet_mask);

void route_delete(mtrie_node_t *root_node, uint32_t ip_addr, uint16_t subnet_mask);

void dump_routing_table(mtrie_node_t *root_node);

#endif