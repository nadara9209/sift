#include "StdAfx.h"
//-----------------------------------------------------------------
#include <iostream>
#include <algorithm> 

using namespace std;

//-----------------------------------------------------------------
#include <math.h>
#include "sift.h"
#include "time.h"

int OCT;
const double SIGMAi      = 0.5;  
const double SIGMA0      = 1.6; /
const int    S           = 3;  // 한 옥타브 당 효율이 높은 것은 3개의 scale을 가지고 있을 때 라고, 논문 p.9 에 명시되어 있다.  
const bool   IMG_DOUBLED = false;   
const double CONTR_THR   = 0.04;    
const double CURV_THR    = 10;
const double EDGE_THR    = (CURV_THR+1.0)*(CURV_THR+1.0)/CURV_THR;


const double ORI_SIG_FCTR= 1.5;
const double ORI_RADIUS  = 3.0*ORI_SIG_FCTR;

const double DESCR_MAG_THR = 0.2;
const double DESCR_INT_FCTR= 512.0;

#define ORI_HIST_BINS 36
#define round(dbl) dbl >= 0.0 ? (int)(dbl + 0.5) : ((dbl - (double)(int)dbl) <= -0.5 ? (int)dbl : (int)(dbl - 0.5))
#define M_PI       3.14159265358979323846 // Pi의 값을 정의한 것.


PGM  create_init_image(PGM &src);
void build_image_pyramid(PGM &base,PGM **L,PGM **DoG);
void scale_space_extrema(PGM **DoG,list<SIFT_DESCRIPTOR> &keys);
void calc_orientation(PGM **L,list<SIFT_DESCRIPTOR> &keys);
void compute_descriptors(PGM **L,list<SIFT_DESCRIPTOR> &keys);


PGM  gaussian_filter(PGM &src,double sig);
PGM  downsample(PGM &src);
void sub_pgm(PGM &dst,PGM &a,PGM &b);
void inverse(double mat[3][3],double inv[3][3]);

#ifndef ABS
#define ABS(x) ( ( x < 0 )? -x : x )
#endif


