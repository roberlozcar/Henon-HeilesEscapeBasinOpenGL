#include "auxiliar.h"
#include "PLANE.h"

#include <gl/glew.h>
#define SOLVE_FGLUT_WARNING
#include <gl/freeglut.h> 

#define GLM_FORCE_RADIANS
#include <glm/glm.hpp>

#include <iostream>

#include<chrono>

//Variables de la ventana
int WIDTH =512;
int HEIGHT = 512;

//////////////////////////////////////////////////////////////
// Variables que nos dan acceso a Objetos OpenGL
//////////////////////////////////////////////////////////////

//Shaders y program
unsigned int vshader; 
unsigned int fshader;
unsigned int program;

unsigned int cshader;
unsigned int program2;

//Atributos (identificadores)
int inPos;
unsigned int ucolortex;
unsigned int ucolcom;

//VAO 
unsigned int vao;

//VBOs que forman parte del objeto 
unsigned int posVBO;

unsigned int upos;
unsigned int uvel;
unsigned int usize;

glm::ivec2 wgs = glm::ivec2(32);
glm::ivec2 size;

float totaltime = 0;
int countframes = 0;

unsigned int uproj;

//////////////////////////////////////////////////////////////
// Funciones auxiliares
//////////////////////////////////////////////////////////////

//Declaración de CB
void renderFunc();
void resizeFunc(int width, int height);
void idleFunc();
void keyboardFunc(unsigned char key, int x, int y);
void mouseFunc(int button, int state, int x, int y);
void mouseMotionFunc(int, int);

//Funciones de inicialización y destrucción
void initContext(int argc, char** argv);
void initOGL();
void initShader(const char *vname, const char *fname);
void initShader2(const char* name);
void initObj();
void initObj2();
void destroy();

//Carga el shader indicado, devuele el ID del shader
//!Por implementar
GLuint loadShader(const char *fileName, GLenum type);

//Crea una textura, la configura, la sube a OpenGL, 
//y devuelve el identificador de la textura 
unsigned int loadTex(const char *fileName);

bool saving = true; 

int main(int argc, char** argv)
{
	//std::locale::global(std::locale("spanish"));// acentos ;)

	initContext(argc, argv);
	initOGL();
	initShader("../shaders/shader.v0.vert", "../shaders/shader.v0.frag");
	initShader2("../shaders/shader.v0.comp");

	initObj();
	initObj2();

	glutMainLoop();		// bucle de eventos

	destroy();

	return 0;
}
	
//////////////////////////////////////////
// Funciones auxiliares 
void initContext(int argc, char** argv){

	// Definimos el contexto
	glutInit(&argc, argv);							// Inicializa Glut
	glutInitContextVersion(4, 3);					// Indicamos la version de OpenGL
	glutInitContextProfile(GLUT_CORE_PROFILE);		// Queremos un contexto sin compabilidad hacia atras

	// Definimos el Frame Buffer y creamos la ventana
	glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGBA | GLUT_DEPTH);	// Frame Buffer por defecto
	glutInitWindowSize(WIDTH, HEIGHT);								// Tamaño de la ventana
	glutInitWindowPosition(0, 0);								// Posicion de la ventana
	glutCreateWindow("Cuenca HH");							// Crea la ventana

	// Inicializamos las extensiones de OpenGL
	glewExperimental = GL_TRUE;
	GLenum err = glewInit(); 
	if (GLEW_OK != err) 
	{ 
		std::cout << "Error: " << glewGetErrorString(err) << std::endl; 
		exit(-1); 
	} 

	// Comprobamos la version de OpenGL
	const GLubyte* oglVersion = glGetString(GL_VERSION);  
	std::cout << "This system supports OpenGL Version: " << oglVersion << std::endl;

	// Le indicamos a Glut que funciones hay que ejecutar cuando se dan ciertos eventos
	glutReshapeFunc(resizeFunc);		// redimension de la ventana
	glutDisplayFunc(renderFunc);		// renderizado
	glutIdleFunc(idleFunc);				// cuando el procesador esta ocioso
	glutKeyboardFunc(keyboardFunc);		// evento de teclado
	glutMouseFunc(mouseFunc);			// evento de raton
	glutMotionFunc(mouseMotionFunc);	// control de la camara con el raton
}

void initOGL()
{
	glDisable(GL_DEPTH_TEST);					
	glClearColor(0.f, 0.f, 0.f, 1.0f);		

	glPolygonMode(GL_FRONT, GL_FILL);	
	glDisable(GL_CULL_FACE);			

}

