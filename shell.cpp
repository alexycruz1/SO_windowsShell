#include <iostream>
#include <unistd.h>
#include <sys/wait.h>
#include <string.h>

#define N 30
using namespace std;

int main(int argc, char *argv[]) {
  char *comando, *opcion, linea[N];
  pid_t pid;
  int wstatus, errorNo;

  while (true) {
    errorNo=0;
    
    cout << "$: ";
    cin.getline(linea, N);

    comando = strtok(linea," ");
    opcion = strtok(NULL," ");
    
    if ((pid=fork())==0){
      errorNo = execlp(comando, comando, opcion, NULL);
      
      if (errorNo == -1){
        cout << "Comando no encontrado" << endl;
      } else {
        wait(&wstatus);
      }
    }
  }

  return 0;
}
