#include <vector>
#include <sstream>
#include "prefixtree.h"

using namespace std;

int main(int argc, char* argv[]) {
    string line;
    prefix_tree<int> t;
    while (cin) {
        getline(cin, line);
        istringstream int_parser(line);
        int x;
        vector<int> int_stream;
        while (!int_parser.eof()) {
            int_parser >> x;
            int_stream.push_back(x);
        }
        t.insert(int_stream.rbegin(), int_stream.rend());
    }
    cout << 
"\\documentclass{standalone}\
\\usepackage{tikz}\
\\begin{document}\
\\usetikzlibrary{graphdrawing,graphdrawing.trees}\
\\begin{tikzpicture}\\begin{scope}[tree layout]";
    t.tikz_print(cout);
    cout <<
"\\end{scope}\
\\end{tikzpicture}\
\\end{document}";
    cout << endl;
}

