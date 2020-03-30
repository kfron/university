#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <windows.h>
#include "fifo.h"

struct pipes {
    HANDLE fifo_in, fifo_out;
    int isA;
};

void pokazBlad(char *komunikat);
static HANDLE openOutPipe(char *name);
static HANDLE openInPipe(char *name);

void closePipes(PipesPtr pipes)
{
  CloseHandle(pipes->fifo_in);
  CloseHandle(pipes->fifo_out);
  free(pipes);
}

PipesPtr initPipes(int argc,char *argv[])
{
    if (argc != 2 || (argv[1][0] != 'A' && argv[1][0] != 'B') || argv[1][1] != '\0')
    {
        printf("\n This program should be called with one argument: A or B\n");
        return 0;
    }
    PipesPtr pipes=(PipesPtr)malloc(sizeof(struct pipes));
    if (pipes == NULL) {
        fprintf(stderr,"Memory allocation error\n");
    } else {
        pipes->isA=(argv[1][0] == 'A');
        pipes->fifo_out = openOutPipe(pipes->isA ? "\\\\.\\pipe\\AtoB" : "\\\\.\\pipe\\BtoA");
        pipes->fifo_in = openInPipe(pipes->isA ? "\\\\.\\pipe\\BtoA" : "\\\\.\\pipe\\AtoB");
    }
    return pipes;
}

static HANDLE openOutPipe(char *name) {
    HANDLE pipe = CreateNamedPipe(
                   name, // name of the pipe
                   PIPE_ACCESS_OUTBOUND, // 1-way pipe -- send only
                   PIPE_TYPE_BYTE, // send data as a byte stream
                   1, // only allow 1 instance of this pipe
                   0, // no outbound buffer
                   0, // no inbound buffer
                   0, // use default wait time
                   NULL // use default security attributes
               );
    if (pipe == NULL || pipe == INVALID_HANDLE_VALUE)
    {
        printf("Error in creating output pipe\n");
        // look up error code here using GetLastError()
        exit(-1);
    }
    return pipe;
}

static HANDLE openInPipe(char *name){
    HANDLE pipe;
    do
    {
        pipe = CreateFile(
                      name,
                      GENERIC_READ, // only need read access
                      FILE_SHARE_READ | FILE_SHARE_WRITE,
                      NULL,
                      OPEN_EXISTING,
                      FILE_ATTRIBUTE_NORMAL,
                      NULL
                  );
        if (pipe == INVALID_HANDLE_VALUE)
        {
            printf("Pipe is not ready to connect - waiting for the second copy of the program ...\n");
            Sleep(3000);
        }
    }
    while (pipe == INVALID_HANDLE_VALUE);
    return pipe;
}

void sendStringToPipe(PipesPtr pipes,const char *data)
{
    DWORD numBytesWritten = 0;
    int result = WriteFile(
                     pipes->fifo_out, // handle to our outbound pipe
                     data, // data to send
                     strlen(data), // length of data to send (bytes)
                     &numBytesWritten, // will store actual amount of data sent
                     NULL // not using overlapped IO
                 );
    if (!result)
    {
        pokazBlad("Failed to send data");
     }
}

bool getStringFromPipe(PipesPtr pipes,char *buffer,size_t size)
{
    DWORD numBytesRead = 0, totalBytesAvail;

    int peekResult = PeekNamedPipe(
                     pipes->fifo_in,
                     buffer, // the data from the pipe will be put here
                     size-1, // number of bytes allocated
                     NULL, // do not want to read from pipe
                     &totalBytesAvail,
                     NULL);
    if (peekResult == 0 || totalBytesAvail == 0)
        return false;
    int result = ReadFile(
                     pipes->fifo_in,
                     buffer, // the data from the pipe will be put here
                     size-1, // number of bytes allocated
                     &numBytesRead, // this will store number of bytes actually read
                     NULL // no using overlapped IO
                 );
    if (result) buffer[numBytesRead] = '\0';
    return result;
}
