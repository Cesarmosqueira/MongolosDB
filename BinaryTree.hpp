#pragma once
#include<functional>
#include <list>
#include<string>
#include <vector>
using namespace std;
namespace Binary_Criteria { enum Ord { RID, IRD, IDR }; }

template<class T>
class Binary_Tree {
	template<typename T> 
	struct Node {
		int height;
		T value;
		Node<T>* left, *right;
		Node(const T& _value) : value(_value) {
			this->left = nullptr;
			this->right = nullptr;
			this->height = 0;
		}
		~Node() {
			if (left)  delete left;
			if (right) delete right;
		}
	};
	std::list<T> linear;
	Node<T>* root;
	int size;
public:
	Binary_Tree() : size(0), root(nullptr) {
		linear.clear();
	}
	~Binary_Tree() {
		clear();
	}
	void for_each(const function<void(const T&)>& action) {
		_for_each(root, action);
	}
	void load_linear(const Binary_Criteria::Ord& o_crit = Binary_Criteria::IRD) {
		linear.clear();
		_load_linear(root, o_crit);
	}
	void clear() {
		_clear(root);
		linear.clear();
	}
	void insert(const T& value, const function<bool(const T&, const T&)>& crit) {
		_insert(root, value, crit);
	}
	void insert(const vector<T>& value, const function<bool(const T&, const T&)> &crit = [](const T&, const T&)->bool {return true; }) {
		for (const T& v : value) {
			_insert(root, v, crit);
		}
	}
	template<typename Stream>
	void order_show(const std::function<void(const T&)>& print_criteria, Stream& stream, const Binary_Criteria::Ord& o_crit = Binary_Criteria::IRD) {
		switch (o_crit) {
		case Binary_Criteria::Ord::RID: _showRID(root, print_criteria, stream); break;
		case Binary_Criteria::Ord::IRD: _showIRD(root, print_criteria, stream); break;
		case Binary_Criteria::Ord::IDR: _showIDR(root, print_criteria, stream); break;
		}
	}
	float average_of(const std::function<float(const T&)>& get_required_value) {
		float result = 0;
		_average_of(root, get_required_value, result);
		return result / size;
	}
	int get_distance_between(const T& val1, const T& val2) {
		return _calc_distance(root, val1, val2);
	}
	T max() {
		Node<T>* aux = root;
		while (aux->right) {
			aux = aux->right;
		}
		return aux->value;
	}
	int get_size() { return size; }
	void print_linear() {
		for (auto x : linear)cout << x << endl;
	}
	list<T> get_linear() { return linear; }
	void print2D(function<void(T)> print_crit) {
		// Pass initial space count as 0  
		_print2D(root, 0, print_crit);
	}
private:
	void _left_rotate(Node<T>* x, Node<T>* y, Node<T>*& p) {
		if (y) {
			p = y;
			x->right = y->left;
			y->left = x;
		}
	}
	void _right_rotate(Node<T>* x, Node<T>* y, Node<T>*& p) {
		p = x;
		y->left = x->right;
		x->right = y;
	}
	int _get_height(Node<T>* node) {
		if (!node) return -1;
		return node->height;
			//int left_height = _get_height(node->left);
			//int right_height = _get_height(node->right);
			//1 + (left_height > right_height ? left_height : right_height);
	}
	void _print2D(Node<T> *node, int space, function<void(T)> print_crit) {
		if (node == nullptr) return;
		space += 10;
		_print2D(node->right, space, print_crit);
		cout << "\n";
		for (int i = 10; i < space; i++)
			cout << " ";
		print_crit(node->value);
		_print2D(node->left, space, print_crit);
	}
	void _load_linear(Node<T>* node, const Binary_Criteria::Ord& o_crit = Binary_Criteria::IRD) {
		switch (o_crit) {
		case Binary_Criteria::RID:
			if (node) {
				linear.push_back(node->value);
				_load_linear(node->left, o_crit);
				_load_linear(node->right, o_crit);
			}
			break;
		case Binary_Criteria::IRD:
			if (node) {
				_load_linear(node->left, o_crit);
				linear.push_back(node->value);
				_load_linear(node->right, o_crit);
			}
			break;
		case Binary_Criteria::IDR:
			if (node) {
				_load_linear(node->left, o_crit);
				_load_linear(node->right, o_crit);
				linear.push_back(node->value);
			}
			break;
		}
	}
	void _for_each(Node<T>* node, const function<void(const T&)>& action, const Binary_Criteria::Ord& o_crit = Binary_Criteria::IRD) {
		switch (o_crit) {
		case Binary_Criteria::IDR:
			if (node) {
				_for_each(node->left, action);
				_for_each(node->right, action);
				action(node->value);
			}
			break;
		case Binary_Criteria::IRD: //default
			if (node) {
				_for_each(node->left, action);
				action(node->value);
				_for_each(node->right, action);
			}
			break;
		case Binary_Criteria::RID:
			if (node) {
				action(node->value);
				_for_each(node->left, action);
				_for_each(node->right, action);
			}
			break;
		}
	}
	template<typename Stream>
	void _showIRD(Node<T>* node, const std::function<void(const T&)>& crit, Stream& stream) {
		if (node) {
			_showIRD(node->left, crit, stream);
			crit(node->value); stream << "\n";
			_showIRD(node->right, crit, stream);
		}
	}
	template<typename Stream>
	void _showIDR(Node<T>* node, const std::function<void(const T&)>& crit, Stream& stream) {
		if (node) {
			_showIDR(node->left, crit, stream);
			_showIDR(node->right, crit, stream);
			crit(node->value); stream << "\n";
		}
	}
	template<typename Stream>
	void _showRID(Node<T>* node, const std::function<void(const T&)>& crit, Stream& stream) {
		if (node) {
			crit(node->value); stream << "\n";
			_showRID(node->left, crit, stream);
			_showRID(node->right, crit, stream);
		}
	}
	void _insert(Node<T>*& node, const T& value, const std::function<bool(const T&, const T&)>& crit) {
		if (!node) {
			node = new Node<T>(value);
			size++;
		}
		else if (crit(value, node->value)) {
			_insert(node->right, value, crit);
		}
		else {
			_insert(node->left, value, crit);
		}
		//AVL AVL AVL AVL AVL AVL AVL AVL AVL AVL AVL 
		int balance = _get_height(node->right) - _get_height(node->left);
		if (1 < balance) { //right case
			int right_right_height = _get_height(node->right->right);
			int right_left_height = _get_height(node->right->left);
			if (right_left_height > right_right_height) //right left case
				_right_rotate(node->right->left, node->right, node->right); 
			_left_rotate(node, node->right, node);//default
		}
		else if (balance <-1) { //left case
			int left_left_height = _get_height(node->left->left);
			int left_right_height = _get_height(node->left->right);
		
			if (left_left_height > left_right_height) //left right case
				_left_rotate(node->left->left, node->left->right, node->left);
			_right_rotate(node->left, node, node);//default
		}
		int left_height = _get_height(node->left);
		int right_height = _get_height(node->right);
		node->height = 1 + (left_height > right_height ? left_height : right_height);
	}
	void _clear(Node<T>* node) {
		if (node) {
			_clear(node->left);
			_clear(node->right);
			delete node;
		}
	}
	void _average_of(Node<T>* node, const std::function<float(const T&)>& crit, float& result) {
		if (!node) return;
		_average_of(node->left, crit, result);
		result += crit(node->value);
		_average_of(node->right, crit, result);
	}
	int _calc_distance(Node<T> *node, const T& val1, const T& n2) {
		int dist1 = -1, d2 = -1, dist;
		Node<T> *lca = _LCA(node, val1, n2, dist1, d2,
			dist, 1);
		if (dist1 != -1 && d2 != -1)
			return dist;
		if (dist1 != -1) {
			dist = _find_depth(lca, n2, 0);
			return dist;
		}
		if (d2 != -1) {
			dist = _find_depth(lca, val1, 0);
			return dist;
		}
		return -1;
	}
	//Node operations
	int _find_depth(Node<T>* root, const T& value, const int& level)
	{
		if (root == nullptr)
			return -1;
		if (root->value == value)
			return level;

		int l = _find_depth(root->left, value, level + 1);
		return (l != -1) ? l : _find_depth(root->right, value, level + 1);
	}
	Node<T> *_LCA(Node<T>* node, const T& val1, const T& val2, int &dist1, int &dist2, int &distance, int lvl) {
		// Base case 
		if (node == nullptr) return nullptr;

		if (node->value == val1) {
			dist1 = lvl;
			return node;
		}
		if (node->value == val2) {
			dist2 = lvl;
			return node;
		}
		Node<T> *left_LCA = _LCA(node->left, val1, val2,
			dist1, dist2, distance, lvl + 1);
		Node<T> *right_LCA = _LCA(node->right, val1, val2,
			dist1, dist2, distance, lvl + 1);
		if (left_LCA && right_LCA)
		{
			distance = dist1 + dist2 - 2 * lvl;
			return node;
		}
		return (left_LCA != nullptr) ? left_LCA : right_LCA;
	}
};
