#include<GL/glut.h>
#include<bits/stdc++.h>

using namespace std;

//used for debugging
// float min_x = numeric_limits<float>::max();
// float min_y = numeric_limits<float>::max();
// float min_z = numeric_limits<float>::max();

// float max_x = numeric_limits<float>::min();
// float max_y = numeric_limits<float>::min();
// float max_z = numeric_limits<float>::min();

class Vertex
{
public:
	float x, y, z;
	Vertex()
	{
		x = 0;
		y = 0;
		z = 0;
	}
};

class Normal
{
public:
	float x, y, z;
	Normal()
	{
		x = 0;
		y = 0;
		z = 0;
	}
};

class FaceItem
{
public:
	int vertexIndex;
	int normalIndex;
	FaceItem()
	{
		vertexIndex=0;
		normalIndex=0;
	}
};

class Face
{
public:
	std::vector<FaceItem> faceItems;
	Face()
	{
		faceItems.clear();
	}
};

class Object
{
public:
	std::vector<Vertex> vertices;
	std::vector<Normal> normals;
	std::vector<Face> faces;

	Object()
	{
		vertices.clear();
		normals.clear();
		faces.clear();

		Vertex v; Face f;
		vertices.push_back(v);
		faces.push_back(f);
	}
};

Object object;

vector<string> getNonEmptySubstrings(string line, char delimiter)
{
	vector <string> tokens; 
	stringstream check1(line); 
	string intermediate; 
	while(getline(check1, intermediate, delimiter)) //\t\f\v\n\r
	{ 
		if(!intermediate.empty() && intermediate!="\n" && intermediate!="\t" && intermediate!="\f" && intermediate!="\v" && intermediate!="\r")
		{
	        //cout<<intermediate<<intermediate.length()<<endl;
			tokens.push_back(intermediate); 
		}
	}

	return tokens;
}

vector<string> getSubstrings(string line, char delimiter)
{
	vector <string> tokens; 
	stringstream check1(line); 
	string intermediate; 
	while(getline(check1, intermediate, delimiter)) //\t\f\v\n\r
	{ 
		if(intermediate!="\n" && intermediate!="\t" && intermediate!="\f" && intermediate!="\v" && intermediate!="\r")
		{
	        //cout<<intermediate<<intermediate.length()<<endl;
			tokens.push_back(intermediate); 
		}
	}

	return tokens;
}

void getObjectDataFromLine(string line)
{
	if(line.length()==0)
		return;

	if(line[0] == '#' || line[0] == 'o' || line[0] == 'g')
		return;

	vector<string> tokens = getNonEmptySubstrings(line,' ');

	if(tokens.size()==0)
		return;

	if(tokens[0] == "v")
	{
		if(tokens.size()!=4)
		{
			cerr<<"Vertices must contain 3 coordinates"<<endl;
			exit(1);
		}

		Vertex vertex;
		vertex.x = atof(tokens[1].c_str());
		vertex.y = atof(tokens[2].c_str());
		vertex.z = atof(tokens[3].c_str());

		// if(vertex.x < min_x)
		// 	min_x = vertex.x;
		// if(vertex.y < min_y)
		// 	min_y = vertex.y;
		// if(vertex.z < min_z)
		// 	min_z = vertex.z;

		// if(vertex.x > max_x)
		// 	max_x = vertex.x;
		// if(vertex.y > max_y)
		// 	max_y = vertex.y;
		// if(vertex.z > max_z)
		// 	max_z = vertex.z;

		object.vertices.push_back(vertex);
	}

	else if(tokens[0] == "vn")
	{
		if(tokens.size()!=4)
		{
			cerr<<"Normals must contain 3 coordinates"<<endl;
			exit(1);
		}

		Normal normal;
		normal.x = atof(tokens[1].c_str());
		normal.y = atof(tokens[2].c_str());
		normal.z = atof(tokens[3].c_str());

		object.normals.push_back(normal);
	}

	else if(tokens[0] == "f")
	{
		if(tokens.size()<4)
		{
			cerr<<"Face must contain information about all 3 coordinates"<<endl;
			exit(1);
		}

		Face face;
		for(int i=1; i<tokens.size(); i++)
		{
			FaceItem item;
			vector<string> v = getSubstrings(tokens[i],'/');

			if(v.size()<1)
				cerr<<"Face information cant be empty"<<endl;

			if(v.size()==1)
			{

				if(v[0]!="")
					item.vertexIndex = atoi(v[0].c_str());
			}

			if(v.size()==3)
			{
				if(v[0]!="")
					item.vertexIndex = atoi(v[0].c_str());
				if(v[2]!="")
					item.normalIndex = atoi(v[2].c_str());
			}
			face.faceItems.push_back(item);
		}

		object.faces.push_back(face);
	}

}

