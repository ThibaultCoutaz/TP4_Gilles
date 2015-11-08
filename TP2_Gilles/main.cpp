
#include <stdio.h>
#include <math.h>
#include <stdlib.h>
#include <GL/glut.h>
#include "struct.h"
#include <iostream>
#include <string>
#include <fstream>

/* au cas ou M_PI ne soit defini */
#ifndef M_PI
#define M_PI 3.14159265358979323846
#endif

using namespace std;

#define WIDTH  600
#define HEIGHT 600

#define KEY_ESC 27

float angleX=0.0f; //angle de rotation en Y de la scene
float angleY=0.0f; //angle de rotation en X de la scene

float pasDeplacement=1.25;


//position lumiere
float xLitePos=0;
float yLitePos=10;
float zLitePos=-100;

float tx=0.0;
float ty=0.0;
float tz=0.0;

//The Array for the Values
float *Sommets;
int *Facettes;
float *Normals;

int nbSommets ,nbFacettes,nbArretes;
string type;
float hight=0;
int nbApF=0; // Nb Arretes par Face

//Middle of the Obj
point3 center(0.,0.,0.);


/* initialisation d'OpenGL*/
static void init()
{
	glClearColor(0.0, 0.0, 0.0, 0.0);
	// Si vous avez des choses à initialiser, c est ici.	
}


//To calculate the normal of each triangle.
void normalFace(point3 _Sommets1,point3 _Sommets2,point3 _Sommets3,int _index){
	point3 v12(_Sommets2.x-_Sommets1.x,_Sommets2.y-_Sommets1.y,_Sommets2.z-_Sommets1.z);
	point3 v13(_Sommets3.x-_Sommets1.x,_Sommets3.y-_Sommets1.y,_Sommets3.z-_Sommets1.z);

	point3 normal(0.,0.,0.);
	normal.x = v12.y*v13.z - v12.z*v13.y;
	normal.y = v12.z*v13.x - v12.x*v13.z;
	normal.z = v12.x*v13.y - v12.y*v13.x;

	Normals[_index]=normal.x;
	Normals[_index+1]=normal.y;
	Normals[_index+2]=normal.z;
}

//To Manage the File
void ManageFile(string File){
	ifstream fichier(File.c_str(),ios::in);

	if(fichier){
		cout<<"Ouverture du fichier (lecture) : "<<File.c_str()<<endl;

		//****Lecture File****//
		fichier>>type>>nbSommets>>nbFacettes>>nbArretes;
		//cout<<nbSommets<<"//"<<nbFacettes<<endl;

		Sommets = new float[nbSommets*3];
		Facettes = new int[nbFacettes*3];
		Normals = new float[nbFacettes*3];
		float x=0;
		float y=0;
		float z=0;

		//To Put in the Sommets[] the value of the Sommets
		int index=0;
		for(int i=0;i<nbSommets;i++){
			fichier>>Sommets[index]>>Sommets[index+1]>>Sommets[index+2];
			
			x+=Sommets[index];
			y+=Sommets[index+1];
			z+=Sommets[index+2];

			//Find the higher value
			if(abs(Sommets[index])>hight){
				hight=Sommets[index];
			}
			if(abs(Sommets[index+1])>hight){
				hight=Sommets[index+1];
			}
			if(abs(Sommets[index+2])>hight){
				hight=Sommets[index+2];
			}
			index+=3;
		}

		index=0;
		//Put the value between -1 and 1
		for(int i=0;i<nbSommets;i++){
			
			Sommets[index]/=hight;
			Sommets[index+1]/=hight;
			Sommets[index+2]/=hight;

			index+=3;

		}


		//***********PENSER A FAIRE LE CENTRAGE DU MODEL***************//
		center.x=x/nbSommets;
		center.y=y/nbSommets;
		center.z=z/nbSommets;

		//cout<<"Centre : "<<center.x<<"/"<<center.y<<"/"<<center.z<<endl;

		//To put in the Facettes[] the value of the Faces
		index=0;
		int nb;//To put the number of Face

		for(int i=0;i<nbFacettes;i++){
			fichier>>nbApF>>Facettes[index]>>Facettes[index+1]>>Facettes[index+2];

			index+=3;
		}

		
		cout<<" Fermeture du fichier (lecture) : "<<File.c_str()<<endl;
		fichier.close();

	}else{
		cout<<"Erreur d'ouverture du fichier (lecture) : "<<File.c_str()<<endl;
	}
}

