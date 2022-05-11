/** Original implementation is from: https://www.geeksforgeeks.org/octree-insertion-and-searching/. */
/** Adjust to fit the purpose of this project */
#include "VanaheimPCH.h"
#include "Octree.h"

#include "Cube.h"

Octree::Octree()
       : m_pCenter(new Node())
       , m_pPosition_TopLeftFront(nullptr)
       , m_pPosition_BottomRightBack(nullptr)
       , m_pChildren(std::vector<Octree*>())
{}
Octree::Octree(float x, float y, float z)
       : m_pCenter(new Node(x, y, z))
       , m_pPosition_TopLeftFront(nullptr)
       , m_pPosition_BottomRightBack(nullptr)
       , m_pChildren(std::vector<Octree*>())
{}
Octree::Octree(const DirectX::XMFLOAT3& position)
       : Octree(position.x, position.y, position.z)
{}
Octree::Octree(float x1, float y1, float z1, float x2, float y2, float z2)
       : m_pCenter(nullptr)
       , m_pPosition_TopLeftFront(nullptr)
       , m_pPosition_BottomRightBack(nullptr)
       , m_pChildren(std::vector<Octree*>())
{
    SetBoundaries({ x1, y1, z1 }, { x2, y2, z2 });
    InitializeChildren();
}
Octree::Octree(const DirectX::XMFLOAT3& position_TopLeftFront, const DirectX::XMFLOAT3& position_BottomRightBack)
       : Octree(position_TopLeftFront.x, position_TopLeftFront.y, position_TopLeftFront.z, position_BottomRightBack.x, position_BottomRightBack.y, position_BottomRightBack.z)
{}
Octree::Octree(float x1, float y1, float z1, const DirectX::XMFLOAT3& position_BottomRightBack)
       : Octree(x1, y1, z1, position_BottomRightBack.x, position_BottomRightBack.y, position_BottomRightBack.z)
{}
Octree::Octree(const DirectX::XMFLOAT3& position_TopLeftFront, float x2, float y2, float z2)
       : Octree(position_TopLeftFront.x, position_TopLeftFront.y, position_TopLeftFront.z, x2, y2, z2)
{}
Octree::~Octree()
{
    DELETE_POINTER(m_pCenter);
    DELETE_POINTER(m_pPosition_TopLeftFront);
    DELETE_POINTER(m_pPosition_BottomRightBack);
    DELETE_POINTERS(m_pChildren, m_pChildren.size());
}

