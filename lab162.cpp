#include <iostream>
#include <queue>
#include <vector>
#include <algorithm>
#include <random>
#include <limits>
#include <fstream>
#include <sstream>
#include <string>

class TreeNode {
public:
	int key;
	TreeNode* left;
	TreeNode* right;

	TreeNode(int val) : key(val), left(nullptr), right(nullptr) {}
};

class BinaryTree {
private:
	TreeNode* root;

	void PrintTreeBeautiful(TreeNode* node, std::string prefix = "", bool isLeft = true) {
		if (node == nullptr) return;

		std::cout << prefix;
		std::cout << (isLeft ? "|-- " : "|__ ");
		std::cout << node->key << std::endl;

		std::string newPrefix = prefix + (isLeft ? "|   " : "    ");
		PrintTreeBeautiful(node->left, newPrefix, true);
		PrintTreeBeautiful(node->right, newPrefix, false);
	}

	void DeleteTree(TreeNode * node) {
		if (node == nullptr) return;
		DeleteTree(node->left);
		DeleteTree(node->right);
		delete node;
	}

	void PostOrderTraversal(TreeNode * node) {
		if (node == nullptr) return;
		PostOrderTraversal(node->left);
		PostOrderTraversal(node->right);
		std::cout << node->key << " ";
	}

	TreeNode* InsertSearchTree(TreeNode * node, int value) {
		if (node == nullptr) return new TreeNode(value);
		if (value < node->key)
			node->left = InsertSearchTree(node->left, value);
		else if (value > node->key)
			node->right = InsertSearchTree(node->right, value);
		return node;
	}

	TreeNode* manualInput() {
		std::cout << "Введите корневое значение (или -1 для пустого дерева): ";

		int rootVal;
		std::cin >> rootVal;
		if (rootVal == -1) return nullptr;

		TreeNode * rootNode = new TreeNode(rootVal);
		std::queue<TreeNode*> q;
		q.push(rootNode);

		while (!q.empty()) {
			TreeNode* current = q.front();
			q.pop();

			std::cout << "Введите левого и правого ребенка для " << current->key << " (-1 если нет): ";
			int leftVal, rightVal;
			std::cin >> leftVal >> rightVal;

			if (leftVal != -1) {
				current->left = new TreeNode(leftVal);
				q.push(current->left);
			}
			if (rightVal != -1) {
				current->right = new TreeNode(rightVal);
				q.push(current->right);
			}
		}
		return rootNode;
	}

	TreeNode* randomInput(int nodeCount) {
		std::vector<int> values;
		for (int i = 1; i <= nodeCount; i++) values.push_back(i);

		std::random_device rd;
		std::mt19937 g(rd());
		std::shuffle(values.begin(), values.end(), g);

		TreeNode * rootNode = nullptr;
		for (int val : values) {
			rootNode = InsertSearchTree(rootNode, val);
		}
		return rootNode;
	}

	TreeNode* fileInput() {
		std::string filename;
		std::cout << "Введите имя файла: ";
		std::cin >> filename;

		std::ifstream file(filename);
		if (!file.is_open()) {
			std::cout << "Ошибка" << filename << std::endl;
			return nullptr;
		}

		std::string line;
		std::getline(file, line);
		file.close();

		if (line.empty()) {
			std::cout << "Файл пуст\n";
			return nullptr;
		}

		std::vector<int> values;
		std::stringstream ss(line);
		std::string token;

		while (std::getline(ss, token, ',')) {
			values.push_back(std::stoi(token));
		}

		if (values.empty() || values[0] == -1) return nullptr;

		TreeNode * rootNode = new TreeNode(values[0]);
		std::queue<TreeNode*> q;
		q.push(rootNode);

		int i = 1;
		while (!q.empty() && i < values.size()) {
			TreeNode* current = q.front();
			q.pop();

			if (i < values.size() && values[i] != -1) {
				current->left = new TreeNode(values[i]);
				q.push(current->left);
			}
			i++;

			if (i < values.size() && values[i] != -1) {
				current->right = new TreeNode(values[i]);
				q.push(current->right);
			}
			i++;
		}
		return rootNode;
	}

public:
	BinaryTree() : root(nullptr) {}
	~BinaryTree() { clear(); }

	void clear() {
		DeleteTree(root);
		root = nullptr;
	}

	void inputData() {
		clear();

		std::cout << "\nВвод данных:\n";
		std::cout << "1. Ввод с клавиатуры\n";
		std::cout << "2. Генерация случайного дерева\n";
		std::cout << "3. Ввод из файла\n";
		std::cout << "Ваш выбор: ";

		int choice;
		std::cin >> choice;

		if (choice == 1) {
			root = manualInput();
		}
		else if (choice == 2) {
			int count;
			std::cout << "Введите кол-во узлов: ";
			std::cin >> count;
			root = randomInput(count);
		}
		else {
			root = fileInput();
		}
	}

	void display(const std::string & title = "Дерево") {
		std::cout << "\n" << title << "\n";
		if (root == nullptr) {
			std::cout << "Дерево пусто\n";
			return;
		}
		PrintTreeBeautiful(root, "", true);
		std::cout << std::endl;
	}

	void postOrderTraversal() {
		if (root == nullptr) {
			std::cout << "Дерево пусто\n";
			return;
		}
		PostOrderTraversal(root);
		std::cout << std::endl;
	}

	TreeNode* getRoot() { return root; }
	bool isEmpty() { return root == nullptr; }
};

class TreeValidator {
public:
	static bool IsAVLBalanced(TreeNode* root) {
		return CheckAVL(root) != -1;
	}

