#include "routing.h"
#include <stdint.h>
#include <stdio.h>
#include <arpa/inet.h>


static mtrie_node_t root_node;

void bitmap_set_bit(uint32_t *bm, int index)
{
    *bm = *bm | (1U << index);
}

// now implement assuming value is always 32 bir=t wide, will have to update the 
// impl based on varying prefix lengths 
bit_type_t get_msb(uint32_t effective_prefix, uint16_t prefix_len)
{
    bit_type_t retval = BIT_TYPE_MAX;

    if(prefix_len < 32U){
        if((effective_prefix >> prefix_len) == 0){
            retval = ZERO;
        }
        else{
            retval = ONE;
        }
    }
    else if(prefix_len == 0U){
        /* If mask is 32, the effective prefix would be xxxx.xxxx.xxxx.xxxx 
           This corresponds to default route of 0.0.0.0/32 */
        retval = DONT_CARE;
    }else{
        /* invalid mask */
    }
    return retval;
}


/* if IP is 127.1.1.0 and subnet mask is 24 (i.e 255.255.255.0)
   wildcard is 0.0.0.255 and effective prefix is IP & wildcard -> 127.1.1.xx/24*/
uint32_t calculate_effective_prefix(uint32_t ip, uint16_t subnet_mask)
{
    uint32_t effective_prefix;


    return effective_prefix;
}

uint16_t match_effective_prefix(uint32_t dst_ip_int, uint32_t effective_prefix)
{

}

void split_mtrie_node(mtrie_node_t *current_node, mtrie_node_t *child_node1, mtrie_node_t *child_node2)
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

void route_insert(mtrie_node_t *root_node, char *dest_ip_addr, uint16_t subnet_mask, char *next_hop_ip)
{
    /* convert the destination IP from presentation format to network format */
    uint32_t dst_ip_int;
    inet_pton(AF_INET, dest_ip_addr, &dst_ip_int);
    dst_ip_int = htonl(dst_ip_int);

    /* now check the MSB of the dst ip */
    bit_type_t child_position;
    child_position = get_msb(dst_ip_int, subnet_mask);

    /* check if the msb corresponding child branch exist 
       If yes, check if there is a match */
    if(root_node->child[child_position] != NULL){

    }

}

unsigned int route_search_exactmatch(mtrie_node_t *root_node, uint32_t ip_addr, uint16_t subnet_mask)
{
    unsigned int next_hop_ip;

    return next_hop_ip;
}

unsigned int route_lookup_lpm(mtrie_node_t *root_node, uint32_t p_addr)
{
    unsigned int next_hop_ip;

    return next_hop_ip;
}

void route_delete(mtrie_node_t *root_node, uint32_t ip_addr, uint16_t subnet_mask)
{

}
