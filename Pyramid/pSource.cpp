#include <iostream>         // Allow for input/output
#include <GL/glew.h>        // GLEW library
#include <GLFW/glfw3.h>     // GLFW library

// GLM Libraries
#include <glm/glm.hpp> 
#include <glm/gtc/matrix_transform.hpp> 
#include <glm/gtc/type_ptr.hpp> 

using namespace std;

int width, height;

// Function to draw pyramid
void draw()
{
	GLenum mode = GL_TRIANGLES;										
	GLsizei indices = 18;										// Number of indices					
	glDrawElements(mode, indices, GL_UNSIGNED_BYTE, nullptr);	// Primitives that share vertices (otherwise glDrawArray)

}

// Create and compile shaders
static GLuint CompileShader(const string& source, GLuint shaderType)
{
	// Create shader object
	GLuint shaderID = glCreateShader(shaderType);
	const char* src = source.c_str();

	// Attach source code to shader object
	glShaderSource(shaderID, 1, &src, nullptr);

	// Compile shader
	glCompileShader(shaderID);

	// Return shader ID
	return shaderID;

}

// Create program object to link shader objects 
static GLuint CreateShaderProgram(const string& vertexShader, const string& fragmentShader)
{
	// Compile vertex shader
	GLuint vertexShaderComp = CompileShader(vertexShader, GL_VERTEX_SHADER);

	// Compile fragment shader
	GLuint fragmentShaderComp = CompileShader(fragmentShader, GL_FRAGMENT_SHADER);

	// Create program object
	GLuint shaderProgram = glCreateProgram();

	// Attach compiled vertex and fragment shaders to program object
	glAttachShader(shaderProgram, vertexShaderComp);
	glAttachShader(shaderProgram, fragmentShaderComp);

	// Link shaders to create final executable shader program
	glLinkProgram(shaderProgram);

	// Delete vertex and fragment shaders
	glDeleteShader(vertexShaderComp);
	glDeleteShader(fragmentShaderComp);

	// Return shader Program
	return shaderProgram;

}


