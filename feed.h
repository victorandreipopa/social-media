#ifndef FEED_H
#define FEED_H

#include "graf.h"
/**
 * Function that handles the calling of every command from task 3
 *
 * Please add any necessary parameters to the functions
*/
void handle_input_feed(char *in, linked_list_t *all_post, list_graph_t *graf);
void print_all_repost(post_t *post, int id_utilizator);
void feed(linked_list_t *all_post, list_graph_t *graf);
void print_all_post(ll_node_t *curr, int  id_utilizator);
void view_profile(linked_list_t *all_p);
void print_all_friends(post_t *post, int id_p, int id_u, list_graph_t *graf);
void friends_repost(list_graph_t *graf, linked_list_t *all_p);
void common_groups(list_graph_t *graf);

#endif // FEED_H