void WriteFile(string File){
	ofstream fichier(File,ios::out);

	if(fichier){
		cout<<"Ouverture du fichier (Ecriture) : "<<File.c_str()<<endl;

		fichier<<"OFF"<<endl;
		fichier<<nbSommets<<" "<<nbFacettes<<" "<<nbArretes<<endl;

		//To write the x,y,z of each sommets
		int index=0;
		for(int i=0;i<nbSommets;i++){
			fichier<<Sommets[index]*hight<<" "<<Sommets[index+1]*hight<<" "<<Sommets[index+2]*hight<<endl;
			
			index+=3;
		}

		//To put in the Facettes[] the value of the Faces
		index=0;
		int nb;//To put the number of Face

		for(int i=0;i<nbFacettes;i++){
			fichier<<nbApF<<" "<<Facettes[index]<<" "<<Facettes[index+1]<<" "<<Facettes[index+2]<<endl;

			index+=3;
		}

		cout<<"Fermeture du fichier (Ecriture) : "<<File.c_str()<<endl;
		fichier.close();
	}else{
		cout<<"Echec Ouverture du fichier (Ecriture) : "<<File.c_str()<<endl;
	}
}

void DrawObj(){

	glPushMatrix();
	glBegin(GL_TRIANGLES);	

	int index=0;

	for(int i=0;i<nbFacettes;i++){

		glColor3f(0.7,0.7,0.7);//couleur gris
		glVertex3f(Sommets[Facettes[index]*3],Sommets[Facettes[index]*3+1], Sommets[Facettes[index]*3+2]);
		glVertex3f(Sommets[Facettes[index+1]*3],Sommets[Facettes[index+1]*3+1], Sommets[Facettes[index+1]*3+2]);
		glVertex3f(Sommets[Facettes[index+2]*3],Sommets[Facettes[index+2]*3+1], Sommets[Facettes[index+2]*3+2]);

		/*cout<<"sommet1 :"<<Sommets[Facettes[index]*3]<<"--"<<Sommets[Facettes[index]*3+1]<<"--"<<Sommets[Facettes[index]*3+2]<<endl;
		cout<<"sommet2 :"<<Sommets[Facettes[index+1]*3]<<"--"<<Sommets[Facettes[index+1]*3+1]<<"--"<<Sommets[Facettes[index+1]*3+2]<<endl;
		cout<<"sommet3 :"<<Sommets[Facettes[index+2]*3]<<"--"<<Sommets[Facettes[index+2]*3+1]<<"--"<<Sommets[Facettes[index+2]*3+2]<<endl;*/

		point3 Sommet1(Sommets[Facettes[index]*3],Sommets[Facettes[index]*3+1],Sommets[Facettes[index]*3+2]);
		point3 Sommet2(Sommets[Facettes[index+1]*3],Sommets[Facettes[index+1]*3+1], Sommets[Facettes[index+1]*3+2]);
		point3 Sommet3(Sommets[Facettes[index+2]*3],Sommets[Facettes[index+2]*3+1], Sommets[Facettes[index+2]*3+2]);

		normalFace(Sommet1,Sommet2,Sommet3,index);

		index+=3;
	}

	glEnd();
	glPopMatrix();	
}

void WriteInfos(){
	cout<<"Nombre de Sommets : "<<nbSommets<<endl;
	cout<<"Nombre d'arretes : "<<nbArretes<<endl;
	cout<<"Nombre de Faces : "<<nbFacettes<<endl;
	cout<<"Nombre d'arretes par Face : "<<nbApF<<endl;

	/*int index=0;
		for(int i=0;i<nbSommets;i++){
			fichier<<Sommets[index]*hight<<" "<<Sommets[index+1]*hight<<" "<<Sommets[index+2]*hight<<endl;
			
			index+=3;
		}*/
}

