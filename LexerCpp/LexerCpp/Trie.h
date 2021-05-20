#pragma once
#include <unordered_map>

struct TrieNode {
    std::unordered_map<char, TrieNode*> map;
    bool isEndOfTheWord;

    TrieNode() {
        isEndOfTheWord = false;
        map = {};
    }
};

class Trie {
private:
    TrieNode* head;

public:
    Trie() {
        head = new TrieNode();
    }

    void insert(std::string word) {
        char x;
        TrieNode* temp = head;
        for (int i = 0; i < word.length(); i++) {
            x = word[i];
            if (temp->map.find(x) == temp->map.end()) {
                temp->map[x] = new TrieNode();
            }
            temp = temp->map[x];
        }
        temp->isEndOfTheWord = true;
    }

    bool search(std::string word) {
        char c;
        TrieNode* temp = head;
        for (int i = 0; i < word.length(); i++) {
            c = word[i];
            if (temp->map.find(c) == temp->map.end()) {
                return false;
            }
            temp = temp->map[c];
        }
        return temp->isEndOfTheWord;
    }
};
