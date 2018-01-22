CC = gcc
CFLAGS = -g -Wall -pthread
PC = prod_consum
PC_SOURCES = prod_consum.c pc.c my_queue.c
#HEADERS = $(wildcard *.h)

$(PC): $(subst .c,.o,$(PC_SOURCES)) $(HEADERS)
	$(CC) $(CFLAGS) -o $@ $^ -lm


%.o: %.c #$(HEADERS)
	$(CC) $(CFLAGS) -c $< $(HEADERS) -lm

clean:
	rm -f *.o
	rm -f *.gch
	rm -f $(PC)
