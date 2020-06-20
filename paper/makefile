
DOCNAME=main
LATEXCOMMAND=latexmk --pdf

.PHONY: ${DOCNAME}.pdf

all: ${DOCNAME}.pdf

${DOCNAME}.pdf: $(DOCNAME).tex
	$(LATEXCOMMAND) $(DOCNAME)
	bibtex $(DOCNAME)
	$(LATEXCOMMAND) $(DOCNAME)
	$(LATEXCOMMAND) $(DOCNAME)

clean:
	rm -f *.aux *.log *.bbl *.blg