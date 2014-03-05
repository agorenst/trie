#include "radixtree.h"

using namespace std;

int main(int argc, char* argv[]) {
    string line;
    radixtree<char> t;
    while (cin) {
        getline(cin, line);
        if (!cin.eof()) {
            t.insert(line.begin(), line.end());
        }
    }
    cout <<
"\\documentclass{standalone}\
\\usepackage{tikz}\
\\usetikzlibrary{graphdrawing,graphdrawing.trees}\
\\begin{document}\
\\begin{tikzpicture}\\begin{scope}[tree layout]";
    t.tikz_print(cout);
cout << "\\end{scope}\
\\end{tikzpicture}\
\\end{document}";
    cout << endl;
}
