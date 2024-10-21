#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "feed.h"
#include "users.h"
#include "graf.h"
#include "friends.h"
#include "posts.h"

void feed(linked_list_t *all_post, list_graph_t *graf) {
	char *nume = strtok(NULL, "\n ");
	char *feed_size = strtok(NULL, "\n ");//extrag nr de postari
	int size = make_int_from_char(feed_size);//il fac de tip intreg
	int id = get_user_id(nume);
	ll_node_t *nod = all_post->head;
	int count = 0;
	while (nod && count < size) {//cat timp exista postari si nr lor < size
		post_t *post = (post_t *)nod->data;
		if (check_friend(graf, id, post->user_id) || post->user_id == id) {
			printf("%s: %s\n", get_user_name(post->user_id), post->titlu);
			//daca postarea ii apartine lui sau unui prieten de-al lui
			count++;
		}
		nod = nod->next;
	}
}

void print_all_repost(post_t *post, int id_utilizator) {
	if (!post->events)
		return;
	ll_node_t *curr;
	if (!post->events->root->children)
		curr = NULL;
	else
		curr = post->events->root->children->head;
	while (curr) {
		post_t *post = (post_t *)curr->data;
		if (post->user_id == id_utilizator) {
			printf("Reposted: %s\n", post->titlu);
			print_all_repost(post, id_utilizator);
			//afisez toate reposturile utilizatorului dorit
		}
		curr = curr->next;
	}
}

void print_all_post(ll_node_t *curr, int  id_utilizator) {
	if (!curr)
		return;
	post_t *post = (post_t *)curr->data;
	if (post->user_id == id_utilizator && !post->repost) {
		post->repost = 1;
		print_all_post(curr->next, id_utilizator);
		printf("Posted: %s\n", post->titlu);
	}
	print_all_post(curr->next, id_utilizator);
	//afisez toate postarile utilizatorului dorit
}

void view_profile(linked_list_t *all_p) {
	char *nume = strtok(NULL, "\n ");
	int id_utilizator = get_user_id(nume);
	//apelez functiile ajutatoare pt a afisa post si repost ale
	//utilizatorului dorit
	ll_node_t *curr = all_p->head;
	if (curr)
		print_all_post(curr, id_utilizator);
	curr = all_p->head;
	while (curr) {
		post_t *post = (post_t *)curr->data;
		print_all_repost(post, id_utilizator);
		curr = curr->next;
	}
}

void print_all_friends(post_t *post, int id_p, int id_u, list_graph_t *graf) {
	if (!post->events)
		return;
	//functie ajutatoare pt a afisa toti prietenii
	ll_node_t *curr;
	if (!post->events->root->children)
		curr = NULL;
	else
		curr = post->events->root->children->head;
	while (curr) {
		post_t *post = (post_t *)curr->data;
		if (check_friend(graf, id_u, post->user_id))
			printf("%s\n", get_user_name(post->user_id));
		print_all_friends(post, id_p, id_u, graf);
		curr = curr->next;
	}
}

void friends_repost(list_graph_t *graf, linked_list_t *all_p) {
	char *nume = strtok(NULL, "\n ");
	int id = get_user_id(nume);
	//afisez toti prietenii care au repostat postarea cu post_id
	char *post = strtok(NULL, "\n ");
	int post_id = make_int_from_char(post);
	post_t *post_to_check = find_post_list(post_id, all_p);
	print_all_friends(post_to_check, post_id, id, graf);
}

// fucntie care imi verifica daca un utilizator este prieten cu toti ceilalti
// din vectorul de frecventa
int check_all_friends(list_graph_t *graf, int *fr, int friend_to_add) {
	for (int i = 0; i < graf->nodes; i++)
		if (fr[i] && !check_friend(graf, i, friend_to_add))
			return 0;
	return 1;
}

// generez un grup de prieteni comuni si verific daca e mai mare
// decat grupul de prieteni maxim
void generare_grup_prieteni(list_graph_t *graf, int **max_fr, int **fr,
							int *nr_max, int *nr, int contor, int id) {
	if (contor >= graf->nodes) {
		if ((*nr) >= (*nr_max)) {
			(*nr_max) = (*nr);
			for (int i = 0; i < graf->nodes; i++)
				(*max_fr)[i] = (*fr)[i];
		}
		return;
	}
	generare_grup_prieteni(graf, max_fr, fr, nr_max, nr, contor + 1, id);
	if (contor != id && check_all_friends(graf, *fr, contor)) {
		(*fr)[contor] = 1;
		(*nr)++;
		generare_grup_prieteni(graf, max_fr, fr, nr_max, nr, contor + 1, id);
		(*fr)[contor] = 0;
		(*nr)--;
	}
}

// functie folosita pentru a afisa grupul de prieteni comuni
void common_groups(list_graph_t *graf) {
	char *nume = strtok(NULL, "\n ");
	int id = get_user_id(nume);
	printf("The closest friend group of %s is:\n", nume);
	// cel mai mare grup de prieteni comuni
	int *max_friends = (int *)calloc(graf->nodes, sizeof(int));
	int nr_max = 0;
	// un grup de prieteni comuni auxiliar pentru functia
	// generare_grup_prieteni
	int *friends = (int *)calloc(graf->nodes, sizeof(int));
	int nr = 1;
	friends[id] = 1;
	generare_grup_prieteni(graf, &max_friends, &friends, &nr_max, &nr, 0, id);
	for (int i = 0; i < graf->nodes; i++)
		if (max_friends[i] == 1)
			printf("%s\n", get_user_name(i));
	free(max_friends);
	free(friends);
}

void handle_input_feed(char *in, linked_list_t *all_post, list_graph_t *graf) {
	char *commands = strdup(in);
	char *cmd = strtok(commands, "\n ");
	if (!cmd)//functia care apeleaza toate celelalte functii de feed
		return;
	if (!strcmp(cmd, "feed"))
		feed(all_post, graf);
	else if (!strcmp(cmd, "view-profile"))
		view_profile(all_post);
	else if (!strcmp(cmd, "friends-repost"))
		friends_repost(graf, all_post);
	else if (!strcmp(cmd, "common-group"))
		common_groups(graf);
	free(commands);
}
