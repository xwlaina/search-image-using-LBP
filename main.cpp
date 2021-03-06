
#include <time.h>
#include <iostream>
#include <cv.h>
#include <fstream>
#include <highgui.h>
#include <dirent.h>// this lib must be loaded 
#include "getblock.h"
#include "dolbp.h"
#include "gethist.h"
#include "comp.h"
#include "getpoints.h"
#include <omp.h>

#define nbpoint 20
using namespace std;
using namespace cv;
void indexm(String a[],int **matmax,int t);
void indexq(IplImage *q,int *a);
void trier(double ** a,int s);
int main(){
	time_t tim1=time(NULL);
	ofstream myfil;
	string all[200];int t=-2;
	
	myfil.open("D:\\paths.txt") ;
	char *cline=new char();
	String imgName;
	DIR *dpdf;
struct dirent *epdf;
dpdf = opendir("D:\\bas");
if (dpdf != NULL){
   while (epdf = readdir(dpdf)){
	  t++;
	   imgName= epdf->d_name;
	   if(imgName !="." && imgName !=".." && imgName !="Thumbs.db" ){
	   		   myfil<<"D:\\bas\\"<<imgName<<endl;
			   if (t>-1) all[t]="D:\\bas\\"+imgName;
	   }
   }
 closedir (dpdf);
    } else {
        cout<<"not present"<<endl;
    }
myfil.close();

cout <<"this is"<<all[1]<<endl;
cout <<"TTT"<<t<<endl;

//cout <<"o num of imgs "<<o<<endl;
//////////////////////////////////////////////////////
/*

 ifstream file( "D:\\paths.txt" );
 
 if (file){
  while (! file.eof() ){
 getline( file, line );
 if(line!=""){
	 t++;
 }else cout<<" "<<endl;
    }
 }else cout<<"le fichier n'existe pas"<<endl;
 cout <<"num imgs"<<t<<endl;
 file.close();
 
 /////////////////////////////////*/
cout <<"num imgs "<<t<<endl;
 int nbg=256*nbpoint;
 String line;
 int *matq=new int [nbg];
 int **matmax =new int * [t];
 for (int j=0;j<t;j++){
	 matmax [j]=new int [nbg];
     for (int l=0;l<nbg;l++) matmax[j][l]=0;
 }
 int* hist =new int [256];
	IplImage * bloc,* blocn;
	
  //////////////////////////////*/
	indexm(all,matmax,t);
	IplImage * im1=cvLoadImage ("D:\\bas\\obj2__0.png",0);
	indexq(im1,matq);
	///////////////////////////////////	
	double **tabmax=new double*[t];
 for(int i=0;i<t;i++)
	 tabmax[i]=new double [2];

 for(int i=0;i<t;i++){
	
tabmax[i][0]=comp(matmax[i],matq);
tabmax[i][1]=i;
 }
 trier(tabmax,t);
 /////////////////////////////////////////
	string l;IplImage *im;
 char * ln=new char();int u;

for(int i=0;i<10;i++){
	
	u=(int )tabmax[i][1];
			l=all[u+1];
			cout<<all[u+1]<<endl;
			/*strcpy(ln,l.c_str());
		    im=cvLoadImage(ln,1);
			cvShowImage("img",im);
		    cvWaitKey();*/
	

}

 time_t tim2=time(NULL);
 cout<<"time : "<<tim2-tim1<<endl;

	system ("pause");
	return 0;
	////////////////////////////////////

} 
void trier(double ** a,int s){
	double min ;double p;
	for(int i=0;i<s-1;i++)
		for(int j=i+1;j<s;j++)
		if (a[i][0]<a[j][0]){
			min=a[i][0];p=a[i][1];
	        a[i][0]=a[j][0];a[i][1]=a[j][1];
			a[j][0]=min;a[j][1]=p;
		}

	
}
void indexm(String a[],int **matmax,int t){
	int ngb=256*nbpoint;
	//String line;
IplImage* img;
	char * cline=new char();
	int* hist =new int [256];
	IplImage * bloc=cvCreateImage(cvSize(64,64),IPL_DEPTH_32F,1);
	IplImage * blocn=cvCreateImage(cvSize(64,64),IPL_DEPTH_32F,1);
	CvPoint2D32f *points=new CvPoint2D32f[nbpoint];
	int i,m;
#pragma omp parallel
	{
#pragma omp parallel for private(i,m)schedule(static)
	for (int p=1;p<t+1;p++){
		//line=all[p];
			strcpy(cline,a[p].c_str());//convertire String to char*
	 img =cvLoadImage (cline,0);
 getpoints(img,points);
	for ( i=0;i<nbpoint;i++){
	int b=(int)points[i].x;int c =(int)points[i].y;
	 if (b<img->height && b>-1 && c<img->width && c>-1){
		getblock(points[i],img,bloc);
	   dolbp (bloc,blocn);
		gethist(blocn,hist);
			for ( m=0;m<256;m++)
				matmax [p-1][i*256+m]=hist[m];
}
}

	}
	}
}
void indexq(IplImage * q,int * a){
	CvPoint2D32f *poin=new CvPoint2D32f[nbpoint];
	int* hist =new int [256];
	IplImage * bloc=cvCreateImage(cvSize(64,64),IPL_DEPTH_32F,1);
	IplImage * blocn=cvCreateImage(cvSize(64,64),IPL_DEPTH_32F,1);
	getpoints(q,poin);
	for (int i=0;i<nbpoint;i++){
	getblock(poin[i],q,bloc);
	dolbp(bloc,blocn); 
	gethist(blocn,hist);
	for (int j=0;j<256;j++)
		a[i*256+j]=hist[j];
	}


}
