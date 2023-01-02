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
        r2=(w[5]+w[6] + w[9]) - (w[7]+w[8]+w[0]);///compare shuffled subset against a "control" coin

        if(r2==0)///remanats 10,11,12
        {
            r3=w[10] - w[11];
            if(r3==0)d=-r1,c=12;
            else if(r3==r1)d=-r3,c=11;
            else d=r3,c=10;
        }
        else if(r2==r1)/// same 5 6
        {
            r3=w[5] - w[6];
            if(r3==r2)d=r3,c=5;
            else d=-r3,c=6;
        }
        else/// swapped 7 8 9
        {
            r3=w[7] - w[8];
            if(r3==0)d=r2,c=9;
            else if(r3==r2)d=-r3,c=8;
            else d=r3,c=7;
        }
    }

    char* delta_descriptions[3]={"it was lighter","I'm unsure whether it was heavier or lighter","it was heavier"};

    printf("coin %d was counterfeit and %s\n",c+1,delta_descriptions[(d>=0) + (d>0)]);
    /**
    with control coin 9
    0 1 2 3 4 - 5 6 7 8 9

    0 1 5 - 2 3 6
    same 0 1 6
    diff 5 2 3
    == 4 7 8

    no real use for this as won't have control coin w/o having additional information about this (unless this 9 is in an == branch)

    26 -
    r0 = 0 1 2 3 4 5 6 7 8 9 a b c - 10 11 12 13 14 15 16 17 18 19 d e f

        r1 = 0 1 2 3 14 15 16 17 8 - 4 5 6 7 10 11 12 13 9
        same 0 1 2 3 10 11 12 13 8 ///ideally want 9 unknown??
            r2 = 0 1 11 - 10 2 3
            same 0 1 10
            diff 11 2 3
            == 12 13 8
        diff 4 5 6 7 14 15 16 17 9
            r2 = 4 5 14 - 6 7 15
            same 6 7 14
            diff 4 5 15
            == 16 17 9
        ==   18 19 a b c d e f
            r2 = 18 19 a - d e b
            same a d e
            diff 18 19 b
            == c f
    ==
    0 1 2 3 4 5 6 7 8, 9 10 11 12 13
        r1 = 0 1 2 3 4 - 5 6 7 8 c
        == 9 10 11 12 13
            r2 9 10 - 11 c
            == 12 13
            else 9 10 11
                9 - 10
        else 0 1 2 3 4 5 6 7 8
            r2 = 0 1 5 - 2 3 6
            same 0 1 6
            diff 2 3 5
            == 4 7 8
                7 - 8

            can only resolve 40
    */
}
