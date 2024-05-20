#include <stdio.h>
#include <stdio.h>
#include <SDL.h>
#include "./constants.h"
#include <stdlib.h>
#include <SDL_image.h>
#include <windows.h>
#include <time.h>



//global variables
SDL_Window* window = NULL;
SDL_Renderer* renderer = NULL;
int game_running = FALSE;
int last_frame_time = 0;
int number_of_rows = window_height / length_of_tile;
int number_of_cols = window_width / length_of_tile;
int turn = 1;





SDL_Texture* loadTexture(SDL_Renderer* renderer, const char* imagePath) {
	SDL_Surface* surface = IMG_Load(imagePath);
	if (!surface) {
		printf("IMG_Load: %s\n", IMG_GetError());
		return NULL;
	}

	SDL_Texture* texture = SDL_CreateTextureFromSurface(renderer, surface);
	SDL_FreeSurface(surface);

	return texture;
}



//structures
struct player
{
	int current_position_on_tile;
	int position_x;
	int position_y;


} player1, player2;

struct tile
{
	int position_x;
	int position_y;
} tiles[10][10];



//list of functions
int initialize_window(void);
void process_input();
void update();
void render();
void destroy_window();
void setup();
void movement_player1(int dice);
void movement_player2(int dice);






int main(int argc, char* args[])
{
	game_running = initialize_window();
	setup();
	while (game_running)
	{
		process_input();
		update();
		render();
	}

	destroy_window();
	return 0;

}


int initialize_window(void)
{

	if (SDL_Init(SDL_INIT_EVERYTHING) != 0)
	{
		fprintf(stderr, "Error initializing SDL\n");
		return FALSE;
	}
	window = SDL_CreateWindow(
		"Snakes&Ladders",     //title
		SDL_WINDOWPOS_CENTERED,  //x position
		SDL_WINDOWPOS_CENTERED,  // y position
		window_width,
		window_height,
		SDL_WINDOW_MAXIMIZED
	);


	if (!window)
	{
		fprintf(stderr, "Error initializing SDL window\n");
		return FALSE;
	}

	renderer = SDL_CreateRenderer(
		window,
		-1,
		SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC
	);

	if (!renderer)
	{
		fprintf(stderr, "Error initializing SDL renderer\n");
		return FALSE;
	}
	return TRUE;
}

void process_input()
{
	SDL_Event event;
	SDL_PollEvent(&event);

	switch (event.type)
	{
	case SDL_QUIT:
		game_running = FALSE;
		break;
	case SDL_KEYDOWN:
		if (event.key.keysym.sym == SDLK_ESCAPE)
			game_running = FALSE;
		if (event.key.keysym.sym == SDLK_RETURN)
		{
			srand(time(NULL));
			int dice = ((rand() % 6) + 1);
			printf("dice rolled a %d\n", dice);
			if (turn == 1)
			{
				movement_player1(dice);
				if (dice == 6)
				{
					printf("Another turn for player 1\n");
					srand(time(NULL));
					dice = ((rand() % 6) + 1);
					printf("dice rolled a %d\n", dice);
					movement_player1(dice);
					if (dice == 6)
					{
						printf("Another turn for player 1\n");
						srand(time(NULL));
						dice = ((rand() % 6) + 1);
						printf("dice rolled a %d\n", dice);
						movement_player1(dice);
						if (dice == 6)
							printf("Player 1 rolled a 6 3 consecutive times, another turn will not be given\n");
					}
				}

				turn = turn * -1;
			}
			else if (turn == -1)
			{
				movement_player2(dice);
				if (dice == 6)
				{
					printf("Another turn for player 2\n");
					dice = ((rand() % 6) + 1);
					printf("dice rolled a %d\n", dice);
					movement_player2(dice);
					if (dice == 6)
					{
						printf("Another turn for player 2\n");
						dice = ((rand() % 6) + 1);
						printf("dice rolled a %d\n", dice);
						movement_player2(dice);
						if (dice == 6)
							printf("Player 2 rolled a 6 3 consecutive times, another turn will not be given\n");
					}
				}
				turn = turn * -1;
			}
		}
		break;
	}
}

