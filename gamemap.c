#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <time.h>


#ifdef _WIN32
#include <windows.h>

#define LilDodo Sleep(1);
#define Dodo Sleep(1000);

#else

#define LilDodo sleep(0);
#define Dodo sleep(1);

#endif

#define NBAI 4 //Number of Players

//For Console Only
#define MAPV '.' //Map Void aka empty charac
#define MAPM '=' //Map Mur
#define MAPC 'O' //Map Coin
#define MAPI 'X' //Map Unbreakable Wall
#define MAPE '~' //Map Eau


#define CAREA 'a' //First character of AI


//fonctions de gestion de terminal
void	ClearTerm();
void	Wait();

//My Functions
int	max_rand(int maxvalue);


//Map Functions

int		**new_2d_array(int n); // Create 2D Array (GameMap)

void		print_state(int **map, int n); //Print 2D Array

void		fprint_state(int **map, int n, int *ai_ar_pos, int *ai_ar_act, int turn, int maxturn); //Print players actions to File

void		print_current_state(int **map, int n, int turn, int maxturn); //Print Current State of game

int		place_coin(int **map, int n); // Randomly place Coin in map

int		place_ai(int **map, int n, int ai_nb); // Randomly place AI in map

int		get_ai_pos(int **map, int n, int ai_nb); //Get AI pos with AI number and Return crypted pos

int		ai_action(int **map, int n, int ai_t, int *ai_ar_val, int *ai_ar_pos, int action); //Convert IA return nb to action

int		start_game(int **map, int n, int maxturn); //Start game



//Variables Globales
int	Scores[NBAI];
int	my_ai_pos[2];



//scanf

//Define Function pointer for AIs

int		(*ai_func_tab[NBAI])(int **map, int n, int me);

//AI Test

int		noob_iaa(int **map, int n, int me)
{
	int test;
	return(max_rand(8) + 1);
}

int		noob_iab(int **map, int n, int me)
{
	map=map;
	n=n;
	me=me;

	return(max_rand(8) + 1);
}


int		noob_iac(int **map, int n, int me)
{
	map=map;
	n=n;
	me=me;

	return(max_rand(8) + 1);
}


int		noob_iad(int **map, int n, int me)
{
	map=map;
	n=n;
	me=me;

	return(max_rand(8) + 1);
}


//
//
//
int		**new_2d_array(int n)
{
	if (n <= 0)
		return (NULL);


	int	**tab;
	int	x = 0;
	int	y = 0;

	//Give a Precalculated Map if size == 8
	if (n == 8)
	{
		int maphuit[8][8] = {
			{MAPV,MAPV,MAPV,MAPV,MAPV,MAPV,MAPV,MAPV},
			{MAPV,MAPI,MAPE,MAPM,MAPM,MAPE,MAPI,MAPV},
			{MAPV,MAPE,MAPE,MAPV,MAPV,MAPE,MAPE,MAPV},
			{MAPV,MAPM,MAPV,MAPV,MAPV,MAPV,MAPM,MAPV},
			{MAPV,MAPM,MAPV,MAPV,MAPV,MAPV,MAPM,MAPV},
			{MAPV,MAPE,MAPE,MAPV,MAPV,MAPE,MAPE,MAPV},
			{MAPV,MAPI,MAPE,MAPM,MAPM,MAPE,MAPI,MAPV},
			{MAPV,MAPV,MAPV,MAPV,MAPV,MAPV,MAPV,MAPV}};

		tab = malloc(n * sizeof(int *));

		while (x < n)
		{
			tab[x] = malloc(n * sizeof(int));
			while (y < n)
			{
				tab[x][y] = maphuit[x][y];
				y++;
			}
			y = 0;
			x++;
		}

		return (tab);
	}

	x = 0;
	y = 0;

	tab = malloc(n * sizeof(int *));

	while (x < n)
	{
		tab[x] = malloc(n * sizeof(int));

		while (y < n)
		{
			if(x <n/10 || y < n/10 || x>=n-n/10 || y >= n-n/10 ||
					((x >= n/10 && x <= n-n/10-1) && (y > n/10 && y < n-n/10-1)))
			{
				tab[x][y] = MAPV; //Vide
			}
			else
			{
				tab[x][y] = MAPM; //Mur
			}


			y++;
		}
		y = 0;
		x++;
	}

	return (tab);
}


