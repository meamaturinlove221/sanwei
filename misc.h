#pragma once
#include "glew.h"

#pragma comment(lib, "glew32.lib")

/*
	����buffer object
	@param  bufferType   buffer������
	@param  size         buffer��С
	@param  usage        GL_STATIC_DRAW
	@param  data         buffer������
	@retval  �����õ�buffer object
	//bufferType �����������IBO=��������GL_ELEMENT_ARRAY_BUFFER  VBO==GL_ARRAY_BUFFER  size��ģ�Ͷ�������Ĵ�С   data���Ƕ�������   usage��GL_STATIC_DRAW /GL_DYNAMIC_DRAW/GL_STREAM_DRAW
*/
GLuint CreateBufferObject(GLenum bufferType, GLsizeiptr size, GLenum usage, void*data = nullptr);

/*
	��ȡ�ļ�����
	@param  path   �ļ�·�����ļ���
	@retval  �ļ�����
*/
char *LoadFileContent(const char*path);

/*
	��������
	@param  imgFileName   ͼƬ��
	@retval  �����õ�����
*/
GLuint CreateTexture(const char* imgFileName);

/*
	����gpu program
	@param  vsShaderPath   vertex shader�ļ���
	@param  fsShaderPath   fragment shader�ļ���
	@retval  �����õ�gpu program
*/
GLuint CreateGPUProgram(const char* vsShaderPath, const char* fsShaderPath);