#ifndef FIFO_H_INCLUDED
#define FIFO_H_INCLUDED
#include <stdbool.h>


typedef struct pipes *PipesPtr;

PipesPtr initPipes(int argc, char *argv[]);
void     sendStringToPipe(PipesPtr channel, const char *data);
bool     getStringFromPipe(PipesPtr channel, char *buffer, size_t size);
void     closePipes(PipesPtr channel);

#endif // FIFO_H_INCLUDED