//
//
//
void		print_state(int **map, int n)
{
	int	x;
	int	y;

	char retrn = '\n';
	char spce = ' ';

	x = 0;
	y = 0;

	while (x < n)
	{
		while (y < n)
		{
			write(1, &map[x][y], 1);
			write(1, &spce, 1);
			y++;
		}
		write(1, &retrn, 1);
		y = 0;
		x++;
	}
}


//
//
//
void		fprint_state(int **map, int n, int *ai_ar_pos, int *ai_ar_act, int turn, int maxturn)
{
	FILE	*fp;
	int	x;
	int	y;

	do
	{
		fp = fopen("map.txt", "w");
	} while (fp == NULL); //Secure print in case UE4 is reading file

	x = 0;
	y = 0;

	fprintf(fp, "%d %d %d %d\n", NBAI, n, turn, maxturn);

	while (x < NBAI)
	{
		fprintf(fp, "%d %d %d %d\n", y = ai_ar_pos[x] % n, (ai_ar_pos[x] - y) / n, ai_ar_act[x], Scores[x]);
		x++;
	}

	x = 0;	
	y = 0;

	while (x < n)
	{

		while (y < n)
		{
			fprintf(fp, "%d ", map[x][y]);
			y++;
		}

		fprintf(fp, "\n");
		y = 0;
		x++;
	}

	fclose(fp);
}


//
//
//
void		print_current_state(int **map, int n, int turn, int maxturn)
{
	printf("Tour : %d/%d\n\n", turn, maxturn);

	print_state(map, n);
}


//
//
//
int		place_coin(int **map, int n)
{
	int	setuppos = 10; //Will try placing 10 times
	int	x;
	int	y;

	while (setuppos--) //Reduce every time AI isn't placed
	{
		x = max_rand(n);
		y = max_rand(n);

		if (map[x][y] == MAPV)
		{
			map[x][y] = MAPC;

			return (1);
		}
	}
	return (-1);
}


//
//
//
int		place_ai(int **map, int n, int ai_nb)
{
	int	setuppos = 10; //Will try placing 10 times
	int	x;
	int	y;

	while (setuppos--) //Reduce every time AI isn't placed
	{
		x = max_rand(n);
		y = max_rand(n);

		if (map[x][y] == MAPV)
		{
			map[x][y] = ai_nb;

			return (x * n + y);
		}
	}
	return (-1);
}


//
//
//
int		get_ai_pos(int **map, int n, int ai_nb)
{
	int	x;
	int	y;

	x = 0;
	y = 0;

	while (x < n)
	{

		while (y < n)
		{
			if (map[x][y] == ai_nb)
				return(x * n + y);
			y++;
		}
		y = 0;
		x++;
	}
	return (-1);
}


