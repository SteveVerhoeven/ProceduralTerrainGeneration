#include "VanaheimPCH.h"
#include "Cube.h"

#include "Scene.h"
#include "Line.h"
#include "Graphics.h"

void Cube::CreateLines(Scene* pScene, const DirectX::XMFLOAT3& pos_TopLeftFront, const DirectX::XMFLOAT3& pos_BottomRightBack)
{
    // Finding the width & height
    const float width{ pos_BottomRightBack.x - pos_TopLeftFront.x };
    const float height{ pos_TopLeftFront.y - pos_BottomRightBack.y };

    const std::string name{ "Line" };

    {   // Horizontal X-axis
        Line* pLine0 = new Line(LineCorner::TOP_LEFT_FRONT, LineCorner::TOP_RIGHT_FRONT, LineType::HORIZONTAL_X, width, height);
        const DirectX::XMFLOAT3 pos0{ pos_TopLeftFront.x, pos_TopLeftFront.y    , pos_TopLeftFront.z };
        pScene->CreateLineObject(name, pos0, pLine0);

        Line* pLine1 = new Line(LineCorner::BOTTOM_LEFT_FRONT, LineCorner::BOTTOM_RIGHT_FRONT, LineType::HORIZONTAL_X, width, height);
        const DirectX::XMFLOAT3 pos1{ pos_TopLeftFront.x, pos_BottomRightBack.y    , pos_TopLeftFront.z };
        pScene->CreateLineObject(name, pos1, pLine1);

        Line* pLine2 = new Line(LineCorner::TOP_LEFT_BACK, LineCorner::TOP_RIGHT_BACK, LineType::HORIZONTAL_X, width, height);
        const DirectX::XMFLOAT3 pos2{ pos_TopLeftFront.x, pos_TopLeftFront.y      , pos_BottomRightBack.z };
        pScene->CreateLineObject(name, pos2, pLine2);

        Line* pLine3 = new Line(LineCorner::BOTTOM_LEFT_BACK, LineCorner::BOTTOM_RIGHT_BACK, LineType::HORIZONTAL_X, width, height);
        const DirectX::XMFLOAT3 pos3{ pos_TopLeftFront.x, pos_BottomRightBack.y   , pos_BottomRightBack.z };
        pScene->CreateLineObject(name, pos3, pLine3);
    }

    {   // Horizontal Z-axis
        Line* pLine4 = new Line(LineCorner::TOP_LEFT_FRONT, LineCorner::TOP_LEFT_BACK, LineType::HORIZONTAL_Z, width, height);
        const DirectX::XMFLOAT3 pos4{ pos_TopLeftFront.x, pos_TopLeftFront.y      , pos_TopLeftFront.z };
        pScene->CreateLineObject(name, pos4, pLine4);

        Line* pLine5 = new Line(LineCorner::TOP_RIGHT_FRONT, LineCorner::TOP_RIGHT_BACK, LineType::HORIZONTAL_Z, width, height);
        const DirectX::XMFLOAT3 pos5{ pos_BottomRightBack.x, pos_TopLeftFront.y      , pos_TopLeftFront.z };
        pScene->CreateLineObject(name, pos5, pLine5);

        Line* pLine6 = new Line(LineCorner::BOTTOM_LEFT_FRONT, LineCorner::BOTTOM_LEFT_BACK, LineType::HORIZONTAL_Z, width, height);
        const DirectX::XMFLOAT3 pos6{ pos_TopLeftFront.x, pos_BottomRightBack.y   , pos_TopLeftFront.z };
        pScene->CreateLineObject(name, pos6, pLine6);

        Line* pLine7 = new Line(LineCorner::BOTTOM_RIGHT_FRONT, LineCorner::BOTTOM_RIGHT_BACK, LineType::HORIZONTAL_Z, width, height);
        const DirectX::XMFLOAT3 pos7{ pos_BottomRightBack.x, pos_BottomRightBack.y   , pos_TopLeftFront.z };
        pScene->CreateLineObject(name, pos7, pLine7);
    }

    {   // Vertical Y-axis
        Line* pLine8 = new Line(LineCorner::TOP_LEFT_FRONT, LineCorner::BOTTOM_LEFT_FRONT, LineType::VERTICAL, width, height);
        const DirectX::XMFLOAT3 pos8{ pos_TopLeftFront.x, pos_BottomRightBack.y, pos_TopLeftFront.z };
        pScene->CreateLineObject(name, pos8, pLine8);

        Line* pLine9 = new Line(LineCorner::TOP_RIGHT_FRONT, LineCorner::BOTTOM_RIGHT_FRONT, LineType::VERTICAL, width, height);
        const DirectX::XMFLOAT3 pos9{ pos_BottomRightBack.x, pos_BottomRightBack.y, pos_TopLeftFront.z };
        pScene->CreateLineObject(name, pos9, pLine9);

        Line* pLine10 = new Line(LineCorner::TOP_LEFT_BACK, LineCorner::BOTTOM_LEFT_BACK, LineType::VERTICAL, width, height);
        const DirectX::XMFLOAT3 pos10{ pos_TopLeftFront.x, pos_BottomRightBack.y, pos_BottomRightBack.z };
        pScene->CreateLineObject(name, pos10, pLine10);

        Line* pLine11 = new Line(LineCorner::TOP_RIGHT_BACK, LineCorner::BOTTOM_RIGHT_BACK, LineType::VERTICAL, width, height);
        const DirectX::XMFLOAT3 pos11{ pos_BottomRightBack.x, pos_BottomRightBack.y, pos_BottomRightBack.z };
        pScene->CreateLineObject(name, pos11, pLine11);
    }

}

HRESULT Cube::CreateInstanceBuffer(ID3D11Device* /*pDevice*/)
{
	return S_OK;
}