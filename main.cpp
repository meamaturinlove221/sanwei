#include"misc.h"
#include"FBO.h"
#include"Param.h"
#include<vector>
#pragma comment(lib, "soil.lib")
#pragma comment(lib, "glew32.lib")

FrameBufferObject* mFbo;

bool bCameraRotate = false;
float cameraXZRotateAngle = 0;
float cameraYRotateAngle = 0;

int lastMousePosX, lastMousePosY;

void DrawRoom();
void DrawRoomSample();

int bed = 0;
int wallDeco = 0;
int window = 0;
int drawer = 0;

int lightOn = 1;

float sunlight = 50.0;

void readModel() {
	wallVertexData = LoadObjModel("Res/group1/wall.obj", &wallIndexes, wallVertexCount, wallIndexCount);
	wallVbo = CreateBufferObject(GL_ARRAY_BUFFER, sizeof(VertexData) * wallVertexCount, GL_STATIC_DRAW, wallVertexData);
	wallIbo = CreateBufferObject(GL_ELEMENT_ARRAY_BUFFER, sizeof(unsigned int) * wallIndexCount, GL_STATIC_DRAW, wallIndexes);

	floorVertexData = LoadObjModel("Res/group1/floor.obj", &floorIndexes, floorVertexCount, floorIndexCount);
	floorVbo = CreateBufferObject(GL_ARRAY_BUFFER, sizeof(VertexData) * floorVertexCount, GL_STATIC_DRAW, floorVertexData);
	floorIbo = CreateBufferObject(GL_ELEMENT_ARRAY_BUFFER, sizeof(unsigned int) * floorIndexCount, GL_STATIC_DRAW, floorIndexes);

	roomVertexData = LoadObjModel("Res/group1/livingRoom.obj", &roomIndexes, roomVertexCount, roomIndexCount);
	roomVbo = CreateBufferObject(GL_ARRAY_BUFFER, sizeof(VertexData) * roomVertexCount, GL_STATIC_DRAW, roomVertexData);
	roomIbo = CreateBufferObject(GL_ELEMENT_ARRAY_BUFFER, sizeof(unsigned int) * roomIndexCount, GL_STATIC_DRAW, roomIndexes);
}

