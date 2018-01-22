#ifndef MY_QUEUE_H
#define MY_QUEUE_H

#include <stdlib.h>

typedef struct Node Node;
struct Node
{
  char * data;
  Node * next;
  Node * prev;
};

typedef struct Queue * Queue;
struct Queue
{
  Node * head;
  Node * tail;
  int size;
};

Queue initialize_queue();
Node * create_node(char * data);
void enqueue(Queue queue, char * data);
char * dequeue(Queue queue);
char * peek(Queue queue);
int get_size(Queue queue);
void print_queue(Queue queue);
void destroy_queue(Queue queue);
void destroy_node(Node * node);


#endif
