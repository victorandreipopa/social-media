#include "graf.h"

#define MAX_COMMAND_LEN 500
#define MAX_PEOPLE 550

linked_list_t *ll_create(unsigned int data_size)
{
	linked_list_t *ll = malloc(sizeof(linked_list_t));
	DIE(!ll, "malloc list failed");
	ll->data_size = data_size;
	ll->head = NULL;
	ll->size = 0;
	return ll;
}

void add_node_at_start(linked_list_t *list, const void *new_data) {
	ll_node_t *node = malloc(sizeof(ll_node_t));
	DIE(!node, "malloc failed");
	node->data = malloc(list->data_size);
	DIE(!node->data, "malloc failed");
	memcpy(node->data, new_data, list->data_size);
	node->next = list->head;
	list->head = node;
	list->size++;
}

void add_node_at_end(linked_list_t *list, const void *new_data) {
	ll_node_t *node = malloc(sizeof(ll_node_t));
	DIE(!node, "malloc failed");
	node->data = malloc(list->data_size);
	DIE(!node->data, "malloc failed");
	memcpy(node->data, new_data, list->data_size);
	node->next = NULL;
	if (!list->head) {
		list->head = node;
		list->size++;
		return;
	}
	ll_node_t *p = list->head;
	while (p->next)
		p = p->next;
	p->next = node;
	list->size++;
}

list_graph_t *lg_create(void) {
	list_graph_t *graf = malloc(sizeof(list_graph_t));
	DIE(!graf, "malloc graph failed");
	graf->nodes = MAX_PEOPLE;
	graf->neighbors = malloc(MAX_PEOPLE * sizeof(linked_list_t *));
	DIE(!graf->neighbors, "malloc neighbours failed");
	for (int i = 0; i < MAX_PEOPLE; i++)
		graf->neighbors[i] = ll_create(sizeof(int));
	return graf;
}

unsigned int ll_get_size(linked_list_t *list)
{
	if (!list)
		return -1;
	return list->size;
}

// dau free la lista , inclusiv la elemntele ei
void ll_free(linked_list_t **pp_list)
{
	if (!pp_list || !*pp_list)
		return;
	ll_node_t *p = (*pp_list)->head;
	while (p) {
		ll_node_t *aux = p;
		p = p->next;
		if (aux->data)
			free(aux->data);
		aux->data = NULL;
		free(aux);
		aux = NULL;
	}
	free(*pp_list);
	*pp_list = NULL;
}

void lg_free(list_graph_t *graph) {
	for (int i = 0; i < graph->nodes; i++)
		ll_free(&graph->neighbors[i]);
	free(graph->neighbors);
	free(graph);
}

void q_free(queue_t **pp_q)
{
	if (!pp_q || !*pp_q)
		return;
	free((*pp_q)->data);
	(*pp_q)->data = NULL;
	free(*pp_q);
	*pp_q = NULL;
}

void q_enqueue(queue_t *q, const void *new_data)
{
	if (q->size == q->max_size)
		return;
	memcpy((char *)q->data + q->back * q->data_size, new_data, q->data_size);
	q->back = (q->back + 1) % q->max_size;
	q->size++;
}

void q_dequeue(queue_t *q)
{
	if (q->size == 0)
		return;
	q->front = (q->front + 1) % q->max_size;
	q->size--;
}

void *q_front(queue_t *q)
{
	if (q->size == 0)
		return NULL;
	return (char *)q->data + q->front * q->data_size;
}

queue_t *q_create(unsigned int data_size, unsigned int max_size)
{
	queue_t *q = malloc(sizeof(queue_t));
	DIE(!q, "malloc queue failed");
	q->data_size = data_size;
	q->max_size = max_size;
	q->size = 0;
	q->front = 0;
	q->back = 0;
	q->data = malloc(data_size * max_size);
	DIE(!q->data, "malloc data failed");
	return q;
}

tree_node_t *tree_node_create(void *data, size_t data_size) {
	tree_node_t *node;
	node = malloc(sizeof(tree_node_t));
	DIE(!node, "node malloc");
	node->data = malloc(data_size);
	DIE(!node->data, "node->data malloc");
	memcpy(node->data, data, data_size);
	return node;
}

tree_t *tree_create(size_t data_size) {
	tree_t *tree = malloc(sizeof(tree_t));
	tree->root = NULL;
	tree->data_size = data_size;
	return tree;
}

void free_post(post_t *post) {
	ll_free(&post->likes);
	free(post->titlu);
	free(post);
}

void tree_free(tree_t *tree) {
	if (!tree->root) {
		free(tree);
		return;
	}
	ll_node_t *curr;
	if (!tree->root->children)
		curr = NULL;
	else
		curr = tree->root->children->head;
	while (curr) {
		post_t *post = (post_t *)curr->data;
		if (post->events)
			tree_free(post->events);
		free_post(post);
		ll_node_t *aux = curr;
		curr = curr->next;
		free(aux);
	}
	free(tree->root->data);
	free(tree->root->children);
	free(tree->root);
	free(tree);
}

void post_list_tree_free(linked_list_t **all_p) {
	ll_node_t *curr = (*all_p)->head;
	while (curr) {
		post_t *post = (post_t *)curr->data;
		if (post->events)
			tree_free(post->events);
		free_post(post);
		ll_node_t *aux = curr;
		curr = curr->next;
		free(aux);
	}
	free(*all_p);
}

int is_in_list(linked_list_t *list, int id) {
	ll_node_t *curr = list->head;
	while (curr) {
		int *id_post = (int *)curr->data;
		if (*id_post == id)
			return 1;
		curr = curr->next;
	}
	return 0;
}

void remove_node_from_list(linked_list_t *list, int *id) {
	ll_node_t *curr = list->head;
	ll_node_t *prev = NULL;
	while (curr) {
		int *id_post = (int *)curr->data;
		if (*id_post == *id) {
			if (!prev) {
				list->head = curr->next;
				free(curr->data);
				free(curr);
				list->size--;
				return;
			}
			prev->next = curr->next;
			free(curr->data);
			free(curr);
			list->size--;
			return;
		}
		prev = curr;
		curr = curr->next;
	}
}
