all: apresentacao compila

apresentacao:
	@echo "---------------------------------------------------------"
	@echo "| \tTRABALHO CURTO 4				|"
	@echo "| \tCOMPUTAÇÃO GRÁFICA 2014/2			|"
	@echo "| \tPROFESSOR	: THIAGO OLIVEIRA DOS SANTOS	|"
	@echo "| \tALUNO 		: GUSTAVO DE ANDRADE GARCIA	|"
	@echo "| \tMATRICULA 	: 2012101405			|"
	@echo "---------------------------------------------------------"

compila:
	@echo ""
	@echo "Compilando..."
	g++ -c Principal.cpp DesenhaTanque.cpp LeituraArquivo.cpp hexaRGB.cpp tinystr.cpp tinyxml.cpp tinyxmlerror.cpp tinyxmlparser.cpp imageloader.cpp -lglut -lGLU -lGL
	g++ -o Principal Principal.o DesenhaTanque.o LeituraArquivo.o hexaRGB.o tinystr.o tinyxml.o tinyxmlerror.o tinyxmlparser.o imageloader.o -lglut -lGLU -lGL

run:
	@echo ""
	@echo "Executando..."
	./Principal

clear:
	@echo ""
	@echo "Limpando..."
	rm -rf *.o
