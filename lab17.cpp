#include <iostream>
#include <fstream>
#include <string>
#include <stack>
#include <queue>
#include <vector>
#include <sstream>
#include <cmath>
#include <algorithm>
#include <cctype>
#include <random>

class Node {
public:
    int value;
    Node* left;
    Node* right;

    Node(int val) : value(val), left(nullptr), right(nullptr) {}
};

class ExpressionTree {
private:
    Node* root;

    void deleteTree(Node* node) {
        if (node == nullptr) return;
        deleteTree(node->left);
        deleteTree(node->right);
        delete node;
    }

    bool isOperator(int val) {
        return val == -1 || val == -2 || val == -3 || val == -4;
    }

    int evaluateNode(Node* node) {
        if (node == nullptr) return 0;
        if (!isOperator(node->value)) return node->value;

        int leftVal = evaluateNode(node->left);
        int rightVal = evaluateNode(node->right);

        switch (node->value) {
        case -1: return leftVal + rightVal;
        case -2: return leftVal - rightVal;
        case -3: return leftVal * rightVal;
        case -4: return leftVal / rightVal;
        default: return 0;
        }
    }

    Node* simplifyAddition(Node* node) {
        if (node == nullptr) return nullptr;

        node->left = simplifyAddition(node->left);
        node->right = simplifyAddition(node->right);

        if (node->value == -1) {
            int result = evaluateNode(node);
            Node* newNode = new Node(result);
            deleteTree(node->left);
            deleteTree(node->right);
            delete node;
            return newNode;
        }

        return node;
    }

    void printTreeBeautiful(Node* node, std::string prefix = "", bool isLeft = true) {
        if (node == nullptr) return;

        std::string valueStr;
        if (isOperator(node->value)) {
            switch (node->value) {
            case -1: valueStr = "+"; break;
            case -2: valueStr = "-"; break;
            case -3: valueStr = "*"; break;
            case -4: valueStr = "/"; break;
            }
        }
        else {
            valueStr = std::to_string(node->value);
        }

        std::cout << prefix;
        std::cout << (isLeft ? "|-- " : "|__ ");
        std::cout << valueStr << std::endl;

        std::string newPrefix = prefix + (isLeft ? "|   " : "    ");
        printTreeBeautiful(node->left, newPrefix, true);
        printTreeBeautiful(node->right, newPrefix, false);
    }

public:
    ExpressionTree() : root(nullptr) {}
    ~ExpressionTree() { deleteTree(root); }

    void manualInput() {
        std::cout << "Введите выражение в обратной польской записи: ";
        std::cin.ignore();
        std::string expression;
        std::getline(std::cin, expression);
        buildFromString(expression);
    }

    void randomInput() {
        std::random_device rd;
        std::mt19937 gen(rd());
        std::uniform_int_distribution<> numDist(0, 9);
        std::uniform_int_distribution<> opDist(1, 4);
        std::uniform_int_distribution<> depthDist(2, 4);

        int depth = depthDist(gen);
        std::vector<std::string> tokens;

        for (int i = 0; i < depth; i++) {
            tokens.push_back(std::to_string(numDist(gen)));
        }

        for (int i = 0; i < depth - 1; i++) {
            int op = opDist(gen);
            switch (op) {
            case 1: tokens.push_back("+"); break;
            case 2: tokens.push_back("-"); break;
            case 3: tokens.push_back("*"); break;
            case 4: tokens.push_back("/"); break;
            }
        }

        std::string expression;
        for (const auto& token : tokens) {
            expression += token + " ";
        }

        std::cout << "Сгенерировано выражение: " << expression << std::endl;
        buildFromString(expression);
    }

    void fileInput() {
        std::string filename;
        std::cout << "Введите имя файла: ";
        std::cin >> filename;

        std::ifstream file(filename);
        if (!file.is_open()) {
            std::cout << "Ошибка при открытии файла " << filename << std::endl;
            return;
        }

        std::string line;
        std::getline(file, line);
        file.close();

        std::cout << "Из файла прочитано: " << line << std::endl;
        buildFromString(line);
    }

    void buildFromString(const std::string& expression) {
        deleteTree(root);
        root = nullptr;

        std::stringstream ss(expression);
        std::string token;
        std::stack<Node*> st;

        while (ss >> token) {
            if (token == "+" || token == "-" || token == "*" || token == "/") {
                Node* op = new Node(
                    token == "+" ? -1 :
                    token == "-" ? -2 :
                    token == "*" ? -3 : -4
                );

                if (st.size() < 2) {
                    std::cout << "Ошибка" << std::endl;
                    deleteTree(op);
                    return;
                }

                op->right = st.top(); st.pop();
                op->left = st.top(); st.pop();
                st.push(op);
            }
            else {
                int num = std::stoi(token);
                if (num < 0 || num > 9) {
                    std::cout << "Ошибка" << std::endl;
                    return;
                }
                st.push(new Node(num));
            }
        }

        if (st.size() != 1) {
            std::cout << "Ошибка" << std::endl;
            return;
        }

        root = st.top();
    }

