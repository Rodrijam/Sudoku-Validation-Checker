/*
	Author: James Rodriguez
    Project: 3
	File: sudoku.c
	Date: 11/21/19
*/

#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>
#include <pthread.h>
#include <unistd.h> 

void *checkCol(void *vargs);
void *checkRow(void *vargs);
void *checkGrid(void *vargs);

struct sargs{
	int idx;
	int tid;
	char *vals;
	int isError;
};

int main()
{
	char sBoard[9][9];
	int c;
	
	for (int i = 0; i < 9; i++)			//creates sudoku board in a 2D array
		for (int j = 0; j < 9; j++)
		{
			c = getchar();
			if (!isspace(c))
				sBoard[i][j] = c;
			else
				j--;
		}

	char revBoard[9][9];			//creates another 2D array with the fliped sudoku board (for column checking)
	for (int i = 0; i < 9; i++)
		for (int j = 0; j < 9; j++)
			revBoard[i][j] = sBoard[j][i];

	char gridBoard[9][9];			//creates another 2D array with each sudoku board subgrid as a row (for subgrid checking)
	
	for (int k = 0; k < 9; k += 3)
		for (int m = 0; m < 9; m+= 3)
			for (int i = m; i < m+3; i++)
				for (int j = k; j < k+3; j++)
					gridBoard[i][j] = sBoard[i][j];	
	
	pthread_t tid[27];
	
	struct sargs *lineArgs;
	struct sargs *lineArgs2;
	struct sargs *lineArgs3;
	// create check_threads
	int valid = 1;
	
	for (int i = 0; i < 9; i++)
	{	

		//check threads for rows
		lineArgs = malloc(sizeof(struct sargs)); // allocate memory created for row thread
		(*lineArgs).idx = i;
		(*lineArgs).tid = tid[i];
		(*lineArgs).vals = sBoard[i];

		//check threads for columns
		lineArgs2 = malloc(sizeof(struct sargs)); // allocate memory created for column thread
		(*lineArgs2).idx = i;
		(*lineArgs2).tid = tid[9+i];
		(*lineArgs2).vals = revBoard[i];
		
		//check threads for subgrids
		lineArgs3 = malloc(sizeof(struct sargs)); // allocate memory created for subgrid thread
		(*lineArgs3).idx = i;
		(*lineArgs3).tid = tid[18+i];
		(*lineArgs3).vals = gridBoard[i];

		pthread_create(&tid[i], NULL, checkRow, (void *)lineArgs);
		pthread_create(&tid[9+i], NULL, checkCol, (void *)lineArgs2);
		pthread_create(&tid[18+i], NULL, checkGrid, (void *)lineArgs3);
		
		if (valid)
			if (lineArgs->isError || lineArgs2->isError || lineArgs3->isError)
				valid = 0;
		
	}
	
	for (int i = 0; i < 18; i++)
		pthread_join(tid[i], NULL);

	// Free all allocated memory
	free(lineArgs);
	free(lineArgs2);
	free(lineArgs3);
	
	if(valid)
		printf("The input is a valid Sudoku.\n");
	else
		printf("The input is not a valid Sudoku.\n");
	
	return 0; 
}

void *checkRow(void *vargs)
{
	struct sargs* rowArg = (struct sargs*)vargs;	
	int index = rowArg->idx;
	char *rowVals = rowArg->vals;

	char checkChar[9] = {'1', '2', '3', '4', '5', '6', '7', '8', '9'};	
	int t;

	for (int i = 0; i < 9; i++)
	{
		t = rowVals[i] - '0';
		if (checkChar[t-1] == 'x')
		{
			printf("Row %d doesn't have the required values.\n", index + 1);
			rowArg->isError = 1;
			return 0;
		}
		else
			checkChar[t-1] = 'x';
	}
	return 0;
}

void *checkCol(void *vargs)
{

	struct sargs* colArg = (struct sargs*)vargs;
	int index = colArg->idx;
	char *colVals = colArg->vals;

	char checkChar[9] = {'1', '2', '3', '4', '5', '6', '7', '8', '9'};
	int t;
	
	for (int i = 0; i < 9; i++)
	{
		t = colVals[i] - '0';
		if (checkChar[t-1] == 'x')
		{
			printf("Column %d doesn't have the required values.\n", index + 1);
			colArg->isError = 1;
			return 0;
		}
		else
			checkChar[t-1] = 'x';
	}
	return 0;
}

void *checkGrid(void *vargs)
{
	struct sargs* gridArg = (struct sargs*)vargs;
	int index = gridArg->idx;
	char *gridVals = gridArg->vals;
	
	char checkChar[9] = {'1', '2', '3', '4', '5', '6', '7', '8', '9'};
	int t;
	
	for (int i = 0; i < 9; i++)
	{
		t = gridVals[i] - '0';
		if (checkChar[t-1] == 'x')
		{
			switch (index+1)
			{
				case 1:
					printf("The left top subgrid doesn't have the required values.\n");
					break;
				case 2:
					printf("The center top subgrid doesn't have the required values.\n");
					break;
				case 3:
					printf("The right top subgrid doesn't have the required values.\n");
					break;
				case 4:
					printf("The left middle subgrid doesn't have the required values.\n");
					break;
				case 5:
					printf("The center middle subgrid doesn't have the required values.\n");
					break;
				case 6:
					printf("The right middle subgrid doesn't have the required values.\n");
					break;
				case 7:
					printf("The left bottom subgrid doesn't have the required values.\n");
					break;
				case 8:
					printf("The center bottom subgrid doesn't have the required values.\n");
					break;
				case 9:
					printf("The right bottom subgrid doesn't have the required values.\n");
					break;
				default:
					break;
			}
			gridArg->isError = 1;
			return 0;
		}
		else
			checkChar[t-1] = 'x';
	}
	return 0;
}
