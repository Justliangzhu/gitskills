// Vect3d.h: interface for the Vect3d class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_VECT3D_H__502CDE6A_1BBB_4EEB_91BC_254812FB3834__INCLUDED_)
#define AFX_VECT3D_H__502CDE6A_1BBB_4EEB_91BC_254812FB3834__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

class Vect3d  
{
public:
	float x, y, z;
	Vect3d();
	Vect3d (float XX, float YY,  float ZZ); 

	virtual ~Vect3d();

    void Get(float &_x, float &_y, float &_z) const
	{ _x = x; _y = y; _z = z;}
	

    
	
    Vect3d (const Vect3d& v) 
	{ x=v.x; y=v.y; z=v.z; };
	
    void Set (float XX, float YY, float ZZ);	
	
    Vect3d& operator = (const Vect3d& A)          // ASSIGNMENT (=)
	{ x=A.x; y=A.y; z=A.z; 
	return(*this);  };
    Vect3d operator + (const Vect3d& A) const     // ADDITION (+)
	{ Vect3d Sum(x+A.x, y+A.y, z+A.z); 
	return(Sum); };
    Vect3d operator - (const Vect3d& A) const     // SUBTRACTION (-)
	{ Vect3d Diff(x-A.x, y-A.y, z-A.z);
	return(Diff); };
    float operator * (const Vect3d& A) const      // DOT-PRODUCT (*)
	{ float DotProd = x*A.x+y*A.y+z*A.z; 
	return(DotProd); };
    Vect3d operator / (const Vect3d& A) const     // CROSS-PRODUCT (/)
	{ Vect3d CrossProd(y*A.z-z*A.y, z*A.x-x*A.z, x*A.y-y*A.x); 
	return(CrossProd); };
    Vect3d operator * (const float s) const       // MULTIPLY BY SCALAR (*)
	{ Vect3d Scaled(x*s, y*s, z*s); 
	return(Scaled); };
    Vect3d operator / (const float s) const       // DIVIDE BY SCALAR (/)
	{ Vect3d Scaled(x/s, y/s, z/s);
	return(Scaled); };
	
    void operator += (const Vect3d A)             // ACCUMULATED VECTOR ADDITION (+=)
	{ x+=A.x; y+=A.y; z+=A.z; };
    void operator -= (const Vect3d A)             // ACCUMULATED VECTOR SUBTRACTION (+=)
	{ x-=A.x; y-=A.y; z-=A.z; };
    void operator *= (const float s)              // ACCUMULATED SCALAR MULT (*=)
	{ x*=s; y*=s; z*=s; };
    void operator /= (const float s)              // ACCUMULATED SCALAR DIV (/=)
	{ x/=s; y/=s; z/=s; };
    Vect3d operator - (void) const                // NEGATION (-)
	{ Vect3d Negated(-x, -y, -z);
	return(Negated); };
	
    float operator [] (const int i) const         // ALLOWS VECTOR ACCESS AS AN ARRAY.
	{ return( (i==0)?x:((i==1)?y:z) ); };
    float & operator [] (const int i)             
	{ return( (i==0)?x:((i==1)?y:z) ); };
	
    float Length (void) const                     // LENGTH OF VECTOR
	{ return ((float)sqrt(x*x+y*y+z*z)); };
    float LengthSqr (void) const                  // LENGTH OF VECTOR (SQUARED)
	{ return (x*x+y*y+z*z); };
    void Normalize (void)                         // NORMALIZE VECTOR
	{ float L = Length();                       // CALCULATE LENGTH
	if (L > 0.0) {
		x/=L; y/=L; z/=L;} };                      // DIVIDE COMPONENTS BY LENGTH
		
		void Print() const
		{ printf("(%.3f, %.3f, %.3f)\n",x, y, z); }
		
		void _min (const Vect3d & a)
		{ if (x>a.x) x=a.x; if (y>a.y) y=a.y; if (z>a.z) z=a.z; }
		void _max (const Vect3d & a)
		{ if (x<a.x) x=a.x; if (y<a.y) y=a.y; if (z<a.z) z=a.z; }
		Vect3d multComponents (const Vect3d& A) const
		{ return Vect3d (x*A.x, y*A.y, z*A.z); }

};

#endif // !defined(AFX_VECT3D_H__502CDE6A_1BBB_4EEB_91BC_254812FB3834__INCLUDED_)
