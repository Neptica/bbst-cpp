#include <iostream>
#include <sstream>
#include <string>
#include <vector>

struct Node {
  int val;
  Node *parent;
  Node *left;
  Node *right;

  bool operator==(const Node *other) const {
    return val == other->val && parent == other->parent &&
           left == other->left && right == other->right;
  }

  Node(int value, Node *daddy) {
    parent = daddy;
    val = value;
  }

  friend std::ostream &operator<<(std::ostream &os, const Node *node) {
    std::stringstream entity;
    entity << "\n\nValue: " << node->val;
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
      Node *newNode = new Node(value, nullptr);
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
          break;
        }
      }
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

        if (deletedNode->parent) {
          successor->parent->left = successor->right;
        }
        successor->right = deletedNode->right;
      } else {
        successor = deletedNode->right;
      }
      successor->parent = deletedNode->parent;
      successor->left = deletedNode->left;
      if (deletedNode->left) {
        deletedNode->left->parent = successor;
      }

      if (deletedNode->parent) {
        successor->parent->right = successor;
      } else {
        root = successor;
      }

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
};

int main() {
  Tree *tree = new Tree();

  std::vector<int> values = {43, 57, 87, 97, 27, 4, 70, 90, 91};
  Node *root = tree->buildTree(values);

  tree->prettyPrint(root);
  root = tree->deleteNode(43);
  root = tree->deleteNode(87);
  // std::cout << root << "\n";
  tree->prettyPrint(root);
}
