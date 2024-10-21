#ifndef POSTS_H
#define POSTS_H

#include "graf.h"
#define LG_MAX 256
/**
 * Function that handles the calling of every command from task 2
 *
 * Please add any necessary parameters to the functions
*/
typedef struct {
	char *sentence;
	char words[LG_MAX][LG_MAX];
	int nr_words;
} words_t;

void find_words(words_t *w);
int make_int_from_char(char c[]);
void handle_input_posts(char *input, int *id, linked_list_t *all_p);
post_t *find_repost(int id_repost, linked_list_t *all_p);
post_t *find_post_list(int id_post, linked_list_t *all_p);
void create_post(int *id, linked_list_t *all_p);
void tree_add_child(post_t *post, post_t *repost);
void add_repost_to_post(post_t *post_to_repost, int user_id, int *id);
void create_repost(int *id, linked_list_t *all_p);
void print_all_response(post_t *post);
void get_reposts(linked_list_t *all_p);
int h_find(post_t *post);
void find_commun_repost(linked_list_t *all_p);
void like_post(linked_list_t *all_p);
void get_likes(linked_list_t *all_p);
void check_repost_likes(post_t *post, int *max_likes, int *repost_id);
void ratio(linked_list_t *all_p);
void delete_post(linked_list_t *all_p);

#endif // POSTS_H
