#define _USE_MATH_DEFINES
#include <cmath>
//glew include
#include <GL/glew.h>

//std includes
#include <string>
#include <iostream>
#include <stdlib.h>
// contains new std::shuffle definition
#include <algorithm>
#include <random>

//glfw include
#include <GLFW/glfw3.h>

// program include
#include "Headers/TimeManager.h"

// Shader include
#include "Headers/Shader.h"

// Model geometric includes
#include "Headers/Sphere.h"
#include "Headers/Cylinder.h"
#include "Headers/Box.h"
#include "Headers/FirstPersonCamera.h"
#include "Headers/ThirdPersonCamera.h"

//GLM include
#define GLM_FORCE_RADIANS
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include "Headers/Texture.h"

// Include loader Model class
#include "Headers/Model.h"
// Font rendering
#include "Headers/FontTypeRendering.h"

// Include Terrain
#include "Headers/Terrain.h"

#include "Headers/AnimationUtils.h"

// Include Colision headers functions
#include "Headers/Colisiones.h"

// OpenAL include
#include <AL/alut.h>

#define ARRAY_SIZE_IN_ELEMENTS(a) (sizeof(a)/sizeof(a[0]))

int screenWidth;
int screenHeight;

const unsigned int SHADOW_WIDTH = 2048, SHADOW_HEIGHT = 2048;

GLFWwindow *window;

Shader shader;
//Shader con skybox
Shader shaderSkybox;
//Shader con multiples luces
Shader shaderMulLighting;
Shader shaderMulLighting2;
//Shader para el terreno
Shader shaderTerrain;
//Shader para las particulas de fountain
Shader shaderParticlesFountain;
//Shader para las particulas de fuego
Shader shaderParticlesFire;
//Shader para visualizar el buffer de profundidad
Shader shaderViewDepth;
//Shader para dibujar el buffer de profunidad
Shader shaderDepth;

std::shared_ptr<Camera> camera(new ThirdPersonCamera());
std::shared_ptr<Camera> camerafij(new ThirdPersonCamera());
float distanceFromTarget = 8.0;

Sphere skyboxSphere(20, 20);
Box boxCollider;
Sphere sphereCollider(10, 10);
Box boxViewDepth;
Box boxLightViewBox;


//Pantallas
Model pantalla1;
Model pantalla2;
Model pantalla3;
Model pantalla4;
Model pantalla5;
Model pantalla6;

// Lamps
Model modelLamp1;
Model modelLamp2;
Model modelLamp3;
// Piedras
Model Piedras1;
Model Rocas1;
Model Rocas2;
Model Hongos;
// Hierba
Model modelGrass;
// Model animate instance
// Prota
Model protaFI;
Model protaFI1;
Model protaFI2;
Model protaFI3;
Model parteNave1;
Model parteNave2;
Model parteNave3;
Model parteNave4;
Model naveCompleta;
Model HongoRecio;
// Terrain model instance
Terrain terrain(-1, -1, 200, 16, "../Textures/heightmap1.png");
Terrain terrain2(-1, -1, 200, 25, "../Textures/heightmap2.png");
GLuint textureTerrainBackgroundID, textureTerrainRID, textureTerrainGID, textureTerrainBID, textureTerrainBlendMapID, textureTerrainBlendMapID2;
GLuint textureParticleFountainID, textureParticleFireID, texId;
GLuint skyboxTextureID;

// Modelo para el redener de texto
//FontTypeRendering::FontTypeRendering *modelText;

GLenum types[6] = {
GL_TEXTURE_CUBE_MAP_POSITIVE_X,
GL_TEXTURE_CUBE_MAP_NEGATIVE_X,
GL_TEXTURE_CUBE_MAP_POSITIVE_Y,
GL_TEXTURE_CUBE_MAP_NEGATIVE_Y,
GL_TEXTURE_CUBE_MAP_POSITIVE_Z,
GL_TEXTURE_CUBE_MAP_NEGATIVE_Z };

std::string fileNames[6] = { "../Textures/blue/bkg1_right.png",
		"../Textures/blue/bkg1_left.png",
		"../Textures/blue/bkg1_top.png",
		"../Textures/blue/bkg1_bot.png",
		"../Textures/blue/bkg1_front.png",
		"../Textures/blue/bkg1_back.png" };

bool exitApp = false;
int lastMousePosX, offsetX = 0;
int lastMousePosY, offsetY = 0;
FontTypeRendering::FontTypeRendering *modelText;
FontTypeRendering::FontTypeRendering *modelText1;
// Model matrix definitions
glm::mat4 modelMatrixPantalla1 = glm::mat4(1.0f);
glm::mat4 modelMatrixPantalla2 = glm::mat4(1.0f);
glm::mat4 modelMatrixPantalla3 = glm::mat4(1.0f);
glm::mat4 modelMatrixPantalla4 = glm::mat4(1.0f);
glm::mat4 modelMatrixPantalla5 = glm::mat4(1.0f);
glm::mat4 modelMatrixPantalla6 = glm::mat4(1.0f);
glm::mat4 modelMatrixParteNave1 = glm::mat4(1.0f);
glm::mat4 modelMatrixParteNave2 = glm::mat4(1.0f);
glm::mat4 modelMatrixParteNave3 = glm::mat4(1.0f);
glm::mat4 modelMatrixParteNave4 = glm::mat4(1.0f);
glm::mat4 modelMatrixProta = glm::mat4(1.0f);
glm::mat4 modelMatrixNaveCompleta = glm::mat4(1.0f);
glm::mat4 modelMatrixHongoRecio = glm::mat4(1.0);

int animationIndex = 2;
float rotDartHead = 0.0, rotDartLeftArm = 0.0, rotDartLeftHand = 0.0, rotDartRightArm = 0.0, rotDartRightHand = 0.0, rotDartLeftLeg = 0.0, rotDartRightLeg = 0.0;
int modelSelected = 2;
bool enableCountSelected = true;
float a = rand() % (1 - 3);
float c = rand() % (1 - 3);
float b = rand() % (1 - 3);
bool parte1, parte2, parte3, parte4 = false;
float reloj1 = 0.0;
float reloj2 = 0.0;
bool primeraCol, segundaCol, terceraCol = false;
int segundoLev = 0;
int contador = 0;
int contadorlv0 = 0;
int contadorlv2 = 0;
int nostart = 0;
// Lamps positions
std::vector<glm::vec3> lamp1Position = { glm::vec3(
		35.17, 0, 80.11), glm::vec3(38.16, 0, 78.11) , glm::vec3(41.16, 0, -72.11) , glm::vec3(43.16, 0, 70.11) , glm::vec3(-45.16, 0, 81.11) , glm::vec3(28.16, 0, 83.11),
		glm::vec3(0,0,30), glm::vec3(0,0,92), glm::vec3(0,0,-94), glm::vec3(0,0,-88), glm::vec3(0,0,66), glm::vec3(-2,0,-42),glm::vec3(-2,0,88), glm::vec3(-27,0,-92),glm::vec3(-25,0,78),glm::vec3(-45,0,60),
		glm::vec3(-70,0,85), glm::vec3(-82,0,91), glm::vec3(-65,0,88), glm::vec3(-70,0,70), glm::vec3(-30,0,55), glm::vec3(-36,0,-45) };
std::vector<float> lamp1Orientation = { -17.0, -82.67, 23.70 };
std::vector<glm::vec3> lamp2Position = { glm::vec3(-88, 0, -22),
		glm::vec3(-91, 0, -85), glm::vec3(-93, 0, 28), glm::vec3(-89, 0, 26),
		glm::vec3(30,0,-60),glm::vec3(28,0,-17), glm::vec3(-58,0,-17), glm::vec3(24,0,11),glm::vec3(64,0,-11),glm::vec3(25,0,5),glm::vec3(65,0,-5),glm::vec3(-22,0,70),
		glm::vec3(22,0,-80),glm::vec3(-26,0,-2),glm::vec3(26,0,-2), };
std::vector<float> lamp2Orientation = { 21.37 + 90, -65.0 + 90 };
std::vector<glm::vec3> lamp3Position = { glm::vec3(-80, 0, 65),
		glm::vec3(-50, 0, 39), glm::vec3(-80, 0, -65), glm::vec3(-40, 0, -30), glm::vec3(80, 0, -65), glm::vec3(40, 0, -30),
		glm::vec3(80, 0, 65),glm::vec3(40, 0, 30),glm::vec3(0,0,-17), glm::vec3(0,0,27) };
std::vector<float> lamp3Orientation = { 51.37 + 90, -95.0 + 10 };

std::vector<glm::vec3> rocas = { glm::vec3(-95 + rand() % (1 - 190), 0,-95 + rand() % (1 - 190)),
		glm::vec3(-95 + rand() % (1 - 190), 0,-95 + rand() % (1 - 190)), glm::vec3(-95 + rand() % (1 - 190), 0,-95 + rand() % (1 - 190)),
		glm::vec3(-95 + rand() % (1 - 190), 0,-95 + rand() % (1 - 190)), glm::vec3(-95 + rand() % (1 - 190), 0,-95 + rand() % (1 - 190)),
		glm::vec3(-95 + rand() % (1 - 190), 0,-95 + rand() % (1 - 190)),
		glm::vec3(-95 + rand() % (1 - 190), 0,-95 + rand() % (1 - 190)),
		glm::vec3(-95 + rand() % (1 - 190), 0,-95 + rand() % (1 - 190)), glm::vec3(-95 + rand() % (1 - 190), 0,-95 + rand() % (1 - 190)),
		glm::vec3(-95 + rand() % (1 - 190), 0,-95 + rand() % (1 - 190)) };
std::vector<float> rocasOrientation = { 51.37 + 90, -95.0 + 10 };

std::vector<glm::vec3> rocas1 = { glm::vec3(0,0,70), glm::vec3(0,0,-70),glm::vec3(-70,0,30),glm::vec3(70,0,-30),glm::vec3(40,0,40) };
std::vector<float> rocas1Orientation = { 51.37 + 90, -95.0 + 10 };

std::vector<glm::vec3> rocas2 = { glm::vec3(-30,0,73),glm::vec3(80,0,0),glm::vec3(-80,0,0),glm::vec3(20,0,-68),glm::vec3(-50,0,65) };
std::vector<float> rocas2Orientation = { 51.37 + 90, -95.0 + 10 };

std::vector<glm::vec3> Hongos2 = { glm::vec3(-50,0,-50),glm::vec3(-76,0,47),glm::vec3(-20,0,-75),glm::vec3(20,0,75),glm::vec3(52,0,-59), glm::vec3(72,0,47) };
std::vector<float> Hongos2Orientation = { 51.37 + 90, -95.0 + 10 };

// Blending model unsorted
std::map<std::string, glm::vec3> blendingUnsorted = {
		{"fire", glm::vec3(0.0, 0.0, 7.0)}
};

double deltaTime;
double currTime, lastTime;

//Variable para poder saltar
bool isJump = false;
float gravity = 1.81;
double tmv = 0;
double startTimeJump = 0;
int contadorTiempo = 0;
int contadorTiempo2 = 0;
// Definition for the particle system
GLuint initVel, startTime;
GLuint VAOParticles;
GLuint nParticles = 8000;
double currTimeParticlesAnimation, lastTimeParticlesAnimation;

// Definition for the particle system fire
GLuint initVelFire, startTimeFire;
GLuint VAOParticlesFire;
GLuint nParticlesFire = 2000;
GLuint posBuf[2], velBuf[2], age[2];
GLuint particleArray[2];
GLuint feedback[2];
GLuint drawBuf = 1;
float particleSize = 0.9, particleLifetime = 2.0;
double currTimeParticlesAnimationFire, lastTimeParticlesAnimationFire;
int banderadetrescolores = 0;
int banderadetrescolores1 = 0;
int banderadetrescolores2 = 0;
int banderadetrescolores3 = 0;
int nivel = 0;
int controles = 0;
float arreciado = -2.0;
// Colliders
std::map<std::string, std::tuple<AbstractModel::OBB, glm::mat4, glm::mat4> > collidersOBB;
std::map<std::string, std::tuple<AbstractModel::SBB, glm::mat4, glm::mat4> > collidersSBB;

// Framesbuffers
GLuint depthMap, depthMapFBO;

/**********************
 * OpenAL config
 */

 // OpenAL Defines
#define NUM_BUFFERS 5
#define NUM_SOURCES 5
#define NUM_BUFFERS1 3
#define NUM_SOURCES1 3
#define NUM_ENVIRONMENTS 1
// Listener
ALfloat listenerPos[] = { 0.0, 0.0, 4.0 };
ALfloat listenerVel[] = { 0.0, 0.0, 0.0 };
ALfloat listenerOri[] = { 0.0, 0.0, 1.0, 0.0, 1.0, 0.0 };
// Source 0
ALfloat source0Pos[] = { -2.0, 0.0, 0.0 };
ALfloat source0Vel[] = { 0.0, 0.0, 0.0 };
// Source 1
ALfloat source1Pos[] = { 88.0, 0.0, -75.0 };
ALfloat source1Vel[] = { 0.0, 0.0, 0.0 };
// Source 2
ALfloat source2Pos[] = { 74.0, 0.0, 45.0 };
ALfloat source2Vel[] = { 0.0, 0.0, 0.0 };
ALfloat source3Pos[] = { -15.0, 0.0, -25.0 };
ALfloat source3Vel[] = { 0.0, 0.0, 0.0 };
ALfloat source4Pos[] = { -40.0, 0.0, 30.0 };
ALfloat source4Vel[] = { 0.0, 0.0, 0.0 };


// Source 1
ALfloat source10Pos[] = { 52.0,0.0,72.0 };
ALfloat source10Vel[] = { 0.0, 0.0, 0.0 };
// Buffers
ALuint buffer[NUM_BUFFERS];
ALuint source[NUM_SOURCES];
ALuint buffer1[NUM_BUFFERS];
ALuint source1[NUM_SOURCES];
ALuint environment[NUM_ENVIRONMENTS];
// Configs
ALsizei size, freq;
ALenum format;
ALvoid *data;
int ch;
ALboolean loop;
std::vector<bool> sourcesPlay = { true, true, true ,true, true };
std::vector<bool> sourcesPlay1 = { true, true, true };
// Se definen todos las funciones.
void reshapeCallback(GLFWwindow *Window, int widthRes, int heightRes);
void keyCallback(GLFWwindow *window, int key, int scancode, int action,
	int mode);
void mouseCallback(GLFWwindow *window, double xpos, double ypos);
void mouseButtonCallback(GLFWwindow *window, int button, int state, int mod);
void scrollCallback(GLFWwindow* window, double xoffset, double yoffset);
void initParticleBuffers();
void init(int width, int height, std::string strTitle, bool bFullScreen);
void destroy();
bool processInput(bool continueApplication = true);
void prepareScene();
void prepareDepthScene();
void renderScene(bool renderParticles = true);

void initParticleBuffers() {
	// Generate the buffers
	glGenBuffers(1, &initVel);   // Initial velocity buffer
	glGenBuffers(1, &startTime); // Start time buffer

	// Allocate space for all buffers
	int size = nParticles * 3 * sizeof(float);
	glBindBuffer(GL_ARRAY_BUFFER, initVel);
	glBufferData(GL_ARRAY_BUFFER, size, NULL, GL_STATIC_DRAW);
	glBindBuffer(GL_ARRAY_BUFFER, startTime);
	glBufferData(GL_ARRAY_BUFFER, nParticles * sizeof(float), NULL, GL_STATIC_DRAW);

	// Fill the first velocity buffer with random velocities
	glm::vec3 v(0.0f);
	float velocity, theta, phi;
	GLfloat *data = new GLfloat[nParticles * 3];
	for (unsigned int i = 0; i < nParticles; i++) {

		theta = glm::mix(0.0f, glm::pi<float>() / 6.0f, ((float)rand() / RAND_MAX));
		phi = glm::mix(0.0f, glm::two_pi<float>(), ((float)rand() / RAND_MAX));

		v.x = sinf(theta) * cosf(phi);
		v.y = cosf(theta);
		v.z = sinf(theta) * sinf(phi);

		velocity = glm::mix(0.6f, 0.8f, ((float)rand() / RAND_MAX));
		v = glm::normalize(v) * velocity;

		data[3 * i] = v.x;
		data[3 * i + 1] = v.y;
		data[3 * i + 2] = v.z;
	}
	glBindBuffer(GL_ARRAY_BUFFER, initVel);
	glBufferSubData(GL_ARRAY_BUFFER, 0, size, data);

	// Fill the start time buffer
	delete[] data;
	data = new GLfloat[nParticles];
	float time = 0.0f;
	float rate = 0.00075f;
	for (unsigned int i = 0; i < nParticles; i++) {
		data[i] = time;
		time += rate;
	}
	glBindBuffer(GL_ARRAY_BUFFER, startTime);
	glBufferSubData(GL_ARRAY_BUFFER, 0, nParticles * sizeof(float), data);

	glBindBuffer(GL_ARRAY_BUFFER, 0);
	delete[] data;

	glGenVertexArrays(1, &VAOParticles);
	glBindVertexArray(VAOParticles);
	glBindBuffer(GL_ARRAY_BUFFER, initVel);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, NULL);
	glEnableVertexAttribArray(0);

	glBindBuffer(GL_ARRAY_BUFFER, startTime);
	glVertexAttribPointer(1, 1, GL_FLOAT, GL_FALSE, 0, NULL);
	glEnableVertexAttribArray(1);

	glBindVertexArray(0);
}