void SIFT(PGM &_src,list<SIFT_DESCRIPTOR> &keys,CString &t)
{
    PGM init_image=create_init_image(_src); ///이미지 생성하는 부분
    
	clock_t start; //필요 없는 시간 재는 부분
	clock_t end;//필요 없는 시간 재는 부분

	t.Format(_T("time= "));//필요 없는 시간 재는 부분

    OCT=(int)(log((double)min(init_image.width(),init_image.height())) / log((double)2) - 2);
	// 위의 식의 경우, 영상을 몇개의 옥타브로 나눌 것인지를 계산하는 부분 입니다. OCT만큼 옥타브가 형성되는 것입니다.
	// 하지만 왜 위와 같은 식을 갖게되었는지는 알 수 없었습니다.
    

    PGM **L=new PGM*[OCT];      // 형성된 옥타브 만큼 L에게 공간을 할당해줍니다.            
    for(int o=0;o<OCT;o++) L[o]=new PGM[S+3]; //  즉, 이미지를 만들어줍니다. 해당 부분의 경우, DOG를 실행하기 이전 부분입니다.향후 사용을 위해 할당.
    
    PGM **DoG=new PGM*[OCT];            // 형성된 옥타브 만큼 DOG에게 공간을 할당해줍니다.
    for(int o=0;o<OCT;o++) DoG[o]=new PGM[S+2]; // 향후 실행할 DOG를 위한 선언 부분
   
	start=clock();//필요 없는 시간 재는 부분

	build_image_pyramid(init_image,L,DoG); // 이미지 피라미드를 실행해줍니다.
    
	end=clock();//필요 없는 시간 재는 부분

	
	t.AppendFormat(_T("%f [sec],"),(end-start)/(double)CLOCKS_PER_SEC); //1//필요 없는 시간 재는 부분
	
	start=clock();//필요 없는 시간 재는 부분

    scale_space_extrema(DoG,keys); // scale space , 극한 값을 계산하는 호출 부분입니다.
    
	
	end=clock();//필요 없는 시간 재는 부분
	
	t.AppendFormat(_T("%f [sec],"),(end-start)/(double)CLOCKS_PER_SEC); //2//필요 없는 시간 재는 부분
	
	start=clock();//필요 없는 시간 재는 부분


    calc_orientation(L,keys); // 방향성을 결정해주는 함수 호출
    
	
	end=clock();//필요 없는 시간 재는 부분
	
	t.AppendFormat(_T("%f [sec],"),(end-start)/(double)CLOCKS_PER_SEC); //3//필요 없는 시간 재는 부분
	
	start=clock();//필요 없는 시간 재는 부분


	//////////////////////////// 요놈이, 간간히 0.3초씩 걸렸음! 나쁜넘!!
    compute_descriptors(L,keys); // 설정자를  기준값과 비교해주는 부분에 대한 호출 입니다.

    
	
	end=clock();//필요 없는 시간 재는 부분
	
	t.AppendFormat(_T("%f [sec],"),(end-start)/(double)CLOCKS_PER_SEC); //4//필요 없는 시간 재는 부분
	
	start=clock();//필요 없는 시간 재는 부분


    for(int o=0;o<OCT;o++){ // 위에서 할당해준 L과 DOG의 공간을 다시 지워주는 부분. 
        delete[] L[o];
        delete[] DoG[o];
    }
    delete[] L;
    delete[] DoG;
    
    for(list<SIFT_DESCRIPTOR>::iterator it=keys.begin();it!=keys.end();it++){
        delete (*it).ddata;
        (*it).ddata=NULL;
    }
    
	
	end=clock();//필요 없는 시간 재는 부분
	
	t.AppendFormat(_T("%f [sec]"),(end-start)/(double)CLOCKS_PER_SEC); //5//필요 없는 시간 재는 부분
	
	start=clock();//필요 없는 시간 재는 부분

}
void SIFT(PGM &_src,list<SIFT_DESCRIPTOR> &keys)/// 해당 부분은 바로위의 SIFT 함수와 동일한 구조를 이루고있는 함수 입니다.
{
    PGM init_image=create_init_image(_src);
    
    OCT=(int)(log((double)min(init_image.width(),init_image.height())) / log((double)2) - 2);
    

    PGM **L=new PGM*[OCT];                  
    for(int o=0;o<OCT;o++) L[o]=new PGM[S+3];
    
    PGM **DoG=new PGM*[OCT];            
    for(int o=0;o<OCT;o++) DoG[o]=new PGM[S+2];
   

	//////////////////////////// 요놈이, 2.0초씩 걸렸음! 나쁜넘!!
	build_image_pyramid(init_image,L,DoG);
    
    scale_space_extrema(DoG,keys);
    

    calc_orientation(L,keys);
    

	//////////////////////////// 요놈이, 간간히 0.3초씩 걸렸음! 나쁜넘!!
    compute_descriptors(L,keys);


    for(int o=0;o<OCT;o++){
        delete[] L[o];
        delete[] DoG[o];
    }
    delete[] L;
    delete[] DoG;
    
    for(list<SIFT_DESCRIPTOR>::iterator it=keys.begin();it!=keys.end();it++){
        delete (*it).ddata;
        (*it).ddata=NULL;
    }
    
}

