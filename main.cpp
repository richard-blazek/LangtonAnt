#include <iostream>
#include <vector>
#include <SDL.h>

constexpr int zoom = 6;
constexpr int width = 250, height = 150;

class Ant
{
private:
	constexpr static SDL_Point directions[] = {{1, 0}, {0, 1}, {-1, 0}, {0, -1}};
	int x, y, direction = 0;

public:
	Ant(int x, int y) : x(x), y(y) {}
	void Move()
	{
		x += directions[direction].x;
		y += directions[direction].y;
	}
	void Turn(bool left)
	{
		direction = (left ? direction + 3 : direction + 1) % 4;
	}
	int X() const
	{
		return x;
	}
	int Y() const
	{
		return y;
	}
	void DrawOn(SDL_Renderer *rend)
	{
		SDL_SetRenderDrawColor(rend, 0, 255, 0, 255);
		SDL_Rect rect{x * zoom, y * zoom, zoom, zoom};
		SDL_RenderFillRect(rend, &rect);
	}
};

class Field
{
private:
	bool field[height][width] = {{false}};

public:
	Field() {}
	void DrawOn(SDL_Renderer *rend)
	{
		for (int y = 0; y < height; ++y)
		{
			for (int x = 0; x < width; ++x)
			{
				bool on = field[y][x];
				SDL_SetRenderDrawColor(rend, on ? 255 : 32, on ? 255 : 32, on ? 255 : 32, 255);

				SDL_Rect rect{x * zoom, y * zoom, zoom, zoom};
				SDL_RenderFillRect(rend, &rect);
			}
		}
	}
	void Toggle(int x, int y)
	{
		field[y][x] = !field[y][x];
	}
	bool Contains(const Ant &ant)
	{
		return ant.X() >= 0 && ant.X() < width && ant.Y() >= 0 && ant.Y() <= height;
	}
	bool Get(int x, int y)
	{
		return field[y][x];
	}
};

void UpdateWindowTitle(SDL_Window *window, int step_count)
{
	std::string title = "Langton's ant, steps: " + std::to_string(step_count);
	SDL_SetWindowTitle(window, title.c_str());
}

int main()
{
	SDL_Init(SDL_INIT_EVERYTHING);

	SDL_Window *window = SDL_CreateWindow("Langton's ant", 40, 40, width * zoom, height * zoom, 0);
	SDL_Renderer *rend = SDL_CreateRenderer(window, -1, 0);
	SDL_Event event;
	int step_count = 0;
	Field field;
	Ant ant{width / 2, height / 2};
	bool repeat = true;
	bool move = false;
	while (repeat)
	{

		SDL_SetRenderDrawColor(rend, 0, 0, 0, 255);
		SDL_RenderClear(rend);

		field.DrawOn(rend);
		ant.DrawOn(rend);
		SDL_RenderPresent(rend);

		if (move)
		{
			++step_count;
			UpdateWindowTitle(window, step_count);

			ant.Turn(field.Get(ant.X(), ant.Y()));
			field.Toggle(ant.X(), ant.Y());
			ant.Move();
			if (!field.Contains(ant))
			{
				SDL_ShowSimpleMessageBox(0, "Langton's ant", "The ant has escaped!", window);
				break;
			}
		}
		while (SDL_PollEvent(&event))
		{
			if (event.type == SDL_QUIT)
			{
				repeat = false;
			}
			else if (event.type == SDL_MOUSEBUTTONDOWN)
			{
				move = !move;
			}
		}
		SDL_Delay(10);
	}
	SDL_Quit();
	return 0;
}