#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <errno.h>
#include <unistd.h>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <arpa/inet.h>
#include <sys/wait.h>
#define PORT 999

void add_piesa(int matrice[6][7], int j, char turn[1])
{
	int i = 0;
	while (matrice[i][j] == 0 && i < 6)
	{
		i++;
	}

	i--;
	if (turn[0] == '1')
		matrice[i][j] = 2;
	else
		matrice[i][j] = 3;
}

int interior(int x, int y)
{
	return (x >= 0 && x <= 5 && y <= 6 && y >= 0);
}

int verifica_castigator(int matrice[6][7])
{
	for (int i = 0; i < 6; i++)
	{
		for (int j = 0; j < 7; j++)
		{
			if (interior(i, j) && interior(i + 1, j) && interior(i + 2, j) && interior(i + 3, j))
			{
				if (matrice[i][j] == 2 && matrice[i + 1][j] == 2 && matrice[i + 2][j] == 2 && matrice[i + 3][j] == 2)
					return 1;
				if (matrice[i][j] == 3 && matrice[i + 1][j] == 3 && matrice[i + 2][j] == 3 && matrice[i + 3][j] == 3)
					return 0;
			}
			if (interior(i, j) && interior(i, j + 1) && interior(i, j + 2) && interior(i, j + 3))
			{
				if (matrice[i][j] == 2 && matrice[i][j + 1] == 2 && matrice[i][j + 2] == 2 && matrice[i][j + 3] == 2)
					return 1;
				if (matrice[i][j] == 3 && matrice[i][j + 1] == 3 && matrice[i][j + 2] == 3 && matrice[i][j + 3] == 3)
					return 0;
			}
			if (interior(i, j) && interior(i + 1, j + 1) && interior(i + 2, j + 2) && interior(i + 3, j + 3))
			{
				if (matrice[i][j] == 2 && matrice[i + 1][j + 1] == 2 && matrice[i + 2][j + 2] == 2 && matrice[i + 3][j + 3] == 2)
					return 1;
				if (matrice[i][j] == 3 && matrice[i + 1][j + 1] == 3 && matrice[i + 2][j + 2] == 3 && matrice[i + 3][j + 3] == 3)
					return 0;
			}
			if (interior(i, j) && interior(i + 1, j - 1) && interior(i + 2, j - 2) && interior(i + 3, j - 3))
			{
				if (matrice[i][j] == 2 && matrice[i + 1][j - 1] == 2 && matrice[i + 2][j - 2] == 2 && matrice[i + 3][j - 3] == 2)
					return 1;
				if (matrice[i][j] == 3 && matrice[i + 1][j - 1] == 3 && matrice[i + 2][j - 2] == 3 && matrice[i + 3][j - 3] == 3)
					return 0;
			}
		}
	}
	return -1;
}
void procesare_mutare(char mutare[1], char turn[1], int matrice[6][7])
{
	int mutarea, i;
	switch (mutare[0])
	{
	case '1':
	{
		mutarea = 0;
		fflush(stdout);
		add_piesa(matrice, mutarea, turn);
		//	printf("mutarea este %d \n",mutarea);
		break;
	}
	case '2':
	{
		mutarea = 1;
		fflush(stdout);
		add_piesa(matrice, mutarea, turn);
		//printf("mutarea este %d\n",mutarea);
		break;
	}
	case '3':
	{
		mutarea = 2;
		fflush(stdout);
		add_piesa(matrice, mutarea, turn);
		//printf("mutarea este %d \n",mutarea);
		break;
	}
	case '4':
	{
		mutarea = 3;
		fflush(stdout);
		add_piesa(matrice, mutarea, turn);
		//printf("mutarea este %d \n",mutarea);
		break;
	}
	case '5':
	{
		mutarea = 4;
		fflush(stdout);
		add_piesa(matrice, mutarea, turn);
		//printf("mutarea este %d \n",mutarea);
		break;
	}
	case '6':
	{
		mutarea = 5;
		fflush(stdout);
		add_piesa(matrice, mutarea, turn);
		//	printf("mutarea este %d \n",mutarea);
		break;
	}
	case '7':
	{
		mutarea = 6;
		fflush(stdout);
		add_piesa(matrice, mutarea, turn);
		//	printf("mutarea este %d \n",mutarea);
		break;
	}
	}
}
bool legit_move(char move[10])
{
	if (strlen(move) < 2 && (move[0] == '1' || move[0] == '2' || move[0] == '3' || move[0] == '4') || move[0] == '5' || move[0] == '6' || move[0] == '7')
		return true;
	return false;
}
int main()
{
	int matrice[6][7];
	struct sockaddr_in server;
	struct sockaddr_in from;
	struct sockaddr_in from2;
	int sd, sd2;
	if ((sd = socket(AF_INET, SOCK_STREAM, 0)) == -1)
	{
		perror("Eroare la socketul nr 1.\n");
		return errno;
	}
	if ((sd2 = socket(AF_INET, SOCK_STREAM, 0)) == -1)
	{
		perror("Eroare la socketul nr 2.\n");
		return errno;
	}

	bzero(&server, sizeof(server));
	bzero(&from, sizeof(from));
	bzero(&from2, sizeof(from2));

	server.sin_family = AF_INET;
	server.sin_addr.s_addr = htonl(INADDR_ANY);
	server.sin_port = htons(999);

	if (bind(sd, (struct sockaddr *)&server, sizeof(struct sockaddr)) == -1)
	{
		perror("Eroarea bind-ului.\n");
		return errno;
	}

	if (listen(sd, 1) == -1)
	{
		perror("Eroare la listen.\n");
		return errno;
	}

	while (1)
	{
		int client, client2, play = 0, nrExit = 0, joc_nou = 0;
		int player1Turn, player2Turn, castigator[1], raspuns[1];
		unsigned int length = sizeof(from);
		unsigned int length2 = sizeof(from2);
		int nrjucatori = 0;
		char comanda1[10] = {0}, comanda2[10] = {0}, turn[1], command[1];
		printf("Se astepta clienti la portul %d...\n", PORT);
		fflush(stdout);

		client = accept(sd, (struct sockaddr *)&from, &length);

		if (client < 0)
		{
			perror("Eroare la acceptul clientului 1.\n");
			continue;
		}

		client2 = accept(sd, (struct sockaddr *)&from, &length);
		if (client2 < 0)
		{
			perror("Eroare la acceptul clientului 2.\n");
			continue;
		}
		int pid;
		if ((pid = fork()) == -1)
		{
			close(client);
			close(client2);
			continue;
		}
		else if (pid > 0)

		{
			//parintele
			close(client);
			close(client2);
			while (waitpid(-1, NULL, WNOHANG))
				;
			continue;
		}
		else if (pid == 0)
		{
			//inceperea jocului
			close(sd);

		replay:
			bzero(comanda1, 10);
			bzero(comanda2, 10);
			bzero(matrice, sizeof(matrice));
			for (int i = 0; i < 6; i++)
				for (int j = 0; j < 7; j++)
					matrice[i][j] = 0;
			read(client, comanda1, 10);
			read(client2, comanda2, 10);
			if (strncmp(comanda1, "exit", 4) == 0 || strncmp(comanda2, "exit", 4) == 0)
			{
				close(client);
				close(client2);
			}
			if (strncmp(comanda1, "play", 4) == 0 && strncmp(comanda2, "play", 4) == 0)
				play = 1;
			if (play == 1)
			{
				write(client, "1", 1);
				write(client2, "0", 1);
				turn[0] = '1';
				while (play)
				{
					castigator[0] = verifica_castigator(matrice);
					//printf("castigatorul este %d \n",castigator);
					write(client, castigator, sizeof(castigator));
					write(client2, castigator, sizeof(castigator));
					write(client, matrice, sizeof(matrice));
					write(client2, matrice, sizeof(matrice));
					if (castigator[0] != -1)
						break;
					write(client, turn, 1);
					write(client2, turn, 1);
					if (turn[0] == '1')
					{
						bzero(comanda1, 10);
						bzero(comanda2, 10);
						read(client, comanda1, 10);
						comanda1[strlen(comanda1)] = '\0';
						fflush(stdout);
						printf("%s comanda de la clientul 1 \n", comanda1);
						fflush(stdout);
						if (legit_move(comanda1))
						{
							command[0] = comanda1[0];
							procesare_mutare(command, turn, matrice);
							fflush(stdout);
							raspuns[0] = 0;
							write(client, raspuns, sizeof(raspuns));
							turn[0] = '0';
						}
						else
						{
							raspuns[0] = 1;
							write(client, raspuns, sizeof(raspuns));
						}
					}
					else
					{
						bzero(comanda1, 10);
						bzero(comanda2, 10);
						read(client2, comanda1, 10);
						comanda1[strlen(comanda1)] = '\0';
						fflush(stdout);
						printf("%s comanda de la clientul 2 \n", comanda1);
						fflush(stdout);
						if (legit_move(comanda1))
						{
							command[0] = comanda1[0];
							procesare_mutare(command, turn, matrice);
							fflush(stdout);
							raspuns[0] = 0;
							write(client2, raspuns, sizeof(raspuns));
							turn[0] = '1';
						}
						else
						{
							raspuns[0] = 1;
							write(client2, raspuns, sizeof(raspuns));
						}
					}
				}
			}
			//meci terminat. reluarea jocului
			play = 0;
			goto replay;
			close(client);
			close(client2);
			exit(0);
		}
	}
}