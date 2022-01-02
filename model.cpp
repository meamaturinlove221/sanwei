#include "model.h"
#include "misc.h"
#include <stdio.h>
#include <string>
#include <sstream>
#include <vector>

/*
* @���� ��ȡģ��
* @���� filePath obj�ļ����ļ���
* @���� indexes �����������Ϣ
* @���� vertexCount �������
* @���� indexCount  ��������
* 
* @���� �ṹ������ ������objģ�͵����ж���λ����Ϣ�������������ꡢ���㷨����Ϣ
*/
VertexData*LoadObjModel(const char* filePath, unsigned int **indexes, int&vertexCount, int&indexCount)
{
	//��ȡ�ı�����
	char*fileContent = LoadFileContent(filePath);
	if (fileContent!=nullptr)
	{
		//����
		struct VertexInfo
		{
			float v[3];
		}; //��ͬ��ʹ��glm��� glm::vec3
		
		//��ʾ����Ƭ�ĵ�i������� λ������/������������/����������
		struct VertexDefine
		{
			int positionIndex;
			int texcoordIndex;
			int normalIndex;
		};

		//�ݴ�obj�ļ������ඥ������
		std::vector<VertexInfo> positions; //����λ����Ϣ�б� ��ͬ��ʹ��glm��� std::vector<glm::vec3>& points, 
		std::vector<VertexInfo> texcoords; //�������������б�
		std::vector<VertexInfo> normals;   //���㷨����Ϣ�б�

		std::vector<unsigned int> objIndexes;// -> opengl indexes  ��������
		std::vector<VertexDefine> vertices;// -> opengl vertexes   ������Ƭ���� λ������/������������/����������

		std::stringstream ssObjFile(fileContent);//�ַ���
		char szOneLine[256];
		std::string temp;//�ݴ��ȡ������
		//��ȡ���ݲ��洢
		while (!ssObjFile.eof())
		{
			memset(szOneLine, 0, 256);//�����ڴ�ռ�
			ssObjFile.getline(szOneLine,256);//��ȡ
			if (strlen(szOneLine)>0)
			{
				std::stringstream ssOneLine(szOneLine);

				//���ݿ�ͷ�ַ��жϽ�����������������
				if (szOneLine[0]=='v')
				{
					if (szOneLine[1]=='t')
					{
						//vertex coord ������������
						ssOneLine >> temp;
						VertexInfo vi;
						ssOneLine >> vi.v[0];
						ssOneLine >> vi.v[1];
						texcoords.push_back(vi);
					}
					else if(szOneLine[1]=='n')
					{
						//normal  ���㷨������
						ssOneLine >> temp;
						VertexInfo vi;
						ssOneLine >> vi.v[0];
						ssOneLine >> vi.v[1];
						ssOneLine >> vi.v[2];
						normals.push_back(vi);
					}
					else
					{
						//position  ����λ������
						ssOneLine >> temp;
						VertexInfo vi;
						ssOneLine >> vi.v[0];
						ssOneLine >> vi.v[1];
						ssOneLine >> vi.v[2];
						positions.push_back(vi);
					}
				}
				else if (szOneLine[0] == 'f')
				{
					//face  ������Ƭ
					ssOneLine >> temp;// 'f'
					std::string vertexStr;
					for (int i=0;i<3;i++)
					{
						ssOneLine >> vertexStr;
						size_t pos = vertexStr.find_first_of('/');//����/�ָ��ַ���
						std::string positionIndexStr = vertexStr.substr(0, pos);//��ȡλ������
						size_t pos2 = vertexStr.find_first_of('/', pos + 1);
						std::string texcoordIndexStr = vertexStr.substr(pos + 1, pos2 - pos - 1);//��ȡ��������
						std::string normalIndexStr = vertexStr.substr(pos2 + 1, vertexStr.length() - pos2 - 1);//��ȡ������Ϣ
						VertexDefine vd;//����һ��������Ƭ����
						vd.positionIndex = atoi(positionIndexStr.c_str())-1; //atio:���ַ���ת��Ϊ������,��Ҫ��������ַ����� .c_str()��string��ĺ���,��stringת��Ϊ�ַ�����
						vd.texcoordIndex = atoi(texcoordIndexStr.c_str()) - 1;
						vd.normalIndex = atoi(normalIndexStr.c_str()) - 1;

						int nCurrentIndex = -1;//indexes
						//����Ƿ����
						size_t nCurrentVerticeCount = vertices.size();
						for (size_t j = 0; j < nCurrentVerticeCount; j++)
						{
							//�����Ѵ���vertices��,���õ�ǰ����ֵ
							if (vertices[j].positionIndex == vd.positionIndex&&
								vertices[j].texcoordIndex == vd.texcoordIndex&&
								vertices[j].normalIndex == vd.normalIndex)
							{
								nCurrentIndex = j;
								break;
							}
						}
						//������,�����¶�������,����vertices����
						if (nCurrentIndex==-1)
						{
							//create new vertice
							nCurrentIndex = vertices.size();
							vertices.push_back(vd);//����������Ƭ��������
						}
						objIndexes.push_back(nCurrentIndex);//���뵱ǰ����  f��һ�����ݾ���һ������
					}
				}
			}
		}
		//��ȡ���

		//�洢������Ϣ,Ϊ�����������������׼��
		//objIndexes->indexes buffer -> ibo
		indexCount = (int)objIndexes.size();
		*indexes = new unsigned int[indexCount];
		for (int i=0;i<indexCount;i++)
		{
			(*indexes)[i] = objIndexes[i];
		}
		//�洢��������,Ϊ���ɶ��㻺�������׼��
		//vertices -> vertexes -> vbo
		vertexCount = (int)vertices.size();
		VertexData *vertexes = new VertexData[vertexCount];  //����һ���ṹ������ָ��
		//��֮ǰ�ݴ��ֵ���Ƹ��ṹ������
		for (int i = 0 ; i < vertexCount ; ++i )
		{
			memcpy(vertexes[i].position, positions[vertices[i].positionIndex].v, sizeof(float) * 3);
			memcpy(vertexes[i].texcoord, texcoords[vertices[i].texcoordIndex].v, sizeof(float) * 2);
			memcpy(vertexes[i].normal, normals[vertices[i].normalIndex].v, sizeof(float) * 3);
		}
		return vertexes;//����
	}
	return nullptr;
}