void destroy()
{
	// LIBERAMOS RECURSOS

	// buffers
	glDeleteBuffers(1, &posVBO);
	glDeleteVertexArrays(1, &vao);

	// shaders y programa 1
	glDetachShader(program, vshader); 
	glDetachShader(program, fshader);
	glDeleteShader(vshader); 
	glDeleteShader(fshader);
	glDeleteProgram(program);

	//shaders y program 2
	glDetachShader(program2, cshader);
	glDeleteProgram(program2);
}

void initShader(const char *vname, const char *fname)
{
	// Cargamos los shaders de vertices y fragmentos
	vshader = loadShader(vname, GL_VERTEX_SHADER); 
	fshader = loadShader(fname, GL_FRAGMENT_SHADER);

	// Enlazamos los shaders en un programa
	program = glCreateProgram();			// crea el programa
	glAttachShader(program, vshader);		// asigna el shader de vertices
	glAttachShader(program, fshader);		// asigna el shader de fragmentos

	glLinkProgram(program);					// enlaza

	// Comprobamos si hay error al enlazar
	int linked; 
	glGetProgramiv(program, GL_LINK_STATUS, &linked);	// comprueba el status de linkado
	if (!linked) 
	{ 
		//Calculamos una cadena de error 
		GLint logLen; 
		glGetProgramiv(program, GL_INFO_LOG_LENGTH, &logLen);
		char *logString = new char[logLen]; 
		glGetProgramInfoLog(program, logLen, NULL, logString); 
		std::cout << "Error: " << logString << std::endl; 
		delete[] logString; 
		exit (-1); 
	}

	// Creamos los idenficadores de los atributos
	inPos = glGetAttribLocation(program, "inPos"); 
	ucolortex = glGetUniformLocation(program, "color");
}

void initShader2(const char* name) {
	// Cargamos los shaders de vertices y fragmentos
	cshader = loadShader(name, GL_COMPUTE_SHADER);

	//Enlazamos los shaders en un programa
	program2 = glCreateProgram();
	glAttachShader(program2, cshader);
	glLinkProgram(program2);

	// Comprobamos si hay error al enlazar
	int linked;
	glGetProgramiv(program2, GL_LINK_STATUS, &linked);
	if (!linked)
	{
		//Calculamos una cadena de error
		GLint logLen;
		glGetProgramiv(program2, GL_INFO_LOG_LENGTH, &logLen);
		char* logString = new char[logLen];
		glGetProgramInfoLog(program2, logLen, NULL, logString);
		std::cout << "Error: " << logString << std::endl;
		delete[] logString;
		exit(-1);
	}
	ucolcom = glGetUniformLocation(program2, "color");
	usize= glGetUniformLocation(program2, "size");
}

void initObj()
{
	glGenVertexArrays(1, &vao);			// crea el VAO
	glBindVertexArray(vao);				// activa el VAO

	glGenBuffers(1, &posVBO);
	
	glBindBuffer(GL_ARRAY_BUFFER, posVBO);
	glBufferData(GL_ARRAY_BUFFER, planeNVertex * sizeof(float) * 2, planeVertexPos, GL_STATIC_DRAW);

	// Lo enlazamos a la variable inPos (location = 0)
	glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 0, 0);
	glEnableVertexAttribArray(0);
}

void initObj2() {

	size = glm::ivec2(WIDTH, HEIGHT);

	glGenTextures(1, &ucolcom);
	glBindTexture(GL_TEXTURE_2D, ucolcom);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA32F, WIDTH, HEIGHT, 0, GL_RGBA, GL_FLOAT, 0);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	glBindImageTexture(0, ucolcom, 0, GL_FALSE, 0, GL_READ_WRITE, GL_RGBA32F);
}

GLuint loadShader(const char *fileName, GLenum type)
{ 
	unsigned int fileLen; 
	char* source = loadStringFromFile(fileName, fileLen);		// carga el codigo del shader

	////////////////////////////////////////////// 
	//Creación y compilación del Shader 
	GLuint shader;												 
	shader = glCreateShader(type);								// crea un ID para el shader
	glShaderSource(shader, 1, 
		(const GLchar **)&source, (const GLint *)&fileLen);		// le asigna el codigo del shader
	glCompileShader(shader);									// compila el shader
	delete[] source;											// libera espacio

	//Comprobamos que se compiló bien 
	GLint compiled; 
	glGetShaderiv(shader, GL_COMPILE_STATUS, &compiled); 
	if (!compiled) { 
		//Calculamos una cadena de error 
		GLint logLen; 
		glGetShaderiv(shader, GL_INFO_LOG_LENGTH, &logLen); 
		char *logString = new char[logLen]; 
		glGetShaderInfoLog(shader, logLen, NULL, logString); 
		std::cout << "Error: " << logString << std::endl; 
		delete[] logString; 
		exit (-1); 
	}

	return shader; 
}

