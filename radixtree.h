#include <string>
#include <memory>
#include <list>
#include <iostream>

using std::endl;

class node;

class edge{
    public:
    std::string s;
    std::shared_ptr<node> n;
    edge(std::string s, std::shared_ptr<node> n): s(s), n(n) {}
    edge(std::string s, node* n): s(s), n(n) {}

    void print(std::ostream& o, std::string prefix);
    void tikz_print(std::ostream& o, std::string s = "");
};

std::string shared_prefix(const std::string l, const std::string r) {
    std::string p{""};
    for (unsigned i = 0; i < std::min(l.size(), r.size()); ++i) {
        if (l[i] == r[i]) {
            p += l[i];
        }
        else {
            break;
        }
    }
    return p;
}

bool proper_prefix_of(const std::string p, const std::string s) {
    if (p.size() >= s.size()) { return false; }
    
    for (unsigned i = 0; i < p.size(); ++i) {
        if (p[i] != s[i]) {
            return false;
        }
    }
    return true;
}

std::string strip_prefix(const std::string p, const std::string s) {
    if (!proper_prefix_of(p, s)) { return "";}
    return s.substr(p.size(), s.size());
}

class node{
    public:
    std::list<std::shared_ptr<edge>> es;

    node(std::initializer_list<std::shared_ptr<edge>> il): es(il) {}
    node(std::initializer_list<edge*> il) {
        for (auto it = il.begin(); it != il.end(); ++it) {
            es.push_back(std::shared_ptr<edge>(*it));
        }
    }
    node() {}

    void insert(std::string& s) {
        //cerr << "Inserting: " << s << endl;

        if (s=="") { return; } // the path we just traversed represenst the tsring

        //cerr << "About to enter for loop" << endl;
        // for every edge,
        for (auto it = es.begin(); it != es.end(); ++it) {
            //cerr << "Starting for loop" << endl;
            auto eptr = *it;
            auto p = shared_prefix(eptr->s, s);
            if (p == "") continue;

            //cerr << "Prefix: " << p << "\tEdgestring: " << eptr->s << endl;

            if (proper_prefix_of(p, s) && proper_prefix_of(p, eptr->s)) {
                auto edgestringprime = strip_prefix(p, eptr->s);
                auto inputstringprime = strip_prefix(p, s);
                es.push_back(std::shared_ptr<edge>(new edge(p, new node{new edge(edgestringprime, eptr->n),
                                                               new edge(inputstringprime, new node())})));
                es.remove(eptr);
                return;
            }
            if (eptr->s == s) { return; }
            if (p == eptr->s) {
                auto inputstringprime = strip_prefix(p, s);
                return eptr->n->insert(inputstringprime); 
            }
            if (p == s) {
                auto edgestringprime = strip_prefix(p, eptr->s);
                es.push_back(std::shared_ptr<edge>(new edge(p, new node{new edge(edgestringprime, eptr->n)})));
                es.remove(eptr);
                return;
            }
        }
        //cerr << "Base case, about to insert" << endl;
        es.push_back(std::shared_ptr<edge>(new edge(s, new node)));
    }

    void print(std::ostream& o, std::string prefix) {
        o << "N" << endl;
        for (auto it = es.begin(); it != es.end(); ++it) {
            (*it)->print(o, prefix);
        }
    }

    void tikz_print(std::ostream& o, std::string s = "") {
        if (es.size() == 0) {
            o << "node{$\\times$} edge from parent node[left] {}";
        }
        else {
        if (es.size() == 1) {
            return (*es.begin())->tikz_print(o, (*es.begin())->s);
        }
        o << "node[circle, draw]{"+s+"}" << endl;
        for (auto it = es.begin(); it != es.end(); ++it) {
            o << "child {" << endl;
            std::string edgelabel = "";
            std::string vertlabel = "";
            edgelabel += (*it)->s[0];
            vertlabel += (*it)->s[1];
            (*it)->tikz_print(o, vertlabel);
            o << "edge from parent node[left]{" << edgelabel << "}" << endl;
            o << "}" << endl;
        }
        }
    }
};
void edge::print(std::ostream& o, std::string prefix) {
    o << "(" << prefix << ")" << s << endl;
    n->print(o, prefix+s);
}
void edge::tikz_print(std::ostream& o, std::string s) {
    n->tikz_print(o, s);
}
class radixtree{
    private:
        std::shared_ptr<node> r;
    public:
        radixtree(): r(new node) {}
        void insert(std::string s) {
            r->insert(s);
        }
        void print(std::ostream& o) {
            r->print(o, "");
        }
        void tikz_print(std::ostream& o) {
            o << "\\";
            r->tikz_print(o);
            o << ";" << endl;
        }
};

std::ostream& operator<<(std::ostream& o, radixtree& r) {
    r.print(o);
    o << endl;
    return o;
}