	static int CheckAVL(TreeNode* root) {
		if (root == nullptr) return 0;

		int leftHeight = CheckAVL(root->left);
		if (leftHeight == -1) return -1;

		int rightHeight = CheckAVL(root->right);
		if (rightHeight == -1) return -1;

		if (std::abs(leftHeight - rightHeight) > 1) return -1;

		return std::max(leftHeight, rightHeight) + 1;
	}

	static bool IsSearchTree(TreeNode * root, int minVal = INT_MIN, int maxVal = INT_MAX) {
		if (root == nullptr) return true;

		if (root->key < minVal || root->key > maxVal) return false;

		return IsSearchTree(root->left, minVal, root->key - 1) &&
			IsSearchTree(root->right, root->key + 1, maxVal);
	}

	static int GetHeight(TreeNode * root) {
		if (root == nullptr) return 0;
		return std::max(GetHeight(root->left), GetHeight(root->right)) + 1;
	}

	struct SearchResult {
		TreeNode* pointer;
		int nodesAnalyzed;
	};

	static SearchResult Search(TreeNode * root, int k) {
		SearchResult result{ nullptr, 0 };
		TreeNode* current = root;

		while (current != nullptr) {
			result.nodesAnalyzed++;

			if (current->key == k) {
				result.pointer = current;
				break;
			}
			else if (k < current->key) {
				current = current->left;
			}
			else {
				current = current->right;
			}
		}

		if (result.pointer == nullptr) {
			std::cout << "Значение " << k << " не найдено в дереве.\n";
		}

		return result;
	}
};

class TreeWork1 {
public:
	static void Execute() {
		std::cout << "\nTreeWork1\n";

		BinaryTree tree;
		tree.inputData();

		if (tree.isEmpty()) {
			std::cout << "\nДерево пусто\n";
			return;
		}

		std::cout << "\nДО ПРЕОБРАЗОВАНИЯ:";
		tree.display("Исходное дерево");
		std::cout << "Результат: ";
		tree.postOrderTraversal();

		std::cout << "\nПОСЛЕ ПРЕОБРАЗОВАНИЯ:";
		tree.display("Дерево после обхода");
	}
};

class TreeWork15 {
public:
	static void Execute() {
		std::cout << "\nTreeWork15\n";

		BinaryTree tree;
		tree.inputData();

		if (tree.isEmpty()) {
			std::cout << "\nДерево пусто\n";
			return;
		}

		if (!TreeValidator::IsSearchTree(tree.getRoot())) {
			std::cout << "\nОШИБКА\n";
			return;
		}

		std::cout << "\nДО ПРЕОБРАЗОВАНИЯ:";
		tree.display("Дерево поиска");

		int k;
		std::cout << "\nВведите значение K для поиска: ";
		std::cin >> k;

		TreeValidator::SearchResult result = TreeValidator::Search(tree.getRoot(), k);

		std::cout << "\nРЕЗУЛЬТАТЫ\n";
		if (result.pointer != nullptr) {
			std::cout << "Значение найдено\n";
			std::cout << "Указатель P2: " << result.pointer << "\n";
		}
		else {
			std::cout << "Значение не найдено\n";
			std::cout << "NULL\n";
		}
		std::cout << "  Количество пройденных вершин N = " << result.nodesAnalyzed << "\n";

		std::cout << "\nПОСЛЕ ПРЕОБРАЗОВАНИЯ:";
		tree.display("Дерево после поиска");
	}
};

class TreeWork20 {
public:
	static void Execute() {
		std::cout << "\nTreeWork20\n";

		BinaryTree tree;
		tree.inputData();

		if (tree.isEmpty()) {
			std::cout << "\nДерево пусто\n";
			std::cout << "Результат: True\n";
			return;
		}

		std::cout << "\nДО ПРЕОБРАЗОВАНИЯ:";
		tree.display("Исходное дерево");

		bool isBalanced = TreeValidator::IsAVLBalanced(tree.getRoot());
		int height = TreeValidator::GetHeight(tree.getRoot());

		std::cout << "\nРЕЗУЛЬТАТ\n";
		std::cout << "Результат: " << (isBalanced ? "True" : "False") << "\n";

		if (isBalanced) {
			std::cout << "Дерево является АВЛ-сбалансированным\n";
		}
		else {
			std::cout << "Дерево не является АВЛ-сбалансированным\n";
		}

		std::cout << "\nПОСЛЕ ПРЕОБРАЗОВАНИЯ:";
		tree.display("Дерево после проверки");
	}
};

class Menu {
public:
	void Run() {
		int choice;
		do {
			std::cout << "\nГЛАВНОЕ МЕНЮ\n";
			std::cout << "1. TreeWork1\n";
			std::cout << "2. TreeWork15\n";
			std::cout << "3. TreeWork20\n";
			std::cout << "0. Выход\n";
			std::cout << "Ваш выбор: ";
			std::cin >> choice;

			switch (choice) {
			case 1:
				TreeWork1::Execute();
				break;
			case 2:
				TreeWork15::Execute();
				break;
			case 3:
				TreeWork20::Execute();
				break;
			case 0:
				break;
			default:
				std::cout << "\nОшибка\n";
				break;
			}

			if (choice != 0) {
				std::cout << "\nНажмите Enter для продолжения...";
				std::cin.ignore();
				std::cin.get();
			}

		} while (choice != 0);
	}
};

int main() {
	setlocale(LC_ALL, "Russian");
	Menu menu;
	menu.Run();
}