int main(void)
{
	// Set values for screen dimensions
	width = 640; height = 480;

	// Declare new window object
	GLFWwindow* window;

	// Initialize glfw library 
	if (!glfwInit())
		return -1;

	// Create GLFW window
	window = glfwCreateWindow(width, height, "3D Pyramid by Paul K.", NULL, NULL);

	// Terminate program if window is not created
	if (!window)
	{
		glfwTerminate();
		return -1;
	}

	// Make context current for calling thread
	glfwMakeContextCurrent(window);

	// Initialize GLEW
	if (glewInit() != GLEW_OK)
		cout << "Error!" << endl;


	// Define vertex data for triangles in a floating point array
	GLfloat vertices[] = {
		
		// Position attribtues x, y, z,
		// Color attributes r, g, b

		-0.5, -0.5, -0.5, // Vertex 0
		1.0, 0.0, 0.0, // Red

		-0.5, -0.5, 0.5, // Vertex 1
		0.0, 1.0, 0.0, // Green

		0.5, -0.5, 0.5,  // Vertex 2	
		0.0, 0.0, 1.0, // blue
	
		0.5, -0.5, -0.5,  // Vertex 3	
		1.0, 0.0, 1.0, // Purple

		0.0, 1.0, 0.0,  // Vertex 4
		1.0, 1.0, 0.0 // Yellow
	};

	// Define element indices (EBO works specifically with indices)
	GLubyte indices[] = {
		0, 1, 2, // Triangle 1
		0, 2, 3, // Triangle 2
		0, 3, 4, // Triangle 3
		0, 4, 1, // Triangle 4
		1, 4, 2, // trianglle 5
		3, 4, 2 // triangle 6
	};

	// Allows for depth comparisons and to update the depth buffer
	glEnable(GL_DEPTH_TEST);

	// Wireframe
	glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);

	// Create variables for reference
	GLuint VBO, EBO, VAO;

	// Create new Vertex Buffer Object, Vertex Array Object, and Element Buffer Object
	glGenBuffers(1, &VBO); // (# of buffers, where to store id for buffer when created)
	glGenBuffers(1, &EBO); 
	glGenVertexArrays(1, &VAO);

	// Activate VAO
	glBindVertexArray(VAO);

	// Select Vertex Buffer Object and Element Buffer Object
	glBindBuffer(GL_ARRAY_BUFFER, VBO); 
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO); 

	// Load/Bind vertex attributes into Vertex Buffer Object (type of buffer, size of attributes, data, how to draw on sceen)
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW); 

	// Load/Bind indices into Vertex Buffer Object (type of buffer, size of indices, data, how to draw on sceen)
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW); 

	// Tell GPU the arrangment (layout) of position attributes (index, size, type, normalized, stride, ptr/ofset)
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(GLfloat), (GLvoid*)0);

	// Tell GPU index (location) of attributes
	glEnableVertexAttribArray(0);

	// Tell GPU the arrangment (layout) of color attributes (index, size, type, normalized, stride, ptr)
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(GLfloat), (GLvoid*)(3 * sizeof(GLfloat)));

	// offset is number of components to get to an atttribute

	// Tell GPU index(location) of  indices
	glEnableVertexAttribArray(1);

	// Unbind VOA
	glBindVertexArray(0); 

	// Vertex shader source code
	string vertexShaderSource =
		"#version 330 core\n"						// Version of OpenGL
		"layout(location = 0) in vec4 vPosition;"	// Specify location of position attributes
		"layout(location = 1) in vec4 aColor;"		// Specify location of color attributes
		"out vec4 oColor;"							
		"uniform mat4 model;"
		"uniform mat4 view;"
		"uniform mat4 projection;"
		"void main()\n" // Entry point for shader
		"{\n"
		"gl_Position = projection * view * model * vPosition;"		// Output position coordinates
		"oColor = aColor;"
		"}\n";

	// Fragment shader source code
	string fragmentShaderSource =
		"#version 330 core\n"						// Version of OpenGL
		"in vec4 oColor;"													 
		"out vec4 fragColor;"						
		"void main()\n"
		"{\n"
		"fragColor = oColor;"						// Specify colors
		"}\n";

	// Create shader program
	GLuint shaderProgram = CreateShaderProgram(vertexShaderSource, fragmentShaderSource);

	// Render loop (infinite loop until user closes window)
	while (!glfwWindowShouldClose(window))
	{

		// Resize window and graphics simultaneously
		glfwGetFramebufferSize(window, &width, &height);
		glViewport(0, 0, width, height);

		// Set background color
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		// Use executable shader program and select VAO before drawing 
		glUseProgram(shaderProgram); // Call Shader per-frame when updating attributes

		// Declare identity matrix
		glm::mat4 modelMatrix = glm::mat4(1.0f);
		glm::mat4 viewMatrix = glm::mat4(1.0f);
		glm::mat4 projectionMatrix = glm::mat4(1.0f);

		// Initialize transforms
		modelMatrix = glm::translate(modelMatrix, glm::vec3(0.0f, 0.0f, 1.0f));
		modelMatrix = glm::rotate(modelMatrix, glm::degrees(5.0f), glm::vec3(0.0f, 1.0f, 0.0f));
		modelMatrix = glm::scale(modelMatrix, glm::vec3(1.0f, 1.0f, 1.0f));

		viewMatrix = glm::translate(viewMatrix, glm::vec3( 0.0f, 0.0f, -3.0f));
		viewMatrix = glm::rotate(viewMatrix, glm::degrees(0.0f), glm::vec3(1.0f, 0.0f, 0.0f));
		
		projectionMatrix = glm::perspective(45.0f, (GLfloat)width / (GLfloat)height, 0.1f, 100.0f);

		// Select uniform shader and variable
		GLuint modelLoc = glGetUniformLocation(shaderProgram, "model");
		GLuint viewLoc = glGetUniformLocation(shaderProgram, "view");
		GLuint projectionLoc = glGetUniformLocation(shaderProgram, "projection");

		// Pass transform to shader
		glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(modelMatrix));
		glUniformMatrix4fv(viewLoc, 1, GL_FALSE, glm::value_ptr(viewMatrix));
		glUniformMatrix4fv(projectionLoc, 1, GL_FALSE, glm::value_ptr(projectionMatrix));

		// Activate VBOs contained in VAO
		glBindVertexArray(VAO);

		// Call function to draw pyramid
		draw();

		// Deactivate VAO
		glBindVertexArray(0);

		// Deactivate program object
		glUseProgram(0);

		// Swap front and back buffers of window
		glfwSwapBuffers(window);

		// Process events
		glfwPollEvents();
	}

	// Delete Vertex Array Object, Vertex Buffer Object, and Element Buffer Object
	glDeleteVertexArrays(1, &VAO);
	glDeleteBuffers(1, &VBO);
	glDeleteBuffers(1, &EBO);

	// End program
	glfwTerminate();
	return 0;
}