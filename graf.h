#ifndef GRAF_H
#define GRAF_H
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include <errno.h>

#define DIE(assertion, call_description)            \
	do                                              \
	{                                               \
		if (assertion)                              \
		{                                           \
			fprintf(stderr, "(%s, %d): ", __FILE__, \
					__LINE__);                      \
			perror(call_description);               \
			exit(errno);                            \
		}                                           \
	} while (0)

// structura de lista
typedef struct tree_t tree_t;
typedef struct tree_node_t tree_node_t;
typedef struct post_t post_t;
typedef struct ll_node_t ll_node_t;
typedef struct linked_list_t linked_list_t;
typedef struct list_graph_t list_graph_t;
typedef struct queue_t queue_t;

struct post_t
{
	int id;
	char *titlu;
	int user_id;
	int nr_likes;
	int repost;
	linked_list_t *likes;
	tree_t *events;
	post_t *parent;
};

struct ll_node_t
{
	void *data;
	ll_node_t *next;
};

struct linked_list_t
{
	ll_node_t *head;
	unsigned int data_size;
	unsigned int size;
};

struct list_graph_t
{
	linked_list_t **neighbors;
	int nodes;
};

struct queue_t
{
	unsigned int data_size;
	unsigned int max_size;
	unsigned int size;
	unsigned int front;
	unsigned int back;
	void *data;
};

struct tree_node_t
{
	/* o lista de copii*/
	linked_list_t *children;
	/* data contained by the node */
	void *data;
};

struct tree_t
{
	/* root of the tree */
	tree_node_t *root;
	size_t data_size;
};

queue_t *q_create(unsigned int data_size, unsigned int max_size);
void q_enqueue(queue_t *q, const void *new_data);
void q_dequeue(queue_t *q);
void *q_front(queue_t *q);
void q_free(queue_t **pp_q);
linked_list_t *ll_create(unsigned int data_size);
void add_node_at_start(linked_list_t *list, const void *new_data);
void add_node_at_end(linked_list_t *list, const void *new_data);
list_graph_t *lg_create(void);
unsigned int ll_get_size(linked_list_t *list);
void ll_free(linked_list_t **pp_list);
void lg_free(list_graph_t *graph);
tree_node_t *tree_node_create(void *data, size_t data_size);
tree_t *tree_create(size_t data_size);

void post_list_tree_free(linked_list_t **all_p);
void free_post(post_t *post);
int is_in_list(linked_list_t *list, int id);
void tree_free(tree_t *tree);
void remove_node_from_list(linked_list_t *list, int *id);
#endif // GRAF_H