void loadData(string filename)
{
	cout<<"Loading Data"<<endl;

	fstream objFile;

	objFile.open(filename,ios::in);

	if(!objFile.is_open())
	{
		cerr<<"Error in opening file "<<filename<<endl;
	}

	string line;
	while(getline(objFile,line))
	{
		getObjectDataFromLine(line);
	}

	cout<<"Number of vertices: "<<object.vertices.size()<<endl;
	cout<<"Number of normals: "<<object.normals.size()<<endl;
	cout<<"Number of faces: "<<object.faces.size()<<endl;

	// cout<<"x varies from "<<min_x<<" to "<<max_x<<endl;
	// cout<<"y varies from "<<min_y<<" to "<<max_y<<endl;
	// cout<<"z varies from "<<min_z<<" to "<<max_z<<endl;
	objFile.close();
}

void init()
{
	glClearColor(0.2,0.2,0.2,0.2);
	glMatrixMode(GL_MODELVIEW);
	glShadeModel( GL_SMOOTH );
	glEnable( GL_DEPTH_TEST );
	GLfloat amb_light[] = { 0.1, 0.1, 0.1, 1.0 };
	GLfloat diffuse[] = { 0.6, 0.6, 0.6, 1 };
	GLfloat specular[] = { 0.7, 0.7, 0.3, 1 };
	GLfloat position[] = {7.0,10.0,5.0};
	glLightModelfv( GL_LIGHT_MODEL_AMBIENT, amb_light );
	glLightfv( GL_LIGHT0, GL_DIFFUSE, diffuse );
	glLightfv( GL_LIGHT0, GL_SPECULAR, specular );
	glLightfv( GL_LIGHT0, GL_POSITION, position);
	glEnable(GL_LIGHTING);
	glEnable(GL_LIGHT0); 
}

void display()
{
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glLoadIdentity();
	glTranslatef(0,0,-17);
	glRotatef (-15.0, 1.0, 0.0, 0.0);
	glTranslatef(0,-5,0);
	glRotatef (-15.0, 1.0, 0.0, 0.0);
	glTranslatef(0,-5,0);
	glRotatef (-45.0, 1.0, 0.0, 0.0);
	glRotatef (10.0, 0.0, .0, 1.0);
	glTranslatef(5,0,-2);

	for(int i=1; i<object.faces.size(); i++)
	{
		vector<FaceItem> items = object.faces[i].faceItems;

		glBegin(GL_POLYGON);
		for(int j=0; j<items.size(); j++)
		{
			FaceItem item = items[j];
			int vertex_index = item.vertexIndex;
			int normal_index = item.normalIndex;
			glNormal3f(object.normals[normal_index].x, object.normals[normal_index].y, object.normals[normal_index].z);
			glVertex3f(object.vertices[vertex_index].x, object.vertices[vertex_index].y, object.vertices[vertex_index].z);
		}
	    // glVertex3f(-4,-5,0);
	    // glVertex3f(10,4,0);
		glEnd();
	}

	glutSwapBuffers();  
}

void reshape(int w, int h)
{
	glViewport(0,0,w,h);
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	gluOrtho2D(-4,10,-8.0,6.0);
	gluPerspective( 45, (float)w/(float)h, 1, 21);
	glMatrixMode( GL_MODELVIEW );
	glLoadIdentity();
}

int main(int argc, char** argv)
{
	loadData("hand.obj");
	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB | GLUT_DEPTH);
	glutInitWindowPosition(20,20);
	glutInitWindowSize(1000,1000);
	glutCreateWindow("3D Object - Hand.obj");
	glutDisplayFunc(display);
	glutReshapeFunc(reshape);
	init();
	glutMainLoop();
	return 0;
}
