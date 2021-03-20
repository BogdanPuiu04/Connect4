#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <errno.h>
#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <netdb.h>
#include <string.h>
#include <arpa/inet.h>
#define black "\e[1;30m"
#define red "\e[1;31m"
#define green "\e[1;32m"
#define yellow "\e[1;33m"
#define blue "\e[1;34m"
#define magenta "\e[1;35m"
#define cyan "\e[1;36m"
#define white "\e[1;37m"
#define reset "\e[0m"

/* codul de eroare returnat de anumite apeluri */

/* portul de conectare la server*/
int port;

void afisare_matrice(int matrice[6][7])
{
  fflush(stdout);
  printf("\n");
  printf("\n");
  printf("\n");
  printf("\n");
  printf("\n");

  printf(white "            １   ２   ３   ４   ５   ６   ７ \n" reset);
  for (int i = 0; i < 6; i++)
  {
    printf("           ");
    for (int j = 0; j < 7; j++)
    {
      if (matrice[i][j] == 0)
      {
        fflush(stdout);
        printf(white "|  | " reset);
      }
      else
      {
        if (matrice[i][j] == 2)
          printf(red "|⬤ | " reset);
        else
          printf(blue "|⬤ | " reset);
        fflush(stdout);
      }
    }
    fflush(stdout);
    printf("\n");
  }
  fflush(stdout);
  printf("\n");
}

int main(int argc, char *argv[])
{
  int matrice[6][7], play = 0, castigator[1], raspuns[1], joc_nou = 0;
  bzero(matrice, sizeof(matrice));
  int sd;
  struct sockaddr_in server;
  char comanda[10] = {0}, c[1], turn[1];

  port = 999;

  if ((sd = socket(AF_INET, SOCK_STREAM, 0)) == -1)
  {
    perror("Eroare la socket().\n");
    return errno;
  }

  server.sin_family = AF_INET;
  server.sin_addr.s_addr = inet_addr("127.0.0.1");
  server.sin_port = htons(port);

  if (connect(sd, (struct sockaddr *)&server, sizeof(struct sockaddr)) == -1)
  {
    perror("Eroare la connect din partea clientului.\n");
    return errno;
  }
  system("clear");

  fflush(stdout);
  printf(white "        Daca sunteti pregatit sa incepeti introduceti 'play'\n" reset);
  printf("\n");
replay:
  joc_nou++;
  if (joc_nou != 1)
    printf(white "           Runda  %d?\n" reset, joc_nou);
  else
    printf(white "           Runda  %d\n" reset, joc_nou);
  read(0, comanda, 10);
  play = 0;
  if (strncmp(comanda, "play", 4) == 0)
    play = 1;
  if (strncmp(comanda, "exit", 4) == 0)
  {
    close(sd);
    exit(0);
  }
  // system("clear");
  if (write(sd, comanda, sizeof(comanda)) <= 0)
  {
    perror("Eroare la write() spre server.\n");
    return errno;
  }
  bzero(c, 10);
  read(sd, c, 1);
  fflush(stdout);
  fflush(stdout);
  while (play)
  {
    //system("clear");
    bzero(castigator, 100);
    read(sd, castigator, sizeof(castigator));
    // printf("castigatorul este %d \n",castigator[0]);
    bzero(matrice, sizeof(matrice));
    read(sd, matrice, sizeof(matrice));
    afisare_matrice(matrice);
    if (castigator[0] == 1 && c[0] == '1')
    {
      printf(green "       Ati castigat\n" reset);
      printf("\n");
      printf(white "Daca doriti sa jucati din nou folositi 'play' sau 'exit' pentru iesire." reset);
      printf("\n");
      fflush(stdout);
      goto replay;
    }
    if (castigator[0] == 1 && c[0] == '0')
    {
      printf(red "       Ati pierdut\n" reset);
      printf("\n");
      printf(white "Daca doriti sa jucati din nou folositi 'play' sau 'exit' pentru iesire." reset);
      printf("\n");
      goto replay;
    }
    if (castigator[0] == 0 && c[0] == '0')
    {
      printf(green "       Ati castigat\n" reset);
      printf("\n");
      printf(white "Daca doriti sa jucati din nou folositi 'play' sau 'exit' pentru iesire." reset);
      printf("\n");
      goto replay;
    }
    if (castigator[0] == 0 && c[0] == '1')
    {
      printf(red "       Ati pierdut\n" reset);
      printf("\n");
      printf(white "Daca doriti sa jucati din nou folositi 'play' sau 'exit' pentru iesire." reset);
      printf("\n");
      goto replay;
    }
    bzero(turn, 1);
    read(sd, turn, 1);
    fflush(stdout);
    fflush(stdout);
    if (c[0] == turn[0])
    {
      printf("Este tura dumneavoastra!\n");
      fflush(stdout);
      bzero(comanda, 100);
      fgets(comanda, 100, stdin);
      sscanf(comanda, "%[^\n]", comanda);
      fflush(stdout);
      write(sd, comanda, 10);
      bzero(comanda, 10);
      read(sd, raspuns, sizeof(raspuns));
      if (raspuns[0] != 0)
      {
        printf("Va rugam introduceti un numar din {1,2,...7}.\n");
        fflush(stdout);
      }
    }
    else
    {
      printf("Nu este tura dumneavoastra \n");
      fflush(stdout);
     // printf(" \n");
      fflush(stdout);
    }
  }
end:

  close(sd);
}