/****************************************************************
* Name:       xxx
* Purpose:    xxx
* Author:     Fredrick Vamstad
* Created:    2015 - June
* Copyright:  Fredrick Vamstad
* Dependency: xxx
****************************************************************/

#include "opengl.h"
#include "rendererforward.h"
#include "camera.h"

static Camera *CameraInstance = nullptr;
RendererBase *RendererInstance = nullptr;
#if 0
static void GLFW_MouseButton(GLFWwindow *window,
							 int button,
							 int action,
							 int mods)
{

	switch (button) {
	case GLFW_MOUSE_BUTTON_LEFT: {
			if (action == GLFW_PRESS) {
				glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_HIDDEN);
				double pos[2];
				glfwGetCursorPos(window, &pos[0], &pos[1]);
				CameraInstance->setMouseOrigin(pos[0],
											   pos[1]);
				CameraInstance->active() = true;
			}
			else {
				glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_NORMAL);
				CameraInstance->active() = false;
			}
			break;
		}
	}
}



static void GLFW_WindowResize(GLFWwindow *window,
							  int width,
							  int height)
{
	RendererInstance->resize(width, height);
}

int main()
{
	/* Initialize the library */
	if (!glfwInit())
		return -1;

	glfwWindowHint(GLFW_RESIZABLE, GL_TRUE);
	glfwWindowHint(GLFW_STENCIL_BITS, 8);
	glfwWindowHint(GLFW_DEPTH_BITS, 24);
	glfwWindowHint(GLFW_SAMPLES, 0);

	const int windowWidth = 1024;
	const int windowHeight = 768;

	/* Create a windowed mode window and its OpenGL context */
	GLFWwindow* window = glfwCreateWindow(windowWidth, windowHeight, "OpenGL Sample", NULL, NULL);
	if (!window) {
		glfwTerminate();
		return -1;
	}

	/* Make the window's context current */
	glfwMakeContextCurrent(window);

	glfwSetMouseButtonCallback(window, &GLFW_MouseButton);
	glfwSetWindowSizeCallback(window, &GLFW_WindowResize);

	if (glewInit() == GLEW_OK) {
		RendererInstance = new RendererForward(window);
		// renderer = new RendererDeferred(window);
		if (RendererInstance) {
			if (RendererInstance->initialize()) {
				RendererInstance->resize(windowWidth, windowHeight);

				CameraInstance = RendererInstance->camera();

				clock_t logicTime = clock();

				/* Loop until the user closes the window */
				while (!glfwWindowShouldClose(window)) {

					if ((clock() - logicTime) >= 30) {
						RendererInstance->logicUpdate();
						logicTime = clock();
					}

					/* Render here */
					RendererInstance->render();
					/* Swap front and back buffers */
					glfwSwapBuffers(window);

					/* Poll for and process events */
					glfwPollEvents();
				}
			}
			RendererInstance->destroy();
			delete RendererInstance;
		}
	}
	glfwTerminate();
	return 0;
}
#endif

int main()
{
	int ret = SDL_Init(SDL_INIT_VIDEO
		| SDL_INIT_EVENTS // Window events etc
	 // | SDL_INIT_HAPTIC // Force feedback
	//	| SDL_INIT_GAMECONTROLLER // Gamepad, joystick etc
		| SDL_INIT_NOPARACHUTE // Disable catch fatal signals
		);
	if (ret == 0) {

		/* Turn on double buffering with a 24bit Z buffer.
		* You may need to change this to 16 or 32 for your system */
		SDL_GL_SetAttribute(SDL_GL_DOUBLEBUFFER, 1);
		SDL_GL_SetAttribute(SDL_GL_DEPTH_SIZE, 24);

		// Important!, Only for this demo
		SDL_GL_SetAttribute(SDL_GL_MULTISAMPLEBUFFERS, 1);
		SDL_GL_SetAttribute(SDL_GL_MULTISAMPLESAMPLES, 8); // 16 Max

		SDL_Window *mainWindow = SDL_CreateWindow("", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED,
								1024, 768, SDL_WINDOW_OPENGL | SDL_WINDOW_SHOWN | SDL_WINDOW_RESIZABLE);


		if (mainWindow) {
			//SDL_SetWindowGrab(mainWindow, SDL_TRUE);

			/* Request opengl 4.3 context.
			*/
			SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, 4);
			SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, 3);
			SDL_GL_SetAttribute(SDL_GL_CONTEXT_PROFILE_MASK, SDL_GL_CONTEXT_PROFILE_COMPATIBILITY);


			/* Create our opengl context and attach it to our window */
			SDL_GLContext mainContext = SDL_GL_CreateContext(mainWindow);
			SDL_GL_SetSwapInterval(1);

			if (glewInit() == GLEW_OK) {
				RendererInstance = new RendererForward(mainWindow);
				if (RendererInstance && RendererInstance->initialize()) {

					int currWindowWidth = 0;
					int currWindowHeight = 0;
					SDL_GetWindowSize(mainWindow, &currWindowWidth, &currWindowHeight);
					RendererInstance->resize(currWindowWidth, currWindowHeight);
					
					CameraInstance = RendererInstance->camera();

					clock_t logicTime = clock();
					
					SDL_Event event;
					bool quit = false;
					while (!quit) {

						while (SDL_PollEvent(&event)) {
							switch (event.type) {
							case SDL_WINDOWEVENT:
								RendererInstance->sdlWindowEvent(event);
								break;
							case SDL_MOUSEBUTTONDOWN:
							case SDL_MOUSEBUTTONUP:
							case SDL_MOUSEWHEEL:
								RendererInstance->sdlMouseKeyEvent(event.type, event);
								break;
							case SDL_QUIT:
								quit = true;
								break;
							}
						}

						if ((clock() - logicTime) >= 30) {
							RendererInstance->logicUpdate();
							logicTime = clock();
						}


						RendererInstance->render();
						SDL_GL_SwapWindow(mainWindow);
					}
					RendererInstance->destroy();
					delete RendererInstance;
				}
			}
		}

	}
	return 0;
}