PGM create_init_image(PGM &src) //이미지 참조하는 부분.
{
    if(IMG_DOUBLED){ // src를 참조하여, 이미지가 존재하지 않을 경우

        cout<<"not defined sequence!!!!!!!!!!!!"<<endl;       
        return src;
	}
    else{
        
		PGM tmp(src);
        double sig_diff=sqrt(SIGMA0*SIGMA0 - SIGMAi*SIGMAi);
        tmp=gaussian_filter(tmp,sig_diff);
        
        return tmp;
    }
}
void build_image_pyramid(PGM &base,PGM **L,PGM **DoG)
{

    double k=pow(2.0,1.0/S); // pow함수는 지수승 역할을 합니다.
	// 해당 식의 경우, 논문 p.7 에 명시되어있는 연산수식이며, 효율적인 연산, 오차의 최소화를 위해 위와 같은 k의 값을 계산합니다.
    
    double sig[S+3]; // S+3 만큼의 배열을 할당해주는 이유는 p.7에 명시 되어있는데, 각 옥타브에 S+3의 이미지를 생성해주어야만 최종 극한값 발견, 탐지(extrema detection)가 전체 옥타브를
	// 처리할 수 있기 때문입니다.
    
    sig[0]=SIGMA0; // SIGMAO 의 경우, 루트2 즉, 1.6의 값으로 선언해주었습니다.
    for(int s=1;s<S+3;s++){
        double sig_prev =pow(k,s-1)*SIGMA0;
        double sig_total=sig_prev*k;
        
        sig[s]=sqrt(sig_total*sig_total - sig_prev*sig_prev);
    }

    for(int o=0;o<OCT;o++){ // 옥타브만큼 for문 진행
        for(int s=0;s<S+3;s++){ // 각 옥타브의 scale만큼 for문을 진행.
            if(o==0 && s==0) L[o][s]=base; // octave가 0일 때, 그리고 scale이 0일 경우 해당 이미지는 원본임을 나타냅니다.
            else if(s==0)    L[o][s]=downsample(L[o-1][S]); // octave가 1,2 등 0이 아닌 임의의 값이며, scale의 0일 경우, downsample 을 진행한다는 뜻입니다.
			                                                // 즉, 각 octave의 레벨0 (최초 또는 처음에 위치한 이미지)임을 뜻합니다.
														    // 그러므로, 이전 옥타브의 레벨0 scale의 값을 다운 샘플링하여, 다음 옥타브의 레벨0을 만들어주는 것입니다.
            else             L[o][s]=gaussian_filter(L[o][s-1],sig[s]); // 이전의 scale을 참조하여 가우시안 필터(스무딩, 블러링)를 진행합니다. 즉, 2차원 배열에 이미지 피라미드를 
			                                                            // 형성합니다.
        }
    }

    for(int o=0;o<OCT;o++){  
        for(int i=0;i<S+2;i++){
            //DoG[o][i]=L[o][i+1]-L[o][i];
            sub_pgm(DoG[o][i],L[o][i+1],L[o][i]); // 같은 옥타브의 다른 스케일 두개를 빼주는 것입니다. 그 값을 DOG 배열에 저장하는 것입니다. 
												  // DOG[o][i] = L[o][i+1] - L[o][i]
        }
    }
}

