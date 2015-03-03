#include <cstdio>
#include <cstdlib>
#include <ctime>
#include <iostream>
#include <string>
#include <vector>
#include <SDL.h>
#include <SDL_opengl.h>
#include <gl/GLU.h>
#include <SDL_image.h>

enum CellState {
	NONE = 0,
	X,
	O
};

CellState states[9];

SDL_Window* window;

SDL_GLContext context;

bool running = true;

class Texture {
	GLuint id;
	int width;
	int height;

public:

	Texture() {
		id = 0;
	}

	void init(std::string fn) {
		SDL_Surface* surf = 0;

		surf = IMG_Load(fn.c_str());

		if(surf == 0) {
			std::cout << "This file doesn't exist" << std::endl;
			return;
		}

		int v = GL_RGB;

		if(surf->format->BytesPerPixel == 4) {
			v = GL_RGBA;
		}

		glGenTextures(1, &id);

		bind();

		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);

		glTexImage2D(
			GL_TEXTURE_2D, 
			0,
			v,
			surf->w,
			surf->h,
			0,
			v,
			GL_UNSIGNED_BYTE,
			surf->pixels);

		unbind();
	}

	void bind() {
		glBindTexture(GL_TEXTURE_2D, id);
	}

	void unbind() {
		glBindTexture(GL_TEXTURE_2D, 0);
	}

	void release() {
		glDeleteTextures(1, &id);
	}
};

Texture tex[3];

void run_computer() {
	int j = 0;


	for(int i = 0; i < 9; i++) {
		int x = rand()%3;
		
		int y = rand()%3;

		if(states[y * 3 + x] == NONE) {
			states[y * 3 + x] = X;
			break;
		}

		j++;

		std::cout << "In loop: " << j << std::endl;
	}
}

void win_condition() {

	bool finished = false;

	for(int i = 0; i < 3; i++) {
		for(int j = 0; j < 3; j++) {

		}
	}
}

int main(int argc, char** argv) {

	srand(time(0));

	SDL_Init(SDL_INIT_EVERYTHING);

	SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, 2);
	SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, 0);
	SDL_GL_SetAttribute(SDL_GL_DOUBLEBUFFER, 1);

	window = SDL_CreateWindow(
		"Tic Tac Toe",
		SDL_WINDOWPOS_UNDEFINED,
		SDL_WINDOWPOS_UNDEFINED,
		600,
		600,
		SDL_WINDOW_OPENGL | SDL_WINDOW_SHOWN);


	context = SDL_GL_CreateContext(window);

	SDL_GL_SetSwapInterval(1);

	SDL_Event ev;

	glEnable(GL_TEXTURE_2D);
	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

	tex[0].init("data/NONE.png");
	tex[1].init("data/X.png");
	tex[2].init("data/O.png");


	while(running) {
		while(SDL_PollEvent(&ev)) {
			if(ev.type == SDL_QUIT) {
				running = false;
			}

			if(ev.type == SDL_MOUSEBUTTONDOWN) {
				int x = ev.button.x / 200;
				int y = ev.button.y / 200;

				if(states[y * 3 + x] == NONE) {
					states[y * 3 + x] = O;
				}

				run_computer();

			}
		}

		// Update Section


		// Rendering Section

		glClear(GL_COLOR_BUFFER_BIT);
		glMatrixMode(GL_PROJECTION);
		glLoadIdentity();
		gluOrtho2D(0.0f, 600.0f, 600.0f, 0.0f);
		glMatrixMode(GL_MODELVIEW);


		for(int y = 0; y < 3; y++) {
			for(int x = 0; x < 3; x++) {

				glPushMatrix();
				glTranslatef(x * 200, y * 200, 0);
				glScalef(200, 200, 0);

				if(states[y * 3 + x] == NONE) {
					tex[0].bind();
				} else if(states[y * 3 + x] == X) {
					tex[1].bind();
				} else if(states[y * 3 + x] == O) {
					tex[2].bind();
				}

				glBegin(GL_TRIANGLES);
				

				glTexCoord2f(0.0f, 0.0f);
				glVertex3f(0.0f, 0.0f, 0.0f);
				glTexCoord2f(1.0f, 0.0f);
				glVertex3f(1.0f, 0.0f, 0.0f);
				glTexCoord2f(0.0f, 1.0f);
				glVertex3f(0.0f, 1.0f, 0.0f);

				glTexCoord2f(0.0f, 1.0f);
				glVertex3f(0.0f, 1.0f, 0.0f);
				glTexCoord2f(1.0f, 0.0f);
				glVertex3f(1.0f, 0.0f, 0.0f);
				glTexCoord2f(1.0f, 1.0f);
				glVertex3f(1.0f, 1.0f, 0.0f);

				glEnd();

				glBindTexture(GL_TEXTURE_2D, 0);

				glPopMatrix();
			}
		}

		glBegin(GL_LINES);

		glColor4f(1.0f, 1.0f, 1.0f, 1.0f);

		for(int i = 1; i <= 2; i++) {
			glVertex3f(200 * i, 0.0f, 0.0f);
			glVertex3f(200 * i, 600.0f, 0.0f);

			glVertex3f(0.0f, 200 * i, 0.0f);
			glVertex3f(600.0f, 200 * i, 0.0f);
		}

		glEnd();

		SDL_GL_SwapWindow(window);
	}

	SDL_Quit();

	return 0;
}