void Octree::Insert(const float x, const float y, const float z)
{ Insert({ x, y, z }); }
void Octree::Insert(const DirectX::XMFLOAT3& position)
{
    // If the point already exists in the octree
    if (Find(position.x, position.y, position.z))
        return;

    // If the point is out of bounds
    if (IsPointOutOfBounds(position))
        return;

    // Binary search to insert the point
    const DirectX::XMFLOAT3 mid{ GetMidPoint() };
    const int pos = GetPositionInCube(position, mid); 

    // If an internal node is encountered
    if (m_pChildren[pos]->m_pCenter == nullptr) 
    {
        m_pChildren[pos]->Insert(position);
        return;
    }
    // If an empty node is encountered
    else if (EqualFloat(m_pChildren[pos]->m_pCenter->point.x, -1.f)) 
    {
        DELETE_POINTER(m_pChildren[pos]);
        m_pChildren[pos] = new Octree(position.x, position.y, position.z);
        return;
    }
    else 
    {
        const DirectX::XMFLOAT3 tempPoint{ m_pChildren[pos]->m_pCenter->point };
        DELETE_POINTER(m_pChildren[pos]);
        if (pos == TopLeftFront)
        {
            DirectX::XMFLOAT3 posTLF{};
            posTLF.x = m_pPosition_TopLeftFront->point.x;
            posTLF.y = m_pPosition_TopLeftFront->point.y;
            posTLF.z = m_pPosition_TopLeftFront->point.z;
            DirectX::XMFLOAT3 posBRB{};
            posBRB.x = mid.x;
            posBRB.y = mid.y;
            posBRB.z = mid.z;
            m_pChildren[pos] = new Octree(posTLF, posBRB);
        }
        else if (pos == TopRightFront) 
        {
            DirectX::XMFLOAT3 posTLF{};
            posTLF.x = mid.x;
            posTLF.y = m_pPosition_TopLeftFront->point.y;
            posTLF.z = m_pPosition_TopLeftFront->point.z;
            DirectX::XMFLOAT3 posBRB{};
            posBRB.x = m_pPosition_BottomRightBack->point.x;
            posBRB.y = mid.y;
            posBRB.z = mid.z;
            m_pChildren[pos] = new Octree(posTLF, posBRB);
        }
        else if (pos == BottomRightFront) 
        {
            DirectX::XMFLOAT3 posTLF{};
            posTLF.x = mid.x;
            posTLF.y = mid.y;
            posTLF.z = m_pPosition_TopLeftFront->point.z;
            DirectX::XMFLOAT3 posBRB{};
            posBRB.x = m_pPosition_BottomRightBack->point.x;
            posBRB.y = m_pPosition_BottomRightBack->point.y;
            posBRB.z = mid.z;
            m_pChildren[pos] = new Octree(posTLF, posBRB);
        }
        else if (pos == BottomLeftFront) 
        {
            DirectX::XMFLOAT3 posTLF{};
            posTLF.x = m_pPosition_TopLeftFront->point.x;
            posTLF.y = mid.y;
            posTLF.z = m_pPosition_TopLeftFront->point.z;
            DirectX::XMFLOAT3 posBRB{};
            posBRB.x = mid.x;
            posBRB.y = m_pPosition_BottomRightBack->point.y;
            posBRB.z = mid.z;
            m_pChildren[pos] = new Octree(posTLF, posBRB);
        }
        else if (pos == TopLeftBack) 
        {
            DirectX::XMFLOAT3 posTLF{};
            posTLF.x = m_pPosition_TopLeftFront->point.x;
            posTLF.y = m_pPosition_TopLeftFront->point.y;
            posTLF.z = mid.z;
            DirectX::XMFLOAT3 posBRB{};
            posBRB.x = mid.x;
            posBRB.y = mid.y;
            posBRB.z = m_pPosition_BottomRightBack->point.z;
            m_pChildren[pos] = new Octree(posTLF, posBRB);
        }
        else if (pos == TopRightBack) 
        {
            DirectX::XMFLOAT3 posTLF{};
            posTLF.x = mid.x;
            posTLF.y = m_pPosition_TopLeftFront->point.y;
            posTLF.z = mid.z;
            DirectX::XMFLOAT3 posBRB{};
            posBRB.x = m_pPosition_BottomRightBack->point.x;
            posBRB.y = mid.y;
            posBRB.z = m_pPosition_BottomRightBack->point.z;
            m_pChildren[pos] = new Octree(posTLF, posBRB);
        }
        else if (pos == BottomRightBack)
        {
            DirectX::XMFLOAT3 posTLF{};
            posTLF.x = mid.x;
            posTLF.y = mid.y;
            posTLF.z = mid.z;
            DirectX::XMFLOAT3 posBRB{};
            posBRB.x = m_pPosition_BottomRightBack->point.x;
            posBRB.y = m_pPosition_BottomRightBack->point.y;
            posBRB.z = m_pPosition_BottomRightBack->point.z;
            m_pChildren[pos] = new Octree(posTLF, posBRB);
        }
        else if (pos == BottomLeftBack) 
        {
            DirectX::XMFLOAT3 posTLF{};
            posTLF.x = m_pPosition_TopLeftFront->point.x;
            posTLF.y = mid.y;
            posTLF.z = mid.z;
            DirectX::XMFLOAT3 posBRB{};
            posBRB.x = mid.x;
            posBRB.y = m_pPosition_BottomRightBack->point.y;
            posBRB.z = m_pPosition_BottomRightBack->point.z;
            m_pChildren[pos] = new Octree(posTLF, posBRB);
        }
        m_pChildren[pos]->Insert(tempPoint);
        m_pChildren[pos]->Insert(position);
    }
}

bool Octree::Find(const float x, const float y, const float z)
{ return Find({ x, y, z }); }
bool Octree::Find(const DirectX::XMFLOAT3& position)
{
    // If point is out of bound
    if (IsPointOutOfBounds(position))
        return false;

    // Otherwise perform binary search
    // for each ordinate
    const DirectX::XMFLOAT3 mid{ GetMidPoint() };
    const int pos = GetPositionInCube(position, mid);

    // If an internal node is encountered
    if (m_pChildren[pos]->m_pCenter == nullptr)
        return m_pChildren[pos]->Find(position.x, position.y, position.z);

    // If an empty node is encountered
    else if (EqualFloat(m_pChildren[pos]->m_pCenter->point.x, -1.f))
        return false;
    else 
        if (m_pChildren[pos]->m_pCenter->IsEqual(position))
        {
            //LOG(ErrorLevel::LOG_INFO, "Point already exist in the tree \n");
            return true;
        }

    return false;
}

