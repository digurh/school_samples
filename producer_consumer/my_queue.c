#include "my_queue.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>


Queue initialize_queue()
{
  Queue queue = (Queue)malloc(sizeof(Queue));
  if(queue == NULL) return NULL;
  queue->head = NULL;
  queue->tail = NULL;
  queue->size = 0;

  return queue;
}

Node * create_node(char * data)
{
  //malloc size for node and data in node
  Node * node = (Node *)malloc(sizeof(Node));
  if(node == NULL) return NULL;
  node->data = malloc(sizeof(data));
  strcpy(node->data, data);
  node->next = NULL;
  node->prev = NULL;

  return node;
}

void enqueue(Queue queue, char * data)
{
  Node * new_node = create_node(data);
  if(queue->size == 0)
  {
    queue->head = new_node;
    queue->tail = new_node;
    queue->size++;
  }
  else
  {
    new_node->prev = queue->tail;
    queue->tail->next = new_node;
    queue->tail = queue->tail->next;
    queue->size++;
  }
}

char * dequeue(Queue queue)
{
  char * queue_empty = "Queue is empty";
  if(queue->size == 0) return queue_empty;

  //set Node * to current head
  Node * dead_node_walking = queue->head;
  //set temp to take data from current head
  char * temp = malloc(sizeof(queue->head->data) + 1);
  strcpy(temp, queue->head->data);
  //move head to next node in queue
  queue->head = queue->head->next;
  //destroy old head
  destroy_node(dead_node_walking);
  queue->size--;
  return temp;
}

char * peek(Queue queue)
{
  char * queue_empty = "Queue is empty";
  if(queue->size == 0) return queue_empty;
  return queue->head->data;
}

int get_size(Queue queue)
{
  return queue->size;
}

void print_queue(Queue queue)
{
  Node * current = queue->head;
  do
  {
    printf("%s\n", current->data);
    current = current->next;
  } while(current != NULL);
}

void destroy_queue(Queue queue)
{
  if(queue->size == 0) free(queue);
  else
  {
    while(queue->size > 0)
    {
      dequeue(queue);
    }
  }
}

void destroy_node(Node * node)
{
  free(node->data);
  free(node);
}
