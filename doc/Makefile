#
# Projekt do predmetu Teorie obvodu
# Autor:	Ondrej Benes
# login:	xbenes00
# Skupina:	1BIA - 10
# Datum:	1.1.2007
#

CO=xvokra00

all: $(CO).pdf

pdf: $(CO).pdf

$(CO).ps: $(CO).dvi
	dvips $(CO)

$(CO).pdf: clean
	pdflatex $(CO)
	# bibtex $(CO)
	pdflatex $(CO)
	pdflatex $(CO)

$(CO).dvi: $(CO).tex $(CO).bib
	latex $(CO)
	bibtex $(CO)
	latex $(CO)
	latex $(CO)

desky:
#	latex desky
#	dvips desky
#	dvipdf desky
	pdflatex desky

clean:
	rm -f *.dvi *.log $(CO).blg $(CO).bbl $(CO).toc *.aux $(CO).out $(CO).lof
	rm -f $(CO).pdf
	rm -f *~

pack:
	tar czvf $(CO).tar.gz *.tex *.bib *.bst ./fig/* ./cls/* Makefile Changelog

pack:
