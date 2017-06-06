
#include "tjsCommHead.h"

#include "GLShaderUtil.h"
#include "GLTextureDrawing.h"

tTVPGLTextureDrawing::tTVPGLTextureDrawing() {}

bool tTVPGLTextureDrawing::InitializeShader() {
	const std::string vs = SHADER_SOURCE
	(
		attribute vec4 a_position;
		attribute vec2 a_texCoord;
		varying vec2 v_texCoord;
		void main()
		{
			gl_Position = a_position;
			v_texCoord = a_texCoord;
		}
	);
	const std::string fs = SHADER_SOURCE
	(
		precision mediump float;
		varying vec2 v_texCoord;
		uniform sampler2D s_texture;
		void main()
		{
			gl_FragColor = texture2D( s_texture, v_texCoord );
		}
	);
	mProgram = CompileProgram(vs, fs);
	if( !mProgram ) {
		return false;
	}

	// Get the attribute locations
	mPositionLoc = glGetAttribLocation(mProgram, "a_position");
	mTexCoordLoc = glGetAttribLocation(mProgram, "a_texCoord");

	// Get the sampler location
	mSamplerLoc = glGetUniformLocation(mProgram, "s_texture");

	return true;
}
void tTVPGLTextureDrawing::DestroyShader() {
	glDeleteProgram( mProgram );
}

void tTVPGLTextureDrawing::DrawTexture( GLuint tex, int x, int y,int w, int h, int sw, int sh ) {
	float left  =((float)x/(float)sw)*2.0f-1.0f;
	float top   =((float)y/(float)sh)*2.0f-1.0f;
	float right =((float)(x+w)/(float)sw)*2.0f-1.0f;
	float bottom=((float)(y+h)/(float)sh)*2.0f-1.0f;
	top   =-top;
	bottom=-bottom;
#if 0
	GLfloat vertices[] = {
        left,  top, 0.0f,  // Position 0
         0.0f,  0.0f,        // TexCoord 0
        left, bottom, 0.0f,  // Position 1
         0.0f,  1.0f,        // TexCoord 1
         right, bottom, 0.0f,  // Position 2
         1.0f,  1.0f,        // TexCoord 2
         right,  top, 0.0f,  // Position 3
         1.0f,  0.0f         // TexCoord 3
	};
#else
	GLfloat vertices[] = {
        -0.5f,  0.5f, 0.0f,  // Position 0
         0.0f,  0.0f,        // TexCoord 0
        -0.5f, -0.5f, 0.0f,  // Position 1
         0.0f,  1.0f,        // TexCoord 1
         0.5f, -0.5f, 0.0f,  // Position 2
         1.0f,  1.0f,        // TexCoord 2
         0.5f,  0.5f, 0.0f,  // Position 3
         1.0f,  0.0f         // TexCoord 3
	};
	/*
	GLfloat vertices[] = {
		-0.5f,  0.5f, 0.0f,  // Position 0
		0.0f,  1.0f,        // TexCoord 0
		-0.5f, -0.5f, 0.0f,  // Position 1
		0.0f,  0.0f,        // TexCoord 1
		0.5f, -0.5f, 0.0f,  // Position 2
		1.0f,  0.0f,        // TexCoord 2
		0.5f,  0.5f, 0.0f,  // Position 3
		1.0f,  1.0f         // TexCoord 3
	};
	*/
#endif
	GLushort indices[] = { 0, 1, 2, 0, 2, 3 };

	// Set the viewport
	// glViewport(0, 0, getWindow()->getWidth(), getWindow()->getHeight());

	// Clear the color buffer
	// glClear(GL_COLOR_BUFFER_BIT);

	// Use the program object
	glUseProgram(mProgram);

	// Load the vertex position
	glVertexAttribPointer(mPositionLoc, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(GLfloat), vertices);
	// Load the texture coordinate
	glVertexAttribPointer(mTexCoordLoc, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(GLfloat), vertices + 3);

	glEnableVertexAttribArray(mPositionLoc);
	glEnableVertexAttribArray(mTexCoordLoc);

	// Bind the texture
	glActiveTexture( GL_TEXTURE0 );
	glBindTexture( GL_TEXTURE_2D, tex );
	glUniform1i( mSamplerLoc, 0 );

	glDrawElements( GL_TRIANGLES, 6, GL_UNSIGNED_SHORT, indices );
}
