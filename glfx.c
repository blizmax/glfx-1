#include <stdio.h>
#include <malloc.h>
#include <assert.h>
#include <GL/gl.h>

#define GLFX_ATTR_VPOSITION 0

static GLuint glfxProgramObject;

static int glfxCompileShader(GLuint *shader, GLenum type, const char *shaderStr) { 
	GLint shaderCompiled;

	*shader = glCreateShader(type);
	glShaderSource(*shader, 1, &shaderStr, NULL);

	glCompileShader(*shader);
	glGetShaderiv(*shader, GL_COMPILE_STATUS, &shaderCompiled);

	if (!shaderCompiled) {
		GLint infoLen = 0;

		glGetShaderiv(*shader, GL_INFO_LOG_LENGTH, &infoLen);

		if (infoLen > 1) {
			char *infoLog = malloc(infoLen);

			glGetShaderInfoLog(*shader, infoLen, NULL, infoLog);
			fprintf(stderr, "Error compiling shader.\n%s", infoLog);

			free(infoLog);
		}

		return 1;
	}

	return 0;
}

static int glfxLinkProgram(GLuint fragShader, GLuint vertShader) {
	GLint programLinked;

	glfxProgramObject = glCreateProgram();
	glAttachShader(glfxProgramObject, fragShader);
	glAttachShader(glfxProgramObject, vertShader);
	glBindAttribLocation(glfxProgramObject, GLFX_ATTR_VPOSITION, "vPosition");
	glLinkProgram(glfxProgramObject);
	glGetProgramiv(glfxProgramObject, GL_LINK_STATUS, &programLinked);

	if (!programLinked) {
		/* Error stuff */
		assert(0);
	}

	return 0;
}

static void glfxMakeProgram(const GLchar *fragShaderStr) {
	// global GLuint glfxProgramObject;
	const char *vertShaderStr = "attribute vec4 vPosition; void main() { gl_Position = vPosition; }";
	GLuint fragShader, vertShader;

	glfxCompileShader(&fragShader, GL_FRAGMENT_SHADER, fragShaderStr);
	glfxCompileShader(&vertShader, GL_VERTEX_SHADER, vertShaderStr);
	glfxLinkProgram(fragShader, vertShader);
}

/*
Background coords:

  (X0, Y0)+-------------+(X1, Y0)
	  |             |
	  |             |
	  |             |
	  |             |
  (X0, Y1)+-------------+(X1, Y1)

  (Depth Z)

*/

#define X0 -1.0f
#define X1 +1.0f
#define Y0 -1.0f
#define Y1 +1.0f
#define Z   0.5f
static void glfxPlaceVertices(void) {
	static GLfloat vertices[] = { X0, Y0, Z,
	                              X0, Y1, Z,
	                              X1, Y0, Z,
	                              X1, Y1, Z };

	glVertexAttribPointer(GLFX_ATTR_VPOSITION, 3, GL_FLOAT, GL_FALSE, 0, vertices);
	glEnableVertexAttribArray(GLFX_ATTR_VPOSITION);
}

void glfxSetup(const GLchar *fragShaderStr) {
	// global GLuint glfxProgramObject;
	glfxMakeProgram(fragShaderStr);
	glUseProgram(glfxProgramObject);
	glfxPlaceVertices();
}

GLint glfxGetUniformLocation(const GLchar *name) {
	// global GLuint glfxProgramObject;
	return glGetUniformLocation(glfxProgramObject, name);
}

void glfxRedraw(void) {
	glDrawArrays(GL_TRIANGLE_STRIP, 0, 4);
}