void initParticleBuffersFire() {
	// Generate the buffers
	glGenBuffers(2, posBuf);    // position buffers
	glGenBuffers(2, velBuf);    // velocity buffers
	glGenBuffers(2, age);       // age buffers

	// Allocate space for all buffers
	int size = nParticlesFire * sizeof(GLfloat);
	glBindBuffer(GL_ARRAY_BUFFER, posBuf[0]);
	glBufferData(GL_ARRAY_BUFFER, 3 * size, 0, GL_DYNAMIC_COPY);
	glBindBuffer(GL_ARRAY_BUFFER, posBuf[1]);
	glBufferData(GL_ARRAY_BUFFER, 3 * size, 0, GL_DYNAMIC_COPY);
	glBindBuffer(GL_ARRAY_BUFFER, velBuf[0]);
	glBufferData(GL_ARRAY_BUFFER, 3 * size, 0, GL_DYNAMIC_COPY);
	glBindBuffer(GL_ARRAY_BUFFER, velBuf[1]);
	glBufferData(GL_ARRAY_BUFFER, 3 * size, 0, GL_DYNAMIC_COPY);
	glBindBuffer(GL_ARRAY_BUFFER, age[0]);
	glBufferData(GL_ARRAY_BUFFER, size, 0, GL_DYNAMIC_COPY);
	glBindBuffer(GL_ARRAY_BUFFER, age[1]);
	glBufferData(GL_ARRAY_BUFFER, size, 0, GL_DYNAMIC_COPY);

	// Fill the first age buffer
	std::vector<GLfloat> initialAge(nParticlesFire);
	float rate = particleLifetime / nParticlesFire;
	for (unsigned int i = 0; i < nParticlesFire; i++) {
		int index = i - nParticlesFire;
		float result = rate * index;
		initialAge[i] = result;
	}
	// Shuffle them for better looking results
	//Random::shuffle(initialAge);
	auto rng = std::default_random_engine{};
	std::shuffle(initialAge.begin(), initialAge.end(), rng);
	glBindBuffer(GL_ARRAY_BUFFER, age[0]);
	glBufferSubData(GL_ARRAY_BUFFER, 0, size, initialAge.data());

	glBindBuffer(GL_ARRAY_BUFFER, 0);

	// Create vertex arrays for each set of buffers
	glGenVertexArrays(2, particleArray);

	// Set up particle array 0
	glBindVertexArray(particleArray[0]);
	glBindBuffer(GL_ARRAY_BUFFER, posBuf[0]);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, 0);
	glEnableVertexAttribArray(0);

	glBindBuffer(GL_ARRAY_BUFFER, velBuf[0]);
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 0, 0);
	glEnableVertexAttribArray(1);

	glBindBuffer(GL_ARRAY_BUFFER, age[0]);
	glVertexAttribPointer(2, 1, GL_FLOAT, GL_FALSE, 0, 0);
	glEnableVertexAttribArray(2);

	// Set up particle array 1
	glBindVertexArray(particleArray[1]);
	glBindBuffer(GL_ARRAY_BUFFER, posBuf[1]);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, 0);
	glEnableVertexAttribArray(0);

	glBindBuffer(GL_ARRAY_BUFFER, velBuf[1]);
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 0, 0);
	glEnableVertexAttribArray(1);

	glBindBuffer(GL_ARRAY_BUFFER, age[1]);
	glVertexAttribPointer(2, 1, GL_FLOAT, GL_FALSE, 0, 0);
	glEnableVertexAttribArray(2);

	glBindVertexArray(0);

	// Setup the feedback objects
	glGenTransformFeedbacks(2, feedback);

	// Transform feedback 0
	glBindTransformFeedback(GL_TRANSFORM_FEEDBACK, feedback[0]);
	glBindBufferBase(GL_TRANSFORM_FEEDBACK_BUFFER, 0, posBuf[0]);
	glBindBufferBase(GL_TRANSFORM_FEEDBACK_BUFFER, 1, velBuf[0]);
	glBindBufferBase(GL_TRANSFORM_FEEDBACK_BUFFER, 2, age[0]);

	// Transform feedback 1
	glBindTransformFeedback(GL_TRANSFORM_FEEDBACK, feedback[1]);
	glBindBufferBase(GL_TRANSFORM_FEEDBACK_BUFFER, 0, posBuf[1]);
	glBindBufferBase(GL_TRANSFORM_FEEDBACK_BUFFER, 1, velBuf[1]);
	glBindBufferBase(GL_TRANSFORM_FEEDBACK_BUFFER, 2, age[1]);

	glBindTransformFeedback(GL_TRANSFORM_FEEDBACK, 0);
}

