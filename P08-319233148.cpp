/*
Práctica 8: Iluminación 2 
*/
//para cargar imagen
#define STB_IMAGE_IMPLEMENTATION

#include <stdio.h>
#include <string.h>
#include <cmath>
#include <vector>
#include <math.h>

#include <glew.h>
#include <glfw3.h>

#include <glm.hpp>
#include <gtc\matrix_transform.hpp>
#include <gtc\type_ptr.hpp>
//para probar el importer
//#include<assimp/Importer.hpp>

#include "Window.h"
#include "Mesh.h"
#include "Shader_light.h"
#include "Camera.h"
#include "Texture.h"
#include "Sphere.h"
#include"Model.h"
#include "Skybox.h"

//para iluminación
#include "CommonValues.h"
#include "DirectionalLight.h"
#include "PointLight.h"
#include "SpotLight.h"
#include "Material.h"
const float toRadians = 3.14159265f / 180.0f;

Window mainWindow;
std::vector<Mesh*> meshList;
std::vector<Shader> shaderList;

Camera camera;

Texture brickTexture;
Texture dirtTexture;
Texture plainTexture;
Texture pisoTexture;
Texture AgaveTexture;
Texture dadoTexture;

Model Kitt_M;
Model Llanta_M;
Model Elantra_M;
Model Cofre_M;
Model Blackhawk_M;
Model Poste_M;
Model Hada_M;


Skybox skybox;

//materiales
Material Material_brillante;
Material Material_opaco;


//Sphere cabeza = Sphere(0.5, 20, 20);
GLfloat deltaTime = 0.0f;
GLfloat lastTime = 0.0f;
static double limitFPS = 1.0 / 60.0;

// luz direccional
DirectionalLight mainLight;
//para declarar varias luces de tipo pointlight
PointLight pointLights[MAX_POINT_LIGHTS];			//4 luces en MAX_POINT_LIGHTS
PointLight pointLightsSLamp[2];
//PointLight pointLights2[4];
SpotLight spotLights[MAX_SPOT_LIGHTS];
SpotLight spotLightsD[4];
SpotLight spotLightsR[4];

// Vertex Shader
static const char* vShader = "shaders/shader_light.vert";

// Fragment Shader
static const char* fShader = "shaders/shader_light.frag";


//función de calculo de normales por promedio de vértices 
void calcAverageNormals(unsigned int* indices, unsigned int indiceCount, GLfloat* vertices, unsigned int verticeCount,
	unsigned int vLength, unsigned int normalOffset)
{
	for (size_t i = 0; i < indiceCount; i += 3)
	{
		unsigned int in0 = indices[i] * vLength;
		unsigned int in1 = indices[i + 1] * vLength;
		unsigned int in2 = indices[i + 2] * vLength;
		glm::vec3 v1(vertices[in1] - vertices[in0], vertices[in1 + 1] - vertices[in0 + 1], vertices[in1 + 2] - vertices[in0 + 2]);
		glm::vec3 v2(vertices[in2] - vertices[in0], vertices[in2 + 1] - vertices[in0 + 1], vertices[in2 + 2] - vertices[in0 + 2]);
		glm::vec3 normal = glm::cross(v1, v2);
		normal = glm::normalize(normal);

		in0 += normalOffset; in1 += normalOffset; in2 += normalOffset;
		vertices[in0] += normal.x; vertices[in0 + 1] += normal.y; vertices[in0 + 2] += normal.z;
		vertices[in1] += normal.x; vertices[in1 + 1] += normal.y; vertices[in1 + 2] += normal.z;
		vertices[in2] += normal.x; vertices[in2 + 1] += normal.y; vertices[in2 + 2] += normal.z;
	}

	for (size_t i = 0; i < verticeCount / vLength; i++)
	{
		unsigned int nOffset = i * vLength + normalOffset;
		glm::vec3 vec(vertices[nOffset], vertices[nOffset + 1], vertices[nOffset + 2]);
		vec = glm::normalize(vec);
		vertices[nOffset] = vec.x; vertices[nOffset + 1] = vec.y; vertices[nOffset + 2] = vec.z;
	}
}

