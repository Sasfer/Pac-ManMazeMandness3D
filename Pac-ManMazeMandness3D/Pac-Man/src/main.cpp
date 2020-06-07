#define _USE_MATH_DEFINES
#include <cmath>
#include <map>

//glew include
#include <GL/glew.h>

//std includes
#include <string>
#include <iostream>
#include <vector>
using namespace std;

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

// Include Terrain
#include "Headers/Terrain.h"

#include "Headers/AnimationUtils.h"

// Include Colision headers functions
#include "Headers/Colisiones.h"

// ShadowBox include
#include "Headers/ShadowBox.h"

// OpenAL include
#include <AL/alut.h>

//FreeType include
#include <ft2build.h>
#include FT_FREETYPE_H


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

//Shader para FreeType
Shader textShader;

std::shared_ptr<Camera> camera(new ThirdPersonCamera());
float distanceFromTarget = 16.0;
glm::vec3 auxCameraPosition, auxCameraTarget;
float auxCamDistanceFromTarget, auxCamAngleTarget, auxCamPitch;
int vistaAerea = 0;
int auxVistaArea = 1;

Sphere skyboxSphere(20, 20);
Box boxCollider;
Sphere sphereCollider(10, 10);
Box boxViewDepth;
Box boxLightViewBox;

ShadowBox * shadowBox;

// Hierba
Model modelGrass;

// Punto
Model puntoModel;
// Para tener un control de los puntos en el laberinto se tiene 
// una asoción de un nombre y el valor del punto
std::map<std::string, int> puntoObtenido;

// Model animate instance
// Pacman
Model pacmanModelAnimate;
Model fantasmaModel;


//Model Antorcha
Model modelAntorcha1;
Model modelAntorcha2;
Model modelAntorcha3;
Model modelAntorcha4;

Model portalFantasma1Model;
Model portalFantasma2Model;
Model portalPacman1Model;
Model portalPacman2Model;

Model pisoAntorcha1Model;
Model pisoAntorcha2Model;
Model pisoAntorcha3Model;
Model pisoAntorcha4Model;

//Model Frutas
Model modelCherry;
Model modelOrange;
Model modelStrawberry;


// Laberinto
Model LE1ModelAnimate;
Model LE2ModelAnimate;
Model LE3ModelAnimate;
Model LE4ModelAnimate;
Model LE5ModelAnimate;
Model LE6ModelAnimate;
Model LE7ModelAnimate;
Model LE8ModelAnimate;
Model LE9ModelAnimate;
Model LE10ModelAnimate;
Model LE11ModelAnimate;
Model LE12ModelAnimate;
Model LE13ModelAnimate;
Model LE14ModelAnimate;
Model LE15ModelAnimate;
Model LE16ModelAnimate;
Model LE17ModelAnimate;
Model LE18ModelAnimate;
Model LE19ModelAnimate;
Model LE20ModelAnimate;
Model LE21ModelAnimate;
Model LE22ModelAnimate;
Model LE23ModelAnimate;
Model LE24ModelAnimate;
Model LE25ModelAnimate;
Model LE26ModelAnimate;
Model LE27ModelAnimate;
Model LE28ModelAnimate;
Model LE29ModelAnimate;
Model LE30ModelAnimate;
Model LE31ModelAnimate;
Model LE32ModelAnimate;
Model LE33ModelAnimate;
Model LE34ModelAnimate;
Model LE35ModelAnimate;
Model LE36ModelAnimate;
Model LE37ModelAnimate;
Model LE38ModelAnimate;
Model LE39ModelAnimate;

// Juego
int vidaPacman = 5;
int puntosPacman = 0;
int tiempoJuego = 0;

int auxGiro = 0;

// Fantasmas movimiento
int moveIzqFantasmaRojo, moveDerFantasmaRojo, verificaMoveFantasmaRojo, auxGiroFantasmaRojo = 0;
int moveIzqFantasmaRosa, moveDerFantasmaRosa, verificaMoveFantasmaRosa, auxGiroFantasmaRosa = 0;
int moveIzqFantasmaCian, moveDerFantasmaCian, verificaMoveFantasmaCian, auxGiroFantasmaCian = 0;
int moveIzqFantasmaNaranja, moveDerFantasmaNaranja, verificaMoveFantasmaNaranja, auxGiroFantasmaNaranja = 0;

// Terrain model instance
Terrain terrain(-1, -1, 200, 16, "../Textures/heightmap.png");

GLuint textureCespedID, textureWallID, textureWindowID, textureHighwayID, textureLandingPadID;
GLuint textureTerrainBackgroundID, textureTerrainRID, textureTerrainGID, textureTerrainBID, textureTerrainBlendMapID;
GLuint textureParticleFountainID, textureParticleFireID, texId;
GLuint skyboxTextureID;
GLuint textTextureID;

GLenum types[6] = {
GL_TEXTURE_CUBE_MAP_POSITIVE_X,
GL_TEXTURE_CUBE_MAP_NEGATIVE_X,
GL_TEXTURE_CUBE_MAP_POSITIVE_Y,
GL_TEXTURE_CUBE_MAP_NEGATIVE_Y,
GL_TEXTURE_CUBE_MAP_POSITIVE_Z,
GL_TEXTURE_CUBE_MAP_NEGATIVE_Z };

std::string fileNames[6] = { "../Textures/desierto/arrakisday_0_ft.tga",
		"../Textures/desierto/arrakisday_0_bk.tga",
		"../Textures/desierto/arrakisday_0_up.tga",
		"../Textures/desierto/arrakisday_0_dn.tga",
		"../Textures/desierto/arrakisday_0_rt.tga",
		"../Textures/desierto/arrakisday_0_lf.tga" };

bool exitApp = false;
int lastMousePosX, offsetX = 0;
int lastMousePosY, offsetY = 0;

// Model matrix definitions
// Pacman
glm::mat4 modelMatrixPacman = glm::mat4(1.0f);
glm::mat4 modelMatrixFantasmaRojo = glm::mat4(1.0f);
glm::mat4 modelMatrixFantasmaRosa = glm::mat4(1.0f);
glm::mat4 modelMatrixFantasmaCian = glm::mat4(1.0f);
glm::mat4 modelMatrixFantasmaNaranja = glm::mat4(1.0f);

glm::mat4 modelMatrixPortalFantasma1 = glm::mat4(1.0);
glm::mat4 modelMatrixPortalFantasma2 = glm::mat4(1.0);
glm::mat4 modelMatrixPortalPacman1 = glm::mat4(1.0);
glm::mat4 modelMatrixPortalPacman2 = glm::mat4(1.0);

//Pisos Antorchas
glm::mat4 modelMatrixPisoAntorcha1 = glm::mat4(1.0);
glm::mat4 modelMatrixPisoAntorcha2 = glm::mat4(1.0);
glm::mat4 modelMatrixPisoAntorcha3 = glm::mat4(1.0);
glm::mat4 modelMatrixPisoAntorcha4 = glm::mat4(1.0);


//Antorchas
glm::mat4 modelMatrixAntorcha1 = glm::mat4(1.0f);
glm::mat4 modelMatrixAntorcha2 = glm::mat4(1.0f);
glm::mat4 modelMatrixAntorcha3 = glm::mat4(1.0f);
glm::mat4 modelMatrixAntorcha4 = glm::mat4(1.0f);


//Frutas
glm::mat4 modelMatrixCherry = glm::mat4(1.0f);
glm::mat4 modelMatrixOrange = glm::mat4(1.0f);
glm::mat4 modelMatrixStrawberry = glm::mat4(1.0f);


// Laberinto
glm::mat4 modelMatrixLE1 = glm::mat4(1.0f);
glm::mat4 modelMatrixLE2 = glm::mat4(1.0f);
glm::mat4 modelMatrixLE3 = glm::mat4(1.0f);
glm::mat4 modelMatrixLE4 = glm::mat4(1.0f);
glm::mat4 modelMatrixLE5 = glm::mat4(1.0f);
glm::mat4 modelMatrixLE6 = glm::mat4(1.0f);
glm::mat4 modelMatrixLE7 = glm::mat4(1.0f);
glm::mat4 modelMatrixLE8 = glm::mat4(1.0f);
glm::mat4 modelMatrixLE9 = glm::mat4(1.0f);
glm::mat4 modelMatrixLE10 = glm::mat4(1.0f);
glm::mat4 modelMatrixLE11 = glm::mat4(1.0f);
glm::mat4 modelMatrixLE12 = glm::mat4(1.0f);
glm::mat4 modelMatrixLE13 = glm::mat4(1.0f);
glm::mat4 modelMatrixLE14 = glm::mat4(1.0f);
glm::mat4 modelMatrixLE15 = glm::mat4(1.0f);
glm::mat4 modelMatrixLE16 = glm::mat4(1.0f);
glm::mat4 modelMatrixLE17 = glm::mat4(1.0f);
glm::mat4 modelMatrixLE18 = glm::mat4(1.0f);
glm::mat4 modelMatrixLE19 = glm::mat4(1.0f);
glm::mat4 modelMatrixLE20 = glm::mat4(1.0f);
glm::mat4 modelMatrixLE21 = glm::mat4(1.0f);
glm::mat4 modelMatrixLE22 = glm::mat4(1.0f);
glm::mat4 modelMatrixLE23 = glm::mat4(1.0f);
glm::mat4 modelMatrixLE24 = glm::mat4(1.0f);
glm::mat4 modelMatrixLE25 = glm::mat4(1.0f);
glm::mat4 modelMatrixLE26 = glm::mat4(1.0f);
glm::mat4 modelMatrixLE27 = glm::mat4(1.0f);
glm::mat4 modelMatrixLE28 = glm::mat4(1.0f);
glm::mat4 modelMatrixLE29 = glm::mat4(1.0f);
glm::mat4 modelMatrixLE30 = glm::mat4(1.0f);
glm::mat4 modelMatrixLE31 = glm::mat4(1.0f);
glm::mat4 modelMatrixLE32 = glm::mat4(1.0f);
glm::mat4 modelMatrixLE33 = glm::mat4(1.0f);
glm::mat4 modelMatrixLE34 = glm::mat4(1.0f);
glm::mat4 modelMatrixLE35 = glm::mat4(1.0f);
glm::mat4 modelMatrixLE36 = glm::mat4(1.0f);
glm::mat4 modelMatrixLE37 = glm::mat4(1.0f);
glm::mat4 modelMatrixLE38 = glm::mat4(1.0f);
glm::mat4 modelMatrixLE39 = glm::mat4(1.0f);

int animationIndex = 1;
// Para controlar la orientación 
// del Pacman mientras se mueve
int anteriorMove = 1;
int rotarMove = 0;
int modelSelected = 0;
bool enableCountSelected = true;

// Blending model unsorted
std::map<std::string, glm::vec3> blendingUnsorted = {
	{"fantasmaRojo", glm::vec3(0.0, 0.05, 2.0)},
	{"fantasmaRosa", glm::vec3(0.0, 0.05, 0.5)},
	{"fantasmaCian", glm::vec3(0.0, 0.05, -1.0)},
	{"fantasmaNaranja", glm::vec3(0.0, 0.05, -2.5)},
	{"grass", glm::vec3(0.0, 5.0, 0.0)},
	{"antorcha",glm::vec3(-18.2f, 5.0f, 17.5f)},
	{"antorcha2",glm::vec3(17.8f, 5.0f, 17.5f)},
	{"antorcha3",glm::vec3(-18.2f, 5.0f, -17.5f)},
	{"antorcha4",glm::vec3(17.8f, 5.0f, -17.5f)}
};

double deltaTime;
double currTime, lastTime;

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
float particleSize = 0.5, particleLifetime = 3.0;
double currTimeParticlesAnimationFire, lastTimeParticlesAnimationFire;

// Para tener un control de los puntos en el laberinto
// Key: Nombre para ubicar el punto
// Value: Tupla que contiene su ubicación
//		  Valor entero 0 -> Punto no comido por Pacman	
//					   1 -> Punto comido por Pacman
std::map<std::string, std::tuple<glm::vec3, int>> puntosPosition = {
	{"punto0", {glm::vec3(-2.25, 0.0, 0.0),		0}},	{"punto1", {glm::vec3(2.25, 0.0, 0.0),		0}},
														{"punto2", {glm::vec3(0.0, 0.0, 2.25),		0}},
	{"punto3", {glm::vec3(-4.45, 0.0, 0.0),		0}},	{"punto4", {glm::vec3(4.45, 0.0, 0.0),		0}},
	{"punto5", {glm::vec3(0.0, 0.0, -4.45),		0}},	{"punto6", {glm::vec3(0.0, 0.0, 4.45),		0}},
	{"punto7", {glm::vec3(-6.75, 0.0, 0.0),		0}},	{"punto8", {glm::vec3(6.75, 0.0, 0.0),		0}},
	{"punto9", {glm::vec3(0.0, 0.0, -6.75),		0}},	{"punto10", {glm::vec3(0.0, 0.0, 6.75),		0}},
	{"punto11", {glm::vec3(-11.25, 0.0, 0.0),	0}},	{"punto12", {glm::vec3(11.25, 0.0, 0.0),	0}},
	{"punto13", {glm::vec3(0.0, 0.0, -11.25),	0}},	{"punto14", {glm::vec3(0.0, 0.0, 11.25),	0}},
	{"punto15", {glm::vec3(-15.75, 0.0, 0.0),	0}},	{"punto16", {glm::vec3(15.75, 0.0, 0.0),	0}},
	{"punto17", {glm::vec3(0.0, 0.0, -15.75),	0}},	{"punto18", {glm::vec3(0.0, 0.0, 15.75),	0}},
	{"punto19", {glm::vec3(-20.25, 0.0, 0.0),	0}},	{"punto20", {glm::vec3(20.25, 0.0, 0.0),	0}},
	{"punto21", {glm::vec3(0.0, 0.0, -20.25),	0}},	{"punto22", {glm::vec3(0.0, 0.0, 20.25),	0}},

	{"punto23", {glm::vec3(-2.25, 0.0, -2.25),	0}},	{"punto24", {glm::vec3(2.25, 0.0, -2.25),	0}},
	{"punto25", {glm::vec3(-2.25, 0.0, 2.25),	0}},	{"punto26", {glm::vec3(2.25, 0.0, 2.25),	0}},
	{"punto27", {glm::vec3(-6.75, 0.0, -2.25),	0}},	{"punto28", {glm::vec3(6.75, 0.0, -2.25),	0}},
	{"punto29", {glm::vec3(-6.75, 0.0, 2.25),	0}},	{"punto30", {glm::vec3(6.75, 0.0, 2.25),	0}},
	{"punto31", {glm::vec3(-11.25, 0.0, -2.25),	0}},	{"punto32", {glm::vec3(11.25, 0.0, -2.25),	0}},
	{"punto33", {glm::vec3(-11.25, 0.0, 2.25),	0}},	{"punto34", {glm::vec3(11.25, 0.0, 2.25),	0}},
	{"punto35", {glm::vec3(-15.75, 0.0, -2.25),	0}},	{"punto36", {glm::vec3(15.75, 0.0, -2.25),	0}},
	{"punto37", {glm::vec3(-15.75, 0.0, 2.25),	0}},	{"punto38", {glm::vec3(15.75, 0.0, 2.25),	0}},
	{"punto39", {glm::vec3(-20.25, 0.0, -2.25),	0}},	{"punto40", {glm::vec3(20.25, 0.0, -2.25),	0}},
	{"punto41", {glm::vec3(-20.25, 0.0, 2.25),	0}},	{"punto42", {glm::vec3(20.25, 0.0, 2.25),	0}},

	{"punto43", {glm::vec3(-6.75, 0.0, -4.5),	0}},	{"punto44", {glm::vec3(6.75, 0.0, -4.5),	0}},
	{"punto45", {glm::vec3(-6.75, 0.0, 4.5),	0}},	{"punto46", {glm::vec3(6.75, 0.0, 4.5),		0}},
	{"punto47", {glm::vec3(-11.25, 0.0, -4.5),	0}},	{"punto48", {glm::vec3(11.25, 0.0, -4.5),	0}},
	{"punto49", {glm::vec3(-11.25, 0.0, 4.5),	0}},	{"punto50", {glm::vec3(11.25, 0.0, 4.5),	0}},
	{"punto51", {glm::vec3(-15.75, 0.0, -4.5),	0}},	{"punto52", {glm::vec3(15.75, 0.0, -4.5),	0}},
	{"punto53", {glm::vec3(-15.75, 0.0, 4.5),	0}},	{"punto54", {glm::vec3(15.75, 0.0, 4.5),	0}},
	{"punto55", {glm::vec3(-20.25, 0.0, -4.5),	0}},	{"punto56", {glm::vec3(20.25, 0.0, -4.5),	0}},
	{"punto57", {glm::vec3(-20.25, 0.0, 4.5),	0}},	{"punto58", {glm::vec3(20.25, 0.0, 4.5),	0}},

	{"punto59", {glm::vec3(-2.25, 0.0, -6.75),	0}},	{"punto60", {glm::vec3(2.25, 0.0, -6.75),	0}},
	{"punto61", {glm::vec3(-2.25, 0.0, 6.75),	0}},	{"punto62", {glm::vec3(2.25, 0.0, 6.75),	0}},
	{"punto63", {glm::vec3(-4.5, 0.0, -6.75),	0}},	{"punto64", {glm::vec3(4.5, 0.0, -6.75),	0}},
	{"punto65", {glm::vec3(-4.5, 0.0, 6.75),	0}},	{"punto66", {glm::vec3(4.5, 0.0, 6.75),		0}},
	{"punto67", {glm::vec3(-6.75, 0.0, -6.75),	0}},	{"punto68", {glm::vec3(6.75, 0.0, -6.75),	0}},
	{"punto69", {glm::vec3(-6.75, 0.0, 6.75),	0}},	{"punto70", {glm::vec3(6.75, 0.0, 6.75),	0}},
	{"punto71", {glm::vec3(-9.0, 0.0, -6.75),	0}},	{"punto72", {glm::vec3(9.0, 0.0, -6.75),	0}},
	{"punto73", {glm::vec3(-9.0, 0.0, 6.75),	0}},	{"punto74", {glm::vec3(9.0, 0.0, 6.75),		0}},
	{"punto75", {glm::vec3(-11.25, 0.0, -6.75),	0}},	{"punto76", {glm::vec3(11.25, 0.0, -6.75),	0}},
	{"punto77", {glm::vec3(-11.25, 0.0, 6.75),	0}},	{"punto78", {glm::vec3(11.25, 0.0, 6.75),	0}},
	{"punto79", {glm::vec3(-15.75, 0.0, -6.75),	0}},	{"punto80", {glm::vec3(15.75, 0.0, -6.75),	0}},
	{"punto81", {glm::vec3(-15.75, 0.0, 6.75),	0}},	{"punto82", {glm::vec3(15.75, 0.0, 6.75),	0}},
	{"punto83", {glm::vec3(-20.25, 0.0, -6.75),	0}},	{"punto84", {glm::vec3(20.25, 0.0, -6.75),	0}},
	{"punto85", {glm::vec3(-20.25, 0.0, 6.75),	0}},	{"punto86", {glm::vec3(20.25, 0.0, 6.75),	0}},

	{"punto87", {glm::vec3(-4.5, 0.0, -9.0),	0}},	{"punto88", {glm::vec3(4.5, 0.0, -9.0),		0}},
	{"punto89", {glm::vec3(-4.5, 0.0, 9.0),		0}},	{"punto90", {glm::vec3(4.5, 0.0, 9.0),		0}},
	{"punto91", {glm::vec3(-15.75, 0.0, -9.0),	0}},	{"punto92", {glm::vec3(15.75, 0.0, -9.0),	0}},
	{"punto93", {glm::vec3(-15.75, 0.0, 9.0),	0}},	{"punto94", {glm::vec3(15.75, 0.0, 9.0),	0}},
	{"punto95", {glm::vec3(-20.25, 0.0, -9.0),	0}},	{"punto96", {glm::vec3(20.25, 0.0, -9.0),	0}},
	{"punto97", {glm::vec3(-20.25, 0.0, 9.0),	0}},	{"punto98", {glm::vec3(20.25, 0.0, 9.0),	0}},

	{"punto99", {glm::vec3(-2.25, 0.0, -11.25),		0}},	{"punto100", {glm::vec3(2.25, 0.0, -11.25),	0}},
	{"punto101", {glm::vec3(-2.25, 0.0, 11.25),		0}},	{"punto102", {glm::vec3(2.25, 0.0, 11.25),	0}},
	{"punto103", {glm::vec3(-4.5, 0.0, -11.25),		0}},	{"punto104", {glm::vec3(4.5, 0.0, -11.25),	0}},
	{"punto105", {glm::vec3(-4.5, 0.0, 11.25),		0}},	{"punto106", {glm::vec3(4.5, 0.0, 11.25),	0}},
	{"punto107", {glm::vec3(-6.75, 0.0, -11.25),	0}},	{"punto108", {glm::vec3(6.75, 0.0, -11.25),	0}},
	{"punto109", {glm::vec3(-6.75, 0.0, 11.25),		0}},	{"punto110", {glm::vec3(6.75, 0.0, 11.25),	0}},
	{"punto111", {glm::vec3(-9.0, 0.0, -11.25),		0}},	{"punto112", {glm::vec3(9.0, 0.0, -11.25),	0}},
	{"punto113", {glm::vec3(-9.0, 0.0, 11.25),		0}},	{"punto114", {glm::vec3(9.0, 0.0, 11.25),	0}},
	{"punto115", {glm::vec3(-11.25, 0.0, -11.25),	0}},	{"punto116", {glm::vec3(11.25, 0.0, -11.25),0}},
	{"punto117", {glm::vec3(-11.25, 0.0, 11.25),	0}},	{"punto118", {glm::vec3(11.25, 0.0, 11.25),	0}},
	{"punto119", {glm::vec3(-13.5, 0.0, -11.25),	0}},	{"punto120", {glm::vec3(13.5, 0.0, -11.25),	0}},
	{"punto121", {glm::vec3(-13.5, 0.0, 11.25),		0}},	{"punto122", {glm::vec3(13.5, 0.0, 11.25),	0}},
	{"punto123", {glm::vec3(-15.75, 0.0, -11.25),	0}},	{"punto124", {glm::vec3(15.75, 0.0, -11.25),0}},
	{"punto125", {glm::vec3(-15.75, 0.0, 11.25),	0}},	{"punto126", {glm::vec3(15.75, 0.0, 11.25),	0}},
	{"punto127", {glm::vec3(-18.0, 0.0, -11.25),	0}},	{"punto128", {glm::vec3(18.0, 0.0, -11.25),	0}},
	{"punto129", {glm::vec3(-18.0, 0.0, 11.25),		0}},	{"punto130", {glm::vec3(18.0, 0.0, 11.25),	0}},
	{"punto131", {glm::vec3(-20.25, 0.0, -11.25),	0}},	{"punto132", {glm::vec3(20.25, 0.0, -11.25),0}},
	{"punto133", {glm::vec3(-20.25, 0.0, 11.25),	0}},	{"punto134", {glm::vec3(20.25, 0.0, 11.25),	0}},

	{"punto135", {glm::vec3(-15.75, 0.0, -13.5),	0}},	{"punto136", {glm::vec3(15.75, 0.0, -13.5),	0}},
	{"punto137", {glm::vec3(-15.75, 0.0, 13.5),		0}},	{"punto138", {glm::vec3(15.75, 0.0, 13.5),	0}},
	{"punto139", {glm::vec3(-20.25, 0.0, -13.5),	0}},	{"punto140", {glm::vec3(20.25, 0.0, -13.5),	0}},
	{"punto141", {glm::vec3(-20.25, 0.0, 13.5),		0}},	{"punto142", {glm::vec3(20.25, 0.0, 13.5),	0}},

	{"punto143", {glm::vec3(-2.25, 0.0, -15.75),	0}},	{"punto144", {glm::vec3(2.25, 0.0, -15.75),	0}},
	{"punto145", {glm::vec3(-2.25, 0.0, 15.75),		0}},	{"punto146", {glm::vec3(2.25, 0.0, 15.75),	0}},
	{"punto147", {glm::vec3(-4.5, 0.0, -15.75),		0}},	{"punto148", {glm::vec3(4.5, 0.0, -15.75),	0}},
	{"punto149", {glm::vec3(-4.5, 0.0, 15.75),		0}},	{"punto150", {glm::vec3(4.5, 0.0, 15.75),	0}},
	{"punto151", {glm::vec3(-6.75, 0.0, -15.75),	0}},	{"punto152", {glm::vec3(6.75, 0.0, -15.75),	0}},
	{"punto153", {glm::vec3(-6.75, 0.0, 15.75),		0}},	{"punto154", {glm::vec3(6.75, 0.0, 15.75),	0}},
	{"punto155", {glm::vec3(-9.0, 0.0, -15.75),		0}},	{"punto156", {glm::vec3(9.0, 0.0, -15.75),	0}},
	{"punto157", {glm::vec3(-9.0, 0.0, 15.75),		0}},	{"punto158", {glm::vec3(9.0, 0.0, 15.75),	0}},
	{"punto159", {glm::vec3(-11.25, 0.0, -15.75),	0}},	{"punto160", {glm::vec3(11.25, 0.0, -15.75),0}},
	{"punto161", {glm::vec3(-11.25, 0.0, 15.75),	0}},	{"punto162", {glm::vec3(11.25, 0.0, 15.75),	0}},
	{"punto163", {glm::vec3(-13.5, 0.0, -15.75),	0}},	{"punto164", {glm::vec3(13.5, 0.0, -15.75),	0}},
	{"punto165", {glm::vec3(-13.5, 0.0, 15.75),		0}},	{"punto166", {glm::vec3(13.5, 0.0, 15.75),	0}},
	{"punto167", {glm::vec3(-15.75, 0.0, -15.75),	0}},	{"punto168", {glm::vec3(15.75, 0.0, -15.75),0}},
	{"punto169", {glm::vec3(-15.75, 0.0, 15.75),	0}},	{"punto170", {glm::vec3(15.75, 0.0, 15.75),	0}},
	{"punto171", {glm::vec3(-20.25, 0.0, -15.75),	0}},	{"punto172", {glm::vec3(20.25, 0.0, -15.75),0}},
	{"punto173", {glm::vec3(-20.25, 0.0, 15.75),	0}},	{"punto174", {glm::vec3(20.25, 0.0, 15.75),	0}},

	{"punto175", {glm::vec3(-11.25, 0.0, -18.0),	0}},	{"punto176", {glm::vec3(11.25, 0.0, -18.0),	0}},
	{"punto177", {glm::vec3(-11.25, 0.0, 18.0),		0}},	{"punto178", {glm::vec3(11.25, 0.0, 18.0),	0}},
	{"punto179", {glm::vec3(-20.25, 0.0, -18.0),	0}},	{"punto180", {glm::vec3(20.25, 0.0, -18.0),	0}},
	{"punto181", {glm::vec3(-20.25, 0.0, 18.0),		0}},	{"punto182", {glm::vec3(20.25, 0.0, 18.0),	0}},

	{"punto183", {glm::vec3(-2.25, 0.0, -20.25),	0}},	{"punto184", {glm::vec3(2.25, 0.0, -20.25),	0}},
	{"punto185", {glm::vec3(-2.25, 0.0, 20.25),		0}},	{"punto186", {glm::vec3(2.25, 0.0, 20.25),	0}},
	{"punto187", {glm::vec3(-4.5, 0.0, -20.25),		0}},	{"punto188", {glm::vec3(4.5, 0.0, -20.25),	0}},
	{"punto189", {glm::vec3(-4.5, 0.0, 20.25),		0}},	{"punto190", {glm::vec3(4.5, 0.0, 20.25),	0}},
	{"punto191", {glm::vec3(-6.75, 0.0, -20.25),	0}},	{"punto192", {glm::vec3(6.75, 0.0, -20.25),	0}},
	{"punto193", {glm::vec3(-6.75, 0.0, 20.25),		0}},	{"punto194", {glm::vec3(6.75, 0.0, 20.25),	0}},
	{"punto195", {glm::vec3(-9.0, 0.0, -20.25),		0}},	{"punto196", {glm::vec3(9.0, 0.0, -20.25),	0}},
	{"punto197", {glm::vec3(-9.0, 0.0, 20.25),		0}},	{"punto198", {glm::vec3(9.0, 0.0, 20.25),	0}},
	{"punto199", {glm::vec3(-11.25, 0.0, -20.25),	0}},	{"punto200", {glm::vec3(11.25, 0.0, -20.25),0}},
	{"punto201", {glm::vec3(-11.25, 0.0, 20.25),	0}},	{"punto202", {glm::vec3(11.25, 0.0, 20.25),0}},
	{"punto203", {glm::vec3(-13.5, 0.0, -20.25),	0}},	{"punto204", {glm::vec3(13.5, 0.0, -20.25),	0}},
	{"punto205", {glm::vec3(-13.5, 0.0, 20.25),		0}},	{"punto206", {glm::vec3(13.5, 0.0, 20.25),	0}},
	{"punto207", {glm::vec3(-15.75, 0.0, -20.25),	0}},	{"punto208", {glm::vec3(15.75, 0.0, -20.25),0}},
	{"punto209", {glm::vec3(-15.75, 0.0, 20.25),	0}},	{"punto210", {glm::vec3(15.75, 0.0, 20.25),0}},
	{"punto211", {glm::vec3(-18.0, 0.0, -20.25),	0}},	{"punto212", {glm::vec3(18.0, 0.0, -20.25),	0}},
	{"punto213", {glm::vec3(-18.0, 0.0, 20.25),		0}},	{"punto214", {glm::vec3(18.0, 0.0, 20.25),	0}},
	{"punto215", {glm::vec3(-20.25, 0.0, -20.25),	0}},	{"punto216", {glm::vec3(20.25, 0.0, -20.25),0}},
	{"punto217", {glm::vec3(-20.25, 0.0, 20.25),	0}},	{"punto218", {glm::vec3(20.25, 0.0, 20.25),0}},

};