void update()
{
	float delta_time = (SDL_GetTicks() - last_frame_time) / 1000.0f;
	last_frame_time = SDL_GetTicks();


}

void render()
{
	SDL_Surface* background = SDL_LoadBMP("./snakes&ladders.bmp");

	SDL_Texture* texture = SDL_CreateTextureFromSurface(renderer, background);
	SDL_SetRenderDrawColor(renderer, 255, 165, 0, 255);
	SDL_RenderClear(renderer);
	SDL_RenderCopy(renderer, texture, NULL, NULL);
	//to add colour to the window

	//creating sprite for player 1
	SDL_Rect player =
	{
		player1.position_x,
		player1.position_y,
		length_of_tile / 2,
		length_of_tile / 2
	};
	SDL_Rect playerb =
	{
		player2.position_x,
		player2.position_y,
		length_of_tile / 2,
		length_of_tile / 2
	};
	SDL_Texture* player1Sprite = loadTexture(renderer, "./player1.png");
	SDL_Texture* player2Sprite = loadTexture(renderer, "./player2.png");


	//creating a rectangle to represent player 1

	//SDL_RenderCopy(renderer, player1Sprite, NULL, &player);
	SDL_RenderCopy(renderer, player1Sprite, NULL, &player);

	SDL_RenderCopy(renderer, player2Sprite, NULL, &playerb);


	//code to initialize renderer
	SDL_RenderPresent(renderer);
	SDL_DestroyTexture(player1Sprite); //destroy the sprite of player 1
	SDL_DestroyTexture(player2Sprite); //destroy the sprite of player 2
	SDL_DestroyTexture(texture);
	SDL_FreeSurface(background);
}

void setup()
{


	//setting locations and indices of tiles
	for (int i = 0; i < number_of_cols; i++)
	{
		for (int j = 0; j < number_of_rows; j++)
		{
			if (i % 2 == 0)
			{
				tiles[i][j].position_x = (length_of_tile / 2) + (j * length_of_tile);
				tiles[i][j].position_y = (window_width - length_of_tile / 2) - (i * length_of_tile);
			}
			else
			{
				tiles[i][j].position_x = (window_height - length_of_tile / 2) - (j * length_of_tile);
				tiles[i][j].position_y = (window_width - length_of_tile / 2) - (i * length_of_tile);
			}
			//printf("the position of the tiles[%d][%d] are %d , %d\n", i, j, tiles[i][j].position_x, tiles[i][j].position_y);


		}
	}


	//setting initial position of players
	player1.current_position_on_tile = 0;
	player1.position_x = tiles[0][0].position_x - (length_of_tile / 200);
	player1.position_y = tiles[0][0].position_y - (length_of_tile / 4);
	player2.current_position_on_tile = 0;
	player2.position_x = tiles[0][0].position_x - (length_of_tile / 2.5);
	player2.position_y = tiles[0][0].position_y - (length_of_tile / 4);
}

void destroy_window()
{
	SDL_DestroyRenderer(renderer);
	SDL_DestroyWindow(window);
	SDL_Quit();
}

