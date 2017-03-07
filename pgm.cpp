#include "StdAfx.h"
//-----------------------------------------------------------------
#include "pgm.h"

#include <fstream>
using namespace std; // std::cout 를 통해 이야기 하는 부분.

PGM::PGM(int _w,int _h) // 너비와 높이를 입력 받는 PGM 이며, 초기화에 해당한다고 생각하며된다.
{
    pixels=NULL; 
    reset(_w,_h);
}
PGM::PGM(int _w,int _h,unsigned char *gray)
{
    pixels=NULL;
    reset(_w,_h);

    for(int x=0;x<_w;x++){
        for(int y=0;y<_h;y++){
            pixels[x][y]=gray[x+640*y]/255.0;
        }
    }
}
//-----------------------------------------------------------------
PGM::PGM(const PGM &src)
{
    pixels=NULL;
    reset(src.W,src.H);
    
    std::memcpy(pixels[0],src.pixels[0],W*H*sizeof(double));
}
//-----------------------------------------------------------------
PGM::PGM(const string &file_name)
{
    pixels=NULL;
    load_from_file(file_name);
}
PGM::~PGM()
{
    free();
}
void PGM::reset(int _w,int _h) // reset 부분
{
    free(); // pixels의 배열 을 초기화 해준다.
    
    W=_w;
    H=_h;
    
    pixels   =new double*[W];
    pixels[0]=new double[W*H];
    for(int x=1;x<W;x++) pixels[x]=pixels[x-1]+H;
}
void PGM::free()
{
    if(pixels!=NULL){ // pixels에 수가 있을 경우 if문
        if(pixels[0]!=NULL) delete[] pixels[0]; //
        delete[] pixels;
        pixels=NULL;
    }
    
    W=0;
    H=0;
}
PGM& PGM::operator=(const PGM &a)
{
    reset(a.width(),a.height());
    std::memcpy(pixels[0],a.pixels[0],W*H*sizeof(double));
    
    return *this;
}
double& PGM::operator()(int i,int j)
{
    if(i<0)       i=0;
    else if(i>=W) i=W-1;
    if(j<0)       j=0;
    else if(j>=H) j=H-1;
    
    return pixels[i][j];
}
PGM PGM::operator*(double a)
{
    PGM tmp(W,H);
    
    for(int x=0;x<W;x++){
        for(int y=0;y<H;y++){
            tmp.pixels[x][y]=a*pixels[x][y];
        }
    }    
    
    return tmp;
}
PGM PGM::operator+(const PGM &a)
{
    int min_w = (a.width()  < W ) ? a.width()  : W;
    int min_h = (a.height() < H ) ? a.height() : H;
    
    PGM tmp(min_w,min_h);
    
    for(int x=0;x<min_w;x++){
        for(int y=0;y<min_h;y++){
            tmp.pixels[x][y]=a.pixels[x][y]+pixels[x][y];
        }
    }
    
    return tmp;
}
PGM PGM::operator-(const PGM &a)
{
    int min_w = (a.width()  < W ) ? a.width()  : W;
    int min_h = (a.height() < H ) ? a.height() : H;
    
    PGM tmp(min_w,min_h);
    
    for(int x=0;x<min_w;x++){
        for(int y=0;y<min_h;y++){
            tmp.pixels[x][y]=a.pixels[x][y]-pixels[x][y];
        }
    }
    
    return tmp;
}
bool PGM::load_from_file(const string &file_name)
{

	//ifstream file("c:\\img3.pgm");
	ifstream file(file_name.c_str(),ios::in|ios::binary);
    
    if(!file.is_open()) return false;
    
    bool grayscale=true;
    bool p5=false;
	string str;
	
    
    file>>str;
    if(str=="P3") grayscale=false;     
    else if(str!="P2" && str!="P5") return false;    //PGMファイル？
	else if(str=="P5") p5=true;

    for(int i=0;i<3;i++){
        file>>str;
        while(str[0]=='#'){
            getline(file,str);
            file>>str;
        }
        switch(i){
            case 0: W=std::atoi(str.c_str()); break;
            case 1: H=std::atoi(str.c_str()); break;
        }
    }
    
    //配列確保
    reset(W,H);
    
	unsigned char * buffer = new unsigned char[W*H+1];

	file.read((char*)buffer, W*H+1); 

	int fi=1; 
	if(p5)
	{
		for(int y=0;y<H;y++){
			for(int x=0;x<W;x++){
				pixels[x][y]=(buffer[fi])/255.0;
				fi++;
			}
		}
	}
	else
	{
		
		for(int y=0;y<H;y++){
			for(int x=0;x<W;x++){
				file>>str;
				while(str[0]=='#'){
					getline(file,str);
					file>>str;
				}
            
				if(grayscale) pixels[x][y]=std::atoi(str.c_str())/255.0;
				else{
					double r=std::atoi(str.c_str())/255.0;
					double g,b;
					for(int j=0;j<2;j++){
						file>>str;
						while(str[0]=='#'){
							getline(file,str);
							file>>str;
						}
						switch(j){
							case 0: g=std::atoi(str.c_str())/255.0; break;
							case 1: b=std::atoi(str.c_str())/255.0; break;
						}
					}
                
					pixels[x][y]=r*0.299+g*0.587+b*0.114;
				}
			}
		}
    }
    file.close();
    return true;
}
bool PGM::save_to_file(const string &file_name)
{
    ofstream file(file_name.c_str());
    
    if(!file.is_open()) return false;
    
    file<<"P2"<<std::endl;
    
    file<<"# CREATOR: no name"<<std::endl;
    file<<W<<" "<<H<<std::endl;
    file<<255<<std::endl;
    
    for(int y=0;y<H;y++){
        for(int x=0;x<W;x++){
            double tmp=pixels[x][y];
            if(tmp<0) tmp=0;
            else if(tmp>1) tmp=1;
            
            file<<(int)(255*tmp)<<endl;
        }
    }
    file.close();
    
    return true;
}
//-----------------------------------------------------------------


