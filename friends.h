#ifndef FRIENDS_H
#define FRIENDS_H

#include "graf.h"

#define MAX_COMMAND_LEN 500
#define MAX_PEOPLE 550

/**
 * Function that handles the calling of every command from task 1
 *
 * Please add any necessary parameters to the functions
*/
void handle_input_friends(char *input, list_graph_t *graf);
void add_friend(list_graph_t *graf, int f1, int f2);
int check_friend(list_graph_t *graf, int f1, int f2);
void bfs_list_graph(list_graph_t *lg, int start, int dest);
void friends(list_graph_t *graf, int f1);
void popular(list_graph_t *graf, int f1);
void common(list_graph_t *graf, int f1, int f2);
void suggestions(list_graph_t *graf, int f1);
void remove_friend_from_list(linked_list_t *list, int f);
void remove_friend(list_graph_t *graf, int f1, int f2);

#endif // FRIENDS_H
