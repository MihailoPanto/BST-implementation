#include <string>
#include <iostream>
#include <iomanip>
#include <fstream>
#include <vector>
#include <stack>
#include <queue>

using namespace std;

class TreeNode {
public:
	TreeNode() : parent(nullptr), left(nullptr), right(nullptr) { key = ""; translation = ""; }
	TreeNode(string key, string word) : key(key), translation(word), parent(nullptr), left(nullptr), right(nullptr) {}
	string key;
	string translation;
	TreeNode* parent;
	TreeNode* left;
	TreeNode* right;
};

class BST {
public:
	BST() { root = nullptr; }

	BST(vector<string>& words, vector<string>& translation);

	//1. Formiranja stabla na osnovu zadatog skupa reci i njihovog prevoda
	BST(string file_name) {
		string line;

		ifstream read_file(file_name);

		while (getline(read_file, line)) {
			int pos = line.find(' ');
			string word = line.substr(0, pos);
			string translation = line.substr(pos + 1);
			insert(word, translation);
		}
		read_file.close();
	}

	//2. Pretragu stabla na zadatu rec i dohvatanje prevoda
	vector<string> search(string word);
	//3. Umetanje u stablo novog para reci
	bool insert(string word, string translation);
	//4. Ispis sadržaja stabla
	void print_tree();
	//5. Brisanje zadatog kljuca
	bool erase(string key);

	//6. Brisanje stabla iz memorije
	~BST() {
		if (root == nullptr) return;

		TreeNode* temp = root;
		stack<TreeNode*> s;
		s.push(temp);

		while (!s.empty()) {
			temp = s.top();
			s.pop();
			if (temp->left) s.push(temp->left);
			if (temp->right) s.push(temp->right);
			delete temp;
		}
	}

	void delete_tree() {
		if (root == nullptr) return;
		TreeNode* temp = root;
		stack<TreeNode*> s;
		s.push(temp);

		while (!s.empty()) {
			temp = s.top();
			s.pop();
			if (temp->left) s.push(temp->left);
			if (temp->right) s.push(temp->right);
			delete temp;
		}
		root = nullptr;
	}

	//7. Pronalazenje najkrace reci sa kojom zadata rec ima zajednicki prefiks
	string find_shortest_word(string word);


	void print_tree_horizontally() {
		print_tree_horizontally(this->root);
	}
	void print_inorder();

private:
	TreeNode* root;
	static TreeNode* min_val(TreeNode* curr);
	static void print_tree_horizontally(TreeNode* root);
	int tree_height();
};

BST::BST(vector<string>& words, vector<string>& translations) {
	int n = words.size();
	for (int i = 0; i < n; ++i) {
		insert(words[i], translations[i]);
	}
}

vector<string> BST::search(string word) {

	TreeNode* temp = root;
	vector<string> translations;
	while (temp != nullptr) {
		if (word > temp->key) {
			temp = temp->right;
		}
		else {
			if (word == temp->key) {
				translations.push_back(temp->translation);
			}
			temp = temp->left;
		}
	}
	return translations;
}

bool BST::insert(string word, string translation) {
	TreeNode* new_node = new TreeNode(word, translation);
	TreeNode* temp = root, * p = nullptr, * curr = nullptr;

	while (temp != nullptr) {
		if (word == temp->key) {
			curr = temp;
			temp = temp->left;
		}
		else {
			p = temp;
			if (word > temp->key) {
				temp = temp->right;
			}
			else {
				temp = temp->left;
			}
		}
	}
	if (curr != nullptr) {
		temp = curr;
		if (temp->left == nullptr) {
			temp->left = new_node;
			new_node->parent = temp;
		}
		else {
			temp = temp->left;
			while (temp->right != nullptr) {
				temp = temp->right;
			}
			temp->right = new_node;
			new_node->parent = temp;
		}
		return true;
	}
	if (p == nullptr) {
		root = new_node;
	}
	else if (p->key < word) {
		p->right = new_node;
	}
	else {
		p->left = new_node;
	}
	new_node->parent = p;
	return true;
}

