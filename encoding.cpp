#include <iostream>
#include <queue>
#include <string>
#include <unordered_map>
#include <fstream>
#include <vector>
using namespace std;

struct Node{
	Node* ind_left,* ind_right;
	int freq;
	char val;
};

struct Comp {
	bool operator()(Node* lhs, Node* rhs)const {
		return lhs->freq > rhs->freq;
	}
};


unordered_map<char, string> code;
unordered_map<char, int> freq;

void dfs(Node* cur, string& str) {
	if (cur->ind_left != NULL) {
		str += '0';
		dfs(cur->ind_left, str);
		str.pop_back();
		str += '1';
		dfs(cur->ind_right, str);
		str.pop_back();
		return;
	}
	code[cur->val] = str;
}


int main() {
	ifstream input("input.txt");
	if (!input) {
		cout << "FILE NOT OPENED";
		return 0;
	}
	char x;
	while(input >> noskipws >> x) {
		freq[x]++;
	}
	input.close();

	priority_queue<Node*, vector<Node*>, Comp>  queue;
	vector<Node> nodes;
	nodes.reserve(1000);
	for (auto& val : freq) {
		nodes.push_back({NULL, NULL, val.second, val.first});
		queue.push(&nodes.back());

	}

	while (queue.size() != 1) {
		Node* f1 = queue.top();
		queue.pop();
		Node* f2 = queue.top();
		queue.pop();
		nodes.push_back({f1, f2, f1->freq + f2->freq, 0});
		queue.push(&nodes.back());
	}

	Node* root = queue.top();
	string s;
	dfs(root, s);

	input.open("input.txt");
	string outp;
	while(input >> x) {
		outp += code[x];
	}
	outp += '1';
	while (outp.size() % 8) {
		outp += '0';
	}
	cout << outp;
	for (int i = 0;i < outp.size() / 8;++i) {
		x = 0;
		for (int j = 0;j < 8;++j) {
			x = x << 1;
			if (outp[8 * i + j] == '1')
				++x;
		}
		outp[i] = x;
	}
	outp.resize(outp.size() / 8);
	input.close();
	ofstream shifr("shifr.bin", ios::binary);
	shifr << outp;


	string to_decode;
	for (char y : outp) {
		for (int j = 7;j >= 0;--j) {
			if (y & (1 << j)) {
				to_decode += '1';
			} else {
				to_decode += '0';
			}
		}
	}
	while (to_decode.back() == '0')
		to_decode.pop_back();
	to_decode.pop_back();
	ofstream out("output.bin", ios::binary);
	Node* cur_node = root;
	for (char el :  to_decode) {
		if (el == '1') {
			cur_node = cur_node->ind_right;
		} else {
			cur_node = cur_node->ind_left;
		}
		if (!cur_node->ind_left) {
			out << cur_node->val;
			cur_node = root;
		}
	}
	return 0;
}
