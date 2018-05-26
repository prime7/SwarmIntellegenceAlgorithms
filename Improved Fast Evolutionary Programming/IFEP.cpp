#include<bits/stdc++.h>
#include <time.h>
using namespace std;
FILE* out=freopen("Func1.txt","w",stdout);
double upper,lower;
int n,iteration,fncr;
#define population 100
#define individual 30

struct pop{
    	double x[individual];
		double eta[individual];
		double fit;
		int wins;
} parent[2*population],select[population];

int uniform(int a, int b){
	return rand() / (RAND_MAX + 1.0) * (b - a) + a;
}

double NormalRV(){
	static unsigned int counter=0;
	static double x1,x2;
	counter++;
	if(counter%2==0) return x2;
    start:
	double u1=rand()/(RAND_MAX*1.0);
	double u2=rand()/(RAND_MAX*1.0);
	double v1=2*u1-1;
	double v2=2*u2-1;
	double w=v1*v1 + v2*v2;
	if(w>1) goto start;
	double y=sqrt((-2*log(w))/w);
	x1=v1*y;
	x2=v2*y;
	return x1;
}

double CauchyRV(){
	double x = rand()/(double)RAND_MAX;
	double t = rand()/(double)RAND_MAX;
	double y;

	if((t = (1+x*x)*t) >= 1)
    for(;;)
    {
        x =  rand()/(double)RAND_MAX;
        y =  rand()/(double)RAND_MAX;
        t = (1+x*x)*y;
        if(t < 1)
            break;
	}

	if( (rand()/(double)RAND_MAX)>0.5)
        x=-x;

	if( (rand()/(double)RAND_MAX)>0.5)
	{
		while(fabs(x)<0.0001)
			x = rand()/(double)RAND_MAX;
		x= 1.0/x;
	}
	return x;

}

double fitness(int i,int f){
    double ans=0,mul=0;
    if(f==1)
    {
        for(int k=0;k<n;++k)
            ans = ans + parent[i].x[k]*parent[i].x[k];
    }
    if(f==2)
    {
        for(int k=0;k<n;++k)
		{
			ans= ans + fabs(parent[i].x[k] );
			mul= mul * fabs(parent[i].x[k] ) ;
		}
        ans=ans+mul;
    }
    if(f==3)
	{
		double ss;
		ss=0;
		for(int j=0;j<n;j++)
			ss = ss + (  (j+1) * parent[i].x[j]  ) * (  (j+1) * parent[i].x[j]  ) ;
		ans=ss;
	}

    if(f==4)
    {
        ans=fabs(parent[i].x[0]);

        for(int k=0;k<n;++k)
		{
			if(fabs(parent[i].x[k] )>=ans)
				ans=fabs(parent[i].x[k] );
        }


    }
    if(f==5)
    {
        //function 9
        double m = 0.0;
        for(int k=0;k<n;++k)
		{
		    m= parent[i].x[k];
			ans= ans + ( (m*m)  - 10 * cos(2*3.1416 *3.1416*  m /180) +10 ) ;
        }
    }
    else if(f==6)
    {
        //function 10
        double m ,t1,t2;
        for(int k=0;k<n;++k)
		{
            m= parent[i].x[k];
            ans= ans + (m*m)  ;
		}
		t1=ans/30;
		ans=0.0;
		for(int k=0;k<n;++k)
		{
			m= parent[i].x[k];
			ans= ans + cos(2*3.1416 *3.1416*  m /180) ;
		}
		t2=ans/30;
		ans= ( (-20) * exp ((-0.2) * sqrt( t1 ) ) ) - exp ( t2 ) +20 + exp(1.0);

    }
    return ans;
}

void parameter(int i){
    if(i==1)
    {
        n=30;
        iteration = 1500;
        upper = -100.0;
        lower = 100.0;
    }
    else if(i==2)
    {
        n=30;
        iteration = 2000;
        upper = -10.0;
        lower = 10.0;
    }
    else if(i==3)
    {
        n=30;
        iteration= 5000;
        upper= -10.0;
        lower= 10.0;

    }
    else if(i==4)
    {
        n=30;
        iteration= 5000;
        upper= -100.0;
        lower= 100.0;

    }
    else if(i==5)
    {
        //function 9
        n=30;
        iteration= 5000;
        upper= -5.12;
        lower= 5.12;

    }
    else if(i==6)
    {
        //function 10
        n=30;
        iteration= 1500;
        upper= -32;
        lower= 32;

    }
}