unsigned int loadTex(const char *fileName)
{ 
	unsigned char* map; 
	unsigned int w, h; 

	// cargamos la textura en map
	map = loadTexture(fileName, w, h); 

	// comprobacion de que ha cargado
	if (!map) 
	{ 
		std::cout << "Error cargando el fichero: " << fileName << std::endl; 
		exit(-1); 
	}

	// creamos el id de la textura, la activamos y la subimos a la tarjeta grafica
	unsigned int texId; 
	glGenTextures(1, &texId);										// crea
	glBindTexture(GL_TEXTURE_2D, texId);							// activa	
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA8, w, h, 0, GL_RGBA,		// sube a la grafica
		GL_UNSIGNED_BYTE, (GLvoid*)map);

	// generamos el resto de niveles
	glGenerateMipmap(GL_TEXTURE_2D);

	GLfloat fLargest;
	if (glewIsSupported("GL_EXT_texture_filter_anisotropic")) {
		glGetFloatv(GL_MAX_TEXTURE_MAX_ANISOTROPY_EXT, &fLargest);
		glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MAX_ANISOTROPY_EXT, fLargest);
	}

	// configuramos el acceso
	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR); // tengo menos texeles
	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);				// tengo mas texeles
	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);			// me paso de la coordenada X
	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);			// me paso de la coordenada Y

	// liberamos memoria
	delete[] map;

	return texId;	// devolvemos el ID de la textura
}

void renderFunc()
{
	glClear(GL_COLOR_BUFFER_BIT);


	glUseProgram(program2);	
	glActiveTexture(GL_TEXTURE0);
	glUniform1i(ucolcom, 0);
	if(usize!=-1)
		glUniform2iv(usize, 1,&size[0]);

	auto start0 = std::chrono::system_clock::now();
	// Se lanzan los compute shaders
	glDispatchCompute((size.x - 1) / wgs.x + 1, (size.y - 1) / wgs.y + 1, 1);

	glMemoryBarrier(GL_SHADER_IMAGE_ACCESS_BARRIER_BIT);
	
	auto end0 = std::chrono::system_clock::now();

	glUseProgram(program);
	glUniform1i(ucolortex, 0);

	// Activamos el VAO
	glBindVertexArray(vao);

	auto start1 = std::chrono::system_clock::now();

	// Se renderiza el sistema de particulas
	glDrawArrays(GL_TRIANGLE_STRIP, 0, 4);

	auto end1 = std::chrono::system_clock::now();

	if (saving)
		saveimage(WIDTH, HEIGHT);

	glutSwapBuffers();

	// Se calculan los tiempos de renderizado y computo y el numero de frames por segundo
	std::chrono::duration<float, std::milli> duration1 = end1 - start1;
	std::chrono::duration<float, std::milli> duration0 = end0 - start0;
	totaltime += duration1.count()+duration0.count();
	countframes++;
	std::cout << duration0.count() << " s compute" << std::endl;
	std::cout << duration1.count() << " s render" << std::endl;
	std::cout << countframes / totaltime << " mean total FPS " << std::endl;

}

void resizeFunc(int width, int height)
{
	WIDTH = width;
	HEIGHT = height;

	size = glm::vec2(width, height);
	glBindTexture(GL_TEXTURE_2D, ucolcom);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA32F, width, height, 0, GL_RGBA, GL_FLOAT, 0);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	glBindImageTexture(0, ucolcom, 0, GL_FALSE, 0, GL_READ_WRITE, GL_RGBA32F);

	// transformamos coordenadas normalizadas a coordenadas de pixeles
	glViewport(0,0,width,height);	
	glutPostRedisplay();
}


void idleFunc()
{
	//glutPostRedisplay();
}

void keyboardFunc(unsigned char key, int x, int y){

	
}

void mouseFunc(int button, int state, int x, int y){
	
}

void mouseMotionFunc(int x, int y)
{
	
}