## Programs
MV := mv
RM := rm -f
PDFLATEX := pdflatex

## Files:
TEX := $(shell ls *.tex)
PDF := $(patsubst %.tex,%.pdf,$(TEX))

.PHONY: doc
doc: $(PDF)

.PHONY: clean
clean:
	$(RM) *.aux *.log *.out *.pdf
	

%.pdf: %.tex
	$(PDFLATEX) $<
