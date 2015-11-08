#Makefile Linux
EXEC=main
LD=g++

GLUT_INC=
GLUT_LIBS=-lglut
GL_INC=
GL_LIBS= -lGLU -lGL

#Chemin d'acces des includes
CXXINCS =  $(GLUT_INC) $(GL_INC) 
#Parametres de compilation
CXXFLAGS = $(CXXINCS) -ansi -Wall
#Chemin d'acces des librairies
LIBS = $(GLUT_LIBS) $(GL_LIBS) 

#Chemin d'acces des .h et .cpp (qui pourraient etre ailleurs que dans le répertoire principal)
PATH_INC=.
PATH_SRC=.

#Declare les targets qui ne sont pas des fichiers 
.PHONY: all all-before clean all-after mrproper

#Cree l'executable sous Linux
all: all-before $(EXEC) all-after

$(EXEC): struct.o main.o
	$(LD) -o $@ $^ $(LIBS)
	
struct.o:  $(PATH_SRC)/struct.cpp $(PATH_INC)/struct.h 
	$(CXX) -o $@ -c $< $(CXXFLAGS)

main.o: main.cpp $(PATH_INC)/struct.h

#Comfirme la fin de l'execution
all-after:
	@echo Compilation terminee avec succes !
	
#Efface l'ecran au debut de la compilation
all-before:
	clear
	
#Efface les fichiers .o
clean:
	rm -rf *.o
	
#Efface les .o + l'executable
mrproper: clean
	rm -rf $(EXEC).exe
