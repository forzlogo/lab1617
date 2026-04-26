#include <iostream>
#include <queue>
#include <vector>
#include <algorithm>
#include <random>
#include <limits>

class TreeNode {
public:
    int key;
    TreeNode* left;
    TreeNode* right;

    TreeNode(int val) : key(val), left(nullptr), right(nullptr) {}
};

typedef TreeNode* PNode;

void PrintTreeBeautiful(PNode root, std::string prefix = "", bool isLeft = true) {
    if (root == nullptr) {
        return;
    }

    std::cout << prefix;
    std::cout << (isLeft ? "|-- " : "|__ ");
    std::cout << root->key << std::endl;

    std::string newPrefix = prefix + (isLeft ? "|   " : "    ");
    PrintTreeBeautiful(root->left, newPrefix, true);
    PrintTreeBeautiful(root->right, newPrefix, false);
}

void DisplayTree(PNode root, const std::string& title = "Дерево") {
    std::cout << "\n=== " << title << " ===\n";
    if (root == nullptr) {
        std::cout << "Дерево пусто\n";
        return;
    }
    PrintTreeBeautiful(root, "", true);
    std::cout << std::endl;
}

class DataInput {
private:
    static PNode InsertSearchTree(PNode root, int value) {
        if (root == nullptr)
            return new TreeNode(value);

        if (value < root->key)
            root->left = InsertSearchTree(root->left, value);
        else if (value > root->key)
            root->right = InsertSearchTree(root->right, value);

        return root;
    }

public:
    static PNode ManualInput() {
        std::cout << "\n--- Ручной ввод дерева ---\n";
        std::cout << "Введите корневое значение (или -1 для пустого дерева): ";

        int rootVal;
        std::cin >> rootVal;

        if (rootVal == -1) return nullptr;

        PNode root = new TreeNode(rootVal);
        std::queue<PNode> q;
        q.push(root);

        while (!q.empty()) {
            PNode current = q.front();
            q.pop();

            std::cout << "Введите левого и правого ребенка для " << current->key
                << " (-1 если нет): ";
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

        return root;
    }

    static PNode GenerateRandomSearchTree(int nodeCount) {
        std::cout << "\n--- Генерация случайного дерева поиска (" << nodeCount << " узлов) ---\n";

        std::vector<int> values;
        for (int i = 1; i <= nodeCount; i++) {
            values.push_back(i);
        }

        std::random_device rd;
        std::mt19937 g(rd());
        std::shuffle(values.begin(), values.end(), g);

        PNode root = nullptr;
        for (int val : values) {
            root = InsertSearchTree(root, val);
        }

        return root;
    }

    static PNode InputTree() {
        std::cout << "\nВыберите способ ввода дерева:\n";
        std::cout << "1. Ручной ввод с клавиатуры\n";
        std::cout << "2. Генерация случайного дерева поиска\n";
        std::cout << "Ваш выбор: ";

        int choice;
        std::cin >> choice;

        if (choice == 1) {
            return ManualInput();
        }
        else {
            int count;
            std::cout << "Введите количество узлов: ";
            std::cin >> count;
            return GenerateRandomSearchTree(count);
        }
    }
};

class TreeMemory {
public:
    static void DeleteTree(PNode root) {
        if (root == nullptr) return;
        DeleteTree(root->left);
        DeleteTree(root->right);
        delete root;
    }
};

class TreeValidator {
public:
    static bool IsAVLBalanced(PNode root) {
        return CheckAVL(root) != -1;
    }

    static int CheckAVL(PNode root) {
        if (root == nullptr) return 0;

        int leftHeight = CheckAVL(root->left);
        if (leftHeight == -1) return -1;

        int rightHeight = CheckAVL(root->right);
        if (rightHeight == -1) return -1;

        if (std::abs(leftHeight - rightHeight) > 1) return -1;

        return std::max(leftHeight, rightHeight) + 1;
    }

    static bool IsSearchTree(PNode root, int minVal = INT_MIN, int maxVal = INT_MAX) {
        if (root == nullptr) return true;

        if (root->key < minVal || root->key > maxVal) return false;

        return IsSearchTree(root->left, minVal, root->key - 1) &&
            IsSearchTree(root->right, root->key + 1, maxVal);
    }

    static int GetHeight(PNode root) {
        if (root == nullptr) return 0;
        return std::max(GetHeight(root->left), GetHeight(root->right)) + 1;
    }
};

class TreeWork1 {
public:
    static void PostOrderTraversal(PNode root) {
        if (root == nullptr) return;

        PostOrderTraversal(root->left);
        PostOrderTraversal(root->right);
        std::cout << root->key << " ";
    }

    static void Execute() {
        std::cout << "\n========== TreeWork1: КОНЦЕВОЙ ОБХОД ==========\n";

        PNode root = DataInput::InputTree();

        if (root == nullptr) {
            std::cout << "\nДерево пусто\n";
            return;
        }
        DisplayTree(root, "Исходное дерево");
        std::cout << "Результат: ";
        PostOrderTraversal(root);
        std::cout << "\n";

        TreeMemory::DeleteTree(root);
    }
};

class TreeWork15 {
public:
    struct SearchResult {
        PNode pointer;
        int nodesAnalyzed;
    };

    static SearchResult Search(PNode root, int k) {
        SearchResult result{ nullptr, 0 };
        PNode current = root;

        std::cout << "\nПроцесс поиска значения " << k << ":\n";

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
            std::cout << "\nЗначение " << k << " не найдено в дереве.\n";
        }

        return result;
    }

    static void Execute() {
        std::cout << "\n========== TreeWork15: ПОИСК ЗНАЧЕНИЯ В ДЕРЕВЕ ==========\n";

        PNode root = DataInput::InputTree();

        if (root == nullptr) {
            std::cout << "\nДерево пусто\n";
            return;
        }

        if (!TreeValidator::IsSearchTree(root)) {
            std::cout << "\nОшибка\n";
            TreeMemory::DeleteTree(root);
            return;
        }

        DisplayTree(root, "Дерево поиска");

        int k;
        std::cout << "\nВведите значение K для поиска: ";
        std::cin >> k;

        SearchResult result = Search(root, k);

        if (result.pointer != nullptr) {
            std::cout << "Значение найдено\n";
            std::cout << "Указатель P2: " << result.pointer << "\n";
        }
        else {
            std::cout << "NULL\n";
        }
        std::cout << "Количество пройденных вершин N = " << result.nodesAnalyzed << "\n";

        TreeMemory::DeleteTree(root);
    }
};

class TreeWork20 {
public:
    static void Execute() {
        std::cout << "\n========== TreeWork20: ПРОВЕРКА АВЛ-СБАЛАНСИРОВАННОСТИ ==========\n";

        PNode root = DataInput::InputTree();

        if (root == nullptr) {
            std::cout << "\nДерево пусто\n";
            std::cout << "Результат: True\n";
            return;
        }

        DisplayTree(root, "Исходное дерево");

        bool isBalanced = TreeValidator::IsAVLBalanced(root);
        int height = TreeValidator::GetHeight(root);

        std::cout << "Результат: " << (isBalanced ? "True" : "False") << "\n";
        TreeMemory::DeleteTree(root);
    }
};

class Menu {
public:
    void Run() {
        int choice;
        do {
            std::cout << "\n========== ГЛАВНОЕ МЕНЮ ==========\n";
            std::cout << "1. TreeWork1 - Концевой обход (снизу-вверх)\n";
            std::cout << "2. TreeWork15 - Поиск значения в дереве\n";
            std::cout << "3. TreeWork20 - Проверка АВЛ-сбалансированности\n";
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