    void display(const std::string& title = "Дерево") {
        std::cout << "\n=== " << title << " ===\n";
        if (root == nullptr) {
            std::cout << "Дерево пусто\n";
            return;
        }
        printTreeBeautiful(root, "", true);
        std::cout << std::endl;
    }

    void simplifyAdditions() {
        root = simplifyAddition(root);
    }

    Node* getRoot() { return root; }
    bool isEmpty() { return root == nullptr; }
};

class CalcTree2 {
public:
    static void Execute() {
        std::cout << "\n========== CalcTree2: ОБРАТНАЯ ПОЛЬСКАЯ ЗАПИСЬ ==========\n";

        ExpressionTree tree;

        std::cout << "\nВЫБЕРИТЕ СПОСОБ ВВОДА ДАННЫХ:\n";
        std::cout << "1. Ввод с клавиатуры\n";
        std::cout << "2. Генерация случайного выражения\n";
        std::cout << "3. Ввод из файла\n";
        std::cout << "Ваш выбор: ";

        int choice;
        std::cin >> choice;

        switch (choice) {
        case 1:
            tree.manualInput();
            break;
        case 2:
            tree.randomInput();
            break;
        case 3:
            tree.fileInput();
            break;
        default:
            std::cout << "Неверный выбор\n";
            return;
        }

        if (tree.isEmpty()) {
            std::cout << "Ошибка при построении дерева!\n";
            return;
        }

        std::cout << "\nДО ПРЕОБРАЗОВАНИЯ:";
        tree.display("Исходное дерево");

        std::cout << "\nУпрощаем поддеревья со сложением\n";
        tree.simplifyAdditions();

        std::cout << "\nПОСЛЕ ПРЕОБРАЗОВАНИЯ:";
        tree.display("Дерево без операций сложения");

        std::cout << "\nУказатель на корень: " << tree.getRoot() << std::endl;
    }
};

class ExpressionNode {
public:
    std::string value;
    ExpressionNode* left;
    ExpressionNode* right;

    ExpressionNode(const std::string& val) : value(val), left(nullptr), right(nullptr) {}
};

class ExpressionTree24 {
private:
    ExpressionNode* root;

    void deleteTree(ExpressionNode* node) {
        if (node == nullptr) return;
        deleteTree(node->left);
        deleteTree(node->right);
        delete node;
    }

    int getPriority(const std::string& op) {
        if (op == "^") return 4;
        if (op == "*" || op == "/" || op == "%") return 3;
        if (op == "+" || op == "-") return 2;
        return 1;
    }

    bool isOperator(const std::string& token) {
        return token == "+" || token == "-" || token == "*" || token == "/" || token == "%" || token == "^";
    }

    int applyOperation(int a, int b, const std::string& op) {
        if (op == "+") return a + b;
        if (op == "-") return a - b;
        if (op == "*") return a * b;
        if (op == "/") return a / b;
        if (op == "%") return a % b;
        if (op == "^") return (int)pow(a, b);
        return 0;
    }

    int evaluate(ExpressionNode* node, int x) {
        if (node == nullptr) return 0;

        if (!isOperator(node->value)) {
            if (node->value == "x") return x;
            return std::stoi(node->value);
        }

        int leftVal = evaluate(node->left, x);
        int rightVal = evaluate(node->right, x);

        return applyOperation(leftVal, rightVal, node->value);
    }

    void printTreeBeautiful(ExpressionNode* node, std::string prefix = "", bool isLeft = true) {
        if (node == nullptr) return;

        std::cout << prefix;
        std::cout << (isLeft ? "|-- " : "|__ ");
        std::cout << node->value << std::endl;

        std::string newPrefix = prefix + (isLeft ? "|   " : "    ");
        printTreeBeautiful(node->left, newPrefix, true);
        printTreeBeautiful(node->right, newPrefix, false);
    }

    ExpressionNode* swapXMul(ExpressionNode* node) {
        if (node == nullptr) return nullptr;

        node->left = swapXMul(node->left);
        node->right = swapXMul(node->right);

        if (node->value == "*") {
            if (node->left != nullptr && node->left->value == "x" && node->right != nullptr) {
                ExpressionNode* temp = node->left;
                node->left = node->right;
                node->right = temp;
                std::cout << "  Заменено x * A на A * x\n";
            }
        }

        return node;
    }

    std::vector<std::string> infixToRPN(const std::string& expression) {
        std::vector<std::string> output;
        std::stack<std::string> operators;

        std::stringstream ss(expression);
        std::string token;

        while (ss >> token) {
            if (token == "(") {
                operators.push(token);
            }
            else if (token == ")") {
                while (!operators.empty() && operators.top() != "(") {
                    output.push_back(operators.top());
                    operators.pop();
                }
                if (!operators.empty()) operators.pop();
            }
            else if (isOperator(token)) {
                while (!operators.empty() && operators.top() != "(" &&
                    getPriority(operators.top()) >= getPriority(token)) {
                    output.push_back(operators.top());
                    operators.pop();
                }
                operators.push(token);
            }
            else {
                output.push_back(token);
            }
        }

        while (!operators.empty()) {
            output.push_back(operators.top());
            operators.pop();
        }

        return output;
    }

public:
    ExpressionTree24() : root(nullptr) {}
    ~ExpressionTree24() { deleteTree(root); }

