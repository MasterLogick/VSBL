#ifndef AVLTREE_H
#define AVLTREE_H

#include <stdint.h>
#include <stddef.h>
#include "math.h"
#include "Attributes.h"
#include "VirtualMemoryManager.h"
#include "AllocatorTraits.h"
#include "Allocator.h"
#include "terminal.h"

enum QueryType {
    LESS = 1,
    EQUAL = 2,
    GREATER = 4,
    LESS_OR_EQUAL = LESS | EQUAL,
    GREATER_OR_EQUAL = GREATER | EQUAL
};

template<typename K, typename V, typename Alloc = Allocator<>>
class AVLTree {
public:
    struct Node {
        K key;
        V val;
        int8_t balance;
        uint8_t height;
        Node *left;
        Node *right;
        Node *parent;

        Node() : balance(0), height(0), left(nullptr), right(nullptr), parent(this) {}

        Node(K key, V val) : Node() {
            this->key = key;
            this->val = val;
        }
    };

private:
    Node *rootNode;
    typename AllocatorTraits<Alloc>::template Rebind<Node>::Other::BaseType allocator;

    void rotateRight(Node *n) {
        Node *left = n->left;
        Node *parent = n->parent;
        Node *leftRight = n->left->right;
        if (n != rootNode) {
            if (parent->left == n) {
                parent->left = left;
            } else {
                parent->right = left;
            }
            left->parent = parent;
        } else {
            left->parent = left;
            rootNode = left;
        }
        left->right = n;
        n->parent = left;

        n->left = leftRight;
        if (leftRight) {
            leftRight->parent = n;
        }
    }

    void rotateLeft(Node *n) {
        Node *right = n->right;
        Node *parent = n->parent;
        Node *rightLeft = n->right->left;
        if (n != rootNode) {
            if (parent->left == n) {
                parent->left = right;
            } else {
                parent->right = right;
            }
            right->parent = parent;
        } else {
            right->parent = right;
            rootNode = right;
        }
        right->left = n;
        n->parent = right;

        n->right = rightLeft;
        if (rightLeft) {
            rightLeft->parent = n;
        }
    }

    void postRotationUpdate(Node *n, Node *upd) {
        int8_t lHeight = getHeight(n->left);
        int8_t rHeight = getHeight(n->right);
        n->balance = lHeight - rHeight;
        n->height = 1 + max(lHeight, rHeight);
        update(upd);
    }

    void update(Node *n) {
        int8_t lHeight = getHeight(n->left);
        int8_t rHeight = getHeight(n->right);
        n->balance = lHeight - rHeight;
        n->height = 1 + max(lHeight, rHeight);
        if (n->balance == 2) {
            if (n->left->balance > 0) {
                rotateRight(n);
                postRotationUpdate(n, n->parent->left);
            } else {
                rotateLeft(n->left);
                rotateRight(n);
                postRotationUpdate(n, n->parent->left);
            }
        } else if (n->balance == -2) {
            if (n->right->balance > 0) {
                rotateRight(n->right);
                rotateLeft(n);
                postRotationUpdate(n, n->parent->right);
            } else {
                rotateLeft(n);
                postRotationUpdate(n, n->parent->right);
            }
        } else if (n != rootNode) {
            update(n->parent);
        }
    }

    inline uint8_t getHeight(Node *node) {
        return node ? node->height : 0;
    }

    template<typename Val>
    inline void insertT(K key, Val value) {
        Node *node = new(allocator.allocate(1))Node();
        node->key = key;
        node->val = value;
        insertNode(node);
    }

