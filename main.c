#include <stdio.h>
#include <SDL2/SDL.h>
#include <stdbool.h>

#define ZOOM 6
#define WIDTH 250
#define HEIGHT 150

typedef struct ant
{
	int x, y, next;
} ant;

ant move_ant(ant a)
{
	static SDL_Point moves[] = {{1, 0}, {0, 1}, {-1, 0}, {0, -1}};
	ant moved = {a.x + moves[a.next].x, a.y + moves[a.next].y, a.next};
	return moved;
}

ant turn_ant(ant a, bool left)
{
	ant turned = {a.x, a.y, (left ? a.next + 3 : a.next + 1) % 4};
	return turned;
}

bool ant_escaped(ant a)
{
	return a.x < 0 || a.x >= WIDTH || a.y < 0 || a.y >= HEIGHT;
}

void draw_ant(SDL_Renderer *rend, ant ant)
{
	SDL_SetRenderDrawColor(rend, 0, 255, 0, 255);
	SDL_Rect rect = {ant.x * ZOOM, ant.y * ZOOM, ZOOM, ZOOM};
	SDL_RenderFillRect(rend, &rect);
}

void draw_field(SDL_Renderer *rend, bool field[HEIGHT][WIDTH])
{
	for (int y = 0; y < HEIGHT; ++y)
	{
		for (int x = 0; x < WIDTH; ++x)
		{
			SDL_SetRenderDrawColor(rend, field[y][x] ? 255 : 32, field[y][x] ? 255 : 32, field[y][x] ? 255 : 32, 255);
			SDL_Rect rect = {x * ZOOM, y * ZOOM, ZOOM, ZOOM};
			SDL_RenderFillRect(rend, &rect);
		}
	}
}

void update_window_title(SDL_Window *window, int step_count)
{
	char buffer[64];
	snprintf(buffer, 64, "Langton's ant, steps: %d", step_count);
	SDL_SetWindowTitle(window, buffer);
}

int main()
{
	SDL_Init(SDL_INIT_EVERYTHING);

	SDL_Window *window = SDL_CreateWindow("Langton's ant", 40, 40, WIDTH * ZOOM, HEIGHT * ZOOM, 0);
	SDL_Renderer *rend = SDL_CreateRenderer(window, -1, 0);
	SDL_Event event;
	int step_count = 0;
	bool field[HEIGHT][WIDTH] = {{false}};
	ant ant = {WIDTH / 2, HEIGHT / 2, 1};
	bool move = false;
	while (!SDL_QuitRequested())
	{

		SDL_SetRenderDrawColor(rend, 0, 0, 0, 255);
		SDL_RenderClear(rend);

		draw_field(rend, field);
		draw_ant(rend, ant);
		SDL_RenderPresent(rend);

		if (move)
		{
			++step_count;
			update_window_title(window, step_count);

			ant = turn_ant(ant, field[ant.y][ant.x]);
			field[ant.y][ant.x] = !field[ant.y][ant.x];
			ant = move_ant(ant);
			if (ant_escaped(ant))
			{
				SDL_ShowSimpleMessageBox(0, "Langton's ant", "The ant has escaped!", window);
				break;
			}
		}
		while (SDL_PollEvent(&event))
		{
			if (event.type == SDL_MOUSEBUTTONDOWN)
			{
				move = !move;
			}
		}
		SDL_Delay(10);
	}
	SDL_Quit();
	return 0;
}