#include "pc.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>


int perfect_square(int x)
{
  if(pow((int)sqrt(x), 2) == x) return 0;
  else return -1;
}

int is_prime(int data)
{
  int i;
  for(i = 2; i < data; i++)
  {
    if(data % i == 0) return -1;
  }
  return 0;
}

int is_fibonacci(int data)
{
  if(perfect_square(5 * data * data + 4) == 0 || perfect_square(5 * data * data  - 4) == 0) return 0;
  else return -1;
}

int is_multiple(int data)
{
  if(data % 3 == 0 || data % 5 == 0) return 0;
  else return -1;
}

int validate(char * request)
{
  //int data = atoi(request);
  //if(is_prime(data) == 0 ||  is_fibonacci(data) == 0 || is_multiple(data) == 0) return 0;

  char identifier[8];
  int data;
  if(sscanf(request, "%s %d", identifier, &data) == 2)
  {
    //printf("identifier = %s : data = %d\n", identifier, data);
    if(strcmp("red", identifier) == 0 || strcmp("RED", identifier) == 0 || strcmp("Red", identifier) == 0)
    {
      if(is_prime(data) == 0) return 0;
    }
    if(strcmp("green", identifier) == 0 || strcmp("GREEN", identifier) == 0 || strcmp("Green", identifier) == 0)
    {
      if(is_fibonacci(data) == 0) return 1;
    }
    if(strcmp("blue", identifier) == 0 || strcmp("BLUE", identifier) == 0 || strcmp("Blue", identifier) == 0)
    {
      if(is_multiple(data) == 0) return 2;
    }
  }

  return -1;
}
