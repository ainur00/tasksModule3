// run-report https://contest.yandex.ru/contest/43508/run-report/76186545/

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

// ��������� ������ ��������� ������
struct BinaryTreeNode {
	BinaryTreeNode(int inKey, BinaryTreeNode* inLeft, BinaryTreeNode* inRight);
	~BinaryTreeNode();
	int key;
	BinaryTreeNode* left;
	BinaryTreeNode* right;
};

// �����������
BinaryTreeNode::BinaryTreeNode(int inKey, BinaryTreeNode* inLeft, BinaryTreeNode* inRight) {
	key = inKey;
	left = inLeft;
	right = inRight;
}

// ����������
BinaryTreeNode::~BinaryTreeNode() {
	delete left;
	delete right;
}

// ��������� ���������� ��� ������� InOrderDFS ��� ������ �������� �� ����
struct InOrderDfsParameter {
	InOrderDfsParameter(BinaryTreeNode* inBinaryTreeNode, int inSegment) {
		binaryTreeNode = inBinaryTreeNode;
		segment = inSegment;
	}
	BinaryTreeNode* binaryTreeNode;
	int segment;
};

// ���������� ����
void AddNodeToBinaryTree(BinaryTreeNode* binaryTreeNode, int key) {
	BinaryTreeNode* currentBinaryTreeNode = binaryTreeNode;
	if (key >= currentBinaryTreeNode->key)
		if (currentBinaryTreeNode->right)
			AddNodeToBinaryTree(currentBinaryTreeNode->right, key);
		else
			currentBinaryTreeNode->right = new BinaryTreeNode(key, nullptr, nullptr);
	else
		if (currentBinaryTreeNode->left)
			AddNodeToBinaryTree(currentBinaryTreeNode->left, key);
		else
			currentBinaryTreeNode->left = new BinaryTreeNode(key, nullptr, nullptr);
}

// ������� ������ ������, ���������� ����� � ������ �������
std::vector<int> GetBinaryTreeInOrderDFS(BinaryTreeNode* binaryTreeNode) {
	std::vector<int> binaryTreeInOrderDFS;
	std::stack<InOrderDfsParameter> InOrderDfsParameters;
	InOrderDfsParameters.push({ binaryTreeNode, 0 });
	while (!InOrderDfsParameters.empty()) {
		InOrderDfsParameter currentInOrderDfsParameter = InOrderDfsParameters.top();
		switch (currentInOrderDfsParameter.segment) {
		case 0:
			if (!currentInOrderDfsParameter.binaryTreeNode)
				InOrderDfsParameters.pop();
			else {
				InOrderDfsParameters.top().segment = 1;
				InOrderDfsParameters.push({ currentInOrderDfsParameter.binaryTreeNode->left, 0 });
			}
			break;
		case 1:
			binaryTreeInOrderDFS.push_back(currentInOrderDfsParameter.binaryTreeNode->key);
			InOrderDfsParameters.top().segment = 2;
			InOrderDfsParameters.push({ currentInOrderDfsParameter.binaryTreeNode->right, 0 });
			break;
		case 2:
			InOrderDfsParameters.pop();
			break;
		}
	}
	return binaryTreeInOrderDFS;
}

// ���������� ������� ������ ������ ��� ����, ����� ���������� ��� ���������
std::vector<int> GetBinaryTreeInOrderDFS(int nodeCount, int* keys) {
	BinaryTreeNode* root = new BinaryTreeNode(keys[0], nullptr, nullptr);
	for (int i = 1; i < nodeCount; ++i)
		AddNodeToBinaryTree(root, keys[i]);
	return GetBinaryTreeInOrderDFS(root);
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
