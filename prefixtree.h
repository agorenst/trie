#ifndef _PREFIXTREE_H_
#define _PREFIXTREE_H_

#include <string>
#include <memory>
#include <map>
#include <iostream>

using namespace std;

template<class S>
class node {
    public:
        // inefficient, but we want to be straightforward
        map<S, shared_ptr<node>> edges;

        // works very naturally with iterators.
        template<class Iter>
        void insert(Iter b, Iter e) {
            if (b == e) { return; } // we are the end of this input
            auto symb = *b;
            
            // we need to make a new child!
            if (edges.find(symb) == edges.end()) {
                edges[symb] = shared_ptr<node<S>>(new node<S>);
            }
            edges[symb]->insert(b+1, e);
        }

        // most of this is making sure everything plays nice with tikz.
        void tikz_print(ostream& o) {
            o << "node[circle, draw]{}" << endl;
            for (auto it = edges.begin(); it != edges.end(); ++it) {
                o << "child {" << endl;
                it->second->tikz_print(o);
                o << "edge from parent node[left]{" << it->first << "}" << endl;
                o << "}" << endl;
            }
        }
};

template<class S>
class prefix_tree {
    private:
        shared_ptr<node<S>> r;
    public:
        prefix_tree(): r(new node<S>) {}
        template<class Iter>
        void insert(Iter b, Iter e) {
            r->insert(b, e);
        }
        void tikz_print(ostream& o){
            o << "\\";
            r->tikz_print(o);
            o << ";" << endl;
        }

};

#endif
