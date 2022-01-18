//
//  skip_list.h
//  DataStructures
//
//  Created by Dousir9 on 2022/1/18.
//

#ifndef skip_list_h
#define skip_list_h

/*
verify: https://www.luogu.com.cn/problem/P2580
performance: skip list:727ms; std::map: 989ms; faster than std::map
 */

template<typename K, typename V>
class SkipList {
private:
    struct Node {
        K key_;
        V val_;
        Node** next_;
        Node() {}
        Node(K key, V val, int level) : key_(key), val_(val) {
            next_ = new Node*[level];
            for (int i = 0; i < level; ++i) {
                next_[i] = nullptr;
            }
        }
        ~Node() {
            delete [] next_;
        }
    };
    
    Node* head_; // 头结点
    int max_level_; // 最大层数
    int cur_max_level_; // 当前最大层数
    int rand_ = RAND_MAX / 2; // rand_ / rand() = 0.5
    Node** pre_nodes_; // add 和 erase 时用到的临时变量，表示每层的前驱结点
public:
    SkipList(int max_level = 32) : max_level_(max_level) {
        head_ = new Node(K(), V(), max_level);
        pre_nodes_ = new Node*[max_level];
        for (int i = 0; i < max_level_; ++i) {
            pre_nodes_[i] = head_;
        }
    }
    
    ~SkipList() {
        Node *p = head_;
        Node *temp;
        while (nullptr != p) {
            temp = p;
            p = p->next_[0];
            delete temp;
        }
        delete [] pre_nodes_;
    }
    
    int get_random_level() {
        int level = 1;
        while (rand() < rand_ && level < max_level_) {
            ++level;
        }
        return level;
    }
    
    void set(const K &key, const V &val) {
        Node* p = head_;
        for (int i = cur_max_level_ - 1; i >= 0; --i) {
            while (nullptr != p->next_[i] && p->next_[i]->key_ < key) {
                p = p->next_[i];
            }
            pre_nodes_[i] = p;
        }
        if (nullptr != p->next_[0] && key == p->next_[0]->key_) {
            p->next_[0]->val_ = val;
            return;
        }
        int level = get_random_level(); // 随机生成新结点的层数
        Node* new_node = new Node(key, val, level);
        for (int i = 0; i < level; ++i) {
            new_node->next_[i] = pre_nodes_[i]->next_[i];
            pre_nodes_[i]->next_[i] = new_node;
        }
        cur_max_level_ = level > cur_max_level_ ? level : cur_max_level_;
    }
    
    bool erase(const K &key) {
        Node *p = head_;
        for (int i = cur_max_level_ - 1; i >= 0; --i) {
            while (nullptr != p->next_[i] && p->next_[i]->key_ < key) {
                p = p->next_[i];
            }
            pre_nodes_[i] = p;
        }
        if (nullptr == p->next_[0] || p->next_[0]->key_ != key) {
            return false;
        }
        Node *del = p->next_[0];
        // 每一层都将这个结点删除
        for (int i = 0; i < cur_max_level_; ++i) {
            if (pre_nodes_[i]->next_[i] == del) {
                pre_nodes_[i]->next_[i] = del->next_[i];
            }
        }
        delete del;
        while (cur_max_level_ > 1 && nullptr == head_->next_[cur_max_level_ - 1]) {
            --cur_max_level_;
        }
        return true;
    }
    
    V get(const K &key) {
        Node *p = head_;
        for (int i = cur_max_level_ - 1; i >= 0; --i) {
            while (nullptr != p->next_[i] && p->next_[i]->key_ < key) {
                p = p->next_[i];
            }
        }
        if (nullptr == p->next_[0] || p->next_[0]->key_ != key) {
            return V(); // default, example: map<int, int> mp; 0 == mp[1] is true;
        }
        return p->next_[0]->val_;
    }
};

#endif /* skip_list_h */
