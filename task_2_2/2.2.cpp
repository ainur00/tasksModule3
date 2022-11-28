// run-report https://contest.yandex.ru/contest/43508/run-report/76846396/

/*
Дано число N < 106 и последовательность пар целых чисел из [-231, 231] длиной N.
Построить декартово дерево из N узлов, характеризующихся парами чисел (Xi, Yi).
Каждая пара чисел (Xi, Yi) определяет ключ Xi и приоритет Yi в декартовом дереве.
Добавление узла в декартово дерево выполняйте второй версией алгоритма, рассказанного на лекции:
При добавлении узла (x, y) выполняйте спуск по ключу до узла P с меньшим приоритетом.
Затем разбейте найденное поддерево по ключу x так, чтобы в первом поддереве все ключи меньше x,
а во втором больше или равны x. Получившиеся два дерева сделайте дочерними для нового узла (x, y).
Новый узел вставьте на место узла P. Построить также наивное дерево поиска по ключам Xi.
Равные ключи добавляйте в правое поддерево. Вычислить количество узлов в самом широком слое декартового
дерева и количество узлов в самом широком слое наивного дерева поиска.
Вывести их разницу. Разница может быть отрицательна.
*/

#include <iostream>
#include <random>
#include <queue>

// структура ячейки бинарного дерева поиска
struct BinaryTreeNode {
	BinaryTreeNode(int inKey);
	~BinaryTreeNode();
	int key;
	BinaryTreeNode* left = nullptr;
	BinaryTreeNode* right = nullptr;
};
BinaryTreeNode::BinaryTreeNode(int inKey) {
	key = inKey;
}
BinaryTreeNode::~BinaryTreeNode() {
	delete left;
	delete right;
}

// класс бинарного дерева поиска
class BinaryTree {
public:
	~BinaryTree();
	void Add(int key);
	int GetWidth();
private:
	BinaryTreeNode* root = nullptr;
};
BinaryTree::~BinaryTree() {
	delete root;
}
void BinaryTree::Add(int key) {
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
int BinaryTree::GetWidth() {
	int width = 0;
	int thisLayerElementCount = 0;
	int nextLayerElementCount = 0;
	std::queue<BinaryTreeNode*> pass;
	if (root) {
		pass.push(root);
		width = 1;
		thisLayerElementCount = 1;
	}
	while (!pass.empty()) {
		BinaryTreeNode* currentBinaryTreeNode = pass.front();
		pass.pop();
		thisLayerElementCount--;
		if (currentBinaryTreeNode->left) {
			pass.push(currentBinaryTreeNode->left);
			nextLayerElementCount++;
		}
		if (currentBinaryTreeNode->right) {
			pass.push(currentBinaryTreeNode->right);
			nextLayerElementCount++;
		}
		if (thisLayerElementCount == 0) {
			width = std::max(width, nextLayerElementCount);
			thisLayerElementCount = nextLayerElementCount;
			nextLayerElementCount = 0;
		}
	}
	return width;
}

// структура ячейки декартового дерева
struct TreapNode {
	TreapNode(int inKey, int inPriority);
	~TreapNode();
	int key;
	int priority;
	TreapNode* left = nullptr;
	TreapNode* right = nullptr;
};
TreapNode::TreapNode(int inKey, int inPriority) {
	key = inKey;
	priority = inPriority;
}
TreapNode::~TreapNode() {
	delete left;
	delete right;
}

// класс декартового дерева
class Treap {
public:
	~Treap();
	void Add(int key, int priority);
	int GetWidth();
private:
	TreapNode* root = nullptr;
	std::pair<TreapNode*, TreapNode*> Split(int key, TreapNode* currentTreapNode);
	TreapNode* Merge(TreapNode* left, TreapNode* right);
	void Add(int key, int priority, TreapNode*& currentTreapNode);
};
Treap::~Treap() {
	delete root;
}
std::pair<TreapNode*, TreapNode*> Treap::Split(int key, TreapNode* currentTreapNode) {
	if (!currentTreapNode)
		return { nullptr, nullptr };
	if (currentTreapNode->key < key) {
		std::pair<TreapNode*, TreapNode*> splitResult = Split(key, currentTreapNode->right);
		currentTreapNode->right = splitResult.first;
		return { currentTreapNode, splitResult.second };
	}
	else {
		std::pair<TreapNode*, TreapNode*> splitResult = Split(key, currentTreapNode->left);
		currentTreapNode->left = splitResult.second;
		return { splitResult.first, currentTreapNode };
	}
}
TreapNode* Treap::Merge(TreapNode* left, TreapNode* right) {
	if (left == nullptr || right == nullptr) {
		return left == nullptr ? right : left;
	}
	if (left->priority > right->priority) {
		left->right = Merge(left->right, right);
		return left;
	}
	else {
		right->left = Merge(left, right->left);
		return right;
	}
}
void Treap::Add(int key, int priority) {
	Add(key, priority, root);
}
void Treap::Add(int key, int priority, TreapNode*& currentTreapNode) {
	if (!currentTreapNode) {
		currentTreapNode = new TreapNode(key, priority);
		return;
	}
	else if (currentTreapNode->priority >= priority)
		if (key < currentTreapNode->key)
			Add(key, priority, currentTreapNode->left);
		else
			Add(key, priority, currentTreapNode->right);
	else {
		std::pair<TreapNode*, TreapNode*> splitResult = Split(key, currentTreapNode);
		TreapNode* treapNodeToAdd = new TreapNode(key, priority);
		treapNodeToAdd->left = splitResult.first;
		treapNodeToAdd->right = splitResult.second;
		currentTreapNode = treapNodeToAdd;
	}
}
int Treap::GetWidth() {
	int width = 0;
	int thisLayerElementCount = 0;
	int nextLayerElementCount = 0;
	std::queue<TreapNode*> pass;
	if (root) {
		pass.push(root);
		width = 1;
		thisLayerElementCount = 1;
	}
	while (!pass.empty()) {
		TreapNode* currentBinaryTreeNode = pass.front();
		pass.pop();
		thisLayerElementCount--;
		if (currentBinaryTreeNode->left) {
			pass.push(currentBinaryTreeNode->left);
			nextLayerElementCount++;
		}
		if (currentBinaryTreeNode->right) {
			pass.push(currentBinaryTreeNode->right);
			nextLayerElementCount++;
		}
		if (thisLayerElementCount == 0) {
			width = std::max(width, nextLayerElementCount);
			thisLayerElementCount = nextLayerElementCount;
			nextLayerElementCount = 0;
		}
	}
	return width;
}

// структура для удобно обработки входных данных
struct Node {
	int key;
	int priority;
};

// строит 2 дерева: дерево бинарное поиска поиска и декартово дерево, возвращает разницу ширин деревьев (ширина декартового дерева - ширина бинарного дерева поиска)
int GetWidthDifferent(Node* nodes, int nodeCount) {
	BinaryTree binaryTree;
	Treap treap;
	for (int i = 0; i < nodeCount; ++i) {
		binaryTree.Add(nodes[i].key);
		treap.Add(nodes[i].key, nodes[i].priority);
	}
	int binaryTreeWidth = binaryTree.GetWidth();
	int treapWidth = treap.GetWidth();
	return treapWidth - binaryTreeWidth;
}

int main() {
	int nodeCount = 0;
	std::cin >> nodeCount;
	Node* nodes = new Node[nodeCount];
	for (int i = 0; i < nodeCount; ++i)
		std::cin >> nodes[i].key >> nodes[i].priority;
	std::cout << GetWidthDifferent(nodes, nodeCount);
	delete[] nodes;
	return 0;
}