/* Dessin */
void display(void)
{
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT); //effacement du buffer

	glEnable(GL_DEPTH_TEST);
	//glEnable(GL_LIGHTING);
	//glEnable(GL_LIGHT0);
	glEnable(GL_CULL_FACE);
	//Description de la scene
	glLoadIdentity();
	//rotation de la scene suivant les mouvements de la souris
	glRotatef(-angleY,0.0f,0.0f,1.0f);
	glRotatef(angleX,0.0f,1.0f,0.0f);
	glTranslatef(tx,ty,tz);

	glRotatef(-angleY,0.0f,0.0f,1.0f);
	glRotatef(angleX,0.0f,1.0f,0.0f);

	DrawObj();
	WriteFile("test.off");

	//affiche les axes du repere
		glColor3f(0.0,1.0,0.0); //Y vert
		glBegin(GL_LINES);
			glVertex3f(0,0,0);
			glVertex3f(0,20,0);
		glEnd();

		glColor3f(0.0,0.0,1.0); //Z bleu
		glBegin(GL_LINES);
			glVertex3f(0,0,0);
			glVertex3f(0,0,20);
		glEnd();
		
		glColor3f(1.0,0.0,0.0); //X rouge
		glBegin(GL_LINES);
			glVertex3f(0,0,0);
			glVertex3f(20,0,0);
		glEnd();

	glutSwapBuffers();// echange des buffers
}



/* Au cas ou la fenetre est modifiee ou deplacee */
void reshape(int w, int h)
{
   glViewport(0, 0, (GLsizei) w, (GLsizei) h);
   glMatrixMode(GL_PROJECTION);
   glLoadIdentity();
   glOrtho(-20, 20, -20, 20, -10, 10);
   glMatrixMode(GL_MODELVIEW);
   glLoadIdentity();
}

GLvoid gestionSouris(int x, int y)
{
	angleX=x*720/WIDTH; //gere l'axe Oy
	angleY=-(y*180/HEIGHT-90)*4; //gere l'axe Ox

	glutPostRedisplay();
}

GLvoid gestionFleche(int key, int x, int y) 
{
	switch (key) {
		case GLUT_KEY_UP :
			tz+=int(pasDeplacement);glutPostRedisplay();
		break;

		case GLUT_KEY_DOWN :
			tz-=int(pasDeplacement);glutPostRedisplay();
		break;
	glutPostRedisplay();
	}

}

GLvoid window_key_down(unsigned char key, int x, int y)  //appuie des touches
{  
switch (key) 
{

//deplacement de la camera
	case 'z':
		ty+=int(pasDeplacement);glutPostRedisplay();
	break;

	case 's':
		ty-=int(pasDeplacement);glutPostRedisplay();
	break;

	case 'q':
		tx-=int(pasDeplacement);glutPostRedisplay();
	break;

	case 'd':
		tx+=int(pasDeplacement);glutPostRedisplay();
	break;

//sortie
	case KEY_ESC:
		exit(0);
		break;

	default:
		printf ("La touche %d non active.\n", key);
	break;
	}
glutPostRedisplay();
}

int main(int argc, char **argv)
{
	ManageFile("buddha.off");
   glutInitWindowSize(1000, 1000);
   glutInit(&argc, argv);
   glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB | GLUT_DEPTH);
   glutCreateWindow("Mon Obj");
   init();
  // glutReshapeFunc(reshape);
   glutKeyboardFunc(&window_key_down);
   glutDisplayFunc(display);
   glutPassiveMotionFunc(gestionSouris);
   glutSpecialFunc(&gestionFleche);
   glutMainLoop();
   return 0;


   //**To Clean**//
   delete[] Sommets;
   delete[] Facettes;
}
