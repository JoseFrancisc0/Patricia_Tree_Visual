#ifndef PATRICIA_TREE_H
#define PATRICIA_TREE_H

#include <list>
#include <string>
#include <vector>
#include <unordered_map>
using namespace std;

struct Node{
    unordered_map<string, Node*> children;
    bool endWord;

    Node(): endWord(false){};
};

class PatriciaTree{
    private:
        Node* root;

        string getCommonPrefix(const string& str1, const string& str2);

    public:
        PatriciaTree(): root(nullptr){};

        Node* getRoot();

        void insert(string key);
        void remove(string key);
        bool search(string key);

        bool visualSearch(string key, vector<Node*>& searchPath);

        ~PatriciaTree() = default;
};


#endif //PATRICIA_TREE_H