bool Octree::IsPointOutOfBounds(const DirectX::XMFLOAT3& position) const
{
    if (position.x < m_pPosition_TopLeftFront->point.x    ||
        position.x > m_pPosition_BottomRightBack->point.x ||
        position.y > m_pPosition_TopLeftFront->point.y    ||
        position.y < m_pPosition_BottomRightBack->point.y ||
        position.z < m_pPosition_TopLeftFront->point.z    ||
        position.z > m_pPosition_BottomRightBack->point.z)
        return true;
    
    return false;
}
DirectX::XMFLOAT3 Octree::GetMidPoint() const
{
    const float midx = (m_pPosition_TopLeftFront->point.x + m_pPosition_BottomRightBack->point.x) / 2.f;
    const float midy = (m_pPosition_TopLeftFront->point.y + m_pPosition_BottomRightBack->point.y) / 2.f;
    const float midz = (m_pPosition_TopLeftFront->point.z + m_pPosition_BottomRightBack->point.z) / 2.f;

    return DirectX::XMFLOAT3{midx, midy, midz};
}
int Octree::GetPositionInCube(const DirectX::XMFLOAT3& position, const DirectX::XMFLOAT3& mid) const
{
    if (position.x <= mid.x) 
    {
        if (position.y <= mid.y) 
        {
            if (position.z <= mid.z)
                return BottomLeftFront;
            else
                return BottomLeftBack;
        }
        else 
        {
            if (position.z <= mid.z)
                return TopLeftFront;
            else
                return TopLeftBack;
        }
    }
    else 
    {
        if (position.y <= mid.y) 
        {
            if (position.z <= mid.z)
                return BottomRightFront;
            else
                return BottomRightBack;
        }
        else 
        {
            if (position.z <= mid.z)
                return TopRightFront;
            else
                return TopRightBack;
        }
    }
}

void Octree::SetBoundaries(const DirectX::XMFLOAT3& position_TopLeftFront, const DirectX::XMFLOAT3& position_BottomRightBack)
{
    // This use to construct Octree
    // with boundaries defined
    if (position_BottomRightBack.x < position_TopLeftFront.x ||
        position_TopLeftFront.y < position_BottomRightBack.y ||
        position_BottomRightBack.z < position_TopLeftFront.z)
    {
        LOG(ErrorLevel::LOG_INFO, "Boundaries are not valid \n");
        return;
    }

    m_pCenter = nullptr;
    m_pPosition_TopLeftFront = new Node(position_TopLeftFront.x, position_TopLeftFront.y, position_TopLeftFront.z);
    m_pPosition_BottomRightBack = new Node(position_BottomRightBack.x, position_BottomRightBack.y, position_BottomRightBack.z);
}
void Octree::InitializeChildren()
{
    // Assigning null to the children
    m_pChildren.assign(8, nullptr);
    for (int i = TopLeftFront; i <= BottomLeftBack; ++i)
        m_pChildren[i] = new Octree();
}

// TODO: Move to a cube class!!!!! this stuff uuuugllyyyy AF
void Octree::Visualize(Scene* pScene)
{
    Cube cube{};
    cube.CreateLines(pScene, m_pPosition_TopLeftFront->point, m_pPosition_BottomRightBack->point);

    const size_t nbrOfChildren{ 8 };        // DAAAAAMMMNNNNN that's a lot of kids
    for (size_t i{}; i < nbrOfChildren; ++i)
    {
        Octree* pOctree{ m_pChildren[i] };
        if (pOctree && !pOctree->m_pCenter)
            pOctree->Visualize(pScene);
    }
}

void Octree::OutputOctreeSize(size_t& treeSize, size_t& nodeCount) const
{
    ++nodeCount;

    // Pointers
    treeSize += (m_pCenter)                     ? sizeof(Node*) : 0;   // Center
    treeSize += (m_pPosition_TopLeftFront)      ? sizeof(Node*) : 0;   // Top left front
    treeSize += (m_pPosition_BottomRightBack)   ? sizeof(Node*) : 0;   // Bottom right back

    // Objects
    treeSize += (m_pCenter)                     ? sizeof(Node) : 0;   // Center
    treeSize += (m_pPosition_TopLeftFront)      ? sizeof(Node) : 0;   // Top left front
    treeSize += (m_pPosition_BottomRightBack)   ? sizeof(Node) : 0;   // Bottom right back

    const size_t nbrOfChildren{ m_pChildren.size() };
    
    treeSize += sizeof(std::vector<Octree*>) * nbrOfChildren;
    
    for (size_t i{}; i < nbrOfChildren; ++i)
    {
        Octree* pChild{ m_pChildren[i] };
        if (!pChild)
            continue;

        m_pChildren[i]->OutputOctreeSize(treeSize, nodeCount);
    }
}