#pragma once
#define NDEBUG 
#include<GL/glut.h>
#include"model.h"
#include"Glm/glm.hpp" //OpenGL Mathematics ����OpenGL��ɫ���ԣ�GLSL���淶��ͼ�������ͷ�ļ�C ++��ѧ�⡣
#include"Glm/ext.hpp"
#include<vector>

//   ����ȫ�ֱ���

// matrix
glm::mat4 modelMatrix;
glm::mat4 viewMatrix;
glm::mat4 projectionMatrix;
glm::mat4 normalMatrix;
glm::mat4 lightProjectionMatrix;
glm::mat4 lightViewMatrix;

//  texture and program

GLuint livingRoomWallPosLoc, livingRoomWallTexcoordLoc, livingRoomWallNormalLoc;
GLuint livingRoomWallMLoc, livingRoomWallVLoc, livingRoomWallPLoc, livingRoomWallNMLoc, livingRoomWallViewPosLocation;
GLuint livingRoomWallLightVMatrixLoc, livingRoomWallLightPMatrixLoc;
GLuint livingRoomWallLightOnLoc;
GLuint livingRoomWallTextureLoc, livingRoomShadowMapLoc;

std::vector<GLuint> paintings;//�����һ�����
std::vector<GLuint> walls;//����ǽ������
std::vector<GLuint> floors;//�����ذ�����
std::vector<GLuint> otherPaintings;//�������߹һ�����
int floorsSize;//vector�ĳ���
int wallsSize;
int paintingsSize;
int OtherPaintingsSize;
GLuint floorTexture = 0;//vector�ĵڼ�������
GLuint wallTexture = 0;
GLuint paintTexture = 0;
GLuint otherPaintTexture = 0;


GLuint livingRoomWallTexture; //����ǽ���������
GLuint roomTexture, shadowMap;  //�����������Ӱ
GLuint renderProgram, depthProgram, livingRoomWallProgram;
GLuint flowerpotTexture, parterreTexture;

// ��Ⱦ���ò���
GLuint renderPosLoc, renderTexcoordLoc, renderNormalLoc;
GLuint renderMLoc, renderVLoc, renderPLoc, renderNMLoc, renderViewPosLocation;
GLuint renderLightVMatrixLoc, renderLightPMatrixLoc;
GLuint renderLightOnLoc;
GLuint renderTextureLoc, shadowMapLoc;

GLfloat rendersunlight;

GLuint depthPosLoc, depthTexcoordLoc, depthNormalLoc;
GLuint depthMLoc, depthVLoc, depthPLoc;


//----------------------------------------------------
//����ģ����ر���
//----------------------------------------------------
//����


//ǽ��
VertexData* wallVertexData;
int wallVertexCount, wallIndexCount;
unsigned int* wallIndexes;
GLuint wallVbo, wallIbo;

//�ذ�
VertexData* floorVertexData;
int floorVertexCount, floorIndexCount;
unsigned int* floorIndexes;
GLuint floorVbo, floorIbo;
//����
VertexData* roomVertexData;
int roomVertexCount, roomIndexCount;
unsigned int* roomIndexes;
GLuint roomVbo, roomIbo;

//����  ǽ��--��1
VertexData* pVertexData;
int pVertexCount, pIndexCount;
unsigned int* pIndexes;
GLuint pVbo, pIbo;

//����  ǽ��--��2
VertexData* p2VertexData;
int p2VertexCount, p2IndexCount;
unsigned int* p2Indexes;
GLuint p2Vbo, p2Ibo;



//----------------------------------------
//ģ����ر����������
//----------------------------------------

//----------------------------------------
//�����������ر���
//----------------------------------------
glm::vec3 cameraPos;//�����λ��
glm::vec3 cameraCenter;
glm::vec3 cameraForward;

glm::vec3 cameraTarget;//ԭ��λ��
glm::vec3 cameraDirection;//���������
glm::vec3 up; //������
glm::vec3 cameraRight;//������
glm::vec3 cameraUp;//������
float cameraSpeed = 1.25f; //�ƶ��ٶ�
float fov = 60.0f; //������ӽǣ�ͨ�����������֣��Ӷ��������Ӽ�С�ӽǣ������Ŵ���С��Ч����
//----------------------------------------
//�������ر����������
//----------------------------------------