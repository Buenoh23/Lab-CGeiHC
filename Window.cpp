#include "Window.h"

Window::Window()
{
	width = 800;
	height = 600;
	for (size_t i = 0; i < 1024; i++)
	{
		keys[i] = 0;
	}
}
Window::Window(GLint windowWidth, GLint windowHeight)
{
	width = windowWidth;
	height = windowHeight;
	muevex = 2.0f;
	rotax = 0.0f;
	rotay = 0.0f;
	rotaz = 0.0f;
	transx_carro = 0.0f;
	transx_helicopter = 0.0f;
	transy = 0.0f;
	transz = 0.0f;
	articulacion1 = 0.0f;
	articulacion2 = 0.0f;
	arregloLampara = 0.0f;
	direccionCarro = 0.0f;
	movimiento = 0.0f;

	for (size_t i = 0; i < 1024; i++)
	{
		keys[i] = 0;
	}
}
int Window::Initialise()
{
	//Inicialización de GLFW
	if (!glfwInit())
	{
		printf("Falló inicializar GLFW");
		glfwTerminate();
		return 1;
	}
	//Asignando variables de GLFW y propiedades de ventana
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	//para solo usar el core profile de OpenGL y no tener retrocompatibilidad
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
	glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);

	//CREAR VENTANA
	mainWindow = glfwCreateWindow(width, height, "Practica 08: Iluminacion 2", NULL, NULL);

	if (!mainWindow)
	{
		printf("Fallo en crearse la ventana con GLFW");
		glfwTerminate();
		return 1;
	}
	//Obtener tamaño de Buffer
	glfwGetFramebufferSize(mainWindow, &bufferWidth, &bufferHeight);

	//asignar el contexto
	glfwMakeContextCurrent(mainWindow);

	//MANEJAR TECLADO y MOUSE
	createCallbacks();


	//permitir nuevas extensiones
	glewExperimental = GL_TRUE;

	if (glewInit() != GLEW_OK)
	{
		printf("Falló inicialización de GLEW");
		glfwDestroyWindow(mainWindow);
		glfwTerminate();
		return 1;
	}

	glEnable(GL_DEPTH_TEST); //HABILITAR BUFFER DE PROFUNDIDAD
							 // Asignar valores de la ventana y coordenadas
							 
							 //Asignar Viewport
	glViewport(0, 0, bufferWidth, bufferHeight);
	//Callback para detectar que se está usando la ventana
	glfwSetWindowUserPointer(mainWindow, this);
}

void Window::createCallbacks()
{
	glfwSetKeyCallback(mainWindow, ManejaTeclado);
	glfwSetCursorPosCallback(mainWindow, ManejaMouse);
}
GLfloat Window::getXChange()
{
	GLfloat theChange = xChange;
	xChange = 0.0f;
	return theChange;
}

GLfloat Window::getYChange()
{
	GLfloat theChange = yChange;
	yChange = 0.0f;
	return theChange;
}




void Window::ManejaTeclado(GLFWwindow* window, int key, int code, int action, int mode)
{
	Window* theWindow = static_cast<Window*>(glfwGetWindowUserPointer(window));

	if (key == GLFW_KEY_ESCAPE && action == GLFW_PRESS)
	{
		glfwSetWindowShouldClose(window, GL_TRUE);
	}
	if (key == GLFW_KEY_Y)
	{
		theWindow-> muevex += 1.0;
	}
	if (key == GLFW_KEY_U)
	{
		theWindow-> muevex -= 1.0;
	}

	if (key == GLFW_KEY_E)
	{
		theWindow->rotax += 10.0;
	}
	if (key == GLFW_KEY_R)
	{
		theWindow->rotay += 10.0; //rotar sobre el eje y 10 grados
	}
	if (key == GLFW_KEY_T)
	{
		theWindow->rotaz += 10.0;
	}

	if (key == GLFW_KEY_O)
	{
		theWindow->transx_carro += 1.0; //avanzar sobre el eje x 5 unidades
		theWindow->direccionCarro = 1;
		if (action == GLFW_PRESS)
		{
			theWindow->keys[key] = true;
			theWindow->movimiento = 1;
		}
		else if (action == GLFW_RELEASE)
		{
			theWindow->keys[key] = false;
			theWindow->movimiento = 0;
		}
	}
	if (key == GLFW_KEY_P)
	{
		theWindow->transx_carro -= 1.0; //retroceder sobre el eje x 5 unidades
		theWindow->direccionCarro = 0;
		if (action == GLFW_PRESS)
		{
			theWindow->keys[key] = true;
			theWindow->movimiento = 1;
		}
		else if (action == GLFW_RELEASE)
		{
			theWindow->keys[key] = false;
			theWindow->movimiento = 0;
		}
	}

	if (key == GLFW_KEY_X)
	{
		theWindow->transx_helicopter += 1.0; //avanzar sobre el eje x 5 unidades
	}
	if (key == GLFW_KEY_C)
	{
		theWindow->transx_helicopter -= 1.0; //retroceder sobre el eje x 5 unidades
	}

	//teclas para la rotación del cofre
	if (key == GLFW_KEY_F)
	{
		theWindow->articulacion1 += 5.0;
		if (theWindow->articulacion1 > 45.0) {
			theWindow->articulacion1 = 45.0;
		}
	}
	if (key == GLFW_KEY_G)
	{
		theWindow->articulacion1 -= 5.0;
		if (theWindow->articulacion1 < -0.0) {
			theWindow->articulacion1 = -0;
		}
	}
	//teclas para la rotación de las llantas
	if (key == GLFW_KEY_K)
	{
		theWindow->articulacion2 += 15.0;
	}
	if (key == GLFW_KEY_L)
	{
		theWindow->articulacion2 -= 15.0;
	}

	if (key == GLFW_KEY_M)
	{
		theWindow->arregloLampara = 0;
	}
	if (key == GLFW_KEY_N)
	{
		theWindow->arregloLampara = 1;
	}
	if (key == GLFW_KEY_B)
	{
		theWindow->arregloHada = 0;
	}
	if (key == GLFW_KEY_V)
	{
		theWindow->arregloHada = 1;
	}



	if (key >= 0 && key < 1024)
	{
		if (action == GLFW_PRESS)
		{
			theWindow->keys[key] = true;
			//printf("se presiono la tecla %d'\n", key);
		}
		else if (action == GLFW_RELEASE)
		{
			theWindow->keys[key] = false;
			//printf("se solto la tecla %d'\n", key);
		}
	}
}

void Window::ManejaMouse(GLFWwindow* window, double xPos, double yPos)
{
	Window* theWindow = static_cast<Window*>(glfwGetWindowUserPointer(window));

	if (theWindow->mouseFirstMoved)
	{
		theWindow->lastX = xPos;
		theWindow->lastY = yPos;
		theWindow->mouseFirstMoved = false;
	}

	theWindow->xChange = xPos - theWindow->lastX;
	theWindow->yChange = theWindow->lastY - yPos;

	theWindow->lastX = xPos;
	theWindow->lastY = yPos;
}


Window::~Window()
{
	glfwDestroyWindow(mainWindow);
	glfwTerminate();

}
