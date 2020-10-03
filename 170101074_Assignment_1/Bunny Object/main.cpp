#include<GL/glut.h>
#include<bits/stdc++.h>

using namespace std;

float min_x = numeric_limits<float>::max();
float min_y = numeric_limits<float>::max();
float min_z = numeric_limits<float>::max();

float max_x = numeric_limits<float>::min();
float max_y = numeric_limits<float>::min();
float max_z = numeric_limits<float>::min();

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

class Texture
{
public:
  float x, y, z;
  Texture()
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
  int textureIndex;
  FaceItem()
  {
    vertexIndex=0;
    normalIndex=0;
    textureIndex=0;
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
  std::vector<Texture> textures;
  std::vector<Normal> normals;
  std::vector<Face> faces;

  Object()
  {
    vertices.clear();
    textures.clear();
    normals.clear();
    faces.clear();

    Vertex v; Texture t; Face f;
    vertices.push_back(v);
    textures.push_back(t);
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

      if(vertex.x < min_x)
        min_x = vertex.x;
      if(vertex.y < min_y)
        min_y = vertex.y;
      if(vertex.z < min_z)
        min_z = vertex.z;

      if(vertex.x > max_x)
        max_x = vertex.x;
      if(vertex.y > max_y)
        max_y = vertex.y;
      if(vertex.z > max_z)
        max_z = vertex.z;

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

    else if(tokens[0] == "vt")
    {
      Texture texture;
      if(tokens.size()==3)
      {
        texture.x = atof(tokens[1].c_str());
        texture.y = atof(tokens[2].c_str());
      }
      else if(tokens.size()==4)
      {
        texture.x = atof(tokens[1].c_str());
        texture.y = atof(tokens[2].c_str());
        texture.z = atof(tokens[3].c_str());
      }
      else
      {
        cerr<<"textures must contain 2 or 3 coordinates"<<endl;
        exit(1);
      }

      object.textures.push_back(texture);
    }

    else if(tokens[0] == "f")
    {
      if(tokens.size()!=4)
      {
        cerr<<"Face must contain information about all 3 coordinates"<<endl;
        exit(1);
      }

      Face face;
      for(int i=1; i<4; i++)
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
        if(v.size()==2)
        {
          if(v[0]!="")
            item.vertexIndex = atoi(v[0].c_str());
          if(v[1]!="")
            item.textureIndex = atoi(v[1].c_str());
        }
        if(v.size()==3)
        {
          if(v[0]!="")
            item.vertexIndex = atoi(v[0].c_str());
          if(v[1]!="")
            item.textureIndex = atoi(v[1].c_str());
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
  cout<<"Number of textures: "<<object.textures.size()<<endl;
  cout<<"Number of faces: "<<object.faces.size()<<endl;

  cout<<"x varies from "<<min_x<<" to "<<max_x<<endl;
  cout<<"y varies from "<<min_y<<" to "<<max_y<<endl;
  cout<<"z varies from "<<min_z<<" to "<<max_z<<endl;
  objFile.close();
}

void init()
{
  glClearColor(0.0,0.0,0.0,0.0);
  glShadeModel (GL_SMOOTH);
  glEnable(GL_DEPTH_TEST);
  glEnable(GL_LIGHTING);
  glEnable(GL_LIGHT0);
}

void display()
{
  glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
  glLoadIdentity();
  gluLookAt (0.0, 0.0, 1.0, 0.0, 0.0, 0.0, 0.0, 1.0, 0.0);
  
  for(int i=1; i<object.faces.size(); i++)
  {
    vector<FaceItem> items = object.faces[i].faceItems;

    glBegin(GL_POLYGON);
    for(int j=0; j<items.size(); j++)
    {
      FaceItem item = items[j];
      int vertex_index = item.vertexIndex;

      glVertex3f(object.vertices[vertex_index].x, object.vertices[vertex_index].y, object.vertices[vertex_index].z);
    }
    // glVertex3f(0,0,0);
    glEnd();
  }

  glutSwapBuffers();  
}

void reshape(int w, int h)
{
  glViewport(0,0,w,h);
  glMatrixMode(GL_PROJECTION);
  glLoadIdentity();
  gluOrtho2D(-1,1,-0.5,1.5);
  gluPerspective( 45, (float)w/(float)h, 0, 2);
  glMatrixMode( GL_MODELVIEW );
  glLoadIdentity();
}

int main(int argc, char** argv)
{
  loadData("lowpolybunny.obj");
  glutInit(&argc, argv);
  glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB | GLUT_DEPTH);
  glutInitWindowPosition(200,200);
    glutInitWindowSize(500,500);
  glutCreateWindow("3D Object");
  glutDisplayFunc(display);
  glutReshapeFunc(reshape);
  init();
  glutMainLoop();
  return 0;
}