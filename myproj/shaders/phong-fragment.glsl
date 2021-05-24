#version 130 


in vec3 mynormal ; 
in vec4 myvertex ; 


vec4 light0posn = vec4(5, 0, 5, 1); 
vec4 light0color = vec4(0.4, 0.5, 0.4, 1) ; 

uniform vec4 kd;

uniform mat4 myview_matrix;
uniform mat3 mynormal_matrix;

out vec4 color;


vec4 ComputeLight (const in vec3 direction, const in vec4 lightcolor, const in vec3 normal, const in vec3 halfvec, const in vec4 myambient, const in vec4 mydiffuse, const in vec4 myspecular, const in float myshininess) 
{
        float nDotL = dot(normal, direction)  ;         
        vec4 diffuse = mydiffuse * lightcolor * max (nDotL, 0.0) ;  

        float nDotH = dot(normal, halfvec) ; 
        vec4 specular = myspecular * lightcolor * pow (max(nDotH, 0.0), myshininess) ; 

	    vec4 retval = myambient + diffuse + specular ; 
				
        return retval ;            
} 
 

void main (void) 
{      
        const vec3 eyepos = vec3(0,0,0) ; 
		
        vec4 _mypos = myview_matrix * myvertex ; 
        vec3 mypos = _mypos.xyz / _mypos.w ; 
		
		vec4 _lightpos = light0posn;
		vec3 lightpos = _lightpos.xyz / light0posn.w ; 

		vec3 normal = normalize(mynormal_matrix * mynormal) ; 
		if (!gl_FrontFacing) normal = -normal;

		vec3 eyedirn = normalize(eyepos - mypos) ;
		vec3 lightdirn = normalize(lightpos - mypos);
		vec3 half0 = normalize (lightdirn + eyedirn) ;  

        color = ComputeLight(lightdirn, light0color, normal, half0, 
								  vec4(0.0,0.1,0.1,100), kd, vec4(0.1,0.5,0.1,0), 100) ;
}