// Implementacion de todas las funciones.
void init(int width, int height, std::string strTitle, bool bFullScreen) {

	if (!glfwInit()) {
		std::cerr << "Failed to initialize GLFW" << std::endl;
		exit(-1);
	}

	screenWidth = width;
	screenHeight = height;

	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

	if (bFullScreen)
		window = glfwCreateWindow(width, height, strTitle.c_str(),
			glfwGetPrimaryMonitor(), nullptr);
	else
		window = glfwCreateWindow(width, height, strTitle.c_str(), nullptr,
			nullptr);

	if (window == nullptr) {
		std::cerr
			<< "Error to create GLFW window, you can try download the last version of your video card that support OpenGL 3.3+"
			<< std::endl;
		destroy();
		exit(-1);
	}

	glfwMakeContextCurrent(window);
	glfwSwapInterval(0);

	glfwSetWindowSizeCallback(window, reshapeCallback);
	glfwSetKeyCallback(window, keyCallback);
	glfwSetCursorPosCallback(window, mouseCallback);
	glfwSetMouseButtonCallback(window, mouseButtonCallback);
	glfwSetScrollCallback(window, scrollCallback);
	glfwSetInputMode(window, GLFW_STICKY_KEYS, GL_TRUE);

	// Init glew
	glewExperimental = GL_TRUE;
	GLenum err = glewInit();
	if (GLEW_OK != err) {
		std::cerr << "Failed to initialize glew" << std::endl;
		exit(-1);
	}

	glViewport(0, 0, screenWidth, screenHeight);
	glClearColor(0.0f, 0.0f, 0.4f, 0.0f);

	glEnable(GL_DEPTH_TEST);
	//glEnable(GL_CULL_FACE);

	// Inicialización de los shaders
	shader.initialize("../Shaders/colorShader.vs", "../Shaders/colorShader.fs");
	shaderSkybox.initialize("../Shaders/skyBox.vs", "../Shaders/skyBox_fog.fs");
	shaderMulLighting.initialize("../Shaders/iluminacion_textura_animation_fog.vs", "../Shaders/multipleLights_Fog.fs");
	shaderMulLighting2.initialize("../Shaders/iluminacion_textura_animation.vs", "../Shaders/multipleLights.fs");
	shaderTerrain.initialize("../Shaders/terrain_fog.vs", "../Shaders/terrain_fog.fs");
	shaderParticlesFountain.initialize("../Shaders/particlesFountain.vs", "../Shaders/particlesFountain.fs");
	shaderParticlesFire.initialize("../Shaders/particlesFire.vs", "../Shaders/particlesFire.fs", { "Position", "Velocity", "Age" });
	shaderViewDepth.initialize("../Shaders/texturizado.vs", "../Shaders/texturizado_depth_view.fs");
	shaderDepth.initialize("../Shaders/shadow_mapping_depth.vs", "../Shaders/shadow_mapping_depth.fs");

	// Inicializacion de los objetos.
	skyboxSphere.init();
	skyboxSphere.setShader(&shaderSkybox);
	skyboxSphere.setScale(glm::vec3(100.0f, 100.0f, 100.0f));

	boxCollider.init();
	boxCollider.setShader(&shader);
	boxCollider.setColor(glm::vec4(1.0, 1.0, 1.0, 1.0));

	sphereCollider.init();
	sphereCollider.setShader(&shader);
	sphereCollider.setColor(glm::vec4(1.0, 1.0, 1.0, 1.0));

	boxViewDepth.init();
	boxViewDepth.setShader(&shaderViewDepth);

	boxLightViewBox.init();
	boxLightViewBox.setShader(&shaderViewDepth);
	modelMatrixNaveCompleta = glm::translate(modelMatrixNaveCompleta, glm::vec3(52.0, 0.0, 72.0));


	terrain.init();
	terrain.setShader(&shaderTerrain);
	terrain.setPosition(glm::vec3(100, 0, 100));

	terrain2.init();
	terrain2.setShader(&shaderTerrain);
	terrain2.setPosition(glm::vec3(100, 0, 100));
	//carga de partes de la nave
	parteNave1.loadModel("../models/Parte1.obj");
	parteNave1.setShader(&shaderMulLighting);

	parteNave2.loadModel("../models/Parte2.obj");
	parteNave2.setShader(&shaderMulLighting);

	parteNave3.loadModel("../models/Parte3.obj");
	parteNave3.setShader(&shaderMulLighting);

	parteNave4.loadModel("../models/Parte4.obj");
	parteNave4.setShader(&shaderMulLighting);

	naveCompleta.loadModel("../models/NaveFinal.obj");
	naveCompleta.setShader(&shaderMulLighting);

	//Pantallas
	pantalla1.loadModel("../models/L1.obj");
	pantalla1.setShader(&shaderMulLighting2);
	pantalla2.loadModel("../models/L2.obj");
	pantalla2.setShader(&shaderMulLighting2);
	pantalla3.loadModel("../models/L3.obj");
	pantalla3.setShader(&shaderMulLighting2);
	pantalla4.loadModel("../models/L4.obj");
	pantalla4.setShader(&shaderMulLighting2);
	pantalla5.loadModel("../models/L5.obj");
	pantalla5.setShader(&shaderMulLighting2);
	pantalla6.loadModel("../models/L6.obj");
	pantalla6.setShader(&shaderMulLighting2);

	//Lamp models
	modelLamp1.loadModel("../models/Hongo2.obj");
	modelLamp1.setShader(&shaderMulLighting);
	modelLamp2.loadModel("../models/Hongo1.obj");
	modelLamp2.setShader(&shaderMulLighting);
	modelLamp3.loadModel("../models/Hongo3.obj");
	modelLamp3.setShader(&shaderMulLighting);
	Piedras1.loadModel("../models/Roca3.obj");
	Piedras1.setShader(&shaderMulLighting);
	Hongos.loadModel("../models/Hongo4.obj");
	Hongos.setShader(&shaderMulLighting);
	Rocas1.loadModel("../models/Roca1.obj");
	Rocas1.setShader(&shaderMulLighting);
	Rocas2.loadModel("../models/Roca4.obj");
	Rocas2.setShader(&shaderMulLighting);
	HongoRecio.loadModel("../models/Hongo1.obj");
	HongoRecio.setShader(&shaderMulLighting);
	//Grass
	modelGrass.loadModel("../models/grass/grassModel.obj");
	modelGrass.setShader(&shaderMulLighting);
	protaFI.loadModel("../models/ProtaFI1.fbx");
	protaFI.setShader(&shaderMulLighting);

	protaFI1.loadModel("../models/ProtaFI2.fbx");
	protaFI1.setShader(&shaderMulLighting);
	protaFI2.loadModel("../models/ProtaFI3.fbx");
	protaFI2.setShader(&shaderMulLighting);
	protaFI3.loadModel("../models/ProtaFI4.fbx");
	protaFI3.setShader(&shaderMulLighting);

	camera->setPosition(glm::vec3(0.0, 0.0, 10.0));
	camera->setDistanceFromTarget(distanceFromTarget);
	camera->setSensitivity(1.0);

	camerafij->setPosition(glm::vec3(0.0, 0.0, 10.0));
	camerafij->setDistanceFromTarget(distanceFromTarget);
	camerafij->setSensitivity(1.0);

	// Definimos el tamanio de la imagen
	int imageWidth, imageHeight;
	FIBITMAP *bitmap;
	unsigned char *data;

	// Carga de texturas para el skybox
	Texture skyboxTexture = Texture("");
	glGenTextures(1, &skyboxTextureID);
	// Tipo de textura CUBE MAP
	glBindTexture(GL_TEXTURE_CUBE_MAP, skyboxTextureID);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);// set texture wrapping to GL_REPEAT (default wrapping method)
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);// set texture wrapping to GL_REPEAT (default wrapping method)
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

	for (int i = 0; i < ARRAY_SIZE_IN_ELEMENTS(types); i++) {
		skyboxTexture = Texture(fileNames[i]);
		FIBITMAP *bitmap = skyboxTexture.loadImage(true);
		unsigned char *data = skyboxTexture.convertToData(bitmap, imageWidth,
			imageHeight);
		if (data) {
			glTexImage2D(types[i], 0, GL_RGBA, imageWidth, imageHeight, 0,
				GL_BGRA, GL_UNSIGNED_BYTE, data);
		}
		else
			std::cout << "Failed to load texture" << std::endl;
		skyboxTexture.freeImage(bitmap);
	}




	// Definiendo la textura a utilizar
	Texture textureTerrainBackground("../Textures/Tres.png");
	// Carga el mapa de bits (FIBITMAP es el tipo de dato de la libreria)
	bitmap = textureTerrainBackground.loadImage();
	// Convertimos el mapa de bits en un arreglo unidimensional de tipo unsigned char
	data = textureTerrainBackground.convertToData(bitmap, imageWidth,
		imageHeight);
	// Creando la textura con id 1
	glGenTextures(1, &textureTerrainBackgroundID);
	// Enlazar esa textura a una tipo de textura de 2D.
	glBindTexture(GL_TEXTURE_2D, textureTerrainBackgroundID);
	// set the texture wrapping parameters
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT); // set texture wrapping to GL_REPEAT (default wrapping method)
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	// set texture filtering parameters
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	// Verifica si se pudo abrir la textura
	if (data) {
		// Transferis los datos de la imagen a memoria
		// Tipo de textura, Mipmaps, Formato interno de openGL, ancho, alto, Mipmaps,
		// Formato interno de la libreria de la imagen, el tipo de dato y al apuntador
		// a los datos
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, imageWidth, imageHeight, 0,
			GL_BGRA, GL_UNSIGNED_BYTE, data);
		// Generan los niveles del mipmap (OpenGL es el ecargado de realizarlos)
		glGenerateMipmap(GL_TEXTURE_2D);
	}
	else
		std::cout << "Failed to load texture" << std::endl;
	// Libera la memoria de la textura
	textureTerrainBackground.freeImage(bitmap);

	// Definiendo la textura a utilizar
	Texture textureTerrainR("../Textures/Cuatro.png");
	// Carga el mapa de bits (FIBITMAP es el tipo de dato de la libreria)
	bitmap = textureTerrainR.loadImage();
	// Convertimos el mapa de bits en un arreglo unidimensional de tipo unsigned char
	data = textureTerrainR.convertToData(bitmap, imageWidth,
		imageHeight);
	// Creando la textura con id 1
	glGenTextures(1, &textureTerrainRID);
	// Enlazar esa textura a una tipo de textura de 2D.
	glBindTexture(GL_TEXTURE_2D, textureTerrainRID);
	// set the texture wrapping parameters
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT); // set texture wrapping to GL_REPEAT (default wrapping method)
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	// set texture filtering parameters
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	// Verifica si se pudo abrir la textura
	if (data) {
		// Transferis los datos de la imagen a memoria
		// Tipo de textura, Mipmaps, Formato interno de openGL, ancho, alto, Mipmaps,
		// Formato interno de la libreria de la imagen, el tipo de dato y al apuntador
		// a los datos
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, imageWidth, imageHeight, 0,
			GL_BGRA, GL_UNSIGNED_BYTE, data);
		// Generan los niveles del mipmap (OpenGL es el ecargado de realizarlos)
		glGenerateMipmap(GL_TEXTURE_2D);
	}
	else
		std::cout << "Failed to load texture" << std::endl;
	// Libera la memoria de la textura
	textureTerrainR.freeImage(bitmap);

	// Definiendo la textura a utilizar
	Texture textureTerrainG("../Textures/Cinco.png");
	// Carga el mapa de bits (FIBITMAP es el tipo de dato de la libreria)
	bitmap = textureTerrainG.loadImage();
	// Convertimos el mapa de bits en un arreglo unidimensional de tipo unsigned char
	data = textureTerrainG.convertToData(bitmap, imageWidth,
		imageHeight);
	// Creando la textura con id 1
	glGenTextures(1, &textureTerrainGID);
	// Enlazar esa textura a una tipo de textura de 2D.
	glBindTexture(GL_TEXTURE_2D, textureTerrainGID);
	// set the texture wrapping parameters
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT); // set texture wrapping to GL_REPEAT (default wrapping method)
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	// set texture filtering parameters
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	// Verifica si se pudo abrir la textura
	if (data) {
		// Transferis los datos de la imagen a memoria
		// Tipo de textura, Mipmaps, Formato interno de openGL, ancho, alto, Mipmaps,
		// Formato interno de la libreria de la imagen, el tipo de dato y al apuntador
		// a los datos
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, imageWidth, imageHeight, 0,
			GL_BGRA, GL_UNSIGNED_BYTE, data);
		// Generan los niveles del mipmap (OpenGL es el ecargado de realizarlos)
		glGenerateMipmap(GL_TEXTURE_2D);
	}
	else
		std::cout << "Failed to load texture" << std::endl;
	// Libera la memoria de la textura
	textureTerrainG.freeImage(bitmap);

	// Definiendo la textura a utilizar
	Texture textureTerrainB("../Textures/Dos.png");
	// Carga el mapa de bits (FIBITMAP es el tipo de dato de la libreria)
	bitmap = textureTerrainB.loadImage();
	// Convertimos el mapa de bits en un arreglo unidimensional de tipo unsigned char
	data = textureTerrainB.convertToData(bitmap, imageWidth,
		imageHeight);
	// Creando la textura con id 1
	glGenTextures(1, &textureTerrainBID);
	// Enlazar esa textura a una tipo de textura de 2D.
	glBindTexture(GL_TEXTURE_2D, textureTerrainBID);
	// set the texture wrapping parameters
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT); // set texture wrapping to GL_REPEAT (default wrapping method)
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	// set texture filtering parameters
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	// Verifica si se pudo abrir la textura
	if (data) {
		// Transferis los datos de la imagen a memoria
		// Tipo de textura, Mipmaps, Formato interno de openGL, ancho, alto, Mipmaps,
		// Formato interno de la libreria de la imagen, el tipo de dato y al apuntador
		// a los datos
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, imageWidth, imageHeight, 0,
			GL_BGRA, GL_UNSIGNED_BYTE, data);
		// Generan los niveles del mipmap (OpenGL es el ecargado de realizarlos)
		glGenerateMipmap(GL_TEXTURE_2D);
	}
	else
		std::cout << "Failed to load texture" << std::endl;
	// Libera la memoria de la textura
	textureTerrainB.freeImage(bitmap);

	// Definiendo la textura a utilizar
	Texture textureTerrainBlendMap("../Textures/blendMap2.png");
	// Carga el mapa de bits (FIBITMAP es el tipo de dato de la libreria)
	bitmap = textureTerrainBlendMap.loadImage(true);
	// Convertimos el mapa de bits en un arreglo unidimensional de tipo unsigned char
	data = textureTerrainBlendMap.convertToData(bitmap, imageWidth,
		imageHeight);
	// Creando la textura con id 1
	glGenTextures(1, &textureTerrainBlendMapID);
	// Enlazar esa textura a una tipo de textura de 2D.
	glBindTexture(GL_TEXTURE_2D, textureTerrainBlendMapID);
	// set the texture wrapping parameters
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT); // set texture wrapping to GL_REPEAT (default wrapping method)
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	// set texture filtering parameters
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	// Verifica si se pudo abrir la textura
	if (data) {
		// Transferis los datos de la imagen a memoria
		// Tipo de textura, Mipmaps, Formato interno de openGL, ancho, alto, Mipmaps,
		// Formato interno de la libreria de la imagen, el tipo de dato y al apuntador
		// a los datos
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, imageWidth, imageHeight, 0,
			GL_BGRA, GL_UNSIGNED_BYTE, data);
		// Generan los niveles del mipmap (OpenGL es el ecargado de realizarlos)
		glGenerateMipmap(GL_TEXTURE_2D);
	}
	else
		std::cout << "Failed to load texture" << std::endl;
	// Libera la memoria de la textura
	textureTerrainBlendMap.freeImage(bitmap);

	Texture textureTerrainBlendMap1("../Textures/blend2.png");
	// Carga el mapa de bits (FIBITMAP es el tipo de dato de la libreria)
	bitmap = textureTerrainBlendMap1.loadImage(true);
	// Convertimos el mapa de bits en un arreglo unidimensional de tipo unsigned char
	data = textureTerrainBlendMap1.convertToData(bitmap, imageWidth,
		imageHeight);
	// Creando la textura con id 1
	glGenTextures(1, &textureTerrainBlendMapID2);
	// Enlazar esa textura a una tipo de textura de 2D.
	glBindTexture(GL_TEXTURE_2D, textureTerrainBlendMapID2);
	// set the texture wrapping parameters
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT); // set texture wrapping to GL_REPEAT (default wrapping method)
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	// set texture filtering parameters
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	// Verifica si se pudo abrir la textura
	if (data) {
		// Transferis los datos de la imagen a memoria
		// Tipo de textura, Mipmaps, Formato interno de openGL, ancho, alto, Mipmaps,
		// Formato interno de la libreria de la imagen, el tipo de dato y al apuntador
		// a los datos
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, imageWidth, imageHeight, 0,
			GL_BGRA, GL_UNSIGNED_BYTE, data);
		// Generan los niveles del mipmap (OpenGL es el ecargado de realizarlos)
		glGenerateMipmap(GL_TEXTURE_2D);
	}
	else
		std::cout << "Failed to load texture" << std::endl;
	// Libera la memoria de la textura
	textureTerrainBlendMap1.freeImage(bitmap);


	Texture textureParticleFire("../Textures/fire.png");
	bitmap = textureParticleFire.loadImage();
	data = textureParticleFire.convertToData(bitmap, imageWidth, imageHeight);
	glGenTextures(1, &textureParticleFireID);
	glBindTexture(GL_TEXTURE_2D, textureParticleFireID);
	// set the texture wrapping parameters
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);// set texture wrapping to GL_REPEAT (default wrapping method)
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	// set texture filtering parameters
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	if (data) {
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, imageWidth, imageHeight, 0,
			GL_BGRA, GL_UNSIGNED_BYTE, data);
		glGenerateMipmap(GL_TEXTURE_2D);
	}
	else
		std::cout << "Failed to load texture" << std::endl;
	textureParticleFire.freeImage(bitmap);

	std::uniform_real_distribution<float> distr01 = std::uniform_real_distribution<float>(0.0f, 1.0f);
	std::mt19937 generator;
	std::random_device rd;
	generator.seed(rd());
	int size = nParticlesFire * 2;
	std::vector<GLfloat> randData(size);
	for (int i = 0; i < randData.size(); i++) {
		randData[i] = distr01(generator);
	}

	glGenTextures(1, &texId);
	glBindTexture(GL_TEXTURE_1D, texId);
	glTexStorage1D(GL_TEXTURE_1D, 1, GL_R32F, size);
	glTexSubImage1D(GL_TEXTURE_1D, 0, 0, size, GL_RED, GL_FLOAT, randData.data());
	glTexParameteri(GL_TEXTURE_1D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_1D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);

	shaderParticlesFire.setInt("Pass", 1);
	shaderParticlesFire.setInt("ParticleTex", 0);
	shaderParticlesFire.setInt("RandomTex", 1);
	shaderParticlesFire.setFloat("ParticleLifetime", particleLifetime);
	shaderParticlesFire.setFloat("ParticleSize", particleSize);
	shaderParticlesFire.setVectorFloat3("Accel", glm::value_ptr(glm::vec3(0.0f, 0.1f, 0.0f)));
	shaderParticlesFire.setVectorFloat3("Emitter", glm::value_ptr(glm::vec3(0.0f)));

	glm::mat3 basis;
	glm::vec3 u, v, n;
	v = glm::vec3(0, 1, 0);
	n = glm::cross(glm::vec3(1, 0, 0), v);
	if (glm::length(n) < 0.00001f) {
		n = glm::cross(glm::vec3(0, 1, 0), v);
	}
	u = glm::cross(v, n);
	basis[0] = glm::normalize(u);
	basis[1] = glm::normalize(v);
	basis[2] = glm::normalize(n);
	shaderParticlesFire.setMatrix3("EmitterBasis", 1, false, glm::value_ptr(basis));

	/*******************************************
	 * Inicializacion de los buffers de la fuente
	 *******************************************/
	initParticleBuffers();

	/*******************************************
	 * Inicializacion de los buffers del fuego
	 *******************************************/
	initParticleBuffersFire();

	/*******************************************
	 * Inicializacion del framebuffer para
	 * almacenar el buffer de profunidadad
	 *******************************************/
	glGenFramebuffers(1, &depthMapFBO);
	glGenTextures(1, &depthMap);
	glBindTexture(GL_TEXTURE_2D, depthMap);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_DEPTH_COMPONENT,
		SHADOW_WIDTH, SHADOW_HEIGHT, 0, GL_DEPTH_COMPONENT, GL_FLOAT, NULL);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	/*glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);*/
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_BORDER);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_BORDER);
	float borderColor[] = { 1.0, 1.0, 1.0, 1.0 };
	glTexParameterfv(GL_TEXTURE_2D, GL_TEXTURE_BORDER_COLOR, borderColor);
	glBindFramebuffer(GL_FRAMEBUFFER, depthMapFBO);
	glFramebufferTexture2D(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_TEXTURE_2D, depthMap, 0);
	glDrawBuffer(GL_NONE);
	glReadBuffer(GL_NONE);
	glBindFramebuffer(GL_FRAMEBUFFER, 0);

	/*******************************************
	 * OpenAL init
	 *******************************************/
	alutInit(0, nullptr);
	alListenerfv(AL_POSITION, listenerPos);
	alListenerfv(AL_VELOCITY, listenerVel);
	alListenerfv(AL_ORIENTATION, listenerOri);
	alGetError(); // clear any error messages
	if (alGetError() != AL_NO_ERROR) {
		printf("- Error creating buffers !!\n");
		exit(1);
	}
	else {
		printf("init() - No errors yet.");
	}
	// Config source 0
	// Generate buffers, or else no sound will happen!
	alGenBuffers(NUM_BUFFERS, buffer);
	buffer[0] = alutCreateBufferFromFile("../sounds/Space.wav");
	buffer[1] = alutCreateBufferFromFile("../sounds/fornais.wav");
	buffer[2] = alutCreateBufferFromFile("../sounds/fornais.wav");
	buffer[3] = alutCreateBufferFromFile("../sounds/fornais.wav");
	buffer[4] = alutCreateBufferFromFile("../sounds/fornais.wav");
	alGenBuffers(NUM_BUFFERS, buffer1);
	buffer1[0] = alutCreateBufferFromFile("../sounds/Space.wav");
	buffer1[1] = alutCreateBufferFromFile("../sounds/nave.wav");
	buffer1[2] = alutCreateBufferFromFile("../sounds/nave.wav");
	int errorAlut = alutGetError();
	if (errorAlut != ALUT_ERROR_NO_ERROR) {
		printf("- Error open files with alut %d !!\n", errorAlut);
		exit(2);
	}


	alGetError(); /* clear error */
	alGenSources(NUM_SOURCES, source);
	alGenSources(NUM_SOURCES1, source1);

	if (alGetError() != AL_NO_ERROR) {
		printf("- Error creating sources !!\n");
		exit(2);
	}
	else {
		printf("init - no errors after alGenSources\n");
	}
	alSourcef(source[0], AL_PITCH, 1.0f);
	alSourcef(source[0], AL_GAIN, 0.1f);
	alSourcefv(source[0], AL_POSITION, source0Pos);
	alSourcefv(source[0], AL_VELOCITY, source0Vel);
	alSourcei(source[0], AL_BUFFER, buffer[0]);
	alSourcei(source[0], AL_LOOPING, AL_TRUE);
	alSourcef(source[0], AL_MAX_DISTANCE, 2000);

	alSourcef(source[1], AL_PITCH, 1.0f);
	alSourcef(source[1], AL_GAIN, 0.5f);
	alSourcefv(source[1], AL_POSITION, source1Pos);
	alSourcefv(source[1], AL_VELOCITY, source1Vel);
	alSourcei(source[1], AL_BUFFER, buffer[1]);
	alSourcei(source[1], AL_LOOPING, AL_TRUE);
	alSourcef(source[1], AL_MAX_DISTANCE, 200000);

	alSourcef(source[2], AL_PITCH, 1.0f);
	alSourcef(source[2], AL_GAIN, 0.5f);
	alSourcefv(source[2], AL_POSITION, source2Pos);
	alSourcefv(source[2], AL_VELOCITY, source2Vel);
	alSourcei(source[2], AL_BUFFER, buffer[2]);
	alSourcei(source[2], AL_LOOPING, AL_TRUE);
	alSourcef(source[2], AL_MAX_DISTANCE, 200000);


	alSourcef(source[3], AL_PITCH, 1.0f);
	alSourcef(source[3], AL_GAIN, 0.5f);
	alSourcefv(source[3], AL_POSITION, source3Pos);
	alSourcefv(source[3], AL_VELOCITY, source3Vel);
	alSourcei(source[3], AL_BUFFER, buffer[3]);
	alSourcei(source[3], AL_LOOPING, AL_TRUE);
	alSourcef(source[3], AL_MAX_DISTANCE, 200000);


	alSourcef(source[4], AL_PITCH, 1.0f);
	alSourcef(source[4], AL_GAIN, 0.5f);
	alSourcefv(source[4], AL_POSITION, source4Pos);
	alSourcefv(source[4], AL_VELOCITY, source4Vel);
	alSourcei(source[4], AL_BUFFER, buffer[4]);
	alSourcei(source[4], AL_LOOPING, AL_TRUE);
	alSourcef(source[4], AL_MAX_DISTANCE, 200000);


	alSourcef(source1[0], AL_PITCH, 1.0f);
	alSourcef(source1[0], AL_GAIN, 0.1f);
	alSourcefv(source1[0], AL_POSITION, source0Pos);
	alSourcefv(source1[0], AL_VELOCITY, source0Vel);
	alSourcei(source1[0], AL_BUFFER, buffer1[0]);
	alSourcei(source1[0], AL_LOOPING, AL_TRUE);
	alSourcef(source1[0], AL_MAX_DISTANCE, 2000);

	alSourcef(source1[1], AL_PITCH, 1.0f);
	alSourcef(source1[1], AL_GAIN, 0.1f);
	alSourcefv(source1[1], AL_POSITION, source10Pos);
	alSourcefv(source1[1], AL_VELOCITY, source10Vel);
	alSourcei(source1[1], AL_BUFFER, buffer1[1]);
	alSourcei(source1[1], AL_LOOPING, AL_TRUE);
	alSourcef(source1[1], AL_MAX_DISTANCE, 200000);

	alSourcef(source1[2], AL_PITCH, 1.0f);
	alSourcef(source1[2], AL_GAIN, 0.1f);
	alSourcefv(source1[2], AL_POSITION, source2Pos);
	alSourcefv(source1[2], AL_VELOCITY, source2Vel);
	alSourcei(source1[2], AL_BUFFER, buffer1[2]);
	alSourcei(source1[2], AL_LOOPING, AL_TRUE);
	alSourcef(source1[2], AL_MAX_DISTANCE, 200000);

	// Se inicializa el modelo de texeles.
	modelText = new FontTypeRendering::FontTypeRendering(screenWidth,
		screenHeight);
	modelText->Initialize();
	modelText1 = new FontTypeRendering::FontTypeRendering(screenWidth,
		screenHeight);
	modelText1->Initialize();
}

void destroy() {
	glfwDestroyWindow(window);
	glfwTerminate();
	// --------- IMPORTANTE ----------
	// Eliminar los shader y buffers creados.

	// Shaders Delete
	shader.destroy();
	shaderMulLighting.destroy();
	shaderMulLighting2.destroy();
	shaderSkybox.destroy();
	shaderTerrain.destroy();
	shaderParticlesFountain.destroy();
	shaderParticlesFire.destroy();

	// Basic objects Delete
	skyboxSphere.destroy();
	boxCollider.destroy();
	sphereCollider.destroy();
	boxViewDepth.destroy();
	boxLightViewBox.destroy();

	// Terrains objects Delete
	terrain.destroy();

	// Custom objects Delete

	modelLamp1.destroy();
	modelLamp2.destroy();
	modelLamp3.destroy();
	Piedras1.destroy();
	modelGrass.destroy();
	pantalla1.destroy();
	pantalla2.destroy();
	pantalla3.destroy();
	pantalla4.destroy();
	pantalla5.destroy();
	pantalla6.destroy();
	// Custom objects animate
	protaFI.destroy();

	// Textures Delete
	glBindTexture(GL_TEXTURE_2D, 0);
	glDeleteTextures(1, &textureTerrainBackgroundID);
	glDeleteTextures(1, &textureTerrainRID);
	glDeleteTextures(1, &textureTerrainGID);
	glDeleteTextures(1, &textureTerrainBID);
	glDeleteTextures(1, &textureTerrainBlendMapID);
	glDeleteTextures(1, &textureParticleFountainID);
	glDeleteTextures(1, &textureParticleFireID);

	// Cube Maps Delete
	glBindTexture(GL_TEXTURE_CUBE_MAP, 0);
	glDeleteTextures(1, &skyboxTextureID);

	// Remove the buffer of the fountain particles
	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glDeleteBuffers(1, &initVel);
	glDeleteBuffers(1, &startTime);
	glBindVertexArray(0);
	glDeleteVertexArrays(1, &VAOParticles);

	// Remove the buffer of the fire particles
	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glDeleteBuffers(2, posBuf);
	glDeleteBuffers(2, velBuf);
	glDeleteBuffers(2, age);
	glBindTransformFeedback(GL_TRANSFORM_FEEDBACK, 0);
	glDeleteTransformFeedbacks(2, feedback);
	glBindVertexArray(0);
	glDeleteVertexArrays(1, &VAOParticlesFire);
}

