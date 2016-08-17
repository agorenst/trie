#ifndef _PREFIXTREE_H_
#define _PREFIXTREE_H_

#include <string>
#include <memory>
#include <map>
#include <iostream>

// An extremely straightforward implementation. The only confusion is that if
// we want to reason about the "nodes", we need to find the symbols on the
// edges.
// This is old code, and I'm revisiting it for a later project. I'm realizing
// how embarassing these 80-some-odd lines can be...


// The entry point.
template<class S>
class prefix_tree {
    private:
    class node {
        private:
        typedef std::shared_ptr<node> pnode;
        public:
        // inefficient, but we want to be straightforward
        std::map<S, pnode> edges;
    
        // works very naturally with iterators.
        template<class Iter>
        void insert(Iter b, Iter e) {
            if (b == e) { return; } // we are the end of this input
    
            auto symb = *b++; // we advance b, having read from it.
            // we need to make a new child!
            if (edges.find(symb) == edges.end()) {
                edges[symb] = std::make_shared<node>();
            }
    
            // recursive call.
            edges[symb]->insert(b, e);
        }
    
        // Stolen from parse-tree code in my cfg repository.
        void simple_print(std::ostream& o, S symb, int d = 0) {
            for (int i = 0; i < 2*d; ++i) {
                o << " ";
            }
            o << symb << std::endl;
            for (auto&& symb_and_child : edges) {
                symb_and_child.second->simple_print(o, symb_and_child.first, d+1);
            }
        }
    
        // We don't bother incrementing a depth counter, tikz will take care of
        // that for us.
        void tikz_print(std::ostream& o) {
            o << "node[circle, draw]{}" << std::endl;
            for (auto it = edges.begin(); it != edges.end(); ++it) {
                o << "child {" << std::endl;
                it->second->tikz_print(o);
                o << "edge from parent node[left]{" << it->first << "}" << std::endl;
                o << "}" << std::endl;
            }
        }
    };
    std::shared_ptr<node> r;
    public:
    prefix_tree(): r(std::make_shared<node>()) {}
    template<class Iter>
    void insert(Iter b, Iter e) {
        r->insert(b, e);
    }
    void tikz_print(std::ostream& o){
        o << "\\";
        r->tikz_print(o);
        o << ";" << std::endl;
    }
    void simple_print(std::ostream& o) {
        r->simple_print(o, S());
    }
};

#endif
