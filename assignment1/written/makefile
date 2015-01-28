default:
	latex written.tex
	dvips written.dvi

pdf: written.ps
	ps2pdf written.ps

clean:
	rm -fr written.aux written.dvi written.log written.ps written.pdf
