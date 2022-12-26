// run-report https://contest.yandex.ru/contest/43508/run-report/76787220/

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
#include <utility>

// структура ячейки бинарного дерева
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
	// собираются ячейки для удаления обходом Post-Order, в конце nodesToDelete будет находится root. в конце функции освобождаем память для всех ячеек дерева в обходе Post-Order кроме root, так как после выхода из деструктора это сделается автоматически.
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

// структура параметров для функции InOrderDFS для замены рекурсии на цикл
struct DfsParameter {
	DfsParameter(BinaryTreeNode* inBinaryTreeNode, int inSegment);
	BinaryTreeNode* binaryTreeNode;
	int segment;
};
DfsParameter::DfsParameter(BinaryTreeNode* inBinaryTreeNode, int inSegment) {
	binaryTreeNode = inBinaryTreeNode;
	segment = inSegment;
}

// добавление узла
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
// функция обхода дерева, возвращает ключи в нужном порядке
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
// перегрузка функции обхода дерева для того, чтобы изначально его построить
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
