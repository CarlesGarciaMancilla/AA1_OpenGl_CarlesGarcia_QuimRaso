#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <glm.hpp>
#include <gtc/type_ptr.hpp>
#include <gtc/matrix_transform.hpp>
#include <iostream>
#include <string>
#include <fstream>
#include <vector>

#define WINDOW_WIDTH 800
#define WINDOW_HEIGHT 800



struct GameObject
{
	glm::vec3 position = glm::vec3(-0.65f, 0.f, 0.f);
	glm::vec3 rotation = glm::vec3(0.f);
	glm::vec3 scale = glm::vec3(1.0f);
	glm::vec3 forward = glm::vec3(0.f, 1.f, 0.f);
	float fVelocity = 1.f;
	float fAngularVelocity = 90.f;
	float fScaleVelocity = 1.f;

};

struct ShaderProgram {

	GLuint vertexShader = 0;
	GLuint geometryShader = 0;
	GLuint fragmentShader = 0;
};

std::vector<GLuint> compiledPrograms;
GLenum drawMode = GL_FILL;
bool drawFill = true;
bool drawCube = true;
bool drawOrto = true;
bool drawPiramide = true;
bool pause = false;
GameObject cube;
GameObject ortoedroObject;
GameObject piramideObject;

void Resize_Window(GLFWwindow* window, int iFrameBufferWidth, int iFrameBufferHeight) {

	//Definir nuevo tamaño del viewport
	glViewport(0, 0, iFrameBufferWidth, iFrameBufferHeight);

	glUniform2f(glGetUniformLocation(compiledPrograms[0], "windowSize"), iFrameBufferWidth, iFrameBufferHeight);

}

void ChangeMode()
{

	if (drawFill == true)
	{

		drawMode = GL_LINE;
		drawFill = false;

	}
	else
	{

		drawMode = GL_FILL;
		drawFill = true;

	}

}

void VelocityUp()
{
	//cubo
	cube.fVelocity *= 1.1f;
	cube.fAngularVelocity *= 1.1f;
	cube.fScaleVelocity *= 1.1f;
	//ortoedro
	ortoedroObject.fVelocity *= 1.1f;
	ortoedroObject.fAngularVelocity *= 1.1f;
	ortoedroObject.fScaleVelocity *= 1.1f;
	//piramide
	piramideObject.fVelocity *= 1.1f;
	piramideObject.fAngularVelocity *= 1.1f;
	piramideObject.fScaleVelocity *= 1.1f;
}

void VelocityDown()
{
	//cubo
	cube.fVelocity *= 0.9f;
	cube.fAngularVelocity *= 0.9f;
	cube.fScaleVelocity *= 0.9f;
	//ortoedro
	ortoedroObject.fVelocity *= 0.9f;
	ortoedroObject.fAngularVelocity *= 0.9f;
	ortoedroObject.fScaleVelocity *= 0.9f;
	//piramide
	piramideObject.fVelocity *= 0.9f;
	piramideObject.fAngularVelocity *= 0.9f;
	piramideObject.fScaleVelocity *= 0.9f;
}

float getCurrentTime() {
	float time = glfwGetTime();
	return time;
}

float getDeltaTime() {
	//Calculamos el deltaTime para las transformaciones
	static float lastFrameTime = 0.f;
	float currentFrameTime = getCurrentTime(); // Supongamos que esta función te da el tiempo actual en segundos.
	float deltaTime = currentFrameTime - lastFrameTime;
	lastFrameTime = currentFrameTime;
	return deltaTime;
}