void loadTexture() {
	paintings.push_back(CreateTexture("Res/paintings/paint2.png"));
	paintings.push_back(CreateTexture("Res/paintings/paint3.png"));

	paintingsSize = paintings.size();

	otherPaintings.push_back(CreateTexture("Res/paintings/paint2.png"));
	otherPaintings.push_back(CreateTexture("Res/paintings/paint3.png"));

	OtherPaintingsSize = otherPaintings.size();

	walls.push_back(CreateTexture("Res/walls/wall1.png"));
	walls.push_back(CreateTexture("Res/walls/wall2.png"));
	walls.push_back(CreateTexture("Res/walls/wall3.png"));
	walls.push_back(CreateTexture("Res/walls/wall4.png"));
	walls.push_back(CreateTexture("Res/colors/red.png"));
	walls.push_back(CreateTexture("Res/colors/white.png"));
	walls.push_back(CreateTexture("Res/colors/yellow.png"));
	walls.push_back(CreateTexture("Res/colors/green.png"));
	walls.push_back(CreateTexture("Res/colors/orange.png"));
	walls.push_back(CreateTexture("Res/colors/gray.png"));
	walls.push_back(CreateTexture("Res/colors/brown.png"));
	walls.push_back(CreateTexture("Res/colors/blue.png"));
	walls.push_back(CreateTexture("Res/colors/pink.png"));
	wallsSize = walls.size();

	floors.push_back(CreateTexture("Res/floors/Floor1.png"));
	floors.push_back(CreateTexture("Res/floors/Floor2.png"));
	floors.push_back(CreateTexture("Res/floors/Floor3.png"));
	floors.push_back(CreateTexture("Res/floors/Floor4.png"));
	floors.push_back(CreateTexture("Res/floors/Floor5.png"));
	floors.push_back(CreateTexture("Res/colors/red.png"));
	floors.push_back(CreateTexture("Res/colors/white.png"));
	floors.push_back(CreateTexture("Res/colors/yellow.png"));
	floors.push_back(CreateTexture("Res/colors/green.png"));
	floors.push_back(CreateTexture("Res/colors/orange.png"));
	floors.push_back(CreateTexture("Res/colors/gray.png"));
	floors.push_back(CreateTexture("Res/colors/brown.png"));
	floors.push_back(CreateTexture("Res/colors/blue.png"));
	floors.push_back(CreateTexture("Res/colors/pink.png"));
	floorsSize = floors.size();
}
void init()
{
	readModel();
	loadTexture();

	cameraPos = glm::vec3(0.0f, 10.0f, 40.0f); //��ʼ���������λ��(x,y,z)
	cameraTarget = glm::vec3(-30.0f, 10.0f, 0.0f);//���������
	up = glm::vec3(0.0f, 1.0f, 0.0f);//���������� ���ڲ��

	cameraDirection = glm::normalize(cameraTarget - cameraPos);
	cameraRight = glm::normalize(glm::cross(up, cameraDirection));
	cameraUp = glm::cross(cameraDirection, cameraRight);
	cameraSpeed = 2.5f;

	viewMatrix = glm::lookAt(cameraPos, cameraTarget, glm::vec3(0.0f, 1.0f, 0.0f));

	lightProjectionMatrix = glm::ortho(-520.0f, 520.0f, -520.0f, 520.0f, 0.1f, 1000.0f);

	lightViewMatrix = glm::lookAt(glm::vec3(150.0f, 150.0f, 100.0f), glm::vec3(0.0f, 0.0f, -50.0f), glm::vec3(0.0f, 0.0f, -1.0f));

	renderProgram = CreateGPUProgram("Res/light.vs", "Res/light.fs");
	depthProgram = CreateGPUProgram("Res/sample.vs", "Res/sample.fs");
	livingRoomWallProgram = CreateGPUProgram("Res/livingRoomWallTexture.vs", "Res/livingRoomWallTexture.fs");

	renderPosLoc = 0;
	renderTexcoordLoc = 1;
	renderNormalLoc = 2;
	renderMLoc = glGetUniformLocation(renderProgram, "M");
	renderVLoc = glGetUniformLocation(renderProgram, "V");
	renderPLoc = glGetUniformLocation(renderProgram, "P");
	renderNMLoc = glGetUniformLocation(renderProgram, "NM");
	renderViewPosLocation = glGetUniformLocation(renderProgram, "ViewPos");
	renderTextureLoc = glGetUniformLocation(renderProgram, "U_MainTexture");
	shadowMapLoc = glGetUniformLocation(renderProgram, "U_ShadowMap");
	renderLightVMatrixLoc = glGetUniformLocation(renderProgram, "LightViewMatrix"); //�۲����
	renderLightPMatrixLoc = glGetUniformLocation(renderProgram, "LightProjectionMatrix"); //ͶӰ����
	renderLightOnLoc = glGetUniformLocation(renderProgram, "lightOn");

	rendersunlight = glGetUniformLocation(renderProgram, "sunlight");

	roomTexture = CreateTexture("Res/Colors.png");

	depthPosLoc = 0;
	depthTexcoordLoc = 1;
	depthNormalLoc = 2;
	depthMLoc = glGetUniformLocation(depthProgram, "M");
	depthVLoc = glGetUniformLocation(depthProgram, "V");
	depthPLoc = glGetUniformLocation(depthProgram, "P");

	glEnable(GL_DEPTH_TEST);//��Ȳ���

	mFbo = new FrameBufferObject;
	mFbo->AttachColorBuffer("color", GL_COLOR_ATTACHMENT0, 800, 600);
	mFbo->AttachDepthBuffer("depth", 800, 600);
	mFbo->Finish();

	mFbo->Bind();
	glClear(GL_DEPTH_BUFFER_BIT);
	DrawRoomSample();
	mFbo->Unbind();

	shadowMap = mFbo->GetBuffer("depth");

	glClearColor(0.6f, 0.6f, 0.8f, 1.0f);
}

