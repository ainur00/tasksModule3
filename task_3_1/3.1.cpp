// run-report https://contest.yandex.ru/contest/43508/run-report/80030539/

/*
В одной военной части решили построить в одну шеренгу по росту. 
Т.к. часть была далеко не образцовая, то солдаты часто приходили не вовремя, 
а то их и вовсе приходилось выгонять из шеренги за плохо начищенные сапоги. 
Однако солдаты в процессе прихода и ухода должны были всегда быть выстроены по росту – сначала самые высокие, 
а в конце – самые низкие. За расстановку солдат отвечал прапорщик, 
который заметил интересную особенность – все солдаты в части разного роста. 
Ваша задача состоит в том, чтобы помочь прапорщику правильно расставлять солдат, 
а именно для каждого приходящего солдата указывать, 
перед каким солдатом в строе он должен становится. 
Требуемая скорость выполнения команды - O(log n).
*/

#include <algorithm>
#include <iostream>
#include <vector>

// структура для работы с ячейками AVL дерева
struct AVLTreeNode {
	AVLTreeNode(int inKey);
	~AVLTreeNode();
	int key;
	int height = 1;
	int count = 1;
	AVLTreeNode* left = nullptr;
	AVLTreeNode* right = nullptr;
	static int GetHeight(AVLTreeNode* currentAVLTreeNode);
	static int GetCount(AVLTreeNode* currentAVLTreeNode);
	static void UpdateHeight(AVLTreeNode* currentAVLTreeNode);
	static void UpdateCount(AVLTreeNode* currentAVLTreeNode);
	static int GetBalance(AVLTreeNode* currentAVLTreeNode);
};
AVLTreeNode::AVLTreeNode(int inKey) {
	key = inKey;
}
AVLTreeNode::~AVLTreeNode() {
	delete left;
	delete right;
}
int AVLTreeNode::GetHeight(AVLTreeNode* currentAVLTreeNode) {
	return (currentAVLTreeNode ? currentAVLTreeNode->height : 0);
}
int AVLTreeNode::GetCount(AVLTreeNode* currentAVLTreeNode) {
	return (currentAVLTreeNode ? currentAVLTreeNode->count : 0);
}
void AVLTreeNode::UpdateHeight(AVLTreeNode* currentAVLTreeNode) {
	if (currentAVLTreeNode)
		currentAVLTreeNode->height = std::max(GetHeight(currentAVLTreeNode->left), GetHeight(currentAVLTreeNode->right)) + 1;
}
void AVLTreeNode::UpdateCount(AVLTreeNode* currentAVLTreeNode) {
	if (currentAVLTreeNode)
		currentAVLTreeNode->count = GetCount(currentAVLTreeNode->left) + GetCount(currentAVLTreeNode->right) + 1;
}
int AVLTreeNode::GetBalance(AVLTreeNode* currentAVLTreeNode) {
	return (currentAVLTreeNode ? GetHeight(currentAVLTreeNode->left) - GetHeight(currentAVLTreeNode->right) : 0);
}

