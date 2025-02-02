#include <iostream>
#include <string>
#include <vector>

struct Node {
  int val;
  Node *parent;
  Node *left;
  Node *right;

  Node(int value, Node *daddy) {
    parent = daddy;
    val = value;
  }
};

class Tree {
private:
  Node *root;

public:
  // const prettyPrint = (node, prefix = "", isLeft = true) => {
  //   if (node === null) {
  //     return;
  //   }
  //   if (node.right !== null) {
  //     prettyPrint(node.right, `${prefix}${isLeft ? "│   " : "    "}`, false);
  //   }
  //   console.log(`${prefix}${isLeft ? "└── " : "┌── "}${node.data}`);
  //   if (node.left !== null) {
  //     prettyPrint(node.left, `${prefix}${isLeft ? "    " : "│   "}`, true);
  //   }
  // };

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
};

int main() {
  Tree *tree = new Tree();

  std::vector<int> values = {43, 57, 87, 97, 27, 4};
  Node *root = tree->buildTree(values);

  tree->prettyPrint(root);
}
