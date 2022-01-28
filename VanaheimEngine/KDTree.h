#pragma once
#include "MathHelper.h"

#define DIMENSIONS 3

struct KDTreeNode
{
	~KDTreeNode()
	{
		DELETE_POINTER(pLeftNode);
		DELETE_POINTER(pRightNode);
	}

	float point[DIMENSIONS];		// K Dimensional point
	KDTreeNode* pLeftNode;
	KDTreeNode* pRightNode;

	/** Check if the point from this node is the same as the values in this function parameter */
	inline bool IsEqual(const float* values, const unsigned int length) const
	{
		for (unsigned int i{}; i < length; ++i)
		{
			const bool isEqual{ EqualFloat(point[i], values[i]) };
			if (isEqual == false)
				return false;
		}

		return true;
	}
};

class KDTree
{
	public:
		KDTree();
		~KDTree();

		KDTree(const KDTree&) = delete;
		KDTree& operator=(const KDTree&) = delete;
		KDTree(KDTree&&) = delete;
		KDTree& operator=(KDTree&&) = delete;

		/** Function that inserts a new node in the K-D Tree and returns it */
		KDTreeNode* Insert(const float* values);
		KDTreeNode* Insert(const float* values, const unsigned int depth);
		KDTreeNode* Insert(KDTreeNode* pRootNode, const float* values, const unsigned int depth);

		/** Function that finds a node with the values that is asked for */
		/** The parameter 'depth' is used to determine the current axis (= dimension) */
		KDTreeNode* Find(const float* values);
		KDTreeNode* Find(const float* values, const unsigned int depth);
		KDTreeNode* Find(KDTreeNode* pRootNode, const float* values, const unsigned int depth);
		bool FindNode(const float* values);
		bool FindNode(const float* values, const unsigned int depth);
		bool FindNode(KDTreeNode* pRootNode, const float* values, const unsigned int depth);

		KDTreeNode* GetRootNode() const { return m_pRootNode; }
		void SetRootNode(KDTreeNode* pRootNode) { m_pRootNode = pRootNode; }

		void GetTreeSize(size_t& treeSize, size_t& nodeCount);

	protected:

	private:
		size_t m_NodeCounter;
		KDTreeNode* m_pRootNode;

		/** Function that creates a new node and copys the values to the new node */
		KDTreeNode* CreateNewNode(const float values[], const int length);
};