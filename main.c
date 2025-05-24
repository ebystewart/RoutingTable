#include "routing.h"
#include <stdio.h>
#include <string.h>


int main(int argc, char **argv)
{
    mtrie_node_t *root_node;
    char dst_ip[16];
    uint16_t dst_ip_mask;
    char next_hop_ip[16];
    char arg;
    if(argc <= 5U)
    {
        if (strcmp(argv[1],"-a") == 0U)
        {
            printf("%s: Add the route %s with mask %u for %s\n", __FUNCTION__, argv[2], atoi(argv[3]), argv[4]);
            strcpy(dst_ip, argv[2]);
            strcpy(next_hop_ip, argv[4]);
            dst_ip_mask = atoi(argv[3]);// - '0';
            root_node = init_routing_table();
            route_insert(root_node, dst_ip, dst_ip_mask, next_hop_ip);
            dump_routing_table(root_node);
        }
    }
    while(1)
    {
        arg = getchar();
        if(arg == '-'){

        }
    }
    return 0;
}