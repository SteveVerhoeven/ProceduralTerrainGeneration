#pragma once
#include "MathHelper.h"
#define MeshOctree

enum OctreePosition
{
	TopLeftFront = 0,
	TopRightFront = 1,
	BottomRightFront = 2,
	BottomLeftFront = 3,
	TopLeftBack = 4,
	TopRightBack = 5,
	BottomRightBack = 6,
	BottomLeftBack = 7
};

struct Node
{
	Node()
		: point({ -1.f, -1.f, -1.f })
	{}
	Node(const float x, const float y, const float z)
		: point({ x, y, z })
	{}

	inline bool IsEqual(const DirectX::XMFLOAT3& pointToCheck)
	{
		if (EqualFloat3(point, pointToCheck))
			return true;

		return false;
	}

	DirectX::XMFLOAT3 point;
};

class Scene;
class Octree final
{
	public:
		Octree();
		Octree(const float x, const float y, const float z);
		Octree(const DirectX::XMFLOAT3& position);
		Octree(const float x1, const float y1, const float z1, const float x2, const float y2, const float z2);
		Octree(const DirectX::XMFLOAT3& position_TopLeftFront, const DirectX::XMFLOAT3& position_BottomRightBack);
		Octree(const float x1, const float y1, const float z1, const DirectX::XMFLOAT3& position_BottomRightBack);
		Octree(const DirectX::XMFLOAT3& position_TopLeftFront, const float x2, const float y2, const float z2);
		~Octree();

		Octree(const Octree&) = delete;
		Octree& operator=(const Octree&) = delete;
		Octree(Octree&&) = delete;
		Octree& operator=(Octree&&) = delete;

		void Insert(const float x, const float y, const float z);
		void Insert(const DirectX::XMFLOAT3& position);
		bool Find(const float x, const float y, const float z);
		bool Find(const DirectX::XMFLOAT3& position);

		void Visualize(Scene* pScene);

		/** Calculate and display the size of octree */
		void OutputOctreeSize(size_t& treeSize, size_t& nodeCount) const;

	protected:

	private:
		Node* m_pCenter;
		Node* m_pPosition_TopLeftFront;
		Node* m_pPosition_BottomRightBack;
		std::vector<Octree*> m_pChildren;

		/** Set the boundaries of the octant */
		void SetBoundaries(const DirectX::XMFLOAT3& position_TopLeftFront, const DirectX::XMFLOAT3& position_BottomRightBack);
		/** Initialize the collection of children */
		void InitializeChildren();
		
		/** Check if point is outside of the octant */
		bool IsPointOutOfBounds(const DirectX::XMFLOAT3& position) const;
		/** Get the middle of the octant */
		DirectX::XMFLOAT3 GetMidPoint() const;
		/** Find the position of the point in the cube */
		int GetPositionInCube(const DirectX::XMFLOAT3& position, const DirectX::XMFLOAT3& mid) const;
};
