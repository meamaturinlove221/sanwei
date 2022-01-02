#include "misc.h"
#include <stdio.h>
#include"SOIL2.h"
#include<stdlib.h>

void printShaderInfoLog(GLuint obj)
{
	int infologLength = 0;
	int charsWritten = 0;
	char* infoLog;

	glGetShaderiv(obj, GL_INFO_LOG_LENGTH, &infologLength);

	if (infologLength > 0)
	{
		infoLog = (char*)malloc(infologLength);
		glGetShaderInfoLog(obj, infologLength, &charsWritten, infoLog); 
		printf("%s\n", infoLog);
		free(infoLog);
	}
}

//bufferType �����������VEO VBO==GL_ARRAY_BUFFER  size��ģ�Ͷ�������Ĵ�С   data���Ƕ�������   usage��GL_STATIC_DRAW /GL_DYNAMIC_DRAW/GL_STREAM_DRAW 
GLuint CreateBufferObject(GLenum bufferType, GLsizeiptr size, GLenum usage, void*data /* = nullptr */)
{
	//unsigned int ����һ������������
	GLuint object;
	glGenBuffers(1, &object); //���ɻ���������
	glBindBuffer(bufferType, object); //�󶨻���������
	glBufferData(bufferType, size, data, usage); //glBufferData��һ��ר���������û���������ݸ��Ƶ���ǰ�󶨻���ĺ���������������й���
	glBindBuffer(bufferType, 0); //��󻺳���,�ȵ���Ҫ������ɫ������ʱ�ٰ�ʹ��
	return object;
}

//��ȡ�ı����ݲ�����
char *LoadFileContent(const char*path)
{
	FILE* pFile = fopen(path, "rb");//rb:����ʽ��һ���������ļ�
	if (pFile)
	{
		fseek(pFile, 0, SEEK_END);
		int nLen = ftell(pFile);
		char*buffer = nullptr;
		if (nLen!=0)
		{
			buffer=new char[nLen + 1];
			rewind(pFile);
			fread(buffer, nLen, 1, pFile);
			buffer[nLen] = '\0';
		}
		else
		{
			printf("load file fail %s content len is 0\n", path);
		}
		fclose(pFile);
		return buffer;
	}
	else
	{
		printf("open file %s fail\n",path);
	}
	fclose(pFile);
	return nullptr;
}

//GLuint CreateTexture(const char* imgFileName)
//{
//	//����������� һЩ��������������
//	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);//����Ŀ����GL_TEXTURE_2D
//	//�Ż����� ���ö༶��Զ����
//	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
//	//glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
//
//	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT); //�ڶ���������Ҫ����ָ�����õ�ѡ����Ӧ�õ�������
//	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT); //���һ��������Ҫ���Ǵ���һ�����Ʒ�ʽ(Wrapping)���˴������ǰ����������趨�����Ʒ�ʽΪGL_MIRRORED_REPEAT��
//	return SOIL_load_OGL_texture(imgFileName, 0, 0, SOIL_FLAG_POWER_OF_TWO | SOIL_FLAG_INVERT_Y);
//}

GLuint CreateTexture(const char* imgFileName)
{
	GLuint texture;
	texture = SOIL_load_OGL_texture(
		imgFileName,
		SOIL_LOAD_AUTO,
		SOIL_CREATE_NEW_ID,
		SOIL_FLAG_INVERT_Y
	);

	if (texture == 0)
		return -1;
	//�������������Ҫ�󶨣���Ȼ��û�˰���������
	glBindTexture(GL_TEXTURE_2D, texture);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);

	return texture;
}

GLuint CreateGPUProgram(const char* vsShaderPath, const char* fsShaderPath)
{
	GLuint vsShader = glCreateShader(GL_VERTEX_SHADER);  //������������ɫ������ 
	GLuint fsShader = glCreateShader(GL_FRAGMENT_SHADER);
	const char* vsCode = LoadFileContent(vsShaderPath); //ָ���Ӧ����ɫ��GLSL�����ļ�
	const char* fsCode = LoadFileContent(fsShaderPath);
	glShaderSource(vsShader, 1, &vsCode, nullptr); //glShaderSource ����������ɫ���ַ�Դ�루ͨ�����������������͸����ǵ���ɫ������
	glShaderSource(fsShader, 1, &fsCode, nullptr);//ram -> vram
	glCompileShader(vsShader); //������Ǳ�����ɫ������
	glCompileShader(fsShader);
	printShaderInfoLog(vsShader);
	printShaderInfoLog(fsShader);
	GLuint program = glCreateProgram();
	//Ȼ�󸽼ӵ���ɫ��������
	glAttachShader(program, vsShader);
	glAttachShader(program, fsShader);
	//���Ӷ�����ɫ����ƬԪ��ɫ��
	glLinkProgram(program);
	glDetachShader(program, vsShader);
	glDetachShader(program, fsShader);
	//ɾ���ͷ���ɫ������
	glDeleteShader(vsShader);
	glDeleteShader(fsShader);
	return program;
}
