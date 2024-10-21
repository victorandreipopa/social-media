#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <errno.h>
#include "users.h"
#include "posts.h"
#include "graf.h"

// functie ajutatoare care imi returneaza cuvintele dintr-o comanda
void find_words(words_t *w)
{
	char delim[] =  "\n ";
	int nr = 0;
	for (char *p = strtok(NULL, delim); p; p = strtok(NULL, delim)) {
		strcpy((*w).words[nr], p);
		nr++;
	}
	w->nr_words = nr;
}

// transform numarul citit in int
int make_int_from_char(char c[])
{
	int n = strlen(c);
	char p[LG_MAX];
	int nr = 0;
	strcpy(p, c);
	for (int i = 0; i < n; i++)
		nr = nr * 10 + (c[i] - '0');
	return nr;
}

// creez o postare noua si o adaug in lista de postari si creez un nou arbore
// pentru fiecare postare
void create_post(int *id, linked_list_t *all_p) {
	char *user = strtok(NULL, "\n ");
	char *titlu = strtok(NULL, "\n");
	post_t *new_post = malloc(sizeof(post_t));
	new_post->id = (*id);
	new_post->user_id = get_user_id(user);
	new_post->nr_likes = 0;
	new_post->titlu = malloc(strlen(titlu) + 1);
	strcpy(new_post->titlu, titlu);
	new_post->repost = 0;
	new_post->parent = NULL;
	// fiecarei postari ii atribui o lista de like-uri
	new_post->likes = ll_create(sizeof(int));
	// fiecarei postari ii atribui un arbore de reposturi
	new_post->events = tree_create(sizeof(int));
	new_post->events->root = tree_node_create(id, sizeof(int));
	// fiecarui arbore ii atribui o lista de copii
	new_post->events->root->children = ll_create(sizeof(post_t));
	add_node_at_start(all_p, new_post);
	free(new_post);
	printf("Created %s for %s\n", titlu, user);
}

// fucntie care imi returneaza postarea cu un anumit id dintr-o lista
post_t *find_post_list(int id_post, linked_list_t *all_p) {
	if (!all_p->head)
		return NULL;
	ll_node_t *curr = all_p->head;
	while (curr) {
		post_t *post = (post_t *)curr->data;
		if (post->id == id_post)
			return post;
		curr = curr->next;
	}
	return NULL;
}

// functie care imi returneaza repostul cu un anumit id dintr-o lista
post_t *find_repost(int id_repost, linked_list_t *list) {
	if (!list->head)
		return NULL;
	ll_node_t *curr = list->head;
	post_t *post = NULL;
	while (curr) {
		post = (post_t *)curr->data;
		if (post->id == id_repost)
			return post;
		if (post->events && post->events->root) {
			post = find_repost(id_repost, post->events->root->children);
		if (post)
			return post;
		}
		curr = curr->next;
	}
	return NULL;
}

// fucntie care imi adauga la un nod un copil, in lista de copii
void tree_add_child(post_t *post, post_t *repost) {
	add_node_at_end(post->events->root->children, repost);
}

// fucntie care imi adauga un repost la o postare
void add_repost_to_post(post_t *post_to_repost, int user_id, int *id) {
	post_t *new_repost = malloc(sizeof(post_t));
	new_repost->id = (*id);
	new_repost->user_id = user_id;
	new_repost->nr_likes = 0;
	new_repost->titlu = malloc(strlen(post_to_repost->titlu) + 1);
	strcpy(new_repost->titlu, post_to_repost->titlu);
	new_repost->repost = 1;
	new_repost->likes = ll_create(sizeof(int));
	new_repost->parent = post_to_repost;
	new_repost->events = NULL;
	tree_add_child(post_to_repost, new_repost);
	free(new_repost);
}

// fucntie care imi creeaza un repost
void create_repost(int *id, linked_list_t *all_p) {
	words_t *w = malloc(sizeof(words_t));
	find_words(w);
	int user_id = get_user_id(w->words[0]);
	int post_id = make_int_from_char(w->words[1]);
	int r_id = 0;
	post_t *p_to_r = find_post_list(post_id, all_p);
	if (w->nr_words == 3) {
		// daca am un repost la un repost
		r_id = make_int_from_char(w->words[2]);
		post_t *p_off_r = find_repost(r_id, p_to_r->events->root->children);
		if (!p_off_r) {
			printf("The post to repost does not exist\n");
			return;
		}
		if (!p_off_r->events) {
			// pentru ca initial un repost are arborele de copi setat pe NULL
			// creez un arbore nou
			p_off_r->events = tree_create(sizeof(int));
			p_off_r->events->root = tree_node_create(id, sizeof(int));
			p_off_r->events->root->children = ll_create(sizeof(post_t));
		}
		add_repost_to_post(p_off_r, user_id, id);
	} else {
		// daca am un repost la o postare
		add_repost_to_post(p_to_r, user_id, id);
	}
	printf("Created repost #%d for %s\n", *id, w->words[0]);
	free(w);
}

