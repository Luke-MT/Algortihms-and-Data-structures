#include <arpa/inet.h>
#include <ctype.h>
#include <netdb.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <unistd.h>

/*
 * Luca Arborio - 20038688
 */

int main(int argc, char *argv[]) {

  int simpleSocket = 0;
  int simplePort = 0;
  int returnStatus = 0;
  char buffer[256] = "";
  struct sockaddr_in simpleServer;

  if (3 != argc) {
    fprintf(stderr, "Usage: %s <server> <port>\n", argv[0]);
    exit(1);
  }

  simpleSocket = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);

  if (simpleSocket == -1) {
    fprintf(stderr, "Could not create a socket!\n");
    exit(1);
  } 

  simplePort = atoi(argv[2]);

  memset(&simpleServer, '\0', sizeof(simpleServer));
  simpleServer.sin_family = AF_INET;
  simpleServer.sin_addr.s_addr = inet_addr(argv[1]);
  simpleServer.sin_port = htons(simplePort);

  returnStatus = connect(simpleSocket, (struct sockaddr *)&simpleServer, sizeof(simpleServer));

  if (returnStatus != 0) {
    fprintf(stderr, "Could not connect to address!\n");
    close(simpleSocket);
    exit(1);
  } 

  /*leggo il messaggio di benvenuto del server*/
  returnStatus = read(simpleSocket, buffer, sizeof(buffer));
  if (returnStatus <= 0) {
    fprintf(stderr, "Return Status = %d \n", returnStatus);
  } else {

    /*se il messaggio non presenta la parola OK, chiudo la connessione*/
    if (strncmp("OK", buffer, 2) != 0) {
      printf("Messaggio di benvenuto non riconosciuto\n");
      close(simpleSocket);
      exit(1);

    /*se il messaggio di benvenuto è corretto*/
    } else {
      char *token;
      token = strtok(buffer, " ");

      /*leggo i tentativi disponibili*/
      int tentativi = atoi(strtok(NULL, " "));

      /*mostro il messaggio di benvenuto all'utente*/
      token = strtok(NULL, " ");
      while (token != NULL) {
        printf("%s ", token);
        token = strtok(NULL, " ");
      }
      printf("\n");

      while (1) {
        /*chiedo all'utente quale operazione desidera fare*/
        int ans = 0;
        do {
          printf("Il numero di tentativi è %d\n", tentativi);
          printf("Cosa vuoi fare? [1/2]\n1-Indovinare la parola\n2-Abbandona\n");
          scanf("%d", &ans);
        } while (ans != 1 && ans != 2);

        /*se desidera indovinare la parola*/
        if (ans == 1) { 
          char guess[6];
          int isCorrect;

          /*chiedo di inserire una parola e controllo se è composta solo da 
            lettere dell'alfabeto*/
          do {
            isCorrect = 1;
            printf("Inserisci un parola di 5 lettere: ");
            scanf("%s", guess);

            for (int i = 0; guess[i] != '\0'; i++)
              if (isalpha(guess[i]) == 0) {
                isCorrect = 0;
                printf("La parola può avere solo lettere dell'alfabeto\n");
                break;
              }
          } while (isCorrect == 0);

          /*invio il comando WORD al server*/
          memset(buffer, 0, sizeof(buffer));
          snprintf(buffer, sizeof(buffer), "WORD %s\n", guess);
          write(simpleSocket, buffer, sizeof(buffer));

          /*leggo la risposta del server e mostro all'utente uno dei seguenti risultati*/
          returnStatus = read(simpleSocket, buffer, sizeof(buffer));
          if (returnStatus <= 0) {
            fprintf(stderr, "Return Status = %d \n", returnStatus);
          } else {

            if (strncmp("OK PERFECT", buffer, 10) == 0) {
              printf("Parola indovinata\n");
              break;

            } else if (strncmp("OK", buffer, 2) == 0) {
              token = strtok(buffer, " ");
              printf("\n\nPAROLA NON ESATTA\n");
              tentativi = atoi(strtok(NULL, " "));
              printf("\nLegenda\n*: la lettera è corretta,\n+: la lettera è "
                     "corretta ma nella posizione sbagliata,\n-: la lettera "
                     "non è presente nella parola.\n");
              printf("\n%s\n", guess);
              printf("%s\n", strtok(NULL, " "));
              continue;

            } else if (strncmp("END", buffer, 3) == 0) {
              printf("Tentativi esauriti\n");
              break;

            } else if (strncmp("ERR", buffer, 3) == 0) {
              token = strtok(buffer, " ");
              token = strtok(NULL, " ");
              while (token != NULL) {
                printf("%s ", token);
                token = strtok(NULL, " ");
              }
              break;
            }
          }
        }

        /*se desidera interrompere il gioco*/
        else if (ans == 2) {
          /*invio il comando QUIT al server*/
          memset(buffer, 0, sizeof(buffer));
          snprintf(buffer, sizeof(buffer), "QUIT\n");
          write(simpleSocket, buffer, sizeof(buffer));

          /*leggo la risposta del server al comando QUIT del client*/
          returnStatus = read(simpleSocket, buffer, sizeof(buffer));
          if (returnStatus <= 0) {
            fprintf(stderr, "Return Status = %d \n", returnStatus);
          } else {
            /*mostro all'utente il messaggio ricevuto dal server*/
              token = strtok(buffer, " ");
              token = strtok(NULL, " ");
              while (token != NULL) {
                printf("%s ", token);
                token = strtok(NULL, " ");
              }
              break;
          }
        }
      }
    }
  }

  close(simpleSocket);
  return 0;
}
