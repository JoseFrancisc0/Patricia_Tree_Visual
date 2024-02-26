#include "patricia_tree.h"

string PatriciaTree::getCommonPrefix(const string& str1, const string& str2){
    size_t index = 0;
    while(index < str1.length() && index < str2.length() && str1[index] == str2[index])
        index++;
    return str1.substr(0, index);
};

Node* PatriciaTree::getRoot(){
    return root;
}

void PatriciaTree::insert(string key){
    if (root == nullptr)
        root = new Node();

    Node* current = root;
    size_t index = 0;

    // Buscamos key en tree
    while(index < key.length()){
        bool inserted = false;

        for(auto& pair : current->children){
            string prefix = pair.first;

            // Word tiene un prefijo exacto en tree?
            if(key.substr(index).find(prefix) == 0){ // Caso 1
                current = pair.second;
                index += prefix.length();
                inserted = true;
                break;
            }

            string commonPrefix = getCommonPrefix(prefix, key.substr(index));
            if(!commonPrefix.empty()){ // Caso 2 / 3: Match parcial de word en tree (word es prefijo / word tiene prefijo parcial)
                // Spliteamos nodos
                Node* newNode = new Node();
                newNode->children[prefix.substr(commonPrefix.length())] = current->children[prefix];
                current->children.erase(prefix);
                current->children[commonPrefix] = newNode;

                // Actualizar las flags de endWord
                if(commonPrefix.length() == prefix.length()){
                    // Caso 4 : word esta en tree
                    newNode->endWord = current->children[commonPrefix]->endWord;
                    current->children[commonPrefix]->endWord = false;
                }

                // Actualizar current
                current = newNode;
                index += commonPrefix.length();
                inserted = true;
                break;
            }
        }

        if(!inserted){
            // Caso 0: word no esta en tree
            current->children[key.substr(index)] = new Node();
            current = current->children[key.substr(index)];
            current->endWord = true;
            break;
        }
    }

    current->endWord = true; // Completamos la insercion
}

void PatriciaTree::remove(string key){
    if (root == nullptr)
        return;

    Node* current = root;
    Node* parent = nullptr;
    Node* grandparent = nullptr;
    string parentKey;
    string grandparentKey;

    // Buscamos dentro del tree
    while (current != nullptr && !current->children.empty() && key.length() > 0) {
        bool found = false;

        for (auto& pair : current->children) {
            string prefix = pair.first;

            if (key.find(prefix) == 0) {
                grandparent = parent;
                grandparentKey = parentKey;
                parent = current;
                parentKey = prefix;
                current = pair.second;
                key = key.substr(prefix.length());
                found = true;
                break;
            }

            string commonPrefix = getCommonPrefix(prefix, key);
            if (!commonPrefix.empty()) {
                grandparent = parent;
                grandparentKey = parentKey;
                parent = current;
                parentKey = prefix;
                current = pair.second;
                key = key.substr(commonPrefix.length());
                found = true;
                break;
            }
        }

        if (!found)
            return; // No esta en tree
    }

    // Hacemos los removes
    if (current != nullptr && current->endWord) {
        current->endWord = false;

        if (current->children.empty()) {
            delete current; // Borrado
            parent->children.erase(parentKey);

            // Si necesitamos comprimir
            if (parent != root && parent->children.size() == 1 && !parent->endWord) {
                auto childPair = *(parent->children.begin());
                string childPrefix = childPair.first;
                Node* childNode = childPair.second;

                // Mergeamos
                string mergedPrefix = grandparentKey + childPrefix;
                grandparent->children.erase(grandparentKey);
                grandparent->children[mergedPrefix] = childNode;

                delete parent; // Borrado
            }
        }
    }
}

bool PatriciaTree::search(string key){
    if(root == nullptr)
        return false;
    
    Node* current = root;
    int index = 0;

    // Empezamos la busqueda
    while(current != nullptr && index < key.length()){
        bool found = false;

        for(auto& pair : current->children){
            string prefix = pair.first;

            if(key.substr(index).find(prefix) == 0){ // Encontramos prefijo para word
                // Vamos para el hijo
                current = pair.second;
                index += prefix.length();
                found = true;
                break;
            }

            string commonPrefix = getCommonPrefix(prefix, key.substr(index));
            if(!commonPrefix.empty() && commonPrefix == prefix){ // Hay match parcial para word
                // Vamos para el hijo
                current = pair.second;
                index += commonPrefix.length();
                found = true;
                break;
            }
        }

        if(!found) 
            return false; // word no aparece en tree
    }

    return current != nullptr && current->endWord; // El nodo representa una palabra ?
}

bool PatriciaTree::visualSearch(string key, vector<Node*>& searchPath){
    Node* current = root;
    int index = 0;

    // Traverse the tree to find the node containing the key
    while (current != nullptr && index < key.length()) {
        searchPath.push_back(current); // Add the current node to the search path

        bool found = false;
        for (auto& pair : current->children) {
            const std::string& prefix = pair.first;

            // Check if the prefix matches the remaining part of the key
            if (key.substr(index).find(prefix) == 0) {
                current = pair.second; // Move to the child node
                index += prefix.length();
                found = true;
                break;
            }

            // Check for common prefix
            std::string commonPrefix = getCommonPrefix(prefix, key.substr(index));
            if (!commonPrefix.empty() && commonPrefix == prefix) {
                current = pair.second; // Move to the child node
                index += commonPrefix.length();
                found = true;
                break;
            }
        }

        if (!found)
            return false; // Key does not exist in the tree
    }

    // Check if the remaining key matches the current node and the node is marked as end of word
    if(current != nullptr && current->endWord && index == key.length()){
        searchPath.push_back(current);
        return true;
    }
    
    return false;
}