#include <memory>
#include <string>

class AVL;

class AVLNode {
 public:
 	AVLNode(int key, std::weak_ptr<AVLNode> parent);

 private:
  int key_;
  int height_;
  std::weak_ptr<AVLNode> parent_;
  std::shared_ptr<AVLNode> left_;
  std::shared_ptr<AVLNode> right_;

  friend class AVL;
}; // class AVLNode

class AVL {
 public:
 	AVL();

 	std::shared_ptr<AVLNode> Insert(std::shared_ptr<AVLNode> node, int key);
 	std::string JSON() const;
    void setRoot(std::shared_ptr<AVLNode> newRoot);
    std::shared_ptr<AVLNode> getRoot();
    std::shared_ptr<AVLNode> findParent(int key);
    std::shared_ptr<AVLNode> rightRotate(std::shared_ptr<AVLNode> node);
    std::shared_ptr<AVLNode> leftRotate(std::shared_ptr<AVLNode> node);
    int getBalance(std::shared_ptr<AVLNode> node);
    int height(std::shared_ptr<AVLNode> N);
    bool empty() const;
    int max(int a, int b);

 private:
 	std::shared_ptr<AVLNode> root_;
 	size_t size_;
}; // class AVL