void movement_player1(int dice)
{
	if (player1.current_position_on_tile == (number_of_cols * number_of_rows) - 1)
	{
		printf("THE PLAYER 1 HAS WON THE GAME\n");
		game_running = FALSE;
		return;
	}
	else {

		if ((dice + player1.current_position_on_tile) >= (number_of_cols * number_of_rows))
		{
			dice = (number_of_cols * number_of_rows) - player1.current_position_on_tile - 1;
			printf("dice roll changed to %d\n", dice);
		}
		for (int k = 1; k <= dice; k++) {
			player1.current_position_on_tile += 1;
			int i = player1.current_position_on_tile / number_of_cols;
			int j = player1.current_position_on_tile - (number_of_rows * i);
			if (j > (number_of_cols - 1)) {
				player1.position_x = tiles[i + 1][j].position_x - (length_of_tile / 200);
				player1.position_y = tiles[i + 1][j].position_y - (length_of_tile / 4);
			}
			else {
				player1.position_x = tiles[i][j].position_x - (length_of_tile / 200);
				player1.position_y = tiles[i][j].position_y - (length_of_tile / 4);
			}
			render();
			Sleep(300);


		}


		printf("player 1 index increased to %d\n", player1.current_position_on_tile + 1);

		//adding some snakes
		//lets put the snakes mouth at indices 28 -> 10, 37 -> 3 , 47 -> 16 , 75 -> 32 , 96 -> 42 , 94 -> 71 
		if (player1.current_position_on_tile == 27)
		{
			printf("PLAYER 1 WAS BITTEN BY A SNAKE AND MOVED FROM 28 TO POSITION 10\n");
			player1.position_x = tiles[0][9].position_x - (length_of_tile / 200);
			player1.position_y = tiles[0][9].position_y - (length_of_tile / 4);
			player1.current_position_on_tile = 9;
		}
		if (player1.current_position_on_tile == 36)
		{
			printf("PLAYER 1 WAS BITTEN BY A SNAKE AND MOVED FROM 37 TO POSITION 3\n");
			player1.position_x = tiles[0][2].position_x - (length_of_tile / 200);
			player1.position_y = tiles[0][2].position_y - (length_of_tile / 4);
			player1.current_position_on_tile = 2;
		}
		if (player1.current_position_on_tile == 46)
		{
			printf("PLAYER 1 WAS BITTEN BY A SNAKE AND MOVED FROM 47 TO POSITION 16\n");
			player1.position_x = tiles[1][5].position_x - (length_of_tile / 200);
			player1.position_y = tiles[1][5].position_y - (length_of_tile / 4);
			player1.current_position_on_tile = 15;
		}
		if (player1.current_position_on_tile == 74)
		{
			printf("PLAYER 1 WAS BITTEN BY A SNAKE AND MOVED FROM 75 TO POSITION 32\n");
			player1.position_x = tiles[3][1].position_x - (length_of_tile / 200);
			player1.position_y = tiles[3][1].position_y - (length_of_tile / 4);
			player1.current_position_on_tile = 31;
		}
		if (player1.current_position_on_tile == 95)
		{
			printf("PLAYER 1 WAS BITTEN BY A SNAKE AND MOVED FROM 96 TO POSITION 42\n");
			player1.position_x = tiles[4][1].position_x - (length_of_tile / 200);
			player1.position_y = tiles[4][1].position_y - (length_of_tile / 4);
			player1.current_position_on_tile = 41;
		}
		if (player1.current_position_on_tile == 93)
		{
			printf("PLAYER 1 WAS BITTEN BY A SNAKE AND MOVED FROM 94 TO POSITION 71\n");
			player1.position_x = tiles[7][0].position_x - (length_of_tile / 200);
			player1.position_y = tiles[7][0].position_y - (length_of_tile / 4);
			player1.current_position_on_tile = 70;
		}

		//adding some ladders
		//lets put the ladder start at indices 4 -> 56 , 14 -> 55 , 54 -> 88 , 12 -> 50 , 22 -> 58 , 41 -> 79
		if (player1.current_position_on_tile == 3)
		{
			printf("PLAYER 1 TOOK A LADDER AND CLIMED FROM 4 TO 56\n");
			player1.position_x = tiles[5][5].position_x - (length_of_tile / 200);
			player1.position_y = tiles[5][5].position_y - (length_of_tile / 4);
			player1.current_position_on_tile = 55;
		}
		if (player1.current_position_on_tile == 13)
		{
			printf("PLAYER 1 TOOK A LADDER AND CLIMED FROM 14 TO 55\n");
			player1.position_x = tiles[5][4].position_x - (length_of_tile / 200);
			player1.position_y = tiles[5][4].position_y;
			player1.current_position_on_tile = 54;
		}
		if (player1.current_position_on_tile == 53)
		{
			printf("PLAYER 1 TOOK A LADDER AND CLIMED FROM 54 TO 88\n");
			player1.position_x = tiles[8][7].position_x - (length_of_tile / 200);
			player1.position_y = tiles[8][7].position_y - (length_of_tile / 4);
			player1.current_position_on_tile = 87;
		}
		if (player1.current_position_on_tile == 11)
		{
			printf("PLAYER 1 TOOK A LADDER AND CLIMED FROM 12 TO 50\n");
			player1.position_x = tiles[4][9].position_x - (length_of_tile / 200);
			player1.position_y = tiles[4][9].position_y;
			player1.current_position_on_tile = 49;
		}
		if (player1.current_position_on_tile == 21)
		{
			printf("PLAYER 1 TOOK A LADDER AND CLIMED FROM 22 TO 58\n");
			player1.position_x = tiles[5][7].position_x - (length_of_tile / 200);
			player1.position_y = tiles[5][7].position_y - (length_of_tile / 4);
			player1.current_position_on_tile = 57;
		}
		if (player1.current_position_on_tile == 40)
		{
			printf("PLAYER 1 TOOK A LADDER AND CLIMED FROM 41 TO 79\n");
			player1.position_x = tiles[7][8].position_x - (length_of_tile / 200);
			player1.position_y = tiles[7][8].position_y - (length_of_tile / 4);
			player1.current_position_on_tile = 78;
		}
		if (player1.current_position_on_tile == (number_of_cols * number_of_rows) - 1)
		{
			printf("THE PLAYER 1 HAS WON THE GAME\n");
			game_running = FALSE;
			return;
		}
	}

}

