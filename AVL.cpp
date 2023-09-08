#include "AVL.h"

#include <cassert>
#include <iostream>
#include <string>
#include <queue>

#include "json.hpp"

AVLNode::AVLNode(int key, std::weak_ptr<AVLNode> parent) :
	key_(key),
    height_(1),
	parent_(parent),
	left_(nullptr),
	right_(nullptr) {}

AVL::AVL() : root_(nullptr), size_(0) {}

bool AVL::empty() const {
	return size_ == 0;
}

std::shared_ptr<AVLNode> AVL::getRoot(){
    return this->root_;
}

void AVL::setRoot(std::shared_ptr<AVLNode> newRoot){
    this->root_ = newRoot;
}

int AVL::height(std::shared_ptr<AVLNode> N)
{
	if (N == nullptr)
		return 0;
	return N->height_;
}

int AVL::max(int a, int b)
{
	return (a > b)? a : b;
}

int AVL::getBalance(std::shared_ptr<AVLNode> node)
{
	if (node == nullptr)
		return 0;
	return height(node->right_) - height(node->left_);
}

std::shared_ptr<AVLNode> AVL::rightRotate(std::shared_ptr<AVLNode> y)
{
    std::shared_ptr<AVLNode> x = y->left_;
	std::shared_ptr<AVLNode> T2 = x->right_;

    if(T2 != nullptr && y != this->root_){
        T2->parent_ = x->parent_;
        x->parent_ = y->parent_;
        y->parent_ = x;
        x->right_ = y;
        y->left_ = T2;
    } else if(y == this->root_ && T2 != nullptr){
        T2->parent_ = x->parent_;
        x->parent_.reset();
        y->parent_ = x;
        x->right_ = y;
        y->left_ = T2;
    } else if(y != this->root_ && T2 == nullptr){
        x->parent_ = y->parent_;
        y->parent_ = x;
        x->right_ = y;
        y->left_ = T2;
    } else if(y == this->root_ && T2 == nullptr){
        x->parent_.reset();
        y->parent_ = x;
        x->right_ = y;
        y->left_ = T2;
    }

	// Update heights
	y->height_ = max(height(y->left_),
					height(y->right_)) + 1;
	x->height_ = max(height(x->left_),
					height(x->right_)) + 1;

	// Return new root
	return x;
}

std::shared_ptr<AVLNode> AVL::leftRotate(std::shared_ptr<AVLNode> x)
{
	std::shared_ptr<AVLNode> y = x->right_;
	std::shared_ptr<AVLNode> T2 = y->left_;

	// Perform rotation
    if(T2 != nullptr && x != this->root_){
        T2->parent_ = y->parent_;
        y->parent_ = x->parent_;
        x->parent_ = y;
        y->left_ = x;
        x->right_ = T2;
    } else if(x == this->root_ && T2 != nullptr){
        T2->parent_ = y->parent_;
        y->parent_.reset();
        x->parent_ = y;
        y->left_ = x;
        x->right_ = T2;
    } else if(x != this->root_ && T2 == nullptr){
        y->parent_ = x->parent_;
        x->parent_ = y;
        y->left_ = x;
        x->right_ = T2;
    } else if(x == this->root_ && T2 == nullptr){
        x->parent_ = y;
        y->parent_.reset();
        y->left_ = x;
        x->right_ = T2;
    }
	// Update heights
    x->height_ = max(height(x->left_),
					height(x->right_)) + 1;
	y->height_ = max(height(y->left_),
					height(y->right_)) + 1;
	// Return new root
	return y;
}

std::shared_ptr<AVLNode> AVL::findParent(int key) {
	std::shared_ptr<AVLNode> currentNode = root_;
	while (currentNode != nullptr) {
		if (currentNode->key_ < key && currentNode->right_ == nullptr){
            return currentNode;
		} else if(currentNode->key_ > key && currentNode->left_ == nullptr){
            return currentNode;
        }
		currentNode = (key < currentNode->key_) ?
			currentNode->left_ : currentNode->right_;
	}
    return currentNode;
}

std::shared_ptr<AVLNode> AVL::Insert(std::shared_ptr<AVLNode> node, int key){
    if (node == nullptr) {
        if(this->root_ == nullptr){
            this->root_ = std::make_shared<AVLNode>(key, std::weak_ptr<AVLNode>());
            node = this->root_;
            size_++;
		    return node;
        }
        std::shared_ptr<AVLNode> parent = this->findParent(key);
		node = std::make_shared<AVLNode>(key, parent);
		size_++;
		return node;
	}
    if (key < node->key_)
		node->left_ = Insert(node->left_, key);
    else if (key > node->key_)
		node->right_ = Insert(node->right_, key);
	else // Equal keys are not allowed in BST
		return node;
    
    node->height_ = 1 + max(height(node->left_),
						height(node->right_));

    int balance = getBalance(node);
    
    if (balance < -1 && key < node->left_->key_)
		return rightRotate(node);
    
    if (balance > 1 && key > node->right_->key_)
		return leftRotate(node);

    if (balance < -1 && key > node->left_->key_)
	{
		node->left_ = leftRotate(node->left_);
		return rightRotate(node);
	}

    if (balance > 1 && key < node->right_->key_)
	{
		node->right_ = rightRotate(node->right_);
		return leftRotate(node);
	}

    return node;
}

std::string AVL::JSON() const {
	nlohmann::json result;
	std::queue< std::shared_ptr<AVLNode> > nodes;
	if (root_ != nullptr) {
		result["root"] = root_->key_;
        result["height"] = root_->height_ - 1;
		nodes.push(root_);
		while (!nodes.empty()) {
			auto v = nodes.front();
			nodes.pop();
			std::string key = std::to_string(v->key_);
            if(v->left_ == nullptr && v->right_ == nullptr){
                result[key]["balance factor"] = 0;
                result[key]["height"] = 0;
            } else if(v->left_ == nullptr && v->right_ != nullptr){
                result[key]["balance factor"] = v->right_->height_;
                result[key]["height"] = v->height_ - 1;
            } else if(v->left_ != nullptr && v->right_ == nullptr){
                result[key]["balance factor"] = 0 - v->left_->height_;
                result[key]["height"] = v->height_ - 1;
            } else {
                result[key]["balance factor"] = v->right_->height_ - v->left_->height_;
                result[key]["height"] = v->height_ - 1;
            }
			if (v->left_ != nullptr) {
				result[key]["left"] = v->left_->key_;
				nodes.push(v->left_);
			}
			if (v->right_ != nullptr) {
				result[key]["right"] = v->right_->key_;
				nodes.push(v->right_);
			}
			if (v->parent_.lock() != nullptr) {
				result[key]["parent"] = v->parent_.lock()->key_;
			} else {
				result[key]["root"] = true;
			}
		}
	}
	result["size"] = size_;
	return result.dump(2) + "\n";
}