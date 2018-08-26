#include <iostream>
#include <unistd.h>
#include <sys/wait.h>
#include <string.h>

#define N 30
using namespace std;

int main(int argc, char *argv[])
{
  char *comando, *comando2, *opcion, linea[N];
  pid_t pid1, pid2;
  int wstatus, errorNo, tuberia[2];

  if (pipe(tuberia) < 0)
  {
    cout << "No se inizializo la tuberia";
  }

  while (true)
  {

    errorNo = 0;
    
    cout << "$: ";
    cin.getline(linea, N);

    comando = strtok(linea, "|");
    comando2 = strtok(NULL, " ");
    comando = strtok(linea, " ");
    opcion = strtok(NULL, " ");

    /*cout << "Comando 1: " << comando;
    cout << "Comando 2: " << comando2;*/

    pid1 = fork();

    if (pid1 < 0)
    {
      cout << "No se pudo hacer fork del pid1";
      
    }

    if (pid1 == 0)
    {
      //cout << "Llegue hasta aqui";
      close(tuberia[0]);
      dup2(tuberia[1], 1);
      close(tuberia[1]);
      
      errorNo = execlp(comando, comando, comando2, NULL);
      

      if (errorNo == -1)
      {
        cout << "Comando 1 no encontrado";
      } else {
        wait(&wstatus);
      }
    }
    else
    {
      pid2 = fork();

      if (pid2 < 0)
      {
        cout << "No se pudo hacer fork del pid2";
      }

      if (pid2 == 0 && comando2 != "")
      {
        close(tuberia[1]);
        dup2(tuberia[0], 0);
        close(tuberia[0]);

        errorNo = execlp(comando2, comando2, opcion, NULL);

        if (errorNo == -1)
        {
          cout << "Comando 2 no encontrado";
        }
      }
      else
      {
        wait(&wstatus);
      }
    }
  }

  /*while (true) {
    errorNo=0;
    
    cout << "$: ";
    cin.getline(linea, N);

    comando = strtok(linea, "|");
    comando2 = strtok(NULL, " ");
    comando = strtok(linea, " ");
    opcion = strtok(NULL, " ");
    
    if ((pid1 = fork()) == 0){
      errorNo = execlp(comando, comando, opcion, NULL);
      
      if (errorNo == -1){
        cout << "Comando no encontrado" << endl;
      } else {
        wait(&wstatus);
      }
    }
  }*/

  return 0;
}