// Colliders
std::map<std::string, std::tuple<AbstractModel::OBB, glm::mat4, glm::mat4> > collidersOBB;
std::map<std::string, std::tuple<AbstractModel::SBB, glm::mat4, glm::mat4> > collidersSBB;

// Framesbuffers
GLuint depthMap, depthMapFBO;

/**********************
 * OpenAL config
 */

// OpenAL Defines
#define NUM_BUFFERS 3
#define NUM_SOURCES 3
#define NUM_ENVIRONMENTS 1
// Listener
ALfloat listenerPos[] = { 0.0, 0.0, 4.0 };
ALfloat listenerVel[] = { 0.0, 0.0, 0.0 };
ALfloat listenerOri[] = { 0.0, 0.0, 1.0, 0.0, 1.0, 0.0 };
// Source 0
ALfloat source0Pos[] = { -2.0, 0.0, 0.0 };
ALfloat source0Vel[] = { 0.0, 0.0, 0.0 };
// Source 1
ALfloat source1Pos[] = { 2.0, 0.0, 0.0 };
ALfloat source1Vel[] = { 0.0, 0.0, 0.0 };
// Source 2
ALfloat source2Pos[] = { 2.0, 0.0, 0.0 };
ALfloat source2Vel[] = { 0.0, 0.0, 0.0 };
// Buffers
ALuint buffer[NUM_BUFFERS];
ALuint source[NUM_SOURCES];
ALuint environment[NUM_ENVIRONMENTS];
// Configs
ALsizei size, freq;
ALenum format;
ALvoid *data;
int ch;
ALboolean loop;
std::vector<bool> sourcesPlay = {true, true, true};

/**********************
 *  FreeType config   *
 **********************/
FT_Library ft;

struct Character {
	unsigned int TextureID;  // ID handle of the glyph texture
	glm::ivec2   Size;       // Size of glyph
	glm::ivec2   Bearing;    // Offset from baseline to left/top of glyph
	unsigned int Advance;    // Offset to advance to next glyph
};

glm::mat4 textProjection;
std::map<GLchar, Character> Characters;
unsigned int textVAO, textVBO;


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
void applicationLoop();
void prepareScene();
void prepareDepthScene();
void renderScene(bool renderParticles = true);
//FreeType
void RenderText(Shader& shader, std::string text, float x, float y, float scale, glm::vec3 color);

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
	for(unsigned int i = 0; i < nParticlesFire; i++ ) {
		int index = i - nParticlesFire;
		float result = rate * index;
		initialAge[i] = result;
	}
	// Shuffle them for better looking results
	//Random::shuffle(initialAge);
	auto rng = std::default_random_engine {};
	std::shuffle(initialAge.begin(), initialAge.end(), rng);
	glBindBuffer(GL_ARRAY_BUFFER, age[0]);
	glBufferSubData(GL_ARRAY_BUFFER, 0, size, initialAge.data());

	glBindBuffer(GL_ARRAY_BUFFER,0);

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
	glEnable(GL_CULL_FACE);

	// Inicialización de los shaders
	shader.initialize("../Shaders/colorShader.vs", "../Shaders/colorShader.fs");
	shaderSkybox.initialize("../Shaders/skyBox.vs", "../Shaders/skyBox_fog.fs");
	shaderMulLighting.initialize("../Shaders/iluminacion_textura_animation_shadow.vs", "../Shaders/multipleLights_shadow.fs");
	shaderTerrain.initialize("../Shaders/terrain_shadow.vs", "../Shaders/terrain_shadow.fs");
	shaderParticlesFountain.initialize("../Shaders/particlesFountain.vs", "../Shaders/particlesFountain.fs");
	shaderParticlesFire.initialize("../Shaders/particlesFire.vs", "../Shaders/particlesFire.fs", {"Position", "Velocity", "Age"});
	shaderViewDepth.initialize("../Shaders/texturizado.vs", "../Shaders/texturizado_depth_view.fs");
	shaderDepth.initialize("../Shaders/shadow_mapping_depth.vs", "../Shaders/shadow_mapping_depth.fs");

	//FreeType shaders
	textShader.initialize("../Shaders/text.vs", "../Shaders/text.fs");

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

	terrain.init();
	terrain.setShader(&shaderTerrain);
	terrain.setPosition(glm::vec3(100, 0, 100));

	//Grass
	modelGrass.loadModel("../models/grass/grassModel.obj");
	modelGrass.setShader(&shaderMulLighting);

	//Pacman
	// Animaciones 0 -> Dying	1 -> Hang Raising	2 -> Idle	3 -> Walking
	pacmanModelAnimate.loadModel("../models/Pacman/MS-PACMAN_ANIMACIONES.fbx");
	pacmanModelAnimate.setShader(&shaderMulLighting);

	//Fantasmas
	fantasmaModel.loadModel("../models/Fantasma/fantasma.obj");
	fantasmaModel.setShader(&shaderMulLighting);

	//Antorcha1
	modelAntorcha1.loadModel("../models/Antorcha/antorcha.obj");
	modelAntorcha1.setShader(&shaderMulLighting);
	//Antorcha2
	modelAntorcha2.loadModel("../models/Antorcha/antorcha.obj");
	modelAntorcha2.setShader(&shaderMulLighting);
	//Antorcha3
	modelAntorcha3.loadModel("../models/Antorcha/antorcha.obj");
	modelAntorcha3.setShader(&shaderMulLighting);
	//Antorcha 4
	modelAntorcha4.loadModel("../models/Antorcha/antorcha.obj");
	modelAntorcha4.setShader(&shaderMulLighting);

	//Frutas
	modelCherry.loadModel("../models/Frutas/cherryO.obj");
	modelCherry.setShader(&shaderMulLighting);


	modelOrange.loadModel("../models/Frutas/orangeO.obj");
	modelOrange.setShader(&shaderMulLighting);

	modelStrawberry.loadModel("../models/Frutas/strawberry.obj");
	modelStrawberry.setShader(&shaderMulLighting);


	// Punto
	puntoModel.loadModel("../models/Pacman/punto.obj");
	puntoModel.setShader(&shaderMulLighting);

	// Portales
	portalFantasma1Model.loadModel("../models/Cajas/portalFantasma1.obj");
	portalFantasma1Model.setShader(&shaderMulLighting);

	portalFantasma2Model.loadModel("../models/Cajas/portalFantasma2.obj");
	portalFantasma2Model.setShader(&shaderMulLighting);

	portalPacman1Model.loadModel("../models/Cajas/portalPacman1.obj");
	portalPacman1Model.setShader(&shaderMulLighting);

	portalPacman2Model.loadModel("../models/Cajas/portalPacman2.obj");
	portalPacman2Model.setShader(&shaderMulLighting);

	// Piso antorchas -> bases
	pisoAntorcha1Model.loadModel("../models/Cajas/pisoAntorcha1.obj");
	pisoAntorcha1Model.setShader(&shaderMulLighting);

	pisoAntorcha2Model.loadModel("../models/Cajas/pisoAntorcha2.obj");
	pisoAntorcha2Model.setShader(&shaderMulLighting);

	pisoAntorcha3Model.loadModel("../models/Cajas/pisoAntorcha3.obj");
	pisoAntorcha3Model.setShader(&shaderMulLighting);

	pisoAntorcha4Model.loadModel("../models/Cajas/pisoAntorcha4.obj");
	pisoAntorcha4Model.setShader(&shaderMulLighting);

	//Laberinto
	LE1ModelAnimate.loadModel("../models/LaberintoEgipto/LE1.obj");
	LE1ModelAnimate.setShader(&shaderMulLighting);
	LE2ModelAnimate.loadModel("../models/LaberintoEgipto/LE2.obj");
	LE2ModelAnimate.setShader(&shaderMulLighting);
	LE3ModelAnimate.loadModel("../models/LaberintoEgipto/LE3.obj");
	LE3ModelAnimate.setShader(&shaderMulLighting);
	LE4ModelAnimate.loadModel("../models/LaberintoEgipto/LE4.obj");
	LE4ModelAnimate.setShader(&shaderMulLighting);
	LE5ModelAnimate.loadModel("../models/LaberintoEgipto/LE5.obj");
	LE5ModelAnimate.setShader(&shaderMulLighting);
	LE6ModelAnimate.loadModel("../models/LaberintoEgipto/LE6.obj");
	LE6ModelAnimate.setShader(&shaderMulLighting);
	LE7ModelAnimate.loadModel("../models/LaberintoEgipto/LE7.obj");
	LE7ModelAnimate.setShader(&shaderMulLighting);
	LE8ModelAnimate.loadModel("../models/LaberintoEgipto/LE8.obj");
	LE8ModelAnimate.setShader(&shaderMulLighting);
	LE9ModelAnimate.loadModel("../models/LaberintoEgipto/LE9.obj");
	LE9ModelAnimate.setShader(&shaderMulLighting);
	LE10ModelAnimate.loadModel("../models/LaberintoEgipto/LE10.obj");
	LE10ModelAnimate.setShader(&shaderMulLighting);
	LE11ModelAnimate.loadModel("../models/LaberintoEgipto/LE11.obj");
	LE11ModelAnimate.setShader(&shaderMulLighting);
	LE12ModelAnimate.loadModel("../models/LaberintoEgipto/LE12.obj");
	LE12ModelAnimate.setShader(&shaderMulLighting);
	LE13ModelAnimate.loadModel("../models/LaberintoEgipto/LE13.obj");
	LE13ModelAnimate.setShader(&shaderMulLighting);
	LE14ModelAnimate.loadModel("../models/LaberintoEgipto/LE14.obj");
	LE14ModelAnimate.setShader(&shaderMulLighting);
	LE15ModelAnimate.loadModel("../models/LaberintoEgipto/LE15.obj");
	LE15ModelAnimate.setShader(&shaderMulLighting);
	LE16ModelAnimate.loadModel("../models/LaberintoEgipto/LE16.obj");
	LE16ModelAnimate.setShader(&shaderMulLighting);
	LE17ModelAnimate.loadModel("../models/LaberintoEgipto/LE17.obj");
	LE17ModelAnimate.setShader(&shaderMulLighting);
	LE18ModelAnimate.loadModel("../models/LaberintoEgipto/LE18.obj");
	LE18ModelAnimate.setShader(&shaderMulLighting);
	LE19ModelAnimate.loadModel("../models/LaberintoEgipto/LE19.obj");
	LE19ModelAnimate.setShader(&shaderMulLighting);
	LE20ModelAnimate.loadModel("../models/LaberintoEgipto/LE20.obj");
	LE20ModelAnimate.setShader(&shaderMulLighting);
	LE21ModelAnimate.loadModel("../models/LaberintoEgipto/LE21.obj");
	LE21ModelAnimate.setShader(&shaderMulLighting);
	LE22ModelAnimate.loadModel("../models/LaberintoEgipto/LE22.obj");
	LE22ModelAnimate.setShader(&shaderMulLighting);
	LE23ModelAnimate.loadModel("../models/LaberintoEgipto/LE23.obj");
	LE23ModelAnimate.setShader(&shaderMulLighting);
	LE24ModelAnimate.loadModel("../models/LaberintoEgipto/LE24.obj");
	LE24ModelAnimate.setShader(&shaderMulLighting);
	LE25ModelAnimate.loadModel("../models/LaberintoEgipto/LE25.obj");
	LE25ModelAnimate.setShader(&shaderMulLighting);
	LE26ModelAnimate.loadModel("../models/LaberintoEgipto/LE26.obj");
	LE26ModelAnimate.setShader(&shaderMulLighting);
	LE27ModelAnimate.loadModel("../models/LaberintoEgipto/LE27.obj");
	LE27ModelAnimate.setShader(&shaderMulLighting);
	LE28ModelAnimate.loadModel("../models/LaberintoEgipto/LE28.obj");
	LE28ModelAnimate.setShader(&shaderMulLighting);
	LE29ModelAnimate.loadModel("../models/LaberintoEgipto/LE29.obj");
	LE29ModelAnimate.setShader(&shaderMulLighting);
	LE30ModelAnimate.loadModel("../models/LaberintoEgipto/LE30.obj");
	LE30ModelAnimate.setShader(&shaderMulLighting);
	LE31ModelAnimate.loadModel("../models/LaberintoEgipto/LE31.obj");
	LE31ModelAnimate.setShader(&shaderMulLighting);
	LE32ModelAnimate.loadModel("../models/LaberintoEgipto/LE32.obj");
	LE32ModelAnimate.setShader(&shaderMulLighting);
	LE33ModelAnimate.loadModel("../models/LaberintoEgipto/LE33.obj");
	LE33ModelAnimate.setShader(&shaderMulLighting);
	LE34ModelAnimate.loadModel("../models/LaberintoEgipto/LE34.obj");
	LE34ModelAnimate.setShader(&shaderMulLighting);
	LE35ModelAnimate.loadModel("../models/LaberintoEgipto/LE35.obj");
	LE35ModelAnimate.setShader(&shaderMulLighting);
	LE36ModelAnimate.loadModel("../models/LaberintoEgipto/LE36.obj");
	LE36ModelAnimate.setShader(&shaderMulLighting);
	LE37ModelAnimate.loadModel("../models/LaberintoEgipto/LE37.obj");
	LE37ModelAnimate.setShader(&shaderMulLighting);
	LE38ModelAnimate.loadModel("../models/LaberintoEgipto/LE38.obj");
	LE38ModelAnimate.setShader(&shaderMulLighting);
	LE39ModelAnimate.loadModel("../models/LaberintoEgipto/LE39.obj");
	LE39ModelAnimate.setShader(&shaderMulLighting);
	
	camera->setPosition(glm::vec3(0.0, 0.0, 10.0));
	camera->setDistanceFromTarget(distanceFromTarget);
	camera->setSensitivity(1.0);

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
		} else
			std::cout << "Failed to load texture" << std::endl;
		skyboxTexture.freeImage(bitmap);
	}

	// Definiendo la textura a utilizar -> COLOR NEGRO
	Texture textureTerrainBackground("../Textures/arena.jpg");
	// Carga el mapa de bits (FIBITMAP es el tipo de dato de la libreria)
	bitmap = textureTerrainBackground.loadImage();
	// Convertimos el mapa de bits en un arreglo unidimensional de tipo unsigned char
	data = textureTerrainBackground.convertToData(bitmap, imageWidth, imageHeight);
	// Creando la textura con id 1
	glGenTextures(1, &textureTerrainBackgroundID);
	// Enlazar esa textura a una tipo de textura de 2D.
	glBindTexture(GL_TEXTURE_2D, textureTerrainBackgroundID);
	// Set the texture wrapping parameters
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT); // set texture wrapping to GL_REPEAT (default wrapping method)
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	// Set texture filtering parameters
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	// Verifica si se pudo abrir la textura
	if (data) {
		// Transferis los datos de la imagen a memoria
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, imageWidth, imageHeight, 0, GL_BGRA, GL_UNSIGNED_BYTE, data);
		// Generan los niveles del mipmap (OpenGL es el ecargado de realizarlos)
		glGenerateMipmap(GL_TEXTURE_2D);
	} else
		std::cout << "Failed to load texture" << std::endl;
	// Libera la memoria de la textura
	textureTerrainBackground.freeImage(bitmap);

	// Definiendo la textura a utilizar -> COLOR ROJO
	Texture textureTerrainR("../Textures/ladrilloCafe2.jpg");
	// Carga el mapa de bits (FIBITMAP es el tipo de dato de la libreria)
	bitmap = textureTerrainR.loadImage();
	// Convertimos el mapa de bits en un arreglo unidimensional de tipo unsigned char
	data = textureTerrainR.convertToData(bitmap, imageWidth, imageHeight);
	// Creando la textura con id 1
	glGenTextures(1, &textureTerrainRID);
	// Enlazar esa textura a una tipo de textura de 2D.
	glBindTexture(GL_TEXTURE_2D, textureTerrainRID);
	// Set the texture wrapping parameters
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT); 
	// Set texture wrapping to GL_REPEAT (default wrapping method)
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	// Set texture filtering parameters
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	// Verifica si se pudo abrir la textura
	if (data) {
		// Transferis los datos de la imagen a memoria
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, imageWidth, imageHeight, 0, GL_BGRA, GL_UNSIGNED_BYTE, data);
		// Generan los niveles del mipmap (OpenGL es el ecargado de realizarlos)
		glGenerateMipmap(GL_TEXTURE_2D);
	} else
		std::cout << "Failed to load texture" << std::endl;
	// Libera la memoria de la textura
	textureTerrainR.freeImage(bitmap);

	// Definiendo la textura a utilizar -> COLOR VERDE
	Texture textureTerrainG("../Textures/pastoAmarillo.jpg");
	// Carga el mapa de bits (FIBITMAP es el tipo de dato de la libreria)
	bitmap = textureTerrainG.loadImage();
	// Convertimos el mapa de bits en un arreglo unidimensional de tipo unsigned char
	data = textureTerrainG.convertToData(bitmap, imageWidth, imageHeight);
	// Creando la textura con id 1
	glGenTextures(1, &textureTerrainGID);
	// Enlazar esa textura a una tipo de textura de 2D.
	glBindTexture(GL_TEXTURE_2D, textureTerrainGID);
	// Set the texture wrapping parameters
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT); 
	// Set texture wrapping to GL_REPEAT (default wrapping method)
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	// Set texture filtering parameters
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	// Verifica si se pudo abrir la textura
	if (data) {
		// Transferis los datos de la imagen a memoria
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, imageWidth, imageHeight, 0,
		GL_BGRA, GL_UNSIGNED_BYTE, data);
		// Generan los niveles del mipmap (OpenGL es el ecargado de realizarlos)
		glGenerateMipmap(GL_TEXTURE_2D);
	} else
		std::cout << "Failed to load texture" << std::endl;
	// Libera la memoria de la textura
	textureTerrainG.freeImage(bitmap);

	// Definiendo la textura a utilizar -> COLOR AZUL
	Texture textureTerrainB("../Textures/water.jpg");
	// Carga el mapa de bits (FIBITMAP es el tipo de dato de la libreria)
	bitmap = textureTerrainB.loadImage();
	// Convertimos el mapa de bits en un arreglo unidimensional de tipo unsigned char
	data = textureTerrainB.convertToData(bitmap, imageWidth, imageHeight);
	// Creando la textura con id 1
	glGenTextures(1, &textureTerrainBID);
	// Enlazar esa textura a una tipo de textura de 2D.
	glBindTexture(GL_TEXTURE_2D, textureTerrainBID);
	// Set the texture wrapping parameters
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT); 
	// Set texture wrapping to GL_REPEAT (default wrapping method)
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	// Set texture filtering parameters
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	// Verifica si se pudo abrir la textura
	if (data) {
		// Transferis los datos de la imagen a memoria
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, imageWidth, imageHeight, 0,
		GL_BGRA, GL_UNSIGNED_BYTE, data);
		// Generan los niveles del mipmap (OpenGL es el ecargado de realizarlos)
		glGenerateMipmap(GL_TEXTURE_2D);
	} else
		std::cout << "Failed to load texture" << std::endl;
	// Libera la memoria de la textura
	textureTerrainB.freeImage(bitmap);

	// Definiendo la textura a utilizar -> MAPA DE ALTURAS
	Texture textureTerrainBlendMap("../Textures/blendMap.png");
	// Carga el mapa de bits (FIBITMAP es el tipo de dato de la libreria)
	bitmap = textureTerrainBlendMap.loadImage(true);
	// Convertimos el mapa de bits en un arreglo unidimensional de tipo unsigned char
	data = textureTerrainBlendMap.convertToData(bitmap, imageWidth, imageHeight);
	// Creando la textura con id 1
	glGenTextures(1, &textureTerrainBlendMapID);
	// Enlazar esa textura a una tipo de textura de 2D.
	glBindTexture(GL_TEXTURE_2D, textureTerrainBlendMapID);
	// Set the texture wrapping parameters
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT); 
	// Set texture wrapping to GL_REPEAT (default wrapping method)
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	// Set texture filtering parameters
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	// Verifica si se pudo abrir la textura
	if (data) {
		// Transferis los datos de la imagen a memoria
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, imageWidth, imageHeight, 0, GL_BGRA, GL_UNSIGNED_BYTE, data);
		// Generan los niveles del mipmap (OpenGL es el ecargado de realizarlos)
		glGenerateMipmap(GL_TEXTURE_2D);
	} else
		std::cout << "Failed to load texture" << std::endl;
	// Libera la memoria de la textura
	textureTerrainBlendMap.freeImage(bitmap);

	Texture textureParticlesFountain("../Textures/bluewater.png");
	bitmap = textureParticlesFountain.loadImage();
	data = textureParticlesFountain.convertToData(bitmap, imageWidth, imageHeight);
	glGenTextures(1, &textureParticleFountainID);
	glBindTexture(GL_TEXTURE_2D, textureParticleFountainID);
	// set the texture wrapping parameters
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);// set texture wrapping to GL_REPEAT (default wrapping method)
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	// set texture filtering parameters
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	if (data) {
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, imageWidth, imageHeight, 0, GL_BGRA, GL_UNSIGNED_BYTE, data);
		glGenerateMipmap(GL_TEXTURE_2D);
	}
	else
		std::cout << "Failed to load texture" << std::endl;
	textureParticlesFountain.freeImage(bitmap);

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
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, imageWidth, imageHeight, 0, GL_BGRA, GL_UNSIGNED_BYTE, data);
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
	for( int i = 0; i < randData.size(); i++ ) {
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
	shaderParticlesFire.setVectorFloat3("Accel", glm::value_ptr(glm::vec3(0.0f,0.1f,0.0f)));
	shaderParticlesFire.setVectorFloat3("Emitter", glm::value_ptr(glm::vec3(0.0f)));

	glm::mat3 basis;
	glm::vec3 u, v, n;
	v = glm::vec3(0,1,0);
	n = glm::cross(glm::vec3(1,0,0), v);
	if( glm::length(n) < 0.00001f ) {
		n = glm::cross(glm::vec3(0,1,0), v);
	}
	u = glm::cross(v,n);
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
	glTexImage2D(GL_TEXTURE_2D, 0, GL_DEPTH_COMPONENT, SHADOW_WIDTH, SHADOW_HEIGHT, 0, GL_DEPTH_COMPONENT, GL_FLOAT, NULL);
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
	buffer[0] = alutCreateBufferFromFile("../sounds/fountain.wav");
	buffer[1] = alutCreateBufferFromFile("../sounds/fire.wav");
	buffer[2] = alutCreateBufferFromFile("../sounds/darth_vader.wav");
	int errorAlut = alutGetError();
	if (errorAlut != ALUT_ERROR_NO_ERROR){
		printf("- Error open files with alut %d !!\n", errorAlut);
		exit(2);
	}

	alGetError(); /* clear error */
	alGenSources(NUM_SOURCES, source);

	if (alGetError() != AL_NO_ERROR) {
		printf("- Error creating sources !!\n");
		exit(2);
	}
	else {
		printf("init - no errors after alGenSources\n");
	}

	alSourcef(source[0], AL_PITCH, 1.0f);
	alSourcef(source[0], AL_GAIN, 3.0f);
	alSourcefv(source[0], AL_POSITION, source0Pos);
	alSourcefv(source[0], AL_VELOCITY, source0Vel);
	alSourcei(source[0], AL_BUFFER, buffer[0]);
	alSourcei(source[0], AL_LOOPING, AL_TRUE);
	alSourcef(source[0], AL_MAX_DISTANCE, 2000);

	alSourcef(source[1], AL_PITCH, 1.0f);
	alSourcef(source[1], AL_GAIN, 3.0f);
	alSourcefv(source[1], AL_POSITION, source1Pos);
	alSourcefv(source[1], AL_VELOCITY, source1Vel);
	alSourcei(source[1], AL_BUFFER, buffer[1]);
	alSourcei(source[1], AL_LOOPING, AL_TRUE);
	alSourcef(source[1], AL_MAX_DISTANCE, 2000);

	alSourcef(source[2], AL_PITCH, 1.0f);
	alSourcef(source[2], AL_GAIN, 0.3f);
	alSourcefv(source[2], AL_POSITION, source2Pos);
	alSourcefv(source[2], AL_VELOCITY, source2Vel);
	alSourcei(source[2], AL_BUFFER, buffer[2]);
	alSourcei(source[2], AL_LOOPING, AL_TRUE);
	alSourcef(source[2], AL_MAX_DISTANCE, 500);

	/*******************************************
	 * FreeType init
	 *******************************************/
	//glUniformMatrix4fv(textShader.getUniformLocation("textProjection"), 1, GL_FALSE, glm::value_ptr(textProjection));
	textProjection = glm::ortho(0.0f, static_cast<float>(screenWidth), 0.0f, static_cast<float>(screenHeight));
	textShader.setMatrix4("textProjection", 1, false, glm::value_ptr(textProjection));

	if (FT_Init_FreeType(&ft))
		std::cout << "ERROR::FREETYPE: Could not init FreeType Library" << std::endl;

	// load font as face
	FT_Face face;
	if (FT_New_Face(ft, "../fonts/ConcertOne-Regular.ttf", 0, &face))
		std::cout << "ERROR::FREETYPE: Failed to load font" << std::endl;

	// set size to load glyphs as
	FT_Set_Pixel_Sizes(face, 0, 48);
	
	glPixelStorei(GL_UNPACK_ALIGNMENT, 1);
	for (unsigned char c = 0; c < 128; c++)
	{
		// Load character glyph 
		if (FT_Load_Char(face, c, FT_LOAD_RENDER))
		{
			std::cout << "ERROR::FREETYTPE: Failed to load Glyph" << std::endl;
			continue;
		}
		// generate texture
		unsigned int texture;
		glGenTextures(1, &texture);
		glBindTexture(GL_TEXTURE_2D, texture);
		glTexImage2D(
			GL_TEXTURE_2D,
			0,
			GL_RED,
			face->glyph->bitmap.width,
			face->glyph->bitmap.rows,
			0,
			GL_RED,
			GL_UNSIGNED_BYTE,
			face->glyph->bitmap.buffer
		);
		// set texture options
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
		// now store character for later use
		Character character = {
			texture,
			glm::ivec2(face->glyph->bitmap.width, face->glyph->bitmap.rows),
			glm::ivec2(face->glyph->bitmap_left, face->glyph->bitmap_top),
			face->glyph->advance.x
		};
		Characters.insert(std::pair<char, Character>(c, character));
	}

	glBindTexture(GL_TEXTURE_2D, textTextureID);
	// destroy FreeType once we're finished
	FT_Done_Face(face);
	FT_Done_FreeType(ft);


	// configure VAO/VBO for texture quads
	// -----------------------------------
	glGenVertexArrays(1, &textVAO);
	glGenBuffers(1, &textVBO);
	glBindVertexArray(textVAO);
	glBindBuffer(GL_ARRAY_BUFFER, textVBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(float) * 6 * 4, NULL, GL_DYNAMIC_DRAW);
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0, 4, GL_FLOAT, GL_FALSE, 4 * sizeof(float), 0);
	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindVertexArray(0);

}



