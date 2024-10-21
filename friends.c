#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <errno.h>
#include "friends.h"
#include "users.h"
#include "graf.h"

void add_friend(list_graph_t *graf, int f1, int f2) {
	add_node_at_start(graf->neighbors[f1], &f2);
	add_node_at_start(graf->neighbors[f2], &f1);
	//adaug legatura in ambele sensuri
	printf("Added connection %s - %s\n", get_user_name(f1), get_user_name(f2));
}

int check_friend(list_graph_t *graf, int f1, int f2) {
	ll_node_t *nod = graf->neighbors[f1]->head;
	while (nod) {//verific daca f2 este prieten cu f1
		int id = *(int *)nod->data;
		if (id == f2)
			return 1;
		nod = nod->next;
	}
	return 0;
}

// distanta dintre 2 noduri
void bfs_list_graph(list_graph_t *lg, int start, int dest) {
	int *visited = calloc(lg->nodes, sizeof(int));
	//bfs din laborator modificat pt a satisface cerinta cu distanta
	DIE(!visited, "calloc failed");
	int *dist = calloc(lg->nodes, sizeof(int));
	DIE(!dist, "caloc failed");
	queue_t *q = q_create(sizeof(int), lg->nodes);
	visited[start] = 1;
	dist[start] = 0;
	q_enqueue(q, &start);
	while (q->size) {
		int node = *(int *)q_front(q);
		q_dequeue(q);
		ll_node_t *nod = lg->neighbors[node]->head;
		while (nod) {
			int neighbor = *(int *)nod->data;
			if (visited[neighbor] == 0) {
				visited[neighbor] = 1;
				dist[neighbor] = dist[node] + 1;
				//distanta pt nodul in care ma duc este distanta de la
				//nodul din care am pornit + 1
				q_enqueue(q, nod->data);
			}
			nod = nod->next;
		}
	}
	int distance = dist[dest];//afisez distanta pana la destinatie
	free(visited);
	free(dist);
	q_free(&q);
	char *name1 = get_user_name(start);
	char *name2 = get_user_name(dest);
	if (distance == 0)//daca nu se poate ajunge
		printf("There is no way to get from %s to %s\n", name1, name2);
	else//altfel, o afisez
		printf("The distance between %s - %s is %d\n", name1, name2, distance);
}

void friends(list_graph_t *graf, int f1) {
	int prieteni = 0;//afisez nr de prieteni ai lui f1
	if (graf->neighbors[f1]->size)
		prieteni = graf->neighbors[f1]->size;
	printf("%s has %d friends\n", get_user_name(f1), prieteni);
}

void popular(list_graph_t *graf, int f1) {
	unsigned int max = graf->neighbors[f1]->size;
	int max_id = f1;
	ll_node_t *nod = graf->neighbors[f1]->head;
	while (nod) {//determin prietenul lui f1 cu cei mai multi prieteni
		int id = *(int *)nod->data;
		if (graf->neighbors[id]->size > max) {
			max = graf->neighbors[id]->size;
			max_id = id;
		}
		nod = nod->next;
	}
	if (max_id == f1) {
		printf("%s is the most popular\n", get_user_name(f1));
		return;//daca f1 are cel mai multi prieteni at el este cel mai popular
	}
	//altfel, afisez pe cel mai popular
	char *max_name = get_user_name(max_id);
	char *name = get_user_name(f1);
	printf("%s is the most popular friend of %s\n", max_name, name);
}

void common(list_graph_t *graf, int f1, int f2) {
	ll_node_t *nod1 = graf->neighbors[f1]->head;
	ll_node_t *nod2 = graf->neighbors[f2]->head;
	int *friends1 = calloc(graf->nodes, sizeof(int));
	DIE(!friends1, "calloc failed");
	int *friends2 = calloc(graf->nodes, sizeof(int));
	DIE(!friends2, "calloc failed");
	int common = 0;
	while (nod1) {//pun prietenii lui f1 intr-un vector de frecventa
		int id = *(int *)nod1->data;
		friends1[id] = 1;
		nod1 = nod1->next;
	}
	while (nod2) {//pun si pe ai lui f2 in alt vector si verific daca sunt
		int id = *(int *)nod2->data;//si in primul
		friends2[id] = 1;
		if (friends1[id])
			common++;//daca da,maresc contorul
		nod2 = nod2->next;
	}
	char *name1 = get_user_name(f1);
	char *name2 = get_user_name(f2);
	if (common == 0) {//in cazul in care nu e niciunul comun
		free(friends1);
		free(friends2);
		printf("No common friends for %s and %s\n", name1, name2);
		return;
	}
	//altfel, afisez prietenii comuni
	printf("The common friends between %s and %s are:\n", name1, name2);
	for (int i = 0; i < graf->nodes; i++)
		if (friends1[i] && friends2[i])
			printf("%s\n", get_user_name(i));
	free(friends1);
	free(friends2);
}

