#include "linked_list.h"

#include <stdlib.h>

void free_util(struct list_node* node);

struct list_node *new_node(size_t value) {
    struct list_node* node = malloc(sizeof(struct list_node));;
    node->value = value;
    return node; 
}

void insert_at_head(struct linked_list *list, size_t value) {
    struct list_node* node = new_node(value);

    node->next = list->head;
    list->head = node;
}

void insert_at_tail(struct linked_list *list, size_t value) {
    struct list_node* node = list->head;
    while(node->next != NULL){
	node = node->next;
    }

    node->next = new_node(value);
}

size_t remove_from_head(struct linked_list *list) {
    struct list_node* delete = list->head;
    size_t value = list->head->value;

    list->head = list->head->next;

    free(delete);

    return value; 
}

size_t remove_from_tail(struct linked_list *list) {
    struct list_node* node = list->head;
    struct list_node* prev;
    size_t value;
    while(node->next != NULL){
	prev = node;
	node = node->next;
    }

    value = node->value;
    prev->next = NULL;
    free(node);

    return value; 
}

void free_list(struct linked_list list) {
    free_util(list.head);
}

void free_util(struct list_node* node){
    if(node->next == NULL){
	free(node);
	return;
    }

    free_util(node->next);
    free(node);
}

// Utility function to help you debugging, do not modify
void dump_list(FILE *fp, struct linked_list list) {
  fprintf(fp, "[ ");
  for (struct list_node *cur = list.head; cur != NULL; cur = cur->next) {
    fprintf(fp, "%zu ", cur->value);
  }
  fprintf(fp, "]\n");
}