//
//
//
int		ai_action(int **map, int n, int ai_t, int *ai_ar_val, int *ai_ar_pos, int action)
{
	int x;
	int y;
	int trash;

	trash = 0;
	y = (ai_ar_pos[ai_t] % n);
	x = (ai_ar_pos[ai_t] - y) / n;

	switch(action)
	{
		case 0:
			return (0);
			break;

		case 1: //Move Up

			if (x - 1 >= 0) //Check if going out of array
			{
				if (map[x-1][y] == MAPV || map[x-1][y] == MAPC)
				{
					if (map[x-1][y] == MAPC)
					{
						place_coin(map, n);
						Scores[ai_t] = Scores[ai_t] + 1;
					}

					map[x][y] = MAPV; //Set position to empty
					map[x-1][y] = ai_ar_val[ai_t];
					ai_ar_pos[ai_t] = (x-1) * n + y;
				}
			}
			
			return (1);
			break;

		case 2: //Move Down
			
			if (x + 1 < n) //Check if going out of array
			{
				if (map[x+1][y] == MAPV || map[x+1][y] == MAPC)
				{
					if (map[x+1][y] == MAPC)
					{
						place_coin(map, n);
						Scores[ai_t] = Scores[ai_t] + 1;
					}

					map[x][y] = MAPV; //Set position to empty
					map[x+1][y] = ai_ar_val[ai_t];
					ai_ar_pos[ai_t] = (x+1) * n + y;
				}
			}

			return (2);
			break;

		case 3: //Move Left
			
			if (y - 1 >= 0) //Check if going out of array
			{
				if (map[x][y-1] == MAPV || map[x][y-1] == MAPC)
				{
					if (map[x][y-1] == MAPC)
					{
						place_coin(map, n);
						Scores[ai_t] = Scores[ai_t] + 1;
					}

					map[x][y] = MAPV; //Set position to empty
					map[x][y-1] = ai_ar_val[ai_t];
					ai_ar_pos[ai_t] = x * n + (y-1);
				}
			}

			return (3);
			break;

		case 4: //Move Right
			
			if (y + 1 < n) //Check if going out of array
			{
				if (map[x][y+1] == MAPV || map[x][y+1] == MAPC)
				{
					if (map[x][y+1] == MAPC)
					{
						place_coin(map, n);
						Scores[ai_t] = Scores[ai_t] + 1;
					}

					map[x][y] = MAPV; //Set position to empty
					map[x][y+1] = ai_ar_val[ai_t];
					ai_ar_pos[ai_t] = x * n + (y+1);
				}
			}

			return (4);
			break;

		case 5: //Shoot Up

			while (--x >= 0)
			{
				if (map[x][y] == MAPI)
				{
					return (5);
				}
				else if (map[x][y] == MAPM)
				{
					map[x][y] = MAPV;
					return (5);
				}
				else if (map[x][y] >= CAREA && map[x][y] <= CAREA + 30) //If is player (carac players between a and z)
				{
					while (trash++ < NBAI)
					{
						if (ai_ar_val[trash] == map[x][y]) //Search for player id
						{
							Scores[ai_t] = Scores[ai_t] + 3;
							map[x][y] = MAPV;
							//ai_ar_pos[trash] = -20 - trash; //Uncomment and AI Die
							ai_ar_pos[trash] = place_ai(map, n, CAREA + trash);

							return (5);
						}
					}
				}

			}

			return (5);
			break;

		case 6: //Shoot Down

			while (++x < n)
			{
				if (map[x][y] == MAPI)
				{
					return (5);
				}
				else if (map[x][y] == MAPM)
				{
					map[x][y] = MAPV;
					return (6);
				}
				else if (map[x][y] >= CAREA && map[x][y] <= CAREA + 30) //If is player (carac players between a and z)
				{
					while (trash++ < NBAI)
					{
						if (ai_ar_val[trash] == map[x][y]) //Search for player id
						{
							Scores[ai_t] = Scores[ai_t] + 3;
							map[x][y] = MAPV;
							//ai_ar_pos[trash] = -20 - trash; //Uncomment and AI Die
							ai_ar_pos[trash] = place_ai(map, n, CAREA + trash);

							return (6);
						}
					}
				}

			}

			return (6);
			break;

		case 7: //Shoot Left

			while (--y >= 0)
			{

				if (map[x][y] == MAPI)
				{
					return (5);
				}
				else if (map[x][y] == MAPM)
				{
					map[x][y] = MAPV;
					return (7);
				}
				else if (map[x][y] >= CAREA && map[x][y] <= CAREA + 30) //If is player (carac players between a and z)
				{
					while (trash++ < NBAI)
					{
						if (ai_ar_val[trash] == map[x][y]) //Search for player id
						{
							Scores[ai_t] = Scores[ai_t] + 3;
							map[x][y] = MAPV;
							//ai_ar_pos[trash] = -20 - trash; //Uncomment and AI Die
							ai_ar_pos[trash] = place_ai(map, n, CAREA + trash);

							return (7);
						}
					}
				}
			}

			return (7);
			break;

		case 8: //Shoot Right

			while (++y < n)
			{

				if (map[x][y] == MAPI)
				{
					return (5);
				}
				else if (map[x][y] == MAPM)
				{
					map[x][y] = MAPV;
					return (8);
				}
				else if (map[x][y] >= CAREA && map[x][y] <= CAREA + 30) //If is player (carac players between a and z)
				{
					while (trash++ < NBAI)
					{
						if (ai_ar_val[trash] == map[x][y]) //Search for player id
						{
							Scores[ai_t] = Scores[ai_t] + 3;
							map[x][y] = MAPV;
							//ai_ar_pos[trash] = -20 - trash; //Uncomment and AI Die
							ai_ar_pos[trash] = place_ai(map, n, CAREA + trash);

							return (8);
						}
					}
				}
			}

			return (8);
			break;

	}
	return(-1); //Action does not exist
}





