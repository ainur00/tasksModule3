// run-report https://contest.yandex.ru/contest/43508/run-report/80030740/

#include <vector>
#include <string>
#include <iostream>

// класс для работы с хэш-таблицей
class HashTable {
public:
	HashTable(int inSize);
	double fillFactor = 0;
	int realSize = 0;
	bool Add(std::string code);
	bool Remove(std::string code);
	bool Has(std::string code);
	int Size();
private:
	std::vector<std::string> hashTable;
	int GetHash(const std::string& code, int hashTableSize);
	void UpdateFillFactor();
	void Rehashing(bool isGrow);
};
HashTable::HashTable(int inSize = 8) : hashTable(inSize) {}
bool HashTable::Add(std::string code) {
	int codeHash = GetHash(code, hashTable.size());
	int firstDelPos = -1;
	int i = 0;
	do {
		if (hashTable[codeHash] == code)
			return false;
		if ((hashTable[codeHash] == "0") && (firstDelPos == -1))
			firstDelPos = codeHash;
		if (hashTable[codeHash] == "\0")
			break;
		if (i == hashTable.size() - 1)
			codeHash = GetHash(code, hashTable.size());
		codeHash = (codeHash + i + 1) % hashTable.size();
		++i;
	} while (i < hashTable.size());
	hashTable[(firstDelPos == -1) ? codeHash : firstDelPos] = code;
	++realSize;
	UpdateFillFactor();
	if (fillFactor >= 0.75)
		Rehashing(true);
	return true;
}
bool HashTable::Remove(std::string code) {
	int codeHash = GetHash(code, hashTable.size());
	int i = 0;
	do {
		if (hashTable[codeHash] == code) {
			hashTable[codeHash] = "0";
			--realSize;
			UpdateFillFactor();
			if ((fillFactor <= 0.25) && (hashTable.size() > 8))
				Rehashing(false);
			return true;
		}
		if ((hashTable[codeHash] == "\0") || (i == hashTable.size() - 1))
			return false;
		codeHash = (codeHash + i + 1) % hashTable.size();
		++i;
	} while (i < hashTable.size());
}
bool HashTable::Has(std::string code) {
	int codeHash = GetHash(code, hashTable.size());
	int i = 0;
	do {
		if (hashTable[codeHash] == code)
			return true;
		if ((hashTable[codeHash] == "\0") || (i == hashTable.size() - 1))
			return false;
		codeHash = (codeHash + i + 1) % hashTable.size();
		++i;
	} while (i < hashTable.size());
}
int HashTable::Size() {
	return hashTable.size();
}
int HashTable::GetHash(const std::string& code, int hashTableSize) {
	int codeHash = 0;
	for (int i = 0; i < code.length(); ++i)
		codeHash = (codeHash * 26 + code[i]) % hashTableSize;
	return codeHash;
}
void HashTable::UpdateFillFactor() {
	fillFactor = (double)(realSize) / hashTable.size();
}
void HashTable::Rehashing(bool isGrow) {
	std::vector<std::string> hashTableCopy = hashTable;
	hashTable.clear();
	realSize = 0;
	fillFactor = 0;
	hashTable.resize(isGrow ? 2 * hashTableCopy.size() : hashTableCopy.size() / 2);
	for (int i = 0; i < hashTableCopy.size(); ++i)
		if (hashTableCopy[i] != "0")
			this->Add(hashTableCopy[i]);
}

int main() {
	HashTable hashTable;
	char command = ' ';
	std::string code = "";
	while (std::cin >> command >> code) {
		switch (command) {
		case '+':
			std::cout << (hashTable.Add(code) == true ? "OK" : "FAIL") << std::endl;
			break;
		case '-':
			std::cout << (hashTable.Remove(code) == true ? "OK" : "FAIL") << std::endl;
			break;
		case '?':
			std::cout << (hashTable.Has(code) == true ? "OK" : "FAIL") << std::endl;
			break;
		}
	}
	return 0;
}