// класс для работы с AVL деревом
class AVLTree {
	public:
		~AVLTree();
		int Add(int key);
		bool Delete(int key);
		bool DeleteAt(int stat);
	private:
		AVLTreeNode* root = nullptr;
		void RotateLeft(AVLTreeNode*& currentAVLTreeNode);
		void RotateRight(AVLTreeNode*& currentAVLTreeNode);
		void FixBalance(AVLTreeNode*& currentAVLTreeNode);
		int Add(int key, AVLTreeNode*& currentAVLTreeNode);
		int GetMinKeyAndDeleteMin(AVLTreeNode*& currentAVLTreeNode);
		bool Delete(int key, AVLTreeNode*& currentAVLTreeNode);
		bool DeleteAt(int stat, AVLTreeNode*& currentAVLTreeNode);
};
AVLTree::~AVLTree() {
	delete root;
}
int AVLTree::Add(int key) {
	// особенность задачи, возвращаем статистику с конца
	int avlTreeNodeCount = AVLTreeNode::GetCount(root);
	return avlTreeNodeCount - Add(key, root);
}
bool AVLTree::Delete(int key) {
	return Delete(key, root);
}
bool AVLTree::DeleteAt(int stat) {
	// особенность задачи, удаляем статистику с конца
	int avlTreeNodeCount = AVLTreeNode::GetCount(root);
	stat = avlTreeNodeCount - stat - 1;
	return DeleteAt(stat, root);
}
void AVLTree::RotateLeft(AVLTreeNode*& currentAVLTreeNode) {
	AVLTreeNode* currentAVLTreeNodeRight = currentAVLTreeNode->right;
	currentAVLTreeNode->right = currentAVLTreeNodeRight->left;
	currentAVLTreeNodeRight->left = currentAVLTreeNode;
	currentAVLTreeNode = currentAVLTreeNodeRight;
	AVLTreeNode::UpdateHeight(currentAVLTreeNode);
	AVLTreeNode::UpdateCount(currentAVLTreeNode);
	if (currentAVLTreeNode->left) {
		AVLTreeNode::UpdateHeight(currentAVLTreeNode->left);
		AVLTreeNode::UpdateCount(currentAVLTreeNode->left);
	}
	if (currentAVLTreeNode->right) {
		AVLTreeNode::UpdateHeight(currentAVLTreeNode->right);
		AVLTreeNode::UpdateCount(currentAVLTreeNode->right);
	}
}
void AVLTree::RotateRight(AVLTreeNode*& currentAVLTreeNode) {
	AVLTreeNode* currentAVLTreeNodeLeft = currentAVLTreeNode->left;
	currentAVLTreeNode->left = currentAVLTreeNodeLeft->right;
	currentAVLTreeNodeLeft->right = currentAVLTreeNode;
	currentAVLTreeNode = currentAVLTreeNodeLeft;
	AVLTreeNode::UpdateHeight(currentAVLTreeNode);
	AVLTreeNode::UpdateCount(currentAVLTreeNode);
	if (currentAVLTreeNode->left) {
		AVLTreeNode::UpdateHeight(currentAVLTreeNode->left);
		AVLTreeNode::UpdateCount(currentAVLTreeNode->left);
	}
	if (currentAVLTreeNode->right) {
		AVLTreeNode::UpdateHeight(currentAVLTreeNode->right);
		AVLTreeNode::UpdateCount(currentAVLTreeNode->right);
	}
}
void AVLTree::FixBalance(AVLTreeNode*& currentAVLTreeNode) {
	if (!currentAVLTreeNode)
		return;
	int currentAVLTreeNodeBalance = AVLTreeNode::GetBalance(currentAVLTreeNode);
	if (currentAVLTreeNodeBalance == -2) {
		if (AVLTreeNode::GetBalance(currentAVLTreeNode->right) == 1)
			RotateRight(currentAVLTreeNode->right);
		RotateLeft(currentAVLTreeNode);
	}
	else if (currentAVLTreeNodeBalance == 2) {
		if (AVLTreeNode::GetBalance(currentAVLTreeNode->left) == -1)
			RotateLeft(currentAVLTreeNode->left);
		RotateRight(currentAVLTreeNode);
	}
}
int AVLTree::Add(int key, AVLTreeNode*& currentAVLTreeNode) {
	if (!currentAVLTreeNode) {
		currentAVLTreeNode = new AVLTreeNode(key);
		return 0;
	}
	int stat = 0;
	if (key < currentAVLTreeNode->key)
		stat = Add(key, currentAVLTreeNode->left);
	else
		stat = Add(key, currentAVLTreeNode->right) + AVLTreeNode::GetCount(currentAVLTreeNode->left) + 1;
	FixBalance(currentAVLTreeNode);
	AVLTreeNode::UpdateHeight(currentAVLTreeNode);
	AVLTreeNode::UpdateCount(currentAVLTreeNode);
	return stat;
}
int AVLTree::GetMinKeyAndDeleteMin(AVLTreeNode*& currentAVLTreeNode) {
	int minAVLTreeNodeKey = 0;
	if (!currentAVLTreeNode->left) {
		minAVLTreeNodeKey = currentAVLTreeNode->key;
		if (currentAVLTreeNode->right) {
			AVLTreeNode nodeToSave = *currentAVLTreeNode->right;
			delete currentAVLTreeNode;
			currentAVLTreeNode = new AVLTreeNode(nodeToSave);
		}
		else
			currentAVLTreeNode = nullptr;
	}
	else if (currentAVLTreeNode->left->left)
		minAVLTreeNodeKey = GetMinKeyAndDeleteMin(currentAVLTreeNode->left);
	else {
		minAVLTreeNodeKey = currentAVLTreeNode->left->key;
		currentAVLTreeNode->left = currentAVLTreeNode->left->right;
	}
	FixBalance(currentAVLTreeNode);
	AVLTreeNode::UpdateHeight(currentAVLTreeNode);
	AVLTreeNode::UpdateCount(currentAVLTreeNode);
	return minAVLTreeNodeKey;
}
bool AVLTree::Delete(int key, AVLTreeNode*& currentAVLTreeNode) {
	bool isDeleted = false;
	if (!currentAVLTreeNode)
		isDeleted = false;
	else if (key == currentAVLTreeNode->key) {
		if ((!currentAVLTreeNode->left) || (!currentAVLTreeNode->right)) {
			if ((currentAVLTreeNode->left) || (currentAVLTreeNode->right)) {
				AVLTreeNode nodeToSave = *(currentAVLTreeNode->left ? currentAVLTreeNode->left : currentAVLTreeNode->right);
				delete currentAVLTreeNode;
				currentAVLTreeNode = new AVLTreeNode(nodeToSave);
			}
			else
				currentAVLTreeNode = nullptr;
		}
		else
			currentAVLTreeNode->key = GetMinKeyAndDeleteMin(currentAVLTreeNode->right);
		isDeleted = true;
	}
	else if (key < currentAVLTreeNode->key)
		isDeleted = Delete(key, currentAVLTreeNode->left);
	else
		isDeleted = Delete(key, currentAVLTreeNode->right);
	FixBalance(currentAVLTreeNode);
	AVLTreeNode::UpdateHeight(currentAVLTreeNode);
	AVLTreeNode::UpdateCount(currentAVLTreeNode);
	return isDeleted;
}
bool AVLTree::DeleteAt(int stat, AVLTreeNode*& currentAVLTreeNode) {
	bool isDeleted = false;
	if (stat >= AVLTreeNode::GetCount(currentAVLTreeNode))
		return isDeleted;
	else if (stat == AVLTreeNode::GetCount(currentAVLTreeNode->left)) {
		if ((!currentAVLTreeNode->left) || (!currentAVLTreeNode->right)) {
			if ((currentAVLTreeNode->left) || (currentAVLTreeNode->right)) {
				AVLTreeNode nodeToSave = *(currentAVLTreeNode->left ? currentAVLTreeNode->left : currentAVLTreeNode->right);
				delete currentAVLTreeNode;
				currentAVLTreeNode = new AVLTreeNode(nodeToSave);
			}
			else
				currentAVLTreeNode = nullptr;
		}
		else
			currentAVLTreeNode->key = GetMinKeyAndDeleteMin(currentAVLTreeNode->right);
		isDeleted = true;
	}
	else if (stat > AVLTreeNode::GetCount(currentAVLTreeNode->left))
		isDeleted = DeleteAt(stat - AVLTreeNode::GetCount(currentAVLTreeNode->left) - 1, currentAVLTreeNode->right);
	else
		isDeleted = DeleteAt(stat, currentAVLTreeNode->left);
	FixBalance(currentAVLTreeNode);
	AVLTreeNode::UpdateHeight(currentAVLTreeNode);
	AVLTreeNode::UpdateCount(currentAVLTreeNode);
	return isDeleted;
}

// структура для обработки комманд
struct Command {
	int type;
	int parameter;
};
// решение задачи по входным даныным, возвращает результаты комманд
std::vector<int> GetCommandsRsult(int commandsCount, Command* commands) {
	AVLTree line;
	std::vector<int> commandsResult;
	for (int i = 0; i < commandsCount; ++i) {
		switch (commands[i].type) {
		case 1:
			commandsResult.push_back(line.Add(commands[i].parameter));
			break;
		case 2:
			line.DeleteAt(commands[i].parameter);
			break;
		}
	}
	return commandsResult;
}

int main() {
	int commandsCount = 0;
	std::cin >> commandsCount;
	Command* commands = new Command[commandsCount];
	for (int i = 0; i < commandsCount; ++i)
		std::cin >> commands[i].type >> commands[i].parameter;
	std::vector<int> commandsResult = GetCommandsRsult(commandsCount, commands);
	for (int i = 0; i < commandsResult.size(); ++i)
		std::cout << commandsResult[i] << std::endl;
	delete[] commands;
	return 0;
}