int		start_game(int **map, int n, int maxturn)
{
	FILE	*fwait;
	int	*ai_ar_val;
	int	*ai_ar_pos;
	int	*ai_ar_act;

	int	turn = 0;
	int	ai_t = 0;
	int	trash = 0;



	ai_ar_val = malloc(NBAI * sizeof(int));
	ai_ar_pos = malloc(NBAI * sizeof(int));
	ai_ar_act = malloc(NBAI * sizeof(int));


	ai_func_tab[0] = noob_iaa;
	ai_func_tab[1] = noob_iab;
	ai_func_tab[2] = noob_iac;
	ai_func_tab[3] = noob_iad;


	while (ai_t < NBAI)
	{
		ai_ar_val[ai_t] = CAREA + ai_t;
		ai_ar_pos[ai_t] = place_ai(map, n, CAREA + ai_t);

		place_coin(map, n);

		ai_t++;

	}
	ai_t = 0;

	//Use txt file to prepare Unreal Engine
	fwait = fopen("map.txt", "w");
	ClearTerm();
	//fprint_state(map, n, ai_ar_pos, turn, maxturn);
	//print_current_state(map, n, turn, maxturn);

	Wait();
	fclose(fwait);

	while (turn <= maxturn)
	{
		ClearTerm();

		while (ai_t < NBAI)
		{
			if (ai_ar_pos[ai_t] >= 0)
			{
				my_ai_pos[0] = ai_ar_pos[ai_t] % n;
				my_ai_pos[1] = (ai_ar_pos[ai_t] - my_ai_pos[0]) / n;

				printf("Action : %d\n", ai_ar_act[ai_t] = ai_action(map, n, ai_t, ai_ar_val, ai_ar_pos, (*ai_func_tab[ai_t])(map, n, ai_ar_val[ai_t])));
				printf("IA%d : %d  Score : %d\t%d : %d\n", ai_t, ai_ar_val[ai_t], Scores[ai_t], my_ai_pos[0], my_ai_pos[1]);
			}
			else
			{
				printf("IA%d est mort\n\n", ai_t);
				ai_ar_act[ai_t] = -1;
			}

			fprint_state(map, n, ai_ar_pos, ai_ar_act, turn, maxturn); 

			//print_current_state(map, n, turn, maxturn);
			//Wait();

			//Comment for better use in Console
			//Dodo(1000);

			ai_t++;
		}
		ai_t = 0;

		print_current_state(map, n, turn, maxturn);

		turn++;

		//Remove Comment to use in console
		Wait();
	}

	return (0);
}




int		main(int argc, char **argv)
{
	//Base Value
	int	taillemap = 8;
	int	maxturn = 30;

	srand(time(NULL));
	
	//Check If Parameters are given (1: map size, 2: Nb of turn
	if (argv[1] != NULL)
		taillemap = atoi(argv[1]);

	if (argv[1] != NULL && argv[2] != NULL)
		maxturn = atoi(argv[2]);


	int **map = new_2d_array(taillemap);

	start_game(map, taillemap, maxturn);

	return (0);
}





#ifdef _WIN32

void ClearTerm()
{
	system("cls");
}

void Wait()
{
	system("pause");
}

#else

void ClearTerm()
{
	printf("\033[H\033[J\x1b[0m");
}

void Wait()
{
	getchar();
}
#endif

int	max_rand(int maxvalue)
{
	int x;

	x = (int) rand() % maxvalue;

	return(x);
}