bool is_extremum(PGM **DoG,int o,int s,int u,int v) // 극값 추출할 때, 예외처리 부분인 것으로 추정.
{
    double val=DoG[o][s][u][v]; // o는 octave , s는 scale .. u, v의 경우 너비, 높이 등으로 추정된다.
								// 
    
    if(val>0){ // 
        for(int ds=s-1;ds<=s+1;ds++) // 앞뒤 scale에서 존재하는 값을 비교하기 위한 for문
            for(int du=u-1;du<=u+1;du++) // 
                for(int dv=v-1;dv<=v+1;dv++) // 기준점을  9개 값을 비교,
                    if(val<DoG[o][ds][du][dv]) return false;
    }
    else{ 
        for(int ds=s-1;ds<=s+1;ds++) //
            for(int du=u-1;du<=u+1;du++) //
                for(int dv=v-1;dv<=v+1;dv++) //
                    if(val>DoG[o][ds][du][dv]) return false;    
    }
    
    return true;
}
SIFT_DESCRIPTOR* interp_extremum(PGM **DoG,int o,int s,int u,int v)// 극한 값 
{
    double dx[3];
    double dD[3];
    double Hm[3][3],Hi[3][3];
    
    int W=DoG[o][s].width();
    int H=DoG[o][s].height();
    
    int cnt=0;
    while(cnt<5){

        dD[0]=(DoG[o][s][u+1][v]-DoG[o][s][u-1][v])/2.0;   //Dx
        dD[1]=(DoG[o][s][u][v+1]-DoG[o][s][u][v-1])/2.0;   //Dy
        dD[2]=(DoG[o][s+1][u][v]-DoG[o][s-1][u][v])/2.0;   //Ds
        
        double val=DoG[o][s][u][v];
        

        double Dxx=(DoG[o][s][u+1][v]+DoG[o][s][u-1][v]-2*val);
        double Dyy=(DoG[o][s][u][v+1]+DoG[o][s][u][v-1]-2*val);
        double Dss=(DoG[o][s+1][u][v]+DoG[o][s-1][u][v]-2*val);
        double Dxy=(DoG[o][s][u+1][v+1]-DoG[o][s][u-1][v+1]
                    -DoG[o][s][u+1][v-1]+DoG[o][s][u-1][v-1])/4.0;
        double Dxs=(DoG[o][s+1][u+1][v]-DoG[o][s+1][u-1][v] 
                    -DoG[o][s-1][u+1][v]+DoG[o][s-1][u-1][v])/4.0;
        double Dys=(DoG[o][s+1][u][v+1]-DoG[o][s+1][u][v-1]
                    -DoG[o][s-1][u][v+1]+DoG[o][s-1][u][v-1])/4.0;
        
        Hm[0][0]=Dxx; Hm[0][1]=Dxy; Hm[0][2]=Dxs;
        Hm[1][0]=Dxy; Hm[1][1]=Dyy; Hm[1][2]=Dys;
        Hm[2][0]=Dxs; Hm[2][1]=Dys; Hm[2][2]=Dss;
        

        inverse(Hm,Hi);
        
        dx[0]=-(Hi[0][0]*dD[0]+Hi[0][1]*dD[1]+Hi[0][2]*dD[2]);
        dx[1]=-(Hi[1][0]*dD[0]+Hi[1][1]*dD[1]+Hi[1][2]*dD[2]);
        dx[2]=-(Hi[2][0]*dD[0]+Hi[2][1]*dD[1]+Hi[2][2]*dD[2]);
        
        if(ABS(dx[0])<0.5 && ABS(dx[1])<0.5 && ABS(dx[2])<0.5) break;
        
        u+=(int)round(dx[0]);
        v+=(int)round(dx[1]);
        s+=(int)round(dx[2]);
        
        if(s<1 || s>S || u<1 || v<1 || u>=W-1 || v>=H-1) return NULL;
        
        cnt++;
    }
    if(cnt>=5) return NULL;

    double Dpow=DoG[o][s][u][v]+0.5*(dD[0]*dx[0]+dD[1]*dx[1]+dD[2]*dx[2]);
    if(ABS(Dpow)<CONTR_THR/S) return NULL;
    
    double trc=Hm[0][0]+Hm[1][1];
    double det=Hm[0][0]*Hm[1][1]-Hm[0][1]*Hm[1][0];
    if(det<=0 || trc*trc/det>=EDGE_THR) return NULL;
    
    SIFT_DESCRIPTOR *feat=new SIFT_DESCRIPTOR;
    feat->ddata=new DETECTION_DATA;
    feat->ddata->u =u;
    feat->ddata->v =v;
    feat->ddata->o =o;
    feat->ddata->s =s;
    feat->ddata->ds=dx[2];
    feat->ddata->scl_octv=SIGMA0*pow(2.0,(s+dx[2])/S);
    
    feat->x  =(u+dx[0])*pow(2.0,o);
    feat->y  =(v+dx[1])*pow(2.0,o);
    feat->scl=SIGMA0*pow(2.0,o+(s+dx[2])/S);
    
    if(IMG_DOUBLED){
        feat->x  /=2.0;
        feat->y  /=2.0;
        feat->scl/=2.0;
    }
    
    return feat;
}
void scale_space_extrema(PGM **DoG,list<SIFT_DESCRIPTOR> &keys)
{
    double pre_thr=0.5*CONTR_THR/S;
    
    for(int o=0;o<OCT;o++){
        int W=DoG[o][0].width();
        int H=DoG[o][0].height();
        
        for(int s=1;s<S+1;s++){
            for(int u=1;u<W-1;u++){
                for(int v=1;v<H-1;v++){
                    if( ABS(DoG[o][s][u][v]) < pre_thr ) continue;
                    
                    if(is_extremum(DoG,o,s,u,v)){
                        SIFT_DESCRIPTOR *feat=interp_extremum(DoG,o,s,u,v);
                        
                        if(feat != NULL){
                            keys.push_back(*feat);
                        }
                    }
                }
            }
        }
    }
}


