#include <algorithm>
#include <iostream>
#include <sstream>
#include <string>
#include <vector>

struct Node {
  int val;
  int balance;
  Node *parent = nullptr;
  Node *left = nullptr;
  Node *right = nullptr;

  bool operator==(const Node *other) const {
    return val == other->val && parent == other->parent &&
           left == other->left && right == other->right;
  }

  Node(int value) { val = value; }

  // ~Node() {
  //   delete parent;
  //   delete left;
  //   delete right;
  // }

  friend std::ostream &operator<<(std::ostream &os, const Node *node) {
    std::stringstream entity;
    entity << "\n\nValue: " << node->val;
    if (node->balance) {
      entity << "\nBalance: " << node->balance;
    }
    entity << "\nParent: ";
    if (node->parent) {
      entity << node->parent->val;
    } else {
      entity << "nullptr ";
    }

    entity << "\nleft: ";
    if (node->left) {
      entity << node->left;
    } else {
      entity << "nullptr ";
    }

    entity << "\nRight: ";
    if (node->right) {
      entity << node->right;
    } else {
      entity << "nullptr ";
    }
    return os << entity.str();
  }
};

class Tree {
private:
  Node *root;

public:
  void prettyPrint(Node *node, std::string prefix = "", bool isLeft = true) {
    if (node) {
      if (node->right) {
        std::string morePrefix = isLeft ? "│   " : "    ";
        prettyPrint(node->right, prefix + morePrefix, false);
      }
      std::string bracket = isLeft ? "└── " : "┌── ";
      std::cout << prefix << bracket << node->val << "\n";
      if (node->left) {
        std::string morePrefix = isLeft ? "    " : "│   ";
        prettyPrint(node->left, prefix + morePrefix, true);
      }
    }
  }

  Node *buildTree(std::vector<int> &numbers) {
    for (int value : numbers) {
      Node *newNode = new Node(value);
      insertNode(newNode);
    }
    return root;
  }

  Node *insertNode(Node *node) {
    if (!root) {
      root = node;
    } else {
      Node *curr = root;
      while (curr) {
        if (node->val < curr->val) {
          if (curr->left) {
            curr = curr->left;
          } else {
            curr->left = node;
            node->parent = curr;
            break;
          }
        } else if (node->val > curr->val) {
          if (curr->right) {
            curr = curr->right;
          } else {
            curr->right = node;
            node->parent = curr;
            break;
          }
        } else {
          std::cout << "duplicate found\n";
          delete node;
          node = nullptr;
          break;
        }
      }
      if (node)
        balanceTree(node->parent); // new node never has children
    }
    return root;
  }

  Node *deleteNode(int value) {
    Node *deletedNode = find(value);
    if (!deletedNode) {
      return nullptr;
    }

    Node *successor;
    if (deletedNode->right) {
      if (deletedNode->right->left) {
        successor = deletedNode->right->left;
        while (successor->left) {
          successor = successor->left;
        }

        // Detaching Successor
        successor->parent->left = successor->right;
        if (successor->right) {
          successor->right->parent = successor->parent;
        }
        successor->right = deletedNode->right;
        deletedNode->right->parent = successor;
      } else {
        successor = deletedNode->right;
      }
      successor->left = deletedNode->left;
      if (deletedNode->left) {
        deletedNode->left->parent = successor;
      }

      if (deletedNode->parent) {
        if (deletedNode->parent->right == deletedNode) {
          deletedNode->parent->right = successor;
        } else {
          deletedNode->parent->left = successor;
        }
      } else {
        root = successor;
      }
      successor->parent = deletedNode->parent;

    } else {
      successor = deletedNode->left;
      if (deletedNode->parent) {
        if (deletedNode->parent->left == deletedNode) {
          deletedNode->parent->left = successor;
        } else {
          deletedNode->parent->right = successor;
        }
        if (deletedNode->left) {
          successor->parent = deletedNode->parent;
        }
      } else {
        root = successor;
      }
    }
    delete deletedNode;

    // WARN: Maybe not sufficient to start balance at successor
    balanceTree(successor);
    // rebalance(successor);
    return root;
  }