    void manualInput() {
        std::cout << "Введите выражение в инфиксной форме: ";
        std::cin.ignore();
        std::string expression;
        std::getline(std::cin, expression);
        buildFromString(expression);
    }

    void randomInput() {
        std::random_device rd;
        std::mt19937 gen(rd());
        std::uniform_int_distribution<> numDist(1, 30);
        std::uniform_int_distribution<> opDist(1, 6);
        std::uniform_int_distribution<> depthDist(2, 4);

        int depth = depthDist(gen);
        std::string expression;

        for (int i = 0; i < depth; i++) {
            if (i > 0) {
                int op = opDist(gen);
                switch (op) {
                case 1: expression += " + "; break;
                case 2: expression += " - "; break;
                case 3: expression += " * "; break;
                case 4: expression += " / "; break;
                case 5: expression += " % "; break;
                case 6: expression += " ^ "; break;
                }
            }

            if (numDist(gen) % 2 == 0) {
                expression += std::to_string(numDist(gen));
            }
            else {
                expression += "x";
            }
        }

        std::cout << "Сгенерировано выражение: " << expression << std::endl;
        buildFromString(expression);
    }

    void fileInput() {
        std::string filename;
        std::cout << "Введите имя файла: ";
        std::cin >> filename;

        std::ifstream file(filename);
        if (!file.is_open()) {
            std::cout << "Ошибка при открытии файла " << filename << std::endl;
            return;
        }

        std::string line;
        std::getline(file, line);
        file.close();

        std::cout << "Из файла прочитано: " << line << std::endl;
        buildFromString(line);
    }

    void buildFromString(const std::string& expression) {
        deleteTree(root);
        root = nullptr;

        std::vector<std::string> rpn = infixToRPN(expression);
        std::stack<ExpressionNode*> st;

        for (const std::string& token : rpn) {
            if (!isOperator(token)) {
                st.push(new ExpressionNode(token));
            }
            else {
                ExpressionNode* op = new ExpressionNode(token);
                if (st.size() < 2) {
                    std::cout << "Ошибка" << std::endl;
                    deleteTree(op);
                    return;
                }
                op->right = st.top(); st.pop();
                op->left = st.top(); st.pop();
                st.push(op);
            }
        }

        if (st.size() != 1) {
            std::cout << "Ошибка" << std::endl;
            return;
        }

        root = st.top();
    }

    void display(const std::string& title = "Дерево") {
        std::cout << "\n=== " << title << " ===\n";
        if (root == nullptr) {
            std::cout << "Дерево пусто\n";
            return;
        }
        printTreeBeautiful(root, "", true);
        std::cout << std::endl;
    }

    int evaluate(int x) {
        return evaluate(root, x);
    }

    void swapXMulSubtree() {
        root = swapXMul(root);
    }

    ExpressionNode* getRoot() { return root; }
    bool isEmpty() { return root == nullptr; }
};

class CalcTree24 {
public:
    static void Execute() {
        std::cout << "\n========== CalcTree24: ИНФИКСНАЯ ФОРМА ==========\n";

        ExpressionTree24 tree;

        std::cout << "\nВЫБЕРИТЕ СПОСОБ ВВОДА ДАННЫХ:\n";
        std::cout << "1. Ввод с клавиатуры\n";
        std::cout << "2. Генерация случайного выражения\n";
        std::cout << "3. Ввод из файла\n";
        std::cout << "Ваш выбор: ";

        int choice;
        std::cin >> choice;

        switch (choice) {
        case 1:
            tree.manualInput();
            break;
        case 2:
            tree.randomInput();
            break;
        case 3:
            tree.fileInput();
            break;
        default:
            std::cout << "Неверный выбор\n";
            return;
        }

        if (tree.isEmpty()) {
            std::cout << "Ошибка при построении дерева\n";
            return;
        }

        std::cout << "\nДО ПРЕОБРАЗОВАНИЯ:";
        tree.display("Исходное дерево");

        int x;
        std::cout << "\nВведите значение переменной x: ";
        std::cin >> x;

        int result = tree.evaluate(x);
        std::cout << "\nЗначение выражения при x = " << x << ": " << result << std::endl;

        std::cout << "\nПреобразуем поддеревья x * A в A * x\n";
        tree.swapXMulSubtree();

        std::cout << "\nПОСЛЕ ПРЕОБРАЗОВАНИЯ:";
        tree.display("Дерево после преобразования");

        std::cout << "\nУказатель на корень: " << tree.getRoot() << std::endl;
    }
};

class Menu {
public:
    void Run() {
        int choice;
        do {
            std::cout << "\n========== ГЛАВНОЕ МЕНЮ ==========\n";
            std::cout << "1. CalcTree2\n";
            std::cout << "2. CalcTree24\n";
            std::cout << "0. Выход\n";
            std::cout << "Ваш выбор: ";
            std::cin >> choice;

            switch (choice) {
            case 1:
                CalcTree2::Execute();
                break;
            case 2:
                CalcTree24::Execute();
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
    return 0;
}
