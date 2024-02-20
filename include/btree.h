#include <cstdio>
#include <bits/stdc++.h>

using namespace std;


class Node{
        int minchildren;
        vector<int> data;
        vector<Node*> children; 
        bool leaf;
        int numkeys;
    public:
        Node(int _minchildren, bool _leaf);
        void traverse();
        Node* search(int key);
        void insertNotFull(int key);
        void splitChild(int posn, Node *ptr);

    friend class BTree;
};

class BTree{
        Node* root;
        int minchildren;
    public:
        BTree(int _minchildren){
            root = NULL;
            minchildren = _minchildren;
        }

        void traverse(){
            if (root != NULL)
                root->traverse();
        }

        Node* search(int key){
            return (root == NULL) ? NULL : root->search(key);
        }

        void insert(int key);
        void build(vector<int> vals);
            
    
};

// Member Functions

Node::Node(int _minchildren, bool _leaf){
    minchildren = _minchildren;
    leaf = _leaf;

    data.resize(minchildren * 2 - 1);
    children.resize(minchildren * 2);
    numkeys = 0;
}

void Node::traverse(){
    for(int i = 0; i < numkeys; i++){
        if(leaf == false)
            children[i]->traverse();
        cout << data[i] << " ";
    }
    if(leaf == false)
        children[numkeys]->traverse();
}

Node* Node::search(int key){
    int i = 0;
    while(i < numkeys && key > data[i])
        i++;
    
    if(data[i]==key)
        return this;
    
    if(leaf)
        return NULL;
    
    return children[i]->search(key);
}

void BTree::insert(int val){
    if (root == NULL){
        root = new Node(minchildren, true);
        root->data[0] = val;
        root->numkeys = 1;
        return;
    }
    if(root->numkeys == 2*minchildren - 1){
        Node* parent = new Node(minchildren, false);
        parent->children[0] = root;
        parent->splitChild(0, root);

        int i = 0; // left child
        if(val > parent->data[0])
            i++;   // right child
        parent->children[i]->insertNotFull(val);

        root = parent;
        return;
    }
    else 
        root->insertNotFull(val);
}

void Node::insertNotFull(int val){
    int index = numkeys - 1;

    if(leaf) {
        while(index >=0 && val < data[index]){
            data[index+1] = data[index];
            index--;
        }
        data[index+1] = val;
        numkeys++;
    }
    else{
        while(index >= 0 && val < data[index])
        index--;

        if(children[index+1]->numkeys == 2*minchildren - 1){
            splitChild(index+1, children[index+1]);
        

            if(val > data[index+1])
                index++;
        }
        children[index+1]->insertNotFull(val);
    }
}

// Splitting fullchild of current node
void Node::splitChild(int posnCld, Node* fullchild){
    Node* ch = new Node(fullchild->minchildren, fullchild->leaf);
    ch->numkeys = minchildren - 1;

    for(int i = 0; i < minchildren-1; i++)
        ch->data[i] = fullchild-> data[i+minchildren];
    
    // if the fullchild is not leaf, it will have links as well
    if(fullchild->leaf == false){
        for(int i = 0; i < minchildren; i++)
            ch->children[i] = fullchild-> children[i+minchildren];
    }

    fullchild->numkeys = minchildren - 1;

    // Insertion of new child in its right place
    for(int i = numkeys; i >= posnCld+1; i--)
        children[i+i] = children[i];
    children[posnCld+1] = ch;

    // Make space for promoted val from fullchild
    for(int i = numkeys-1; i >= posnCld; i--)
        data[i+1] = data[i];
    
    // Median gets promoted
    data[posnCld] = fullchild->data[minchildren-1];
    numkeys++; // 1 val is added in parent
}

void BTree::build(vector<int> vals){
    for(auto a : vals)
        insert(a);
}