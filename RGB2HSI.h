#include <stdio.h>
#include <math.h>
//#include <function_code.h>

int ABS_func(int N)
{
	if( N < 0 )
	{
		N *= (-1);
	}
	return N;
}


void RGB2HSI(float r,float g,float b, float* bh, float* bs, float* bi)
{
	float min, angle;
	float h, s, i;
	
	//float H, S, I;
		
	r=r/32.0f;
	g=g/64.0f;
	b=b/32.0f;
	
	//uPrintf0("[R:%f G:%f B:%f]",r,g,b);
	
	if((r<=g) && (r<=b)) 
		min=r;
	else if((g<=r) && (g<=b)) 
		min=g;
    else 
		min=b;
	
	
    i=(r+g+b)/3.0f;
	
	if(((ABS_func(r*255 - g*255)) < 15) && ((ABS_func(b*255 - g*255)) < 15))
	{
		s=0.0f;
		h=0.0f;
		i=(i*255.0f);
		*bh=h;
		*bs=s;
		*bi=i;
		return;
	}
    else 
    {
        s = 1.0f-(3.0f/(r+g+b)) * min;      //S의 계산
		angle = ( r - 0.5f * g - 0.5 * b) / sqrt( ( r - g )*( r - g )+( r - b )*( g - b ) );
        h = acos(angle);                         //H의 계산 
        h *= 57.29577951f;                       //Radian 으로 변환
    }
    if(( b > g ))
	{
		h=360.0f-h;
	}
	
    h=(h*255.0f/360.0f);             //0~255사이에 올수 있도록 정규화
	s=(s*255.0f);
    i=(i*255.0f);
	
	*bh=h;
	*bs=s;
	*bi=i;


}