    Node *searchNode(QueryType type, K key, Node *n) {
        if (type & LESS) {
            Node *last = n->key < key ? n : nullptr;
            while (true) {
                while (key <= n->key) {
                    if ((type & EQUAL) && n->key == key)return n;
                    if (n->left) {
                        n = n->left;
                    } else {
                        return last;
                    }
                }
                while (key > n->key) {
                    if (n->right) {
                        last = n;
                        n = n->right;
                    } else {
                        return n;
                    }
                }
                if ((type & EQUAL) && n->key == key)return n;
            }
        }
        if (type & GREATER) {
            Node *last = n->key > key ? n : nullptr;
            while (true) {
                while (key >= n->key) {
                    if ((type & EQUAL) && n->key == key)return n;
                    if (n->right) {
                        n = n->right;
                    } else {
                        return last;
                    }
                }
                while (key < n->key) {
                    if (n->left) {
                        last = n;
                        n = n->left;
                    } else {
                        return n;
                    }
                }
                if ((type & EQUAL) && n->key == key)return n;
            }
        }
        if (type & EQUAL) {
            while (true) {
                if (n->key == key) {
                    return n;
                }
                if (n->key > key) {
                    if (n->left) {
                        n = n->left;
                    } else {
                        return nullptr;
                    }
                } else {
                    if (n->right) {
                        n = n->right;
                    } else {
                        return nullptr;
                    }
                }
            }
        }
        return nullptr;
    }

public:

    AVLTree() : rootNode(nullptr), allocator() {}

    void insertNode(Node *node) {
        if (!rootNode) {
            rootNode = node;
        } else {
            Node *n = rootNode;
            while (true) {
                if (n->key > n->key) {
                    if (n->right != nullptr) {
                        n = n->right;
                    } else {
                        break;
                    }
                } else {
                    if (n->left != nullptr) {
                        n = n->left;
                    } else {
                        break;
                    }
                }
            }
            Node *insertedNode = node;
            insertedNode->parent = n;
            if (insertedNode->key > n->key) {
                n->right = insertedNode;
            } else {
                n->left = insertedNode;
            }
            update(insertedNode);
        }
    }

    inline void insert(K key, const V &value) {
        insertT(key, value);
    }

    inline void insert(K key, V &&value) {
        insertT(key, value);
    }

    /**
     *
     * @param type Comparator type such that ((key_of_returned_value) is (type) to (key)) is true
     * @param key
     * @return nearest value to key with (type) held or null if not found
     */
    V *search(QueryType type, K key) {
        if (!rootNode)return nullptr;
        Node *v = searchNode(type, key, rootNode);
        return v ? &v->val : nullptr;
    }

    void remove(K key) {
        if (rootNode) {
            Node *removedNode = searchNode(EQUAL, key, rootNode);
            if (removedNode) {
                if (removedNode->height > 1) {
                    Node *replacer = removedNode;
                    while (true) {
                        if (replacer->left) {
                            replacer = replacer->left;
                        }
                        while (replacer->right) {
                            replacer = replacer->right;
                        }
                        if (!replacer->left)break;
                    }

                    Node *p = replacer->parent;
                    if (p->right == replacer) {
                        p->right = nullptr;
                    } else {
                        p->left = nullptr;
                    }

                    replacer->left = removedNode->left;
                    replacer->right = removedNode->right;

                    if (replacer->left) {
                        replacer->left->parent = replacer;
                    }
                    if (replacer->right) {
                        replacer->right->parent = replacer;
                    }

                    if (removedNode != rootNode) {
                        replacer->parent = removedNode->parent;
                        if (removedNode->parent->left == removedNode) {
                            removedNode->parent->left = replacer;
                        } else {
                            removedNode->parent->right = replacer;
                        }
                    } else {
                        replacer->parent = replacer;
                        rootNode = replacer;
                    }
                    if (p->key != key) {
                        postRotationUpdate(replacer, p);
                    } else {
                        update(replacer);
                    }
                } else {
                    if (removedNode->parent->left == removedNode) {
                        removedNode->parent->left = nullptr;
                    } else {
                        removedNode->parent->right = nullptr;
                    }
                    if (removedNode == rootNode) {
                        rootNode = nullptr;
                    } else {
                        update(removedNode->parent);
                    }
                }
                allocator.deallocate(removedNode);
            }
        }
    }
};

#endif //AVLTREE_H
