#include <string>
#include <memory>
#include <list>
#include <iostream>
#include <cassert>

template<class T>
class node{
    private:
        // an edge is simply a sequence of Ts and the node terminating that sequence
        class edge : public std::pair<std::list<T>, std::shared_ptr<node<T>>> {
            public:
            edge(const std::list<T>& f, const std::shared_ptr<node<T>> s):
                std::pair<std::list<T>, std::shared_ptr<node<T>>> (f,s) {}
        };

        typename std::list<edge> edge_list;

        // really just a "find_if"
        typename std::list<edge>::iterator edge_to_follow(T t) {
            for (auto it = edge_list.begin(); it != edge_list.end(); ++it) {
                if (*(it->first.begin()) == t) {
                    return it;
                }
            }
            return edge_list.end();
        }
    public:
        node(): edge_list() {}
        node(const std::list<edge>& edge_list): edge_list(edge_list) {}

        template<class Iter>
            void insert(Iter first, Iter last) {
                auto edge_iter = edge_to_follow(*first);

                // if no edge has any prefix with [first, last) then we make a new edge.
                if (edge_iter == edge_list.end()) {
                    edge to_add(std::list<T>(first, last), std::shared_ptr<node>(new node));
                    edge_list.push_back(to_add);
                    return;
                }

                auto edge_first = edge_iter->first.begin();
                auto edge_last = edge_iter->first.end();

                // this loop should always run at least once
                // we "consume" the common prefix between [edge_first, edge_list) and [first, last)
                std::list<T> prefix;
                while (first != last &&
                        edge_first != edge_last &&
                        (*edge_first) == (*first)) {
                    prefix.push_back(*first);
                    ++edge_first;
                    ++first;
                }
                assert(prefix.size() > 0);

                // this string is already included
                if (first == last && edge_first == edge_last) {
                    return;
                }
                // we input a superstring, ie, input = "abcde", edge = "abc"
                // recurse to the child
                if (first != last && edge_first == edge_last) {
                    return edge_iter->second->insert(first, last);
                }
                // something about the input sequence means we have to make a new node.
                if (edge_first != edge_last) {
                    // we clip off the prefix, this creates a copy edge with only the suffix sequence
                    edge clipped_orig(std::list<T>(edge_first, edge_last), edge_iter->second);

                    // this creates the new intermediary node
                    edge prefix_edge(prefix, std::shared_ptr<node>(new node(std::list<edge>{clipped_orig})));

                    edge_list.erase(edge_iter); // erase the old copy of our edge

                    edge_list.push_back(prefix_edge); // ok, we've rebuilt the tree with the new node.

                    if (first != last) {
                        prefix_edge.second->insert(first, last);
                    }
                    return;

                }
            }

            void tikz_print(std::ostream& o) {
                o << "node[circle,draw]{}\n";
                for(auto& e : edge_list) {
                    o << "child {\n";
                    e.second->tikz_print(o);
                    o << "    edge from parent node[left]{\n";
                    for (auto& s : e.first) {
                        o << s << " ";
                    }
                    o << "    }\n";
                    o << "}\n";
                }
            }
};

template<class T>
class radixtree {
    private:
    std::shared_ptr<node<T>> root;
    public:

    radixtree(): root(new node<T>) {}

    template<class Iter>
    void insert(Iter first, Iter last) {
        return root->insert(first, last);
    }

    void tikz_print(std::ostream& o) {
        o << "\\";
        root->tikz_print(o);
        o << ";\n";
    }
};
