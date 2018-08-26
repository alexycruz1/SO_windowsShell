#include <iostream>
#include <unistd.h>
#include <sys/wait.h>
#include <string.h>
#include <cstdlib>

#define N 30
using namespace std;

int main(int argc, char *argv[]) {
	char *comando, *opcion, *opcion2, *comando2, linea[N];
	pid_t pid;
	int wstatus, errorNo;
	int pipeConnectors[2];
	char buffer[4096];
	while (true) {
		errorNo=0;

		cout << "$: ";
		cin.getline(linea, N);

		comando = strtok(linea,"|");
		comando2 = strtok(NULL,"");
		comando = strtok(linea, " ");
		opcion = strtok(NULL, " ");

		if (pipe(pipeConnectors) == -1) {
			cout << "ERROR CON EL PIPE" << endl;
		}

		if ((pid=fork())==0){
			cout << "@pid(35) " << pid << endl;
			cout << "1" << endl;
			dup2 (pipeConnectors[1], STDOUT_FILENO);
			close(pipeConnectors[0]);
			close(pipeConnectors[1]);
			errorNo = execlp(comando, comando, (opcion != NULL ? opcion : NULL), NULL);

			if (errorNo == -1){
				cout << "Comando no encontrado (children)" << endl;
			} else {
                _exit(EXIT_SUCCESS);
			}
		} else {
			cout << "2" << endl;
		    close(pipeConnectors[1]);
		    // int nbytes = read(pipeConnectors[0], buffer, sizeof(buffer));
		    // printf("Output: (%.*s)\n", nbytes, buffer);

			cout << "3" << endl;
			cout << "COMANDO 2: " << comando2 << endl;
			pid = fork();
			cout << "@pid(56) " << pid << endl;
			if (pid == 0) {
				errorNo = execlp(comando2, comando2, buffer, NULL);
			}
			cout << "4" << endl;

			if (errorNo == -1){
				cout << "Comando no encontrado" << endl;
			} else {
				wait(&wstatus);	
			}
		}
	}

	return 0;
}