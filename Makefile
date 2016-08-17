all: radixtree prefixtree

radixtree: radixtree.h radixtree.cpp
	clang++ -Wall -std=c++0x radixtree.cpp -o radixtree

# pdf: radix_tree
# 	rm -f rmt.tex
# 	radix_tree < ./../maketree > rmt.tex
# 	pdflatex rmt

prefixtree: prefixtree.h prefixtree.cpp
	clang++ -Wall -std=c++0x prefixtree.cpp -o prefixtree

# pdf: prefix_tree
# 	rm -f pmt.tex
# 	prefix_tree < ./../maketree > pmt.tex
# 	pdflatex pmt

clean:
	rm -f *~ .o *.tex *.aux *.log *.pdf radixtree prefixtree
