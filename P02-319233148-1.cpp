//Práctica 2: índices, mesh, proyecciones, transformaciones geométricas
#include <stdio.h>
#include <string.h>
#include<cmath>
#include<vector>
#include <glew.h>
#include <glfw3.h>
//glm
#include<glm.hpp>
#include<gtc\matrix_transform.hpp>
#include<gtc\type_ptr.hpp>
//clases para dar orden y limpieza al código
#include"Mesh.h"
#include"Shader.h"
#include"Window.h"
//Dimensiones de la ventana
const float toRadians = 3.14159265f / 180.0; //grados a radianes
Window mainWindow;
std::vector<Mesh*> meshList;
std::vector<MeshColor*> meshColorList;
std::vector<Shader>shaderList;
//Vertex Shader
static const char* vShader = "shaders/shader.vert";
static const char* fShader = "shaders/shader.frag";
static const char* vShaderColor = "shaders/shadercolor.vert";
static const char* fShaderColor = "shaders/shadercolor.frag";
//shaders nuevos se crearían acá

float angulo = 0.0f;

//color café en RGB : 0.478, 0.255, 0.067

//Pirámide triangular regular
void CreaPiramide()
{
	unsigned int indices[] = {
		0,1,2,
		1,3,2,
		3,0,2,
		1,0,3

	};
	GLfloat vertices[] = {
		-0.5f, -0.5f,0.0f,	//0
		0.5f,-0.5f,0.0f,	//1
		0.0f,0.5f, -0.25f,	//2
		0.0f,-0.5f,-0.5f,	//3

	};
	Mesh* obj1 = new Mesh();
	obj1->CreateMesh(vertices, indices, 12, 12);
	meshList.push_back(obj1);
}

//Vértices de un cubo
void CrearCubo()
{
	unsigned int cubo_indices[] = {
		// front
		0, 1, 2,
		2, 3, 0,
		// right
		1, 5, 6,
		6, 2, 1,
		// back
		7, 6, 5,
		5, 4, 7,
		// left
		4, 0, 3,
		3, 7, 4,
		// bottom
		4, 5, 1,
		1, 0, 4,
		// top
		3, 2, 6,
		6, 7, 3
	};

	GLfloat cubo_vertices[] = {
		// front
		-0.5f, -0.5f,  0.5f,
		0.5f, -0.5f,  0.5f,
		0.5f,  0.5f,  0.5f,
		-0.5f,  0.5f,  0.5f,
		// back
		-0.5f, -0.5f, -0.5f,
		0.5f, -0.5f, -0.5f,
		0.5f,  0.5f, -0.5f,
		-0.5f,  0.5f, -0.5f
	};
	Mesh* cubo = new Mesh();
	cubo->CreateMesh(cubo_vertices, cubo_indices, 24, 36);
	meshList.push_back(cubo);
}

