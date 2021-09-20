#include "pch.h"
#include "OBJParser.h"

// General includes
#include <string>
#include <fstream>
#include <iostream>
#include <vector>
#include <sstream>

struct VertexCheck
{
	bool found{ false };
	int foundIndex{ -1 };
	Vertex_Input vertexToCheck;
};

void OBJParser::LoadModel(const std::string& givenName, const std::string& filePath, std::vector<Vertex_Input>& vBuffer, std::vector<uint32_t>& iBuffer, const Camera* /*pCamera*/)
{
	std::vector<DirectX::XMFLOAT3> positions;
	std::vector<DirectX::XMFLOAT3> normals;
	std::vector<DirectX::XMFLOAT2> UVs;

	std::string name = givenName;

	std::ifstream in(filePath, std::ios::in);
	if (!in)
	{
		std::cout << ("Model \"" + filePath + "\" does not exist.");
		return;
	}

	std::string line;
	while (getline(in, line))
	{
		if (line.substr(0, 1) == "#" || line.substr(0, 1) == "")
			continue;

		if (line.substr(0, 2) == "v ")
		{
			std::istringstream s{ line.substr(2) };
			DirectX::XMFLOAT3 v{};
			s >> v.x >> v.y >> v.z;
			positions.push_back(DirectX::XMFLOAT3{ v.x, v.y, -v.z });
		}
		else if (line.substr(0, 2) == "vt")
		{
			std::istringstream s(line.substr(2));
			DirectX::XMFLOAT2 v{};
			s >> v.x >> v.y;
			UVs.push_back(DirectX::XMFLOAT2(v.x, 1 - v.y));

			/* Calculation is happening with the Y component on the bottom
			*  So "1 - uv.y" is to convert from OBJ parser uv to the UV used in the rasterizer
			* --------------------------------------------------------------------------------
			*	             UV space rasterizer	   --		UV space OBJ-parser
			*                 [0, 1]---[1, 1]                     [0, 0]---[1, 0]
			*                  ||       ||	                       ||       ||
			*                  ||       ||	                       ||       ||
			*                 [0, 0]---[1, 0]                     [0, 1]---[1, 1]
			* --------------------------------------------------------------------------------
			*/
		}
		else if (line.substr(0, 2) == "vn")
		{
			std::istringstream s(line.substr(2));
			DirectX::XMFLOAT3 v{};
			s >> v.x >> v.y >> v.z;
			normals.push_back(DirectX::XMFLOAT3{ v.x, v.y, -v.z });
		}
		else if (line.substr(0, 2) == "f ")
		{
			std::istringstream s(line.substr(1));
			unsigned short v1, v2, v3;
			s >> v1 >> v2 >> v3;

			Vertex_Input vert0{};
			vert0.Position	= DirectX::XMFLOAT3{ positions[--v1] };
			vert0.Color = DirectX::XMFLOAT3{ 1.f, 0.f, 0.f };
			//vert0.UV		= DirectX::XMFLOAT2{ UVs[vt1] };
			//vert0.Normal	= DirectX::XMFLOAT3{ normals[--vn1] };
			//vert0.Tangent  += Elite::GetNormalized(Elite::Reject(Elite::FVector3{ tangent.x, tangent.y, -tangent.z }, normals[vn1]));

			Vertex_Input vert1{};
			vert1.Position	= DirectX::XMFLOAT3{ positions[--v2] };
			vert1.Color = DirectX::XMFLOAT3{ 0.f, 0.f, 1.f };
			//vert1.UV		= DirectX::XMFLOAT2{ UVs[vt2] };
			//vert1.Normal	= DirectX::XMFLOAT3{ normals[--vn2] };
			//vert1.Tangent += Elite::GetNormalized(Elite::Reject(Elite::FVector3{ tangent.x, tangent.y, -tangent.z }, normals[vn2]));

			Vertex_Input vert2{};
			vert2.Position	= DirectX::XMFLOAT3{ positions[--v3] };
			vert2.Color	= DirectX::XMFLOAT3{ 0.f, 1.f, 0.f };
			//vert2.UV		= DirectX::XMFLOAT2{ UVs[vt3] };
			//vert2.Normal	= DirectX::XMFLOAT3{ normals[--vn3] };
			//vert2.Tangent += Elite::GetNormalized(Elite::Reject(Elite::FVector3{ tangent.x, tangent.y, -tangent.z }, normals[vn3]));

			VertexCheck vCheck1{};	vCheck1.vertexToCheck = vert0;
			VertexCheck vCheck2{};	vCheck2.vertexToCheck = vert1;
			VertexCheck vCheck3{};	vCheck3.vertexToCheck = vert2;

			checkVertexExists(vCheck1, vCheck2, vCheck3, vBuffer);
			{
				if (!vCheck1.found)
				{
					vBuffer.push_back(vert0);
					iBuffer.push_back((uint32_t)vBuffer.size() - 1);
				}
				else
				{
					iBuffer.push_back((uint32_t)vCheck1.foundIndex);
				}
				if (!vCheck2.found)
				{
					vBuffer.push_back(vert1);
					iBuffer.push_back((uint32_t)vBuffer.size() - 1);
				}
				else
				{
					iBuffer.push_back((uint32_t)vCheck2.foundIndex);
				}
				if (!vCheck3.found)
				{
					vBuffer.push_back(vert2);
					iBuffer.push_back((uint32_t)vBuffer.size() - 1);
				}
				else
				{
					iBuffer.push_back((uint32_t)vCheck3.foundIndex);
				}
			}
		}
	}

	std::cout << ("Model \"" + name + "\" parsed & loaded in vBuffer, iBuffer. \n");
}

void OBJParser::checkVertexExists(VertexCheck& vCheck0, VertexCheck& vCheck1, VertexCheck& vCheck2, std::vector<Vertex_Input>& vBuffer)
{
	for (size_t i{}; i < vBuffer.size(); ++i)
	{
		if (!vCheck0.found && vBuffer[i] == vCheck0.vertexToCheck)
		{
			vCheck0.found = true;
			vCheck0.foundIndex = (int)i;
		}
		if (!vCheck1.found && vBuffer[i] == vCheck1.vertexToCheck)
		{
			vCheck1.found = true;
			vCheck1.foundIndex = (int)i;
		}
		if (!vCheck2.found && vBuffer[i] == vCheck2.vertexToCheck)
		{
			vCheck2.found = true;
			vCheck2.foundIndex = (int)i;
		}

		if (vCheck0.found && vCheck1.found && vCheck2.found)
			break;
	}
}