void BST::print_inorder() {
	stack<TreeNode*> s;
	TreeNode* curr = root;

	while (curr != nullptr || !s.empty()) {
		while (curr != nullptr) {
			s.push(curr);
			curr = curr->left;
		}
		curr = s.top();
		s.pop();

		cout << (curr->key) << " " << (curr->translation) << endl;
		curr = curr->right;
	}
}

TreeNode* BST::min_val(TreeNode* curr) {
	TreeNode* temp = curr;
	while (temp->left) {
		temp = temp->left;
	}
	return temp;
}

bool BST::erase(string key) {
	if (root == nullptr) return true;
	TreeNode* temp = root;
	while (temp != nullptr) {

		if (key > temp->key) {
			temp = temp->right;
		}
		else {
			if (key < temp->key) {
				temp = temp->left;
			}
			else {
				TreeNode* deletion_node = temp;
				TreeNode* p = temp->parent;

				if (temp->left == nullptr && temp->right == nullptr) {
					if (p != nullptr) {
						if (p->left == temp) p->left = nullptr;
						else p->right = nullptr;
					}
					if (root == deletion_node) root = nullptr;
					delete deletion_node;
					return true;
				}
				else {
					if (temp->left == nullptr) {
						TreeNode* next = temp->right;
						if (p != nullptr) {
							if (p->left == temp) p->left = temp->right;
							else p->right = temp->right;
						}
						next->parent = p;
						if (root == deletion_node) root = next;
						delete deletion_node;
						temp = next;
					}
					else
						if (temp->right == nullptr) {
							TreeNode* next = temp->left;
							if (p != nullptr) {
								if (p->left == temp) p->left = temp->left;
								else p->right = temp->left;
							}
							next->parent = p;
							if (root == deletion_node) root = next;
							delete deletion_node;
							temp = next;
						}
						else {
							TreeNode* succ = min_val(temp->right);
							temp->key = succ->key;
							temp->translation = succ->translation;
							TreeNode* p = succ->parent;
							if (p != nullptr) {
								if (p->left == succ) p->left = succ->right;
								else p->right = succ->right;
							}
							delete succ;
							temp = temp->left;
						}
				}
			}
		}
	}
	return true;
}

string BST::find_shortest_word(string word) {
	if (word.empty()) return "";

	string ret;
	int min_len = INT_MAX;
	stack<TreeNode*> s;
	TreeNode* curr = root;
	s.push(curr);

	while (!s.empty()) {
		curr = s.top();
		s.pop();

		if (curr->key[0] == word[0]) {
			if (curr->key.length() < min_len) {
				ret = curr->key;
				min_len = curr->key.length();
			}
		}

		if (curr->key[0] > word[0] && curr->left != nullptr) {
			s.push(curr->left);
		}
		else {
			if (curr->key[0] < word[0] && curr->right != nullptr) {
				s.push(curr->right);
			}
			else {
				if (curr->left != nullptr) s.push(curr->left);
				if (curr->right != nullptr) s.push(curr->right);
			}
		}
	}
	return ret;
}

int BST::tree_height() {
	queue<TreeNode*> q;
	int height = 0;

	TreeNode* temp = root;
	q.push(temp);

	while (!q.empty()) {
		int n = q.size();
		height++;
		for (int i = 0; i < n; ++i) {
			TreeNode* curr = q.front();
			q.pop();
			if (curr->left) q.push(curr->left);
			if (curr->right) q.push(curr->right);
		}
	}
	return height;
}

void BST::print_tree_horizontally(TreeNode* root) {
	stack<TreeNode*> node_stack;
	stack<pair<string, bool>> prefix_stack;
	if (root == nullptr) return;

	node_stack.push(root);
	prefix_stack.push({ "", false });

	while (!node_stack.empty()) {
		string prefix = prefix_stack.top().first;
		bool is_left = prefix_stack.top().second;
		prefix_stack.pop();

		std::cout << prefix;
		std::cout << (is_left ? "|--" : "--");

		TreeNode* curr = node_stack.top();
		node_stack.pop();
		std::cout << curr->key << " " << curr->translation << std::endl;

		if (curr->right) {
			node_stack.push(curr->right);
			prefix_stack.push({ prefix + (is_left ? "|   " : "    "), false });
		}
		if (curr->left) {
			node_stack.push(curr->left);
			prefix_stack.push({ prefix + (is_left ? "|   " : "    "), true });
		}
	}
}