void keyEvents(GLFWwindow* window, int key, int scancode, int action, int mods)
{


	if ((key == GLFW_KEY_SPACE && action == GLFW_PRESS))
	{

		pause = !pause;
		std::cout << pause
			<< std::endl;


	}

	if (pause)
	{
		return;
	}

	if ((key == GLFW_KEY_1 && action == GLFW_PRESS))
	{

		ChangeMode();

	}

	if ((key == GLFW_KEY_2 && action == GLFW_PRESS))
	{
		if (drawCube == true)
		{
			drawCube = false;
		}
		else
		{
			drawCube = true;
		}


	}

	if ((key == GLFW_KEY_3 && action == GLFW_PRESS))
	{

		if (drawOrto == true)
		{
			drawOrto = false;
		}
		else
		{
			drawOrto = true;
		}

	}

	if ((key == GLFW_KEY_4 && action == GLFW_PRESS))
	{
		if (drawPiramide == true)
		{
			drawPiramide = false;
		}
		else
		{
			drawPiramide = true;
		}


	}

	if ((key == GLFW_KEY_M && action == GLFW_PRESS))
	{
		VelocityUp();


	}

	if ((key == GLFW_KEY_N && action == GLFW_PRESS))
	{
		VelocityDown();


	}

}

glm::mat4 GenerateTranslationMatrix(glm::vec3 translation)
{
	return glm::translate(glm::mat4(1.0f), translation);

}

glm::mat4 GenerateRotationMatrix(glm::vec3 axis, float fDegrees)
{
	return glm::rotate(glm::mat4(1.0f), glm::radians(fDegrees), glm::normalize(axis));

}

glm::mat4 GenerateScaleMatrix(glm::vec3 scaleAxis)
{
	return glm::scale(glm::mat4(1.0f), scaleAxis);

}

//Funcion que devolvera una string con todo el archivo leido
std::string Load_File(const std::string& filePath) {

	std::ifstream file(filePath);

	std::string fileContent;
	std::string line;

	//Lanzamos error si el archivo no se ha podido abrir
	if (!file.is_open()) {
		std::cerr << "No se ha podido abrir el archivo: " << filePath << std::endl;
		std::exit(EXIT_FAILURE);
	}

	//Leemos el contenido y lo volcamos a la variable auxiliar
	while (std::getline(file, line)) {
		fileContent += line + "\n";
	}

	//Cerramos stream de datos y devolvemos contenido
	file.close();

	return fileContent;
}

GLuint LoadFragmentShader(const std::string& filePath) {

	// Crear un fragment shader
	GLuint fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);

	//Usamos la funcion creada para leer el fragment shader y almacenarlo 
	std::string sShaderCode = Load_File(filePath);
	const char* cShaderSource = sShaderCode.c_str();

	//Vinculamos el fragment shader con su código fuente
	glShaderSource(fragmentShader, 1, &cShaderSource, nullptr);

	// Compilar el fragment shader
	glCompileShader(fragmentShader);

	// Verificar errores de compilación
	GLint success;
	glGetShaderiv(fragmentShader, GL_COMPILE_STATUS, &success);

	//Si la compilacion ha sido exitosa devolvemos el fragment shader
	if (success) {

		return fragmentShader;

	}
	else {

		//Obtenemos longitud del log
		GLint logLength;
		glGetShaderiv(fragmentShader, GL_INFO_LOG_LENGTH, &logLength);

		//Obtenemos el log
		std::vector<GLchar> errorLog(logLength);
		glGetShaderInfoLog(fragmentShader, logLength, nullptr, errorLog.data());

		//Mostramos el log y finalizamos programa
		std::cerr << "Se ha producido un error al cargar el fragment shader:  " << errorLog.data() << std::endl;
		std::exit(EXIT_FAILURE);
	}
}


GLuint LoadGeometryShader(const std::string& filePath) {

	// Crear un vertex shader
	GLuint geometryShader = glCreateShader(GL_GEOMETRY_SHADER);

	//Usamos la funcion creada para leer el vertex shader y almacenarlo 
	std::string sShaderCode = Load_File(filePath);
	const char* cShaderSource = sShaderCode.c_str();

	//Vinculamos el vertex shader con su código fuente
	glShaderSource(geometryShader, 1, &cShaderSource, nullptr);

	// Compilar el vertex shader
	glCompileShader(geometryShader);

	// Verificar errores de compilación
	GLint success;
	glGetShaderiv(geometryShader, GL_COMPILE_STATUS, &success);

	//Si la compilacion ha sido exitosa devolvemos el vertex shader
	if (success) {

		return geometryShader;

	}
	else {

		//Obtenemos longitud del log
		GLint logLength;
		glGetShaderiv(geometryShader, GL_INFO_LOG_LENGTH, &logLength);

		//Obtenemos el log
		std::vector<GLchar> errorLog(logLength);
		glGetShaderInfoLog(geometryShader, logLength, nullptr, errorLog.data());

		//Mostramos el log y finalizamos programa
		std::cerr << "Se ha producido un error al cargar el vertex shader:  " << errorLog.data() << std::endl;
		std::exit(EXIT_FAILURE);
	}
}

