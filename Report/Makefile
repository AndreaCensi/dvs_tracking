name=Report

all:
	pdflatex $(name).tex # first pass
	bibtex $(name) 			 # extracts reference data
	pdflatex $(name).tex # matches citations/references
	pdflatex $(name).tex # finishes all cross-referencing

	# remove temporary files
	rm *.log
	rm *.blg
	rm *.aux
	rm *.toc
	rm *.bbl
	rm *.out

