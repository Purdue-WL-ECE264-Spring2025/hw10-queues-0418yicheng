#include "queue.h"
#include "tile_game.h"
#include <stdlib.h>

int checkWin(struct game_state* state);
void printState(struct game_state* state);
int contains(struct linked_list*, struct game_state*);
int checkVisited(struct queue*, struct game_state*);

void enqueue(struct queue *q, struct game_state state) {
    insert_at_head(&q->data, serialize(state));
}

struct game_state dequeue(struct queue *q) {
    if(q->data.head->next == NULL){
	return deserialize(q->data.head->value);
    }

    return deserialize(remove_from_tail(&q->data)); 
}

int number_of_moves(struct game_state start) {
    struct queue* q = malloc(sizeof(struct queue));

    struct list_node* node = malloc(sizeof(struct list_node));
    node->value = serialize(start);
    node->next = NULL;
    struct linked_list* list = malloc(sizeof(struct linked_list));
    list->head = node;
    q->data = *list;
    struct game_state state; 

    struct linked_list* visited = malloc(sizeof(struct linked_list));
    struct list_node* a = malloc(sizeof(struct list_node));
    a->value = 0;
    a->next = NULL;
    visited->head = a;

    int qSize = 1;
    while(qSize > 0){
	state = dequeue(q);
	qSize--;

	//Base case	
	if(checkWin(&state) == 1){
	    free_list(*visited);
	    free_list(q->data);
	    free(q);
	    return state.num_steps;
	}

	/*
	if(contains(visited, &state) == 1){
	    continue;
	}

	insert_at_head(visited, serialize(state));
	*/

	if(checkVisited(q, &state) == 1){
	    continue;
	}

	if(state.empty_row < 3){
	    move_up(&state);
	    enqueue(q, state);
	    qSize++;
	    move_down(&state);
	    state.num_steps -= 2;
	}

	if(state.empty_row > 0){
	    move_down(&state);
	    enqueue(q, state);
	    qSize++;
	    move_up(&state);
	    state.num_steps-=2;
	}

	if(state.empty_col < 3){
	    move_left(&state);
	    enqueue(q, state);
	    qSize++;
	    move_right(&state);
	    state.num_steps-=2;
	}

	if(state.empty_col > 0){
	    move_right(&state);
	    enqueue(q, state);
	    qSize++;
	    move_left(&state);
	    state.num_steps-=2;
	}


    }

    free_list(*visited);
    free_list(q->data);
    free(q);
    printf("NOT FOUND\n");
    return 0;
}

// Checks if the game is finished
int checkWin(struct game_state* state){
    int r, c;
    for(c = 0; c < 4; c++){
	for(r = 0; r < 4; r++){
	    if(r == 3 && c == 3) continue;
	    if(state->tiles[r][c] != 4*r + c + 1){
		return 0;
	    }
	}
    }
    return 1;
}

void printState(struct game_state* state){
    printf("%d steps\n", state->num_steps);
    int r, c;
    for(r = 0; r < 4; r++){
	for(c = 0; c < 4; c++){
	    printf("%d ", state->tiles[r][c]);
	}
	printf("\n");
    }
    printf("\n");
}

int contains(struct linked_list* list, struct game_state* valueState){
    struct list_node* node = list->head;
    struct game_state curr;
    int r,c;

    while(node->next != NULL){
	if(node->value == 0){
	    node = node->next;
	    continue;
	}

	curr = deserialize(node->value);
	int flag = 0;
	for(r = 0; r < 4 && flag == 0; r++){
	    for(c = 0; c < 4 && flag == 0; c++){
		if(valueState->tiles[r][c] != curr.tiles[r][c]){
		    flag = 1;
		}
	    }
	}
	if(flag == 0){
	    return 1;
	}

	node = node->next;
    }

    return 0;
}

int checkVisited(struct queue* q, struct game_state* state){
    struct linked_list list = q->data;
    struct list_node* node = list.head;

    while(node->next != NULL){
	struct game_state curr = deserialize(node->value);
	
	int flag = 0;
	int r, c;
	for(r = 0; r < 4 && flag == 0; r++){
	    for(c = 0; c < 4 && flag == 0; c++){
		if(curr.tiles[r][c] != state->tiles[r][c]){
		    flag = 1;
		}
	    }
	}

	if(flag == 0){
	    return 1;
	}

	node = node->next;
    }

    return 0;
}