void movement_player2(int dice)
{
	if (player2.current_position_on_tile == (number_of_cols * number_of_rows) - 1)
	{
		printf("THE PLAYER 2 HAS WON THE GAME\n");
		game_running = FALSE;
		return;
	}
	else {


		if ((dice + player2.current_position_on_tile) >= (number_of_cols * number_of_rows))
		{
			dice = (number_of_cols * number_of_rows) - player2.current_position_on_tile - 1;
			printf("dice roll changed to %d\n", dice);
		}
		for (int k = 1; k <= dice; k++)
		{
			player2.current_position_on_tile += 1;
			int i = player2.current_position_on_tile / number_of_cols;
			int j = player2.current_position_on_tile - (number_of_rows * i);
			if (j > (number_of_cols - 1))
			{
				player2.position_x = tiles[i + 1][j].position_x - (length_of_tile / 2.5);
				player2.position_y = tiles[i + 1][j].position_y - (length_of_tile / 2);

			}
			else
			{
				player2.position_x = tiles[i][j].position_x - (length_of_tile / 2.5);
				player2.position_y = tiles[i][j].position_y - (length_of_tile / 4);
			}
			render();
			Sleep(300);
		}

		printf("player 2 index increased to %d\n", player2.current_position_on_tile + 1);

		//adding some snakes
		//lets put the snakes mouth at indices 28 -> 10, 37 -> 3 , 47 -> 16 , 75 -> 32 , 96 -> 42 , 94 -> 71 
		if (player2.current_position_on_tile == 27)
		{
			printf("PLAYER 2 WAS BITTEN BY A SNAKE AND MOVED FROM 28 TO POSITION 10\n");
			player2.position_x = tiles[0][9].position_x - (length_of_tile / 2.5);
			player2.position_y = tiles[0][9].position_y - (length_of_tile / 4);
			player2.current_position_on_tile = 9;
		}
		if (player2.current_position_on_tile == 36)
		{
			printf("PLAYER 2 WAS BITTEN BY A SNAKE AND MOVED FROM 37 TO POSITION 3\n");
			player2.position_x = tiles[0][2].position_x - (length_of_tile / 2.5);
			player2.position_y = tiles[0][2].position_y - (length_of_tile / 4);
			player2.current_position_on_tile = 2;
		}
		if (player2.current_position_on_tile == 46)
		{
			printf("PLAYER 2 WAS BITTEN BY A SNAKE AND MOVED FROM 47 TO POSITION 16\n");
			player2.position_x = tiles[1][5].position_x - (length_of_tile / 2.5);
			player2.position_y = tiles[1][5].position_y - (length_of_tile / 4);
			player2.current_position_on_tile = 15;
		}
		if (player2.current_position_on_tile == 74)
		{
			printf("PLAYER 2 WAS BITTEN BY A SNAKE AND MOVED FROM 75 TO POSITION 32\n");
			player2.position_x = tiles[3][1].position_x - (length_of_tile / 2.5);
			player2.position_y = tiles[3][1].position_y - (length_of_tile / 4);
			player2.current_position_on_tile = 31;
		}
		if (player2.current_position_on_tile == 95)
		{
			printf("PLAYER 2 WAS BITTEN BY A SNAKE AND MOVED FROM 96 TO POSITION 42\n");
			player2.position_x = tiles[4][1].position_x - (length_of_tile / 2.5);
			player2.position_y = tiles[4][1].position_y - (length_of_tile / 4);
			player2.current_position_on_tile = 41;
		}
		if (player2.current_position_on_tile == 93)
		{
			printf("PLAYER 2 WAS BITTEN BY A SNAKE AND MOVED FROM 94 TO POSITION 71\n");
			player2.position_x = tiles[7][0].position_x - (length_of_tile / 2.5);
			player2.position_y = tiles[7][0].position_y - (length_of_tile / 4);
			player2.current_position_on_tile = 70;
		}

		//adding some ladders
		//lets put the ladder start at indices 4 -> 56 , 14 -> 55 , 54 -> 88 , 12 -> 50 , 22 -> 58 , 41 -> 79
		if (player2.current_position_on_tile == 3)
		{
			printf("PLAYER 2 TOOK A LADDER AND CLIMED FROM 4 TO 56\n");
			player2.position_x = tiles[5][5].position_x - (length_of_tile / 2.5);
			player2.position_y = tiles[5][5].position_y - (length_of_tile / 4);
			player2.current_position_on_tile = 55;
		}
		if (player2.current_position_on_tile == 13)
		{
			printf("PLAYER 2 TOOK A LADDER AND CLIMED FROM 14 TO 55\n");
			player2.position_x = tiles[5][4].position_x - (length_of_tile / 2.5);
			player2.position_y = tiles[5][4].position_y - (length_of_tile / 4);
			player2.current_position_on_tile = 54;
		}
		if (player2.current_position_on_tile == 53)
		{
			printf("PLAYER 2 TOOK A LADDER AND CLIMED FROM 54 TO 88\n");
			player2.position_x = tiles[8][7].position_x - (length_of_tile / 2.5);
			player2.position_y = tiles[8][7].position_y - (length_of_tile / 4);
			player2.current_position_on_tile = 87;
		}
		if (player2.current_position_on_tile == 11)
		{
			printf("PLAYER 2 TOOK A LADDER AND CLIMED FROM 12 TO 50\n");
			player2.position_x = tiles[4][9].position_x - (length_of_tile / 2.5);
			player2.position_y = tiles[4][9].position_y - (length_of_tile / 4);
			player2.current_position_on_tile = 49;
		}
		if (player2.current_position_on_tile == 21)
		{
			printf("PLAYER 2 TOOK A LADDER AND CLIMED FROM 22 TO 58\n");
			player2.position_x = tiles[5][7].position_x - (length_of_tile / 2.5);
			player2.position_y = tiles[5][7].position_y - (length_of_tile / 4);
			player2.current_position_on_tile = 57;
		}
		if (player2.current_position_on_tile == 40)
		{
			printf("PLAYER 2 TOOK A LADDER AND CLIMED FROM 41 TO 79\n");
			player2.position_x = tiles[7][8].position_x - (length_of_tile / 2.5);
			player2.position_y = tiles[7][8].position_y - (length_of_tile / 4);
			player2.current_position_on_tile = 78;
		}
		if (player2.current_position_on_tile == (number_of_cols * number_of_rows) - 1)
		{
			printf("THE PLAYER 2 HAS WON THE GAME\n");
			game_running = FALSE;
			return;
		}
	}

}



