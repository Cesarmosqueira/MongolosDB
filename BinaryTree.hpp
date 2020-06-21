#pragma once
#include<functional>
#include<string>
namespace Binary_Criteria { enum Ord { RID, IRD, IDR }; }
template<class T>
class Binary_Tree {
	template<typename T> 
	class Node {
	public:
		T value;
		Node* left, *right;
		Node(const T& _value) : value(_value) {
			this->left = nullptr;
			this->right = nullptr;
		}
		~Node() {
			delete left;
			delete right;
		}
	};
	Node<T>* root;
	vector<string> chained_row;
	int size;
public:
	Binary_Tree() {
		size = 0;
		root = nullptr;
	}
	~Binary_Tree() {
		clear();
		delete root;
	}
	void for_each(const function<void(T)>& action) {
		_for_each(root, action);
	}
	void fill_rows(const function<string(T)>& get_str) {
		_fill_rows(root, get_str);
	}
	void export_ofstream(const function<string(T)>& get_row, ofstream& file) {
		_export_ofstream(root, get_row, file);
	}
	void clear() {
		_clear(root);
	}
	void append(const T& value, const std::function<bool(T, T)>& crit) {
		_append(root, value, crit);
	}
	void append(const std::vector<T>& value, const std::function<bool(T, T)>& crit) {
		for (const T& v : value) {
			_append(root, v, crit);
		}
	}
	template<typename Stream>
	void order_show(const std::function<void(T)>& print_criteria, Stream& stream, Binary_Criteria::Ord o_crit = Binary_Criteria::IRD) {
		switch (o_crit) {
		case Binary_Criteria::Ord::RID: _showRID(root, print_criteria, stream); break;
		case Binary_Criteria::Ord::IRD: _showIRD(root, print_criteria, stream); break;
		case Binary_Criteria::Ord::IDR: _showIDR(root, print_criteria, stream); break;
		}
	}
	float average_of(const std::function<float(T)>& get_required_value) {
		float result = 0;
		_average_of(root, get_required_value, result);
		return result / size;
	}
	int get_distance_between(T val1, T val2) {
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
private:
	void _fill_rows(Node<T>* node, const function<string(T)>& get_str) {
		if (node) {
			_fill_rows(node->left, get_str);
			chained_row.push_back(get_str(node->value));
			_fill_rows(node->right, get_str);
		}
	}
	void _export_ofstream(Node<T>* node, const function<string(T)>& get_row, ofstream& file, Binary_Criteria::Ord o_crit = Binary_Criteria::IRD) {
		switch (o_crit) {
		case Binary_Criteria::IDR:
			if (node) {
				_export_ofstream(node->left, get_row, file, o_crit);
				_export_ofstream(node->right, get_row, file, o_crit);
				file << get_row(node->value) << "\n";
			}
			break;
		case Binary_Criteria::IRD: //default
			if (node) {
				_export_ofstream(node->left, get_row, file, o_crit);
				file << get_row(node->value) << "\n";
				_export_ofstream(node->right, get_row, file, o_crit);
			}
			break;
		case Binary_Criteria::RID:
			if (node) {
				file << get_row(node->value) << "\n";
				_export_ofstream(node->left, get_row, file, o_crit);
				_export_ofstream(node->right, get_row, file, o_crit);
			}
			break;
		}
	}
	void _for_each(Node<T>* node, const function<void(T)>& action, const Binary_Criteria::Ord& o_crit = Binary_Criteria::IRD) {
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
	void _showIRD(Node<T>* node, std::function<void(T)> crit, Stream& stream) {
		if (node) {
			_showIRD(node->left, crit, stream);
			crit(node->value); stream << "\n";
			_showIRD(node->right, crit, stream);
		}
	}
	template<typename Stream>
	void _showIDR(Node<T>* node, std::function<void(T)> crit, Stream& stream) {
		if (node) {
			_showIDR(node->left, crit, stream);
			_showIDR(node->right, crit, stream);
			crit(node->value); stream << "\n";
		}
	}
	template<typename Stream>
	void _showRID(Node<T>* node, std::function<void(T)> crit, Stream& stream) {
		if (node) {
			crit(node->value); stream << "\n";
			_showRID(node->left, crit, stream);
			_showRID(node->right, crit, stream);
		}
	}
	void _append(Node<T>*& node, const T& value, const std::function<bool(T, T)>& crit) {
		if (!node) {
			node = new Node<T>(value);
			size++;
			return;
		}
		else if (crit(value, node->value)) {
			_append(node->right, value, crit);
		}
		else {
			_append(node->left, value, crit);
		}
	}
	void _clear(Node<T>* node) {
		if (node == nullptr) return;
		_clear(node->left);
		_clear(node->right);
		delete node;
	}
	void _average_of(Node<T>* node, const std::function<float(T)>& crit, float& result) {
		if (!node) return;
		_average_of(node->left, crit, result);
		result += crit(node->value);
		_average_of(node->right, crit, result);
	}
	int _calc_distance(Node<T> *node, T val1, T n2) {
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
	int _find_depth(Node<T>* root, T value, int level)
	{
		if (root == nullptr)
			return -1;
		if (root->value == value)
			return level;

		int l = _find_depth(root->left, value, level + 1);
		return (l != -1) ? l : _find_depth(root->right, value, level + 1);
	}
	Node<T> *_LCA(Node<T>* node, T val1, T val2, int &dist1, int &dist2, int &distance, int lvl) {
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