#include <ctype.h>
#include <netdb.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <time.h>
#include <unistd.h>

/*
 * Luca Arborio - 20038688
 */

char MESSAGE[] = "Benvenuto in Wordle!";
char msg[256];

int main(int argc, char *argv[]) {

  int simpleSocket = 0;
  int simplePort = 0;
  int returnStatus = 0;
  char buffer[256] = "";
  struct sockaddr_in simpleServer;

  int tentativi = 6;
  char target[6];

  if (argc < 2) {
    fprintf(stderr, "Usage: %s <port>\n", argv[0]);
    exit(1);
  } else if (argc > 2) {
    tentativi = atoi(argv[2]);
    if (6 > tentativi || tentativi > 10)
      tentativi = 6;
  }

  simpleSocket = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);

  if (simpleSocket == -1) {
    fprintf(stderr, "Could not create a socket!\n");
    exit(1);
  } else {
    fprintf(stderr, "Socket created!\n");
  }

  simplePort = atoi(argv[1]);

  memset(&simpleServer, '\0', sizeof(simpleServer));
  simpleServer.sin_family = AF_INET;
  simpleServer.sin_addr.s_addr = htonl(INADDR_ANY);
  simpleServer.sin_port = htons(simplePort);

  returnStatus = bind(simpleSocket, (struct sockaddr *)&simpleServer,
                      sizeof(simpleServer));

  if (returnStatus == 0) {
    fprintf(stderr, "Bind completed!\n");
  } else {
    fprintf(stderr, "Could not bind to address!\n");
    close(simpleSocket);
    exit(1);
  }

  returnStatus = listen(simpleSocket, 5);
  if (returnStatus == -1) {
    fprintf(stderr, "Cannot listen on socket!\n");
    close(simpleSocket);
    exit(1);
  }

  /*leggo le parole possibili dal file*/
  FILE *fptr = NULL;
  fptr = fopen("wordle_words.txt", "r");
  if (fptr == NULL) {
    printf("no such file.");
    exit(1);
  }
  int i=0;
  int row = 1;
  char words[6];
  char wordle[5][6];

  srand(time(NULL));
  int rand_pick = rand()%40+1;

  /*scelgo 5 parole a caso nel file*/
  while (!feof(fptr)){
    fscanf(fptr, "%s", words);
    if(row++ == rand_pick){
      strcpy(wordle[i++],words);
      if(i==5)
        break;
      rand_pick = rand()%40+1;
      row = 1;
    }
  }

  fclose(fptr);

  printf("Lista di parole da cui scegliere:\n");
  for (int i = 0; i < 5; i++) {
    printf("%s ", wordle[i]);
  }
  printf("\n");

  while (1)

  {

    struct sockaddr_in clientName = {0};
    int simpleChildSocket = 0;
    int clientNameLength = sizeof(clientName);

    /* attesa del client */
    simpleChildSocket = accept(simpleSocket, (struct sockaddr *)&clientName, &clientNameLength);

    if (simpleChildSocket == -1) {
      fprintf(stderr, "Cannot accept connections!\n");
      close(simpleSocket);
      exit(1);
    }

    /*invio al client il messaggio di benvenuto: "OK <tentativi> <messaggio>"*/
    memset(buffer, 0, sizeof(buffer));
    snprintf(buffer, sizeof(buffer), "OK %d %s\n", tentativi, MESSAGE);
    write(simpleChildSocket, buffer, strlen(buffer));

    /*scelgo la parola da indovinare*/
    strcpy(target, wordle[rand() % 5]);
    printf("\nLa parola scelta è: %s\n", target);

    /*inizio la comunicazione con il client*/
    int t;
    for (t = 1; t <= tentativi; t++) {
      memset(buffer, 0, sizeof(buffer));
      returnStatus = read(simpleChildSocket, buffer, sizeof(buffer));
      if (returnStatus > 0) {

        if (strncmp("WORD ", buffer, 5) == 0) {         

          /*controllo se il messaggio ricevuto è formattato nel modo corretto*/
          /*se non lo è chiudo la connessione con il client*/         

          char *token;
          token = strtok(buffer, " ");
          token = strtok(NULL, " ");
          if (strlen(token) != 6) {
            memset(buffer, 0, sizeof(buffer));
            snprintf(buffer, sizeof(buffer),
                     "ERR La parola deve essere lunga 5 lettere\n");
            write(simpleChildSocket, buffer, sizeof(buffer));
            t--;
            break;
          }
          char guess[6];
          strncpy(guess, buffer+5, 5);
          guess[5] = '\0';

          int isCorrect = 1;
          for (int i = 0; guess[i] != '\0'; i++)
            if (isalpha(guess[i]) == 0) {
              memset(buffer, 0, sizeof(buffer));
              snprintf(buffer, sizeof(buffer),
                       "ERR La parola può avere solo lettere dell'alfabeto\n");
              write(simpleChildSocket, buffer, sizeof(buffer));
              t--;
              isCorrect = 0;
              break;
            }
          if (isCorrect == 0)
            break;

          printf("%s\n", guess);

          /*se è stata indovinata la porola inviare il messaggio al client e chiudere la connessione*/
          if (strcmp(target, guess) == 0) {
            memset(buffer, 0, sizeof(buffer));
            strcpy(buffer, "OK PERFECT");
            write(simpleChildSocket, buffer, sizeof(buffer));
            printf("Il client ha indovinato la parola!");
            break;
          }

          /*se la parola non è stata indovinata, individuare le lettere corrette*/
          char solution[6] = "";

          for (int i = 0; i < 5; i++) {
            for (int j = 0; j < 5; j++) {
              if (guess[i] == target[j]) {
                if (i == j) {
                  solution[i] = '*';
                } else {
                  if (solution[i] != '*')
                    solution[i] = '+';
                }
              } else {
                if (solution[i] != '*' && solution[i] != '+')
                  solution[i] = '-';
              }
            }
          }
          strcat(solution, "\0");
          printf("%s\n", solution);

          /*se i tentativi sono finiti, inviare messaggio di fine al client 
            e chiudere la connessione*/
          if (t  == tentativi) {
            memset(buffer, 0, sizeof(buffer));
            snprintf(buffer, sizeof(buffer), "END %d %s\n", tentativi, target);
            write(simpleChildSocket, buffer, sizeof(buffer));
            printf("Tentativi esauriti per il client\n");

          } else{
            /*inviare al client il risultato del tentativo*/
            memset(buffer, 0, sizeof(buffer));
            snprintf(buffer, sizeof(buffer), "OK %d %s\n", t, solution);
            write(simpleChildSocket, buffer, sizeof(buffer));
          }
          

          /*se il comando del client è QUIT, inivare il messaggio di risposta al client 
            e chiudere la connessione*/
        } else if (strncmp("QUIT", buffer, 4) == 0) {
          memset(buffer, 0, sizeof(buffer));
          snprintf(buffer, sizeof(buffer), "QUIT Ciao! La parola era: %s\n",
                   target);
          write(simpleChildSocket, buffer, sizeof(buffer));
          printf("Il client ha scelto di chiudere la connessione\n");
          break;

          /*se il comando non è stato riconosciuto, inviare messaggio di errore al client 
            e chiudere la connessione*/
        } else {
          memset(buffer, 0, sizeof(buffer));
          snprintf(buffer, sizeof(buffer), "ERR Comando non riconosciuto\n");
          write(simpleChildSocket, buffer, sizeof(buffer));
          break;
        }
      } else { /*se il server non riesce a leggere i messaggi del client, 
                 chiudere la connessione*/
        fprintf(stderr, "Return Status = %d \n", returnStatus);
        break;
      }
    }
    

    close(simpleChildSocket);
  }
  close(simpleSocket);
  return 0;
}