GLuint LoadVertexShader(const std::string& filePath) {

	// Crear un vertex shader
	GLuint vertexShader = glCreateShader(GL_VERTEX_SHADER);

	//Usamos la funcion creada para leer el vertex shader y almacenarlo 
	std::string sShaderCode = Load_File(filePath);
	const char* cShaderSource = sShaderCode.c_str();

	//Vinculamos el vertex shader con su código fuente
	glShaderSource(vertexShader, 1, &cShaderSource, nullptr);

	// Compilar el vertex shader
	glCompileShader(vertexShader);

	// Verificar errores de compilación
	GLint success;
	glGetShaderiv(vertexShader, GL_COMPILE_STATUS, &success);

	//Si la compilacion ha sido exitosa devolvemos el vertex shader
	if (success) {

		return vertexShader;

	}
	else {

		//Obtenemos longitud del log
		GLint logLength;
		glGetShaderiv(vertexShader, GL_INFO_LOG_LENGTH, &logLength);

		//Obtenemos el log
		std::vector<GLchar> errorLog(logLength);
		glGetShaderInfoLog(vertexShader, logLength, nullptr, errorLog.data());

		//Mostramos el log y finalizamos programa
		std::cerr << "Se ha producido un error al cargar el vertex shader:  " << errorLog.data() << std::endl;
		std::exit(EXIT_FAILURE);
	}
}

//Función que dado un struct que contiene los shaders de un programa generara el programa entero de la GPU
GLuint CreateProgram(const ShaderProgram& shaders) {

	//Crear programa de la GPU
	GLuint program = glCreateProgram();

	//Verificar que existe un vertex shader y adjuntarlo al programa
	if (shaders.vertexShader != 0) {
		glAttachShader(program, shaders.vertexShader);
	}

	if (shaders.geometryShader != 0) {
		glAttachShader(program, shaders.geometryShader);
	}

	if (shaders.fragmentShader != 0) {
		glAttachShader(program, shaders.fragmentShader);
	}

	// Linkear el programa
	glLinkProgram(program);

	//Obtener estado del programa
	GLint success;
	glGetProgramiv(program, GL_LINK_STATUS, &success);

	//Devolver programa si todo es correcto o mostrar log en caso de error
	if (success) {

		//Liberamos recursos
		if (shaders.vertexShader != 0) {
			glDetachShader(program, shaders.vertexShader);
		}

		//Liberamos recursos
		if (shaders.geometryShader != 0) {
			glDetachShader(program, shaders.geometryShader);
		}

		//Liberamos recursos
		if (shaders.fragmentShader != 0) {
			glDetachShader(program, shaders.fragmentShader);
		}

		return program;
	}
	else {

		//Obtenemos longitud del log
		GLint logLength;
		glGetProgramiv(program, GL_INFO_LOG_LENGTH, &logLength);

		//Almacenamos log
		std::vector<GLchar> errorLog(logLength);
		glGetProgramInfoLog(program, logLength, nullptr, errorLog.data());

		std::cerr << "Error al linkar el programa:  " << errorLog.data() << std::endl;
		std::exit(EXIT_FAILURE);
	}
}