void CreateObjects()
{
	unsigned int indices[] = {
		0, 3, 1,
		1, 3, 2,
		2, 3, 0,
		0, 1, 2
	};

	GLfloat vertices[] = {
		//	x      y      z			u	  v			nx	  ny    nz
			-1.0f, -1.0f, -0.6f,	0.0f, 0.0f,		0.0f, 0.0f, 0.0f,
			0.0f, -1.0f, 1.0f,		0.5f, 0.0f,		0.0f, 0.0f, 0.0f,
			1.0f, -1.0f, -0.6f,		1.0f, 0.0f,		0.0f, 0.0f, 0.0f,
			0.0f, 1.0f, 0.0f,		0.5f, 1.0f,		0.0f, 0.0f, 0.0f
	};

	unsigned int floorIndices[] = {
		0, 2, 1,
		1, 2, 3
	};

	GLfloat floorVertices[] = {
		-10.0f, 0.0f, -10.0f,	0.0f, 0.0f,		0.0f, -1.0f, 0.0f,
		10.0f, 0.0f, -10.0f,	10.0f, 0.0f,	0.0f, -1.0f, 0.0f,
		-10.0f, 0.0f, 10.0f,	0.0f, 10.0f,	0.0f, -1.0f, 0.0f,
		10.0f, 0.0f, 10.0f,		10.0f, 10.0f,	0.0f, -1.0f, 0.0f
	};

	unsigned int vegetacionIndices[] = {
	   0, 1, 2,
	   0, 2, 3,
	   4,5,6,
	   4,6,7
	};

	GLfloat vegetacionVertices[] = {
		-0.5f, -0.5f, 0.0f,		0.0f, 0.0f,		0.0f, 0.0f, -1.0f,
		0.5f, -0.5f, 0.0f,		1.0f, 0.0f,		0.0f, 0.0f, -1.0f,
		0.5f, 0.5f, 0.0f,		1.0f, 1.0f,		0.0f, 0.0f, -1.0f,
		-0.5f, 0.5f, 0.0f,		0.0f, 1.0f,		0.0f, 0.0f, -1.0f,

		0.0f, -0.5f, -0.5f,		0.0f, 0.0f,		0.0f, 0.0f, 0.0f,
		0.0f, -0.5f, 0.5f,		1.0f, 0.0f,		0.0f, 0.0f, 0.0f,
		0.0f, 0.5f, 0.5f,		1.0f, 1.0f,		0.0f, 0.0f, 0.0f,
		0.0f, 0.5f, -0.5f,		0.0f, 1.0f,		0.0f, 0.0f, 0.0f,

	};

	unsigned int dadoIndices[] = {
		//top pyramid
		0, 1, 2,
		3, 4, 5,
		6, 7, 8,
		9, 10, 11,
		//bottom pyramid
		12, 13, 14,
		15, 16, 17,
		18, 19, 20,
		21, 22, 23,

	};

	GLfloat dadoVertices[] = {
		//x			y			z			S		T			NX		NY		NZ
		//top front
		-0.5f,		0.0f,		0.5f,		0.01f,   0.51f,		0.0f,	-1.0f,	-1.0f,	//0
		0.5f,		0.0f,		0.5f,		0.19f,	0.51f,		0.0f,	-1.0f,	-1.0f,	//1
		0.0f,		0.65f,		0.0f,		0.10f,	0.99f,		0.0f,	-1.0f,	-1.0f,	//2
		//top back
		-0.5f,		0.0f,		-0.5f,		0.59f,   0.51f,		0.0f,	-1.0f,	1.0f,	//0
		0.5f,		0.0f,		-0.5f,		0.41f,	0.51f,		0.0f,	-1.0f,	1.0f,	//1
		0.0f,		0.65f,		0.0f,		0.50f,	0.99f,		0.0f,	-1.0f,	1.0f,	//2
		//top right
		0.5f,		0.0f,		0.5f,		0.21f,   0.51f,		-1.0f,	-1.0f,	0.0f,	//0
		0.5f,		0.0f,		-0.5f,		0.39f,	0.51f,		-1.0f,	-1.0f,	0.0f,	//1
		0.0f,		0.65f,		0.0f,		0.30f,	0.99f,		-1.0f,	-1.0f,	0.0f,	//2
		//top left
		-0.5f,		0.0f,		0.5f,		0.79f,   0.51f,		1.0f,	-1.0f,	0.0f,	//0
		-0.5f,		0.0f,		-0.5f,		0.61f,	0.51f,		1.0f,	-1.0f,	0.0f,	//1
		0.0f,		0.65f,		0.0f,		0.70f,	0.99f,		1.0f,	-1.0f,	0.0f,	//2


		//bottom front
		-0.5f,		0.0f,		0.5f,		0.50f,   0.01f,		0.0f,	1.0f,	-1.0f,	//0
		0.5f,		0.0f,		0.5f,		0.41f,	0.49f,		0.0f,	1.0f,	-1.0f,	//1
		0.0f,		-0.65f,		0.0f,		0.59f,	0.49f,		0.0f,	1.0f,	-1.0f,	//2
		//bottom back		
		-0.5f,		0.0f,		-0.5f,		0.01f,   0.49f,		0.0f,	1.0f,	1.0f,	//0
		0.5f,		0.0f,		-0.5f,		0.10f,	0.01f,		0.0f,	1.0f,	1.0f,	//1
		0.0f,		-0.65f,		0.0f,		0.19f,	0.49f,		0.0f,	1.0f,	1.0f,	//2
		//bottom right		
		0.5f,		0.0f,		0.5f,		0.99f,   0.51f,		-1.0f,	1.0f,	0.0f,	//0
		0.5f,		0.0f,		-0.5f,		0.81f,	0.51f,		-1.0f,	1.0f,	0.0f,	//1
		0.0f,		-0.65f,		0.0f,		0.90f,	0.99f,		-1.0f,	1.0f,	0.0f,	//2
		//bottom left		
		-0.5f,		0.0f,		0.5f,		0.21f,   0.49f,		1.0f,	1.0f,	0.0f,	//0
		-0.5f,		0.0f,		-0.5f,		0.30f,	0.01f,		1.0f,	1.0f,	0.0f,	//1
		0.0f,		-0.65f,		0.0f,		0.39f,	0.49f,		1.0f,	1.0f,	0.0f,	//2

	};

	unsigned int paredIndices[] = {
		0, 1, 2,
		0, 2, 3
	};

	GLfloat paredVertices[] = {
		-1.0f, 0.0f, -1.0f,		0.0f, 0.0f,		-1.0f, 0.0f, 0.0f,
		-1.0f, 30.0f, -1.0f,	0.0f, 1.0f,		-1.0f, 0.0f, 0.0f,
		-1.0f, 30.0f, 1.0f,		1.0f, 1.0f,		-1.0f, 0.0f, 0.0f,
		-1.0f, 0.0f, 1.0f,		1.0f, 0.0f,		-1.0f, 0.0f, 0.0f
	};
	
	Mesh *obj1 = new Mesh();
	obj1->CreateMesh(vertices, indices, 32, 12);
	meshList.push_back(obj1);

	Mesh *obj2 = new Mesh();
	obj2->CreateMesh(vertices, indices, 32, 12);
	meshList.push_back(obj2);

	Mesh *obj3 = new Mesh();
	obj3->CreateMesh(floorVertices, floorIndices, 32, 6);
	meshList.push_back(obj3);

	Mesh* obj4 = new Mesh();
	obj4->CreateMesh(vegetacionVertices, vegetacionIndices, 64, 12);
	meshList.push_back(obj4);

	Mesh* obj5 = new Mesh();
	obj5->CreateMesh(dadoVertices, dadoIndices, 192, 24);
	meshList.push_back(obj5);

	Mesh* obj6 = new Mesh();
	obj6->CreateMesh(paredVertices, paredIndices, 36, 6);
	meshList.push_back(obj6);

	calcAverageNormals(indices, 12, vertices, 32, 8, 5);

	calcAverageNormals(vegetacionIndices, 12, vegetacionVertices, 64, 8, 5);

	calcAverageNormals(dadoIndices, 24, dadoVertices, 192, 8, 5);

}