void display()
{
	mFbo->Bind();
	glClear(GL_DEPTH_BUFFER_BIT);
	DrawRoomSample();               //  �򵥻���һ֡���õ���Ӱͼ

	mFbo->Unbind();
	shadowMap = mFbo->GetBuffer("depth");

	glClearColor(0.6f, 0.6f, 0.8f, 1.0f);
	glClear(GL_DEPTH_BUFFER_BIT | GL_COLOR_BUFFER_BIT);

	DrawRoom();         //  ����

	glutSwapBuffers();
	glutPostRedisplay();
}

void keyFunc(GLubyte key, int x, int y)          // ���̽���������   ws�ƶ������   c�л�������  l���ص�
{
	cameraRight = glm::normalize(glm::cross(up, cameraDirection));//���ݳ���ı�����

	switch (key)
	{
		//������ƶ�------------------------------------------------------
	case 'w': case 'W':
		cameraPos += cameraSpeed * cameraDirection;
		cameraTarget += cameraSpeed * cameraDirection;
		break;
	case 's': case 'S':
		cameraPos -= cameraSpeed * cameraDirection;
		cameraTarget -= cameraSpeed * cameraDirection;
		break;
	case 'a': case 'A':
		cameraPos += cameraSpeed * cameraRight;
		cameraTarget += cameraSpeed * cameraRight;
		break;
	case 'd': case 'D':
		cameraPos -= cameraSpeed * cameraRight;
		cameraTarget -= cameraSpeed * cameraRight;
		break;

		break;
		//��Ʒ��ʽ�л�------------------------------------------------------
	case '1'://�л����Ҵ���ʽ
		bed = (bed + 1) % 2;
		break;
	case '2'://�л�������ǽװ��
		wallDeco = (wallDeco + 1) % 2;
		break;
	case '3'://�л����Ҵ�����ʽ
		window = (window + 1) % 4;
		break;
	case '4'://�л�������ʽ
		drawer = (drawer + 1) % 2;
		break;
	case '0'://�л�����ǽ������
		wallTexture++;
		wallTexture %= wallsSize;
		break;
	case '9'://�л������ذ�����
		floorTexture++;
		floorTexture %= floorsSize;
		break;
	case '8'://�л������м�һ�����
		paintTexture++;
		paintTexture %= paintingsSize;
		break;
	case '7'://�л�������������
		otherPaintTexture++;
		otherPaintTexture %= OtherPaintingsSize;
		break;
		//���ص�------------------------------------------------------------
	case 'l': case 'L':
		lightOn = lightOn == 1 ? 0 : 1;
		break;
	}
	viewMatrix = glm::lookAt(cameraPos, cameraTarget, glm::vec3(0.0f, 1.0f, 0.0f));
	lightViewMatrix = glm::lookAt(glm::vec3(150.0f, 150.0f, 100.0f), glm::vec3(0.0f, 0.0f, -50.0f) + glm::vec3(sunlight, sunlight, 0), glm::vec3(0.0f, 0.0f, -1.0f));
}

void MouseFunc(int button, int state, int x, int y)
{
	if (button == GLUT_RIGHT_BUTTON && state == GLUT_DOWN)
	{
		bCameraRotate = true;
	}
	if (button == GLUT_RIGHT_BUTTON && state == GLUT_UP)
	{
		bCameraRotate = false;
	}

	if (button == 3)
	{
		fov += 1.5f;
	}
	if (button == 4) {
		fov -= 1.5f;
	}
}

