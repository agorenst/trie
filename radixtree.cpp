#include "radixtree.h"

using namespace std;

int main(int argc, char* argv[]) {
    string line;
    radixtree t;
    while (cin) {
        getline(cin, line);
        t.insert(line);
    }
    cout << 
"\\documentclass{standalone}\
\\usepackage{tikz}\
\\begin{document}\
\\begin{tikzpicture}";
    t.tikz_print(cout);
    cout <<
"\\end{tikzpicture}\
\\end{document}";
    cout << endl;
}
