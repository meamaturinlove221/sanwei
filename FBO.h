#pragma once

#include"glew.h"
#include<vector>
#include<map>
#include<string>

/*
	Frame buffer object
	�˳�������Ҫ���ڻ�ȡ���ͼ 
*/
class FrameBufferObject {
public:
	GLuint mFrameBufferObject;
	GLint mPrevFrameBuffer;
	std::map<std::string, GLuint> mBuffers;
	std::vector<GLenum> mDrawBuffers;
public:
	/*
		���캯�� genһ��frame buffer object
	*/
	FrameBufferObject();

	/*
		��һ��colorbuffer, ���������map
		��attachment����vector��
		@param bufferName   colorBuffer������,  Ҳ����map�е�key
		@param attachment   GL_COLOR_ATTACHMENT
		@width  colorbuffer�Ŀ�ȣ� ��Ϊ��Ļ��ȼ���
		@height colorbuffer�ĸ߶�
	*/
	void AttachColorBuffer(const char* bufferName, GLenum attachment, int width, int height);

	/*
		��һ��depthbuffer, ���������map
		@param bufferName   depthBuffer������,  Ҳ����map�е�key
		@param width  ���
		@param height �߶�
	*/
	void AttachDepthBuffer(const char* bufferName, int width, int height);

	/*
		���ý�������framebuffer������֡���ݻ��Ƶ�attachment��
	*/
	void Finish();

	/*
		�󶨵�ǰframebufferΪ��framebuffer
		��յ�ǰframebuffer�Ļ�����
	*/
	void Bind();

	/*
		���
	*/
	void Unbind();

	/*
		��ȡ����ʹ�õ�buffer
		@param bufferName  buffer����     map��key
		@retval  GLuint   buffer������Ӧ��buffer    map��value
	*/
	GLuint GetBuffer(const char* bufferName);
};