// functie care imi afiseaza toate reposturile unei postari
void print_all_response(post_t *post) {
	if (!post->events)
		return;
	if (!post->events->root)
		return;
	if (!post->events->root->children)
		return;
	ll_node_t *curr = post->events->root->children->head;
	while (curr) {
		post_t *rep = (post_t *)curr->data;
		printf("Repost #%d by %s\n", rep->id, get_user_name(rep->user_id));
		print_all_response(rep);
		curr = curr->next;
	}
}

// functie care imi afiseaza toate reposturile unei postari sau un repost
void get_reposts(linked_list_t *all_p) {
	words_t *w = malloc(sizeof(words_t));
	find_words(w);
	int post_id = make_int_from_char(w->words[0]);
	post_t *post = find_post_list(post_id, all_p);
	if (!post) {
		printf("The post does not exist\n");
		return;
	}
	int repost_id = 0;
	if (w->nr_words == 2) {
		// daca am un repost
		repost_id = make_int_from_char(w->words[1]);
		post_t *p_off_r = find_repost(repost_id, post->events->root->children);
		if (!p_off_r) {
			printf("The post to repost does not exist\n");
			return;
		}
		printf("Repost #");
		printf("%d by %s\n", p_off_r->id, get_user_name(p_off_r->user_id));
		print_all_response(p_off_r);
	} else {
		// daca am o postare
		printf("%s - Post by %s\n", post->titlu, get_user_name(post->user_id));
		print_all_response(post);
	}
	free(w);
}

// functie care imi returneaza inaltimea unui repost din arborele postari
int h_find(post_t *post) {
	int h = 0;
	while (post->parent) {
		post = post->parent;
		h++;
	}
	return h;
}

// functie care imi returneaza primul repost comun a doua postari
void find_commun_repost(linked_list_t *all_p) {
	words_t *w = malloc(sizeof(words_t));
	find_words(w);
	int post_id = make_int_from_char(w->words[0]);
	int repost_id_1 = make_int_from_char(w->words[1]);
	int repost_id_2 = make_int_from_char(w->words[2]);
	post_t *post = find_post_list(post_id, all_p);
	if (!post) {
		printf("The post does not exist\n");
		return;
	}
	post_t *repost_1 = find_repost(repost_id_1, post->events->root->children);
	post_t *repost_2 = find_repost(repost_id_2, post->events->root->children);
	int h_repost_1 = h_find(repost_1);
	int h_repost_2 = h_find(repost_2);
	// aduc ambele inaltimi la acelasi nivel
	if (h_repost_1 > h_repost_2) {
		while (h_repost_1 != h_repost_2) {
			repost_1 = repost_1->parent;
			h_repost_1--;
		}
	} else {
		while (h_repost_1 != h_repost_2) {
			repost_2 = repost_2->parent;
			h_repost_2--;
		}
	}
	// fiind pe acelasi nivel ma intorc cu ambele pana se intalnesc
	while (repost_1->id != repost_2->id) {
		repost_1 = repost_1->parent;
		repost_2 = repost_2->parent;
	}
	printf("The first common repost of ");
	printf("%d and %d is %d\n", repost_id_1, repost_id_2, repost_1->id);
	free(w);
}

// functie care imi adauga like-uri la o postare sau la un repost
void like_post(linked_list_t *all_p) {
	words_t *w = malloc(sizeof(words_t));
	find_words(w);
	int user_id = get_user_id(w->words[0]);
	int post_id = make_int_from_char(w->words[1]);
	post_t *post = find_post_list(post_id, all_p);
	if (w->nr_words == 3) {
		// daca am un repost
		int repost_id = make_int_from_char(w->words[2]);
		post_t *repost = find_repost(repost_id, post->events->root->children);
		if (is_in_list(repost->likes, user_id)) {
			repost->nr_likes--;
			remove_node_from_list(repost->likes, &user_id);
			printf("User %s ", get_user_name(user_id));
			printf("unliked repost %s\n", repost->titlu);
		} else {
			add_node_at_start(repost->likes, &user_id);
			repost->nr_likes++;
			printf("User %s ", get_user_name(user_id));
			printf("liked repost %s\n", repost->titlu);
		}
	} else {
		// daca am o postare
		if (is_in_list(post->likes, user_id)) {
			post->nr_likes--;
			remove_node_from_list(post->likes, &user_id);
			printf("User %s ", get_user_name(user_id));
			printf("unliked post %s\n", post->titlu);
		} else {
			add_node_at_start(post->likes, &user_id);
			post->nr_likes++;
			printf("User %s ", get_user_name(user_id));
			printf("liked post %s\n", post->titlu);
		};
	}
	free(w);
}

