default: clean resume.tex
	latex resume 1> /dev/null
	pdflatex resume 1> /dev/null
	latex2html -nonavigation -noauto_navigation -nosubdir -nofootnode -noinfo -noaddress resume 1> /dev/null
	sed -i '' -e 's/<HR>//' resume.html
	latex2rtf resume.tex 1> /dev/null

install: resume.pdf
	./copy_out

resume.pdf: default

clean:
	rm -f resume.aux resume.bbl resume.blg resume.dvi resume.log resume.pdf resume.txt resume.html WARNINGS resume.rtf index.html resume.css labels.pl