void ori_hist(PGM &L,DETECTION_DATA *dat,double hst[ORI_HIST_BINS])
{
    int    RAD=(int)round(ORI_RADIUS*dat->scl_octv);
    double sig=ORI_SIG_FCTR*dat->scl_octv;
    
    for(int i=0;i<ORI_HIST_BINS;i++) hst[i]=0;
    
    int W=L.width();
    int H=L.height();
    
    sig=2.0*sig*sig;
    for(int i=-RAD;i<=RAD;i++){
        int px=i+dat->u;
        if(px<=0 || px>=W-1) continue;
        
        for(int j=-RAD;j<=RAD;j++){
            int py=j+dat->v;
            if(py<=0 || py>=H-1) continue;

            double dx =L[px+1][py]-L[px-1][py];
            double dy =L[px][py+1]-L[px][py-1];
            double pow=sqrt(dx*dx+dy*dy);
            double arg=(atan2(dy,dx)+M_PI)/(2*M_PI);
            
            double w  =exp(-(i*i+j*j)/sig);
            int    bin=(int)round(ORI_HIST_BINS*arg);
            
            bin=(bin<ORI_HIST_BINS)?bin:0;
            
            hst[bin]+=w*pow;
        }
    }
}
void calc_orientation(PGM **L,list<SIFT_DESCRIPTOR> &keys)
{
    int    N=keys.size();
    double hst[ORI_HIST_BINS];
    
    for(int i=0;i<N;i++){
        SIFT_DESCRIPTOR *feat=&(keys.front());
        
        ori_hist(L[feat->ddata->o][feat->ddata->s],feat->ddata,hst);
        
        for(int j=0;j<2;j++){
            double prv=hst[ORI_HIST_BINS-1],tmp;
            double h0 =hst[0];
            for(int bin=0;bin<ORI_HIST_BINS;bin++){
            	tmp=hst[bin];
            	hst[bin]=0.25*prv+0.5*hst[bin]+0.25*hst[(bin+1)%ORI_HIST_BINS];
            	prv=tmp;
            }
        }
        
        double max_h=*max_element(hst,hst+ORI_HIST_BINS);
       
        max_h*=0.8;
        
        for(int bin=0;bin<ORI_HIST_BINS;bin++){
            int l=(bin==0) ? ORI_HIST_BINS-1:bin-1;
            int r=(bin+1)%ORI_HIST_BINS;
            
            if(hst[bin]>hst[l] && hst[bin]>hst[r]&& hst[bin]>=max_h){
                double ori=bin + 0.5*(hst[l]-hst[r])/(hst[l]-2.0*hst[bin]+hst[r]);
                ori=(ori<0)?ORI_HIST_BINS+ori:(ori>=ORI_HIST_BINS)?ori-ORI_HIST_BINS:ori;
                
                SIFT_DESCRIPTOR new_feat(*feat);
                new_feat.ddata=new DETECTION_DATA(*(feat->ddata));
                new_feat.arg  =2*M_PI*ori/ORI_HIST_BINS-M_PI;
                
                keys.push_back(new_feat);
            }
        }
        
        delete feat->ddata;
        keys.pop_front();
    }
}
inline void interpolate_hist(double hst[DSCR_WIDTH][DSCR_WIDTH][DSCR_HIST_BINS],double pow,double ubin,double vbin,double obin)
{
    int u0=(int)floor(ubin);
    int v0=(int)floor(vbin);
    int o0=(int)floor(obin);
    
    double du=ubin-u0;
    double dv=vbin-v0;
    double d0=obin-o0;
    
    for(int u=0;u<=1;u++){
        int pu=u0+u;
        if(pu<0 || pu>=DSCR_WIDTH) continue;
        
        double pow_u=pow*( (u==0) ? 1.0-du:du );
        for(int v=0;v<=1;v++){
            int pv=v0+v;
            if(pv<0 || pv>=DSCR_WIDTH) continue;
            
            double pow_v=pow_u*( (v==0) ? 1.0-dv:dv );
            for(int o=0;o<=1;o++){
                int po=(o0+o)%DSCR_HIST_BINS;
                
                double pow_o=pow_v*( (o==0) ? 1.0-d0:d0 );
                hst[pu][pv][po]+=pow_o;
            }
        }
    }
}
void descr_hist(PGM &L,SIFT_DESCRIPTOR &feat,double hst[DSCR_WIDTH][DSCR_WIDTH][DSCR_HIST_BINS])
{
    for(int i=0;i<DSCR_WIDTH;i++){
        for(int j=0;j<DSCR_WIDTH;j++){
            for(int k=0;k<DSCR_HIST_BINS;k++) hst[i][j][k]=0;
        }
    }
    
    double arg0 =feat.arg;
    double cos_t=cos(arg0);
    double sin_t=sin(arg0);
    double scl  =feat.ddata->scl_octv;
    double denom=-1.0/(DSCR_WIDTH*DSCR_WIDTH*0.5);
    
    double bins_per_rad=DSCR_HIST_BINS/(2*M_PI);
    
    double hst_w=3.0*scl;
    int    hst_r=(int)(hst_w*sqrt((double)2)*(DSCR_WIDTH+1.0)*0.5+0.5);

    int W=L.width();
    int H=L.height();
    
    for(int i=-hst_r;i<=hst_r;i++){
        int px=i+feat.ddata->u;
        if(px<=0 || px>=W-1) continue;
        
        for(int j=-hst_r;j<=hst_r;j++){
            int py=j+feat.ddata->v;
            if(py<=0 || py>=H-1) continue;
            
            double u_rot=( i*cos_t+j*sin_t)/hst_w;
            double v_rot=(-i*sin_t+j*cos_t)/hst_w;
            double u_bin=u_rot+DSCR_WIDTH/2.0-0.5;
            double v_bin=v_rot+DSCR_WIDTH/2.0-0.5;
            
            if(u_bin>-1.0 && u_bin<DSCR_WIDTH && v_bin>-1.0 && v_bin<DSCR_WIDTH){
                double dx =L[px+1][py]-L[px-1][py];
                double dy =L[px][py+1]-L[px][py-1];
                double pow=sqrt(dx*dx+dy*dy);
                double arg=atan2(dy,dx);
                
                arg-=arg0;
                while(arg<0.0)    arg+=2*M_PI;
                while(arg>=2*M_PI)arg-=2*M_PI;
                
                double o_bin=arg*bins_per_rad;
                double w=exp(denom*(u_rot*u_rot+v_rot*v_rot));
                
                interpolate_hist(hst,w*pow,u_bin,v_bin,o_bin);
            }
            
        }
    }
    
}
void normalize_descr(SIFT_DESCRIPTOR &feat)   // 설정자의 정리
{
    double sum=0;
    for(int i=0;i<DSCR_LENGTH;i++) sum+=feat.v[i]*feat.v[i];
    
    if(sum!=0){
        sum=1.0/sqrt(sum);
        for(int i=0;i<DSCR_LENGTH;i++) feat.v[i]*=sum;
    }
}
void hist_to_descr(SIFT_DESCRIPTOR &feat,double hst[DSCR_WIDTH][DSCR_WIDTH][DSCR_HIST_BINS])
{
    int cnt=0;
    for(int i=0;i<DSCR_WIDTH;i++){
        for(int j=0;j<DSCR_WIDTH;j++){
            for(int k=0;k<DSCR_HIST_BINS;k++) feat.v[cnt++]=hst[i][j][k];
        }
    }
    
    normalize_descr(feat);
    for(int i=0;i<DSCR_LENGTH;i++){
        if(feat.v[i]>DESCR_MAG_THR) feat.v[i]=DESCR_MAG_THR;
    }
    normalize_descr(feat);

    for(int i=0;i<DSCR_LENGTH;i++) feat.v[i]=min(255,(int)(DESCR_INT_FCTR*feat.v[i]));
}
void compute_descriptors(PGM **L,list<SIFT_DESCRIPTOR> &keys)
{
    double hst[DSCR_WIDTH][DSCR_WIDTH][DSCR_HIST_BINS];
    
    for(list<SIFT_DESCRIPTOR>::iterator it=keys.begin();it!=keys.end();it++){
        descr_hist(L[(*it).ddata->o][(*it).ddata->s],(*it),hst);
        
        hist_to_descr((*it),hst);
    }
}
PGM gaussian_filter(PGM &src,double sig)
{
    int W=src.width();
    int H=src.height();
    
    PGM tmp(W,H);
    PGM dst(W,H);
    
    int Wm=(int)round((3.0*sig)*2+1)|1;  
    

    double *msk=new double[Wm+1];
    
    double sum=-1.0;
    sig=-0.5/(sig*sig);

	msk[0]=1;

    
	for(int i=0;i<=Wm/2;i++){
        double t=exp(sig*i*i);
        
        msk[(Wm/2+i)]=t;
        sum+=msk[(Wm/2+i)]*2;
    }
    
    sum = 1.0/sum;
    for(int i=0;i<=Wm/2;i++){
        msk[(Wm/2+i)]=msk[(Wm/2-i)]=msk[(Wm/2+i)]*sum;
    }
    
    for(int x=0;x<W;x++){
        for(int y=0;y<H;y++){
            sum=0;
            for(int i=0;i<Wm;i++){
                sum+=msk[i]*src(x,y+i-Wm/2);
            }
            tmp[x][y]=sum;
        }
    }

	
    for(int x=0;x<W;x++){
        for(int y=0;y<H;y++){
            sum=0;
            for(int i=0;i<Wm;i++){
                sum+=msk[i]*tmp(x+i-Wm/2,y);
            }
            dst[x][y]=sum;
        }
    }
	
    delete [] msk;
    
    return dst;
}
PGM downsample(PGM &src) // 다운 샘플링
{
    int W=src.width() /2; // W는 입력받은 이미지 src의 반으로 나눈다. 두번쨰 octave의 경우 첫번째 octave의 반으로 저장한다. (너비)
    int H=src.height()/2; // H는 입력받은 이미지 src의 반으로 나눈다. 두번쨰 octave의 경우 첫번째 octave의 반으로 저장한다. (높이)
    PGM tmp(W,H); // 정리한( 너비 높이) 값을 저장한다.
    
    for(int x=0;x<W;x++){ // 줄어든 너비 만큼 for문
        for(int y=0;y<H;y++){// 줄어든 높이만큼 for문
            tmp[x][y]=src[2*x][2*y]; // 즉, 이전 octave의 2,2에 있는 정보를 다음 octave의 1,1 에 넣어주며, 
			                         // 이전 octave의 2,4 에 있는 정보를 다음 octave의 1,2에 넣어준다.
			                         // 이전 octave의 2,6에 있는 정보를 다음 octave의 1,3에 넣어준다.
        }
    }
    
    return tmp;  // 정리된 tmp , 이미지 정보 반환
}
void sub_pgm(PGM &dst,PGM &a,PGM &b)
{
    int W=min(a.width() ,b.width()); 
    int H=min(a.height(),b.height());
    
    dst.reset(W,H);
    for(int x=0;x<W;x++){
        for(int y=0;y<H;y++){
            dst[x][y]=a[x][y]-b[x][y];
        }
    }
}

void inverse(double mat[3][3],double inv[3][3])
{
    double buf;
    
    for(int i=0;i<3;i++){
        for(int j=0;j<3;j++) inv[i][j]=0;
        inv[i][i]=1;
    }

    for(int i=0;i<3;i++){
        buf=1/mat[i][i];
        for(int j=0;j<3;j++){
            mat[i][j]*=buf;
            inv[i][j]*=buf;
        }
        
        for(int j=0;j<3;j++){
            if(i!=j){
                buf=mat[j][i];
                for(int k=0;k<3;k++){
                    mat[j][k]-=mat[i][k]*buf;
                    inv[j][k]-=inv[i][k]*buf;
                }
            }
        }
    }
}
