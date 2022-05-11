/** Original implementation is from: https://www.geeksforgeeks.org/k-dimensional-tree/#:~:text=A%20K%2DD%20Tree(also%20called,in%20a%20K%2DDimensional%20space. */
/** Adjust to fit the purpose of this project */
#include "VanaheimPCH.h"
#include "KDTree.h"

KDTree::KDTree()
	   : m_NodeCounter()
	   , m_pRootNode(nullptr)
{}
KDTree::~KDTree()
{
	DELETE_POINTER(m_pRootNode);
}

KDTreeNode* KDTree::Insert(const float* values)
{ return Insert(values, 0); }
KDTreeNode* KDTree::Insert(const float* values, const unsigned int depth)
{ return Insert(m_pRootNode, values, depth); }
KDTreeNode* KDTree::Insert(KDTreeNode* pRootNode, const float* values, const unsigned int depth)
{
	// Check if the root node is empty, if it is empty then create a new node with these values
	if (pRootNode == nullptr)
		return CreateNewNode(values, DIMENSIONS);

	// Calculate the current dimension (X, Y, Z)
	const unsigned int currentDimension{ depth % DIMENSIONS };

	// Compare the new values with the root node on the current dimension (currentDimension)
	// and decide if the new node is going to be on the left or right side of the tree
	if (values[currentDimension] < pRootNode->point[currentDimension])
		pRootNode->pLeftNode = Insert(pRootNode->pLeftNode, values, depth + 1);
	else
		pRootNode->pRightNode = Insert(pRootNode->pRightNode, values, depth + 1);

	return pRootNode;
}

KDTreeNode* KDTree::Find(const float* values)
{ return Find(m_pRootNode, values, 0); }
KDTreeNode* KDTree::Find(const float* values, const unsigned int depth)
{ return Find(m_pRootNode, values, depth + 1); }
KDTreeNode* KDTree::Find(KDTreeNode* pRootNode, const float* values, const unsigned int depth)
{
	// Check if the root node is empty, if it is empty then create a new node with these values
	if (pRootNode == nullptr)
		return nullptr;
	if (pRootNode->IsEqual(values, DIMENSIONS))
		return pRootNode;

	// Calculate the current dimension (X, Y, Z)
	const unsigned int currentDimension{ depth % DIMENSIONS };

	// Check if the value of the current axis with the axis of the root node
	if (values[currentDimension] < pRootNode->point[currentDimension])
		return Find(pRootNode->pLeftNode, values, depth + 1);

	return Find(pRootNode->pRightNode, values, depth + 1);
}

bool KDTree::FindNode(const float* values)
{ return FindNode(m_pRootNode, values, 0); }
bool KDTree::FindNode(const float* values, const unsigned int depth)
{ return FindNode(m_pRootNode, values, depth + 1); }
bool KDTree::FindNode(KDTreeNode* pRootNode, const float* values, const unsigned int depth)
{
	// Check if the root node is empty, if it is empty then create a new node with these values
	if (pRootNode == nullptr)
		return false;
	if (pRootNode->IsEqual(values, DIMENSIONS))
		return true;

	// Calculate the current dimension (X, Y, Z)
	const unsigned int currentDimension{ depth % DIMENSIONS };

	// Check if the value of the current axis with the axis of the root node
	if (values[currentDimension] < pRootNode->point[currentDimension])
		return FindNode(pRootNode->pLeftNode, values, depth + 1);

	return FindNode(pRootNode->pRightNode, values, depth + 1);
}

KDTreeNode* KDTree::CreateNewNode(const float values[], const int length)
{
	// Create an empty new node
	KDTreeNode* pNewNode{ new KDTreeNode() };

	// Assign the values to the ndoe
	for (int i{}; i < length; ++i)
		pNewNode->point[i] = values[i];

	// Set the left and right node to nullptr
	pNewNode->pLeftNode = nullptr;
	pNewNode->pRightNode = nullptr;

	// Return the new node
	++m_NodeCounter;
	return pNewNode;
}

void KDTree::GetTreeSize(size_t& treeSize, size_t& nodeCount)
{
	treeSize += sizeof(KDTreeNode*);
	
	// Traverse tree
	treeSize += sizeof(KDTreeNode) * m_NodeCounter;

	nodeCount = m_NodeCounter;
}