void CreateShaders()
{
	Shader *shader1 = new Shader();
	shader1->CreateFromFiles(vShader, fShader);
	shaderList.push_back(*shader1);
}



int main()
{
	mainWindow = Window(1366, 768); // 1280, 1024 or 1024, 768
	mainWindow.Initialise();

	CreateObjects();
	CreateShaders();

	camera = Camera(glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(0.0f, 1.0f, 0.0f), -60.0f, 0.0f, 0.3f, 0.5f);

	brickTexture = Texture("Textures/brick.png");
	brickTexture.LoadTextureA();
	dirtTexture = Texture("Textures/dirt.png");
	dirtTexture.LoadTextureA();
	plainTexture = Texture("Textures/plain.png");
	plainTexture.LoadTextureA();
	pisoTexture = Texture("Textures/piso.tga");
	pisoTexture.LoadTextureA();
	AgaveTexture = Texture("Textures/Agave.tga");
	AgaveTexture.LoadTextureA();
	dadoTexture = Texture("Textures/textura_dado.jpg");
	dadoTexture.LoadTexture();

	Kitt_M = Model();
	Kitt_M.LoadModel("Models/kitt_optimizado.obj");
	Llanta_M = Model();
	Llanta_M.LoadModel("Models/Coche/llanta.obj");
	Elantra_M = Model();
	Elantra_M.LoadModel("Models/Coche/elantra.obj");
	Cofre_M = Model();
	Cofre_M.LoadModel("Models/Coche/cofre.obj");
	Blackhawk_M = Model();
	Blackhawk_M.LoadModel("Models/uh60.obj");
	Poste_M = Model();
	Poste_M.LoadModel("Models/poste_luz.obj");
	Hada_M = Model();
	Hada_M.LoadModel("Models/hada.obj");


	std::vector<std::string> skyboxFaces;
	skyboxFaces.push_back("Textures/Skybox/cupertin-lake_rt.tga");
	skyboxFaces.push_back("Textures/Skybox/cupertin-lake_lf.tga");
	skyboxFaces.push_back("Textures/Skybox/cupertin-lake_dn.tga");
	skyboxFaces.push_back("Textures/Skybox/cupertin-lake_up.tga");
	skyboxFaces.push_back("Textures/Skybox/cupertin-lake_bk.tga");
	skyboxFaces.push_back("Textures/Skybox/cupertin-lake_ft.tga");

	skybox = Skybox(skyboxFaces);

	Material_brillante = Material(4.0f, 256);
	Material_opaco = Material(0.3f, 4);


	//luz direccional, sólo 1 y siempre debe de existir
	mainLight = DirectionalLight(1.0f, 1.0f, 1.0f,
		0.3f, 0.3f,											//intensidad (se recomienda < 0.5) y tonlidad de la luz
		0.0f, 0.0f, 1.0f);									//dirección de la luz
	//contador de luces puntuales
	unsigned int pointLightCount = 0;

	//ARREGLO INICIAL
	//Declaración de primer luz puntual
	pointLights[0] = PointLight(1.0f, 0.0f, 0.0f,
		1.0f, 0.2f,											//radiación e intensidad
		0.0f, 1.5f, 1.5f,									//posición en el escenario
		0.3f, 0.2f, 0.1f);
	pointLightCount++;

	//luz del poste
	pointLights[1] = PointLight(1.0f, 1.0f, 1.0f,
		1.0f, 0.1f,
		-3.0f, 5.7f, -5.5,
		0.3f, 0.2f, 0.1f);
	pointLightCount++;

	//luz del hada
	pointLights[2] = PointLight(0.0f, 1.0f, 1.0f,
		0.5f, 0.1f,
		-10.0f, 2.0f, -6.0f,
		0.3f, 0.4f, 0.1f);
	pointLightCount++;

	//ARREGLO SIN LAMPARA
	pointLightsSLamp[0] = pointLights[0];
	pointLightsSLamp[1] = pointLights[2];


	//ARREGLO INICIAL SPOTLIGHTS

	unsigned int spotLightCount = 0;
	//linterna
	/*spotLights[0] = SpotLight(1.0f, 1.0f, 1.0f,
		0.0f, 2.0f,
		0.0f, 0.0f, 0.0f,
		0.0f, -1.0f, 0.0f,
		1.0f, 0.0f, 0.0f,
		5.0f);
	spotLightCount++;*/

	//faros azules
	spotLights[0] = SpotLight(0.0f, 0.0f, 1.0f,
		0.5f, 3.0f,											//segundo valor: que tan fuerte es el color desde el centro a su distancia máxima
		0.0f, 0.0f, 0.0f,									//posición en es escenario
		-1.0f, 0.0f, 0.0f,									//dirección de la luz
		1.0f, 0.0f, 0.0f,
		10.0f);												//ángulo de amplitud del cono de la luz
	spotLightCount++;

	//luz del helicóptero
	spotLights[1] = SpotLight(1.0f, 1.0f, 0.0f,
		0.5f, 1.0f,
		0.0f, 0.0f, 0.0f,
		0.0f, 0.0f, 0.0f,
		1.0f, 0.0f, 0.0f,
		20.0f);
	spotLightCount++;

	//luz del cofre
	spotLights[2] = SpotLight(0.0f, 1.0f, 0.0f,
		0.5f, 3.0f,
		0.0f, 0.0f, 0.0f,
		0.0f, -1.0f, 0.0f,
		1.0f, 0.0f, 0.0f,
		10.0f);
	spotLightCount++;

	//ARREGLO CUANDO EL CARRO AVANZA
	spotLightsD[0] = spotLights[0];
	spotLightsD[1] = spotLights[1];
	spotLightsD[2] = spotLights[2];
	//luz hacia adelante del carro
	spotLightsD[3] = SpotLight(1.0f, 0.0f, 0.0f,
		0.5f, 3.0f,											//segundo valor: que tan fuerte es el color desde el centro a su distancia máxima
		0.0f, 0.0f, 0.0f,									//posición en es escenario
		-1.0f, 0.0f, 0.0f,									//dirección de la luz
		1.0f, 0.0f, 0.0f,
		10.0f);												//ángulo de amplitud del cono de la luz
	spotLightCount++;

	//ARREGLO CUANDO EL CARRO RETROCEDE
	spotLightsR[0] = spotLights[0];
	spotLightsR[1] = spotLights[1];
	spotLightsR[2] = spotLights[2];
	//luz hacia adelante del carro
	spotLightsR[3] = SpotLight(1.0f, 0.0f, 0.0f,
		0.5f, 3.0f,											//segundo valor: que tan fuerte es el color desde el centro a su distancia máxima
		0.0f, 0.0f, 0.0f,									//posición en es escenario
		1.0f, 0.0f, 0.0f,									//dirección de la luz
		1.0f, 0.0f, 0.0f,
		10.0f);												//ángulo de amplitud del cono de la luz
	spotLightCount++;
	
	//se crean mas luces puntuales y spotlight 

	GLuint uniformProjection = 0, uniformModel = 0, uniformView = 0, uniformEyePosition = 0,
		uniformSpecularIntensity = 0, uniformShininess = 0;
	GLuint uniformColor = 0;
	glm::mat4 projection = glm::perspective(45.0f, (GLfloat)mainWindow.getBufferWidth() / mainWindow.getBufferHeight(), 0.1f, 1000.0f);
	////Loop mientras no se cierra la ventana
	while (!mainWindow.getShouldClose())
	{
		GLfloat now = glfwGetTime();
		deltaTime = now - lastTime;
		deltaTime += (now - lastTime) / limitFPS;
		lastTime = now;

		//Recibir eventos del usuario
		glfwPollEvents();
		camera.keyControl(mainWindow.getsKeys(), deltaTime);
		camera.mouseControl(mainWindow.getXChange(), mainWindow.getYChange());

		// Clear the window
		glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
		skybox.DrawSkybox(camera.calculateViewMatrix(), projection);
		shaderList[0].UseShader();
		uniformModel = shaderList[0].GetModelLocation();
		uniformProjection = shaderList[0].GetProjectionLocation();
		uniformView = shaderList[0].GetViewLocation();
		uniformEyePosition = shaderList[0].GetEyePositionLocation();
		uniformColor = shaderList[0].getColorLocation();
		
		//información en el shader de intensidad especular y brillo
		uniformSpecularIntensity = shaderList[0].GetSpecularIntensityLocation();
		uniformShininess = shaderList[0].GetShininessLocation();

		glUniformMatrix4fv(uniformProjection, 1, GL_FALSE, glm::value_ptr(projection));
		glUniformMatrix4fv(uniformView, 1, GL_FALSE, glm::value_ptr(camera.calculateViewMatrix()));
		glUniform3f(uniformEyePosition, camera.getCameraPosition().x, camera.getCameraPosition().y, camera.getCameraPosition().z);

		// luz ligada a la cámara de tipo flash
		//sirve para que en tiempo de ejecución (dentro del while) se cambien propiedades de la luz
			glm::vec3 lowerLight = camera.getCameraPosition();
			glm::vec3 lightDirection = glm::vec3(-1.0f, 0.0f, 0.0f); // Dirección inicial de la luz del cofre

			/*float maxAngle = glm::radians(45.0f); // Ángulo máximo en radianes
			float currentAngle = 0.0f; // Ángulo actual

			// Función para rotar un vector alrededor del eje Z
			glm::vec3 rotateVectorZ(glm::vec3 vector, float angle)
			{
				glm::mat4 rotationMatrix = glm::rotate(glm::mat4(1.0f), angle, glm::vec3(0.0f, 0.0f, 1.0f));
				glm::vec4 rotatedVector = rotationMatrix * glm::vec4(vector, 0.0f);
				return glm::vec3(rotatedVector);
			}

			lightDirection = rotateVectorZ(glm::vec3(-1.0f, 0.0f, 0.0f), currentAngle);
			spotLights[3].SetFlash(glm::vec3(10.799f + mainWindow.gettransx_carro(), 1.895f, 2.05f), lightDirection);*/

		lowerLight.y -= 0.3f;			//mueve la luz un poco debajo de la cámara
		//spotLights[0].SetFlash(lowerLight, camera.getCameraDirection());
		spotLights[0].SetPos(glm::vec3(10.0f + mainWindow.gettransx_carro(), 0.8f, 0.0f));	//se establece la posición dinámica
		spotLights[1].SetPos(glm::vec3(-1.0f + mainWindow.gettransx_helicopter(), 5.0f, 6.0f));	//spotlight del helicoptero
		spotLights[2].SetFlash(glm::vec3(10.799f + mainWindow.gettransx_carro(), 1.895f, 2.05f), glm::vec3(-1.0f, 0.0f, 0.0f));	//spotlight del cofre

		spotLightsD[0].SetPos(glm::vec3(10.0f + mainWindow.gettransx_carro(), 0.8f, 0.0f));	//se establece la posición dinámica
		spotLightsD[1].SetPos(glm::vec3(-1.0f + mainWindow.gettransx_helicopter(), 5.0f, 6.0f));	//spotlight del helicoptero
		spotLightsD[2].SetFlash(glm::vec3(10.799f + mainWindow.gettransx_carro(), 1.895f, 2.05f), glm::vec3(-1.0f, 0.0f, 0.0f));	//spotlight del cofre
		spotLightsD[3].SetPos(glm::vec3(10.0f + mainWindow.gettransx_carro(), 0.8f, 2.0f));

		spotLightsR[0].SetPos(glm::vec3(10.0f + mainWindow.gettransx_carro(), 0.8f, 0.0f));	//se establece la posición dinámica
		spotLightsR[1].SetPos(glm::vec3(-1.0f + mainWindow.gettransx_helicopter(), 5.0f, 6.0f));	//spotlight del helicoptero
		spotLightsR[2].SetFlash(glm::vec3(10.799f + mainWindow.gettransx_carro(), 1.895f, 2.05f), glm::vec3(-1.0f, 0.0f, 0.0f));	//spotlight del cofre
		spotLightsR[3].SetPos(glm::vec3(15.0f + mainWindow.gettransx_carro(), 0.8f, 2.0f));

		//información al shader de fuentes de iluminación
		shaderList[0].SetDirectionalLight(&mainLight);

		if (mainWindow.getmovimiento() == 1) {
			if (mainWindow.getdireccionCarro() == 0)
			{
				shaderList[0].SetSpotLights(spotLightsD, spotLightCount);
			}
			else if (mainWindow.getdireccionCarro() == 1)
			{
				shaderList[0].SetSpotLights(spotLightsR, spotLightCount);
			};
		}
		else if (mainWindow.getmovimiento() == 0) {
			shaderList[0].SetSpotLights(spotLights, spotLightCount);
		}
	
		if (mainWindow.getarregloLampara() == 0 && mainWindow.getarregloHada() == 0)
		{
			shaderList[0].SetPointLights(pointLights, pointLightCount);
		} 
		else if (mainWindow.getarregloLampara() == 1 && mainWindow.getarregloHada() == 0)
		{
			shaderList[0].SetPointLights(pointLightsSLamp, pointLightCount - 1);
		}
		else if (mainWindow.getarregloLampara() == 0 && mainWindow.getarregloHada() == 1)
		{
			shaderList[0].SetPointLights(pointLights, pointLightCount-1);
		}
		else if (mainWindow.getarregloLampara() == 1 && mainWindow.getarregloHada() == 1)
		{
			shaderList[0].SetPointLights(pointLightsSLamp, pointLightCount - 2);
		};

		glm::mat4 model(1.0);
		glm::mat4 modelaux(1.0);
		glm::mat4 posInicial(1.0);
		glm::vec3 color = glm::vec3(1.0f, 1.0f, 1.0f);

		model = glm::mat4(1.0);
		model = glm::translate(model, glm::vec3(0.0f, -1.0f, 0.0f));
		model = glm::scale(model, glm::vec3(30.0f, 1.0f, 30.0f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		glUniform3fv(uniformColor, 1, glm::value_ptr(color));

		pisoTexture.UseTexture();
		Material_opaco.UseMaterial(uniformSpecularIntensity, uniformShininess);

		meshList[2]->RenderMesh();

		model = glm::mat4(1.0);
		model = glm::translate(model, glm::vec3(0.0f, -1.0f, 0.0f));
		model = glm::scale(model, glm::vec3(30.0f, 1.0f, 30.0f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		glUniform3fv(uniformColor, 1, glm::value_ptr(color));
		pisoTexture.UseTexture();
		Material_opaco.UseMaterial(uniformSpecularIntensity, uniformShininess);
		meshList[5]->RenderMesh();

		//Instancia del coche 
		model = glm::mat4(1.0);
		model = glm::translate(model, glm::vec3(14.0f + mainWindow.gettransx_carro(), 1.15f, 2.0f)); //Traslación con el teclado
		modelaux = model;
		model = glm::scale(model, glm::vec3(0.7f, 0.7f, 0.7f));
		model = glm::rotate(model, -90 * toRadians, glm::vec3(0.0f, 1.0f, 0.0f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		Elantra_M.RenderModel();

		//Cofre
		model = modelaux;
		model = glm::translate(model, glm::vec3(-3.201f, 0.745f, 0.05f));
		model = glm::scale(model, glm::vec3(0.7f, 0.7f, 0.7f));
		model = glm::rotate(model, -90 * toRadians, glm::vec3(0.0f, 1.0f, 0.0f));
		model = glm::rotate(model, glm::radians(mainWindow.getarticulacion1()), glm::vec3(-1.0f, 0.0f, 0.0f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		Cofre_M.RenderModel();

		//Llanta delantera izquierda
		model = modelaux;
		model = glm::translate(model, glm::vec3(-4.2f, -1.15f, 2.4f));
		model = glm::scale(model, glm::vec3(0.7f, 0.7f, 0.7f));
		model = glm::rotate(model, -90 * toRadians, glm::vec3(0.0f, 1.0f, 0.0f));
		model = glm::rotate(model, glm::radians(mainWindow.getarticulacion2()), glm::vec3(1.0f, 0.0f, 0.0f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		Llanta_M.RenderModel();	

		//Llanta trasera izquierda
		model = modelaux;
		model = glm::translate(model, glm::vec3(4.22f, -1.15f, 2.4f));
		model = glm::scale(model, glm::vec3(0.7f, 0.7f, 0.7f));
		model = glm::rotate(model, -90 * toRadians, glm::vec3(0.0f, 1.0f, 0.0f));
		model = glm::rotate(model, glm::radians(mainWindow.getarticulacion2()), glm::vec3(1.0f, 0.0f, 0.0f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		Llanta_M.RenderModel();

		//Llanta delantera derecha
		model = modelaux;
		model = glm::translate(model, glm::vec3(-4.2f, -1.15f, -2.4f));
		model = glm::scale(model, glm::vec3(0.7f, 0.7f, 0.7f));
		model = glm::rotate(model, 90 * toRadians, glm::vec3(0.0f, 1.0f, 0.0f));
		model = glm::rotate(model, glm::radians(mainWindow.getarticulacion2()), glm::vec3(1.0f, 0.0f, 0.0f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		Llanta_M.RenderModel();

		//Llanta trasera derecha
		model = modelaux;
		model = glm::translate(model, glm::vec3(4.22f, -1.15f, -2.4f));
		model = glm::scale(model, glm::vec3(0.7f, 0.7f, 0.7f));
		model = glm::rotate(model, 90 * toRadians, glm::vec3(0.0f, 1.0f, 0.0f));
		model = glm::rotate(model, glm::radians(mainWindow.getarticulacion2()), glm::vec3(1.0f, 0.0f, 0.0f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		Llanta_M.RenderModel();
	
		//helicoptero
		model = glm::mat4(1.0);
		model = glm::translate(model, glm::vec3(0.0f + mainWindow.gettransx_helicopter(), 5.0f, 6.0));
		model = glm::scale(model, glm::vec3(0.3f, 0.3f, 0.3f));
		model = glm::rotate(model, -90 * toRadians, glm::vec3(1.0f, 0.0f, 0.0f));
		model = glm::rotate(model, 90 * toRadians, glm::vec3(0.0f, 0.0f, 1.0f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		Blackhawk_M.RenderModel();

		//poste de luz
		model = glm::mat4(1.0);
		model = glm::translate(model, glm::vec3(-3.0f, 3.7f, -6.0));
		model = glm::scale(model, glm::vec3(0.5f, 0.5f, 0.5f));
		model = glm::rotate(model, 180 * toRadians, glm::vec3(0.0f, 1.0f, 0.0f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		Poste_M.RenderModel();

		//dado
		model = glm::mat4(1.0);
		model = glm::translate(model, glm::vec3(-1.5f, 4.5f, -2.0f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		dadoTexture.UseTexture();
		meshList[4]->RenderMesh();

		//hada
		model = glm::mat4(1.0);
		model = glm::translate(model, glm::vec3(-10.0f, 2.0f, -6.0f));
		model = glm::scale(model, glm::vec3(0.5f, 0.5f, 0.5f));
		//model = glm::rotate(model, 180 * toRadians, glm::vec3(0.0f, 1.0f, 0.0f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		Hada_M.RenderModel();

		//Agave ¿qué sucede si lo renderizan antes del coche y el helicóptero?
		model = glm::mat4(1.0);
		model = glm::translate(model, glm::vec3(0.0f, 1.0f, -4.0f));
		model = glm::scale(model, glm::vec3(4.0f, 4.0f, 4.0f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		
				
		//blending: transparencia o traslucidez
		glEnable(GL_BLEND);
		glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
		AgaveTexture.UseTexture();
		Material_opaco.UseMaterial(uniformSpecularIntensity, uniformShininess);
		meshList[3]->RenderMesh();
		glDisable(GL_BLEND);

		glUseProgram(0);

		mainWindow.swapBuffers();
	}

	return 0;
}
