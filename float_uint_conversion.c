
/// this allows conversion between float and unsigned 32 bit interger in a way that preserves order for purposes of greater than/less than comparisons, which is useful for atomic min/max operations

///TECHNICALLY -0 == 0 but we consider them different for simplicity here

typedef union fu
{
    float f;
    uint32_t u;
}
fu;

uint32_t ofu_encode(uint32_t in)
{
    /**
    positive if s=0
    want LARGER if positive, so flip top bit, flip top bit back upon decode
    */

    return in^((in&0x80000000)?0xFFFFFFFF:0x80000000);
}

float ofu_decode_f(uint32_t in)
{
    /**
    negative if sign=0
    want LARGER if positive, so flip top bit, flip top bit back upon decode
    */
    fu f;
    f.u=in^((in&0x80000000)?0x80000000:0xFFFFFFFF);
    return f.f;
}

uint32_t ofu_decode_u(uint32_t in)
{
    return in^((in&0x80000000)?0x80000000:0xFFFFFFFF);
}

void ordered_float_uint_conversion_test(void)
{
    uint32_t i;

    if(ofu_decode_u(ofu_encode(0))!=0)
    {
        printf("0 fails to convert back to itself");
    }
    for(i=1;i<0xFFFFFFFF;i++)
    {
        if(ofu_decode_f(i-1) > ofu_decode_f(i)  ||  ofu_decode_u(ofu_encode(i))!=i)
        {
            printf("fail case: %u < %u : %u : %f >= %f \n",i-1,i,ofu_decode_u(ofu_encode(i)),ofu_decode_f(i-1),ofu_decode_f(i));
            break;
        }
    }
}
