#include <stdio.h>

/// note: treats coin at index 0 as "coin 1" (i.e. coin names are 1 indexed not 0 indexed)
void thirteen_coins(int * w)//weights
{
    int r1,r2,r3;///results of different weighings
    int c,d;/// the suspect coin (c) and whether its heavier, lighter or unknown (d)

    r1=(w[5]+w[6]+w[7]+w[8]) - (w[9]+w[10]+w[11]+w[12]);

    if(r1==0)
    {
        ///5-13 are now "control" coins
        r2=(w[1]+w[2]+w[3]) - (w[10]+w[11]+w[12]);

        if(r2==0)/// is either 0 or 4
        {
            r3=w[4] - w[12];
            if(r3==0)d=0,c=0;///  D:  no way to tell if its heavier b/c didn't get to weigh it
            else d=r3,c=4;
        }
        else
        {
            r3=w[1] - w[2];
            if(r3==0)d=r2,c=3;
            else if(r3==r2)d=r2,c=1;
            else d=r2,c=2;
        }
    }
    else
    {
        r2=(w[5]+w[6]+w[7] + w[9]+w[10]) - (w[0]+w[1]+w[2]+w[3]+w[4]);///compare subset against "control" coins

        if(r2==0)///not among coins in r2; 8,11,12
        {
            r3=w[11]-w[12];

            if(r3==0) d=r1,c=8;
            else if(r3==r1)d=-r1,c=12;
            else d=-r1,c=11;
        }
        else if(r2==r1)///result the same; 5,6,7
        {
            r3=w[5]-w[6];

            if(r3==0)d=r1,c=7;
            else if(r3==r1)d=r1,c=5;
            else d=r1,c=6;
        }
        else /// result swapped; 9,10
        {
            r3=w[9]-w[10];
            if(r3==r1)d=-r1,c=10;
            else if(r3)d=-r1,c=9;
            else d=0,c=-1;
        }
    }

    char* delta_descriptions[3]={"it was lighter","I'm unsure whether it was heavier or lighter","it was heavier"};

    printf("coin %d was counterfit and %s\n",c+1,delta_descriptions[d+1]);
}
