#pragma once

//ģ�Ͷ�ȡͷ�ļ�

/*
	�洢������Ϣ�Ľṹ��
	float position   ����λ����Ϣ v
	float texcoord   ������������ vt
	float normal     ���㷨����Ϣ vn
*/
struct VertexData        
{
	float position[3];  //����λ����Ϣ
	float texcoord[2];  //������������
	float normal[3];    //���㷨����Ϣ
};

/*
	��ȡobj�ļ�����ȡobj������Ϣ
	@param  filePath   obj�ļ���
	@param  indexes    �����������Ϣ
	@param  vertexCount   �������
	@param  indexCount    ��������
	@retval �洢obj��Ϣ��VertexData
*/
VertexData *LoadObjModel(const char* filePath,unsigned int **indexes,int&vertexCount,int&indexCount);
//��ȡ��obj�ļ��󷵻�ָ��һ���ṹ�������ָ�룬������objģ�����еĶ���λ����Ϣ�������������ꡢ���㷨����Ϣ