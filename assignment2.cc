#include <iostream>
#include <fstream>
#include <GL/glut.h>
#include "assignment2.h"

int nRows = 480;
int nCols = 480; 
Volume vol;

static int spin = 0;
static int spin2 = 0;
static int begin;
static int begin2;

int button_id; 
int state_id;

Vector3f L(0,0,1);
Vector3f V(0,0,1);
float tmpf = 0.2;			//Used to adjust the contour level
float opacityScalar = 5.f; //Used to make adjust the opacity of levels


//---------------------------LOAD----------------------------------------------
void Volume::load(char * filename)
{  
	int   temp;
	int   data_compression;
	int   plane_size;
	int   volume_size;
	double f[3];
	int   size[3];
	int   magic_num;
	int   y_counter;
	int   icon_width, icon_height;
	int   compressed_size;

	unsigned char *compressed_ptr;
	unsigned char *scan_ptr = NULL;

	ifstream fin;
	fin.open(filename);

	// Initialize
	if (fin == NULL)
	{
		cerr << filename << " cannot be found " << endl;
		return;
	}

	//size[0] = x, size[1] = z, size[2] = y
	fin >> size[2] >> size[0] >> size[1];


	plane_size = size[2] * size[0]; //x*y
	volume_size = plane_size * size[1]; //x*y*z

	_volume = new unsigned short * [size[1]]; 
	for (int i = 0; i < size[1];i++) 
	{
		_volume[i] = new unsigned short [plane_size]; 
	}

	// Read In Data Plane By Plane (z)
	for( y_counter = 0; y_counter < size[1]; y_counter++ ) 
	{
		//for each y
		for (int i = 0; i < size[2]; i++) 
		{
			//for each x
			for (int j = 0; j < size[0]; j++) {
				_volume[size[1]-1 - y_counter][i*size[2] + j]=fin.get();
				fin >>_volume[size[1]-1 - y_counter][i*size[2] + j];
	//			fout.put(_volume[size[1]-1 - y_counter][i*size[2] + j]);
			}
		}
	}

	_size[0] = size[0];
	_size[1] = size[1];
	_size[2] = size[2];

	cerr << "Read " << volume_size << " points" << endl;
}


//------------------------------DISPLAY----------------------------------------

void myDisplay()
{
	glClear(GL_COLOR_BUFFER_BIT); // Clear OpenGL Window

	Vector3f v1,v2,v3;

	Vector3f N;
	int threshold = tmpf * 255.f;  // if you change this value and you will see different contours  
 
	for (int xi = 1 ; xi < vol.sizex()-1 ; xi++)  
	{
		for (int yi = 1 ; yi < vol.sizey()-1 ; yi++)  
		{ 
			float ain = 0;
			float aout = 0;		//Aplha out (opacity)
			float acurrent = 0;	//Holds current alpha
			float en = 1;		//Brightness
			
			float finalR = 0, finalG = 0, finalB = 0;
			float currentR = 0, currentG = 0, currentB = 0;
			
			for (int zi = 1 ; zi < vol.sizez()-1 ; zi++) 
			{
				//If on desired contour
				if (vol.volume(xi,yi,zi) > threshold) 
				{
					//------Base colour for current voxel-------
					//Muscle colour
					if (vol.volume(xi,yi,zi) <= 100) {
						currentR = 0.55, currentG = 0.1, currentB = 0.11;
					}
					//Bone colour
					else {
						currentR = 1, currentG = 0.83, currentB = 0.75;
					}
					
					//----------Get opacity and intensity-------
					acurrent = (float) vol.volume(xi,yi,zi)/(255.f*opacityScalar);
					
					if (ain == 0) {
						aout = acurrent;
					}
					else {
						aout = acurrent + (1 - acurrent) * ain;
					}
					
					//---------Add to colour for pixel----------
					finalR += (1 - ain) * currentR * acurrent * en; 
					finalG += (1 - ain) * currentG * acurrent * en;
					finalB += (1 - ain) * currentB * acurrent * en;
					
					ain = aout;
					
					//Stop if opacity reaches 1 as nothing beihind is visible
					if (aout >= 1) { break; };
				}
			}
			glColor3f(finalR,finalG,finalB);
			
			//Draws rectangle
			glRecti(xi-vol.sizex()/2 , 2*yi -vol.sizey(),
			xi-vol.sizex()/2+1, 2*yi-vol.sizey()+2);	
		}
	}

	glFlush();// Output everything
}

//---------------------------------KEY INPUT-----------------------------------
void changeCont(unsigned char key, int x, int y)
{

	//---------Contour Up--------------
	if (key == 'n') {
		tmpf += 0.01;
		if (tmpf > 1.0){
			tmpf = 1;
		}
		printf("tmpf now at %f\n", tmpf);
	}
	
	//--------Contour Down-------------
	else if (key == 'b') {
		tmpf -= 0.01;	
		if (tmpf < 0.0) {
			tmpf = 0;
		}
		printf("tmpf now at %f\n", tmpf);
	}
	
	//---------Opacity Up--------------
	if (key == 'v') {
		opacityScalar += 0.1;
		printf("opacityScalar now at %f\n", opacityScalar);
	}
	
	//--------Opacity Down-------------
	else if (key == 'c') {	
		opacityScalar -= 0.1;
		if (opacityScalar < 0.0) {
			opacityScalar = 0;
		}
		printf("opacityScalar now at %f\n", opacityScalar);
	}
	
	glutPostRedisplay();
}

//------------------------------MAIN-------------------------------------------

int main(int argc, char **argv)
{
	if (argc >  1)  {
		vol.load(argv[1]);
	}
	else {
		cerr << argv[0] << " <filename> " << endl;
		exit(1);
	}

	int width, height;

	glutInit(&argc, argv);
	glutInitWindowSize(nRows, nCols);
	glutCreateWindow("Sean Wilson");

	gluOrtho2D(-nRows/2, nRows/2, -(float)nCols/2,  (float)nCols/2);
	glutDisplayFunc(myDisplay);// Callback function
	glutKeyboardFunc(changeCont);
	glutMainLoop();// Display everything and wait
};
/*
if (vol.volume(xi,yi,zi) > threshold) 
{
	//Colour pixel
	float tmpc = (float)vol.volume(xi,yi,zi)/255.f;

	glColor3f(tmpc,tmpc,tmpc);

	//Draws rectangle
	glRecti(xi-vol.sizex()/2 , 2*yi -vol.sizey(),
	xi-vol.sizex()/2+1, 2*yi-vol.sizey()+2);
	break;
}*/