void suggestions(list_graph_t *graf, int f1) {
	int sugestii = 0;
	int *friends = calloc(graf->nodes, sizeof(int));
	ll_node_t *nod = graf->neighbors[f1]->head;
	while (nod) {//pun prietenii lui f1 in vector de frecventa
		int id = *(int *)nod->data;
		friends[id] = 1;
		nod = nod->next;
	}
	nod = graf->neighbors[f1]->head;
	while (nod) {
		int i = *(int *)nod->data;
		ll_node_t *aux = graf->neighbors[i]->head;
		while (aux) {//pt fiecare prieten al lui f1,verific daca are
			int j = *(int *)aux->data;//prieteni diferiti
			if (friends[j] == 0 && j != f1) {
				friends[j] = 2;//daca da,atunci ii contorizez pe aceia
				sugestii++;//nr cati sunt
			}
			aux = aux->next;
		}
		nod = nod->next;
	}
	if (sugestii == 0) {//daca nu e niciunul
		free(friends);
		printf("There are no suggestions for %s\n", get_user_name(f1));
		return;
	}
	//altfel,ii afisez
	printf("Suggestions for %s:\n", get_user_name(f1));
	for (int i = 0; i < graf->nodes; i++)
		if (friends[i] == 2)
			printf("%s\n", get_user_name(i));
	free(friends);
}

void remove_friend_from_list(linked_list_t *list, int f) {
	ll_node_t *nod = list->head;
	ll_node_t *prev = NULL;
	while (nod) {//caut prin lista si elimin nodul cu id ul f
		int id = *(int *)nod->data;
		if (id == f) {
			if (prev)//ma folosesc de prev pt a sterge eficient
				prev->next = nod->next;
			else
				list->head = nod->next;
			free(nod->data);
			free(nod);//eliberez valoarea veche si nodul
			list->size--;
			break;
		}
		prev = nod;
		nod = nod->next;
	}
}

void remove_friend(list_graph_t *graf, int f1, int f2) {
	remove_friend_from_list(graf->neighbors[f1], f2);
	remove_friend_from_list(graf->neighbors[f2], f1);
	//sterg legatura din ambele sensuri
	char *name1 = get_user_name(f1);
	char *name2 = get_user_name(f2);
	printf("Removed connection %s - %s\n", name1, name2);
}

void handle_input_friends(char *input, list_graph_t *graf) {
	char *commands = strdup(input);
	char *cmd = strtok(commands, "\n ");
	char *friend1 = strtok(NULL, "\n ");//separ din linie numele
	char *friend2 = strtok(NULL, "\n ");
	if (!cmd)//functia pentru apelarea celorlalte functii pentru friends
		return;
	if (!strcmp(cmd, "add"))
		add_friend(graf, get_user_id(friend1), get_user_id(friend2));
	else if (!strcmp(cmd, "remove"))
		remove_friend(graf, get_user_id(friend1), get_user_id(friend2));
	else if (!strcmp(cmd, "suggestions"))
		suggestions(graf, get_user_id(friend1));
	else if (!strcmp(cmd, "distance"))
		bfs_list_graph(graf, get_user_id(friend1), get_user_id(friend2));
	else if (!strcmp(cmd, "common"))
		common(graf, get_user_id(friend1), get_user_id(friend2));
	else if (!strcmp(cmd, "friends"))
		friends(graf, get_user_id(friend1));
	else if (!strcmp(cmd, "popular"))
		popular(graf, get_user_id(friend1));
	free(commands);
}