void reshapeCallback(GLFWwindow *Window, int widthRes, int heightRes) {
	screenWidth = widthRes;
	screenHeight = heightRes;
	glViewport(0, 0, widthRes, heightRes);
}

void keyCallback(GLFWwindow *window, int key, int scancode, int action,
	int mode) {
	if (action == GLFW_PRESS) {
		switch (key) {
		case GLFW_KEY_ESCAPE:
			exitApp = true;
			break;
		}
	}
}

void mouseCallback(GLFWwindow *window, double xpos, double ypos) {
	offsetX = xpos - lastMousePosX;
	offsetY = ypos - lastMousePosY;
	lastMousePosX = xpos;
	lastMousePosY = ypos;
}

void scrollCallback(GLFWwindow* window, double xoffset, double yoffset) {
	distanceFromTarget -= yoffset;
	camera->setDistanceFromTarget(distanceFromTarget);
}

void mouseButtonCallback(GLFWwindow *window, int button, int state, int mod) {
	if (state == GLFW_PRESS) {
		switch (button) {
		case GLFW_MOUSE_BUTTON_RIGHT:
			std::cout << "lastMousePos.y:" << lastMousePosY << std::endl;
			break;
		case GLFW_MOUSE_BUTTON_LEFT:
			std::cout << "lastMousePos.x:" << lastMousePosX << std::endl;
			break;
		case GLFW_MOUSE_BUTTON_MIDDLE:
			std::cout << "lastMousePos.x:" << lastMousePosX << std::endl;
			std::cout << "lastMousePos.y:" << lastMousePosY << std::endl;
			break;
		}
	}
}
//aquio empieza lo reciio

