#version 130 

out vec3 mynormal ; 
out vec4 myvertex ; 

uniform mat4 myprojection_matrix;
uniform mat4 myview_matrix;
uniform mat4 mymodel_matrix;

void main() {
    gl_Position = myprojection_matrix * myview_matrix * mymodel_matrix * gl_Vertex ; 
    mynormal = gl_Normal ; 
    myvertex = gl_Vertex ; 
}