// functie care imi afiseaza like-urile unei postari sau ale unui repost
void get_likes(linked_list_t *all_p) {
	words_t *w = malloc(sizeof(words_t));
	find_words(w);
	int post_id = make_int_from_char(w->words[0]);
	post_t *post = find_post_list(post_id, all_p);
	int repost_id = 0;
	if (w->nr_words == 2) {
		repost_id = make_int_from_char(w->words[1]);
		post_t *repost = find_repost(repost_id, post->events->root->children);
		printf("Repost #%d has %d likes\n", repost->id, repost->nr_likes);
	} else {
		printf("Post %s has %d likes\n", post->titlu, post->nr_likes);
	}
	free(w);
}

// functie care imi verifica daca un repost are mai multe like-uri decat
// postarea folosesc aceasta functie pentru ratio
void check_repost_likes(post_t *post, int *max_likes, int *repost_id) {
	if (!post->events)
		return;
	if (!post->events->root)
		return;
	if (!post->events->root->children)
		return;
	ll_node_t *curr = post->events->root->children->head;
	while (curr) {
		post_t *repost = (post_t *)curr->data;
		if (repost->nr_likes > *max_likes ||
			(repost->nr_likes == *max_likes && repost->id > *repost_id)) {
			(*max_likes) = repost->nr_likes;
			(*repost_id) = repost->id;
		}
		check_repost_likes(repost, max_likes, repost_id);
		curr = curr->next;
	}
}

// fucntie care imi afiseaza postarea/repostul cu cel mai mare ratio
void ratio(linked_list_t *all_p) {
	words_t *w = malloc(sizeof(words_t));
	find_words(w);
	int post_id = make_int_from_char(w->words[0]);
	post_t *post = find_post_list(post_id, all_p);
	int max_likes = 0;
	int repost_id = 0;
	check_repost_likes(post, &max_likes, &repost_id);
	if (repost_id && max_likes > post->nr_likes)
		printf("Post %d got ratio'd by repost %d\n", post->id, repost_id);
	else
		printf("The original post is the highest rated\n");
	free(w);
}

// functie care imi sterge o postare sau un repost
void delete_post(linked_list_t *all_p) {
	words_t *w = malloc(sizeof(words_t));
	find_words(w);
	int post_id = make_int_from_char(w->words[0]);
	post_t *post = find_post_list(post_id, all_p);
	// daca am un repost sters si arborele de copii al acestuia
	if (w->nr_words == 2) {
		int rep_id = make_int_from_char(w->words[1]);
		post_t *repost = find_repost(rep_id, post->events->root->children);
		if (repost->events)
			tree_free(repost->events);
		free(repost->titlu);
		ll_free(&repost->likes);
		remove_node_from_list(repost->parent->events->root->children, &rep_id);
		printf("Deleted repost #%d of post %s\n", rep_id, post->titlu);
	} else {
		if (post->events)
			post_list_tree_free(&post->events->root->children);
		printf("Deleted %s\n", post->titlu);
		free(post->titlu);
		ll_free(&post->likes);
		free(post->events->root->data);
		free(post->events->root);
		free(post->events);
		remove_node_from_list(all_p, &post_id);
	}
	free(w);
}

void handle_input_posts(char *input, int *id, linked_list_t *all_p) {
	char *commands = strdup(input);
	char *cmd = strtok(commands, "\n ");
	if (!cmd)
		return;
	if (!strcmp(cmd, "create")) {
		create_post(id, all_p);
		(*id)++;
	}
	if (!strcmp(cmd, "repost")) {
		create_repost(id, all_p);
		(*id)++;
	}
	if (!strcmp(cmd, "common-repost"))
		find_commun_repost(all_p);
	if (!strcmp(cmd, "like"))
		like_post(all_p);
	if (!strcmp(cmd, "ratio"))
		ratio(all_p);
	if (!strcmp(cmd, "delete"))
		delete_post(all_p);
	if (!strcmp(cmd, "get-likes"))
		get_likes(all_p);
	if (!strcmp(cmd, "get-reposts"))
		get_reposts(all_p);
	free(commands);
}