bool processInput(bool continueApplication) {
	if (exitApp || glfwWindowShouldClose(window) != 0) {
		return false;
	}
	//no importa nivel debe funcionar
	//joystick es el 1 
	int control = glfwJoystickPresent(GLFW_JOYSTICK_1);
	//std::cout << "Joystick status :" << control << std::endl;
	int axesCount, buttonscount;
	const float *axes = glfwGetJoystickAxes(GLFW_JOYSTICK_1, &axesCount);
	const unsigned char  *buttons = glfwGetJoystickButtons(GLFW_JOYSTICK_1, &buttonscount);
	if (glfwGetMouseButton(window, GLFW_MOUSE_BUTTON_LEFT) == GLFW_PRESS)
		camera->mouseMoveCamera(offsetX, 0.0, deltaTime);
	if (glfwGetMouseButton(window, GLFW_MOUSE_BUTTON_RIGHT) == GLFW_PRESS)
		camera->mouseMoveCamera(0.0, offsetY, deltaTime);
	if (GLFW_PRESS == buttons[10]) {
		distanceFromTarget -= deltaTime * 3;
		camera->setDistanceFromTarget(distanceFromTarget);
	}
	else if (GLFW_PRESS == buttons[12]) {
		distanceFromTarget += deltaTime * 3;
		camera->setDistanceFromTarget(distanceFromTarget);
	}
	if (GLFW_PRESS == buttons[7] || glfwGetKey(window, GLFW_KEY_ENTER) == GLFW_PRESS)
	{
		nostart = 1;
		std::cout << "Lee el control" << std::endl;
	}
	//todo lo de nivel 1
	if (nivel == 1)
	{
		if (GLFW_PRESS == buttons[1]) {
			controles = 1;
		}
		else {
			controles = 0;
		}

		if (modelSelected == 2 && glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS) {
			modelMatrixProta = glm::rotate(modelMatrixProta, glm::radians(2.0f), glm::vec3(0, 1, 0));
			animationIndex = 1;
		}
		else if (modelSelected == 2 && glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS) {
			modelMatrixProta = glm::rotate(modelMatrixProta, glm::radians(-2.0f), glm::vec3(0, 1, 0));
			animationIndex = 1;
		}if (modelSelected == 2 && glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS) {
			modelMatrixProta = glm::translate(modelMatrixProta, glm::vec3(0, 0, 0.4));
			animationIndex = 1;
		}
		else if (modelSelected == 2 && glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS) {
			modelMatrixProta = glm::translate(modelMatrixProta, glm::vec3(0, 0, -0.4));
			animationIndex = 1;
		}
		if (!isJump && GLFW_PRESS == buttons[0] || !isJump && glfwGetKey(window, GLFW_KEY_SPACE) == GLFW_PRESS) {
			isJump = true;
			tmv = 0;
			startTimeJump = currTime;
		}

		if (GLFW_PRESS == buttons[2]) {
			animationIndex = 0;
		}

		if ((modelSelected == 2 && axes[3] > 0 || modelSelected == 2 && axes[3] < 0) && (modelSelected == 2 && axes[2] > 0 || modelSelected == 2 && axes[2] < 0)) {
			camera->mouseMoveCamera(0.0, axes[3] * -2, deltaTime);
			camera->mouseMoveCamera(axes[2] * 2, 0.0, deltaTime);
		}
		if (modelSelected == 2 && axes[0] < -0.4)
		{

			animationIndex = 1;
			modelMatrixProta = glm::rotate(modelMatrixProta, glm::radians(2.0f), glm::vec3(0, 1, 0));
		}
		else if (modelSelected == 2 && axes[0] > 0.4)
		{
			animationIndex = 1;
			modelMatrixProta = glm::rotate(modelMatrixProta, glm::radians(-2.0f), glm::vec3(0, 1, 0));
		}

		if (modelSelected == 2 && axes[1] > 0.4)
		{
			animationIndex = 1;

			modelMatrixProta = glm::translate(modelMatrixProta, glm::vec3(0.00, 0.00, 0.4));
		}
		else if (modelSelected == 2 && axes[1] < -0.4)
		{
			animationIndex = 1;

			modelMatrixProta = glm::translate(modelMatrixProta, glm::vec3(0.00, 0.00, -0.4));
		}
	}
	/*if (glfwGetMouseButton(window, GLFW_MOUSE_BUTTON_LEFT) == GLFW_PRESS)
		camera->mouseMoveCamera(offsetX, 0.0, deltaTime);
	if (glfwGetMouseButton(window, GLFW_MOUSE_BUTTON_RIGHT) == GLFW_PRESS)
		camera->mouseMoveCamera(0.0, offsetY, deltaTime);*/

		/*if (modelSelected == 2 && glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS) {
			modelMatrixProta = glm::rotate(modelMatrixProta, glm::radians(2.0f), glm::vec3(0, 1, 0));
			animationIndex = 1;
		}
		else if (modelSelected == 2 && glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS) {
			modelMatrixProta = glm::rotate(modelMatrixProta, glm::radians(-2.0f), glm::vec3(0, 1, 0));
			animationIndex = 1;
		}if (modelSelected == 2 && glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS) {
			modelMatrixProta = glm::translate(modelMatrixProta, glm::vec3(0, 0, 0.4));
			animationIndex = 1;
		}
		else if (modelSelected == 2 && glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS) {
			modelMatrixProta = glm::translate(modelMatrixProta, glm::vec3(0, 0, -0.4));
			animationIndex = 1;
		}*/




		//todo lo de nivel 4
	else if (nivel == 4)
	{
		if (GLFW_PRESS == buttons[1]) {
			controles = 1;
		}
		else {
			controles = 0;
		}
		//std::cout << "rot y:" << modelMatrixNaveCompleta[2][2]  << std::endl;
		if (axes[5] > -1) {
			float vel = axes[5] + 1;
			if (vel > 1 && modelMatrixNaveCompleta[3][1] < 2.5)
			{
				vel = 1;
				contadorTiempo2++;
				arreciado = -6.0f;
				//contadorTiempo2 = contadorTiempo2 / 59;
			}
			if (modelMatrixNaveCompleta[3][1] > 2.5)
			{
				vel = 0.04;
			}
			modelMatrixNaveCompleta = glm::translate(modelMatrixNaveCompleta, glm::vec3(0.00, 0.00, vel));
			if (axes[0] < -0.4)
			{

				modelMatrixNaveCompleta = glm::rotate(modelMatrixNaveCompleta, glm::radians(2.0f), glm::vec3(0, 1, 0));
			}
			else if (axes[0] > 0.4)
			{
				modelMatrixNaveCompleta = glm::rotate(modelMatrixNaveCompleta, glm::radians(-2.0f), glm::vec3(0, 1, 0));
			}
		}
		else if (axes[4] > -1) {
			modelMatrixNaveCompleta = glm::translate(modelMatrixNaveCompleta, glm::vec3(0.00, 0.00, -0.4));
			if (axes[0] < -0.4)
			{
				modelMatrixNaveCompleta = glm::rotate(modelMatrixNaveCompleta, glm::radians(2.0f), glm::vec3(0, 1, 0));
			}
			else if (axes[0] > 0.4)
			{
				modelMatrixNaveCompleta = glm::rotate(modelMatrixNaveCompleta, glm::radians(-2.0f), glm::vec3(0, 1, 0));
			}
		}
		else if (axes[0] < -0.4)
		{
			modelMatrixNaveCompleta = glm::rotate(modelMatrixNaveCompleta, glm::radians(2.0f), glm::vec3(0, 1, 0));
		}
		else if (axes[0] > 0.4)
		{
			modelMatrixNaveCompleta = glm::rotate(modelMatrixNaveCompleta, glm::radians(-2.0f), glm::vec3(0, 1, 0));
		}
		else {
			arreciado = -2.0f;
		}
		if ((modelSelected == 2 && axes[3] > 0 || modelSelected == 2 && axes[3] < 0) && (modelSelected == 2 && axes[2] > 0 || modelSelected == 2 && axes[2] < 0)) {
			camera->mouseMoveCamera(0.0, axes[3] * -2, deltaTime);
			camera->mouseMoveCamera(axes[2] * 2, 0.0, deltaTime);
		}

	}



	glfwPollEvents();
	return continueApplication;
}
//aqui va mas recio
void applicationLoop() {
	bool psi = true;

	glm::mat4 view;
	glm::vec3 axis;
	glm::vec3 target;
	float angleTarget;

	lastTime = TimeManager::Instance().GetTime();

	// Time for the particles animation
	currTimeParticlesAnimation = lastTime;
	lastTimeParticlesAnimation = lastTime;

	currTimeParticlesAnimationFire = lastTime;
	lastTimeParticlesAnimationFire = lastTime;

	glm::vec3 lightPos = glm::vec3(10.0, 10.0, 0.0);

	while (psi) {
		currTime = TimeManager::Instance().GetTime();
		if (currTime - lastTime < 0.016666667) {
			glfwPollEvents();
			continue;
		}
		lastTime = currTime;
		TimeManager::Instance().CalculateFrameRate(true);
		deltaTime = TimeManager::Instance().DeltaTime;
		psi = processInput(true);

		std::map<std::string, bool> collisionDetection;

		// Variables donde se guardan las matrices de cada articulacion por 1 frame
		std::vector<float> matrixDartJoints;
		std::vector<glm::mat4> matrixDart;

		glm::mat4 projection = glm::perspective(glm::radians(45.0f),
			(float)screenWidth / (float)screenHeight, 0.1f, 100.0f);


		if (nivel == 0 || nivel == 5 || nivel == 6) {
			axis = glm::axis(glm::quat_cast(modelMatrixPantalla1));
			angleTarget = 92.7;
			target = glm::vec3(0.0);
			camerafij->setDistanceFromTarget(15);
			camerafij->setCameraTarget(target);
			camerafij->setAngleTarget(angleTarget);
			camerafij->updateCamera();
			view = camerafij->getViewMatrix();
		}
		if (nivel == 2 || nivel == 3) {
			axis = glm::axis(glm::quat_cast(modelMatrixPantalla1));
			angleTarget = 92.7;
			target = glm::vec3(0.0);
			camerafij->setDistanceFromTarget(15);
			camerafij->setCameraTarget(target);
			camerafij->setAngleTarget(angleTarget);
			camerafij->updateCamera();
			view = camerafij->getViewMatrix();
		}
		if (nivel == 1) {
			axis = glm::axis(glm::quat_cast(modelMatrixProta));
			angleTarget = glm::angle(glm::quat_cast(modelMatrixProta));
			target = glm::vec3(modelMatrixProta[3][0], modelMatrixProta[3][1] + 3.5, modelMatrixProta[3][2]);
			if (std::isnan(angleTarget))
				angleTarget = 0.0;
			if (axis.y < 0)
				angleTarget = -angleTarget;
			if (modelSelected == 1)
				angleTarget -= glm::radians(90.0f);
			camera->setCameraTarget(target);
			camera->setAngleTarget(angleTarget);
			camera->updateCamera();
			view = camera->getViewMatrix();
		}
		else if (nivel == 4) {
			axis = glm::axis(glm::quat_cast(modelMatrixNaveCompleta));
			angleTarget = glm::angle(glm::quat_cast(modelMatrixNaveCompleta));
			target = glm::vec3(modelMatrixNaveCompleta[3][0], modelMatrixNaveCompleta[3][1] + 3.5, modelMatrixNaveCompleta[3][2]);
			if (std::isnan(angleTarget))
				angleTarget = 0.0;
			if (axis.y < 0)
				angleTarget = -angleTarget;
			if (modelSelected == 2)
				angleTarget -= glm::radians(90.0f);
			camera->setCameraTarget(target);
			camera->setAngleTarget(angleTarget);
			camera->updateCamera();
			view = camera->getViewMatrix();
		}
		// Matriz de proyección del shadow mapping
		glm::mat4 lightProjection, lightView;
		glm::mat4 lightSpaceMatrix;
		float near_plane = 0.1f, far_plane = 20.0f;
		lightProjection = glm::ortho(-25.0f, 25.0f, -25.0f, 25.0f, near_plane, far_plane);
		lightView = glm::lookAt(lightPos, glm::vec3(0.0), glm::vec3(0.0, 1.0, 0.0));
		lightSpaceMatrix = lightProjection * lightView;
		shaderDepth.setMatrix4("lightSpaceMatrix", 1, false, glm::value_ptr(lightSpaceMatrix));

		// Settea la matriz de vista y projection al shader con solo color
		shader.setMatrix4("projection", 1, false, glm::value_ptr(projection));
		shader.setMatrix4("view", 1, false, glm::value_ptr(view));

		// Settea la matriz de vista y projection al shader con skybox
		shaderSkybox.setMatrix4("projection", 1, false,
			glm::value_ptr(projection));
		shaderSkybox.setMatrix4("view", 1, false,
			glm::value_ptr(glm::mat4(glm::mat3(view))));
		// Settea la matriz de vista y projection al shader con multiples luces
		shaderMulLighting.setMatrix4("projection", 1, false,
			glm::value_ptr(projection));
		shaderMulLighting.setMatrix4("view", 1, false,
			glm::value_ptr(view));
		shaderMulLighting.setMatrix4("lightSpaceMatrix", 1, false,
			glm::value_ptr(lightSpaceMatrix));
		//Para las pantallas
		shaderMulLighting2.setMatrix4("projection", 1, false,
			glm::value_ptr(projection));
		shaderMulLighting2.setMatrix4("view", 1, false,
			glm::value_ptr(view));
		shaderMulLighting2.setMatrix4("lightSpaceMatrix", 1, false,
			glm::value_ptr(lightSpaceMatrix));
		// Settea la matriz de vista y projection al shader con multiples luces
		shaderTerrain.setMatrix4("projection", 1, false,
			glm::value_ptr(projection));
		shaderTerrain.setMatrix4("view", 1, false,
			glm::value_ptr(view));
		shaderTerrain.setMatrix4("lightSpaceMatrix", 1, false,
			glm::value_ptr(lightSpaceMatrix));
		// Settea la matriz de vista y projection al shader para el fuego
		shaderParticlesFire.setInt("Pass", 2);
		shaderParticlesFire.setMatrix4("projection", 1, false, glm::value_ptr(projection));
		shaderParticlesFire.setMatrix4("view", 1, false, glm::value_ptr(view));

		/*******************************************
		 * Propiedades de neblina
		 *******************************************/
		 //shaderMulLighting.setVectorFloat3("fogColor", glm::value_ptr(glm::vec3(0.2, 0.0, 0.3)));
		shaderMulLighting.setVectorFloat3("fogColor", glm::value_ptr(glm::vec3(0.2, 0.0, 0.3)));
		shaderTerrain.setVectorFloat3("fogColor", glm::value_ptr(glm::vec3(0.2, 0.0, 0.3)));
		shaderSkybox.setVectorFloat3("fogColor", glm::value_ptr(glm::vec3(0.2, 0.0, 0.3)));

		/*******************************************
		 * Propiedades Luz direccional
		 *******************************************/
		shaderMulLighting.setVectorFloat3("viewPos", glm::value_ptr(camera->getPosition()));
		shaderMulLighting.setVectorFloat3("directionalLight.light.ambient", glm::value_ptr(glm::vec3(0.2, 0.2, 0.2)));
		shaderMulLighting.setVectorFloat3("directionalLight.light.diffuse", glm::value_ptr(glm::vec3(0.5, 0.5, 0.5)));
		shaderMulLighting.setVectorFloat3("directionalLight.light.specular", glm::value_ptr(glm::vec3(0.2, 0.2, 0.2)));
		shaderMulLighting.setVectorFloat3("directionalLight.direction", glm::value_ptr(glm::vec3(-0.707106781, -0.707106781, 0.0)));

		shaderMulLighting2.setVectorFloat3("viewPos", glm::value_ptr(camera->getPosition()));
		shaderMulLighting2.setVectorFloat3("directionalLight.light.ambient", glm::value_ptr(glm::vec3(0.2)));
		shaderMulLighting2.setVectorFloat3("directionalLight.light.diffuse", glm::value_ptr(glm::vec3(0.5)));
		shaderMulLighting2.setVectorFloat3("directionalLight.light.specular", glm::value_ptr(glm::vec3(0.2)));
		shaderMulLighting2.setVectorFloat3("directionalLight.direction", glm::value_ptr(glm::vec3(-0.707106781, -0.707106781, 0.0)));

		/*******************************************
		 * Propiedades Luz direccional Terrain
		 *******************************************/
		shaderTerrain.setVectorFloat3("viewPos", glm::value_ptr(camera->getPosition()));
		shaderTerrain.setVectorFloat3("directionalLight.light.ambient", glm::value_ptr(glm::vec3(0.2, 0.2, 0.2)));
		shaderTerrain.setVectorFloat3("directionalLight.light.diffuse", glm::value_ptr(glm::vec3(0.5, 0.5, 0.5)));
		shaderTerrain.setVectorFloat3("directionalLight.light.specular", glm::value_ptr(glm::vec3(0.2, 0.2, 0.2)));
		shaderTerrain.setVectorFloat3("directionalLight.direction", glm::value_ptr(glm::vec3(-0.707106781, -0.707106781, 0.0)));

		/*******************************************
		 * Propiedades SpotLights
		 *******************************************/
		 //glm::vec3 spotPosition = glm::vec3(modelMatrixHeli * glm::vec4(0.32437, 0.226053, 1.79149, 1.0));
		 //shaderMulLighting.setInt("spotLightCount", 1);
		 //shaderTerrain.setInt("spotLightCount", 1);
		 //shaderMulLighting.setVectorFloat3("spotLights[0].light.ambient", glm::value_ptr(glm::vec3(0.0, 0.0, 0.0)));
		 //shaderMulLighting.setVectorFloat3("spotLights[0].light.diffuse", glm::value_ptr(glm::vec3(0.2, 0.3, 0.2)));
		 //shaderMulLighting.setVectorFloat3("spotLights[0].light.specular", glm::value_ptr(glm::vec3(0.3, 0.3, 0.3)));
		 //shaderMulLighting.setVectorFloat3("spotLights[0].position", glm::value_ptr(spotPosition));
		 //shaderMulLighting.setVectorFloat3("spotLights[0].direction", glm::value_ptr(glm::vec3(0, -1, 0)));
		 //shaderMulLighting.setFloat("spotLights[0].constant", 1.0);
		 //shaderMulLighting.setFloat("spotLights[0].linear", 0.074);
		 //shaderMulLighting.setFloat("spotLights[0].quadratic", 0.03);
		 //shaderMulLighting.setFloat("spotLights[0].cutOff", cos(glm::radians(12.5f)));
		 //shaderMulLighting.setFloat("spotLights[0].outerCutOff", cos(glm::radians(15.0f)));
		 //shaderTerrain.setVectorFloat3("spotLights[0].light.ambient", glm::value_ptr(glm::vec3(0.0, 0.0, 0.0)));
		 //shaderTerrain.setVectorFloat3("spotLights[0].light.diffuse", glm::value_ptr(glm::vec3(0.2, 0.3, 0.2)));
		 //shaderTerrain.setVectorFloat3("spotLights[0].light.specular", glm::value_ptr(glm::vec3(0.3, 0.3, 0.3)));
		 //shaderTerrain.setVectorFloat3("spotLights[0].position", glm::value_ptr(spotPosition));
		 //shaderTerrain.setVectorFloat3("spotLights[0].direction", glm::value_ptr(glm::vec3(0, -1, 0)));
		 //shaderTerrain.setFloat("spotLights[0].constant", 1.0);
		 //shaderTerrain.setFloat("spotLights[0].linear", 0.074);
		 //shaderTerrain.setFloat("spotLights[0].quadratic", 0.03);
		 //shaderTerrain.setFloat("spotLights[0].cutOff", cos(glm::radians(12.5f)));
		 //shaderTerrain.setFloat("spotLights[0].outerCutOff", cos(glm::radians(15.0f)));

		 /*******************************************
		  * Propiedades PointLights
		  *******************************************/
		shaderMulLighting.setInt("pointLightCount", lamp3Position.size());

		shaderTerrain.setInt("pointLightCount", lamp3Position.size());
		for (int i = 0; i < lamp3Position.size(); i++) {
			glm::mat4 matrixAdjustLamp = glm::mat4(1.0f);
			matrixAdjustLamp = glm::translate(matrixAdjustLamp, lamp3Position[i]);
			matrixAdjustLamp = glm::rotate(matrixAdjustLamp, glm::radians(lamp3Orientation[i]), glm::vec3(0, 1, 0));
			matrixAdjustLamp = glm::scale(matrixAdjustLamp, glm::vec3(0.5, 0.5, 0.5));
			matrixAdjustLamp = glm::translate(matrixAdjustLamp, glm::vec3(0, 10.3585, 0));
			glm::vec3 lampPosition = glm::vec3(matrixAdjustLamp[3]);
			shaderMulLighting.setVectorFloat3("pointLights[" + std::to_string(i) + "].light.ambient", glm::value_ptr(glm::vec3(0.0, 0.5, 0.00)));
			shaderMulLighting.setVectorFloat3("pointLights[" + std::to_string(i) + "].light.diffuse", glm::value_ptr(glm::vec3(0.0, 0.5, 0.00)));
			shaderMulLighting.setVectorFloat3("pointLights[" + std::to_string(i) + "].light.specular", glm::value_ptr(glm::vec3(0.0, 0.5, 0.00)));
			shaderMulLighting.setVectorFloat3("pointLights[" + std::to_string(i) + "].position", glm::value_ptr(lampPosition));
			shaderMulLighting.setFloat("pointLights[" + std::to_string(i) + "].constant", 1.0);
			shaderMulLighting.setFloat("pointLights[" + std::to_string(i) + "].linear", 0.09);
			shaderMulLighting.setFloat("pointLights[" + std::to_string(i) + "].quadratic", 0.01);
			shaderTerrain.setVectorFloat3("pointLights[" + std::to_string(i) + "].light.ambient", glm::value_ptr(glm::vec3(0.0, 0.5, 0.0)));
			shaderTerrain.setVectorFloat3("pointLights[" + std::to_string(i) + "].light.diffuse", glm::value_ptr(glm::vec3(0.0, 0.5, 0.0)));
			shaderTerrain.setVectorFloat3("pointLights[" + std::to_string(i) + "].light.specular", glm::value_ptr(glm::vec3(0.0, 0.5, 0.0)));
			shaderTerrain.setVectorFloat3("pointLights[" + std::to_string(i) + "].position", glm::value_ptr(lampPosition));
			shaderTerrain.setFloat("pointLights[" + std::to_string(i) + "].constant", 1.0);
			shaderTerrain.setFloat("pointLights[" + std::to_string(i) + "].linear", 0.09);
			shaderTerrain.setFloat("pointLights[" + std::to_string(i) + "].quadratic", 0.02);
		}
		/*
		for (int i = 0; i < lamp2Position.size(); i++){
			glm::mat4 matrixAdjustLamp = glm::mat4(1.0f);
			matrixAdjustLamp = glm::translate(matrixAdjustLamp, lamp2Position[i]);
			matrixAdjustLamp = glm::rotate(matrixAdjustLamp, glm::radians(lamp2Orientation[i]), glm::vec3(0, 1, 0));
			matrixAdjustLamp = glm::scale(matrixAdjustLamp, glm::vec3(1.0, 1.0, 1.0));
			matrixAdjustLamp = glm::translate(matrixAdjustLamp, glm::vec3(0.759521, 5.00174, 0));
			glm::vec3 lampPosition = glm::vec3(matrixAdjustLamp[3]);
			shaderMulLighting.setVectorFloat3("pointLights[" + std::to_string(lamp1Position.size() + i) + "].light.ambient", glm::value_ptr(glm::vec3(0.2, 0.16, 0.01)));
			shaderMulLighting.setVectorFloat3("pointLights[" + std::to_string(lamp1Position.size() + i) + "].light.diffuse", glm::value_ptr(glm::vec3(0.4, 0.32, 0.02)));
			shaderMulLighting.setVectorFloat3("pointLights[" + std::to_string(lamp1Position.size() + i) + "].light.specular", glm::value_ptr(glm::vec3(0.6, 0.58, 0.03)));
			shaderMulLighting.setVectorFloat3("pointLights[" + std::to_string(lamp1Position.size() + i) + "].position", glm::value_ptr(lampPosition));
			shaderMulLighting.setFloat("pointLights[" + std::to_string(lamp1Position.size() + i) + "].constant", 1.0);
			shaderMulLighting.setFloat("pointLights[" + std::to_string(lamp1Position.size() + i) + "].linear", 0.09);
			shaderMulLighting.setFloat("pointLights[" + std::to_string(lamp1Position.size() + i) + "].quadratic", 0.01);
			shaderTerrain.setVectorFloat3("pointLights[" + std::to_string(lamp1Position.size() + i) + "].light.ambient", glm::value_ptr(glm::vec3(0.2, 0.16, 0.01)));
			shaderTerrain.setVectorFloat3("pointLights[" + std::to_string(lamp1Position.size() + i) + "].light.diffuse", glm::value_ptr(glm::vec3(0.4, 0.32, 0.02)));
			shaderTerrain.setVectorFloat3("pointLights[" + std::to_string(lamp1Position.size() + i) + "].light.specular", glm::value_ptr(glm::vec3(0.6, 0.58, 0.03)));
			shaderTerrain.setVectorFloat3("pointLights[" + std::to_string(lamp1Position.size() + i) + "].position", glm::value_ptr(lampPosition));
			shaderTerrain.setFloat("pointLights[" + std::to_string(lamp1Position.size() + i) + "].constant", 1.0);
			shaderTerrain.setFloat("pointLights[" + std::to_string(lamp1Position.size() + i) + "].linear", 0.09);
			shaderTerrain.setFloat("pointLights[" + std::to_string(lamp1Position.size() + i) + "].quadratic", 0.02);
		}*/

		/*******************************************
		 * 1.- We render the depth buffer
		 *******************************************/
		glClearColor(0.1f, 0.1f, 0.1f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
		// render scene from light's point of view
		glViewport(0, 0, SHADOW_WIDTH, SHADOW_HEIGHT);
		glBindFramebuffer(GL_FRAMEBUFFER, depthMapFBO);
		glClear(GL_DEPTH_BUFFER_BIT);
		//glCullFace(GL_FRONT);
		prepareDepthScene();
		renderScene(false);
		//glCullFace(GL_BACK);
		glBindFramebuffer(GL_FRAMEBUFFER, 0);

		/*******************************************
		 * Debug to view the texture view map
		 *******************************************/
		 // reset viewport
		 /*glViewport(0, 0, screenWidth, screenHeight);
		 glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
		 // render Depth map to quad for visual debugging
		 shaderViewDepth.setMatrix4("projection", 1, false, glm::value_ptr(glm::mat4(1.0)));
		 shaderViewDepth.setMatrix4("view", 1, false, glm::value_ptr(glm::mat4(1.0)));
		 glActiveTexture(GL_TEXTURE0);
		 glBindTexture(GL_TEXTURE_2D, depthMap);
		 boxViewDepth.setScale(glm::vec3(2.0, 2.0, 1.0));
		 boxViewDepth.render();*/

		 /*******************************************
		  * 2.- We render the normal objects
		  *******************************************/
		glViewport(0, 0, screenWidth, screenHeight);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
		prepareScene();
		glActiveTexture(GL_TEXTURE10);
		glBindTexture(GL_TEXTURE_2D, depthMap);
		shaderMulLighting.setInt("shadowMap", 10);
		shaderTerrain.setInt("shadowMap", 10);
		/*******************************************
		 * Skybox
		 *******************************************/
		GLint oldCullFaceMode;
		GLint oldDepthFuncMode;
		// deshabilita el modo del recorte de caras ocultas para ver las esfera desde adentro
		glGetIntegerv(GL_CULL_FACE_MODE, &oldCullFaceMode);
		glGetIntegerv(GL_DEPTH_FUNC, &oldDepthFuncMode);
		shaderSkybox.setFloat("skybox", 0);
		glCullFace(GL_FRONT);
		glDepthFunc(GL_LEQUAL);
		glActiveTexture(GL_TEXTURE0);
		skyboxSphere.render();
		glCullFace(oldCullFaceMode);
		glDepthFunc(oldDepthFuncMode);
		renderScene();
		/*******************************************
		 * Debug to box light box
		 *******************************************/
		 /*glm::vec3 front = glm::normalize(-lightPos);
		 glm::vec3 right = glm::normalize(glm::cross(glm::vec3(0, 1, 0), front));
		 glm::vec3 up = glm::normalize(glm::cross(front, right));
		 glDisable(GL_CULL_FACE);
		 glm::mat4 boxViewTransform = glm::mat4(1.0f);
		 boxViewTransform = glm::translate(boxViewTransform, centerBox);
		 boxViewTransform[0] = glm::vec4(right, 0.0);
		 boxViewTransform[1] = glm::vec4(up, 0.0);
		 boxViewTransform[2] = glm::vec4(front, 0.0);
		 boxViewTransform = glm::scale(boxViewTransform, glm::vec3(shadowBox->getWidth(), shadowBox->getHeight(), shadowBox->getLength()));
		 boxLightViewBox.enableWireMode();
		 boxLightViewBox.render(boxViewTransform);
		 glEnable(GL_CULL_FACE);*/

		 /*******************************************
		  * Creacion de colliders
		  * IMPORTANT do this before interpolations
		  *******************************************/



		if (nivel == 1) {

			// Lamps1 colliders
			for (int i = 0; i < lamp1Position.size(); i++) {
				AbstractModel::OBB lampCollider;
				glm::mat4 modelMatrixColliderLamp = glm::mat4(1.0);
				modelMatrixColliderLamp = glm::translate(modelMatrixColliderLamp, lamp1Position[i]);
				modelMatrixColliderLamp = glm::rotate(modelMatrixColliderLamp, glm::radians(lamp1Orientation[i]),
					glm::vec3(0, 1, 0));
				addOrUpdateColliders(collidersOBB, "lamp1-" + std::to_string(i), lampCollider, modelMatrixColliderLamp);
				// Set the orientation of collider before doing the scale
				lampCollider.u = glm::quat_cast(modelMatrixColliderLamp);
				modelMatrixColliderLamp = glm::scale(modelMatrixColliderLamp, glm::vec3(1.0, 3 + b, 1.0));
				modelMatrixColliderLamp = glm::translate(modelMatrixColliderLamp, modelLamp1.getObb().c);
				lampCollider.c = glm::vec3(modelMatrixColliderLamp[3]);
				lampCollider.e = modelLamp1.getObb().e * glm::vec3(1.0, 3 + b, 1.0);
				std::get<0>(collidersOBB.find("lamp1-" + std::to_string(i))->second) = lampCollider;
			}

			// Lamps2 colliders
			for (int i = 0; i < lamp2Position.size(); i++) {
				AbstractModel::OBB lampCollider;
				glm::mat4 modelMatrixColliderLamp = glm::mat4(1.0);
				modelMatrixColliderLamp = glm::translate(modelMatrixColliderLamp, lamp2Position[i]);
				modelMatrixColliderLamp = glm::rotate(modelMatrixColliderLamp, glm::radians(lamp2Orientation[i]),
					glm::vec3(0, 1, 0));
				addOrUpdateColliders(collidersOBB, "lamp2-" + std::to_string(i), lampCollider, modelMatrixColliderLamp);
				// Set the orientation of collider before doing the scale
				lampCollider.u = glm::quat_cast(modelMatrixColliderLamp);
				modelMatrixColliderLamp = glm::scale(modelMatrixColliderLamp, glm::vec3(1.0, 3.0, 1.0));
				modelMatrixColliderLamp = glm::translate(modelMatrixColliderLamp, modelLamp2.getObb().c);
				lampCollider.c = glm::vec3(modelMatrixColliderLamp[3]);
				lampCollider.e = modelLamp2.getObb().e * glm::vec3(1.0, 3.0, 1.0);
				std::get<0>(collidersOBB.find("lamp2-" + std::to_string(i))->second) = lampCollider;
			}
			//Lampcollliders 3
			for (int i = 0; i < lamp3Position.size(); i++) {
				AbstractModel::OBB lampCollider;
				glm::mat4 modelMatrixColliderLamp = glm::mat4(1.0);
				modelMatrixColliderLamp = glm::translate(modelMatrixColliderLamp, lamp3Position[i]);
				modelMatrixColliderLamp = glm::rotate(modelMatrixColliderLamp, glm::radians(lamp3Orientation[i]),
					glm::vec3(0, 1, 0));
				addOrUpdateColliders(collidersOBB, "lamp3-" + std::to_string(i), lampCollider, modelMatrixColliderLamp);
				// Set the orientation of collider before doing the scale
				lampCollider.u = glm::quat_cast(modelMatrixColliderLamp);
				modelMatrixColliderLamp = glm::scale(modelMatrixColliderLamp, glm::vec3(1.0, 3.0, 1.0));
				modelMatrixColliderLamp = glm::translate(modelMatrixColliderLamp, modelLamp3.getObb().c);
				lampCollider.c = glm::vec3(modelMatrixColliderLamp[3]);
				lampCollider.e = modelLamp3.getObb().e * glm::vec3(1.0, 3.0, 1.0);
				std::get<0>(collidersOBB.find("lamp3-" + std::to_string(i))->second) = lampCollider;
			}
			//Piedras
			for (int i = 0; i < rocas.size(); i++) {
				AbstractModel::OBB lampCollider;
				glm::mat4 modelMatrixColliderLamp = glm::mat4(1.0);
				modelMatrixColliderLamp = glm::translate(modelMatrixColliderLamp, rocas[i]);
				modelMatrixColliderLamp = glm::rotate(modelMatrixColliderLamp, glm::radians(rocasOrientation[i]),
					glm::vec3(0, 1, 0));
				addOrUpdateColliders(collidersOBB, "rocas-" + std::to_string(i), lampCollider, modelMatrixColliderLamp);
				// Set the orientation of collider before doing the scale
				lampCollider.u = glm::quat_cast(modelMatrixColliderLamp);
				modelMatrixColliderLamp = glm::scale(modelMatrixColliderLamp, glm::vec3(0.8));
				modelMatrixColliderLamp = glm::translate(modelMatrixColliderLamp, Piedras1.getObb().c);
				lampCollider.c = glm::vec3(modelMatrixColliderLamp[3]);
				lampCollider.e = Piedras1.getObb().e * glm::vec3(0.8);
				std::get<0>(collidersOBB.find("rocas-" + std::to_string(i))->second) = lampCollider;
			}


			// ****************************Collider de Protagonista
			AbstractModel::OBB protaCollider;
			glm::mat4 modelmatrixColliderProta = glm::mat4(modelMatrixProta);
			modelmatrixColliderProta = glm::rotate(modelmatrixColliderProta,
				glm::radians(-90.0f), glm::vec3(1, 0, 0));
			// Set the orientation of collider before doing the scale
			protaCollider.u = glm::quat_cast(modelmatrixColliderProta);
			modelmatrixColliderProta = glm::scale(modelmatrixColliderProta, glm::vec3(2.0));
			modelmatrixColliderProta = glm::translate(modelmatrixColliderProta,
				glm::vec3(protaFI.getObb().c.x,
					protaFI.getObb().c.y,
					protaFI.getObb().c.z));
			protaCollider.e = protaFI.getObb().e * glm::vec3(2.0) * glm::vec3(0.787401574, 0.787401574, 0.787401574);
			protaCollider.c = glm::vec3(modelmatrixColliderProta[3]);
			addOrUpdateColliders(collidersOBB, "Prota", protaCollider, modelMatrixProta);



			//collider nave
			AbstractModel::OBB parteNave1Collider;
			glm::mat4 modelmatrixColliderparteNave1 = glm::mat4(modelMatrixParteNave1);
			parteNave1Collider.u = glm::quat_cast(modelmatrixColliderparteNave1);
			modelmatrixColliderparteNave1 = glm::scale(modelmatrixColliderparteNave1, glm::vec3(1, 1, 1));
			modelmatrixColliderparteNave1 = glm::translate(modelmatrixColliderparteNave1, glm::vec3(88, 1, -75)); //si le cambio el scale a la matriz del collider el desplazamiento cambia en proporcion
			parteNave1Collider.e = parteNave1.getObb().e * glm::vec3(2, 2, 2);
			parteNave1Collider.c = glm::vec3(modelmatrixColliderparteNave1[3]);
			addOrUpdateColliders(collidersOBB, "partenave1", parteNave1Collider, modelMatrixParteNave1);

			AbstractModel::OBB parteNave2Collider;
			glm::mat4 modelmatrixColliderparteNave2 = glm::mat4(modelMatrixParteNave2);
			parteNave2Collider.u = glm::quat_cast(modelmatrixColliderparteNave2);
			modelmatrixColliderparteNave2 = glm::scale(modelmatrixColliderparteNave2, glm::vec3(1, 1, 1));
			modelmatrixColliderparteNave2 = glm::translate(modelmatrixColliderparteNave2, glm::vec3(74, 1, 45)); //si le cambio el scale a la matriz del collider el desplazamiento cambia en proporcion
			parteNave2Collider.e = parteNave2.getObb().e * glm::vec3(2, 2, 2);
			parteNave2Collider.c = glm::vec3(modelmatrixColliderparteNave2[3]);
			addOrUpdateColliders(collidersOBB, "partenave2", parteNave2Collider, modelMatrixParteNave2);

			AbstractModel::OBB parteNave3Collider;
			glm::mat4 modelmatrixColliderparteNave3 = glm::mat4(modelMatrixParteNave3);
			parteNave3Collider.u = glm::quat_cast(modelmatrixColliderparteNave3);
			modelmatrixColliderparteNave3 = glm::scale(modelmatrixColliderparteNave3, glm::vec3(1, 1, 1));
			modelmatrixColliderparteNave3 = glm::translate(modelmatrixColliderparteNave3, glm::vec3(-15, 4.1, -14.1)); //si le cambio el scale a la matriz del collider el desplazamiento cambia en proporcion
			parteNave3Collider.e = parteNave3.getObb().e * glm::vec3(2, 2, 2);
			parteNave3Collider.c = glm::vec3(modelmatrixColliderparteNave3[3]);
			addOrUpdateColliders(collidersOBB, "partenave3", parteNave3Collider, modelMatrixParteNave3);

			AbstractModel::OBB parteNave4Collider;
			glm::mat4 modelmatrixColliderparteNave4 = glm::mat4(modelMatrixParteNave4);
			parteNave4Collider.u = glm::quat_cast(modelmatrixColliderparteNave4);
			modelmatrixColliderparteNave4 = glm::scale(modelmatrixColliderparteNave4, glm::vec3(1, 1, 1));
			modelmatrixColliderparteNave4 = glm::translate(modelmatrixColliderparteNave4, glm::vec3(-40, 1, 30)); //si le cambio el scale a la matriz del collider el desplazamiento cambia en proporcion
			parteNave4Collider.e = parteNave4.getObb().e * glm::vec3(2, 2, 2);
			parteNave4Collider.c = glm::vec3(modelmatrixColliderparteNave4[3]);
			addOrUpdateColliders(collidersOBB, "partenave4", parteNave4Collider, modelMatrixParteNave4);
		}
		else if (nivel == 4)
		{
			//Piedras2
			for (int i = 0; i < rocas1.size(); i++) {
				AbstractModel::OBB lampCollider;
				glm::mat4 modelMatrixColliderLamp = glm::mat4(1.0);
				modelMatrixColliderLamp = glm::translate(modelMatrixColliderLamp, rocas1[i]);
				modelMatrixColliderLamp = glm::rotate(modelMatrixColliderLamp, glm::radians(rocas1Orientation[i]),
					glm::vec3(0, 1, 0));
				addOrUpdateColliders(collidersOBB, "rocas1-" + std::to_string(i), lampCollider, modelMatrixColliderLamp);
				// Set the orientation of collider before doing the scale
				lampCollider.u = glm::quat_cast(modelMatrixColliderLamp);
				modelMatrixColliderLamp = glm::scale(modelMatrixColliderLamp, glm::vec3(0.25));
				modelMatrixColliderLamp = glm::translate(modelMatrixColliderLamp, Rocas1.getObb().c);
				lampCollider.c = glm::vec3(modelMatrixColliderLamp[3]);
				lampCollider.e = Rocas1.getObb().e * glm::vec3(0.25);
				std::get<0>(collidersOBB.find("rocas1-" + std::to_string(i))->second) = lampCollider;
			}
			//Piedras2
			for (int i = 0; i < rocas2.size(); i++) {
				AbstractModel::OBB lampCollider;
				glm::mat4 modelMatrixColliderLamp = glm::mat4(1.0);
				modelMatrixColliderLamp = glm::translate(modelMatrixColliderLamp, rocas2[i]);
				modelMatrixColliderLamp = glm::rotate(modelMatrixColliderLamp, glm::radians(rocas2Orientation[i]),
					glm::vec3(0, 1, 0));
				addOrUpdateColliders(collidersOBB, "rocas2-" + std::to_string(i), lampCollider, modelMatrixColliderLamp);
				// Set the orientation of collider before doing the scale
				lampCollider.u = glm::quat_cast(modelMatrixColliderLamp);
				modelMatrixColliderLamp = glm::scale(modelMatrixColliderLamp, glm::vec3(0.75));
				modelMatrixColliderLamp = glm::translate(modelMatrixColliderLamp, Rocas2.getObb().c);
				lampCollider.c = glm::vec3(modelMatrixColliderLamp[3]);
				lampCollider.e = Rocas2.getObb().e * glm::vec3(0.75);
				std::get<0>(collidersOBB.find("rocas2-" + std::to_string(i))->second) = lampCollider;
			}
			for (int i = 0; i < Hongos2.size(); i++) {
				AbstractModel::OBB lampCollider;
				glm::mat4 modelMatrixColliderLamp = glm::mat4(1.0);
				modelMatrixColliderLamp = glm::translate(modelMatrixColliderLamp, Hongos2[i]);
				modelMatrixColliderLamp = glm::rotate(modelMatrixColliderLamp, glm::radians(Hongos2Orientation[i]),
					glm::vec3(0, 1, 0));
				addOrUpdateColliders(collidersOBB, "Hongos-" + std::to_string(i), lampCollider, modelMatrixColliderLamp);
				// Set the orientation of collider before doing the scale
				lampCollider.u = glm::quat_cast(modelMatrixColliderLamp);
				modelMatrixColliderLamp = glm::scale(modelMatrixColliderLamp, glm::vec3(3.0));
				modelMatrixColliderLamp = glm::translate(modelMatrixColliderLamp, Hongos.getObb().c);
				lampCollider.c = glm::vec3(modelMatrixColliderLamp[3]);
				lampCollider.e = Hongos.getObb().e * glm::vec3(1.0, 3.0, 1.0);
				std::get<0>(collidersOBB.find("Hongos-" + std::to_string(i))->second) = lampCollider;
			}
			// ****************************Collider de Protagonista
			AbstractModel::OBB protaCollider;
			glm::mat4 modelmatrixColliderProta = glm::mat4(modelMatrixNaveCompleta);
			/*modelmatrixColliderProta = glm::rotate(modelmatrixColliderProta,
				glm::radians(-90.0f), glm::vec3(1, 0, 0));*/
				// Set the orientation of collider before doing the scale
			protaCollider.u = glm::quat_cast(modelmatrixColliderProta);
			modelmatrixColliderProta = glm::scale(modelmatrixColliderProta, glm::vec3(0.45, 0.6, 0.965));
			modelmatrixColliderProta = glm::translate(modelmatrixColliderProta,
				glm::vec3(naveCompleta.getObb().c.x,
					naveCompleta.getObb().c.y,
					naveCompleta.getObb().c.z));
			protaCollider.e = naveCompleta.getObb().e * glm::vec3(0.45, 0.6, 0.965);
			protaCollider.c = glm::vec3(modelmatrixColliderProta[3]);
			addOrUpdateColliders(collidersOBB, "Nave", protaCollider, modelMatrixNaveCompleta);
			/*	std::map<std::string, std::tuple<AbstractModel::OBB, glm::mat4, glm::mat4> > collidersOBB;
				std::map<std::string, std::tuple<AbstractModel::SBB, glm::mat4, glm::mat4> > collidersSBB;*/

			glm::mat4 tuculoM = glm::mat4(1.0f);
			tuculoM = glm::translate(tuculoM, glm::vec3(110, 0, 110));
			AbstractModel::OBB parteNave4Collider;
			glm::mat4 modelmatrixColliderparteNave4 = glm::mat4(tuculoM);
			parteNave4Collider.u = glm::quat_cast(modelmatrixColliderparteNave4);
			modelmatrixColliderparteNave4 = glm::scale(modelmatrixColliderparteNave4, glm::vec3(1, 1, 1));
			modelmatrixColliderparteNave4 = glm::translate(modelmatrixColliderparteNave4, glm::vec3(-40, 1, 30)); //si le cambio el scale a la matriz del collider el desplazamiento cambia en proporcion
			parteNave4Collider.e = parteNave4.getObb().e * glm::vec3(2, 2, 2);
			parteNave4Collider.c = glm::vec3(modelmatrixColliderparteNave4[3]);
			addOrUpdateColliders(collidersOBB, "partenave4", parteNave4Collider, tuculoM);
			addOrUpdateColliders(collidersOBB, "partenave3", parteNave4Collider, tuculoM);
			addOrUpdateColliders(collidersOBB, "partenave2", parteNave4Collider, tuculoM);
			addOrUpdateColliders(collidersOBB, "partenave1", parteNave4Collider, tuculoM);
			addOrUpdateColliders(collidersOBB, "Prota", parteNave4Collider, tuculoM);
			for (int i = 0; i < rocas.size(); i++) {
				addOrUpdateColliders(collidersOBB, "rocas-" + std::to_string(i), parteNave4Collider, tuculoM);
			}
			for (int i = 0; i < lamp1Position.size(); i++) {
				addOrUpdateColliders(collidersOBB, "lamp1-" + std::to_string(i), parteNave4Collider, tuculoM);
			}
			for (int i = 0; i < lamp2Position.size(); i++) {
				addOrUpdateColliders(collidersOBB, "lamp2-" + std::to_string(i), parteNave4Collider, tuculoM);
			}
			for (int i = 0; i < lamp3Position.size(); i++) {
				addOrUpdateColliders(collidersOBB, "lamp3-" + std::to_string(i), parteNave4Collider, tuculoM);
			}
		}
		//No importanto el nivel
		/*******************************************
		 * Render de colliders
		 *******************************************/
		 /*for (std::map<std::string, std::tuple<AbstractModel::OBB, glm::mat4, glm::mat4> >::iterator it =
			 collidersOBB.begin(); it != collidersOBB.end(); it++) {
			 glm::mat4 matrixCollider = glm::mat4(1.0);
			 matrixCollider = glm::translate(matrixCollider, std::get<0>(it->second).c);
			 matrixCollider = matrixCollider * glm::mat4(std::get<0>(it->second).u);
			 matrixCollider = glm::scale(matrixCollider, std::get<0>(it->second).e * 2.0f);
			 boxCollider.setColor(glm::vec4(1.0, 1.0, 1.0, 1.0));
			 boxCollider.enableWireMode();
			 boxCollider.render(matrixCollider);
		 }

		 for (std::map<std::string, std::tuple<AbstractModel::SBB, glm::mat4, glm::mat4> >::iterator it =
			 collidersSBB.begin(); it != collidersSBB.end(); it++) {
			 glm::mat4 matrixCollider = glm::mat4(1.0);
			 matrixCollider = glm::translate(matrixCollider, std::get<0>(it->second).c);
			 matrixCollider = glm::scale(matrixCollider, glm::vec3(std::get<0>(it->second).ratio * 2.0f));
			 sphereCollider.setColor(glm::vec4(1.0, 1.0, 1.0, 1.0));
			 sphereCollider.enableWireMode();
			 sphereCollider.render(matrixCollider);
		 }*/
		 //tampoco importa el nivel
		 /*******************************************
		  * Test Colisions
		  *******************************************/
		int axesCount, buttonscount;
		const float *axes = glfwGetJoystickAxes(GLFW_JOYSTICK_1, &axesCount);
		const unsigned char  *buttons = glfwGetJoystickButtons(GLFW_JOYSTICK_1, &buttonscount);
		for (std::map<std::string,
			std::tuple<AbstractModel::OBB, glm::mat4, glm::mat4> >::iterator it =
			collidersOBB.begin(); it != collidersOBB.end(); it++) {
			bool isCollision = false;
			for (std::map<std::string,
				std::tuple<AbstractModel::OBB, glm::mat4, glm::mat4> >::iterator jt =
				collidersOBB.begin(); jt != collidersOBB.end(); jt++) {
				if (it != jt
					&& testOBBOBB(std::get<0>(it->second),
						std::get<0>(jt->second))) {
					isCollision = true;
					if (GLFW_PRESS == buttons[2] || glfwGetKey(window, GLFW_KEY_E) == GLFW_PRESS) {
						if (jt->first.compare("partenave1") == 0) {
							parte1 = true;
							banderadetrescolores = 1;
						}
						if (jt->first.compare("partenave2") == 0) {
							parte2 = true;
							//std::cout << "Parte2" << std::endl;
							banderadetrescolores1 = 1;
						}
						if (jt->first.compare("partenave3") == 0) {
							parte3 = true;
							banderadetrescolores2 = 1;
							//std::cout << "Parte3" << std::endl;
						}
						if (jt->first.compare("partenave4") == 0) {
							parte4 = true;
							banderadetrescolores3 = 1;
							
						}
					}
				}
			}
			addOrUpdateCollisionDetection(collisionDetection, it->first, isCollision);
		}

		for (std::map<std::string,
			std::tuple<AbstractModel::SBB, glm::mat4, glm::mat4> >::iterator it =
			collidersSBB.begin(); it != collidersSBB.end(); it++) {
			bool isCollision = false;
			for (std::map<std::string,
				std::tuple<AbstractModel::SBB, glm::mat4, glm::mat4> >::iterator jt =
				collidersSBB.begin(); jt != collidersSBB.end(); jt++) {
				if (it != jt
					&& testSphereSphereIntersection(std::get<0>(it->second),
						std::get<0>(jt->second))) {
					std::cout << "Colision " << it->first << " with "
						<< jt->first << std::endl;
					isCollision = true;

				}
			}
			addOrUpdateCollisionDetection(collisionDetection, it->first, isCollision);
		}

		for (std::map<std::string,
			std::tuple<AbstractModel::SBB, glm::mat4, glm::mat4> >::iterator it =
			collidersSBB.begin(); it != collidersSBB.end(); it++) {
			bool isCollision = false;
			std::map<std::string,
				std::tuple<AbstractModel::OBB, glm::mat4, glm::mat4> >::iterator jt =
				collidersOBB.begin();
			for (; jt != collidersOBB.end(); jt++) {
				if (testSphereOBox(std::get<0>(it->second),
					std::get<0>(jt->second))) {
					isCollision = true;
					addOrUpdateCollisionDetection(collisionDetection, jt->first, isCollision);
				}
			}
			addOrUpdateCollisionDetection(collisionDetection, it->first, isCollision);
		}
		int control = glfwJoystickPresent(GLFW_JOYSTICK_1);
		//std::cout << "Joystick status :" << control << std::endl;



		std::map<std::string, bool>::iterator colIt;
		for (colIt = collisionDetection.begin(); colIt != collisionDetection.end();
			colIt++) {
			std::map<std::string,
				std::tuple<AbstractModel::SBB, glm::mat4, glm::mat4> >::iterator it =
				collidersSBB.find(colIt->first);
			std::map<std::string,
				std::tuple<AbstractModel::OBB, glm::mat4, glm::mat4> >::iterator jt =
				collidersOBB.find(colIt->first);
			if (it != collidersSBB.end()) {
				if (!colIt->second)
					addOrUpdateColliders(collidersSBB, it->first);
			}
			if (jt != collidersOBB.end()) {
				if (!colIt->second)
					addOrUpdateColliders(collidersOBB, jt->first);
				else {
					if (jt->first.compare("Prota") == 0)
					{
						modelMatrixProta = std::get<1>(jt->second);
					}
					if (jt->first.compare("Nave") == 0)
					{
						modelMatrixNaveCompleta[3][0] = 52;
						modelMatrixNaveCompleta[3][2] = 72;



						primeraCol = true;

						contadorTiempo2 = 0;
					}
					//aqui se agregan los frenos para las colisiones
				}
			}
		}

		// Constantes de animaciones
		animationIndex = 2;

		/*******************************************
		 * State machines
		 *******************************************/

		 //glfwSwapBuffers(window);

		 /****************************+
		  * Open AL sound data
		  */
		alSourcefv(source[0], AL_POSITION, source0Pos);
		//contadorTiempo2 += 1;
		if (nivel == 0) {

			std::string texto;
			if (nostart == 0)
			{
				modelText->render("Press Start To Continue ...", -0.8, -0.5);
				glfwSwapBuffers(window);

			}
			else
			{
				//std::cout << "Contando" << std::endl;
				contadorlv0++;
				modelText->render("Una mision espacial comandada", -0.95, 0.9);
				modelText->render("por la FI salio mal. . .", -0.95, 0.8);
				modelText->render("El robot de alta ingenieria quedo", -0.95, 0.6);
				modelText->render("varado en un planeta desconocido", -0.95, 0.5);
				modelText->render("Ayuda a Botsito a encontrar", -0.95, 0.3);
				modelText->render("todas las partes de su nave", -0.95, 0.2);
				modelText->render("Para poder escapar", -0.95, 0.1);
				modelText->render("Tienes 40 segundos. . .", -0.95, -0.1);
				modelText->render("Antes de que se acabe la bateria", -0.95, -0.2);
				modelText->render("!!CORREEEEE!!", -0.3, -0.5);
				glfwSwapBuffers(window);

			}


			if ((contadorlv0 / 60) > 20)
			{
				//std::cout << "CAmbio" << std::endl;
				nivel = 1;
			}
		}
		else if (nivel == 1)
		{
			//glfwSwapBuffers(window);
			//cambiar texto para primer nivel
			int partes = 4 - (banderadetrescolores + banderadetrescolores1 + banderadetrescolores2 + banderadetrescolores3);
			if (controles == 1) {
				modelText->render("BATERIA = VERDE DE LA CABEZA", -0.95, 0.2);
				modelText->render("Tomar = boton X mando", -0.95, 0.1);
				modelText->render("Saltar = boton A mando", -0.95, -0.1);
				modelText->render("Mov = Joystick izquierdo", -0.95, -0.2);
			}
			modelText->render("Controles -> mantener B mando", -0.95, 0.9);
			modelText->render("Partes restantes:", -0.95, -0.7);
			modelText1->render(std::to_string(partes), -0.95, -0.8);
			glfwSwapBuffers(window);
			if (banderadetrescolores + banderadetrescolores1 + banderadetrescolores2 + banderadetrescolores3 < 4) {
				contadorTiempo += 1;
			}
			else if (nivel != 5 && nivel != 6)
			{
				nivel = 2;
			}

			//glfwSwapBuffers(window);
		}
		else if (nivel == 2) {
			modelText->render("Tu nave esta ...", -0.95, 0.3);
			modelText->render("Casi lista", -0.95, 0.2);
			modelText->render("Una turbina fallo", -0.95, 0.1);
			modelText->render("Debes acelerar por 30 segundos", -0.95, -0.1);
			modelText->render("seguidos sin chocar para volar", -0.95, -0.2);
			modelText->render("!!tienes 3 oportunidades!!", -0.4, -0.4);
			glfwSwapBuffers(window);
			contadorlv2++;
			if (contadorlv2 / 60 > 10)
			{
				nivel = 4;
			}
		}
		else if (nivel == 3) {
			modelText->render("Botsito Se quedo sin energia", -0.80, 0.0);
			glfwSwapBuffers(window);
		}
		else if (nivel == 4) {
			if (controles == 1) {
				modelText->render("Adelante = Gatillo Derecho", -0.95, 0.1);
				modelText->render("Atras = Gatillo Izquierdo", -0.95, 0.0);
				modelText->render("Mov = Joystick izquierdo", -0.95, -0.1);
			}
			modelText->render("Vidas:", 0.6, -0.8);
			modelText->render(std::to_string(3 - contador), 0.6, -0.9);
			modelText->render("Controles -> mantener B mando", -0.95, 0.9);
			modelText->render("Tiempo Acelerando", -0.95, -0.8);
			modelText->render(std::to_string(contadorTiempo2 / 59), -0.95, -0.9);
			glfwSwapBuffers(window);
			if (contadorTiempo2 / 59 >= 30 && segundoLev != 1)
			{

				nivel = 5;
				std::cout << "Ganaste" << std::endl;
			}

			if (primeraCol == true)
			{
				primeraCol = false;
				contador++;
				std::cout << "Contador:" << contador << std::endl;


			}
			if (contador > 2) {
				nivel = 6;
				//std::cout << "Mamaste hijo de perra" << std::endl;
			}

			//glfwSwapBuffers(window);
		}
		else if (nivel == 5) {
			modelText->render("Botsito logro escapar", -0.5, -0.5);
			glfwSwapBuffers(window);
		}
		else if (nivel == 6) {
			modelText->render("Destruiste la Nave", -0.5, -0.6);
			glfwSwapBuffers(window);
		}
		else {
			glfwSwapBuffers(window);
		}


		//contadorTiempo += 1;
		//std::cout << "TIEMPOOOOOOOOOOOOOOO" << contadorTiempo <<"sec"<< contadorTiempo/60<< std::endl;

		alListenerfv(AL_ORIENTATION, listenerOri);
		if (nivel == 1) {
			// Listener for the Thris person camera
			listenerPos[0] = modelMatrixProta[3].x;
			listenerPos[1] = modelMatrixProta[3].y;
			listenerPos[2] = modelMatrixProta[3].z;
			alListenerfv(AL_POSITION, listenerPos);
			glm::vec3 upModel = glm::normalize(modelMatrixProta[1]);
			glm::vec3 frontModel = glm::normalize(modelMatrixProta[2]);

			listenerOri[0] = frontModel.x;
			listenerOri[1] = frontModel.y;
			listenerOri[2] = frontModel.z;
			listenerOri[3] = upModel.x;
			listenerOri[4] = upModel.y;
			listenerOri[5] = upModel.z;
			for (unsigned int i = 0; i < sourcesPlay.size(); i++) {
				if (sourcesPlay[i]) {
					sourcesPlay[i] = false;
					alSourcePlay(source[i]);
				}
			}
		}
		else {
			for (unsigned int i = 0; i < sourcesPlay.size(); i++) {

				alSourceStop(source[i]);

			}
		}
		if (nivel == 4)
		{
			listenerPos[0] = modelMatrixNaveCompleta[3].x;
			listenerPos[1] = modelMatrixNaveCompleta[3].y;
			listenerPos[2] = modelMatrixNaveCompleta[3].z;
			alListenerfv(AL_POSITION, listenerPos);
			glm::vec3 upModel = glm::normalize(modelMatrixProta[1]);
			glm::vec3 frontModel = glm::normalize(modelMatrixProta[2]);

			listenerOri[0] = frontModel.x;
			listenerOri[1] = frontModel.y;
			listenerOri[2] = frontModel.z;
			listenerOri[3] = upModel.x;
			listenerOri[4] = upModel.y;
			listenerOri[5] = upModel.z;
			for (unsigned int i = 0; i < sourcesPlay1.size(); i++) {
				if (sourcesPlay1[i]) {
					sourcesPlay1[i] = false;
					alSourcePlay(source1[i]);
				}
			}
		}
	}
}

void prepareScene() {

	skyboxSphere.setShader(&shaderSkybox);



	terrain.setShader(&shaderTerrain);
	pantalla1.setShader(&shaderMulLighting2);
	pantalla2.setShader(&shaderMulLighting2);

	pantalla3.setShader(&shaderMulLighting2);
	pantalla4.setShader(&shaderMulLighting2);
	pantalla5.setShader(&shaderMulLighting2);
	pantalla6.setShader(&shaderMulLighting2);






	//Lamp models
	modelLamp1.setShader(&shaderMulLighting);
	modelLamp2.setShader(&shaderMulLighting);
	modelLamp3.setShader(&shaderMulLighting);
	Piedras1.setShader(&shaderMulLighting);
	//Grass
	modelGrass.setShader(&shaderMulLighting);
	HongoRecio.setShader(&shaderMulLighting);
	//Prota
	protaFI.setShader(&shaderMulLighting);
}

void prepareDepthScene() {

	skyboxSphere.setShader(&shaderDepth);


	terrain.setShader(&shaderDepth);
	//Lamp models
	modelLamp1.setShader(&shaderMulLighting);
	modelLamp2.setShader(&shaderMulLighting);
	modelLamp3.setShader(&shaderMulLighting);
	Piedras1.setShader(&shaderMulLighting);
	//Grass
	modelGrass.setShader(&shaderMulLighting);
	HongoRecio.setShader(&shaderMulLighting);
	//Prota
	protaFI.setShader(&shaderMulLighting);
}

void renderScene(bool renderParticles) {

	if (nivel == 0) {
		if (contadorlv0 < 10) {
			glm::mat4 modelMatrixNivelCero = glm::mat4(modelMatrixPantalla1);
			modelMatrixNivelCero = glm::translate(modelMatrixNivelCero, glm::vec3(0, 0.0, 0));
			modelMatrixNivelCero = glm::scale(modelMatrixNivelCero, glm::vec3(3.0, 3.5, 5.0));
			modelMatrixNivelCero = glm::rotate(modelMatrixNivelCero, glm::radians(110.0f), glm::vec3(0, 0, 1));
			modelMatrixNivelCero = glm::rotate(modelMatrixNivelCero, glm::radians(90.0f), glm::vec3(0, 1, 0));

			pantalla1.render(modelMatrixNivelCero);
			//glEnable(GL_CULL_FACE);
		}
		if (contadorlv0 >= 10) {
			glm::mat4 modelMatrixNivelCero = glm::mat4(modelMatrixPantalla4);
			modelMatrixNivelCero = glm::translate(modelMatrixNivelCero, glm::vec3(0, 0.0, 0));
			modelMatrixNivelCero = glm::scale(modelMatrixNivelCero, glm::vec3(3.0, 3.5, 5.0));
			modelMatrixNivelCero = glm::rotate(modelMatrixNivelCero, glm::radians(110.0f), glm::vec3(0, 0, 1));
			modelMatrixNivelCero = glm::rotate(modelMatrixNivelCero, glm::radians(90.0f), glm::vec3(0, 1, 0));

			pantalla4.render(modelMatrixNivelCero);
			//glEnable(GL_CULL_FACE);
		}


	}
	if (nivel == 1) {

		// Forze to enable the unit texture to 0 always ----------------- IMPORTANT
		glm::mat4 modelCesped = glm::mat4(1.0);
		modelCesped = glm::translate(modelCesped, glm::vec3(0.0, 0.0, 0.0));
		modelCesped = glm::scale(modelCesped, glm::vec3(200.0, 0.001, 200.0));
		// Se activa la textura del background
		glActiveTexture(GL_TEXTURE0);
		glBindTexture(GL_TEXTURE_2D, textureTerrainBackgroundID);
		shaderTerrain.setInt("backgroundTexture", 0);
		// Se activa la textura de tierra
		glActiveTexture(GL_TEXTURE1);
		glBindTexture(GL_TEXTURE_2D, textureTerrainRID);
		shaderTerrain.setInt("rTexture", 1);
		// Se activa la textura de hierba
		glActiveTexture(GL_TEXTURE2);
		glBindTexture(GL_TEXTURE_2D, textureTerrainGID);
		shaderTerrain.setInt("gTexture", 2);
		// Se activa la textura del camino
		glActiveTexture(GL_TEXTURE3);
		glBindTexture(GL_TEXTURE_2D, textureTerrainBID);
		shaderTerrain.setInt("bTexture", 3);
		// Se activa la textura del blend map
		glActiveTexture(GL_TEXTURE4);
		glBindTexture(GL_TEXTURE_2D, textureTerrainBlendMapID);
		shaderTerrain.setInt("blendMapTexture", 4);
		shaderTerrain.setVectorFloat2("scaleUV", glm::value_ptr(glm::vec2(40, 40)));
		terrain.render();
		shaderTerrain.setVectorFloat2("scaleUV", glm::value_ptr(glm::vec2(0, 0)));
		glBindTexture(GL_TEXTURE_2D, 0);
		//glActiveTexture(GL_TEXTURE0);

		for (int i = 0; i < lamp1Position.size(); i++) {
			lamp1Position[i].y = terrain.getHeightTerrain(lamp1Position[i].x, lamp1Position[i].z);
			modelLamp1.setPosition(lamp1Position[i]);

			modelLamp1.setScale(glm::vec3(3.0 + a, 4 + b, 3.0 + c));
			modelLamp1.setOrientation(glm::vec3(0, lamp1Orientation[i], 0));
			modelLamp1.render();
		}

		for (int i = 0; i < lamp2Position.size(); i++) {
			lamp2Position[i].y = terrain.getHeightTerrain(lamp2Position[i].x, lamp2Position[i].z);
			modelLamp2.setPosition(lamp2Position[i]);
			modelLamp2.setScale(glm::vec3(4.0));
			modelLamp2.setOrientation(glm::vec3(0, lamp2Orientation[i], 0));
			modelLamp2.render();
		}

		for (int i = 0; i < lamp3Position.size(); i++) {
			lamp3Position[i].y = terrain.getHeightTerrain(lamp3Position[i].x, lamp3Position[i].z);
			modelLamp3.setPosition(lamp3Position[i]);
			modelLamp3.setScale(glm::vec3(4.0));
			modelLamp3.setOrientation(glm::vec3(0, lamp3Orientation[i], 0));
			modelLamp3.render();
		}

		for (int i = 0; i < rocas.size(); i++) {
			rocas[i].y = terrain.getHeightTerrain(rocas[i].x, rocas[i].z);
			Piedras1.setPosition(rocas[i]);
			Piedras1.setScale(glm::vec3(1.0));
			Piedras1.setOrientation(glm::vec3(0, rocasOrientation[i], 0));
			Piedras1.render();
		}

		// Grass
		glDisable(GL_CULL_FACE);
		glm::vec3 grassPosition = glm::vec3(0.0, 0.0, 0.0);
		grassPosition.y = terrain.getHeightTerrain(grassPosition.x, grassPosition.z);
		modelGrass.setPosition(grassPosition);
		modelGrass.render();
		glEnable(GL_CULL_FACE);

		if (!parte1) {
			//Partes de la nave
			glEnable(GL_CULL_FACE);
			modelMatrixParteNave1[3][1] = terrain.getHeightTerrain(modelMatrixParteNave1[3][0], modelMatrixParteNave1[3][2]);
			glm::mat4 modelMatrixParteNave1Core = glm::mat4(modelMatrixParteNave1);
			modelMatrixParteNave1Core = glm::translate(modelMatrixParteNave1Core, glm::vec3(88, 0, -75));
			modelMatrixParteNave1Core = glm::scale(modelMatrixParteNave1Core, glm::vec3(2, 2, 2));
			parteNave1.render(modelMatrixParteNave1Core);
		}
		if (!parte2) {
			//parte 2
			//Partes de la nave
			glEnable(GL_CULL_FACE);
			modelMatrixParteNave2[3][1] = terrain.getHeightTerrain(modelMatrixParteNave2[3][0], modelMatrixParteNave2[3][2]);
			glm::mat4 modelMatrixParteNave2Core = glm::mat4(modelMatrixParteNave2);
			modelMatrixParteNave2Core = glm::translate(modelMatrixParteNave2Core, glm::vec3(74, 0, 45));
			modelMatrixParteNave2Core = glm::scale(modelMatrixParteNave2Core, glm::vec3(2, 2, 2));
			parteNave2.render(modelMatrixParteNave2Core);
		}
		if (!parte3) {
			//parte 3
			//Partes de la nave
			glEnable(GL_CULL_FACE);
			modelMatrixParteNave3[3][1] = terrain.getHeightTerrain(modelMatrixParteNave3[3][0], modelMatrixParteNave3[3][2]);
			glm::mat4 modelMatrixParteNave3Core = glm::mat4(modelMatrixParteNave3);
			modelMatrixParteNave3Core = glm::translate(modelMatrixParteNave3Core, glm::vec3(-15, 3.7, -25));
			modelMatrixParteNave3Core = glm::scale(modelMatrixParteNave3Core, glm::vec3(2, 2, 2));
			parteNave3.render(modelMatrixParteNave3Core);
		}
		if (!parte4) {
			//parte 4
			//Partes de la nave
			glEnable(GL_CULL_FACE);
			modelMatrixParteNave4[3][1] = terrain.getHeightTerrain(modelMatrixParteNave4[3][0], modelMatrixParteNave4[3][2]);
			glm::mat4 modelMatrixParteNave4Core = glm::mat4(modelMatrixParteNave4);
			modelMatrixParteNave4Core = glm::translate(modelMatrixParteNave4Core, glm::vec3(-40, 0, 30));
			modelMatrixParteNave4Core = glm::scale(modelMatrixParteNave4Core, glm::vec3(2, 2, 2));
			parteNave4.render(modelMatrixParteNave4Core);
		}

		/*******************************************
		 * Custom Anim objects obj
		 *******************************************/


		 //Saltar
		modelMatrixProta[3][1] = -gravity * tmv * tmv + 5.5 * tmv + terrain.getHeightTerrain(modelMatrixProta[3][0], modelMatrixProta[3][2]);
		tmv = currTime - startTimeJump;
		if (modelMatrixProta[3][1] < terrain.getHeightTerrain(modelMatrixProta[3][0], modelMatrixProta[3][2])) {
			isJump = false;
			modelMatrixProta[3][1] = terrain.getHeightTerrain(modelMatrixProta[3][0], modelMatrixProta[3][2]);
		}
		glm::mat4 modelMatrixProtaBody = glm::mat4(modelMatrixProta);
		modelMatrixProtaBody = glm::rotate(modelMatrixProtaBody, glm::radians(0.0f), glm::vec3(0, 1, 0));
		modelMatrixProtaBody = glm::scale(modelMatrixProtaBody, glm::vec3(0.021, 0.021, 0.021));

		if (contadorTiempo / 59 >= 0 && contadorTiempo / 59 < 10) {
			protaFI.setAnimationIndex(animationIndex);
			protaFI.render(modelMatrixProtaBody);

		}
		else if (contadorTiempo / 59 >= 10 && contadorTiempo / 59 < 20) {
			protaFI1.setAnimationIndex(animationIndex);
			protaFI1.render(modelMatrixProtaBody);
		}
		else if (contadorTiempo / 59 >= 20 && contadorTiempo / 59 < 30) {
			protaFI2.setAnimationIndex(animationIndex);
			protaFI2.render(modelMatrixProtaBody);
		}
		else if (contadorTiempo / 59 >= 30 && contadorTiempo / 59 < 40) {
			protaFI3.setAnimationIndex(animationIndex);
			protaFI3.render(modelMatrixProtaBody);
		}
		else if (contadorTiempo / 59 >= 40) {
			nivel = 3;
		}
		//glEnable(GL_CULL_FACE);
	}
	if (nivel == 2) {

		glm::mat4 modelMatrixNivelCero = glm::mat4(modelMatrixPantalla2);
		modelMatrixNivelCero = glm::translate(modelMatrixNivelCero, glm::vec3(0, 0.0, 0));
		modelMatrixNivelCero = glm::scale(modelMatrixNivelCero, glm::vec3(3.0, 3.5, 5.0));
		modelMatrixNivelCero = glm::rotate(modelMatrixNivelCero, glm::radians(110.0f), glm::vec3(0, 0, 1));
		modelMatrixNivelCero = glm::rotate(modelMatrixNivelCero, glm::radians(90.0f), glm::vec3(0, 1, 0));

		pantalla2.render(modelMatrixNivelCero);
		//glEnable(GL_CULL_FACE);

	}
	if (nivel == 3) {

		glm::mat4 modelMatrixNivelCero = glm::mat4(modelMatrixPantalla3);
		modelMatrixNivelCero = glm::translate(modelMatrixNivelCero, glm::vec3(0, 0.0, 0));
		modelMatrixNivelCero = glm::scale(modelMatrixNivelCero, glm::vec3(3.0, 3.5, 5.0));
		modelMatrixNivelCero = glm::rotate(modelMatrixNivelCero, glm::radians(110.0f), glm::vec3(0, 0, 1));
		modelMatrixNivelCero = glm::rotate(modelMatrixNivelCero, glm::radians(90.0f), glm::vec3(0, 1, 0));

		pantalla3.render(modelMatrixNivelCero);
		//glEnable(GL_CULL_FACE);

	}
	if (nivel == 4)
	{
		glm::mat4 modelCesped = glm::mat4(1.0);
		modelCesped = glm::translate(modelCesped, glm::vec3(0.0, 0.0, 0.0));
		modelCesped = glm::scale(modelCesped, glm::vec3(200.0, 0.001, 200.0));
		// Se activa la textura del background
		glActiveTexture(GL_TEXTURE0);
		glBindTexture(GL_TEXTURE_2D, textureTerrainBackgroundID);
		shaderTerrain.setInt("backgroundTexture", 0);
		// Se activa la textura de tierra
		glActiveTexture(GL_TEXTURE1);
		glBindTexture(GL_TEXTURE_2D, textureTerrainRID);
		shaderTerrain.setInt("rTexture", 1);
		// Se activa la textura de hierba
		glActiveTexture(GL_TEXTURE2);
		glBindTexture(GL_TEXTURE_2D, textureTerrainGID);
		shaderTerrain.setInt("gTexture", 2);
		// Se activa la textura del camino
		glActiveTexture(GL_TEXTURE3);
		glBindTexture(GL_TEXTURE_2D, textureTerrainBID);
		shaderTerrain.setInt("bTexture", 3);
		// Se activa la textura del blend map
		glActiveTexture(GL_TEXTURE4);
		glBindTexture(GL_TEXTURE_2D, textureTerrainBlendMapID2);
		shaderTerrain.setInt("blendMapTexture", 4);
		shaderTerrain.setVectorFloat2("scaleUV", glm::value_ptr(glm::vec2(40, 40)));
		terrain2.render();
		shaderTerrain.setVectorFloat2("scaleUV", glm::value_ptr(glm::vec2(0, 0)));
		glBindTexture(GL_TEXTURE_2D, 0);
		glEnable(GL_CULL_FACE);
		glActiveTexture(GL_TEXTURE0);
		// Grass
		glDisable(GL_CULL_FACE);
		glm::vec3 grassPosition = glm::vec3(0.0, 0.0, 0.0);
		grassPosition.y = terrain.getHeightTerrain(grassPosition.x, grassPosition.z);
		modelGrass.setPosition(grassPosition);
		modelGrass.render();
		glEnable(GL_CULL_FACE);

		modelMatrixNaveCompleta[3][1] = terrain2.getHeightTerrain(modelMatrixNaveCompleta[3][0], modelMatrixNaveCompleta[3][2]);
		glm::mat4 modelMatrixNaveCompletaCore = glm::mat4(modelMatrixNaveCompleta);
		//modelMatrixNaveCompletaCore = glm::translate(modelMatrixNaveCompletaCore, glm::vec3(50, 0, 50));
		modelMatrixNaveCompletaCore = glm::scale(modelMatrixNaveCompletaCore, glm::vec3(1, 1, 1));
		naveCompleta.render(modelMatrixNaveCompletaCore);

		modelMatrixHongoRecio[3][1] = terrain2.getHeightTerrain(modelMatrixHongoRecio[3][0], modelMatrixHongoRecio[3][2]);
		glm::mat4 modelMatrixHongoRecioR = glm::mat4(modelMatrixHongoRecio);
		//modelMatrixNaveCompletaCore = glm::translate(modelMatrixNaveCompletaCore, glm::vec3(50, 0, 50));
		modelMatrixHongoRecioR = glm::scale(modelMatrixHongoRecioR, glm::vec3(90, 15, 90));
		HongoRecio.render(modelMatrixHongoRecioR);

		for (int i = 0; i < rocas1.size(); i++) {
			rocas1[i].y = terrain2.getHeightTerrain(rocas1[i].x, rocas1[i].z);
			Rocas1.setPosition(rocas1[i]);
			Rocas1.setScale(glm::vec3(0.3));
			Rocas1.setOrientation(glm::vec3(0, rocas1Orientation[i], 0));
			Rocas1.render();
		}
		for (int i = 0; i < rocas2.size(); i++) {
			rocas2[i].y = terrain2.getHeightTerrain(rocas2[i].x, rocas2[i].z);
			Rocas2.setPosition(rocas2[i]);
			Rocas2.setScale(glm::vec3(0.9));
			Rocas2.setOrientation(glm::vec3(0, rocas2Orientation[i], 0));
			Rocas2.render();
		}
		for (int i = 0; i < Hongos2.size(); i++) {
			Hongos2[i].y = terrain2.getHeightTerrain(Hongos2[i].x, Hongos2[i].z);
			Hongos.setPosition(Hongos2[i]);
			Hongos.setScale(glm::vec3(3.0, 3.0, 3.0));
			Hongos.setOrientation(glm::vec3(0, Hongos2Orientation[i], 0));
			Hongos.render();
		}

		std::map<float, std::pair<std::string, glm::vec3>> blendingSorted;
		std::map<std::string, glm::vec3>::iterator itblend;
		for (itblend = blendingUnsorted.begin(); itblend != blendingUnsorted.end(); itblend++) {
			float distanceFromView = glm::length(camera->getPosition() - itblend->second);
			blendingSorted[distanceFromView] = std::make_pair(itblend->first, itblend->second);
		}

		/**********
		 * Render de las transparencias
		 */
		glEnable(GL_BLEND);
		glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
		glDisable(GL_CULL_FACE);
		for (std::map<float, std::pair<std::string, glm::vec3> >::reverse_iterator it = blendingSorted.rbegin(); it != blendingSorted.rend(); it++) {
			if (renderParticles && it->second.first.compare("fire") == 0) {
				/**********
					* Init Render particles systems
					*/
				lastTimeParticlesAnimationFire = currTimeParticlesAnimationFire;
				currTimeParticlesAnimationFire = TimeManager::Instance().GetTime();

				shaderParticlesFire.setInt("Pass", 1);
				shaderParticlesFire.setFloat("Time", currTimeParticlesAnimationFire);
				shaderParticlesFire.setFloat("DeltaT", currTimeParticlesAnimationFire - lastTimeParticlesAnimationFire);

				glActiveTexture(GL_TEXTURE1);
				glBindTexture(GL_TEXTURE_1D, texId);
				glEnable(GL_RASTERIZER_DISCARD);
				glBindTransformFeedback(GL_TRANSFORM_FEEDBACK, feedback[drawBuf]);
				glBeginTransformFeedback(GL_POINTS);
				glBindVertexArray(particleArray[1 - drawBuf]);
				glVertexAttribDivisor(0, 0);
				glVertexAttribDivisor(1, 0);
				glVertexAttribDivisor(2, 0);
				glDrawArrays(GL_POINTS, 0, nParticlesFire);
				glEndTransformFeedback();
				glDisable(GL_RASTERIZER_DISCARD);


				shaderParticlesFire.setInt("Pass", 2);
				glm::mat4 modelFireParticles = modelMatrixNaveCompleta;
				modelFireParticles = glm::translate(modelFireParticles, it->second.second);
				modelFireParticles = glm::translate(modelFireParticles, glm::vec3(3.0, 0.0, -9.0));
				modelFireParticles = glm::scale(modelFireParticles, glm::vec3(0.06, 1.0, arreciado));
				modelFireParticles = glm::rotate(modelFireParticles, glm::radians(90.0f), glm::vec3(1.0, 0.0, 0.0));
				modelFireParticles[3][1] = modelMatrixNaveCompleta[3][1] + 0.8f;


				shaderParticlesFire.setMatrix4("model", 1, false, glm::value_ptr(modelFireParticles));
				shaderParticlesFire.turnOn();
				glActiveTexture(GL_TEXTURE0);
				glBindTexture(GL_TEXTURE_2D, textureParticleFireID);
				glDepthMask(GL_FALSE);
				glBindVertexArray(particleArray[drawBuf]);
				glVertexAttribDivisor(0, 1);
				glVertexAttribDivisor(1, 1);
				glVertexAttribDivisor(2, 1);
				glDrawArraysInstanced(GL_TRIANGLES, 0, 6, nParticlesFire);
				glBindVertexArray(0);
				glDepthMask(GL_TRUE);
				drawBuf = 1 - drawBuf;
				shaderParticlesFire.turnOff();

				/*shaderParticlesFire.setInt("Pass", 3);
				glm::mat4 modelFireParticles2 = modelMatrixNaveCompleta;
				modelFireParticles2 = glm::translate(modelFireParticles2, glm::vec3(-3.0, 0.0, -9.0));
				modelFireParticles2 = glm::scale(modelFireParticles2, glm::vec3(0.06, 1.0, arreciado));
				modelFireParticles2 = glm::rotate(modelFireParticles2, glm::radians(90.0f), glm::vec3(1.0, 0.0, 0.0));
				modelFireParticles2[3][1] = modelMatrixNaveCompleta[3][1] + 0.8f;


				shaderParticlesFire.setMatrix4("model", 1, false, glm::value_ptr(modelFireParticles2));
				shaderParticlesFire.turnOn();
				glActiveTexture(GL_TEXTURE0);
				glBindTexture(GL_TEXTURE_2D, textureParticleFireID);
				glDepthMask(GL_FALSE);
				glBindVertexArray(particleArray[drawBuf]);
				glVertexAttribDivisor(0, 1);
				glVertexAttribDivisor(1, 1);
				glVertexAttribDivisor(2, 1);
				glDrawArraysInstanced(GL_TRIANGLES, 0, 6, nParticlesFire);
				glBindVertexArray(0);
				glDepthMask(GL_TRUE);
				drawBuf = 1 - drawBuf;
				shaderParticlesFire.turnOff();
*/


/****************************+
	* Open AL sound data
	*/
	//source1Pos[0] = modelFireParticles[3].x;
	//source1Pos[1] = modelFireParticles[3].y;
	//source1Pos[2] = modelFireParticles[3].z;
	//alSourcefv(source[1], AL_POSITION, source1Pos);

	/**********
	* End Render particles systems
	*/
			}
		}

	}
	if (nivel == 5) {

		glm::mat4 modelMatrixNivelCero = glm::mat4(modelMatrixPantalla6);
		modelMatrixNivelCero = glm::translate(modelMatrixNivelCero, glm::vec3(0, 0.0, 0));
		modelMatrixNivelCero = glm::scale(modelMatrixNivelCero, glm::vec3(3.0, 3.5, 5.0));
		modelMatrixNivelCero = glm::rotate(modelMatrixNivelCero, glm::radians(110.0f), glm::vec3(0, 0, 1));
		modelMatrixNivelCero = glm::rotate(modelMatrixNivelCero, glm::radians(90.0f), glm::vec3(0, 1, 0));

		pantalla6.render(modelMatrixNivelCero);
		//glEnable(GL_CULL_FACE);

	}
	if (nivel == 6) {

		glm::mat4 modelMatrixNivelCero = glm::mat4(modelMatrixPantalla5);
		modelMatrixNivelCero = glm::translate(modelMatrixNivelCero, glm::vec3(0, 0.0, 0));
		modelMatrixNivelCero = glm::scale(modelMatrixNivelCero, glm::vec3(3.0, 3.5, 5.0));
		modelMatrixNivelCero = glm::rotate(modelMatrixNivelCero, glm::radians(110.0f), glm::vec3(0, 0, 1));
		modelMatrixNivelCero = glm::rotate(modelMatrixNivelCero, glm::radians(90.0f), glm::vec3(0, 1, 0));

		pantalla5.render(modelMatrixNivelCero);
		//glEnable(GL_CULL_FACE);

	}
	//glEnable(GL_CULL_FACE);
}

int main(int argc, char **argv) {
	init(800, 600, "Window GLFW", false);
	applicationLoop();
	destroy();
	return 1;
}