void CrearLetrasyFiguras()
{
	//Letra H
	GLfloat vertices_letraH[] = {
		//X			Y			Z			R		G		B
		-0.2f,	0.3f,		0.5f,			0.137f,	0.447f,	0.212f,
		-0.1f,	0.3f,		0.5f,			0.137f,	0.447f,	0.212f,
		-0.1f,	-0.3f,		0.5f,			0.137f,	0.447f,	0.212f,
		-0.2f,	0.3f,		0.5f,			0.137f,	0.447f,	0.212f,
		-0.2f,	-0.3f,		0.5f,			0.137f,	0.447f,	0.212f,
		-0.1f,	-0.3f,		0.5f,			0.137f,	0.447f,	0.212f,

		-0.1f,	0.05f,		0.5f,			0.137f,	0.447f,	0.212f,
		0.1f,	-0.05f,		0.5f,			0.137f,	0.447f,	0.212f,
		0.1f,	0.05f,		0.5f,			0.137f,	0.447f,	0.212f,
		-0.1f,	0.05f,		0.5f,			0.137f,	0.447f,	0.212f,
		0.1f,	-0.05f,		0.5f,			0.137f,	0.447f,	0.212f,
		-0.1f,	-0.05f,		0.5f,			0.137f,	0.447f,	0.212f,

		0.1f,	0.3f,		0.5f,			0.137f,	0.447f,	0.212f,
		0.2f,	-0.3f,		0.5f,			0.137f,	0.447f,	0.212f,
		0.2f,	0.3f,		0.5f,			0.137f,	0.447f,	0.212f,
		0.1f,	0.3f,		0.5f,			0.137f,	0.447f,	0.212f,
		0.2f,	-0.3f,		0.5f,			0.137f,	0.447f,	0.212f,
		0.1f,	-0.3f,		0.5f,			0.137f,	0.447f,	0.212f,

	};
	MeshColor* letraH = new MeshColor();
	letraH->CreateMeshColor(vertices_letraH, 108);
	meshColorList.push_back(letraH);

	//Letra D
	GLfloat vertices_letraD[] = {
		//X			Y			Z			R		G		B
		-0.2f,	0.3f,		0.5f,			0.89f,	0.42f,	0.12f,
		-0.1f,	0.3f,		0.5f,			0.89f,	0.42f,	0.12f,
		-0.1f,	-0.3f,		0.5f,			0.89f,	0.42f,	0.12f,
		-0.2f,	0.3f,		0.5f,			0.89f,	0.42f,	0.12f,
		-0.2f,	-0.3f,		0.5f,			0.89f,	0.42f,	0.12f,
		-0.1f,	-0.3f,		0.5f,			0.89f,	0.42f,	0.12f,

		-0.2f,	0.3f,		0.5f,			0.89f,	0.42f,	0.12f,
		0.1f,	0.2f,		0.5f,			0.89f,	0.42f,	0.12f,
		-0.2f,	0.2f,		0.5f,			0.89f,	0.42f,	0.12f,
		-0.2f,	0.3f,		0.5f,			0.89f,	0.42f,	0.12f,
		0.1f,	0.2f,		0.5f,			0.89f,	0.42f,	0.12f,
		0.1f,	0.3f,		0.5f,			0.89f,	0.42f,	0.12f,

		-0.2f,	-0.3f,		0.5f,			0.89f,	0.42f,	0.12f,
		0.1f,	-0.2f,		0.5f,			0.89f,	0.42f,	0.12f,
		-0.2f,	-0.2f,		0.5f,			0.89f,	0.42f,	0.12f,
		-0.2f,	-0.3f,		0.5f,			0.89f,	0.42f,	0.12f,
		0.1f,	-0.2f,		0.5f,			0.89f,	0.42f,	0.12f,
		0.1f,	-0.3f,		0.5f,			0.89f,	0.42f,	0.12f,

		0.1f,	0.1f,		0.5f,			0.89f,	0.42f,	0.12f,
		0.2f,	-0.1f,		0.5f,			0.89f,	0.42f,	0.12f,
		0.2f,	0.1f,		0.5f,			0.89f,	0.42f,	0.12f,
		0.1f,	0.1f,		0.5f,			0.89f,	0.42f,	0.12f,
		0.2f,	-0.1f,		0.5f,			0.89f,	0.42f,	0.12f,
		0.1f,	-0.1f,		0.5f,			0.89f,	0.42f,	0.12f,

		0.1f,	0.3f,		0.5f,			0.89f,	0.42f,	0.12f,
		0.2f,	0.1f,		0.5f,			0.89f,	0.42f,	0.12f,
		0.11f,	0.07f,		0.5f,			0.89f,	0.42f,	0.12f,
		0.1f,	0.3f,		0.5f,			0.89f,	0.42f,	0.12f,
		0.11f,	0.07f,		0.5f,			0.89f,	0.42f,	0.12f,
		0.01f,	0.27f,		0.5f,			0.89f,	0.42f,	0.12f,

		0.2f,	-0.1f,		0.5f,			0.89f,	0.42f,	0.12f,
		0.1f,	-0.3f,		0.5f,			0.89f,	0.42f,	0.12f,
		0.11f,	-0.07f,		0.5f,			0.89f,	0.42f,	0.12f,
		0.1f,	-0.3f,		0.5f,			0.89f,	0.42f,	0.12f,
		0.11f,	-0.07f,		0.5f,			0.89f,	0.42f,	0.12f,
		0.01f,	-0.27f,		0.5f,			0.89f,	0.42f,	0.12f,

	};
	MeshColor* letraD = new MeshColor();
	letraD->CreateMeshColor(vertices_letraD, 216);
	meshColorList.push_back(letraD);

	//Letra B
	GLfloat vertices_letraB[] = {
		//X			Y			Z			R		G		B
		-0.2f,	0.3f,		0.5f,			0.302f,	0.541f,	0.545f,
		-0.1f,	0.3f,		0.5f,			0.302f,	0.541f,	0.545f,
		-0.1f,	-0.3f,		0.5f,			0.302f,	0.541f,	0.545f,
		-0.2f,	0.3f,		0.5f,			0.302f,	0.541f,	0.545f,
		-0.2f,	-0.3f,		0.5f,			0.302f,	0.541f,	0.545f,
		-0.1f,	-0.3f,		0.5f,			0.302f,	0.541f,	0.545f,

		-0.2f,	0.3f,		0.5f,			0.302f,	0.541f,	0.545f,
		0.1f,	0.2f,		0.5f,			0.302f,	0.541f,	0.545f,
		-0.2f,	0.2f,		0.5f,			0.302f,	0.541f,	0.545f,
		-0.2f,	0.3f,		0.5f,			0.302f,	0.541f,	0.545f,
		0.1f,	0.2f,		0.5f,			0.302f,	0.541f,	0.545f,
		0.1f,	0.3f,		0.5f,			0.302f,	0.541f,	0.545f,

		-0.2f,	-0.3f,		0.5f,			0.302f,	0.541f,	0.545f,
		0.1f,	-0.2f,		0.5f,			0.302f,	0.541f,	0.545f,
		-0.2f,	-0.2f,		0.5f,			0.302f,	0.541f,	0.545f,
		-0.2f,	-0.3f,		0.5f,			0.302f,	0.541f,	0.545f,
		0.1f,	-0.2f,		0.5f,			0.302f,	0.541f,	0.545f,
		0.1f,	-0.3f,		0.5f,			0.302f,	0.541f,	0.545f,

		-0.1f,	0.05f,		0.5f,			0.302f,	0.541f,	0.545f,
		0.13f,	-0.05f,		0.5f,			0.302f,	0.541f,	0.545f,
		0.13f,	0.05f,		0.5f,			0.302f,	0.541f,	0.545f,
		-0.1f,	0.05f,		0.5f,			0.302f,	0.541f,	0.545f,
		0.13f,	-0.05f,		0.5f,			0.302f,	0.541f,	0.545f,
		-0.1f,	-0.05f,		0.5f,			0.302f,	0.541f,	0.545f,

		0.1f,	0.2f,		0.5f,			0.302f,	0.541f,	0.545f,
		0.2f,	0.05f,		0.5f,			0.302f,	0.541f,	0.545f,
		0.2f,	0.2f,		0.5f,			0.302f,	0.541f,	0.545f,
		0.1f,	0.2f,		0.5f,			0.302f,	0.541f,	0.545f,
		0.2f,	0.05f,		0.5f,			0.302f,	0.541f,	0.545f,
		0.1f,	0.05f,		0.5f,			0.302f,	0.541f,	0.545f,

		0.1f,	-0.05f,		0.5f,			0.302f,	0.541f,	0.545f,
		0.2f,	-0.2f,		0.5f,			0.302f,	0.541f,	0.545f,
		0.2f,	-0.05f,		0.5f,			0.302f,	0.541f,	0.545f,
		0.1f,	-0.05f,		0.5f,			0.302f,	0.541f,	0.545f,
		0.2f,	-0.2f,		0.5f,			0.302f,	0.541f,	0.545f,
		0.1f,	-0.2f,		0.5f,			0.302f,	0.541f,	0.545f,

		0.1f,	0.3f,		0.5f,			0.302f,	0.541f,	0.545f,
		0.1f,	0.2f,		0.5f,			0.302f,	0.541f,	0.545f,
		0.2f,	0.2f,		0.5f,			0.302f,	0.541f,	0.545f,

		0.05f,	0.2f,		0.5f,			0.302f,	0.541f,	0.545f,
		0.1f,	0.2f,		0.5f,			0.302f,	0.541f,	0.545f,
		0.1f,	0.15f,		0.5f,			0.302f,	0.541f,	0.545f,
		0.1f,	0.1f,		0.5f,			0.302f,	0.541f,	0.545f,
		0.05f,	0.05f,		0.5f,			0.302f,	0.541f,	0.545f,
		0.1f,	0.05f,		0.5f,			0.302f,	0.541f,	0.545f,

		0.1f,	-0.3f,		0.5f,			0.302f,	0.541f,	0.545f,
		0.1f,	-0.2f,		0.5f,			0.302f,	0.541f,	0.545f,
		0.2f,	-0.2f,		0.5f,			0.302f,	0.541f,	0.545f,

		0.05f,	-0.2f,		0.5f,			0.302f,	0.541f,	0.545f,
		0.1f,	-0.2f,		0.5f,			0.302f,	0.541f,	0.545f,
		0.1f,	-0.15f,		0.5f,			0.302f,	0.541f,	0.545f,
		0.1f,	-0.1f,		0.5f,			0.302f,	0.541f,	0.545f,
		0.05f,	-0.05f,		0.5f,			0.302f,	0.541f,	0.545f,
		0.1f,	-0.05f,		0.5f,			0.302f,	0.541f,	0.545f,

		0.2f,	0.05f,		0.5f,			0.302f,	0.541f,	0.545f,
		0.13f,	0.05f,		0.5f,			0.302f,	0.541f,	0.545f,
		0.13f,	0.0f,		0.5f,			0.302f,	0.541f,	0.545f,
		0.2f,	-0.05f,		0.5f,			0.302f,	0.541f,	0.545f,
		0.13f,	-0.05f,		0.5f,			0.302f,	0.541f,	0.545f,
		0.13f,	-0.0f,		0.5f,			0.302f,	0.541f,	0.545f,

	};
	MeshColor* letraB = new MeshColor();
	letraB->CreateMeshColor(vertices_letraB, 360);
	meshColorList.push_back(letraB);

}


