#include <stdio.h>
#include <stdlib.h>
#include <iostream>
#include <unistd.h>
#include <fcntl.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <cstddef>
#include <string.h>

#define N 30
using namespace std;

void executeCommand(char command[]);

int main()
{
    char *comando1, *comando2;
    char *opcion1, *opcion2;
    char linea[N];

    char *argument[N];

    pid_t pid;

    int wstatus;
    int errorNo;

    bool tuberia = true;

    while (true)
    {
        comando2 = NULL;
        errorNo = 0;
        cout << "$: ";
        cin.getline(linea, N);

        for (int i = 0; linea[i] != '\0'; i++)
        {
            if (linea[i] == '|')
            {
                comando1 = strtok(linea, "|");
                comando2 = strtok(NULL, "|");

                tuberia = false;

                if (pid = fork() == 0)
                {
                    int pid;
                    int pipeStatus[2];

                    pipe(pipeStatus);

                    if ((pid = fork()) < 0)
                    {
                        cerr << "Fork error al ejecutar el comando pipe" << endl;
                        exit(1);
                    }

                    if (pid == 0)
                    {
                        close(1);
                        dup(pipeStatus[1]);

                        close(pipeStatus[0]);
                        close(pipeStatus[1]);

                        executeCommand(comando1);
                    }
                    else
                    {
                        close(0);
                        dup(pipeStatus[0]);

                        close(pipeStatus[0]);
                        close(pipeStatus[1]);

                        executeCommand(comando2);
                    }
                }
            }
        }

        comando1 = strtok(linea, " ");
        opcion1 = strtok(NULL, " ");

        if ((pid = fork()) == 0 && comando2 == NULL)
        {
            errorNo = execlp(comando1, comando1, opcion1, NULL);
        }

        if (errorNo == -1)
        {
            cout << "Comando no encontrado" << endl;
        }
        else
        {
            wait(&wstatus);
        }
    }
}

void executeCommand(char command[])
{
    int index = 1;
    char *args[30];
    char *token = strtok(command, " ");
    args[0] = strdup(token);

    while ((token = strtok(NULL, " ")) != NULL)
    {
        args[index] = strdup(token);
        ++index;
    }
    args[index] = NULL;

    execvp(args[0], args);
}