  Node *find(int value) {
    Node *curr = root;
    while (curr) {
      if (value < curr->val) {
        curr = curr->left;
      } else if (value > curr->val) {
        curr = curr->right;
      } else {
        return curr;
      }
    }
    return nullptr;
  }

  int depth(Node *node) {
    int depth = 0;
    Node *curr = root;
    while (curr) {
      if (node->val < curr->val) {
        curr = curr->left;
      } else if (node->val > curr->val) {
        curr = curr->right;
      } else {
        return depth;
      }
      depth++;
    }
    return -1;
  }

  int height(Node *node) {
    if (!node) {
      return -1;
    }
    int left = height(node->left);
    int right = height(node->right);
    node->balance = left - right;
    return 1 + std::max(left, right);
  }

  Node *balanceTree(Node *node) {
    height(node);

    if (node->balance == -2) {
      if (node->right->balance == 1) {
        // rotateRightLeft
        rotateRight(node->right);
      }
      node = rotateLeft(node);
    } else if (node->balance == 2) {
      if (node->left->balance == -1) {
        // rotateLeftRight
        rotateLeft(node->left);
      }
      node = rotateRight(node);
    }

    if (!node->parent) {
      root = node;
      return node;
    }
    return balanceTree(node->parent);
  }

  Node *rotateRight(Node *node) {
    Node *jumpingNode = node->left->right;
    node->left->parent = node->parent;
    node->left->right = node;
    if (node->parent && node->parent->left == node) {
      node->parent->left = node->left;
    } else if (node->parent) {
      node->parent->right = node->left;
    }
    node->parent = node->left;
    node->left = jumpingNode;
    if (jumpingNode) {
      jumpingNode->parent = node;
    }
    return node->parent;
  }

  Node *rotateLeft(Node *node) {
    Node *jumpingNode = node->right->left;
    node->right->parent = node->parent;
    node->right->left = node;
    if (node->parent && node->parent->left == node) {
      node->parent->left = node->right;
    } else if (node->parent) {
      node->parent->right = node->right;
    }
    node->parent = node->right;
    node->right = jumpingNode;
    if (jumpingNode) {
      jumpingNode->parent = node;
    }
    return node->parent;
  }
};

int main() {
  Tree *tree = new Tree();

  std::vector<int> values = {43, 57, 87, 27, 30, 4, 70, 90, 89, 91};
  Node *root = tree->buildTree(values);

  tree->prettyPrint(root);
  // Node *first = tree->find(87);
  // std::cout << "depth " << tree->depth(first) << "\t" << first->val << "\n";
  // std::cout << "height " << tree->height(first) << "\t" << first->val <<
  // "\n"; Node *noder = tree->find(57); std::cout << "depth " <<
  // tree->depth(noder) << "\t" << noder->val << "\n"; std::cout << "height " <<
  // tree->height(noder) << "\t" << noder->val << "\n"; std::cout << "height "
  // << tree->height(root) << "\t" << root->val << "\n";

  // root = tree->deleteNode(91);
  root = tree->deleteNode(87);
  tree->prettyPrint(root);
  Node *newNode = new Node(1);
  root = tree->insertNode(newNode);
  tree->prettyPrint(root);
  newNode = new Node(503);
  root = tree->insertNode(newNode);
  tree->prettyPrint(root);
  newNode = new Node(102);
  root = tree->insertNode(newNode);
  tree->prettyPrint(root);
  newNode = new Node(504);
  root = tree->insertNode(newNode);
  tree->prettyPrint(root);
  newNode = new Node(2);
  root = tree->insertNode(newNode);
  tree->prettyPrint(root);
  newNode = new Node(0);
  root = tree->insertNode(newNode);
  tree->prettyPrint(root);
  newNode = new Node(-1);
  root = tree->insertNode(newNode);
  tree->prettyPrint(root);
  newNode = new Node(-2);
  root = tree->insertNode(newNode);
  tree->prettyPrint(root);
  newNode = new Node(-3);
  root = tree->insertNode(newNode);
  tree->prettyPrint(root);

  root = tree->deleteNode(1);
  tree->prettyPrint(root);
  root = tree->deleteNode(-1);
  tree->prettyPrint(root);
  root = tree->deleteNode(2);
  tree->prettyPrint(root);
}