void CreateShaders()
{

	Shader* shader1 = new Shader(); //shader para usar índices: objetos: cubo y  pirámide
	shader1->CreateFromFiles(vShader, fShader);
	shaderList.push_back(*shader1);

	Shader* shader2 = new Shader();//shader para usar color como parte del VAO: letras 
	shader2->CreateFromFiles(vShaderColor, fShaderColor);
	shaderList.push_back(*shader2);
}


int main()
{
	mainWindow = Window(800, 600);
	mainWindow.Initialise();
	CrearLetrasyFiguras(); //usa MeshColor, índices en MeshColorList
	CreateShaders();
	GLuint uniformProjection = 0;
	GLuint uniformModel = 0;
	//Projection: Matriz de Dimensión 4x4 para indicar si vemos en 2D( orthogonal) o en 3D) perspectiva
	glm::mat4 projection = glm::ortho(-1.0f, 1.0f, -1.0f, 1.0f, 0.1f, 100.0f);
	//glm::mat4 projection = glm::perspective(glm::radians(60.0f)	,mainWindow.getBufferWidth() / mainWindow.getBufferHeight(), 0.1f, 100.0f);

	//Model: Matriz de Dimensión 4x4 en la cual se almacena la multiplicación de las transformaciones geométricas.
	glm::mat4 model(1.0); //fuera del while se usa para inicializar la matriz con una identidad

	//Loop mientras no se cierra la ventana
	while (!mainWindow.getShouldClose())
	{
		//Recibir eventos del usuario
		glfwPollEvents();
		//Limpiar la ventana
		glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT); //Se agrega limpiar el buffer de profundidad


		//Para las letras hay que usar el segundo set de shaders con índice 1 en ShaderList 
		shaderList[1].useShader();
		uniformModel = shaderList[1].getModelLocation();
		uniformProjection = shaderList[1].getProjectLocation();

		//Letra H
		model = glm::mat4(1.0);
		model = glm::translate(model, glm::vec3(-0.55f, 0.0f, -4.0f));
		//
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));//FALSE ES PARA QUE NO SEA TRANSPUESTA
		glUniformMatrix4fv(uniformProjection, 1, GL_FALSE, glm::value_ptr(projection));
		meshColorList[0]->RenderMeshColor();

		//Letra D
		model = glm::mat4(1.0);
		model = glm::translate(model, glm::vec3(0.0f, 0.0f, -4.0f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));//FALSE ES PARA QUE NO SEA TRANSPUESTA
		meshColorList[1]->RenderMeshColor();

		//Letra B
		model = glm::mat4(1.0);
		model = glm::translate(model, glm::vec3(0.55f, 0.0f, -4.0f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));//FALSE ES PARA QUE NO SEA TRANSPUESTA
		meshColorList[2]->RenderMeshColor();

		glUseProgram(0);
		mainWindow.swapBuffers();

	}
	return 0;
}
// inicializar matriz: glm::mat4 model(1.0);
// reestablecer matriz: model = glm::mat4(1.0);
//Traslación
//model = glm::translate(model, glm::vec3(0.0f, 0.0f, -5.0f));
//////////////// ROTACIÓN //////////////////
//model = glm::rotate(model, 45 * toRadians, glm::vec3(0.0f, 1.0f, 0.0f));
////////////////  ESCALA ////////////////
//model = glm::scale(model, glm::vec3(0.5f, 0.5f, 0.5f));
///////////////////// T+R////////////////
/*model = glm::translate(model, glm::vec3(valor, 0.0f, 0.0f));
model = glm::rotate(model, 45 * toRadians, glm::vec3(0.0f, 0.0f, 1.0f));
model = glm::rotate(model, glm::radians(angulo), glm::vec3(0.0f, 1.0f, 0.0f));
*/
/////////////R+T//////////
/*model = glm::rotate(model, 45 * toRadians, glm::vec3(0.0f, 0.0f, 1.0f));
model = glm::translate(model, glm::vec3(valor, 0.0f, 0.0f));
*/