void MotionFunc(int x, int y)       // ����ƶ�������  �Ҽ������ƶ����ڶ�����ͷ
{
	y = 600 - y;
	if (bCameraRotate)
	{
		if (x > lastMousePosX)
		{
			cameraXZRotateAngle += 0.02f;
			lastMousePosX = x;
		}
		else if (x < lastMousePosX)
		{
			cameraXZRotateAngle -= 0.02f;
			lastMousePosX = x;
		}
		if (y > lastMousePosY)
		{
			cameraYRotateAngle += 0.02f;
			lastMousePosY = y;
		}
		else if (y < lastMousePosY)
		{
			cameraYRotateAngle -= 0.02f;
			lastMousePosY = y;
		}
	}
	cameraDirection.x = sin(cameraXZRotateAngle);
	cameraDirection.z = -cos(cameraXZRotateAngle);
	cameraDirection.y = sin(cameraYRotateAngle);

	cameraDirection = glm::normalize(cameraDirection);

	cameraTarget = cameraPos + cameraDirection;

	viewMatrix = glm::lookAt(cameraPos, cameraTarget, glm::vec3(0.0f, 1.0f, 0.0f));
}

int main(int argc, char** argv)
{
	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_RGB | GLUT_DOUBLE);
	glutInitWindowPosition(100, 100);
	glutInitWindowSize(800, 600);
	glutCreateWindow("��ά����ҵ");

	glewInit();

	init();
	glutDisplayFunc(display);  // ������ʾ�ص����� -- ÿִ֡��
	glutKeyboardFunc(keyFunc);
	glutMouseFunc(MouseFunc);
	glutMotionFunc(MotionFunc);

	glutMainLoop();

	return 0;
}

void drawOne(GLuint vbo, GLuint ibo, int indexCount) {
	glBindBuffer(GL_ARRAY_BUFFER, vbo);//��vbo

	glVertexAttribPointer(renderPosLoc, 3, GL_FLOAT, GL_FALSE, sizeof(VertexData), (void*)0);
	glVertexAttribPointer(renderTexcoordLoc, 2, GL_FLOAT, GL_FALSE, sizeof(VertexData), (void*)(sizeof(float) * 3));
	glVertexAttribPointer(renderNormalLoc, 3, GL_FLOAT, GL_FALSE, sizeof(VertexData), (void*)(sizeof(float) * 5));

	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ibo);        // ����ibo�е�index����ͼ��
	glDrawElements(GL_TRIANGLES, indexCount, GL_UNSIGNED_INT, 0);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
}

void drawOneSample(GLuint vbo, GLuint ibo, int indexCount) {
	glBindBuffer(GL_ARRAY_BUFFER, vbo);

	glVertexAttribPointer(depthPosLoc, 3, GL_FLOAT, GL_FALSE, sizeof(VertexData), (void*)0);
	glVertexAttribPointer(depthTexcoordLoc, 2, GL_FLOAT, GL_FALSE, sizeof(VertexData), (void*)(sizeof(float) * 3));
	glVertexAttribPointer(depthNormalLoc, 3, GL_FLOAT, GL_FALSE, sizeof(VertexData), (void*)(sizeof(float) * 5));

	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ibo);        // ����ibo�е�index����ͼ��
	glDrawElements(GL_TRIANGLES, indexCount, GL_UNSIGNED_INT, 0);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
}