double uniform_deviate ( int seed ){
	return seed * ( 1.0 / ( RAND_MAX + 1.0 ) );
}

void randinput()
{
    double lowest=upper, highest=lower;
    double range =(highest-lowest);
	for(int i=0;i<population;++i)
    {
		for(int k=0; k<n; k++)
		{
			parent[i].x[k]  = lowest+ uniform_deviate ( rand() ) *range  ;
	  	    parent[i].eta[k]= 3.0;
        }
        parent[i].fit= fitness(i,fncr);
	}
}
double clippedDomain(double x)
{

	if (x>lower)
        x = lower;

	if (x<upper)
        x = upper;
	return x;

}
int main()
{
    srand(time(NULL));
    parameter(1);
    randinput();
    for(int func = 1;func<=6;func++)
    {
        srand(time(0));
        printf("***Function %d***\n",func);
        fncr = func;
        int f = func;
        parameter(f);
        randinput();
        int l = 0;
        while(l<iteration)
        {
            double p=NormalRV();
            double t=n;
            double ta= 1.0 / sqrt( 2* sqrt(t) ) ;
            double tap= 1.0 / sqrt(2*t) ;

            for(int i = population ; i<2*population;++i)
            {
                for(int k =0;k<n;++k)
                {
                    parent[i].x[k]  =  parent[i-population].x[k] + ( parent[i-population].eta[k]* CauchyRV() );
                    parent[i].eta[k] =	parent[i-population].eta[k] * exp(    ( tap * p ) +  ( ta  * NormalRV() ));
                    parent[i].x[k] = clippedDomain(parent[i].x[k]);
                }
                parent[i].fit = fitness(i,fncr);

                int fit_temp=parent[i].fit;
                for(int k=0;k<n;++k)
                {
                    parent[i].x[k]  =  parent[i-population].x[k] + ( parent[i-population].eta[k]* NormalRV() );
                    parent[i].eta[k] =	parent[i-population].eta[k] * exp(    ( tap * p ) +  ( ta  * NormalRV() )    );
                    parent[i].x[k] = clippedDomain(parent[i].x[k]);
                }
                parent[i].fit = fitness(i,fncr);


                if(parent[i].fit<fit_temp)
                {
                    for(int k=0;k<n;++k)
                    {
                        parent[i].x[k]  =  parent[i-population].x[k] + ( parent[i-population].eta[k]* NormalRV() );
                        parent[i].eta[k] =	parent[i-population].eta[k] * exp(    ( tap * p ) +  ( ta  * NormalRV() )    );
                        parent[i].x[k] = clippedDomain(parent[i].x[k]);
                    }
                    parent[i].fit = fitness(i,fncr);
                }
                else
                {

                    for(int k=0;k<n;++k)
                    {
                        parent[i].x[k]  =  parent[i-population].x[k] + ( parent[i-population].eta[k]* CauchyRV() );
                        parent[i].eta[k] =	parent[i-population].eta[k] * exp(    ( tap * p ) +  ( ta  * NormalRV() )    );
                        parent[i].x[k] = clippedDomain(parent[i].x[k]);
                    }
                    parent[i].fit = fitness(i,fncr);
                }
            }
            int oppa[10];
            for(int i=0;i<2*population;++i)
            {
                parent[i].wins=0;
                for(int k=0;k<10;++k)
                {
                    oppa[k]=uniform(0,(2*population)-1);
                    if(parent[i].fit<=parent[oppa[k]].fit)
                        parent[i].wins=	parent[i].wins+1;
                }
            }
            struct pop temp;
            int x,y;
            for(int i=0;i<2*population-1;++i)
            {
                for(int k=i+1;k<2*population;++k)
                {
                    x=parent[i].wins ;
                    y=parent[k].wins ;
                    if(x<y )
                    {
                        temp=parent[i];
                        parent[i]=parent[k];
                        parent[k]=temp;
                    }
                }
            }
            printf("Fitness after %d iteration: %lf\n",l,parent[0].fit);
            l++;
        }
        printf("\nFor Function %d MIN Value : %lf\n",func,parent[0].fit);
    }
}