void destroy() {
	glfwDestroyWindow(window);
	glfwTerminate();
	// --------- IMPORTANTE ----------
	// Eliminar los shader y buffers creados.

	// Shaders Delete
	shader.destroy();
	shaderMulLighting.destroy();
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
	modelGrass.destroy();
	puntoModel.destroy();
	fantasmaModel.destroy();
	portalFantasma1Model.destroy();
	portalFantasma2Model.destroy();
	portalPacman1Model.destroy();
	portalPacman2Model.destroy();
	pisoAntorcha1Model.destroy();
	pisoAntorcha2Model.destroy();
	pisoAntorcha3Model.destroy();
	pisoAntorcha4Model.destroy();
	modelAntorcha1.destroy();
	modelAntorcha2.destroy();
	modelAntorcha3.destroy();
	modelAntorcha4.destroy();
	modelCherry.destroy();
	modelOrange.destroy();
	modelStrawberry.destroy();

	// Custom objects animate
	pacmanModelAnimate.destroy();
	LE1ModelAnimate.destroy();
	LE2ModelAnimate.destroy();
	LE3ModelAnimate.destroy();
	LE4ModelAnimate.destroy();
	LE5ModelAnimate.destroy();
	LE6ModelAnimate.destroy();
	LE7ModelAnimate.destroy();
	LE8ModelAnimate.destroy();
	LE9ModelAnimate.destroy();
	LE10ModelAnimate.destroy();
	LE11ModelAnimate.destroy();
	LE12ModelAnimate.destroy();
	LE13ModelAnimate.destroy();
	LE14ModelAnimate.destroy();
	LE15ModelAnimate.destroy();
	LE16ModelAnimate.destroy();
	LE17ModelAnimate.destroy();
	LE18ModelAnimate.destroy();
	LE19ModelAnimate.destroy();
	LE20ModelAnimate.destroy();
	LE21ModelAnimate.destroy();
	LE22ModelAnimate.destroy();
	LE23ModelAnimate.destroy();
	LE24ModelAnimate.destroy();
	LE25ModelAnimate.destroy();
	LE26ModelAnimate.destroy();
	LE27ModelAnimate.destroy();
	LE28ModelAnimate.destroy();
	LE29ModelAnimate.destroy();
	LE30ModelAnimate.destroy();
	LE31ModelAnimate.destroy();
	LE32ModelAnimate.destroy();
	LE33ModelAnimate.destroy();
	LE34ModelAnimate.destroy();
	LE35ModelAnimate.destroy();
	LE36ModelAnimate.destroy();
	LE37ModelAnimate.destroy();
	LE38ModelAnimate.destroy();
	LE39ModelAnimate.destroy();

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

void scrollCallback(GLFWwindow* window, double xoffset, double yoffset){
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

bool processInput(bool continueApplication) {
	if (exitApp || glfwWindowShouldClose(window) != 0) {
		return false;
	}

	if(glfwGetMouseButton(window, GLFW_MOUSE_BUTTON_LEFT) == GLFW_PRESS)
		camera->mouseMoveCamera(offsetX, 0.0, deltaTime);
	if(glfwGetMouseButton(window, GLFW_MOUSE_BUTTON_RIGHT) == GLFW_PRESS)
		camera->mouseMoveCamera(0.0, offsetY, deltaTime);
	offsetX = 0;
	offsetY = 0;

	// Seleccionar modelo
	if (enableCountSelected && glfwGetKey(window, GLFW_KEY_TAB) == GLFW_PRESS){
		enableCountSelected = false;
		modelSelected++;
		if(modelSelected > 1)
			modelSelected = 0;
		
		std::cout << "modelSelected:" << modelSelected << std::endl;
	}

	else if(glfwGetKey(window, GLFW_KEY_TAB) == GLFW_RELEASE)
		enableCountSelected = true;
	
	// Si se presiona la tecla V, se cambia a vista area para tener 
	// un panorama general del laberinto, el tiempo no se pausa, sigue corriendo
	if (glfwGetKey(window, GLFW_KEY_V) == GLFW_PRESS) {
			vistaAerea = 1;
	}

	// Si se presiona la tecla C, se cambia a vista en tercera 
	// persona del personaje de Pac-Man en el laberinto
	if (glfwGetKey(window, GLFW_KEY_C) == GLFW_PRESS) {
		vistaAerea = 0;
	}

	// Mientras no se este en vista área se tiene control de los
	// movimiento del Pac-Man dentro del laberinto, en cada caso 
	// se valida su orientación con respecto al desplazamiento
	if (modelSelected == 0 && glfwGetKey(window, GLFW_KEY_UP) == GLFW_PRESS && vistaAerea == 0) {
		if (anteriorMove != 1)
			rotarMove = 0;
		if (rotarMove == 0 && anteriorMove != 1) {
			if (anteriorMove == 2)
				modelMatrixPacman = glm::rotate(modelMatrixPacman, glm::radians(-90.0f), glm::vec3(0, 1, 0));
			if (anteriorMove == 3)
				modelMatrixPacman = glm::rotate(modelMatrixPacman, glm::radians(180.0f), glm::vec3(0, 1, 0));
			if (anteriorMove == 4)
				modelMatrixPacman = glm::rotate(modelMatrixPacman, glm::radians(90.0f), glm::vec3(0, 1, 0));
			anteriorMove = 1;
			rotarMove = 1;
		}
		modelMatrixPacman = glm::translate(modelMatrixPacman, glm::vec3(0, 0, 0.1));
		animationIndex = 3;
	}
	else if (modelSelected == 0 && glfwGetKey(window, GLFW_KEY_LEFT) == GLFW_PRESS && vistaAerea == 0) {
		if (anteriorMove != 2)
			rotarMove = 0;
		if (rotarMove == 0 && anteriorMove != 2) {
			if (anteriorMove == 1)
				modelMatrixPacman = glm::rotate(modelMatrixPacman, glm::radians(90.0f), glm::vec3(0, 1, 0));
			if (anteriorMove == 3)
				modelMatrixPacman = glm::rotate(modelMatrixPacman, glm::radians(-90.0f), glm::vec3(0, 1, 0));
			if (anteriorMove == 4)
				modelMatrixPacman = glm::rotate(modelMatrixPacman, glm::radians(180.0f), glm::vec3(0, 1, 0));
			anteriorMove = 2;
			rotarMove = 1;
		}
		modelMatrixPacman = glm::translate(modelMatrixPacman, glm::vec3(0, 0, 0.1));
		animationIndex = 3;
	}
	else if (modelSelected == 0 && glfwGetKey(window, GLFW_KEY_DOWN) == GLFW_PRESS && vistaAerea == 0) {
		if (anteriorMove != 3)
			rotarMove = 0;
		if (rotarMove == 0 && anteriorMove != 3) {
			if (anteriorMove == 1)
				modelMatrixPacman = glm::rotate(modelMatrixPacman, glm::radians(180.0f), glm::vec3(0, 1, 0));
			if (anteriorMove == 2)
				modelMatrixPacman = glm::rotate(modelMatrixPacman, glm::radians(90.0f), glm::vec3(0, 1, 0));
			if (anteriorMove == 4)
				modelMatrixPacman = glm::rotate(modelMatrixPacman, glm::radians(-90.0f), glm::vec3(0, 1, 0));
			anteriorMove = 3;
			rotarMove = 1;
		}
		modelMatrixPacman = glm::translate(modelMatrixPacman, glm::vec3(0, 0, 0.1));
		animationIndex = 3;
	}
	else if (modelSelected == 0 && glfwGetKey(window, GLFW_KEY_RIGHT) == GLFW_PRESS && vistaAerea == 0) {
		if (anteriorMove != 4)
			rotarMove = 0;
		if (rotarMove == 0 && anteriorMove != 4) {
			if (anteriorMove == 1)
				modelMatrixPacman = glm::rotate(modelMatrixPacman, glm::radians(-90.0f), glm::vec3(0, 1, 0));
			if (anteriorMove == 2)
				modelMatrixPacman = glm::rotate(modelMatrixPacman, glm::radians(180.0f), glm::vec3(0, 1, 0));
			if (anteriorMove == 3)
				modelMatrixPacman = glm::rotate(modelMatrixPacman, glm::radians(90.0f), glm::vec3(0, 1, 0));
			anteriorMove = 4;
			rotarMove = 1;
		}
		modelMatrixPacman = glm::translate(modelMatrixPacman, glm::vec3(0, 0, 0.1));
		animationIndex = 3;
	}

	glfwPollEvents();
	return continueApplication;
}

void RenderText(Shader& shader, std::string text, float x, float y, float scale, glm::vec3 color)
{
	//glUniform3f(textShader.getUniformLocation("textColor"), color.x, color.y, color.z);
	shader.setVectorFloat3("textColor", glm::value_ptr(glm::vec3(color.x, color.y, color.z)));
	glActiveTexture(GL_TEXTURE11);
	glBindVertexArray(textVAO);

	// iterate through all characters
	std::string::const_iterator c;
	for (c = text.begin(); c != text.end(); c++)
	{
		Character ch = Characters[*c];

		float xpos = x + ch.Bearing.x * scale;
		float ypos = y - (ch.Size.y - ch.Bearing.y) * scale;

		float w = ch.Size.x * scale;
		float h = ch.Size.y * scale;
		// update VBO for each character
		float vertices[6][4] = {
			{ xpos,     ypos + h,   0.0f, 0.0f },
			{ xpos,     ypos,       0.0f, 1.0f },
			{ xpos + w, ypos,       1.0f, 1.0f },

			{ xpos,     ypos + h,   0.0f, 0.0f },
			{ xpos + w, ypos,       1.0f, 1.0f },
			{ xpos + w, ypos + h,   1.0f, 0.0f }
		};
		// render glyph texture over quad
		glBindTexture(GL_TEXTURE_2D, ch.TextureID);
		// update content of VBO memory
		glBindBuffer(GL_ARRAY_BUFFER, textVBO);
		glBufferSubData(GL_ARRAY_BUFFER, 0, sizeof(vertices), vertices); // be sure to use glBufferSubData and not glBufferData

		glBindBuffer(GL_ARRAY_BUFFER, 0);
		// render quad
		glDrawArrays(GL_TRIANGLES, 0, 6);
		// now advance cursors for next glyph (note that advance is number of 1/64 pixels)
		x += (ch.Advance >> 6) * scale; // bitshift by 6 to get value in pixels (2^6 = 64 (divide amount of 1/64th pixels by 64 to get amount of pixels))
	}
	glBindVertexArray(0);
	glBindTexture(GL_TEXTURE_2D, textTextureID);
	textShader.setInt("text", 11);
}

glm::vec3 hex2rgb(string hex) {
	if (hex.at(0) == '#') {
		hex.erase(0, 1);
	}

	while (hex.length() != 6) {
		hex += "0";
	}

	int r = std::stol(hex.substr(0, 2), nullptr, 16);
	int g = std::stol(hex.substr(2, 2), nullptr, 16);
	int b = std::stol(hex.substr(4, 2), nullptr, 16);

	return glm::vec3(r / 255.0f, g / 255.0f, b / 255.0f);
}

void updateUI_Text() {
	string str;

	//Points
	str = "Points: ";
	RenderText(textShader,str + std::to_string(puntosPacman), screenWidth - 120, screenHeight - 20, 0.5f, hex2rgb("ef233c"));

	//Lives
	str = "Lives: ";
	RenderText(textShader, str + std::to_string(vidaPacman), 12, screenHeight - 20, 0.5f, hex2rgb("ef233c"));

	//Time Left
	str = "Time Left: ";
	RenderText(textShader,str + std::to_string(tiempoJuego), screenWidth/2 -50 , screenHeight - 20, 0.5f, hex2rgb("ef233c"));
}

void applicationLoop() {
	bool psi = true;

	glm::mat4 view;
	glm::vec3 axis;
	glm::vec3 target;
	float angleTarget;

	// Pacman
	modelMatrixPacman = glm::translate(modelMatrixPacman, glm::vec3(0.0f, 0.05f, -2.25f));

	//Antorchas
	modelMatrixAntorcha1 = glm::translate(modelMatrixAntorcha1, glm::vec3(-18.0f, -8.0f, 17.5f));
	modelMatrixAntorcha2 = glm::translate(modelMatrixAntorcha2, glm::vec3(18.0f, -8.0f, 17.5f));
	modelMatrixAntorcha3 = glm::translate(modelMatrixAntorcha3, glm::vec3(-18.0f, -8.0f, -17.5f));
	modelMatrixAntorcha4 = glm::translate(modelMatrixAntorcha4, glm::vec3(18.0f, -8.0f, -17.5f));

	// Frutas
	modelMatrixCherry = glm::translate(modelMatrixCherry, glm::vec3(-20.2f, 5.5f, 18.0f));
	modelMatrixOrange = glm::translate(modelMatrixOrange, glm::vec3(20.0f, 5.5f, -18.0f));
	modelMatrixStrawberry = glm::translate(modelMatrixStrawberry, glm::vec3(1.3f, 3.0f,-2.0f));
	//modelMatrixStrawberry = glm::translate(modelMatrixStrawberry, glm::vec3(1.3f, 3.0f, -2.0f));


	// Fantasma azul
	modelMatrixFantasmaRojo = glm::translate(modelMatrixFantasmaRojo, glm::vec3(-20.25f, 0.05f, 0.0f));
	modelMatrixFantasmaRosa = glm::translate(modelMatrixFantasmaRosa, glm::vec3(20.25f, 0.05f, 0.0f));
	modelMatrixFantasmaCian = glm::translate(modelMatrixFantasmaCian, glm::vec3(0.0f, 0.05, 20.25f));
	modelMatrixFantasmaNaranja = glm::translate(modelMatrixFantasmaNaranja, glm::vec3(0.0f, 0.05, -20.25f));

	modelMatrixPortalFantasma1 = glm::translate(modelMatrixPortalFantasma1, glm::vec3(0.0f, 1.0f, 0.0f));
	modelMatrixPortalFantasma2 = glm::translate(modelMatrixPortalFantasma2, glm::vec3(0.0f, 1.0f, 0.0f));

	modelMatrixPortalPacman1 = glm::translate(modelMatrixPortalPacman1, glm::vec3(0.0f, 1.0f, 0.0f));
	modelMatrixPortalPacman2 = glm::translate(modelMatrixPortalPacman2, glm::vec3(0.0f, 1.0f, 0.0f));

	modelMatrixPisoAntorcha1 = glm::translate(modelMatrixPisoAntorcha1, glm::vec3(0.0f, 1.0f, 0.0f));
	modelMatrixPisoAntorcha2 = glm::translate(modelMatrixPisoAntorcha2, glm::vec3(0.0f, 1.0f, 0.0f));
	modelMatrixPisoAntorcha3 = glm::translate(modelMatrixPisoAntorcha3, glm::vec3(0.0f, 1.0f, 0.0f));
	modelMatrixPisoAntorcha4 = glm::translate(modelMatrixPisoAntorcha4, glm::vec3(0.0f, 1.0f, 0.0f));

	// Laberinto
	modelMatrixLE1 = glm::translate(modelMatrixLE1, glm::vec3(0.0f, 1.0f, 0.0f));
	modelMatrixLE2 = glm::translate(modelMatrixLE2, glm::vec3(0.0f, 1.0f, 0.0f));
	modelMatrixLE3 = glm::translate(modelMatrixLE3, glm::vec3(0.0f, 1.0f, 0.0f));
	modelMatrixLE4 = glm::translate(modelMatrixLE4, glm::vec3(0.0f, 1.0f, 0.0f));
	modelMatrixLE5 = glm::translate(modelMatrixLE5, glm::vec3(0.0f, 1.0f, 0.0f));
	modelMatrixLE6 = glm::translate(modelMatrixLE6, glm::vec3(0.0f, 1.0f, 0.0f));
	modelMatrixLE7 = glm::translate(modelMatrixLE7, glm::vec3(0.0f, 1.0f, 0.0f));
	modelMatrixLE8 = glm::translate(modelMatrixLE8, glm::vec3(0.0f, 1.0f, 0.0f));
	modelMatrixLE9 = glm::translate(modelMatrixLE9, glm::vec3(0.0f, 1.0f, 0.0f));
	modelMatrixLE10 = glm::translate(modelMatrixLE10, glm::vec3(0.0f, 1.0f, 0.0f));
	modelMatrixLE11 = glm::translate(modelMatrixLE11, glm::vec3(0.0f, 1.0f, 0.0f));
	modelMatrixLE12 = glm::translate(modelMatrixLE12, glm::vec3(0.0f, 1.0f, 0.0f));
	modelMatrixLE13 = glm::translate(modelMatrixLE13, glm::vec3(0.0f, 1.0f, 0.0f));
	modelMatrixLE14 = glm::translate(modelMatrixLE14, glm::vec3(0.0f, 1.0f, 0.0f));
	modelMatrixLE15 = glm::translate(modelMatrixLE15, glm::vec3(0.0f, 1.0f, 0.0f));
	modelMatrixLE16 = glm::translate(modelMatrixLE16, glm::vec3(0.0f, 1.0f, 0.0f));
	modelMatrixLE17 = glm::translate(modelMatrixLE17, glm::vec3(0.0f, 1.0f, 0.0f));
	modelMatrixLE18 = glm::translate(modelMatrixLE18, glm::vec3(0.0f, 1.0f, 0.0f));
	modelMatrixLE19 = glm::translate(modelMatrixLE19, glm::vec3(0.0f, 1.0f, 0.0f));
	modelMatrixLE20 = glm::translate(modelMatrixLE20, glm::vec3(0.0f, 1.0f, 0.0f));
	modelMatrixLE21 = glm::translate(modelMatrixLE21, glm::vec3(0.0f, 1.0f, 0.0f));
	modelMatrixLE22 = glm::translate(modelMatrixLE22, glm::vec3(0.0f, 1.0f, 0.0f));
	modelMatrixLE23 = glm::translate(modelMatrixLE23, glm::vec3(0.0f, 1.0f, 0.0f));
	modelMatrixLE24 = glm::translate(modelMatrixLE24, glm::vec3(0.0f, 1.0f, 0.0f));
	modelMatrixLE25 = glm::translate(modelMatrixLE25, glm::vec3(0.0f, 1.0f, 0.0f));
	modelMatrixLE26 = glm::translate(modelMatrixLE26, glm::vec3(0.0f, 1.0f, 0.0f));
	modelMatrixLE27 = glm::translate(modelMatrixLE27, glm::vec3(0.0f, 1.0f, 0.0f));
	modelMatrixLE28 = glm::translate(modelMatrixLE28, glm::vec3(0.0f, 1.0f, 0.0f));
	modelMatrixLE29 = glm::translate(modelMatrixLE29, glm::vec3(0.0f, 1.0f, 0.0f));
	modelMatrixLE30 = glm::translate(modelMatrixLE30, glm::vec3(0.0f, 1.0f, 0.0f));
	modelMatrixLE31 = glm::translate(modelMatrixLE31, glm::vec3(0.0f, 1.0f, 0.0f));
	modelMatrixLE32 = glm::translate(modelMatrixLE32, glm::vec3(0.0f, 1.0f, 0.0f));
	modelMatrixLE33 = glm::translate(modelMatrixLE33, glm::vec3(0.0f, 1.0f, 0.0f));
	modelMatrixLE34 = glm::translate(modelMatrixLE34, glm::vec3(0.0f, 1.0f, 0.0f));
	modelMatrixLE35 = glm::translate(modelMatrixLE35, glm::vec3(0.0f, 1.0f, 0.0f));
	modelMatrixLE36 = glm::translate(modelMatrixLE36, glm::vec3(0.0f, 1.0f, 0.0f));
	modelMatrixLE37 = glm::translate(modelMatrixLE37, glm::vec3(0.0f, 1.0f, 0.0f));
	modelMatrixLE38 = glm::translate(modelMatrixLE38, glm::vec3(0.0f, 1.0f, 0.0f));
	modelMatrixLE39 = glm::translate(modelMatrixLE39, glm::vec3(0.0f, 1.0f, 0.0f));
	
	lastTime = TimeManager::Instance().GetTime();

	// Time for the particles animation
	currTimeParticlesAnimation = lastTime;
	lastTimeParticlesAnimation = lastTime;

	currTimeParticlesAnimationFire = lastTime;
	lastTimeParticlesAnimationFire = lastTime;

	glm::vec3 lightPos = glm::vec3(10.0, 10.0, 0.0);

	shadowBox = new ShadowBox(-lightPos, camera.get(), 15.0f, 0.1f, 45.0f);

	while (psi) {
		currTime = TimeManager::Instance().GetTime();
		if(currTime - lastTime < 0.016666667){
			glfwPollEvents();
			continue;
		}
		lastTime = currTime;
		TimeManager::Instance().CalculateFrameRate(true);
		deltaTime = TimeManager::Instance().DeltaTime;
		psi = processInput(true);

		std::map<std::string, bool> collisionDetection;

		glm::mat4 projection = glm::perspective(glm::radians(45.0f),
				(float) screenWidth / (float) screenHeight, 0.1f, 100.0f);

		
		if(modelSelected == 0){
			angleTarget = 0.0f;
			axis = glm::axis(glm::quat_cast(modelMatrixPacman));
			target = modelMatrixPacman[3];
		}

		if (vistaAerea == 0) {
			if (auxVistaArea == 0) {
				camera->setAngleTarget(auxCamAngleTarget);
				camera->setDistanceFromTarget(auxCamDistanceFromTarget);
				camera->setPosition(auxCameraPosition);
				camera->setCameraTarget(auxCameraTarget);
				camera->setPitch(auxCamPitch);
				auxVistaArea = 1;
			}

			camera->setCameraTarget(target);
			camera->setAngleTarget(angleTarget);
		}
		else {
			if (auxVistaArea == 1) {
				auxCamAngleTarget = camera->getAngleTarget();
				auxCamDistanceFromTarget = camera->getDistanceFromTarget();
				auxCameraPosition = camera->getPosition();
				auxCameraTarget = camera->getCameraTarget();
				auxCamPitch = camera->getPitch();
				auxVistaArea = 0;
			}

			camera->setAngleTarget(glm::radians(180.0f));
			camera->setDistanceFromTarget(60.0f);
			camera->setPosition(glm::vec3(0.0, 0.0, 10.0));
			camera->setCameraTarget(glm::vec3(0.0, 0.0, 0.0));
			camera->setPitch(glm::radians(90.0f));
		}

		camera->updateCamera();
		view = camera->getViewMatrix();
		
		shadowBox->update(screenWidth, screenHeight);
		glm::vec3 centerBox = shadowBox->getCenter();

		// Projection light shadow mapping
		glm::mat4 lightProjection = glm::mat4(1.0f), lightView = glm::mat4(1.0f);
		glm::mat4 lightSpaceMatrix;

		lightProjection[0][0] = 2.0f / shadowBox->getWidth();
		lightProjection[1][1] = 2.0f / shadowBox->getHeight();
		lightProjection[2][2] = -2.0f / shadowBox->getLength();
		lightProjection[3][3] = 1.0f;

		lightView = glm::lookAt(centerBox, centerBox + glm::normalize(-lightPos), glm::vec3(0.0, 1.0, 0.0));

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
		// Settea la matriz de vista y projection al shader con multiples luces
		shaderTerrain.setMatrix4("projection", 1, false,
					glm::value_ptr(projection));
		shaderTerrain.setMatrix4("view", 1, false,
				glm::value_ptr(view));
		shaderTerrain.setMatrix4("lightSpaceMatrix", 1, false,
				glm::value_ptr(lightSpaceMatrix));
		// Settea la matriz de vista y projection al shader para el fountain
		shaderParticlesFountain.setMatrix4("projection", 1, false,
					glm::value_ptr(projection));
		shaderParticlesFountain.setMatrix4("view", 1, false,
				glm::value_ptr(view));
		// Settea la matriz de vista y projection al shader para el fuego
		shaderParticlesFire.setInt("Pass", 2);
		shaderParticlesFire.setMatrix4("projection", 1, false, glm::value_ptr(projection));
		shaderParticlesFire.setMatrix4("view", 1, false, glm::value_ptr(view));

		/*******************************************
		 * Propiedades de neblina
		 *******************************************/
		shaderMulLighting.setVectorFloat3("fogColor", glm::value_ptr(glm::vec3(0.5, 0.5, 0.4)));
		shaderTerrain.setVectorFloat3("fogColor", glm::value_ptr(glm::vec3(0.5, 0.5, 0.4)));
		shaderSkybox.setVectorFloat3("fogColor", glm::value_ptr(glm::vec3(0.5, 0.5, 0.4)));

		/*******************************************
		 * Propiedades Luz direccional
		 *******************************************/
		shaderMulLighting.setVectorFloat3("viewPos", glm::value_ptr(camera->getPosition()));
		shaderMulLighting.setVectorFloat3("directionalLight.light.ambient", glm::value_ptr(glm::vec3(0.15, 0.15, 0.15)));
		shaderMulLighting.setVectorFloat3("directionalLight.light.diffuse", glm::value_ptr(glm::vec3(0.45, 0.45, 0.45)));
		shaderMulLighting.setVectorFloat3("directionalLight.light.specular", glm::value_ptr(glm::vec3(0.15, 0.15, 0.15)));
		shaderMulLighting.setVectorFloat3("directionalLight.direction", glm::value_ptr(glm::vec3(-0.707106781, -0.707106781, 0.0)));

		/*******************************************
		 * Propiedades Luz direccional Terrain
		 *******************************************/
		shaderTerrain.setVectorFloat3("viewPos", glm::value_ptr(camera->getPosition()));
		shaderTerrain.setVectorFloat3("directionalLight.light.ambient", glm::value_ptr(glm::vec3(0.15, 0.15, 0.15)));
		shaderTerrain.setVectorFloat3("directionalLight.light.diffuse", glm::value_ptr(glm::vec3(0.45, 0.45, 0.45)));
		shaderTerrain.setVectorFloat3("directionalLight.light.specular", glm::value_ptr(glm::vec3(0.15, 0.15, 0.15)));
		shaderTerrain.setVectorFloat3("directionalLight.direction", glm::value_ptr(glm::vec3(-0.707106781, -0.707106781, 0.0)));

		/*******************************************
		 * Propiedades SpotLights
		 *******************************************/



		/*******************************************
		 * Propiedades PointLights
		 *******************************************/
		


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
		// Collider de Pacman
		AbstractModel::SBB pacmanCollider;
		glm::mat4 modelMatrixColliderPacman = glm::mat4(modelMatrixPacman);
		modelMatrixColliderPacman = glm::scale(modelMatrixColliderPacman, glm::vec3(0.005, 0.005, 0.005));
		// Se hace la suma del vector, porque el pivote esta en el origen, e la esfera
		// de colisión no se ubica de forma adecuada con respecto al modelo
		modelMatrixColliderPacman = glm::translate(modelMatrixColliderPacman,
			pacmanModelAnimate.getSbb().c + glm::vec3(0.0, 160.0, -60.0));
		pacmanCollider.c = glm::vec3(modelMatrixColliderPacman[3]);
		pacmanCollider.ratio = pacmanModelAnimate.getSbb().ratio * 0.005;
		addOrUpdateColliders(collidersSBB, "pacman", pacmanCollider, modelMatrixPacman);


		//Collider cereza 
		AbstractModel::SBB cherryCollider;
		glm::mat4 modelMatrixColliderCherry = glm::mat4(modelMatrixCherry);
		modelMatrixColliderCherry = glm::scale(modelMatrixColliderCherry, glm::vec3(0.005, 0.005, 0.005));
		modelMatrixColliderCherry = glm::translate(modelMatrixColliderCherry, modelCherry.getSbb().c + glm::vec3(0.0, 70.0, 0.0));
		cherryCollider.c = glm::vec3(modelMatrixColliderCherry[3]);
		cherryCollider.ratio = modelCherry.getSbb().ratio * 0.5;
		addOrUpdateColliders(collidersSBB, "cherry", cherryCollider, modelMatrixCherry);

		//Collider naranja
		AbstractModel::SBB orangeCollider;
		glm::mat4 modelMatrixColliderOrange = glm::mat4(modelMatrixOrange);
		modelMatrixColliderOrange = glm::scale(modelMatrixColliderOrange, glm::vec3(0.05, 0.05, 0.05));
		modelMatrixColliderOrange = glm::translate(modelMatrixColliderOrange, modelOrange.getSbb().c + glm::vec3(0.0, 8.0, 0.0));
		orangeCollider.c = glm::vec3(modelMatrixColliderOrange[3]);
		orangeCollider.ratio = modelOrange.getSbb().ratio * 0.7;
		addOrUpdateColliders(collidersSBB, "orange", orangeCollider, modelMatrixOrange);

		//Collider fresa
		AbstractModel::SBB strawberryCollider;
		glm::mat4 modelMatrixColliderStrawberry = glm::mat4(modelMatrixStrawberry);
		modelMatrixColliderStrawberry = glm::scale(modelMatrixColliderStrawberry, glm::vec3(0.005, 0.005, 0.005));
		modelMatrixColliderStrawberry = glm::translate(modelMatrixColliderStrawberry, modelStrawberry.getSbb().c + glm::vec3(0.0, 70.0, 0.0));
		strawberryCollider.c = glm::vec3(modelMatrixColliderStrawberry[3]);
		strawberryCollider.ratio = modelStrawberry.getSbb().ratio * 0.7;
		addOrUpdateColliders(collidersSBB, "fresa", strawberryCollider, modelMatrixStrawberry);

		/*

		
		
		*/
		
		// Punto
		for (int i = 0; i < puntosPosition.size(); i++) {
			if (std::get<1>(puntosPosition.find("punto" + std::to_string(i))->second) == 0) {
				AbstractModel::SBB puntoCollider;
				glm::mat4 modelMatrixColliderPunto = glm::mat4(1.0);
				modelMatrixColliderPunto = glm::translate(modelMatrixColliderPunto, 
					std::get<0>(puntosPosition.find("punto" + std::to_string(i))->second));
				addOrUpdateColliders(collidersSBB, "punto" + std::to_string(i), puntoCollider, modelMatrixColliderPunto);

				modelMatrixColliderPunto = glm::translate(modelMatrixColliderPunto, puntoModel.getSbb().c);
				puntoCollider.c = glm::vec3(modelMatrixColliderPunto[3]);
				puntoCollider.ratio = puntoModel.getSbb().ratio * 0.6;
				std::get<0>(collidersSBB.find("punto" + std::to_string(i))->second) = puntoCollider;
			}
		}

		//Fantasma Rojo
		AbstractModel::OBB fantasmaRojoCollider;
		glm::mat4 modelMatrixColliderFantasmaRojo = glm::mat4(modelMatrixFantasmaRojo);
		modelMatrixColliderFantasmaRojo[3][1] = terrain.getHeightTerrain(modelMatrixColliderFantasmaRojo[3][0], modelMatrixColliderFantasmaRojo[3][2]);
		fantasmaRojoCollider.u = glm::quat_cast(modelMatrixColliderFantasmaRojo);
		modelMatrixColliderFantasmaRojo = glm::scale(modelMatrixColliderFantasmaRojo, glm::vec3(0.074, 0.074, 0.074));
		modelMatrixColliderFantasmaRojo = glm::translate(modelMatrixColliderFantasmaRojo, fantasmaModel.getObb().c);
		fantasmaRojoCollider.c = glm::vec3(modelMatrixColliderFantasmaRojo[3]);
		fantasmaRojoCollider.e = fantasmaModel.getObb().e * glm::vec3(0.074, 0.074, 0.074);
		addOrUpdateColliders(collidersOBB, "fantasmaRojo", fantasmaRojoCollider, modelMatrixFantasmaRojo);

		//Fantasma Rosa
		AbstractModel::OBB fantasmaRosaCollider;
		glm::mat4 modelMatrixColliderFantasmaRosa = glm::mat4(modelMatrixFantasmaRosa);
		modelMatrixColliderFantasmaRosa[3][1] = terrain.getHeightTerrain(modelMatrixColliderFantasmaRosa[3][0], modelMatrixColliderFantasmaRosa[3][2]);
		fantasmaRosaCollider.u = glm::quat_cast(modelMatrixColliderFantasmaRosa);
		modelMatrixColliderFantasmaRosa = glm::scale(modelMatrixColliderFantasmaRosa, glm::vec3(0.074, 0.074, 0.074));
		modelMatrixColliderFantasmaRosa = glm::translate(modelMatrixColliderFantasmaRosa, glm::vec3(fantasmaModel.getObb().c.x,
			fantasmaModel.getObb().c.y, fantasmaModel.getObb().c.z));
		fantasmaRosaCollider.c = glm::vec3(modelMatrixColliderFantasmaRosa[3]);
		fantasmaRosaCollider.e = fantasmaModel.getObb().e * glm::vec3(0.074, 0.074, 0.074);
		addOrUpdateColliders(collidersOBB, "fantasmaRosa", fantasmaRosaCollider, modelMatrixFantasmaRosa);

		//Fantasma Cian
		AbstractModel::OBB fantasmaCianCollider;
		glm::mat4 modelMatrixColliderFantasmaCian = glm::mat4(modelMatrixFantasmaCian);
		modelMatrixColliderFantasmaCian[3][1] = terrain.getHeightTerrain(modelMatrixColliderFantasmaCian[3][0], modelMatrixColliderFantasmaCian[3][2]);
		fantasmaCianCollider.u = glm::quat_cast(modelMatrixColliderFantasmaCian);
		modelMatrixColliderFantasmaCian = glm::scale(modelMatrixColliderFantasmaCian, glm::vec3(0.074, 0.074, 0.074));
		modelMatrixColliderFantasmaCian = glm::translate(modelMatrixColliderFantasmaCian, glm::vec3(fantasmaModel.getObb().c.x,
			fantasmaModel.getObb().c.y, fantasmaModel.getObb().c.z));
		fantasmaCianCollider.c = glm::vec3(modelMatrixColliderFantasmaCian[3]);
		fantasmaCianCollider.e = fantasmaModel.getObb().e * glm::vec3(0.074, 0.074, 0.074);
		addOrUpdateColliders(collidersOBB, "fantasmaCian", fantasmaCianCollider, modelMatrixFantasmaCian);

		//Fantasma Naranja
		AbstractModel::OBB fantasmaNaranjaCollider;
		glm::mat4 modelMatrixColliderFantasmaNaranja = glm::mat4(modelMatrixFantasmaNaranja);
		modelMatrixColliderFantasmaNaranja[3][1] = terrain.getHeightTerrain(modelMatrixColliderFantasmaNaranja[3][0], modelMatrixColliderFantasmaNaranja[3][2]);
		fantasmaNaranjaCollider.u = glm::quat_cast(modelMatrixColliderFantasmaNaranja);
		modelMatrixColliderFantasmaNaranja = glm::scale(modelMatrixColliderFantasmaNaranja, glm::vec3(0.074, 0.074, 0.074));
		modelMatrixColliderFantasmaNaranja = glm::translate(modelMatrixColliderFantasmaNaranja, glm::vec3(fantasmaModel.getObb().c.x,
			fantasmaModel.getObb().c.y, fantasmaModel.getObb().c.z));
		fantasmaNaranjaCollider.c = glm::vec3(modelMatrixColliderFantasmaNaranja[3]);
		fantasmaNaranjaCollider.e = fantasmaModel.getObb().e * glm::vec3(0.074, 0.074, 0.074);
		addOrUpdateColliders(collidersOBB, "fantasmaNaranja", fantasmaNaranjaCollider, modelMatrixFantasmaNaranja);

		/*********************************/
		// Calculo distancia entre fantasmas
		/*********************************/

		if (glm::distance(fantasmaRojoCollider.c, fantasmaRosaCollider.c) <= 3.0f) {
			auxGiroFantasmaRojo = 1;
			auxGiroFantasmaRosa = 1;
		}
		if (glm::distance(fantasmaRojoCollider.c, fantasmaCianCollider.c) <= 3.0f) {
			auxGiroFantasmaRojo = 1;
			auxGiroFantasmaCian = 1;
		}
		if (glm::distance(fantasmaRojoCollider.c, fantasmaNaranjaCollider.c) <= 3.0f) {
			auxGiroFantasmaRojo = 1;
			auxGiroFantasmaNaranja = 1;
		}
		if (glm::distance(fantasmaRosaCollider.c, fantasmaCianCollider.c) <= 3.0f) {
			auxGiroFantasmaRosa = 1;
			auxGiroFantasmaCian = 1;
		}
		if (glm::distance(fantasmaRosaCollider.c, fantasmaNaranjaCollider.c) <= 3.0f) {
			auxGiroFantasmaRosa = 1;
			auxGiroFantasmaNaranja = 1;
		}
		if (glm::distance(fantasmaCianCollider.c, fantasmaNaranjaCollider.c) <= 3.0f) {
			auxGiroFantasmaCian = 1;
			auxGiroFantasmaNaranja = 1;
		}

		// Portal fantasma
		AbstractModel::OBB portalFantasmaCollider;
		glm::mat4 modelMatrixColliderPortalFantasma1 = glm::mat4(modelMatrixPortalFantasma1);
		modelMatrixColliderPortalFantasma1 = glm::scale(modelMatrixColliderPortalFantasma1, glm::vec3(0.5, 0.5, 0.5));
		portalFantasmaCollider.u = glm::quat_cast(modelMatrixColliderPortalFantasma1);
		modelMatrixColliderPortalFantasma1 = glm::translate(modelMatrixColliderPortalFantasma1, glm::vec3(portalFantasma1Model.getObb().c.x, portalFantasma1Model.getObb().c.y, portalFantasma1Model.getObb().c.z));
		portalFantasmaCollider.e = portalFantasma1Model.getObb().e * glm::vec3(0.5, 0.5, 0.5);
		portalFantasmaCollider.c = glm::vec3(modelMatrixColliderPortalFantasma1[3]);
		addOrUpdateColliders(collidersOBB, "portalFantasma1", portalFantasmaCollider, modelMatrixPortalFantasma1);

		glm::mat4 modelMatrixColliderPortalFantasma2 = glm::mat4(modelMatrixPortalFantasma2);
		modelMatrixColliderPortalFantasma2 = glm::scale(modelMatrixColliderPortalFantasma2, glm::vec3(0.5, 0.5, 0.5));
		portalFantasmaCollider.u = glm::quat_cast(modelMatrixColliderPortalFantasma2);
		modelMatrixColliderPortalFantasma2 = glm::translate(modelMatrixColliderPortalFantasma2, glm::vec3(portalFantasma2Model.getObb().c.x, portalFantasma2Model.getObb().c.y, portalFantasma2Model.getObb().c.z));
		portalFantasmaCollider.e = portalFantasma2Model.getObb().e * glm::vec3(0.5, 0.5, 0.5);
		portalFantasmaCollider.c = glm::vec3(modelMatrixColliderPortalFantasma2[3]);
		addOrUpdateColliders(collidersOBB, "portalFantasma2", portalFantasmaCollider, modelMatrixPortalFantasma2);

		// Portal pacman
		AbstractModel::OBB portalPacmanCollider;
		glm::mat4 modelMatrixColliderPortalPacman1 = glm::mat4(modelMatrixPortalPacman1);
		modelMatrixColliderPortalPacman1 = glm::scale(modelMatrixColliderPortalPacman1, glm::vec3(0.5, 0.5, 0.5));
		portalPacmanCollider.u = glm::quat_cast(modelMatrixColliderPortalPacman1);
		modelMatrixColliderPortalPacman1 = glm::translate(modelMatrixColliderPortalPacman1, glm::vec3(portalPacman1Model.getObb().c.x, portalPacman1Model.getObb().c.y, portalPacman1Model.getObb().c.z));
		portalPacmanCollider.e = portalPacman1Model.getObb().e * glm::vec3(0.5, 0.5, 0.5);
		portalPacmanCollider.c = glm::vec3(modelMatrixColliderPortalPacman1[3]);
		addOrUpdateColliders(collidersOBB, "portalPacman1", portalPacmanCollider, modelMatrixPortalPacman1);

		glm::mat4 modelMatrixColliderPortalPacman2 = glm::mat4(modelMatrixPortalPacman2);
		modelMatrixColliderPortalPacman2 = glm::scale(modelMatrixColliderPortalPacman2, glm::vec3(0.5, 0.5, 0.5));
		portalPacmanCollider.u = glm::quat_cast(modelMatrixColliderPortalPacman2);
		modelMatrixColliderPortalPacman2 = glm::translate(modelMatrixColliderPortalPacman2, glm::vec3(portalPacman2Model.getObb().c.x, portalPacman2Model.getObb().c.y, portalPacman2Model.getObb().c.z));
		portalPacmanCollider.e = portalPacman2Model.getObb().e * glm::vec3(0.5, 0.5, 0.5);
		portalPacmanCollider.c = glm::vec3(modelMatrixColliderPortalPacman2[3]);
		addOrUpdateColliders(collidersOBB, "portalPacman2", portalPacmanCollider, modelMatrixPortalPacman2);

		// Piso Antrocha
		AbstractModel::OBB pisoAntorchaCollider;
		glm::mat4 modelMatrixColliderPisoAntorcha1 = glm::mat4(modelMatrixPisoAntorcha1);
		modelMatrixColliderPisoAntorcha1 = glm::scale(modelMatrixColliderPisoAntorcha1, glm::vec3(0.5, 0.5, 0.5));
		pisoAntorchaCollider.u = glm::quat_cast(modelMatrixColliderPisoAntorcha1);
		modelMatrixColliderPisoAntorcha1 = glm::translate(modelMatrixColliderPisoAntorcha1, glm::vec3(pisoAntorcha1Model.getObb().c.x, pisoAntorcha1Model.getObb().c.y, pisoAntorcha1Model.getObb().c.z));
		pisoAntorchaCollider.e = pisoAntorcha1Model.getObb().e * glm::vec3(0.5, 0.5, 0.5);
		pisoAntorchaCollider.c = glm::vec3(modelMatrixColliderPisoAntorcha1[3]);
		addOrUpdateColliders(collidersOBB, "pisoAntorcha1", pisoAntorchaCollider, modelMatrixPisoAntorcha1);

		glm::mat4 modelMatrixColliderPisoAntorcha2 = glm::mat4(modelMatrixPisoAntorcha2);
		modelMatrixColliderPisoAntorcha2 = glm::scale(modelMatrixColliderPisoAntorcha2, glm::vec3(0.5, 0.5, 0.5));
		pisoAntorchaCollider.u = glm::quat_cast(modelMatrixColliderPisoAntorcha2);
		modelMatrixColliderPisoAntorcha2 = glm::translate(modelMatrixColliderPisoAntorcha2, glm::vec3(pisoAntorcha2Model.getObb().c.x, pisoAntorcha2Model.getObb().c.y, pisoAntorcha2Model.getObb().c.z));
		pisoAntorchaCollider.e = pisoAntorcha2Model.getObb().e * glm::vec3(0.5, 0.5, 0.5);
		pisoAntorchaCollider.c = glm::vec3(modelMatrixColliderPisoAntorcha2[3]);
		addOrUpdateColliders(collidersOBB, "pisoAntorcha2", pisoAntorchaCollider, modelMatrixPisoAntorcha2);

		glm::mat4 modelMatrixColliderPisoAntorcha3 = glm::mat4(modelMatrixPisoAntorcha3);
		modelMatrixColliderPisoAntorcha3 = glm::scale(modelMatrixColliderPisoAntorcha3, glm::vec3(0.5, 0.5, 0.5));
		pisoAntorchaCollider.u = glm::quat_cast(modelMatrixColliderPisoAntorcha3);
		modelMatrixColliderPisoAntorcha3 = glm::translate(modelMatrixColliderPisoAntorcha3, glm::vec3(pisoAntorcha3Model.getObb().c.x, pisoAntorcha3Model.getObb().c.y, pisoAntorcha3Model.getObb().c.z));
		pisoAntorchaCollider.e = pisoAntorcha3Model.getObb().e * glm::vec3(0.5, 0.5, 0.5);
		pisoAntorchaCollider.c = glm::vec3(modelMatrixColliderPisoAntorcha3[3]);
		addOrUpdateColliders(collidersOBB, "pisoAntorcha3", pisoAntorchaCollider, modelMatrixPisoAntorcha3);

		glm::mat4 modelMatrixColliderPisoAntorcha4 = glm::mat4(modelMatrixPisoAntorcha4);
		modelMatrixColliderPisoAntorcha4 = glm::scale(modelMatrixColliderPisoAntorcha4, glm::vec3(0.5, 0.5, 0.5));
		pisoAntorchaCollider.u = glm::quat_cast(modelMatrixColliderPisoAntorcha4);
		modelMatrixColliderPisoAntorcha4 = glm::translate(modelMatrixColliderPisoAntorcha4, glm::vec3(pisoAntorcha4Model.getObb().c.x, pisoAntorcha4Model.getObb().c.y, pisoAntorcha4Model.getObb().c.z));
		pisoAntorchaCollider.e = pisoAntorcha4Model.getObb().e * glm::vec3(0.5, 0.5, 0.5);
		pisoAntorchaCollider.c = glm::vec3(modelMatrixColliderPisoAntorcha4[3]);
		addOrUpdateColliders(collidersOBB, "pisoAntorcha4", pisoAntorchaCollider, modelMatrixPisoAntorcha4);

		//Laberinto
		AbstractModel::OBB LECollider;
		glm::mat4 modelmatrixColliderLE1 = glm::mat4(modelMatrixLE1);
		modelmatrixColliderLE1 = glm::scale(modelmatrixColliderLE1, glm::vec3(0.5, 0.5, 0.5));
		LECollider.u = glm::quat_cast(modelmatrixColliderLE1);
		modelmatrixColliderLE1 = glm::translate(modelmatrixColliderLE1, glm::vec3(LE1ModelAnimate.getObb().c.x, LE1ModelAnimate.getObb().c.y, LE1ModelAnimate.getObb().c.z));
		LECollider.e = LE1ModelAnimate.getObb().e * glm::vec3(0.5, 0.5, 0.5);
		LECollider.c = glm::vec3(modelmatrixColliderLE1[3]);
		addOrUpdateColliders(collidersOBB, "LE1", LECollider, modelMatrixLE1);
		
		glm::mat4 modelmatrixColliderLE2 = glm::mat4(modelMatrixLE2);
		modelmatrixColliderLE2 = glm::scale(modelmatrixColliderLE2, glm::vec3(0.5, 0.5, 0.5));
		LECollider.u = glm::quat_cast(modelmatrixColliderLE2);
		modelmatrixColliderLE2 = glm::translate(modelmatrixColliderLE2, glm::vec3(LE2ModelAnimate.getObb().c.x, LE2ModelAnimate.getObb().c.y, LE2ModelAnimate.getObb().c.z));
		LECollider.e = LE2ModelAnimate.getObb().e * glm::vec3(0.5, 0.5, 0.5);
		LECollider.c = glm::vec3(modelmatrixColliderLE2[3]);
		addOrUpdateColliders(collidersOBB, "LE2", LECollider, modelMatrixLE2);
		
		glm::mat4 modelmatrixColliderLE3 = glm::mat4(modelMatrixLE3);
		modelmatrixColliderLE3 = glm::scale(modelmatrixColliderLE3, glm::vec3(0.5, 0.5, 0.5));
		LECollider.u = glm::quat_cast(modelmatrixColliderLE3);
		modelmatrixColliderLE3 = glm::translate(modelmatrixColliderLE3, glm::vec3(LE3ModelAnimate.getObb().c.x, LE3ModelAnimate.getObb().c.y, LE3ModelAnimate.getObb().c.z));
		LECollider.e = LE3ModelAnimate.getObb().e * glm::vec3(0.5, 0.5, 0.5);
		LECollider.c = glm::vec3(modelmatrixColliderLE3[3]);
		addOrUpdateColliders(collidersOBB, "LE3", LECollider, modelMatrixLE3);
		
		glm::mat4 modelmatrixColliderLE4 = glm::mat4(modelMatrixLE4);
		modelmatrixColliderLE4 = glm::scale(modelmatrixColliderLE4, glm::vec3(0.5, 0.5, 0.5));
		LECollider.u = glm::quat_cast(modelmatrixColliderLE4);
		modelmatrixColliderLE4 = glm::translate(modelmatrixColliderLE4, glm::vec3(LE4ModelAnimate.getObb().c.x, LE4ModelAnimate.getObb().c.y, LE4ModelAnimate.getObb().c.z));
		LECollider.e = LE4ModelAnimate.getObb().e * glm::vec3(0.5, 0.5, 0.5);
		LECollider.c = glm::vec3(modelmatrixColliderLE4[3]);
		addOrUpdateColliders(collidersOBB, "LE4", LECollider, modelMatrixLE4);
		
		glm::mat4 modelmatrixColliderLE5 = glm::mat4(modelMatrixLE5);
		modelmatrixColliderLE5 = glm::scale(modelmatrixColliderLE5, glm::vec3(0.5, 0.5, 0.5));
		LECollider.u = glm::quat_cast(modelmatrixColliderLE5);
		modelmatrixColliderLE5 = glm::translate(modelmatrixColliderLE5, glm::vec3(LE5ModelAnimate.getObb().c.x, LE5ModelAnimate.getObb().c.y, LE5ModelAnimate.getObb().c.z));
		LECollider.e = LE5ModelAnimate.getObb().e * glm::vec3(0.5, 0.5, 0.5);
		LECollider.c = glm::vec3(modelmatrixColliderLE5[3]);
		addOrUpdateColliders(collidersOBB, "LE5", LECollider, modelMatrixLE5);
		
		glm::mat4 modelmatrixColliderLE6 = glm::mat4(modelMatrixLE6);
		modelmatrixColliderLE6 = glm::scale(modelmatrixColliderLE6, glm::vec3(0.5, 0.5, 0.5));
		LECollider.u = glm::quat_cast(modelmatrixColliderLE6);
		modelmatrixColliderLE6 = glm::translate(modelmatrixColliderLE6, glm::vec3(LE6ModelAnimate.getObb().c.x, LE6ModelAnimate.getObb().c.y, LE6ModelAnimate.getObb().c.z));
		LECollider.e = LE6ModelAnimate.getObb().e * glm::vec3(0.5, 0.5, 0.5);
		LECollider.c = glm::vec3(modelmatrixColliderLE6[3]);
		addOrUpdateColliders(collidersOBB, "LE6", LECollider, modelMatrixLE6);
		
		glm::mat4 modelmatrixColliderLE7 = glm::mat4(modelMatrixLE7);
		modelmatrixColliderLE7 = glm::scale(modelmatrixColliderLE7, glm::vec3(0.5, 0.5, 0.5));
		LECollider.u = glm::quat_cast(modelmatrixColliderLE7);
		modelmatrixColliderLE7 = glm::translate(modelmatrixColliderLE7, glm::vec3(LE7ModelAnimate.getObb().c.x, LE7ModelAnimate.getObb().c.y, LE7ModelAnimate.getObb().c.z));
		LECollider.e = LE7ModelAnimate.getObb().e * glm::vec3(0.5, 0.5, 0.5);
		LECollider.c = glm::vec3(modelmatrixColliderLE7[3]);
		addOrUpdateColliders(collidersOBB, "LE7", LECollider, modelMatrixLE7);
		
		glm::mat4 modelmatrixColliderLE8 = glm::mat4(modelMatrixLE8);
		modelmatrixColliderLE8 = glm::scale(modelmatrixColliderLE8, glm::vec3(0.5, 0.5, 0.5));
		LECollider.u = glm::quat_cast(modelmatrixColliderLE8);
		modelmatrixColliderLE8 = glm::translate(modelmatrixColliderLE8, glm::vec3(LE8ModelAnimate.getObb().c.x, LE8ModelAnimate.getObb().c.y, LE8ModelAnimate.getObb().c.z));
		LECollider.e = LE8ModelAnimate.getObb().e * glm::vec3(0.5, 0.5, 0.5);
		LECollider.c = glm::vec3(modelmatrixColliderLE8[3]);
		addOrUpdateColliders(collidersOBB, "LE8", LECollider, modelMatrixLE8);
		
		glm::mat4 modelmatrixColliderLE9 = glm::mat4(modelMatrixLE9);
		modelmatrixColliderLE9 = glm::scale(modelmatrixColliderLE9, glm::vec3(0.5, 0.5, 0.5));
		LECollider.u = glm::quat_cast(modelmatrixColliderLE9);
		modelmatrixColliderLE9 = glm::translate(modelmatrixColliderLE9, glm::vec3(LE9ModelAnimate.getObb().c.x, LE9ModelAnimate.getObb().c.y, LE9ModelAnimate.getObb().c.z));
		LECollider.e = LE9ModelAnimate.getObb().e * glm::vec3(0.5, 0.5, 0.5);
		LECollider.c = glm::vec3(modelmatrixColliderLE9[3]);
		addOrUpdateColliders(collidersOBB, "LE9", LECollider, modelMatrixLE9);
		
		glm::mat4 modelmatrixColliderLE10 = glm::mat4(modelMatrixLE10);
		modelmatrixColliderLE10 = glm::scale(modelmatrixColliderLE10, glm::vec3(0.5, 0.5, 0.5));
		LECollider.u = glm::quat_cast(modelmatrixColliderLE10);
		modelmatrixColliderLE10 = glm::translate(modelmatrixColliderLE10, glm::vec3(LE10ModelAnimate.getObb().c.x, LE10ModelAnimate.getObb().c.y, LE10ModelAnimate.getObb().c.z));
		LECollider.e = LE10ModelAnimate.getObb().e * glm::vec3(0.5, 0.5, 0.5);
		LECollider.c = glm::vec3(modelmatrixColliderLE10[3]);
		addOrUpdateColliders(collidersOBB, "LE10", LECollider, modelMatrixLE10);
		
		glm::mat4 modelmatrixColliderLE11 = glm::mat4(modelMatrixLE11);
		modelmatrixColliderLE11 = glm::scale(modelmatrixColliderLE11, glm::vec3(0.5, 0.5, 0.5));
		LECollider.u = glm::quat_cast(modelmatrixColliderLE11);
		modelmatrixColliderLE11 = glm::translate(modelmatrixColliderLE11, glm::vec3(LE11ModelAnimate.getObb().c.x, LE11ModelAnimate.getObb().c.y, LE11ModelAnimate.getObb().c.z));
		LECollider.e = LE11ModelAnimate.getObb().e * glm::vec3(0.5, 0.5, 0.5);
		LECollider.c = glm::vec3(modelmatrixColliderLE11[3]);
		addOrUpdateColliders(collidersOBB, "LE11", LECollider, modelMatrixLE11);
		
		glm::mat4 modelmatrixColliderLE12 = glm::mat4(modelMatrixLE12);
		modelmatrixColliderLE12 = glm::scale(modelmatrixColliderLE12, glm::vec3(0.5, 0.5, 0.5));
		LECollider.u = glm::quat_cast(modelmatrixColliderLE12);
		modelmatrixColliderLE12 = glm::translate(modelmatrixColliderLE12, glm::vec3(LE12ModelAnimate.getObb().c.x, LE12ModelAnimate.getObb().c.y, LE12ModelAnimate.getObb().c.z));
		LECollider.e = LE12ModelAnimate.getObb().e * glm::vec3(0.5, 0.5, 0.5);
		LECollider.c = glm::vec3(modelmatrixColliderLE12[3]);
		addOrUpdateColliders(collidersOBB, "LE12", LECollider, modelMatrixLE12);
		
		glm::mat4 modelmatrixColliderLE13 = glm::mat4(modelMatrixLE13);
		modelmatrixColliderLE13 = glm::scale(modelmatrixColliderLE13, glm::vec3(0.5, 0.5, 0.5));
		LECollider.u = glm::quat_cast(modelmatrixColliderLE13);
		modelmatrixColliderLE13 = glm::translate(modelmatrixColliderLE13, glm::vec3(LE13ModelAnimate.getObb().c.x, LE13ModelAnimate.getObb().c.y, LE13ModelAnimate.getObb().c.z));
		LECollider.e = LE13ModelAnimate.getObb().e * glm::vec3(0.5, 0.5, 0.5);
		LECollider.c = glm::vec3(modelmatrixColliderLE13[3]);
		addOrUpdateColliders(collidersOBB, "LE13", LECollider, modelMatrixLE13);
		
		glm::mat4 modelmatrixColliderLE14 = glm::mat4(modelMatrixLE14);
		modelmatrixColliderLE14 = glm::scale(modelmatrixColliderLE14, glm::vec3(0.5, 0.5, 0.5));
		LECollider.u = glm::quat_cast(modelmatrixColliderLE14);
		modelmatrixColliderLE14 = glm::translate(modelmatrixColliderLE14, glm::vec3(LE14ModelAnimate.getObb().c.x, LE14ModelAnimate.getObb().c.y, LE14ModelAnimate.getObb().c.z));
		LECollider.e = LE14ModelAnimate.getObb().e * glm::vec3(0.5, 0.5, 0.5);
		LECollider.c = glm::vec3(modelmatrixColliderLE14[3]);
		addOrUpdateColliders(collidersOBB, "LE14", LECollider, modelMatrixLE14);
		
		glm::mat4 modelmatrixColliderLE15 = glm::mat4(modelMatrixLE15);
		modelmatrixColliderLE15 = glm::scale(modelmatrixColliderLE15, glm::vec3(0.5, 0.5, 0.5));
		LECollider.u = glm::quat_cast(modelmatrixColliderLE15);
		modelmatrixColliderLE15 = glm::translate(modelmatrixColliderLE15, glm::vec3(LE15ModelAnimate.getObb().c.x, LE15ModelAnimate.getObb().c.y, LE15ModelAnimate.getObb().c.z));
		LECollider.e = LE15ModelAnimate.getObb().e * glm::vec3(0.5, 0.5, 0.5);
		LECollider.c = glm::vec3(modelmatrixColliderLE15[3]);
		addOrUpdateColliders(collidersOBB, "LE15", LECollider, modelMatrixLE15);
		
		glm::mat4 modelmatrixColliderLE16 = glm::mat4(modelMatrixLE16);
		modelmatrixColliderLE16 = glm::scale(modelmatrixColliderLE16, glm::vec3(0.5, 0.5, 0.5));
		LECollider.u = glm::quat_cast(modelmatrixColliderLE16);
		modelmatrixColliderLE16 = glm::translate(modelmatrixColliderLE16, glm::vec3(LE16ModelAnimate.getObb().c.x, LE16ModelAnimate.getObb().c.y, LE16ModelAnimate.getObb().c.z));
		LECollider.e = LE16ModelAnimate.getObb().e * glm::vec3(0.5, 0.5, 0.5);
		LECollider.c = glm::vec3(modelmatrixColliderLE16[3]);
		addOrUpdateColliders(collidersOBB, "LE16", LECollider, modelMatrixLE16);
		
		glm::mat4 modelmatrixColliderLE17 = glm::mat4(modelMatrixLE17);
		modelmatrixColliderLE17 = glm::scale(modelmatrixColliderLE17, glm::vec3(0.5, 0.5, 0.5));
		LECollider.u = glm::quat_cast(modelmatrixColliderLE17);
		modelmatrixColliderLE17 = glm::translate(modelmatrixColliderLE17, glm::vec3(LE17ModelAnimate.getObb().c.x, LE17ModelAnimate.getObb().c.y, LE17ModelAnimate.getObb().c.z));
		LECollider.e = LE17ModelAnimate.getObb().e * glm::vec3(0.5, 0.5, 0.5);
		LECollider.c = glm::vec3(modelmatrixColliderLE17[3]);
		addOrUpdateColliders(collidersOBB, "LE17", LECollider, modelMatrixLE17);
		
		glm::mat4 modelmatrixColliderLE18 = glm::mat4(modelMatrixLE18);
		modelmatrixColliderLE18 = glm::scale(modelmatrixColliderLE18, glm::vec3(0.5, 0.5, 0.5));
		LECollider.u = glm::quat_cast(modelmatrixColliderLE18);
		modelmatrixColliderLE18 = glm::translate(modelmatrixColliderLE18, glm::vec3(LE18ModelAnimate.getObb().c.x, LE18ModelAnimate.getObb().c.y, LE18ModelAnimate.getObb().c.z));
		LECollider.e = LE18ModelAnimate.getObb().e * glm::vec3(0.5, 0.5, 0.5);
		LECollider.c = glm::vec3(modelmatrixColliderLE18[3]);
		addOrUpdateColliders(collidersOBB, "LE18", LECollider, modelMatrixLE18);
		
		glm::mat4 modelmatrixColliderLE19 = glm::mat4(modelMatrixLE19);
		modelmatrixColliderLE19 = glm::scale(modelmatrixColliderLE19, glm::vec3(0.5, 0.5, 0.5));
		LECollider.u = glm::quat_cast(modelmatrixColliderLE19);
		modelmatrixColliderLE19 = glm::translate(modelmatrixColliderLE19, glm::vec3(LE19ModelAnimate.getObb().c.x, LE19ModelAnimate.getObb().c.y, LE19ModelAnimate.getObb().c.z));
		LECollider.e = LE19ModelAnimate.getObb().e * glm::vec3(0.5, 0.5, 0.5);
		LECollider.c = glm::vec3(modelmatrixColliderLE19[3]);
		addOrUpdateColliders(collidersOBB, "LE19", LECollider, modelMatrixLE19);
		
		glm::mat4 modelmatrixColliderLE20 = glm::mat4(modelMatrixLE20);
		modelmatrixColliderLE20 = glm::scale(modelmatrixColliderLE20, glm::vec3(0.5, 0.5, 0.5));
		LECollider.u = glm::quat_cast(modelmatrixColliderLE20);
		modelmatrixColliderLE20 = glm::translate(modelmatrixColliderLE20, glm::vec3(LE20ModelAnimate.getObb().c.x, LE20ModelAnimate.getObb().c.y, LE20ModelAnimate.getObb().c.z));
		LECollider.e = LE20ModelAnimate.getObb().e * glm::vec3(0.5, 0.5, 0.5);
		LECollider.c = glm::vec3(modelmatrixColliderLE20[3]);
		addOrUpdateColliders(collidersOBB, "LE20", LECollider, modelMatrixLE20);
		
		glm::mat4 modelmatrixColliderLE21 = glm::mat4(modelMatrixLE21);
		modelmatrixColliderLE21 = glm::scale(modelmatrixColliderLE21, glm::vec3(0.5, 0.5, 0.5));
		LECollider.u = glm::quat_cast(modelmatrixColliderLE21);
		modelmatrixColliderLE21 = glm::translate(modelmatrixColliderLE21, glm::vec3(LE21ModelAnimate.getObb().c.x, LE21ModelAnimate.getObb().c.y, LE21ModelAnimate.getObb().c.z));
		LECollider.e = LE21ModelAnimate.getObb().e * glm::vec3(0.5, 0.5, 0.5);
		LECollider.c = glm::vec3(modelmatrixColliderLE21[3]);
		addOrUpdateColliders(collidersOBB, "LE21", LECollider, modelMatrixLE21);
		
		glm::mat4 modelmatrixColliderLE22 = glm::mat4(modelMatrixLE22);
		modelmatrixColliderLE22 = glm::scale(modelmatrixColliderLE22, glm::vec3(0.5, 0.5, 0.5));
		LECollider.u = glm::quat_cast(modelmatrixColliderLE22);
		modelmatrixColliderLE22 = glm::translate(modelmatrixColliderLE22, glm::vec3(LE22ModelAnimate.getObb().c.x, LE22ModelAnimate.getObb().c.y, LE22ModelAnimate.getObb().c.z));
		LECollider.e = LE22ModelAnimate.getObb().e * glm::vec3(0.5, 0.5, 0.5);
		LECollider.c = glm::vec3(modelmatrixColliderLE22[3]);
		addOrUpdateColliders(collidersOBB, "LE22", LECollider, modelMatrixLE22);
		
		glm::mat4 modelmatrixColliderLE23 = glm::mat4(modelMatrixLE23);
		modelmatrixColliderLE23 = glm::scale(modelmatrixColliderLE23, glm::vec3(0.5, 0.5, 0.5));
		LECollider.u = glm::quat_cast(modelmatrixColliderLE23);
		modelmatrixColliderLE23 = glm::translate(modelmatrixColliderLE23, glm::vec3(LE23ModelAnimate.getObb().c.x, LE23ModelAnimate.getObb().c.y, LE23ModelAnimate.getObb().c.z));
		LECollider.e = LE23ModelAnimate.getObb().e * glm::vec3(0.5, 0.5, 0.5);
		LECollider.c = glm::vec3(modelmatrixColliderLE23[3]);
		addOrUpdateColliders(collidersOBB, "LE23", LECollider, modelMatrixLE23);
		
		glm::mat4 modelmatrixColliderLE24 = glm::mat4(modelMatrixLE24);
		modelmatrixColliderLE24 = glm::scale(modelmatrixColliderLE24, glm::vec3(0.5, 0.5, 0.5));
		LECollider.u = glm::quat_cast(modelmatrixColliderLE24);
		modelmatrixColliderLE24 = glm::translate(modelmatrixColliderLE24, glm::vec3(LE24ModelAnimate.getObb().c.x, LE24ModelAnimate.getObb().c.y, LE24ModelAnimate.getObb().c.z));
		LECollider.e = LE24ModelAnimate.getObb().e * glm::vec3(0.5, 0.5, 0.5);
		LECollider.c = glm::vec3(modelmatrixColliderLE24[3]);
		addOrUpdateColliders(collidersOBB, "LE24", LECollider, modelMatrixLE24);
		
		glm::mat4 modelmatrixColliderLE25 = glm::mat4(modelMatrixLE25);
		modelmatrixColliderLE25 = glm::scale(modelmatrixColliderLE25, glm::vec3(0.5, 0.5, 0.5));
		LECollider.u = glm::quat_cast(modelmatrixColliderLE25);
		modelmatrixColliderLE25 = glm::translate(modelmatrixColliderLE25, glm::vec3(LE25ModelAnimate.getObb().c.x, LE25ModelAnimate.getObb().c.y, LE25ModelAnimate.getObb().c.z));
		LECollider.e = LE25ModelAnimate.getObb().e * glm::vec3(0.5, 0.5, 0.5);
		LECollider.c = glm::vec3(modelmatrixColliderLE25[3]);
		addOrUpdateColliders(collidersOBB, "LE25", LECollider, modelMatrixLE25);
		
		glm::mat4 modelmatrixColliderLE26 = glm::mat4(modelMatrixLE26);
		modelmatrixColliderLE26 = glm::scale(modelmatrixColliderLE26, glm::vec3(0.5, 0.5, 0.5));
		LECollider.u = glm::quat_cast(modelmatrixColliderLE26);
		modelmatrixColliderLE26 = glm::translate(modelmatrixColliderLE26, glm::vec3(LE26ModelAnimate.getObb().c.x, LE26ModelAnimate.getObb().c.y, LE26ModelAnimate.getObb().c.z));
		LECollider.e = LE26ModelAnimate.getObb().e * glm::vec3(0.5, 0.5, 0.5);
		LECollider.c = glm::vec3(modelmatrixColliderLE26[3]);
		addOrUpdateColliders(collidersOBB, "LE26", LECollider, modelMatrixLE26);
		
		glm::mat4 modelmatrixColliderLE27 = glm::mat4(modelMatrixLE27);
		modelmatrixColliderLE27 = glm::scale(modelmatrixColliderLE27, glm::vec3(0.5, 0.5, 0.5));
		LECollider.u = glm::quat_cast(modelmatrixColliderLE27);
		modelmatrixColliderLE27 = glm::translate(modelmatrixColliderLE27, glm::vec3(LE27ModelAnimate.getObb().c.x, LE27ModelAnimate.getObb().c.y, LE27ModelAnimate.getObb().c.z));
		LECollider.e = LE27ModelAnimate.getObb().e * glm::vec3(0.5, 0.5, 0.5);
		LECollider.c = glm::vec3(modelmatrixColliderLE27[3]);
		addOrUpdateColliders(collidersOBB, "LE27", LECollider, modelMatrixLE27);
		
		glm::mat4 modelmatrixColliderLE28 = glm::mat4(modelMatrixLE28);
		modelmatrixColliderLE28 = glm::scale(modelmatrixColliderLE28, glm::vec3(0.5, 0.5, 0.5));
		LECollider.u = glm::quat_cast(modelmatrixColliderLE28);
		modelmatrixColliderLE28 = glm::translate(modelmatrixColliderLE28, glm::vec3(LE28ModelAnimate.getObb().c.x, LE28ModelAnimate.getObb().c.y, LE28ModelAnimate.getObb().c.z));
		LECollider.e = LE28ModelAnimate.getObb().e * glm::vec3(0.5, 0.5, 0.5);
		LECollider.c = glm::vec3(modelmatrixColliderLE28[3]);
		addOrUpdateColliders(collidersOBB, "LE28", LECollider, modelMatrixLE28);
		
		glm::mat4 modelmatrixColliderLE29 = glm::mat4(modelMatrixLE29);
		modelmatrixColliderLE29 = glm::scale(modelmatrixColliderLE29, glm::vec3(0.5, 0.5, 0.5));
		LECollider.u = glm::quat_cast(modelmatrixColliderLE29);
		modelmatrixColliderLE29 = glm::translate(modelmatrixColliderLE29, glm::vec3(LE29ModelAnimate.getObb().c.x, LE29ModelAnimate.getObb().c.y, LE29ModelAnimate.getObb().c.z));
		LECollider.e = LE29ModelAnimate.getObb().e * glm::vec3(0.5, 0.5, 0.5);
		LECollider.c = glm::vec3(modelmatrixColliderLE29[3]);
		addOrUpdateColliders(collidersOBB, "LE29", LECollider, modelMatrixLE29);
		
		glm::mat4 modelmatrixColliderLE30 = glm::mat4(modelMatrixLE30);
		modelmatrixColliderLE30 = glm::scale(modelmatrixColliderLE30, glm::vec3(0.5, 0.5, 0.5));
		LECollider.u = glm::quat_cast(modelmatrixColliderLE30);
		modelmatrixColliderLE30 = glm::translate(modelmatrixColliderLE30, glm::vec3(LE30ModelAnimate.getObb().c.x, LE30ModelAnimate.getObb().c.y, LE30ModelAnimate.getObb().c.z));
		LECollider.e = LE30ModelAnimate.getObb().e * glm::vec3(0.5, 0.5, 0.5);
		LECollider.c = glm::vec3(modelmatrixColliderLE30[3]);
		addOrUpdateColliders(collidersOBB, "LE30", LECollider, modelMatrixLE30);
		
		glm::mat4 modelmatrixColliderLE31 = glm::mat4(modelMatrixLE31);
		modelmatrixColliderLE31 = glm::scale(modelmatrixColliderLE31, glm::vec3(0.5, 0.5, 0.5));
		LECollider.u = glm::quat_cast(modelmatrixColliderLE31);
		modelmatrixColliderLE31 = glm::translate(modelmatrixColliderLE31, glm::vec3(LE31ModelAnimate.getObb().c.x, LE31ModelAnimate.getObb().c.y, LE31ModelAnimate.getObb().c.z));
		LECollider.e = LE31ModelAnimate.getObb().e * glm::vec3(0.5, 0.5, 0.5);
		LECollider.c = glm::vec3(modelmatrixColliderLE31[3]);
		addOrUpdateColliders(collidersOBB, "LE31", LECollider, modelMatrixLE31);
		
		glm::mat4 modelmatrixColliderLE32 = glm::mat4(modelMatrixLE32);
		modelmatrixColliderLE32 = glm::scale(modelmatrixColliderLE32, glm::vec3(0.5, 0.5, 0.5));
		LECollider.u = glm::quat_cast(modelmatrixColliderLE32);
		modelmatrixColliderLE32 = glm::translate(modelmatrixColliderLE32, glm::vec3(LE32ModelAnimate.getObb().c.x, LE32ModelAnimate.getObb().c.y, LE32ModelAnimate.getObb().c.z));
		LECollider.e = LE32ModelAnimate.getObb().e * glm::vec3(0.5, 0.5, 0.5);
		LECollider.c = glm::vec3(modelmatrixColliderLE32[3]);
		addOrUpdateColliders(collidersOBB, "LE32", LECollider, modelMatrixLE32);
		
		glm::mat4 modelmatrixColliderLE33 = glm::mat4(modelMatrixLE33);
		modelmatrixColliderLE33 = glm::scale(modelmatrixColliderLE33, glm::vec3(0.5, 0.5, 0.5));
		LECollider.u = glm::quat_cast(modelmatrixColliderLE33);
		modelmatrixColliderLE33 = glm::translate(modelmatrixColliderLE33, glm::vec3(LE33ModelAnimate.getObb().c.x, LE33ModelAnimate.getObb().c.y, LE33ModelAnimate.getObb().c.z));
		LECollider.e = LE33ModelAnimate.getObb().e * glm::vec3(0.5, 0.5, 0.5);
		LECollider.c = glm::vec3(modelmatrixColliderLE33[3]);
		addOrUpdateColliders(collidersOBB, "LE33", LECollider, modelMatrixLE33);
		
		glm::mat4 modelmatrixColliderLE34 = glm::mat4(modelMatrixLE34);
		modelmatrixColliderLE34 = glm::scale(modelmatrixColliderLE34, glm::vec3(0.5, 0.5, 0.5));
		LECollider.u = glm::quat_cast(modelmatrixColliderLE34);
		modelmatrixColliderLE34 = glm::translate(modelmatrixColliderLE34, glm::vec3(LE34ModelAnimate.getObb().c.x, LE34ModelAnimate.getObb().c.y, LE34ModelAnimate.getObb().c.z));
		LECollider.e = LE34ModelAnimate.getObb().e * glm::vec3(0.5, 0.5, 0.5);
		LECollider.c = glm::vec3(modelmatrixColliderLE34[3]);
		addOrUpdateColliders(collidersOBB, "LE34", LECollider, modelMatrixLE34);
		
		glm::mat4 modelmatrixColliderLE35 = glm::mat4(modelMatrixLE35);
		modelmatrixColliderLE35 = glm::scale(modelmatrixColliderLE35, glm::vec3(0.5, 0.5, 0.5));
		LECollider.u = glm::quat_cast(modelmatrixColliderLE35);
		modelmatrixColliderLE35 = glm::translate(modelmatrixColliderLE35, glm::vec3(LE35ModelAnimate.getObb().c.x, LE35ModelAnimate.getObb().c.y, LE35ModelAnimate.getObb().c.z));
		LECollider.e = LE35ModelAnimate.getObb().e * glm::vec3(0.5, 0.5, 0.5);
		LECollider.c = glm::vec3(modelmatrixColliderLE35[3]);
		addOrUpdateColliders(collidersOBB, "LE35", LECollider, modelMatrixLE35);
		
		glm::mat4 modelmatrixColliderLE36 = glm::mat4(modelMatrixLE36);
		modelmatrixColliderLE36 = glm::scale(modelmatrixColliderLE36, glm::vec3(0.5, 0.5, 0.5));
		LECollider.u = glm::quat_cast(modelmatrixColliderLE36);
		modelmatrixColliderLE36 = glm::translate(modelmatrixColliderLE36, glm::vec3(LE36ModelAnimate.getObb().c.x, LE36ModelAnimate.getObb().c.y, LE36ModelAnimate.getObb().c.z));
		LECollider.e = LE36ModelAnimate.getObb().e * glm::vec3(0.5, 0.5, 0.5);
		LECollider.c = glm::vec3(modelmatrixColliderLE36[3]);
		addOrUpdateColliders(collidersOBB, "LE36", LECollider, modelMatrixLE36);
		
		glm::mat4 modelmatrixColliderLE37 = glm::mat4(modelMatrixLE37);
		modelmatrixColliderLE37 = glm::scale(modelmatrixColliderLE37, glm::vec3(0.5, 0.5, 0.5));
		LECollider.u = glm::quat_cast(modelmatrixColliderLE37);
		modelmatrixColliderLE37 = glm::translate(modelmatrixColliderLE37, glm::vec3(LE37ModelAnimate.getObb().c.x, LE37ModelAnimate.getObb().c.y, LE37ModelAnimate.getObb().c.z));
		LECollider.e = LE37ModelAnimate.getObb().e * glm::vec3(0.5, 0.5, 0.5);
		LECollider.c = glm::vec3(modelmatrixColliderLE37[3]);
		addOrUpdateColliders(collidersOBB, "LE37", LECollider, modelMatrixLE37);
		
		glm::mat4 modelmatrixColliderLE38 = glm::mat4(modelMatrixLE38);
		modelmatrixColliderLE38 = glm::scale(modelmatrixColliderLE38, glm::vec3(0.5, 0.5, 0.5));
		LECollider.u = glm::quat_cast(modelmatrixColliderLE38);
		modelmatrixColliderLE38 = glm::translate(modelmatrixColliderLE38, glm::vec3(LE38ModelAnimate.getObb().c.x, LE38ModelAnimate.getObb().c.y, LE38ModelAnimate.getObb().c.z));
		LECollider.e = LE38ModelAnimate.getObb().e * glm::vec3(0.5, 0.5, 0.5);
		LECollider.c = glm::vec3(modelmatrixColliderLE38[3]);
		addOrUpdateColliders(collidersOBB, "LE38", LECollider, modelMatrixLE38);

		glm::mat4 modelmatrixColliderLE39 = glm::mat4(modelMatrixLE39);
		modelmatrixColliderLE39 = glm::scale(modelmatrixColliderLE39, glm::vec3(0.5, 0.5, 0.5));
		LECollider.u = glm::quat_cast(modelmatrixColliderLE39);
		modelmatrixColliderLE39 = glm::translate(modelmatrixColliderLE39, glm::vec3(LE39ModelAnimate.getObb().c.x, LE39ModelAnimate.getObb().c.y, LE39ModelAnimate.getObb().c.z));
		LECollider.e = LE39ModelAnimate.getObb().e * glm::vec3(0.5, 0.5, 0.5);
		LECollider.c = glm::vec3(modelmatrixColliderLE39[3]);
		addOrUpdateColliders(collidersOBB, "LE39", LECollider, modelMatrixLE39);
		
		/*******************************************
		 * Render de colliders
		 *******************************************/
		for (std::map<std::string, std::tuple<AbstractModel::OBB, glm::mat4, glm::mat4> >::iterator it =
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
		}

		/*******************************************
		 * Test Colisions
		 *******************************************/
		for (std::map<std::string,
				std::tuple<AbstractModel::OBB, glm::mat4, glm::mat4> >::iterator it =
				collidersOBB.begin(); it != collidersOBB.end(); it++) {
			bool isCollision = false;
			for (std::map<std::string,
					std::tuple<AbstractModel::OBB, glm::mat4, glm::mat4> >::iterator jt =
					collidersOBB.begin(); jt != collidersOBB.end(); jt++) {
				if (it != jt && testOBBOBB(std::get<0>(it->second), std::get<0>(jt->second))) {
					// Se verifica la colision de un fantasma con alguna pared
					if (it->first.compare("fantasmaRojo") == 0 && jt->first[0] != 'f') {
						if (verificaMoveFantasmaRojo == 1){
							moveIzqFantasmaRojo = 1;
						}
						else if (verificaMoveFantasmaRojo == 3){
							moveDerFantasmaRojo = 1;
						}
					}
					if (it->first.compare("fantasmaRosa") == 0 && jt->first[0] != 'f') {
						if (verificaMoveFantasmaRosa == 1) {
							moveIzqFantasmaRosa = 1;
						}
						else if (verificaMoveFantasmaRosa == 3) {
							moveDerFantasmaRosa = 1;
						}
					}
					if (it->first.compare("fantasmaCian") == 0 && jt->first[0] != 'f') {
						if (verificaMoveFantasmaCian == 1) {
							moveIzqFantasmaCian = 1;
						}
						else if (verificaMoveFantasmaCian == 3) {
							moveDerFantasmaCian = 1;
						}
					}
					if (it->first.compare("fantasmaNaranja") == 0 && jt->first[0] != 'f') {
						if (verificaMoveFantasmaNaranja == 1) {
							moveIzqFantasmaNaranja = 1;
						}
						else if (verificaMoveFantasmaNaranja == 3) {
							moveDerFantasmaNaranja = 1;
						}
					}
					
					//std::cout << "Colision " << it->first << " with " << jt->first << std::endl;

					if (it->first[0] == 'f' && jt->first[0] == 'f')
						isCollision = false;
					else
						isCollision = true;
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
				if (it != jt && testSphereSphereIntersection(std::get<0>(it->second), std::get<0>(jt->second))) {
					//std::cout << "Colision " << it->first << " with " << jt->first << std::endl;
					// Se verifica colision entre un punto y Pacman
					if (it->first[0] == 'p' && it->first[1] == 'u') {
						std::get<1>(puntosPosition.find(it->first)->second) = 1;
						puntoObtenido.insert(std::pair<std::string, int>(it->first, 1));
					}
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
				if (testSphereOBox(std::get<0>(it->second), std::get<0>(jt->second))) {
					// Se evita detectar la colision entre un PUNTO y un FANTASMA
					if (it->first[1] != 'u' && jt->first[0] != 'f') {
						isCollision = true;
						addOrUpdateCollisionDetection(collisionDetection, jt->first, isCollision);
					}
					// Se detecta colision entre PACMAN y FANTASMA
					if (it->first[1] == 'a' && jt->first[0] == 'f') {
						if (vidaPacman >= 2) {
							vidaPacman = vidaPacman - 1;
							std::cout << "Puntos -> " << puntosPacman << std::endl;
							std::cout << "Vida -> " << vidaPacman << std::endl;
						}
						else {
							psi = false;
							std::cout << "******** FIN DEL JUEGO, SIN VIDAD ********" << std::endl;
							break;
						}
						isCollision = true;
						addOrUpdateCollisionDetection(collisionDetection, jt->first, isCollision);
					}
				}
			}
			addOrUpdateCollisionDetection(collisionDetection, it->first, isCollision);
		}

		std::map<std::string, bool>::iterator colIt;
		for (colIt = collisionDetection.begin(); colIt != collisionDetection.end(); colIt++) {
			std::map<std::string,
					std::tuple<AbstractModel::SBB, glm::mat4, glm::mat4> >::iterator it =
					collidersSBB.find(colIt->first);
			std::map<std::string,
					std::tuple<AbstractModel::OBB, glm::mat4, glm::mat4> >::iterator jt =
					collidersOBB.find(colIt->first);
			if (it != collidersSBB.end()) {
				if (!colIt->second)
					addOrUpdateColliders(collidersSBB, it->first);
				else {
					if (it->first.compare("pacman") == 0)
						modelMatrixPacman = std::get<1>(it->second);
				}
			}
			if (jt != collidersOBB.end()) {
				if (!colIt->second)
					addOrUpdateColliders(collidersOBB, jt->first);
				else {
					if (jt->first.compare("fantasmaRojo") == 0)
						modelMatrixFantasmaRojo = std::get<1>(jt->second);
					if (jt->first.compare("fantasmaRosa") == 0)
						modelMatrixFantasmaRosa = std::get<1>(jt->second);
					if (jt->first.compare("fantasmaCian") == 0)
						modelMatrixFantasmaCian = std::get<1>(jt->second);
					if (jt->first.compare("fantasmaNaranja") == 0)
						modelMatrixFantasmaNaranja = std::get<1>(jt->second);
					if (jt->first.compare("LE1") == 0)
						modelMatrixLE1 = std::get<1>(jt->second);
					if (jt->first.compare("LE2") == 0)
						modelMatrixLE2 = std::get<1>(jt->second);
					if (jt->first.compare("LE3") == 0)
						modelMatrixLE3 = std::get<1>(jt->second);
					if (jt->first.compare("LE4") == 0)
						modelMatrixLE4 = std::get<1>(jt->second);
					if (jt->first.compare("LE5") == 0)
						modelMatrixLE5 = std::get<1>(jt->second);
					if (jt->first.compare("LE6") == 0)
						modelMatrixLE6 = std::get<1>(jt->second);
					if (jt->first.compare("LE7") == 0)
						modelMatrixLE7 = std::get<1>(jt->second);
					if (jt->first.compare("LE8") == 0)
						modelMatrixLE8 = std::get<1>(jt->second);
					if (jt->first.compare("LE9") == 0)
						modelMatrixLE9 = std::get<1>(jt->second);
					if (jt->first.compare("LE10") == 0)
						modelMatrixLE10 = std::get<1>(jt->second);
					if (jt->first.compare("LE11") == 0)
						modelMatrixLE11 = std::get<1>(jt->second);
					if (jt->first.compare("LE12") == 0)
						modelMatrixLE12 = std::get<1>(jt->second);
					if (jt->first.compare("LE13") == 0)
						modelMatrixLE13 = std::get<1>(jt->second);
					if (jt->first.compare("LE14") == 0)
						modelMatrixLE14 = std::get<1>(jt->second);
					if (jt->first.compare("LE15") == 0)
						modelMatrixLE15 = std::get<1>(jt->second);
					if (jt->first.compare("LE16") == 0)
						modelMatrixLE16 = std::get<1>(jt->second);
					if (jt->first.compare("LE17") == 0)
						modelMatrixLE17 = std::get<1>(jt->second);
					if (jt->first.compare("LE18") == 0)
						modelMatrixLE18 = std::get<1>(jt->second);
					if (jt->first.compare("LE19") == 0)
						modelMatrixLE19 = std::get<1>(jt->second);
					if (jt->first.compare("LE20") == 0)
						modelMatrixLE20 = std::get<1>(jt->second);
					if (jt->first.compare("LE21") == 0)
						modelMatrixLE21 = std::get<1>(jt->second);
					if (jt->first.compare("LE22") == 0)
						modelMatrixLE22 = std::get<1>(jt->second);
					if (jt->first.compare("LE23") == 0)
						modelMatrixLE23 = std::get<1>(jt->second);
					if (jt->first.compare("LE24") == 0)
						modelMatrixLE24 = std::get<1>(jt->second);
					if (jt->first.compare("LE25") == 0)
						modelMatrixLE25 = std::get<1>(jt->second);
					if (jt->first.compare("LE26") == 0)
						modelMatrixLE26 = std::get<1>(jt->second);
					if (jt->first.compare("LE27") == 0)
						modelMatrixLE27 = std::get<1>(jt->second);
					if (jt->first.compare("LE28") == 0)
						modelMatrixLE28 = std::get<1>(jt->second);
					if (jt->first.compare("LE29") == 0)
						modelMatrixLE29 = std::get<1>(jt->second);
					if (jt->first.compare("LE30") == 0)
						modelMatrixLE30 = std::get<1>(jt->second);
					if (jt->first.compare("LE31") == 0)
						modelMatrixLE31 = std::get<1>(jt->second);
					if (jt->first.compare("LE32") == 0)
						modelMatrixLE32 = std::get<1>(jt->second);
					if (jt->first.compare("LE33") == 0)
						modelMatrixLE33 = std::get<1>(jt->second);
					if (jt->first.compare("LE34") == 0)
						modelMatrixLE34 = std::get<1>(jt->second);
					if (jt->first.compare("LE35") == 0)
						modelMatrixLE35 = std::get<1>(jt->second);
					if (jt->first.compare("LE36") == 0)
						modelMatrixLE36 = std::get<1>(jt->second);
					if (jt->first.compare("LE37") == 0)
						modelMatrixLE37 = std::get<1>(jt->second);
					if (jt->first.compare("LE38") == 0)
						modelMatrixLE38 = std::get<1>(jt->second);
					if (jt->first.compare("LE39") == 0)
						modelMatrixLE39 = std::get<1>(jt->second);
					if (jt->first.compare("portalFantasma1") == 0)
						modelMatrixPortalFantasma1 = std::get<1>(jt->second);
					if (jt->first.compare("portalFantasma2") == 0)
						modelMatrixPortalFantasma1 = std::get<1>(jt->second);
					if (jt->first.compare("portalPacman1") == 0)
						modelMatrixPortalPacman1 = std::get<1>(jt->second);
					if (jt->first.compare("portalPacman2") == 0)
						modelMatrixPortalPacman2 = std::get<1>(jt->second);
					if (jt->first.compare("pisoAntorcha1") == 0)
						modelMatrixPisoAntorcha1 = std::get<1>(jt->second);
					if (jt->first.compare("pisoAntorcha2") == 0)
						modelMatrixPisoAntorcha2 = std::get<1>(jt->second);
					if (jt->first.compare("pisoAntorcha3") == 0)
						modelMatrixPisoAntorcha3 = std::get<1>(jt->second);
					if (jt->first.compare("pisoAntorcha4") == 0)
						modelMatrixPisoAntorcha4 = std::get<1>(jt->second);

				}
			}
		}
		
		for (std::map<std::string, int>::iterator p = puntoObtenido.begin(); p != puntoObtenido.end(); p++) {
			collidersSBB.erase(collidersSBB.find(p->first));
			puntosPacman = puntosPacman + p->second;
			std::cout << "Puntos -> " << puntosPacman << std::endl;
			std::cout << "Vida -> " << vidaPacman << std::endl;
		}

		puntoObtenido.clear();

		// Constantes de animaciones
		animationIndex = 2;

		/*******************************************
		 * State machines
		 *******************************************/
		// Movimiento fantasma 
		// Para poder recorrer más dinamicamente el laberinto
		// verificamos los posibles caminos para ver la posibilidad
		// de algun giro al avanzar hacia al frente

		/********************************/
		// Movimiento fantasma ROJO
		/********************************/
		// Verificamos si chocamos con alguna pared a la izquierda
		if (verificaMoveFantasmaRojo == 0) {
			modelMatrixFantasmaRojo = glm::translate(modelMatrixFantasmaRojo, glm::vec3(-0.5625, 0.0, 0.0));
			verificaMoveFantasmaRojo = 1;
		}
		// Esperamos un tiempo
		else if (verificaMoveFantasmaRojo == 1)
			verificaMoveFantasmaRojo = 2;
		// Verificamos si chocamos con alguna pared a la derecha
		else if (verificaMoveFantasmaRojo == 2) {
			modelMatrixFantasmaRojo = glm::translate(modelMatrixFantasmaRojo, glm::vec3(0.5625, 0, 0.0));
			verificaMoveFantasmaRojo = 3;
		}
		// Esperamos un tiempo
		else if (verificaMoveFantasmaRojo == 3)
			verificaMoveFantasmaRojo = 4;
		// Avanzamos hacia al frente
		else if (verificaMoveFantasmaRojo == 4) {
			if (moveIzqFantasmaRojo == 1 && moveDerFantasmaRojo == 0) 
				modelMatrixFantasmaRojo = glm::rotate(modelMatrixFantasmaRojo, glm::radians(90.0f), glm::vec3(0, 1, 0));
			else if (moveIzqFantasmaRojo == 0 && moveDerFantasmaRojo == 1) 
				modelMatrixFantasmaRojo = glm::rotate(modelMatrixFantasmaRojo, glm::radians(-90.0f), glm::vec3(0, 1, 0));
			else if (moveIzqFantasmaRojo == 0 && moveDerFantasmaRojo == 0) {
				auxGiro = rand() % 10;
				if (auxGiro == 0 || auxGiro == 2 || auxGiro == 4 || auxGiro == 6 || auxGiro == 8)
					modelMatrixFantasmaRojo = glm::rotate(modelMatrixFantasmaRojo, glm::radians(90.0f), glm::vec3(0, 1, 0));
				else
					modelMatrixFantasmaRojo = glm::rotate(modelMatrixFantasmaRojo, glm::radians(-90.0f), glm::vec3(0, 1, 0));
			}
			if (auxGiroFantasmaRojo == 1) {
				modelMatrixFantasmaRojo = glm::rotate(modelMatrixFantasmaRojo, glm::radians(180.0f), glm::vec3(0, 1, 0));
				auxGiroFantasmaRojo = 0;
			}
			modelMatrixFantasmaRojo = glm::translate(modelMatrixFantasmaRojo, glm::vec3(0.0, 0, 0.5625));
			verificaMoveFantasmaRojo = 0;
			moveIzqFantasmaRojo = 0;
			moveDerFantasmaRojo = 0;
		}

		/********************************/
		// Movimiento fantasma ROSA
		/********************************/
		// Verificamos si chocamos con alguna pared a la izquierda
		if (verificaMoveFantasmaRosa == 0) {
			modelMatrixFantasmaRosa = glm::translate(modelMatrixFantasmaRosa, glm::vec3(-0.5625, 0.0, 0.0));
			verificaMoveFantasmaRosa = 1;
		}
		// Esperamos un tiempo
		else if (verificaMoveFantasmaRosa == 1)
			verificaMoveFantasmaRosa = 2;
		// Verificamos si chocamos con alguna pared a la derecha
		else if (verificaMoveFantasmaRosa == 2) {
			modelMatrixFantasmaRosa = glm::translate(modelMatrixFantasmaRosa, glm::vec3(0.5625, 0, 0.0));
			verificaMoveFantasmaRosa = 3;
		}
		// Esperamos un tiempo
		else if (verificaMoveFantasmaRosa == 3)
			verificaMoveFantasmaRosa = 4;
		// Avanzamos hacia al frente
		else if (verificaMoveFantasmaRosa == 4) {
			if (moveIzqFantasmaRosa == 1 && moveDerFantasmaRosa == 0)
				modelMatrixFantasmaRosa = glm::rotate(modelMatrixFantasmaRosa, glm::radians(90.0f), glm::vec3(0, 1, 0));
			else if (moveIzqFantasmaRosa == 0 && moveDerFantasmaRosa == 1)
				modelMatrixFantasmaRosa = glm::rotate(modelMatrixFantasmaRosa, glm::radians(-90.0f), glm::vec3(0, 1, 0));
			else if (moveIzqFantasmaRosa == 0 && moveDerFantasmaRosa == 0) {
				auxGiro = rand() % 10;
				if (auxGiro == 0 || auxGiro == 2 || auxGiro == 4 || auxGiro == 6 || auxGiro == 8)
					modelMatrixFantasmaRosa = glm::rotate(modelMatrixFantasmaRosa, glm::radians(90.0f), glm::vec3(0, 1, 0));
				else
					modelMatrixFantasmaRosa = glm::rotate(modelMatrixFantasmaRosa, glm::radians(-90.0f), glm::vec3(0, 1, 0));
			}
			if (auxGiroFantasmaRosa == 1) {
				modelMatrixFantasmaRosa = glm::rotate(modelMatrixFantasmaRosa, glm::radians(180.0f), glm::vec3(0, 1, 0));
				auxGiroFantasmaRosa = 0;
			}
			modelMatrixFantasmaRosa = glm::translate(modelMatrixFantasmaRosa, glm::vec3(0.0, 0, 0.5625));
			verificaMoveFantasmaRosa = 0;
			moveIzqFantasmaRosa = 0;
			moveDerFantasmaRosa = 0;
		}

		/********************************/
		// Movimiento fantasma CIAN
		/********************************/
		// Verificamos si chocamos con alguna pared a la izquierda
		if (verificaMoveFantasmaCian == 0) {
			modelMatrixFantasmaCian = glm::translate(modelMatrixFantasmaCian, glm::vec3(-0.5625, 0.0, 0.0));
			verificaMoveFantasmaCian = 1;
		}
		// Esperamos un tiempo
		else if (verificaMoveFantasmaCian == 1)
			verificaMoveFantasmaCian = 2;
		// Verificamos si chocamos con alguna pared a la derecha
		else if (verificaMoveFantasmaCian == 2) {
			modelMatrixFantasmaCian = glm::translate(modelMatrixFantasmaCian, glm::vec3(0.5625, 0, 0.0));
			verificaMoveFantasmaCian = 3;
		}
		// Esperamos un tiempo
		else if (verificaMoveFantasmaCian == 3)
			verificaMoveFantasmaCian = 4;
		// Avanzamos hacia al frente
		else if (verificaMoveFantasmaCian == 4) {
			if (moveIzqFantasmaCian == 1 && moveDerFantasmaCian == 0)
				modelMatrixFantasmaCian = glm::rotate(modelMatrixFantasmaCian, glm::radians(90.0f), glm::vec3(0, 1, 0));
			else if (moveIzqFantasmaCian == 0 && moveDerFantasmaCian == 1)
				modelMatrixFantasmaCian = glm::rotate(modelMatrixFantasmaCian, glm::radians(-90.0f), glm::vec3(0, 1, 0));
			else if (moveIzqFantasmaCian == 0 && moveDerFantasmaCian == 0) {
				auxGiro = rand() % 10;
				if (auxGiro == 0 || auxGiro == 2 || auxGiro == 4 || auxGiro == 6 || auxGiro == 8)
					modelMatrixFantasmaCian = glm::rotate(modelMatrixFantasmaCian, glm::radians(90.0f), glm::vec3(0, 1, 0));
				else
					modelMatrixFantasmaCian = glm::rotate(modelMatrixFantasmaCian, glm::radians(-90.0f), glm::vec3(0, 1, 0));
			}
			if (auxGiroFantasmaCian == 1) {
				modelMatrixFantasmaCian = glm::rotate(modelMatrixFantasmaCian, glm::radians(180.0f), glm::vec3(0, 1, 0));
				auxGiroFantasmaCian = 0;
			}
			modelMatrixFantasmaCian = glm::translate(modelMatrixFantasmaCian, glm::vec3(0.0, 0, 0.5625));
			verificaMoveFantasmaCian = 0;
			moveIzqFantasmaCian = 0;
			moveDerFantasmaCian = 0;
		}

		/********************************/
		// Movimiento fantasma NARANJA
		/********************************/
		// Verificamos si chocamos con alguna pared a la izquierda
		if (verificaMoveFantasmaNaranja == 0) {
			modelMatrixFantasmaNaranja = glm::translate(modelMatrixFantasmaNaranja, glm::vec3(-0.5625, 0.0, 0.0));
			verificaMoveFantasmaNaranja = 1;
		}
		// Esperamos un tiempo
		else if (verificaMoveFantasmaNaranja == 1)
			verificaMoveFantasmaNaranja = 2;
		// Verificamos si chocamos con alguna pared a la derecha
		else if (verificaMoveFantasmaNaranja == 2) {
			modelMatrixFantasmaNaranja = glm::translate(modelMatrixFantasmaNaranja, glm::vec3(0.5625, 0, 0.0));
			verificaMoveFantasmaNaranja = 3;
		}
		// Esperamos un tiempo
		else if (verificaMoveFantasmaNaranja == 3)
			verificaMoveFantasmaNaranja = 4;
		// Avanzamos hacia al frente
		else if (verificaMoveFantasmaNaranja == 4) {
			if (moveIzqFantasmaNaranja == 1 && moveDerFantasmaNaranja == 0)
				modelMatrixFantasmaNaranja = glm::rotate(modelMatrixFantasmaNaranja, glm::radians(90.0f), glm::vec3(0, 1, 0));
			else if (moveIzqFantasmaNaranja == 0 && moveDerFantasmaNaranja == 1)
				modelMatrixFantasmaNaranja = glm::rotate(modelMatrixFantasmaNaranja, glm::radians(-90.0f), glm::vec3(0, 1, 0));
			else if (moveIzqFantasmaNaranja == 0 && moveDerFantasmaNaranja == 0) {
				auxGiro = rand() % 10;
				if (auxGiro == 0 || auxGiro == 2 || auxGiro == 4 || auxGiro == 6 || auxGiro == 8)
					modelMatrixFantasmaNaranja = glm::rotate(modelMatrixFantasmaNaranja, glm::radians(90.0f), glm::vec3(0, 1, 0));
				else
					modelMatrixFantasmaNaranja = glm::rotate(modelMatrixFantasmaNaranja, glm::radians(-90.0f), glm::vec3(0, 1, 0));
			}
			if (auxGiroFantasmaNaranja == 1) {
				modelMatrixFantasmaNaranja = glm::rotate(modelMatrixFantasmaNaranja, glm::radians(180.0f), glm::vec3(0, 1, 0));
				auxGiroFantasmaNaranja = 0;
			}
			modelMatrixFantasmaNaranja = glm::translate(modelMatrixFantasmaNaranja, glm::vec3(0.0, 0, 0.5625));
			verificaMoveFantasmaNaranja = 0;
			moveIzqFantasmaNaranja = 0;
			moveDerFantasmaNaranja = 0;
		}

		glfwSwapBuffers(window);

		/****************************+
		 * Open AL sound data
		 */
		// Listener for the thiird person camera -> PACMAN
		listenerPos[0] = modelMatrixPacman[3].x;
		listenerPos[1] = modelMatrixPacman[3].y;
		listenerPos[2] = modelMatrixPacman[3].z;
		alListenerfv(AL_POSITION, listenerPos);

		glm::vec3 upModel = glm::normalize(modelMatrixPacman[1]);
		glm::vec3 frontModel = glm::normalize(modelMatrixPacman[2]);

		listenerOri[0] = frontModel.x;
		listenerOri[1] = frontModel.y;
		listenerOri[2] = frontModel.z;
		listenerOri[3] = upModel.x;
		listenerOri[4] = upModel.y;
		listenerOri[5] = upModel.z;

		alListenerfv(AL_ORIENTATION, listenerOri);

		for(unsigned int i = 0; i < sourcesPlay.size(); i++){
			if(sourcesPlay[i]){
				sourcesPlay[i] = false;
				alSourcePlay(source[i]);
			}
		}
	}
}

void prepareScene(){

	skyboxSphere.setShader(&shaderSkybox);

	terrain.setShader(&shaderTerrain);

	//Grass
	modelGrass.setShader(&shaderMulLighting);

	//Pacman
	pacmanModelAnimate.setShader(&shaderMulLighting);

	//Antochas
	modelAntorcha1.setShader(&shaderMulLighting);
	modelAntorcha2.setShader(&shaderMulLighting);
	modelAntorcha3.setShader(&shaderMulLighting);
	modelAntorcha4.setShader(&shaderMulLighting);

	//Frutas
	modelCherry.setShader(&shaderMulLighting);
	modelOrange.setShader(&shaderMulLighting);
	modelStrawberry.setShader(&shaderMulLighting);


}

void prepareDepthScene(){

	skyboxSphere.setShader(&shaderDepth);

	terrain.setShader(&shaderDepth);

	//Grass
	modelGrass.setShader(&shaderDepth);

	//Pacman
	pacmanModelAnimate.setShader(&shaderDepth);
}

void renderScene(bool renderParticles){

	/*******************************************
	 * Terrain Cesped
	 *******************************************/
	// Se activa la textura del ladrillo rojo
	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, textureTerrainBackgroundID);
	shaderTerrain.setInt("backgroundTexture", 0);
	// Se activa la textura de desierto
	glActiveTexture(GL_TEXTURE1);
	glBindTexture(GL_TEXTURE_2D, textureTerrainRID);
	shaderTerrain.setInt("rTexture", 1);
	// Se activa la textura de arena
	glActiveTexture(GL_TEXTURE2);
	glBindTexture(GL_TEXTURE_2D, textureTerrainGID);
	shaderTerrain.setInt("gTexture", 2);
	// Se activa la textura de agua
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

	/*******************************************
	 * Custom objects obj
	 *******************************************/

	// Puntos
	for (int i = 0; i < puntosPosition.size(); i++) {
		if (std::get<1>(puntosPosition.find("punto" + std::to_string(i))->second) == 0) {
			std::get<0>(puntosPosition.find("punto" + std::to_string(i))->second).y =
				terrain.getHeightTerrain(std::get<0>(puntosPosition.find("punto" + std::to_string(i))->second).x,
					std::get<0>(puntosPosition.find("punto" + std::to_string(i))->second).z);
			puntoModel.setPosition(std::get<0>(puntosPosition.find("punto" + std::to_string(i))->second));
			puntoModel.render();
		}
	}

	// Portal fantasma
	modelMatrixPortalFantasma1[3][1] = terrain.getHeightTerrain(modelMatrixPortalFantasma1[3][0], modelMatrixPortalFantasma1[3][2]);
	glm::mat4 modelMatrixPortalFantasma1Body = glm::mat4(modelMatrixPortalFantasma1);
	modelMatrixPortalFantasma1Body = glm::scale(modelMatrixPortalFantasma1Body, glm::vec3(0.5, 0.5, 0.5));
	portalFantasma1Model.render(modelMatrixPortalFantasma1Body);

	modelMatrixPortalFantasma2[3][1] = terrain.getHeightTerrain(modelMatrixPortalFantasma2[3][0], modelMatrixPortalFantasma2[3][2]);
	glm::mat4 modelMatrixPortalFantasma2Body = glm::mat4(modelMatrixPortalFantasma2);
	modelMatrixPortalFantasma2Body = glm::scale(modelMatrixPortalFantasma2Body, glm::vec3(0.5, 0.5, 0.5));
	portalFantasma2Model.render(modelMatrixPortalFantasma2Body);

	// Portal pacman
	modelMatrixPortalPacman1[3][1] = terrain.getHeightTerrain(modelMatrixPortalPacman1[3][0], modelMatrixPortalPacman1[3][2]);
	glm::mat4 modelMatrixPortalPacman1Body = glm::mat4(modelMatrixPortalPacman1);
	modelMatrixPortalPacman1Body = glm::scale(modelMatrixPortalPacman1Body, glm::vec3(0.5, 0.5, 0.5));
	portalPacman1Model.render(modelMatrixPortalPacman1Body);

	modelMatrixPortalPacman2[3][1] = terrain.getHeightTerrain(modelMatrixPortalPacman2[3][0], modelMatrixPortalPacman2[3][2]);
	glm::mat4 modelMatrixPortalPacman2Body = glm::mat4(modelMatrixPortalPacman2);
	modelMatrixPortalPacman2Body = glm::scale(modelMatrixPortalPacman2Body, glm::vec3(0.5, 0.5, 0.5));
	portalPacman2Model.render(modelMatrixPortalPacman2Body);

	// Piso antorcha
	modelMatrixPisoAntorcha1[3][1] = terrain.getHeightTerrain(modelMatrixPisoAntorcha1[3][0], modelMatrixPisoAntorcha1[3][2]);
	glm::mat4 modelMatrixPisoAntorcha1Body = glm::mat4(modelMatrixPisoAntorcha1);
	modelMatrixPisoAntorcha1Body = glm::scale(modelMatrixPisoAntorcha1Body, glm::vec3(0.5, 0.5, 0.5));
	pisoAntorcha1Model.render(modelMatrixPisoAntorcha1Body);

	modelMatrixPisoAntorcha2[3][1] = terrain.getHeightTerrain(modelMatrixPisoAntorcha2[3][0], modelMatrixPisoAntorcha2[3][2]);
	glm::mat4 modelMatrixPisoAntorcha2Body = glm::mat4(modelMatrixPisoAntorcha2);
	modelMatrixPisoAntorcha2Body = glm::scale(modelMatrixPisoAntorcha2Body, glm::vec3(0.5, 0.5, 0.5));
	pisoAntorcha2Model.render(modelMatrixPisoAntorcha2Body);

	modelMatrixPisoAntorcha3[3][1] = terrain.getHeightTerrain(modelMatrixPisoAntorcha3[3][0], modelMatrixPisoAntorcha3[3][2]);
	glm::mat4 modelMatrixPisoAntorcha3Body = glm::mat4(modelMatrixPisoAntorcha3);
	modelMatrixPisoAntorcha3Body = glm::scale(modelMatrixPisoAntorcha3Body, glm::vec3(0.5, 0.5, 0.5));
	pisoAntorcha3Model.render(modelMatrixPisoAntorcha3Body);

	modelMatrixPisoAntorcha4[3][1] = terrain.getHeightTerrain(modelMatrixPisoAntorcha4[3][0], modelMatrixPisoAntorcha1[3][2]);
	glm::mat4 modelMatrixPisoAntorcha4Body = glm::mat4(modelMatrixPisoAntorcha4);
	modelMatrixPisoAntorcha4Body = glm::scale(modelMatrixPisoAntorcha4Body, glm::vec3(0.5, 0.5, 0.5));
	pisoAntorcha4Model.render(modelMatrixPisoAntorcha4Body);

	/*******************************************
	 * Custom Anim objects obj
	 *******************************************/
	//Pacman
	modelMatrixPacman[3][1] = terrain.getHeightTerrain(modelMatrixPacman[3][0], modelMatrixPacman[3][2]);
	glm::mat4 modelMatrixPacmanBody = glm::mat4(modelMatrixPacman);
	modelMatrixPacmanBody = glm::scale(modelMatrixPacmanBody, glm::vec3(0.005, 0.005, 0.005));
	pacmanModelAnimate.setAnimationIndex(animationIndex);
	pacmanModelAnimate.render(modelMatrixPacmanBody);


	//Antorchas

	modelMatrixAntorcha1[3][1] = terrain.getHeightTerrain(modelMatrixAntorcha1[3][0], modelMatrixAntorcha1[3][2]);
	modelAntorcha1.render(modelMatrixAntorcha1);

	modelMatrixAntorcha2[3][1] = terrain.getHeightTerrain(modelMatrixAntorcha2[3][0], modelMatrixAntorcha2[3][2]);
	modelAntorcha2.render(modelMatrixAntorcha2);

	modelMatrixAntorcha3[3][1] = terrain.getHeightTerrain(modelMatrixAntorcha3[3][0], modelMatrixAntorcha3[3][2]);
	modelAntorcha3.render(modelMatrixAntorcha3);

	modelMatrixAntorcha4[3][1] = terrain.getHeightTerrain(modelMatrixAntorcha4[3][0], modelMatrixAntorcha4[3][2]);
	modelAntorcha4.render(modelMatrixAntorcha4);


	//Frutas
	modelMatrixCherry[3][1] = terrain.getHeightTerrain(modelMatrixCherry[3][0], modelMatrixCherry[3][2]);
	modelCherry.render(modelMatrixCherry);
	modelMatrixOrange[3][1] = terrain.getHeightTerrain(modelMatrixOrange[3][0], modelMatrixOrange[3][2]);
	modelOrange.render(modelMatrixOrange);
	modelMatrixStrawberry[3][1] = terrain.getHeightTerrain(modelMatrixStrawberry[3][0], modelMatrixStrawberry[3][2]);
	modelStrawberry.render(modelMatrixStrawberry);
	
	//Laberinto
	modelMatrixLE1[3][1] = terrain.getHeightTerrain(modelMatrixLE1[3][0], modelMatrixLE1[3][2]);
	glm::mat4 modelMatrixLE1Body = glm::mat4(modelMatrixLE1);
	modelMatrixLE1Body = glm::scale(modelMatrixLE1Body, glm::vec3(0.5, 0.5, 0.5));
	LE1ModelAnimate.render(modelMatrixLE1Body);

	modelMatrixLE2[3][1] = terrain.getHeightTerrain(modelMatrixLE2[3][0], modelMatrixLE2[3][2]);
	glm::mat4 modelMatrixLE2Body = glm::mat4(modelMatrixLE2);
	modelMatrixLE2Body = glm::scale(modelMatrixLE2Body, glm::vec3(0.5, 0.5, 0.5));
	LE2ModelAnimate.render(modelMatrixLE2Body);
	modelMatrixLE3[3][1] = terrain.getHeightTerrain(modelMatrixLE3[3][0], modelMatrixLE3[3][2]);
	glm::mat4 modelMatrixLE3Body = glm::mat4(modelMatrixLE3);
	modelMatrixLE3Body = glm::scale(modelMatrixLE3Body, glm::vec3(0.5, 0.5, 0.5));
	LE3ModelAnimate.render(modelMatrixLE3Body);
	modelMatrixLE4[3][1] = terrain.getHeightTerrain(modelMatrixLE4[3][0], modelMatrixLE4[3][2]);
	glm::mat4 modelMatrixLE4Body = glm::mat4(modelMatrixLE4);
	modelMatrixLE4Body = glm::scale(modelMatrixLE4Body, glm::vec3(0.5, 0.5, 0.5));
	LE4ModelAnimate.render(modelMatrixLE4Body);
	modelMatrixLE5[3][1] = terrain.getHeightTerrain(modelMatrixLE5[3][0], modelMatrixLE5[3][2]);
	glm::mat4 modelMatrixLE5Body = glm::mat4(modelMatrixLE5);
	modelMatrixLE5Body = glm::scale(modelMatrixLE5Body, glm::vec3(0.5, 0.5, 0.5));
	LE5ModelAnimate.render(modelMatrixLE5Body);
	modelMatrixLE6[3][1] = terrain.getHeightTerrain(modelMatrixLE6[3][0], modelMatrixLE6[3][2]);
	glm::mat4 modelMatrixLE6Body = glm::mat4(modelMatrixLE6);
	modelMatrixLE6Body = glm::scale(modelMatrixLE6Body, glm::vec3(0.5, 0.5, 0.5));
	LE6ModelAnimate.render(modelMatrixLE6Body);
	modelMatrixLE7[3][1] = terrain.getHeightTerrain(modelMatrixLE7[3][0], modelMatrixLE7[3][2]);
	glm::mat4 modelMatrixLE7Body = glm::mat4(modelMatrixLE7);
	modelMatrixLE7Body = glm::scale(modelMatrixLE7Body, glm::vec3(0.5, 0.5, 0.5));
	LE7ModelAnimate.render(modelMatrixLE7Body);
	modelMatrixLE8[3][1] = terrain.getHeightTerrain(modelMatrixLE8[3][0], modelMatrixLE8[3][2]);
	glm::mat4 modelMatrixLE8Body = glm::mat4(modelMatrixLE8);
	modelMatrixLE8Body = glm::scale(modelMatrixLE8Body, glm::vec3(0.5, 0.5, 0.5));
	LE8ModelAnimate.render(modelMatrixLE8Body);
	modelMatrixLE9[3][1] = terrain.getHeightTerrain(modelMatrixLE9[3][0], modelMatrixLE9[3][2]);
	glm::mat4 modelMatrixLE9Body = glm::mat4(modelMatrixLE9);
	modelMatrixLE9Body = glm::scale(modelMatrixLE9Body, glm::vec3(0.5, 0.5, 0.5));
	LE9ModelAnimate.render(modelMatrixLE9Body);
	modelMatrixLE10[3][1] = terrain.getHeightTerrain(modelMatrixLE10[3][0], modelMatrixLE10[3][2]);
	glm::mat4 modelMatrixLE10Body = glm::mat4(modelMatrixLE10);
	modelMatrixLE10Body = glm::scale(modelMatrixLE10Body, glm::vec3(0.5, 0.5, 0.5));
	LE10ModelAnimate.render(modelMatrixLE10Body);
	modelMatrixLE11[3][1] = terrain.getHeightTerrain(modelMatrixLE11[3][0], modelMatrixLE11[3][2]);
	glm::mat4 modelMatrixLE11Body = glm::mat4(modelMatrixLE11);
	modelMatrixLE11Body = glm::scale(modelMatrixLE11Body, glm::vec3(0.5, 0.5, 0.5));
	LE11ModelAnimate.render(modelMatrixLE11Body);
	modelMatrixLE12[3][1] = terrain.getHeightTerrain(modelMatrixLE12[3][0], modelMatrixLE12[3][2]);
	glm::mat4 modelMatrixLE12Body = glm::mat4(modelMatrixLE12);
	modelMatrixLE12Body = glm::scale(modelMatrixLE12Body, glm::vec3(0.5, 0.5, 0.5));
	LE12ModelAnimate.render(modelMatrixLE12Body);
	modelMatrixLE13[3][1] = terrain.getHeightTerrain(modelMatrixLE13[3][0], modelMatrixLE13[3][2]);
	glm::mat4 modelMatrixLE13Body = glm::mat4(modelMatrixLE13);
	modelMatrixLE13Body = glm::scale(modelMatrixLE13Body, glm::vec3(0.5, 0.5, 0.5));
	LE13ModelAnimate.render(modelMatrixLE13Body);
	modelMatrixLE14[3][1] = terrain.getHeightTerrain(modelMatrixLE14[3][0], modelMatrixLE14[3][2]);
	glm::mat4 modelMatrixLE14Body = glm::mat4(modelMatrixLE14);
	modelMatrixLE14Body = glm::scale(modelMatrixLE14Body, glm::vec3(0.5, 0.5, 0.5));
	LE14ModelAnimate.render(modelMatrixLE14Body);
	modelMatrixLE15[3][1] = terrain.getHeightTerrain(modelMatrixLE15[3][0], modelMatrixLE15[3][2]);
	glm::mat4 modelMatrixLE15Body = glm::mat4(modelMatrixLE15);
	modelMatrixLE15Body = glm::scale(modelMatrixLE15Body, glm::vec3(0.5, 0.5, 0.5));
	LE15ModelAnimate.render(modelMatrixLE15Body);
	modelMatrixLE16[3][1] = terrain.getHeightTerrain(modelMatrixLE16[3][0], modelMatrixLE16[3][2]);
	glm::mat4 modelMatrixLE16Body = glm::mat4(modelMatrixLE16);
	modelMatrixLE16Body = glm::scale(modelMatrixLE16Body, glm::vec3(0.5, 0.5, 0.5));
	LE16ModelAnimate.render(modelMatrixLE16Body);
	modelMatrixLE17[3][1] = terrain.getHeightTerrain(modelMatrixLE17[3][0], modelMatrixLE17[3][2]);
	glm::mat4 modelMatrixLE17Body = glm::mat4(modelMatrixLE17);
	modelMatrixLE17Body = glm::scale(modelMatrixLE17Body, glm::vec3(0.5, 0.5, 0.5));
	LE17ModelAnimate.render(modelMatrixLE17Body);
	modelMatrixLE18[3][1] = terrain.getHeightTerrain(modelMatrixLE18[3][0], modelMatrixLE18[3][2]);
	glm::mat4 modelMatrixLE18Body = glm::mat4(modelMatrixLE18);
	modelMatrixLE18Body = glm::scale(modelMatrixLE18Body, glm::vec3(0.5, 0.5, 0.5));
	LE18ModelAnimate.render(modelMatrixLE18Body);
	modelMatrixLE19[3][1] = terrain.getHeightTerrain(modelMatrixLE19[3][0], modelMatrixLE19[3][2]);
	glm::mat4 modelMatrixLE19Body = glm::mat4(modelMatrixLE19);
	modelMatrixLE19Body = glm::scale(modelMatrixLE19Body, glm::vec3(0.5, 0.5, 0.5));
	LE19ModelAnimate.render(modelMatrixLE19Body);
	modelMatrixLE20[3][1] = terrain.getHeightTerrain(modelMatrixLE20[3][0], modelMatrixLE20[3][2]);
	glm::mat4 modelMatrixLE20Body = glm::mat4(modelMatrixLE20);
	modelMatrixLE20Body = glm::scale(modelMatrixLE20Body, glm::vec3(0.5, 0.5, 0.5));
	LE20ModelAnimate.render(modelMatrixLE20Body);
	modelMatrixLE21[3][1] = terrain.getHeightTerrain(modelMatrixLE21[3][0], modelMatrixLE21[3][2]);
	glm::mat4 modelMatrixLE21Body = glm::mat4(modelMatrixLE21);
	modelMatrixLE21Body = glm::scale(modelMatrixLE21Body, glm::vec3(0.5, 0.5, 0.5));
	LE21ModelAnimate.render(modelMatrixLE21Body);
	modelMatrixLE22[3][1] = terrain.getHeightTerrain(modelMatrixLE22[3][0], modelMatrixLE22[3][2]);
	glm::mat4 modelMatrixLE22Body = glm::mat4(modelMatrixLE22);
	modelMatrixLE22Body = glm::scale(modelMatrixLE22Body, glm::vec3(0.5, 0.5, 0.5));
	LE22ModelAnimate.render(modelMatrixLE22Body);
	modelMatrixLE23[3][1] = terrain.getHeightTerrain(modelMatrixLE23[3][0], modelMatrixLE23[3][2]);
	glm::mat4 modelMatrixLE23Body = glm::mat4(modelMatrixLE23);
	modelMatrixLE23Body = glm::scale(modelMatrixLE23Body, glm::vec3(0.5, 0.5, 0.5));
	LE23ModelAnimate.render(modelMatrixLE23Body);
	modelMatrixLE24[3][1] = terrain.getHeightTerrain(modelMatrixLE24[3][0], modelMatrixLE24[3][2]);
	glm::mat4 modelMatrixLE24Body = glm::mat4(modelMatrixLE24);
	modelMatrixLE24Body = glm::scale(modelMatrixLE24Body, glm::vec3(0.5, 0.5, 0.5));
	LE24ModelAnimate.render(modelMatrixLE24Body);
	modelMatrixLE25[3][1] = terrain.getHeightTerrain(modelMatrixLE25[3][0], modelMatrixLE25[3][2]);
	glm::mat4 modelMatrixLE25Body = glm::mat4(modelMatrixLE25);
	modelMatrixLE25Body = glm::scale(modelMatrixLE25Body, glm::vec3(0.5, 0.5, 0.5));
	LE25ModelAnimate.render(modelMatrixLE25Body);
	modelMatrixLE26[3][1] = terrain.getHeightTerrain(modelMatrixLE26[3][0], modelMatrixLE26[3][2]);
	glm::mat4 modelMatrixLE26Body = glm::mat4(modelMatrixLE26);
	modelMatrixLE26Body = glm::scale(modelMatrixLE26Body, glm::vec3(0.5, 0.5, 0.5));
	LE26ModelAnimate.render(modelMatrixLE26Body);
	modelMatrixLE27[3][1] = terrain.getHeightTerrain(modelMatrixLE27[3][0], modelMatrixLE27[3][2]);
	glm::mat4 modelMatrixLE27Body = glm::mat4(modelMatrixLE27);
	modelMatrixLE27Body = glm::scale(modelMatrixLE27Body, glm::vec3(0.5, 0.5, 0.5));
	LE27ModelAnimate.render(modelMatrixLE27Body);
	modelMatrixLE28[3][1] = terrain.getHeightTerrain(modelMatrixLE28[3][0], modelMatrixLE28[3][2]);
	glm::mat4 modelMatrixLE28Body = glm::mat4(modelMatrixLE28);
	modelMatrixLE28Body = glm::scale(modelMatrixLE28Body, glm::vec3(0.5, 0.5, 0.5));
	LE28ModelAnimate.render(modelMatrixLE28Body);
	modelMatrixLE29[3][1] = terrain.getHeightTerrain(modelMatrixLE29[3][0], modelMatrixLE29[3][2]);
	glm::mat4 modelMatrixLE29Body = glm::mat4(modelMatrixLE29);
	modelMatrixLE29Body = glm::scale(modelMatrixLE29Body, glm::vec3(0.5, 0.5, 0.5));
	LE29ModelAnimate.render(modelMatrixLE29Body);
	modelMatrixLE30[3][1] = terrain.getHeightTerrain(modelMatrixLE30[3][0], modelMatrixLE30[3][2]);
	glm::mat4 modelMatrixLE30Body = glm::mat4(modelMatrixLE30);
	modelMatrixLE30Body = glm::scale(modelMatrixLE30Body, glm::vec3(0.5, 0.5, 0.5));
	LE30ModelAnimate.render(modelMatrixLE30Body);
	modelMatrixLE31[3][1] = terrain.getHeightTerrain(modelMatrixLE31[3][0], modelMatrixLE31[3][2]);
	glm::mat4 modelMatrixLE31Body = glm::mat4(modelMatrixLE31);
	modelMatrixLE31Body = glm::scale(modelMatrixLE31Body, glm::vec3(0.5, 0.5, 0.5));
	LE31ModelAnimate.render(modelMatrixLE31Body);
	modelMatrixLE32[3][1] = terrain.getHeightTerrain(modelMatrixLE32[3][0], modelMatrixLE32[3][2]);
	glm::mat4 modelMatrixLE32Body = glm::mat4(modelMatrixLE32);
	modelMatrixLE32Body = glm::scale(modelMatrixLE32Body, glm::vec3(0.5, 0.5, 0.5));
	LE32ModelAnimate.render(modelMatrixLE32Body);
	modelMatrixLE33[3][1] = terrain.getHeightTerrain(modelMatrixLE33[3][0], modelMatrixLE33[3][2]);
	glm::mat4 modelMatrixLE33Body = glm::mat4(modelMatrixLE33);
	modelMatrixLE33Body = glm::scale(modelMatrixLE33Body, glm::vec3(0.5, 0.5, 0.5));
	LE33ModelAnimate.render(modelMatrixLE33Body);
	modelMatrixLE34[3][1] = terrain.getHeightTerrain(modelMatrixLE34[3][0], modelMatrixLE34[3][2]);
	glm::mat4 modelMatrixLE34Body = glm::mat4(modelMatrixLE34);
	modelMatrixLE34Body = glm::scale(modelMatrixLE34Body, glm::vec3(0.5, 0.5, 0.5));
	LE34ModelAnimate.render(modelMatrixLE34Body);
	modelMatrixLE35[3][1] = terrain.getHeightTerrain(modelMatrixLE35[3][0], modelMatrixLE35[3][2]);
	glm::mat4 modelMatrixLE35Body = glm::mat4(modelMatrixLE35);
	modelMatrixLE35Body = glm::scale(modelMatrixLE35Body, glm::vec3(0.5, 0.5, 0.5));
	LE35ModelAnimate.render(modelMatrixLE35Body);
	modelMatrixLE36[3][1] = terrain.getHeightTerrain(modelMatrixLE36[3][0], modelMatrixLE36[3][2]);
	glm::mat4 modelMatrixLE36Body = glm::mat4(modelMatrixLE36);
	modelMatrixLE36Body = glm::scale(modelMatrixLE36Body, glm::vec3(0.5, 0.5, 0.5));
	LE36ModelAnimate.render(modelMatrixLE36Body);
	modelMatrixLE37[3][1] = terrain.getHeightTerrain(modelMatrixLE37[3][0], modelMatrixLE37[3][2]);
	glm::mat4 modelMatrixLE37Body = glm::mat4(modelMatrixLE37);
	modelMatrixLE37Body = glm::scale(modelMatrixLE37Body, glm::vec3(0.5, 0.5, 0.5));
	LE37ModelAnimate.render(modelMatrixLE37Body);
	modelMatrixLE38[3][1] = terrain.getHeightTerrain(modelMatrixLE38[3][0], modelMatrixLE38[3][2]);
	glm::mat4 modelMatrixLE38Body = glm::mat4(modelMatrixLE38);
	modelMatrixLE38Body = glm::scale(modelMatrixLE38Body, glm::vec3(0.5, 0.5, 0.5));
	LE38ModelAnimate.render(modelMatrixLE38Body);
	modelMatrixLE39[3][1] = terrain.getHeightTerrain(modelMatrixLE39[3][0], modelMatrixLE39[3][2]);
	glm::mat4 modelMatrixLE39Body = glm::mat4(modelMatrixLE39);
	modelMatrixLE39Body = glm::scale(modelMatrixLE39Body, glm::vec3(0.5, 0.5, 0.5));
	LE39ModelAnimate.render(modelMatrixLE39Body);
	
	/**********
	 * Update the position with alpha objects
	 */

	// Update Fantasma Rojo
	blendingUnsorted.find("fantasmaRojo")->second = glm::vec3(modelMatrixFantasmaRojo[3]);
	// Update Fantasma Rosa
	blendingUnsorted.find("fantasmaRosa")->second = glm::vec3(modelMatrixFantasmaRosa[3]);
	// Update Fantasma Cian
	blendingUnsorted.find("fantasmaCian")->second = glm::vec3(modelMatrixFantasmaCian[3]);
	// Update Fantasma Naranja
	blendingUnsorted.find("fantasmaNaranja")->second = glm::vec3(modelMatrixFantasmaNaranja[3]);

	// Update Grass
	glm::vec3 grassPosition = blendingUnsorted.find("grass")->second;
	grassPosition[1] = terrain.getHeightTerrain(grassPosition[0], grassPosition[2]);
	blendingUnsorted.find("grass")->second = grassPosition;

	/**********
	 * Sorter with alpha objects
	 */
	std::map<float, std::pair<std::string, glm::vec3>> blendingSorted;
	std::map<std::string, glm::vec3>::iterator itblend;
	for(itblend = blendingUnsorted.begin(); itblend != blendingUnsorted.end(); itblend++){
		float distanceFromView = glm::length(camera->getPosition() - itblend->second);
		blendingSorted[distanceFromView] = std::make_pair(itblend->first, itblend->second);
	}

	/**********
	 * Render de las transparencias
	 */
	glEnable(GL_BLEND);
	glDisable(GL_CULL_FACE);
	for(std::map<float, std::pair<std::string, glm::vec3> >::reverse_iterator it = blendingSorted.rbegin(); it != blendingSorted.rend(); it++){
		if (it->second.first.compare("grass") == 0) {
			glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
			modelGrass.setPosition(it->second.second);
			modelGrass.render();
		}
		else if (it->second.first.compare("fantasmaRojo") == 0) {
			glBlendFunc(GL_CONSTANT_COLOR, GL_DST_COLOR);
			glBlendEquation(GL_FUNC_ADD);
			glBlendColor(1.0, 0.0, 0.0, 1.0);
			//Fantasma Rojo
			glm::mat4 modelMatrixFantasmaRojoBlend = glm::mat4(modelMatrixFantasmaRojo);
			modelMatrixFantasmaRojoBlend[3][1] = terrain.getHeightTerrain(modelMatrixFantasmaRojoBlend[3][0], modelMatrixFantasmaRojoBlend[3][2]);
			modelMatrixFantasmaRojoBlend = glm::scale(modelMatrixFantasmaRojoBlend, glm::vec3(0.074, 0.074, 0.074));
			fantasmaModel.render(modelMatrixFantasmaRojoBlend);
		}
		else if (it->second.first.compare("fantasmaRosa") == 0) {
			glBlendFunc(GL_CONSTANT_COLOR, GL_DST_COLOR);
			glBlendEquation(GL_FUNC_ADD);
			glBlendColor(1.0, 0.078431, 0.576470, 1.0);
			//Fantasma Rosa
			glm::mat4 modelMatrixFantasmaRosaBlend = glm::mat4(modelMatrixFantasmaRosa);
			modelMatrixFantasmaRosaBlend[3][1] = terrain.getHeightTerrain(modelMatrixFantasmaRosaBlend[3][0], modelMatrixFantasmaRosaBlend[3][2]);
			modelMatrixFantasmaRosaBlend = glm::scale(modelMatrixFantasmaRosaBlend, glm::vec3(0.074, 0.074, 0.074));
			fantasmaModel.render(modelMatrixFantasmaRosaBlend);
		}
		else if (it->second.first.compare("fantasmaCian") == 0) {
			glBlendFunc(GL_CONSTANT_COLOR, GL_DST_COLOR);
			glBlendEquation(GL_FUNC_ADD);
			glBlendColor(0.0, 1.0, 1.0, 1.0);
			//Fantasma Cian
			glm::mat4 modelMatrixFantasmaCianBlend = glm::mat4(modelMatrixFantasmaCian);
			modelMatrixFantasmaCianBlend[3][1] = terrain.getHeightTerrain(modelMatrixFantasmaCianBlend[3][0], modelMatrixFantasmaCianBlend[3][2]);
			modelMatrixFantasmaCianBlend = glm::scale(modelMatrixFantasmaCianBlend, glm::vec3(0.074, 0.074, 0.074));
			fantasmaModel.render(modelMatrixFantasmaCianBlend);
		}
		else if (it->second.first.compare("fantasmaNaranja") == 0) {
			glBlendFunc(GL_CONSTANT_COLOR, GL_DST_COLOR);
			glBlendEquation(GL_FUNC_ADD);
			glBlendColor(1.0, 0.549019, 0.0, 1.0);
			//Fantasma Naranja
			glm::mat4 modelMatrixFantasmaNaranjaBlend = glm::mat4(modelMatrixFantasmaNaranja);
			modelMatrixFantasmaNaranjaBlend[3][1] = terrain.getHeightTerrain(modelMatrixFantasmaNaranjaBlend[3][0], modelMatrixFantasmaNaranjaBlend[3][2]);
			modelMatrixFantasmaNaranjaBlend = glm::scale(modelMatrixFantasmaNaranjaBlend, glm::vec3(0.074, 0.074, 0.074));
			fantasmaModel.render(modelMatrixFantasmaNaranjaBlend);
		}
		else if(renderParticles && it->second.first.compare("fountain") == 0){
			glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
			/**********
			* Init Render particles systems
			*/
			glm::mat4 modelMatrixParticlesFountain = glm::mat4(1.0);
			modelMatrixParticlesFountain = glm::translate(modelMatrixParticlesFountain, it->second.second);
			modelMatrixParticlesFountain[3][1] = terrain.getHeightTerrain(modelMatrixParticlesFountain[3][0], modelMatrixParticlesFountain[3][2]) + 0.36 * 10.0;
			modelMatrixParticlesFountain = glm::scale(modelMatrixParticlesFountain, glm::vec3(3.0, 3.0, 3.0));
			currTimeParticlesAnimation = TimeManager::Instance().GetTime();
			if(currTimeParticlesAnimation - lastTimeParticlesAnimation > 10.0)
				lastTimeParticlesAnimation = currTimeParticlesAnimation;
			//glDisable(GL_DEPTH_TEST);
			glDepthMask(GL_FALSE);
			// Set the point size
			glPointSize(10.0f);
			glActiveTexture(GL_TEXTURE0);
			glBindTexture(GL_TEXTURE_2D, textureParticleFountainID);
			shaderParticlesFountain.turnOn();
			shaderParticlesFountain.setFloat("Time", float(currTimeParticlesAnimation - lastTimeParticlesAnimation));
			shaderParticlesFountain.setFloat("ParticleLifetime", 3.5f);
			shaderParticlesFountain.setInt("ParticleTex", 0);
			shaderParticlesFountain.setVectorFloat3("Gravity", glm::value_ptr(glm::vec3(0.0f, -0.3f, 0.0f)));
			shaderParticlesFountain.setMatrix4("model", 1, false, glm::value_ptr(modelMatrixParticlesFountain));
			glBindVertexArray(VAOParticles);
			glDrawArrays(GL_POINTS, 0, nParticles);
			glDepthMask(GL_TRUE);
			//glEnable(GL_DEPTH_TEST)
			shaderParticlesFountain.turnOff();
			/**********
			* End Render particles systems
			*/
		}
		else if(renderParticles && it->second.first.compare("fire") == 0){
			glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
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
			glBindVertexArray(particleArray[1-drawBuf]);
			glVertexAttribDivisor(0,0);
			glVertexAttribDivisor(1,0);
			glVertexAttribDivisor(2,0);
			glDrawArrays(GL_POINTS, 0, nParticlesFire);
			glEndTransformFeedback();
			glDisable(GL_RASTERIZER_DISCARD);

			shaderParticlesFire.setInt("Pass", 2);
			glm::mat4 modelFireParticles = glm::mat4(1.0);
			modelFireParticles = glm::translate(modelFireParticles, it->second.second);
			modelFireParticles[3][1] = terrain.getHeightTerrain(modelFireParticles[3][0], modelFireParticles[3][2]);
			shaderParticlesFire.setMatrix4("model", 1, false, glm::value_ptr(modelFireParticles));

			shaderParticlesFire.turnOn();
			glActiveTexture(GL_TEXTURE0);
			glBindTexture(GL_TEXTURE_2D, textureParticleFireID);
			glDepthMask(GL_FALSE);
			glBindVertexArray(particleArray[drawBuf]);
			glVertexAttribDivisor(0,1);
			glVertexAttribDivisor(1,1);
			glVertexAttribDivisor(2,1);
			glDrawArraysInstanced(GL_TRIANGLES, 0, 6, nParticlesFire);
			glBindVertexArray(0);
			glDepthMask(GL_TRUE);
			drawBuf = 1 - drawBuf;
			shaderParticlesFire.turnOff();
			
			/****************************+
			 * Open AL sound data
			 */
			source1Pos[0] = modelFireParticles[3].x;
			source1Pos[1] = modelFireParticles[3].y;
			source1Pos[2] = modelFireParticles[3].z;
			alSourcefv(source[1], AL_POSITION, source1Pos);

			/**********
			 * End Render particles systems
			 */
		}

		else if (renderParticles && it->second.first.compare("antorcha") == 0) {
		glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
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
		glm::mat4 modelFireParticles = glm::mat4(1.0);
		modelFireParticles = glm::translate(modelFireParticles, it->second.second);
		//modelFireParticles[3][1] = terrain.getHeightTerrain(modelFireParticles[3][0], modelFireParticles[3][2]);
		modelFireParticles = glm::rotate(modelFireParticles, glm::radians(10.0f), glm::vec3(1, 0, 0));
		modelFireParticles = glm::scale(modelFireParticles, glm::vec3(0.00625, 0.7, 0.00625));;
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


		/****************************+
		 * Open AL sound data
		 */

		source1Pos[0] = modelFireParticles[3].x;
		source1Pos[1] = modelFireParticles[3].y;
		source1Pos[2] = modelFireParticles[3].z;
		alSourcefv(source[1], AL_POSITION, source1Pos);

		/**********
		 * End Render particles systems
		 */
		}

		else if (renderParticles && it->second.first.compare("antorcha2") == 0) {
		glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
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
		glm::mat4 modelFireParticles2 = glm::mat4(1.0);
		modelFireParticles2 = glm::translate(modelFireParticles2, it->second.second);
		//modelFireParticles[3][1] = terrain.getHeightTerrain(modelFireParticles[3][0], modelFireParticles[3][2]);
		modelFireParticles2 = glm::rotate(modelFireParticles2, glm::radians(10.0f), glm::vec3(1, 0, 0));
		modelFireParticles2 = glm::scale(modelFireParticles2, glm::vec3(0.00625, 0.7, 0.00625));;
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


		/****************************+
		 * Open AL sound data
		 */

		source1Pos[0] = modelFireParticles2[3].x;
		source1Pos[1] = modelFireParticles2[3].y;
		source1Pos[2] = modelFireParticles2[3].z;
		alSourcefv(source[1], AL_POSITION, source1Pos);

		/**********
		 * End Render particles systems
		 */
		}

		else if (renderParticles && it->second.first.compare("antorcha3") == 0) {
		glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
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
		glm::mat4 modelFireParticles3 = glm::mat4(1.0);
		modelFireParticles3 = glm::translate(modelFireParticles3, it->second.second);
		//modelFireParticles[3][1] = terrain.getHeightTerrain(modelFireParticles[3][0], modelFireParticles[3][2]);
		modelFireParticles3 = glm::rotate(modelFireParticles3, glm::radians(10.0f), glm::vec3(1, 0, 0));
		modelFireParticles3 = glm::scale(modelFireParticles3, glm::vec3(0.00625, 0.7, 0.00625));;
		shaderParticlesFire.setMatrix4("model", 1, false, glm::value_ptr(modelFireParticles3));

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


		/****************************+
		 * Open AL sound data
		 */

		source1Pos[0] = modelFireParticles3[3].x;
		source1Pos[1] = modelFireParticles3[3].y;
		source1Pos[2] = modelFireParticles3[3].z;
		alSourcefv(source[1], AL_POSITION, source1Pos);

		/**********
		 * End Render particles systems
		 */
		}


		else if (renderParticles && it->second.first.compare("antorcha4") == 0) {
		glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
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
		glm::mat4 modelFireParticles4 = glm::mat4(1.0);
		modelFireParticles4 = glm::translate(modelFireParticles4, it->second.second);
		//modelFireParticles[3][1] = terrain.getHeightTerrain(modelFireParticles[3][0], modelFireParticles[3][2]);
		modelFireParticles4 = glm::rotate(modelFireParticles4, glm::radians(10.0f), glm::vec3(1, 0, 0));
		modelFireParticles4 = glm::scale(modelFireParticles4, glm::vec3(0.00625, 0.7, 0.00625));;
		shaderParticlesFire.setMatrix4("model", 1, false, glm::value_ptr(modelFireParticles4));

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


		/****************************+
		 * Open AL sound data
		 */

		source1Pos[0] = modelFireParticles4[3].x;
		source1Pos[1] = modelFireParticles4[3].y;
		source1Pos[2] = modelFireParticles4[3].z;
		alSourcefv(source[1], AL_POSITION, source1Pos);

		/**********
		 * End Render particles systems
		 */
		}

	}

	/*******************************************
		 * FreeType text Rendering
	*******************************************/
	updateUI_Text();
	glDisable(GL_BLEND);
	glEnable(GL_CULL_FACE);
}

int main(int argc, char **argv) {
	init(800, 700, "Window GLFW", false);
	srand(time(NULL));
	applicationLoop();
	destroy();
	return 1;
}
