#include "routing.h"
#include <stdio.h>
#include <string.h>

int main(int argc, char **argv)
{
    mtrie_node_t *root_node;
    char dst_ip[16];
    uint16_t dst_ip_mask;
    char next_hop_ip[16];
    char arg[30];
    char *ptr;
    if(argc <= 5U)
    {
        if (strcmp(argv[1],"-a") == 0U)
        {
            printf("%s: Add the route %s with mask %u for %s\n", __FUNCTION__, argv[2], atoi(argv[3]), argv[4]);
            strcpy(dst_ip, argv[2]);
            strncpy(next_hop_ip, argv[4], 16U);
            next_hop_ip[15] = '\0';
            dst_ip_mask = atoi(argv[3]);// - '0';
            root_node = init_routing_table();
            route_insert(root_node, dst_ip, dst_ip_mask, next_hop_ip);
        }
    }
    while(1)
    {
        char *rest = arg;
        memset(arg, '\0', sizeof(arg));
        printf("\n#router:> ");
        gets(arg);
        printf("%s: The input is %s\n", __FUNCTION__, arg);
        if(strncmp(arg,"--print", sizeof("--print")) == 0U){// To check logic
            dump_routing_table(root_node);
        }
        else if(arg[0] =='-' && arg[1]== 'l')
        {
            ptr = strtok_r(rest, " ", &rest);
            printf("%s: The arg is %s\n", __FUNCTION__, ptr);
            ptr = strtok_r(rest, " ", &rest);
            printf("%s: The arg is %s\n", __FUNCTION__, ptr);
            strcpy(dst_ip, ptr);
            ptr = strtok_r(rest, " ", &rest);
            dst_ip_mask = atoi(ptr);
            printf("%s: The arg is %s and %u\n", __FUNCTION__, ptr, dst_ip_mask);
            route_lookup_lpm(root_node, dst_ip, dst_ip_mask);
            ptr = NULL;
        }
        else if(arg[0] =='-' && arg[1]== 'i')
        {
            ptr = strtok_r(rest, " ", &rest);
            printf("%s: The arg is %s\n", __FUNCTION__, ptr);
            ptr = strtok_r(rest, " ", &rest);
            printf("%s: The arg is %s\n", __FUNCTION__, ptr);
            strcpy(dst_ip, ptr);
            ptr = strtok_r(rest, " ", &rest);
            printf("%s: The arg is %s\n", __FUNCTION__, ptr);
            dst_ip_mask = atoi(ptr);
            ptr = strtok_r(rest, " ", &rest);
            printf("%s: The arg is %s\n", __FUNCTION__, ptr);
            strcpy(next_hop_ip, ptr);
            route_insert(root_node, dst_ip, dst_ip_mask, next_hop_ip);
            ptr = NULL;
        }
        else{
            printf("Invalid argument %s\n", arg);
        }
    }
    return 0;
}