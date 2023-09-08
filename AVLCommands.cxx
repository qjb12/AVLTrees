#include "AVL.h"
#include <cassert>
#include <iostream>
#include <fstream>
#include <string>
#include <queue>
#include "json.hpp"

using namespace std;

int main(int argc, char** argv){
    if(argc > 2){
        cout << "Too many files" << endl;
        return 0;
    }
    ifstream file(argv[1]);
    nlohmann::json jsonObject;
    if(file.is_open()){
        file >> jsonObject;
    } else {
        cout << "file cannot be opened" << endl;
        return 0;
    }

    AVL tree;

    for (auto itr = jsonObject.begin(); itr != jsonObject.end(); ++itr){
        if(itr.key() == "metadata"){
            break;
        }

        for(auto it = jsonObject[itr.key()].begin(); it != jsonObject[itr.key()].end(); ++it){
            if(it.key() == "key"){
                std::shared_ptr<AVLNode> currRoot = tree.getRoot();
                std::shared_ptr<AVLNode> newRoot = tree.AVL::Insert(currRoot, it.value());
                tree.setRoot(newRoot);
            }
        }
    }
    string treePrint = tree.JSON();
    cout <<treePrint;
    return 0;
}