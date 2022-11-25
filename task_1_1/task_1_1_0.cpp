// run-report https://contest.yandex.ru/contest/43508/run-report/76186545/

/*
Дано число N <= 104 и последовательность целых чисел из [-231..231] длиной N.
Требуется построить бинарное дерево, заданное наивным порядком вставки.
Т.е., при добавлении очередного числа K в дерево с корнем root,
если root->Key <= K, то узел K добавляется в правое поддерево root;
иначе в левое поддерево root. Выведите элементы в порядке in-order (слева направо).
Рекурсия запрещена.
*/

#include <stack>
#include <iostream>
#include <vector>

// структура ячейки бинарного дерева
struct BinaryTreeNode {
	BinaryTreeNode(int inKey, BinaryTreeNode* inLeft, BinaryTreeNode* inRight);
	~BinaryTreeNode();
	int key;
	BinaryTreeNode* left;
	BinaryTreeNode* right;
};

// конструктор
BinaryTreeNode::BinaryTreeNode(int inKey, BinaryTreeNode* inLeft, BinaryTreeNode* inRight) {
	key = inKey;
	left = inLeft;
	right = inRight;
}

// деструктор
BinaryTreeNode::~BinaryTreeNode() {
	delete left;
	delete right;
}

// структура параметров для функции InOrderDFS для замены рекурсии на цикл
struct InOrderDfsParameter {
	InOrderDfsParameter(BinaryTreeNode* inBinaryTreeNode, int inSegment) {
		binaryTreeNode = inBinaryTreeNode;
		segment = inSegment;
	}
	BinaryTreeNode* binaryTreeNode;
	int segment;
};

// добавление узла
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

// функция обхода дерева, возвращает ключи в нужном порядке
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

// перегрузка функции обхода дерева для того, чтобы изначально его построить
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
