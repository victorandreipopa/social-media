/**
 * The entrypoint of the homework. Every initialization must be done here
*/
#include <stdlib.h>
#include <stdio.h>
#include <string.h>

#include "users.h"
#include "friends.h"
#include "posts.h"
#include "feed.h"
#include "graf.h"

/**
 * Initializez every task based on which task we are running
*/
void init_tasks(void)
{
	#ifdef TASK_1

	#endif

	#ifdef TASK_2

	#endif

	#ifdef TASK_3

	#endif
}

/**
 * Entrypoint of the program, compiled with different defines for each task
*/
int main(void)
{
	init_users();

	init_tasks();
	int id = 1;
	char *input = (char *)malloc(MAX_COMMAND_LEN);
	list_graph_t *graf = lg_create();
	linked_list_t *all_post = ll_create(sizeof(post_t));
	while (1) {
		char *command = fgets(input, MAX_COMMAND_LEN, stdin);

		// If fgets returns null, we reached EOF
		if (!command)
			break;

		#ifdef TASK_1
		handle_input_friends(input, graf);
		#endif

		#ifdef TASK_2
		handle_input_posts(input, &id, all_post);
		#endif

		#ifdef TASK_3
		handle_input_feed(input, all_post, graf);
		#endif
	}
	(void)id;
	lg_free(graf);
	post_list_tree_free(&all_post);
	free_users();
	free(input);

	return 0;
}
