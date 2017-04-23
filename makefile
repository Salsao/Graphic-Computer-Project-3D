all: apresentacao compila

apresentacao:
	@echo "---------------------------------------------------------"
	@echo "| \tFINAL PROJECT				|"
	@echo "| \tGRAPHIC COMPUTER 2014/2			|"
	@echo "---------------------------------------------------------"

compila:
	@echo ""
	@echo "Compiling..."
	g++ -c Principal.cpp DesenhaTanque.cpp LeituraArquivo.cpp hexaRGB.cpp tinystr.cpp tinyxml.cpp tinyxmlerror.cpp tinyxmlparser.cpp imageloader.cpp -lglut -lGLU -lGL
	g++ -o Principal Principal.o DesenhaTanque.o LeituraArquivo.o hexaRGB.o tinystr.o tinyxml.o tinyxmlerror.o tinyxmlparser.o imageloader.o -lglut -lGLU -lGL

run:
	@echo ""
	@echo "Executing..."
	./Principal

clear:
	@echo ""
	@echo "Cleaning..."
	rm -rf *.o
