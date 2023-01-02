#include <stdio.h>
#include <stdlib.h>
#include <math.h>

/// for Eric :D

void simultaneous_equations_level_1_question(int question_index)
{
    /// v0y+v1x+v2=0, v3y+v4x+v5=0
    int v[6],i,r;

    srand(question_index);

    for(i=0;i<6;i++)
    {
        r=1+rand()%18;
        r-=(r>9)*19;
        v[i]=r;
    }

    printf("%dy %s %dx %s %d = 0 , %dy %s %dx %s %d = 0\n solve for x & y\n",
           v[0],v[1]<0?"-":"+",abs(v[1]),v[2]<0?"-":"+",abs(v[2]),
           v[3],v[4]<0?"-":"+",abs(v[4]),v[5]<0?"-":"+",abs(v[5]));

}

int simultaneous_equations_level_1_answer(int question_index,int print_result)
{
    /// v0y+v1x+v2=0, v3y+v4x+v5=0
    srand(question_index);
    int v[6],i,r,yn,yd,xn,xd,p[4]={2,3,5,7};
    double x,y,r1,r2;

    srand(question_index);

    for(i=0;i<6;i++)
    {
        r=1+rand()%18;
        r-=(r>9)*19;
        v[i]=r;
    }

    xn=v[3]*v[2]-v[5]*v[0];
    xd=v[4]*v[0]-v[3]*v[1];

    if(xn)for(i=0;i<4;i++)
    {
        while(!(xn%p[i]) && !(xd%p[i]))
        {
            xn/=p[i];
            xd/=p[i];
        }
    }
    if(xd<0)
    {
        xd*=-1;
        xn*=-1;
    }

    yn=-v[1]*xn-v[2]*xd;
    yd=v[0]*xd;

    if(yn)for(i=0;i<4;i++)
    {
        while(!(yn%p[i]) && !(yd%p[i]))
        {
            yn/=p[i];
            yd/=p[i];
        }
    }
    if(yd<0)
    {
        yd*=-1;
        yn*=-1;
    }



    x=((double)xn)/((double)xd);
    y=((double)yn)/((double)yd);

    r1=((double)v[0])*y+((double)v[1])*x+((double)v[2]);
    r2=((double)v[3])*y+((double)v[4])*x+((double)v[5]);

    if(fabsf(r1)<0.000000001 && fabsf(r2)<0.000000001)
    {
        if(print_result)
        {
            if(!xn)puts("x = 0");
            else if(xd==1)printf("x = %d\n",xn);
            else printf("x = %d/%d\n",xn,xd);
            if(!yn)puts("y = 0");
            else if(yd==1)printf("y = %d\n",yn);
            else printf("y = %d/%d\n",yn,yd);
        }
        return 1;
    }
    else if(xd==0)
    {
        if(print_result)puts("Psych! there is no solution! I got you! :D");
        return 0;
    }
    else
    {
        printf("%dy %s %dx %s %d = 0 , %dy %s %dx %s %d = 0\n",
           v[0],v[1]<0?"-":"+",abs(v[1]),v[2]<0?"-":"+",abs(v[2]),
           v[3],v[4]<0?"-":"+",abs(v[4]),v[5]<0?"-":"+",abs(v[5]));
        printf("%f %f, %d/%d, %d/%d, %f %f\n",x,y,xn,xd,yn,yd, r1,r2);
        puts("oh fuck, oh fuck, I couldn't figure it out! please report the above lines!");
        return 0;
    }
}