void main() {

	//Definir semillas del rand según el tiempo
	srand(static_cast<unsigned int>(time(NULL)));

	//Inicializamos GLFW para gestionar ventanas e inputs
	glfwInit();

	//Configuramos la ventana
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 4);
	glfwWindowHint(GLFW_RESIZABLE, GL_TRUE);

	//Inicializamos la ventana
	GLFWwindow* window = glfwCreateWindow(WINDOW_WIDTH, WINDOW_HEIGHT, "My Engine", NULL, NULL);

	//Asignamos función de callback para cuando el frame buffer es modificado
	glfwSetFramebufferSizeCallback(window, Resize_Window);

	//Definimos espacio de trabajo
	glfwMakeContextCurrent(window);

	//Permitimos a GLEW usar funcionalidades experimentales
	glewExperimental = GL_TRUE;

	//Activamos cull face
	//glEnable(GL_CULL_FACE);

	//Indicamos lado del culling
	//glCullFace(GL_BACK);

	//Inicializamos GLEW y controlamos errores
	if (glewInit() == GLEW_OK) {

		//Declarar instancia de GameObject


		//Declarar vec2 para definir el offset
		glm::vec2 offset = glm::vec2(0.f, 0.f);

		//Compilar shaders
		ShaderProgram Program;
		Program.vertexShader = LoadVertexShader("VertexShader.glsl");
		Program.geometryShader = LoadGeometryShader("GeometryShader.glsl");
		Program.fragmentShader = LoadFragmentShader("FragmentShader.glsl");

		ShaderProgram piramideProgram;
		piramideProgram.vertexShader = LoadVertexShader("VertexShader.glsl");
		piramideProgram.geometryShader = LoadGeometryShader("GeometryShader.glsl");
		piramideProgram.fragmentShader = LoadFragmentShader("PiramideFragmentShader.glsl");

		//Compilar programa
		compiledPrograms.push_back(CreateProgram(Program));
		compiledPrograms.push_back(CreateProgram(piramideProgram));

		//Definimos color para limpiar el buffer de color
		glClearColor(0.f, 0.f, 0.f, 1.f);

		GLuint vaoCubo, vaoPiramide, vboCubo, vboPiramide;


		////////////////////////////////////////////  CUBO  ////////////////////////////////////////////

		//Definimos cantidad de vao a crear y donde almacenarlos 
		glGenVertexArrays(1, &vaoCubo);

		//Indico que el VAO activo de la GPU es el que acabo de crear
		glBindVertexArray(vaoCubo);

		//Definimos cantidad de vbo a crear y donde almacenarlos
		glGenBuffers(1, &vboCubo);

		//Indico que el VBO activo es el que acabo de crear y que almacenará un array. Todos los VBO que genere se asignaran al último VAO que he hecho glBindVertexArray
		glBindBuffer(GL_ARRAY_BUFFER, vboCubo);

		//Posición X e Y del punto
		GLfloat cubo[] = {
			-0.125f,0.125f,-0.125f, //3
			0.125f,0.125f,-0.125f, //2
			-0.125f,-0.125f,-0.125f, //6
			0.125,-0.125f,-0.125f, //7
			0.125f,-0.125f,0.125f,//4
			0.125f,0.125f,-0.125f, //2
			0.125f,0.125f,0.125f,//0
			-0.125f,0.125f,-0.125f, //3
			-0.125f,0.125f,0.125f, //1
			-0.125f,-0.125f,-0.125f, //6
			-0.125,-0.125,0.125f, //5
			0.125f,-0.125f,0.125f,//4
			-0.125f,0.125f,0.125f, //1
			0.125f,0.125f,0.125f,//0
		};


		//Definimos modo de dibujo para cada cara
		glPolygonMode(GL_FRONT_AND_BACK, drawMode);

		//Ponemos los valores en el VBO creado
		glBufferData(GL_ARRAY_BUFFER, sizeof(cubo), cubo, GL_STATIC_DRAW);

		//Indicamos donde almacenar y como esta distribuida la información
		glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(GLfloat), (GLvoid*)0);

		//Indicamos que la tarjeta gráfica puede usar el atributo 0
		glEnableVertexAttribArray(0);

		//Desvinculamos VBO
		glBindBuffer(GL_ARRAY_BUFFER, 0);

		//Desvinculamos VAO
		glBindVertexArray(0);




		///////////////////////////////////////  ORTOEDRO  ////////////////////////////////////////////

		//Inicializamos la posicion y escala del otroedro
		ortoedroObject.position = glm::vec3(0.f);
		ortoedroObject.scale = glm::vec3(1.0f, 2.0f, 1.0f);

		//Definimos cantidad de vao a crear y donde almacenarlos 
		glGenVertexArrays(1, &vaoCubo);

		//Indico que el VAO activo de la GPU es el que acabo de crear
		glBindVertexArray(vaoCubo);

		//Definimos cantidad de vbo a crear y donde almacenarlos
		glGenBuffers(1, &vboCubo);

		//Indico que el VBO activo es el que acabo de crear y que almacenará un array. Todos los VBO que genere se asignaran al último VAO que he hecho glBindVertexArray
		glBindBuffer(GL_ARRAY_BUFFER, vboCubo);

		//Posición X e Y del punto
		GLfloat ortoedro[] = {
			-0.125f,0.125f,-0.125f, //3
			0.125f,0.125f,-0.125f, //2
			-0.125f,-0.125f,-0.125f, //6
			0.125,-0.125f,-0.125f, //7
			0.125f,-0.125f,0.125f,//4
			0.125f,0.125f,-0.125f, //2
			0.125f,0.125f,0.125f,//0
			-0.125f,0.125f,-0.125f, //3
			-0.125f,0.125f,0.125f, //1
			-0.125f,-0.125f,-0.125f, //6
			-0.125,-0.125,0.125f, //5
			0.125f,-0.125f,0.125f,//4
			-0.125f,0.125f,0.125f, //1
			0.125f,0.125f,0.125f,//0
		};

		//Definimos modo de dibujo para cada cara
		glPolygonMode(GL_FRONT_AND_BACK, drawMode);

		//Ponemos los valores en el VBO creado
		glBufferData(GL_ARRAY_BUFFER, sizeof(ortoedro), ortoedro, GL_STATIC_DRAW);

		//Indicamos donde almacenar y como esta distribuida la información
		glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(GLfloat), (GLvoid*)0);

		//Indicamos que la tarjeta gráfica puede usar el atributo 0
		glEnableVertexAttribArray(0);

		//Desvinculamos VBO
		glBindBuffer(GL_ARRAY_BUFFER, 0);

		//Desvinculamos VAO
		glBindVertexArray(0);



		////////////////////////////////////////// PIRAMIDE  ////////////////////////////////////////////


		//Inicializamos la posicion y escala de la piramide
		piramideObject.position = glm::vec3(0.65f, 0.f, 0.f);
		piramideObject.scale = glm::vec3(0.33f);

		//Definimos cantidad de vao a crear y donde almacenarlos 
		glGenVertexArrays(1, &vaoPiramide);

		//Indico que el VAO activo de la GPU es el que acabo de crear
		glBindVertexArray(vaoPiramide);

		//Definimos cantidad de vbo a crear y donde almacenarlos
		glGenBuffers(1, &vboPiramide);

		//Indico que el VBO activo es el que acabo de crear y que almacenará un array. Todos los VBO que genere se asignaran al último VAO que he hecho glBindVertexArray
		glBindBuffer(GL_ARRAY_BUFFER, vboPiramide);

		GLfloat piramide[] = {
			// Cuadrado base (en sentido antihorario)
			-0.5f, 0.0f, -0.5f,  
			 0.5f, 0.0f, -0.5f,  
			 0.5f, 0.0f,  0.5f,  
			-0.5f, 0.0f,  0.5f, 

			// Triángulo frontal
			-0.5f, 0.0f,  0.5f, 
			 0.5f, 0.0f,  0.5f, 
			 0.0f, 1.0f,  0.0f,  

			 // Triángulo derecho
			  0.5f, 0.0f,  0.5f,  
			  0.5f, 0.0f, -0.5f, 
			  0.0f, 1.0f,  0.0f,  

			  // Triángulo trasero
			   0.5f, 0.0f, -0.5f,  
			  -0.5f, 0.0f, -0.5f,  
			   0.0f, 1.0f,  0.0f,  

			   // Triángulo izquierdo
			   -0.5f, 0.0f, -0.5f,  
			   -0.5f, 0.0f,  0.5f,  
				0.0f, 1.0f,  0.0f   
		};


		//Definimos modo de dibujo para cada cara
		glPolygonMode(GL_FRONT_AND_BACK, drawMode);

		//Ponemos los valores en el VBO creado
		glBufferData(GL_ARRAY_BUFFER, sizeof(piramide), piramide, GL_STATIC_DRAW);

		//Indicamos donde almacenar y como esta distribuida la información
		glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(GLfloat), (GLvoid*)0);

		//Indicamos que la tarjeta gráfica puede usar el atributo 0
		glEnableVertexAttribArray(0);

		//Desvinculamos VBO
		glBindBuffer(GL_ARRAY_BUFFER, 0);

		//Desvinculamos VAO
		glBindVertexArray(0);
		
		float time = 0.f;
		float lasttime = 0.f;
		//Generamos el game loop
		while (!glfwWindowShouldClose(window)) {
			glfwSetKeyCallback(window, keyEvents);
			//definimos delta time 
			float deltaTime = getDeltaTime();	
			if (pause)
			{
				glfwSetKeyCallback(window, keyEvents);
				glfwPollEvents();
				lasttime = getCurrentTime();
				deltaTime = 0;	//como las transformaciones dependen de este valor, lo seteamos a 0	para pausar el movimiento		
			}			
			if (!pause) {
				//Calculamos el tiempo para el cambio de color del triangulo y que sea el pertinente despues de pausar
				time = getCurrentTime() - lasttime/2;	

			}
			
			glUseProgram(compiledPrograms[0]);

			//Asignar valores iniciales al programa
			glUniform2f(glGetUniformLocation(compiledPrograms[0], "windowSize"), WINDOW_WIDTH, WINDOW_HEIGHT);

			//Pulleamos los eventos (botones, teclas, mouse...)
			glfwPollEvents();

			//Limpiamos los buffers
			glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT | GL_STENCIL_BUFFER_BIT);

			////////////////////////////////////////////  CUBO  ////////////////////////////////////////////
			glPolygonMode(GL_FRONT_AND_BACK, drawMode);
			//Definimos que queremos usar el VAO con los puntos
			glBindVertexArray(vaoCubo);

			//Generar modelo de la matriz MVP
			glm::mat4 cubeModeMatrix = glm::mat4(1.0f);

			//Calculamos la nueva transformacion del cubo
			cube.position = cube.position + cube.forward * cube.fVelocity * deltaTime;
			cube.rotation = cube.rotation + glm::vec3(0.f, 1.f, 0.f) * cube.fAngularVelocity * deltaTime;


			//Invertimos direccion si se sale de los limites
			if (cube.position.y <= -0.875f) {
				cube.forward = glm::vec3(0.f, 1.f, 0.f);;
			}			
			else if (cube.position.y >= 0.875f) {
				cube.forward = glm::vec3(0.f, -1.f, 0.f);;
			}			

			///////////////////////////////////////  ORTOEDRO  ////////////////////////////////////////////

			//Genero una matriz de traslacion
			glm::mat4 cubeTranslationMatrix = GenerateTranslationMatrix(cube.position);
			glm::mat4 cubeRotationMatrix = GenerateRotationMatrix(glm::vec3(0.f, 1.f, 0.f), cube.rotation.y);
			glm::mat4 cubeScaleMatrix = GenerateScaleMatrix(cube.scale);

			//Aplicamos las matrices
			cubeModeMatrix = cubeTranslationMatrix * cubeModeMatrix * cubeRotationMatrix * cubeScaleMatrix;

			glUniformMatrix4fv(glGetUniformLocation(compiledPrograms[0], "transform"), 1, GL_FALSE, glm::value_ptr(cubeModeMatrix));


			//Definimos que queremos dibujar
			if (drawCube)
			{
				glDrawArrays(GL_TRIANGLE_STRIP, 0, 14);
			}

			//Dejamos de usar el VAO indicado anteriormente
			glBindVertexArray(0);

			//Definimos que queremos usar el VAO con los puntos
			glBindVertexArray(vaoCubo);

			//Generar modelo de la matriz MVP
			glm::mat4 ortoModeMatrix = glm::mat4(1.0f);

			//Calculamos la nueva transformacion del ortoedro
			ortoedroObject.position = ortoedroObject.position + ortoedroObject.forward * ortoedroObject.fVelocity * deltaTime;
			ortoedroObject.rotation = ortoedroObject.rotation + glm::vec3(0.f, 0.f, 1.f) * ortoedroObject.fAngularVelocity * deltaTime;
			ortoedroObject.scale = ortoedroObject.scale - (glm::vec3(0.f, 1.f, 0.f) * ortoedroObject.fScaleVelocity) * deltaTime;

			//Invertimos la escala cuando llega a sus maximos y minimos
			if (ortoedroObject.scale.y >= 2.f || ortoedroObject.scale.y <= 1.f)
			{
				ortoedroObject.fScaleVelocity = ortoedroObject.fScaleVelocity * -1.0f;
			}


			//Genero una matriz de traslacion
			glm::mat4 ortoRotationMatrix = GenerateRotationMatrix(glm::vec3(0.f, 0.f, 1.f), ortoedroObject.rotation.z);
			glm::mat4 ortoScaleMatrix = GenerateScaleMatrix(ortoedroObject.scale);
			//Aplicamos las matrices
			ortoModeMatrix = ortoModeMatrix * ortoRotationMatrix * ortoScaleMatrix;

			glUniformMatrix4fv(glGetUniformLocation(compiledPrograms[0], "transform"), 1, GL_FALSE, glm::value_ptr(ortoModeMatrix));

			//Definimos que queremos dibujar
			if (drawOrto)
			{
				glDrawArrays(GL_TRIANGLE_STRIP, 0, 14);
			}

			//Dejamos de usar el VAO indicado anteriormente
			glBindVertexArray(0);

			////////////////////////////////////////////  PIRAMIDE  ////////////////////////////////////////////
			
			//Indicar a la tarjeta GPU que programa debe usar
			glUseProgram(compiledPrograms[1]);

					
			//Asignar valores iniciales al programa			
			glBindVertexArray(vaoPiramide);
			glUniform1f(glGetUniformLocation(compiledPrograms[1], "time"), time);

			//Generar modelo de la matriz MVP
			glm::mat4 piramideModeMatrix = glm::mat4(1.0f);

			//Calculamos la nueva transformacion del piramide
			piramideObject.position = piramideObject.position + piramideObject.forward * piramideObject.fVelocity * deltaTime;
			piramideObject.rotation = piramideObject.rotation + glm::vec3(0.f, 1.f, 0.f) * piramideObject.fAngularVelocity * deltaTime;


			//Invertimos direccion si se sale de los limites
			if (piramideObject.position.y <= -0.8f) {
				piramideObject.forward = glm::vec3(0.f, 1.f, 0.f);;
			}
			else if (piramideObject.position.y >= 0.8f) {
				piramideObject.forward = glm::vec3(0.f, -1.f, 0.f);;
			}

			//Genero una matriz de traslacion
			glm::mat4 piramideTranslationMatrix = GenerateTranslationMatrix(piramideObject.position);
			glm::mat4 piramideRotationMatrix = GenerateRotationMatrix(glm::vec3(1.f, 1.f, 0.f), piramideObject.rotation.y);
			glm::mat4 piramideScaleMatrix = GenerateScaleMatrix(piramideObject.scale);
			//Aplicamos las matrices

			piramideModeMatrix = piramideTranslationMatrix * piramideRotationMatrix * piramideScaleMatrix * piramideModeMatrix;

			glUniformMatrix4fv(glGetUniformLocation(compiledPrograms[1], "transform"), 1, GL_FALSE, glm::value_ptr(piramideModeMatrix));

			//Definimos que queremos dibujar
			if (drawPiramide)
			{
				glDrawArrays(GL_TRIANGLE_STRIP, 0, 16);
			}

			//Dejamos de usar el VAO indicado anteriormente
			glBindVertexArray(0);


			//Cambiamos buffers
			glFlush();
			glfwSwapBuffers(window);
		}

		//Desactivar y eliminar programa
		glUseProgram(0);
		glDeleteProgram(compiledPrograms[0]);

	}
	else {
		std::cout << "Ha petao." << std::endl;
		glfwTerminate();
	}
	
	//Finalizamos GLFW
	glfwTerminate();

}