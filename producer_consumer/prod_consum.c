#include "pc.h"
#include "my_queue.h"
#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include <pthread.h>
#include <unistd.h>
#include <sys/time.h>

#define MAX_QUEUE_LENGTH 5

pthread_mutex_t lock;
pthread_cond_t cond;
Queue queue;

//this thread will pull prime numbers
void * red_thread(void * vargp)
{
  pthread_detach(pthread_self());
  while(1)
  {
    pthread_mutex_lock(&lock);

    while(get_size(queue) == 0)
    {
      pthread_cond_signal(&cond);
      pthread_cond_wait(&cond, &lock);
    }
    //check queue to see if first item belongs to this thread
    if(validate(peek(queue)) == 0) printf("%s\n", dequeue(queue));
    pthread_mutex_unlock(&lock);
  }
  return NULL;
}

//this thread will pull fibonacci numbers
void * green_thread(void * vargp)
{
  pthread_detach(pthread_self());
  while(1)
  {
    pthread_mutex_lock(&lock);

    while(get_size(queue) == 0)
    {
      pthread_cond_signal(&cond);
      pthread_cond_wait(&cond, &lock);
    }
    //check queue to see if first item belongs to this thread
    if(validate(peek(queue)) == 1) printf("        %s\n", dequeue(queue));
    pthread_mutex_unlock(&lock);
  }
  return NULL;
}

//this thread will pull multiples of 3 and 5
void * blue_thread(void * vargp)
{
  pthread_detach(pthread_self());
  while(1)
  {
    pthread_mutex_lock(&lock);

    while(get_size(queue) == 0)
    {
      pthread_cond_signal(&cond);
      pthread_cond_wait(&cond, &lock);
    }
    //check queue to see if first item belongs to this thread
    if(validate(peek(queue)) == 2) printf("                %s\n", dequeue(queue));
    pthread_mutex_unlock(&lock);
  }
  return NULL;
}

void * producer_thread(void * vargp)
{
char * ts_error = "ERROR: Please enter a valid request: <identifier> <data>\n";
char * quit_com = "quit";
char request[2048];

pthread_detach(pthread_self());

  //reads input from user on loop, validates each time input is given
  while(scanf(" %[^\r\n]", request) > 0 && !feof(stdin))
  {
    pthread_mutex_lock(&lock);

    if(strcmp(request, quit_com) == 0) break;
    if(validate(request) >= 0)
    {
      //printf("\nHooray! Success!!! You entered a valid request: %s\n", request);
      //queue will be added to here
      enqueue(queue, request);
    }
    else printf("%s\n", ts_error);
    while(get_size(queue) == MAX_QUEUE_LENGTH)
    {
      pthread_cond_signal(&cond);
      printf("Queue is at max length. Signaling consumers to take over.\n");
      pthread_cond_wait(&cond, &lock);
      printf("Queue is empty again. Producer is resuming.\n");
    }
    memset(&request[0], 0, sizeof(request));
    pthread_mutex_unlock(&lock);
  }
  pthread_exit(0);
}

int main(int argc, char * argv[])
{
  pthread_t producer_id, red_id, green_id, blue_id;
  int producer_var = 1;

  queue = initialize_queue();

  pthread_mutex_init(&lock, NULL);
  pthread_cond_init(&cond, NULL);

  pthread_create(&producer_id, NULL, producer_thread, &producer_var);
  pthread_create(&red_id, NULL, red_thread, &producer_var);
  pthread_create(&green_id, NULL, green_thread, &producer_var);
  pthread_create(&blue_id, NULL, blue_thread, &producer_var);

  pthread_exit(0);
}
