#include <iostream>
#include <vector>
#include "objsdl/objsdl.h"
#include "mylibraries/func.h"

using namespace std;

class Ant
{
private:
	SDL::Point pos, direction;
public:
	Ant(SDL::Point pos):pos(pos), direction(1,0){}
	void Move()
	{
		pos+=direction;
	}
	void Turn(bool left)
	{
		direction=~direction*(left^(direction.x==0)?-1:1);
	}
	SDL::Point Position()const
	{
		return pos;
	}
};

class Field
{
private:
	SDL::Surface surface;
	static SDL::Color NegateColor(SDL::Color col)
	{
		return SDL::Color(255-col.r, 255-col.g, 255-col.b, 128);
	}
public:
	Field(SDL::Point size, const vector<SDL::Color>& colors):surface(size, colors, 8) {}
	void DrawOn(SDL::Renderer& rend, SDL::Rect dst)
	{
		rend.Draw(surface, surface.Size(), dst);
	}
	void NegateColor(SDL::Point pos)
	{
		surface.Draw(pos, NegateColor(surface[pos]));
	}
	SDL::Rect Area()const
	{
		return SDL::Rect(surface.Size());
	}
	SDL::Color operator[](SDL::Point xy)
	{
		return surface[xy];
	}
	void Draw(SDL::Point xy, SDL::Color col)
	{
		surface.Draw(xy, col);
	}
};

int main(int argc, const char* argv[])try
{
	SDL::Init _sdl;
	SDL::Font font("font.ttf", 12);
	SDL::Window window("Langton's ant", SDL::Rect(70,70, 900, 600));
	SDL::Renderer rend(window);
	SDL::KeyboardState kb;
	uint32 step_count=0;
	constexpr int zoom=2;
	Field field(SDL::Point(800,600)/zoom, {SDL::Color(15,15,15), SDL::Color(240,240,240)});
	Ant ant(SDL::Point(400, 300)/zoom);
	bool repeat=true;
	bool move=false;
	while(repeat)
	{
		rend.Repaint(SDL::Color());
		field.DrawOn(rend, SDL::Point(800,600));
		rend.Draw(SDL::Rect(ant.Position()*zoom, zoom), SDL::Color(0,255,0));
		rend.Draw(SDL::Rect(800,0,100,600), SDL::Color(255,255,255));
		rend.Draw(font, to_string(step_count), SDL::Color(0,0,0), SDL::Rect(800,0, 100,20));
		rend.Show();
		if(move)
		{
			++step_count;
			ant.Turn(field[ant.Position()]==SDL::Color(240,240,240));
			field.NegateColor(ant.Position());
			ant.Move();
			if(!field.Area().Encloses(ant.Position()))
			{
				SDL::MessageBox::Show("Langton's ant", "The ant has escaped!");
				break;
			}
		}
		for(auto& event:SDL::events::Handler())
		{
			if(event.Type()==SDL::events::Type::Quit)
			{
				repeat=false;
			}
			else if(event.Type()==SDL::events::Type::MouseButtonDown)
			{
				move=!move;
			}
		}
		SDL::Wait(20);
	}
	return 0;
}
catch(exception& exc)
{
	SDL::MessageBox::Show("Error", "Error: '"s+exc.what()+"'");
}