void DrawRoom()
{
	//--------------------------------------------------------------------------------------------------------------
	//׼������
	//--------------------------------------------------------------------------------------------------------------
	modelMatrix = glm::scale(2.0f, 2.0f, 2.0f);
	normalMatrix = glm::inverseTranspose(modelMatrix); //ģ�;��󣬿���ƽ�ơ���ת������

	glUseProgram(renderProgram);
	//glUniformMatrix4fv����ɫ������ֵ����һ��������λ�ã�2��3���䣬���ĸ�������Ҫ���ľ���
	glUniformMatrix4fv(renderMLoc, 1, GL_FALSE, glm::value_ptr(modelMatrix));             // ��gpu program�д�ֵ
	glUniformMatrix4fv(renderVLoc, 1, GL_FALSE, glm::value_ptr(viewMatrix));			//��Щ��������47�и�ֵ
	projectionMatrix = glm::perspective(fov, 800.0f / 600.0f, 0.1f, 800.0f);//ͶӰ����������޸���fov������������Ҫʵʱ���ݾ������ɫ������
	glUniformMatrix4fv(renderPLoc, 1, GL_FALSE, glm::value_ptr(projectionMatrix));
	glUniformMatrix4fv(renderNMLoc, 1, GL_FALSE, glm::value_ptr(normalMatrix));
	glUniform3fv(renderViewPosLocation, 1, glm::value_ptr(cameraPos));
	//glUniform1i����ÿ���������ķ�ʽ��������OpenGLÿ����ɫ�������������ĸ�����Ԫ������Ԫ�ܹ���16��
	glUniformMatrix4fv(renderLightPMatrixLoc, 1, GL_FALSE, glm::value_ptr(lightProjectionMatrix));
	glUniformMatrix4fv(renderLightVMatrixLoc, 1, GL_FALSE, glm::value_ptr(lightViewMatrix));
	glUniform1i(renderLightOnLoc, lightOn);

	//��Դλ�ô�ֵ
	glUniform1f(rendersunlight, sunlight);

	//������Ԫ
	glBindTextureUnit(0, roomTexture);      // ������Ԫ0
	glBindTextureUnit(1, shadowMap);

	glEnableVertexAttribArray(renderPosLoc);
	glEnableVertexAttribArray(renderTexcoordLoc);
	glEnableVertexAttribArray(renderNormalLoc);
	//--------------------------------------------------------------------------------------------------------------
	//��ʼ����
	//--------------------------------------------------------------------------------------------------------------
	//�ذ�
	glBindTextureUnit(0, floors[floorTexture]);      // ������Ԫ0
	drawOne(floorVbo, floorIbo, floorIndexCount);
	glBindTextureUnit(0, roomTexture);
	//ǽ��
	glBindTextureUnit(0, walls[wallTexture]);      // ������Ԫ0
	drawOne(wallVbo, wallIbo, wallIndexCount);
	glBindTextureUnit(0, roomTexture);

	//������Ʒ
	drawOne(roomVbo, roomIbo, roomIndexCount);
	//���� ǽ��--��1
	glBindTextureUnit(0, paintings[paintTexture]);
	drawOne(pVbo, pIbo, pIndexCount);
	glBindTextureUnit(0, roomTexture);
	//���� ǽ��--��2
	glBindTextureUnit(0, otherPaintings[otherPaintTexture]);
	drawOne(p2Vbo, p2Ibo, p2IndexCount);
	glBindTextureUnit(0, roomTexture);

	glUseProgram(0);
	//--------------------------------------------------------------------------------------------------------------
	//���ƽ���
	//--------------------------------------------------------------------------------------------------------------
}

void DrawRoomSample()
{
	modelMatrix = glm::scale(2.0f, 2.0f, 2.0f);
	glUseProgram(depthProgram);
	glUniformMatrix4fv(depthMLoc, 1, GL_FALSE, glm::value_ptr(modelMatrix));
	glUniformMatrix4fv(depthVLoc, 1, GL_FALSE, glm::value_ptr(lightViewMatrix));
	glUniformMatrix4fv(depthPLoc, 1, GL_FALSE, glm::value_ptr(lightProjectionMatrix));

	glEnableVertexAttribArray(depthPosLoc);
	glEnableVertexAttribArray(depthTexcoordLoc);
	glEnableVertexAttribArray(depthNormalLoc);

	//--------------------------------------------------------------------------------------------------------------
	//��ʼ����
	//--------------------------------------------------------------------------------------------------------------

	//�ذ�
	drawOneSample(floorVbo, floorIbo, floorIndexCount);
	//ǽ��
	drawOneSample(wallVbo, wallIbo, wallIndexCount);

	//������Ʒ
	drawOne(roomVbo, roomIbo, roomIndexCount);
	//���� ǽ��--��1
	drawOneSample(pVbo, pIbo, pIndexCount);
	//���� ǽ��--��2
	drawOneSample(p2Vbo, p2Ibo, p2IndexCount);

	glUseProgram(0);
}