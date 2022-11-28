// run-report https://contest.yandex.ru/contest/43508/run-report/76787220/

/*
���� ����� N <= 104 � ������������������ ����� ����� �� [-231..231] ������ N.
��������� ��������� �������� ������, �������� ������� �������� �������.
�.�., ��� ���������� ���������� ����� K � ������ � ������ root,
���� root->Key <= K, �� ���� K ����������� � ������ ��������� root;
����� � ����� ��������� root. �������� �������� � ������� in-order (����� �������).
�������� ���������.
*/

#include <stack>
#include <iostream>
#include <vector>
#include <utility>

// ��������� ������ ��������� ������
struct BinaryTreeNode {
	BinaryTreeNode(int inKey, BinaryTreeNode* inLeft, BinaryTreeNode* inRight);
	~BinaryTreeNode();
	int key;
	BinaryTreeNode* left;
	BinaryTreeNode* right;
};
BinaryTreeNode::BinaryTreeNode(int inKey, BinaryTreeNode* inLeft = nullptr, BinaryTreeNode* inRight = nullptr) {
	key = inKey;
	left = inLeft;
	right = inRight;
}
BinaryTreeNode::~BinaryTreeNode() {
	// ���������� ������ ��� �������� ������� Post-Order, � ����� nodesToDelete ����� ��������� root. � ����� ������� ����������� ������ ��� ���� ����� ������ � ������ Post-Order ����� root, ��� ��� ����� ������ �� ����������� ��� ��������� �������������.
	std::vector<BinaryTreeNode*> nodesToDelete;
	std::stack<std::pair<BinaryTreeNode*, int>> postOrderDfsParameters;
	postOrderDfsParameters.push({ this, 0 });
	while (!postOrderDfsParameters.empty()) {
		std::pair<BinaryTreeNode*, int> currentPostOrderDfsParameter = postOrderDfsParameters.top();
		switch (currentPostOrderDfsParameter.second) {
		case 0:
			if (!currentPostOrderDfsParameter.first)
				postOrderDfsParameters.pop();
			else {
				postOrderDfsParameters.top().second = 1;
				postOrderDfsParameters.push({ currentPostOrderDfsParameter.first->left, 0 });
			}
			break;
		case 1:
			postOrderDfsParameters.top().second = 2;
			postOrderDfsParameters.push({ currentPostOrderDfsParameter.first->right, 0 });
			break;
		case 2:
			nodesToDelete.push_back(postOrderDfsParameters.top().first);
			postOrderDfsParameters.pop();
			break;
		}
	}
	for (int i = 0; i < nodesToDelete.size() - 1; ++i)
		free(nodesToDelete[i]);
}

// ��������� ���������� ��� ������� InOrderDFS ��� ������ �������� �� ����
struct DfsParameter {
	DfsParameter(BinaryTreeNode* inBinaryTreeNode, int inSegment);
	BinaryTreeNode* binaryTreeNode;
	int segment;
};
DfsParameter::DfsParameter(BinaryTreeNode* inBinaryTreeNode, int inSegment) {
	binaryTreeNode = inBinaryTreeNode;
	segment = inSegment;
}

// ���������� ����
void AddNodeToBinaryTree(int key, BinaryTreeNode* root) {
	if (!root) {
		root = new BinaryTreeNode(key);
		return;
	}
	BinaryTreeNode* currentBinaryTreeNode = root;
	while (true) {
		if (currentBinaryTreeNode->key > key) {
			if (currentBinaryTreeNode->left != nullptr)
				currentBinaryTreeNode = currentBinaryTreeNode->left;
			else {
				currentBinaryTreeNode->left = new BinaryTreeNode(key);
				break;
			}
		}
		else {
			if (currentBinaryTreeNode->right != nullptr)
				currentBinaryTreeNode = currentBinaryTreeNode->right;
			else {
				currentBinaryTreeNode->right = new BinaryTreeNode(key);
				break;
			}
		}
	}
}
// ������� ������ ������, ���������� ����� � ������ �������
std::vector<int> GetBinaryTreeInOrderDFS(BinaryTreeNode* binaryTreeNode) {
	std::vector<int> binaryTreeInOrderDFS;
	std::stack<DfsParameter> inOrderDfsParameters;
	inOrderDfsParameters.push({ binaryTreeNode, 0 });
	while (!inOrderDfsParameters.empty()) {
		DfsParameter currentInOrderDfsParameter = inOrderDfsParameters.top();
		switch (currentInOrderDfsParameter.segment) {
		case 0:
			if (!currentInOrderDfsParameter.binaryTreeNode)
				inOrderDfsParameters.pop();
			else {
				inOrderDfsParameters.top().segment = 1;
				inOrderDfsParameters.push({ currentInOrderDfsParameter.binaryTreeNode->left, 0 });
			}
			break;
		case 1:
			binaryTreeInOrderDFS.push_back(currentInOrderDfsParameter.binaryTreeNode->key);
			inOrderDfsParameters.top().segment = 2;
			inOrderDfsParameters.push({ currentInOrderDfsParameter.binaryTreeNode->right, 0 });
			break;
		case 2:
			inOrderDfsParameters.pop();
			break;
		}
	}
	return binaryTreeInOrderDFS;
}
// ���������� ������� ������ ������ ��� ����, ����� ���������� ��� ���������
std::vector<int> GetBinaryTreeInOrderDFS(int nodeCount, int* keys) {
	BinaryTreeNode* root = new BinaryTreeNode(keys[0]);
	for (int i = 1; i < nodeCount; ++i)
		AddNodeToBinaryTree(keys[i], root);
	std::vector<int> binaryTreeInOrderDFS = GetBinaryTreeInOrderDFS(root);
	delete root;
	return binaryTreeInOrderDFS;
}

int main() {
	int nodeCount = 0;
	std::cin >> nodeCount;
	int* keys = new int[nodeCount];
	for (int i = 0; i < nodeCount; ++i)
		std::cin >> keys[i];
	std::vector<int> binaryTreeInOrderDFS = GetBinaryTreeInOrderDFS(nodeCount, keys);
	for (int i = 0; i < nodeCount; ++i)
		std::cout << binaryTreeInOrderDFS[i] << ' ';
	delete[] keys;
	return 0;
}
