#include "VanaheimPCH.h"
#include "OBJWriter.h"

std::string OBJWriter::WriteOBJ(const std::string& fileName, const std::string& filePath, const std::vector<Vertex_Input>& vBuffer, const std::vector<uint32_t>& iBuffer) const
{
    // Open stream
    std::ofstream file(filePath + fileName + ".obj"); //open in constructor
    if (!file.is_open())
    {
        LOG(ErrorLevel::LOG_WARNING, "Writing OBJ file failed, path could not be found");
        return "";
    }

    std::vector<DirectX::XMFLOAT3> positions{};
    std::vector<DirectX::XMFLOAT2> UVs{};
    std::vector<int> indices{};
    std::vector<Face> faces{};

    // Gather all the vertices & UV's
    const size_t vCount{ vBuffer.size() };
    for (size_t vertex = 0; vertex < vCount; ++vertex)
    {
        const DirectX::XMFLOAT3 position{ vBuffer[vertex].Position };
        const DirectX::XMFLOAT2 uv{ vBuffer[vertex].UV };

        positions.push_back(position);
        UVs.push_back(uv);
    }

    // Gather all the indices
    const size_t iCount{ iBuffer.size() };
    for (size_t index = 0; index < iCount; ++index)
    {
        const int value{ int(iBuffer[index]) };
        indices.push_back(value);
    }

    // Form the faces
    for (size_t face = 0; face < iCount; face += 3)
    {
        const Face f{ {indices[face + 0], indices[face + 0], indices[face + 0]},
                      {indices[face + 1], indices[face + 1], indices[face + 1]},
                      {indices[face + 2], indices[face + 2], indices[face + 2]}};

        faces.push_back(f);
    }

    // Write to file
    const size_t pCount{ positions.size() };
    for (size_t p = 0; p < pCount; ++p)
    {
        const DirectX::XMFLOAT3 position{ positions[p] };
        file << "v " << position.x << " " << position.y << " " << position.z << "\n";
    }
    file << "# " << pCount << " vertices coords" << "\n\n";

    const size_t uvCount{ UVs.size() };
    for (size_t uv = 0; uv < uvCount; ++uv)
    {
        const DirectX::XMFLOAT2 UV{ UVs[uv] };
        file << "vt " << UV.x << " " << UV.y << " " << "\n";
    }
    file << "# " << uvCount << " texture coords" << "\n\n";

    const size_t fCount{ faces.size() };
    for (size_t f = 0; f < fCount; ++f)
    {
        const Face face{ faces[f] };
        file << "f " << face.v0.x << "/" << face.v0.y << "/" << face.v0.z << " " 
                     << face.v1.x << "/" << face.v1.y << "/" << face.v1.z << " "
                     << face.v2.x << "/" << face.v2.y << "/" << face.v2.z << " " << "\n";
    }
    file << "# " << fCount << " faces" << "\n\n";

    // close stream
    file.close();

    return (filePath + fileName + ".obj");
}