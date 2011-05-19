default: clean resume.tex
	latex resume
	pdflatex resume
	latex2html -nonavigation -noauto_navigation -nosubdir -nofootnode -noinfo -noaddress resume
	sed -i '' -e 's/<HR>//' resume.html
	latex2rtf resume.tex

install:
	./copy_out

clean:
	rm -f resume.aux resume.bbl resume.blg resume.dvi resume.log resume.pdf resume.txt resume.html WARNINGS resume.rtf index.html resume.css labels.pl
