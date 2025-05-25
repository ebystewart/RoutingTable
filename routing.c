#include "routing.h"
#include <stdint.h>
#include <stdio.h>
#include <stdbool.h>
#include <stdlib.h>
#include <string.h>
#include <arpa/inet.h>


void bitmap_set_bit(uint32_t *bm, uint32_t index)
{
    *bm = *bm | (1U << index);
}

void bitmap_clr_bit(uint32_t *bm, uint32_t index)
{
    *bm = *bm & (~(1U << index));
}

void bitmap_set_bits(uint32_t *bm, uint32_t start, uint32_t end)
{
    uint32_t index;
    for(index = start; index <= end; index++)
    {
        *bm = *bm | (1U << index);
    }
}

void bitmap_clr_bits(uint32_t *bm, uint32_t start, uint32_t end)
{
    uint32_t index;
    for(index = start; index < end; index++)
    {
        *bm = *bm & (~(1U << index));
    }
}

// now implement assuming value is always 32 bir=t wide, will have to update the 
// impl based on varying prefix lengths 
bit_type_t get_msb(uint32_t effective_prefix, uint16_t prefix_len)
{
    bit_type_t retval = BIT_TYPE_MAX;

    if(prefix_len <= 32U){
        if((effective_prefix >> (prefix_len -1U)) == 0){
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

uint16_t match_effective_prefix(uint32_t dst_ip_int, uint32_t effective_prefix, uint16_t prefix_len)
{
    uint16_t match_len = 0U;
    uint16_t l_count = 31U;
    uint16_t r_count = prefix_len;
    bool l_value;
    bool r_value;

    do{
        l_value = (dst_ip_int >> l_count) & 1U;
        r_value = (effective_prefix >> r_count) & 1U;
        l_count--;
        r_count--;
        match_len++;
    }
    while((l_value == r_value) && (l_count > 0U) && (r_count > 0U));

    return match_len;
}

void split_mtrie_node(mtrie_node_t *current_node, uint16_t match_len, uint32_t dst_ip, uint16_t dst_ip_mask, char *next_hop_ip, \
     mtrie_node_t *child_node1, mtrie_node_t *child_node2)
{
    uint32_t effective_prefix_c;
    uint32_t effective_prefix_c1;
    uint32_t effective_prefix_c2;
    /* for ex. dst_ip_mask is 32 and match_len is 16 s*/
    effective_prefix_c = (current_node->prefix >> (dst_ip_mask - match_len));
    effective_prefix_c1 = current_node->prefix;
    bitmap_clr_bits(&effective_prefix_c1, (dst_ip_mask - match_len), dst_ip_mask);
    effective_prefix_c2 = dst_ip;
    bitmap_clr_bits(&effective_prefix_c2, (dst_ip_mask - match_len), dst_ip_mask);

    /* child 1 */
    bit_type_t child1 = get_msb(effective_prefix_c1, (dst_ip_mask - match_len));
    current_node->child[child1] = child_node1;
    child_node1->parent = current_node;
    child_node1->prefix = effective_prefix_c1;
    child_node1->prefix_len = dst_ip_mask - match_len;
    bitmap_clr_bits(&child_node1->wildcard, (32U - child_node1->prefix_len), 31U);
    strcpy((char *)&child_node1->data, current_node->data);

    /* update current node */
    current_node->data = NULL;
    current_node->prefix = effective_prefix_c;
    current_node->prefix_len = match_len;
    bitmap_clr_bits(&current_node->wildcard, (32U - current_node->prefix_len), 31U);

    /* child 2 */
    bit_type_t child2 = get_msb(effective_prefix_c2, (dst_ip_mask - match_len));
    current_node->child[child2] = child_node2;
    child_node2->parent = current_node;
    child_node2->prefix = effective_prefix_c2;
    child_node2->prefix_len = dst_ip_mask - match_len;
    bitmap_clr_bits(&child_node2->wildcard, (32U - child_node2->prefix_len), 31U);
    strcpy((char *)&child_node2->data, next_hop_ip);
}

mtrie_node_t *init_routing_table(void)
{
    mtrie_node_t *root_node = (mtrie_node_t *)calloc(1, sizeof(mtrie_node_t));
    root_node->prefix = 0U;
    root_node->prefix_len = 0U;
    root_node->wildcard = 0U;
    root_node->parent = NULL;
    root_node->child[ZERO] = NULL;
    root_node->child[ONE] = NULL;
    root_node->child[DONT_CARE] = NULL;
    root_node->data = NULL;
    printf("%s: Root Node created @ %x\n", __FUNCTION__, root_node);
    return root_node;
}

void route_insert(mtrie_node_t *root_node, char *dest_ip_addr, uint16_t subnet_mask, char *next_hop_ip)
{
    mtrie_node_t *curr_node;

    /* convert the destination IP from presentation format to network format */
    uint32_t dst_ip_int;
    inet_pton(AF_INET, dest_ip_addr, &dst_ip_int);
    printf("%s: Network form of IP is %x\n", __FUNCTION__, dst_ip_int);
    dst_ip_int = htonl(dst_ip_int);
    printf("%s: Host to network byte order changed IP is %x\n", __FUNCTION__, dst_ip_int);


    /* now check the MSB of the dst ip */
    bit_type_t child_position;
    printf("%s: subnet mask is %u\n", __FUNCTION__, subnet_mask);
    child_position = get_msb(dst_ip_int, subnet_mask);
    printf("%s: New node position is %u\n", __FUNCTION__, child_position);
    /* check if the msb corresponding child branch exist 
       If yes, check if there is a match */
    if(root_node->child[child_position] != NULL){
        printf("%s: Child of root node exist\n", __FUNCTION__);
        curr_node = root_node->child[child_position];
        uint16_t match_length = match_effective_prefix(dst_ip_int, curr_node->prefix, curr_node->prefix_len);
        if(match_length == 0U)
        {
            printf("%s: Undefined state\n", __FUNCTION__);
        }
        else{
            printf("%s: New route is to be inserted\n",__FUNCTION__);
            mtrie_node_t *child_node1 = (mtrie_node_t *)calloc(1, sizeof(mtrie_node_t));
            mtrie_node_t *child_node2 = (mtrie_node_t *)calloc(1, sizeof(mtrie_node_t));
            split_mtrie_node(curr_node, match_length, dst_ip_int, subnet_mask, next_hop_ip, child_node1, child_node2);
            printf("%s: Route Insertion Complete\n", __FUNCTION__);
        }
    }
    else{
        printf("%s: No child exists to root node, create one\n", __FUNCTION__);
        printf("%s: New node position is %u\n", __FUNCTION__, child_position);
        curr_node = (mtrie_node_t *)calloc(1, sizeof(mtrie_node_t));
        printf("%s: child node created @ %x\n", __FUNCTION__, curr_node);
        /* link to root node */
        root_node->child[child_position] = curr_node;
        curr_node->parent = root_node;
        /* set other parameters */
        curr_node->prefix = dst_ip_int;
        curr_node->prefix_len = subnet_mask;
        bitmap_clr_bits(&curr_node->wildcard, (32U - subnet_mask), 31U);
        strcpy((char *)&curr_node->data, next_hop_ip);
        printf("%s: New child inserted at root node\n", __FUNCTION__);
    }
}

unsigned int route_search_exactmatch(mtrie_node_t *root_node, uint32_t ip_addr, uint16_t subnet_mask)
{
    unsigned int next_hop_ip;

    return next_hop_ip;
}

unsigned int route_lookup_lpm(mtrie_node_t *root_node, uint32_t r_addr, uint16_t subnet_mask)
{
    unsigned int next_hop_ip;
    mtrie_node_t *curr_node;

    /* convert the destination IP from presentation format to network format */
    uint32_t dst_ip_int;
    inet_pton(AF_INET, r_addr, &dst_ip_int);
    printf("%s: Network form of IP is %x\n", __FUNCTION__, dst_ip_int);
    dst_ip_int = htonl(dst_ip_int);
    printf("%s: Host to network byte order changed IP is %x\n", __FUNCTION__, dst_ip_int);


    /* now check the MSB of the dst ip */
    bit_type_t child_position;
    printf("%s: subnet mask is %u\n", __FUNCTION__, subnet_mask);
    child_position = get_msb(dst_ip_int, subnet_mask);
    printf("%s: New node position is %u\n", __FUNCTION__, child_position);

    /* check if the msb corresponding child branch exist 
       If yes, check if there is a match */
    if(root_node->child[child_position] != NULL){
        curr_node = root_node->child[child_position];
        while(curr_node != NULL)
        {
            printf("%s: Child of root node exist\n", __FUNCTION__);
            uint16_t match_length = match_effective_prefix(dst_ip_int, curr_node->prefix, curr_node->prefix_len);
            if (match_length == 0U)
            {
                printf("%s: No match \n", __FUNCTION__);
                return 0;
            }
            else /* match length between 1 and 32 */
            {
                printf("%s: New route is to be inserted\n", __FUNCTION__);
                //child_position = get_msb(dst_ip_int, subnet_mask);
                printf("%s: Route Insertion Complete\n", __FUNCTION__);
            }
            curr_node = curr_node->child[child_position];
        }
    }
    return next_hop_ip;
}

void route_delete(mtrie_node_t *root_node, uint32_t ip_addr, uint16_t subnet_mask)
{

}

void dump_routing_table(mtrie_node_t *root_node)
{
    mtrie_node_t *curr_node;
    printf("\t%s: Printing routing table\n", __FUNCTION__);
    if(root_node->child[ONE] != NULL)
    {
        printf("\t%s: Printing ONE branch\n", __FUNCTION__);
        curr_node = root_node->child[ONE];
        printf("\t%s: Route prefix is %x\n", __FUNCTION__, curr_node->prefix);
        printf("\t%s: Route prefix length is %u\n", __FUNCTION__, curr_node->prefix_len);
        printf("\t%s: Route wildcard is %x\n", __FUNCTION__, curr_node->wildcard);
        printf("\t%s: Next hop IP is %s\n", __FUNCTION__, (char *)&curr_node->data);
    }
    if(root_node->child[ZERO] != NULL)
    {
        printf("\t%s: Printing ZERO branch @ %x\n", __FUNCTION__, root_node->child[ZERO]);
        curr_node = root_node->child[ZERO];
        printf("\t%s: Route prefix is %x\n", __FUNCTION__, curr_node->prefix);
        printf("\t%s: Route prefix length is %u\n",  __FUNCTION__, curr_node->prefix_len);
        printf("\t%s: Route wildcard is %x\n",  __FUNCTION__, curr_node->wildcard);
        printf("\t%s: Next hop IP is %s\n",  __FUNCTION__, (char *)&curr_node->data);
    }
    if(root_node->child[DONT_CARE] != NULL)
    {
        printf("\t%s: Printing DONT_CARE branch\n", __FUNCTION__);
        curr_node = root_node->child[DONT_CARE];
        printf("\t%s: Route prefix is %x\n", __FUNCTION__, curr_node->prefix);
        printf("\t%s: Route prefix length is %u\n", __FUNCTION__, curr_node->prefix_len);
        printf("\t%s: Route wildcard is %x\n", __FUNCTION__, curr_node->wildcard);
        printf("\t%s: Next hop IP is %s\n", __FUNCTION__, (char *)&curr_node->data);
    }
}