void BST::print_tree() {
	if (root == nullptr) return;
	queue<TreeNode*> q_w;
	queue<TreeNode*> q_t;
	int height = tree_height();
	int line_len = 120;
	int first_skip = line_len, in_between_skip;

	q_w.push(root);
	q_t.push(root);

	for (int i = 0; i < height; i++) {
		int j = 1 << i, k, l;  // pow(2, i)
		in_between_skip = first_skip; // (line_len - (j + 1) * 6) / j; 
		first_skip = (first_skip - 2) / 2;
		for (k = 0; k < first_skip; k++) cout << " ";
		for (k = 0; k < j; k++) {
			TreeNode* node = q_w.front();
			q_w.pop();
			if (node != nullptr) {
				q_w.push(node->left);
				q_w.push(node->right);
			}
			else {
				q_w.push(nullptr);
				q_w.push(nullptr);
			}
			if (node != nullptr) cout << setw(6) << node->key;
			else cout << "      ";
			for (l = 0; l < in_between_skip; l++) cout << " ";
		}
		cout << endl;
		for (k = 0; k < first_skip; k++) cout << " ";
		for (k = 0; k < j; k++) {
			TreeNode* node = q_t.front();
			q_t.pop();
			if (node != nullptr) {
				q_t.push(node->left);
				q_t.push(node->right);
			}
			else {
				q_t.push(nullptr);
				q_t.push(nullptr);
			}
			if (node != nullptr) cout << setw(6) << node->translation;
			else cout << "      ";
			for (l = 0; l < in_between_skip; l++) cout << " ";
		}

		cout << endl;
	}
}

int main() {
	int option = 1;
	string file_name;

	BST* tree = nullptr;

	string word1, word2;
	vector<string> words, translations;
	int n;

	while (option != 0) {
		cout << endl;
		cout << "1. Formiranja stabla sa standardnog ulaza" << endl;
		cout << "2. Pretraga stabla na zadatu rec i dohvatanje prevoda" << endl;
		cout << "3. Umetanje u stablo novog para reci" << endl;
		cout << "4. Ispis sadrzaja stabla" << endl;
		cout << "5. Brisanje zadatog kljuca" << endl;
		cout << "6. Brisanje stabla iz memorije" << endl;
		cout << "7. Nalazenje najkrace reci sa istim prefiksom kao trazena rec" << endl;
		cout << "8. Formiranje stabla iz fajla" << endl;
		cout << "9. Ispis inorder poretka" << endl;
		cout << "0. Izlazak" << endl;
		cout << "Izaberite opciju: ";
		cin >> option;

		switch (option) {
			case 1: cout << "Unesite broj parova: ";
				cin >> n;
				for (int i = 0; i < n; ++i) {
					cout << "Unesite rec: ";
					cin >> word1;
					cout << "Unesite prevod: ";
					cin >> word2;
					words.push_back(word1);
					translations.push_back(word2);
				}
				tree = new BST(words, translations);
				break;

			case 2: cout << "Unesite rec za pretragu: ";
				cin >> word1;
				translations = tree->search(word1);

				for (string s : translations) {
					cout << s << " ";
				}
				cout << endl;
				break;

			case 3:
				cout << "Unesite rec: ";
				cin >> word1;
				cout << "Unesite prevod: ";
				cin >> word2;
				tree->insert(word1, word2);
				break;

			case 4: //tree->print_tree();
				tree->print_tree();
				break;

			case 5: printf("Unesite kljuc: ");
				cin >> word1;
				tree->erase(word1);
				break;

			case 6: tree->delete_tree();
				break;

			case 7:
				cout << "Unesite rec: ";
				cin >> word2;
				cout << "Najkraca rec u stablu je: ";
				cout << tree->find_shortest_word(word2);
				break;

			case 8: cout << "Unesite naziv fajla: ";
				cin >> file_name;
				tree = new BST(file_name);
				break;

			case 9:
				tree->print_inorder();
				break;

			case 10:
				tree->print_tree_horizontally();
				break;
			default: break;
		}
	}

	

	return 0;
}
