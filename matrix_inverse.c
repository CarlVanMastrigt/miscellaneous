mat44f m44f_inv_old(mat44f m)
{
    __m128 inv_det,ysp,ysn,zsp,zsn,wsp,wsn;///respective axies shifted posotively (+,p) or negatively (-,n) for the seubset of relevant entries
    mat44f inv;

    ///there is likely some way to optimise the swizzling performed such that values vectors can be recycled when calculating different columns

    ///probably also possible to optimise by replacing dp instructions with summedadditions of some description <---- almost certainly this
    ///     ^ accumulators of stacked shuffled multiplications...

    ///x not yet relevant for it's axis, and x components not relevant to following vectors
    ///note component swizzles done forward order, index numbers done reversed

    /// __y_z_w ; x-0's irrelevant
    ysp=_mm_castsi128_ps(_mm_shuffle_epi32(_mm_castps_si128(m.y.v),0x9C));// __w_y_z 2130
    ysn=_mm_castsi128_ps(_mm_shuffle_epi32(_mm_castps_si128(m.y.v),0x78));// __z_w_y 1320
    zsp=_mm_castsi128_ps(_mm_shuffle_epi32(_mm_castps_si128(m.z.v),0x9C));// __w_y_z 2130
    zsn=_mm_castsi128_ps(_mm_shuffle_epi32(_mm_castps_si128(m.z.v),0x78));// __z_w_y 1320
    wsp=_mm_castsi128_ps(_mm_shuffle_epi32(_mm_castps_si128(m.w.v),0x9C));// __w_y_z 2130
    wsn=_mm_castsi128_ps(_mm_shuffle_epi32(_mm_castps_si128(m.w.v),0x78));// __z_w_y 1320

    inv.x.v=_mm_blend_ps(
        _mm_blend_ps(
            _mm_dp_ps(m.y.v,_mm_sub_ps(_mm_mul_ps(zsn,wsp),_mm_mul_ps(zsp,wsn)),0xE1),
            _mm_dp_ps(m.x.v,_mm_sub_ps(_mm_mul_ps(zsp,wsn),_mm_mul_ps(zsn,wsp)),0xE2),
            0x02),
        _mm_blend_ps(
            _mm_dp_ps(m.x.v,_mm_sub_ps(_mm_mul_ps(ysn,wsp),_mm_mul_ps(ysp,wsn)),0xE4),
            _mm_dp_ps(m.x.v,_mm_sub_ps(_mm_mul_ps(ysp,zsn),_mm_mul_ps(ysn,zsp)),0xE8),
            0x08),
        0x0C);


    /// x___z_w ; y-1's irrelevant
    ysp=_mm_castsi128_ps(_mm_shuffle_epi32(_mm_castps_si128(m.y.v),0x87));// w___x_z 2013
    ysn=_mm_castsi128_ps(_mm_shuffle_epi32(_mm_castps_si128(m.y.v),0x36));// z___w_x 0312
    zsp=_mm_castsi128_ps(_mm_shuffle_epi32(_mm_castps_si128(m.z.v),0x87));// w___x_z 2013
    zsn=_mm_castsi128_ps(_mm_shuffle_epi32(_mm_castps_si128(m.z.v),0x36));// z___w_x 0312
    wsp=_mm_castsi128_ps(_mm_shuffle_epi32(_mm_castps_si128(m.w.v),0x87));// w___x_z 2013
    wsn=_mm_castsi128_ps(_mm_shuffle_epi32(_mm_castps_si128(m.w.v),0x36));// z___w_x 0312

    inv.y.v=_mm_blend_ps(
        _mm_blend_ps(
            _mm_dp_ps(m.y.v,_mm_sub_ps(_mm_mul_ps(zsp,wsn),_mm_mul_ps(zsn,wsp)),0xD1),
            _mm_dp_ps(m.x.v,_mm_sub_ps(_mm_mul_ps(zsn,wsp),_mm_mul_ps(zsp,wsn)),0xD2),
            0x02),
        _mm_blend_ps(
            _mm_dp_ps(m.x.v,_mm_sub_ps(_mm_mul_ps(ysp,wsn),_mm_mul_ps(ysn,wsp)),0xD4),
            _mm_dp_ps(m.x.v,_mm_sub_ps(_mm_mul_ps(ysn,zsp),_mm_mul_ps(ysp,zsn)),0xD8),
            0x08),
        0x0C);


    /// x_y___w ; z-2's irrelevant
    ysp=_mm_castsi128_ps(_mm_shuffle_epi32(_mm_castps_si128(m.y.v),0x63));// w_x___y 1203
    ysn=_mm_castsi128_ps(_mm_shuffle_epi32(_mm_castps_si128(m.y.v),0x2D));// y_w___x 0231
    zsp=_mm_castsi128_ps(_mm_shuffle_epi32(_mm_castps_si128(m.z.v),0x63));// w_x___y 1203
    zsn=_mm_castsi128_ps(_mm_shuffle_epi32(_mm_castps_si128(m.z.v),0x2D));// y_w___x 0231
    wsp=_mm_castsi128_ps(_mm_shuffle_epi32(_mm_castps_si128(m.w.v),0x63));// w_x___y 1203
    wsn=_mm_castsi128_ps(_mm_shuffle_epi32(_mm_castps_si128(m.w.v),0x2D));// y_w___x 0231

    inv.z.v=_mm_blend_ps(
        _mm_blend_ps(
            _mm_dp_ps(m.y.v,_mm_sub_ps(_mm_mul_ps(zsn,wsp),_mm_mul_ps(zsp,wsn)),0xB1),
            _mm_dp_ps(m.x.v,_mm_sub_ps(_mm_mul_ps(zsp,wsn),_mm_mul_ps(zsn,wsp)),0xB2),
            0x02),
        _mm_blend_ps(
            _mm_dp_ps(m.x.v,_mm_sub_ps(_mm_mul_ps(ysn,wsp),_mm_mul_ps(ysp,wsn)),0xB4),
            _mm_dp_ps(m.x.v,_mm_sub_ps(_mm_mul_ps(ysp,zsn),_mm_mul_ps(ysn,zsp)),0xB8),
            0x08),
        0x0C);


    /// x_y_z__ ; w-3's irrelevant
    ysp=_mm_castsi128_ps(_mm_shuffle_epi32(_mm_castps_si128(m.y.v),0xD2));// z_x_y__ 3102
    ysn=_mm_castsi128_ps(_mm_shuffle_epi32(_mm_castps_si128(m.y.v),0xC9));// y_z_x__ 3021
    zsp=_mm_castsi128_ps(_mm_shuffle_epi32(_mm_castps_si128(m.z.v),0xD2));// z_x_y__ 3102
    zsn=_mm_castsi128_ps(_mm_shuffle_epi32(_mm_castps_si128(m.z.v),0xC9));// y_z_x__ 3021
    wsp=_mm_castsi128_ps(_mm_shuffle_epi32(_mm_castps_si128(m.w.v),0xD2));// z_x_y__ 3102
    wsn=_mm_castsi128_ps(_mm_shuffle_epi32(_mm_castps_si128(m.w.v),0xC9));// y_z_x__ 3021

    inv.w.v=_mm_blend_ps(
        _mm_blend_ps(
            _mm_dp_ps(m.y.v,_mm_sub_ps(_mm_mul_ps(zsp,wsn),_mm_mul_ps(zsn,wsp)),0x71),
            _mm_dp_ps(m.x.v,_mm_sub_ps(_mm_mul_ps(zsn,wsp),_mm_mul_ps(zsp,wsn)),0x72),
            0x02),
        _mm_blend_ps(
            _mm_dp_ps(m.x.v,_mm_sub_ps(_mm_mul_ps(ysp,wsn),_mm_mul_ps(ysn,wsp)),0x74),
            _mm_dp_ps(m.x.v,_mm_sub_ps(_mm_mul_ps(ysn,zsp),_mm_mul_ps(ysp,zsn)),0x78),
            0x08),
        0x0C);


    inv_det=_mm_dp_ps(m.x.v,_mm_movelh_ps(_mm_unpacklo_ps(inv.x.v,inv.y.v),_mm_unpacklo_ps(inv.z.v,inv.w.v)),0xFF);
    assert(_mm_cvtss_f32(inv_det)!=0.0f);///matrix is not invertible
    inv_det=_mm_div_ps(_mm_set1_ps(1.0f),inv_det);

    inv.x.v=_mm_mul_ps(inv.x.v,inv_det);
    inv.y.v=_mm_mul_ps(inv.y.v,inv_det);
    inv.z.v=_mm_mul_ps(inv.z.v,inv_det);
    inv.w.v=_mm_mul_ps(inv.w.v,inv_det);

    return inv;
}
static inline mat44f m44f_inv_A(mat44f m)
{
    //mat44f inv;
    __m128 rx,ry,rz,rw,t1,t2;

    ///p1
    t1=_mm_sub_ps(_mm_mul_ps(_mm_castsi128_ps(_mm_shuffle_epi32(_mm_castps_si128(m.z.v),0x9E)),_mm_castsi128_ps(_mm_shuffle_epi32(_mm_castps_si128(m.w.v),0x7B))),
                   _mm_mul_ps(_mm_castsi128_ps(_mm_shuffle_epi32(_mm_castps_si128(m.w.v),0x9E)),_mm_castsi128_ps(_mm_shuffle_epi32(_mm_castps_si128(m.z.v),0x7B))));
    rx=_mm_mul_ps(_mm_castsi128_ps(_mm_shuffle_epi32(_mm_castps_si128(m.y.v),0x01)),t1);
    ry=_mm_mul_ps(_mm_castsi128_ps(_mm_shuffle_epi32(_mm_castps_si128(m.x.v),0x01)),t1);

    t2=_mm_sub_ps(_mm_mul_ps(_mm_castsi128_ps(_mm_shuffle_epi32(_mm_castps_si128(m.y.v),0x01)),_mm_castsi128_ps(_mm_shuffle_epi32(_mm_castps_si128(m.x.v),0x7B))),///
                   _mm_mul_ps(_mm_castsi128_ps(_mm_shuffle_epi32(_mm_castps_si128(m.x.v),0x01)),_mm_castsi128_ps(_mm_shuffle_epi32(_mm_castps_si128(m.y.v),0x7B))));
    rz=_mm_mul_ps(_mm_castsi128_ps(_mm_shuffle_epi32(_mm_castps_si128(m.w.v),0x9E)),t2);
    rw=_mm_mul_ps(_mm_castsi128_ps(_mm_shuffle_epi32(_mm_castps_si128(m.z.v),0x9E)),t2);


    ///p2
    t1=_mm_sub_ps(_mm_mul_ps(_mm_castsi128_ps(_mm_shuffle_epi32(_mm_castps_si128(m.z.v),0x33)),_mm_castsi128_ps(_mm_shuffle_epi32(_mm_castps_si128(m.w.v),0x8D))),
                   _mm_mul_ps(_mm_castsi128_ps(_mm_shuffle_epi32(_mm_castps_si128(m.w.v),0x33)),_mm_castsi128_ps(_mm_shuffle_epi32(_mm_castps_si128(m.z.v),0x8D))));
    rx=_mm_add_ps(rx,_mm_mul_ps(_mm_castsi128_ps(_mm_shuffle_epi32(_mm_castps_si128(m.y.v),0x5A)),t1));
    ry=_mm_add_ps(ry,_mm_mul_ps(_mm_castsi128_ps(_mm_shuffle_epi32(_mm_castps_si128(m.x.v),0x5A)),t1));

    t2=_mm_sub_ps(_mm_mul_ps(_mm_castsi128_ps(_mm_shuffle_epi32(_mm_castps_si128(m.y.v),0x5A)),_mm_castsi128_ps(_mm_shuffle_epi32(_mm_castps_si128(m.x.v),0x8D))),
                   _mm_mul_ps(_mm_castsi128_ps(_mm_shuffle_epi32(_mm_castps_si128(m.x.v),0x5A)),_mm_castsi128_ps(_mm_shuffle_epi32(_mm_castps_si128(m.y.v),0x8D))));
    rz=_mm_add_ps(rz,_mm_mul_ps(_mm_castsi128_ps(_mm_shuffle_epi32(_mm_castps_si128(m.w.v),0x33)),t2));
    rw=_mm_add_ps(rw,_mm_mul_ps(_mm_castsi128_ps(_mm_shuffle_epi32(_mm_castps_si128(m.z.v),0x33)),t2));


    ///p3
    t1=_mm_sub_ps(_mm_mul_ps(_mm_castsi128_ps(_mm_shuffle_epi32(_mm_castps_si128(m.z.v),0x49)),_mm_castsi128_ps(_mm_shuffle_epi32(_mm_castps_si128(m.w.v),0x12))),
                   _mm_mul_ps(_mm_castsi128_ps(_mm_shuffle_epi32(_mm_castps_si128(m.w.v),0x49)),_mm_castsi128_ps(_mm_shuffle_epi32(_mm_castps_si128(m.z.v),0x12))));
    rx=_mm_add_ps(rx,_mm_mul_ps(_mm_castsi128_ps(_mm_shuffle_epi32(_mm_castps_si128(m.y.v),0xBF)),t1));
    ry=_mm_add_ps(ry,_mm_mul_ps(_mm_castsi128_ps(_mm_shuffle_epi32(_mm_castps_si128(m.x.v),0xBF)),t1));

    t2=_mm_sub_ps(_mm_mul_ps(_mm_castsi128_ps(_mm_shuffle_epi32(_mm_castps_si128(m.y.v),0xBF)),_mm_castsi128_ps(_mm_shuffle_epi32(_mm_castps_si128(m.x.v),0x12))),
                   _mm_mul_ps(_mm_castsi128_ps(_mm_shuffle_epi32(_mm_castps_si128(m.x.v),0xBF)),_mm_castsi128_ps(_mm_shuffle_epi32(_mm_castps_si128(m.y.v),0x12))));
    rz=_mm_add_ps(rz,_mm_mul_ps(_mm_castsi128_ps(_mm_shuffle_epi32(_mm_castps_si128(m.w.v),0x49)),t2));
    rw=_mm_add_ps(rw,_mm_mul_ps(_mm_castsi128_ps(_mm_shuffle_epi32(_mm_castps_si128(m.z.v),0x49)),t2));


    __m128 inv_det=_mm_dp_ps(m.x.v,rx,0xFF);
    assert(_mm_cvtss_f32(inv_det)!=0.0f);///matrix is not invertible
    inv_det=_mm_div_ps(_mm_set1_ps(1.0f),inv_det);

    rx=_mm_mul_ps(rx,inv_det);
    rz=_mm_mul_ps(rz,inv_det);

    inv_det=_mm_castsi128_ps(_mm_xor_si128(_mm_set1_epi32(0x80000000),_mm_castps_si128(inv_det)));

    ry=_mm_mul_ps(ry,inv_det);
    rw=_mm_mul_ps(rw,inv_det);

    m.x.v=rx;
    m.y.v=ry;
    m.z.v=rz;
    m.w.v=rw;


    t1=_mm_unpacklo_ps(rx,ry);
    t2=_mm_unpacklo_ps(rz,rw);
    m.x.v=_mm_movelh_ps(t1,t2);
    m.y.v=_mm_movehl_ps(t2,t1);

    t1=_mm_unpackhi_ps(rx,ry);
    t2=_mm_unpackhi_ps(rz,rw);
    m.z.v=_mm_movelh_ps(t1,t2);
    m.w.v=_mm_movehl_ps(t2,t1);



    return m;
}

static inline mat44f m44f_inv_B(mat44f m)
{
    mat44f inv;

    __m128 p1,p2,p3;//rename to c for column?

    __m128 z_zwyz=_mm_castsi128_ps(_mm_shuffle_epi32(_mm_castps_si128(m.z.v),0x9E));
    __m128 w_wzwy=_mm_castsi128_ps(_mm_shuffle_epi32(_mm_castps_si128(m.w.v),0x7B));//discard
    __m128 z_wzwy=_mm_castsi128_ps(_mm_shuffle_epi32(_mm_castps_si128(m.z.v),0x7B));//discard
    __m128 w_zwyz=_mm_castsi128_ps(_mm_shuffle_epi32(_mm_castps_si128(m.w.v),0x9E));
    p1=_mm_sub_ps(_mm_mul_ps(z_zwyz,w_wzwy),_mm_mul_ps(z_wzwy,w_zwyz));



    __m128 z_wxwx=_mm_castsi128_ps(_mm_shuffle_epi32(_mm_castps_si128(m.z.v),0x33));
    __m128 w_ywxz=_mm_castsi128_ps(_mm_shuffle_epi32(_mm_castps_si128(m.w.v),0x8D));//discard
    __m128 z_ywxz=_mm_castsi128_ps(_mm_shuffle_epi32(_mm_castps_si128(m.z.v),0x8D));//discard
    __m128 w_wxwx=_mm_castsi128_ps(_mm_shuffle_epi32(_mm_castps_si128(m.w.v),0x33));
    p2=_mm_sub_ps(_mm_mul_ps(z_wxwx,w_ywxz),_mm_mul_ps(z_ywxz,w_wxwx));



    __m128 z_yzxy=_mm_castsi128_ps(_mm_shuffle_epi32(_mm_castps_si128(m.z.v),0x49));
    __m128 w_zxyx=_mm_castsi128_ps(_mm_shuffle_epi32(_mm_castps_si128(m.w.v),0x12));//discard
    __m128 z_zxyx=_mm_castsi128_ps(_mm_shuffle_epi32(_mm_castps_si128(m.z.v),0x12));//discard
    __m128 w_yzxy=_mm_castsi128_ps(_mm_shuffle_epi32(_mm_castps_si128(m.w.v),0x49));
    p3=_mm_sub_ps(_mm_mul_ps(z_yzxy,w_zxyx),_mm_mul_ps(z_zxyx,w_yzxy));



    __m128 y_yxxx=_mm_castsi128_ps(_mm_shuffle_epi32(_mm_castps_si128(m.y.v),0x01));
    __m128 y_zzyy=_mm_castsi128_ps(_mm_shuffle_epi32(_mm_castps_si128(m.y.v),0x5A));
    __m128 y_wwwz=_mm_castsi128_ps(_mm_shuffle_epi32(_mm_castps_si128(m.y.v),0xBF));

    __m128 x_yxxx=_mm_castsi128_ps(_mm_shuffle_epi32(_mm_castps_si128(m.x.v),0x01));
    __m128 x_zzyy=_mm_castsi128_ps(_mm_shuffle_epi32(_mm_castps_si128(m.x.v),0x5A));
    __m128 x_wwwz=_mm_castsi128_ps(_mm_shuffle_epi32(_mm_castps_si128(m.x.v),0xBF));

    inv.x.v=_mm_add_ps(_mm_add_ps(
        _mm_mul_ps(y_yxxx,p1),
        _mm_mul_ps(y_zzyy,p2)),
        _mm_mul_ps(y_wwwz,p3));



    ///can just negate this whole thing! (using same guts)
    ///_mm_xor_si128 < this, flip sign bits
    inv.y.v=_mm_castsi128_ps(_mm_xor_si128(_mm_set1_epi32(0x80000000),_mm_castps_si128(
        _mm_add_ps(_mm_add_ps(
            _mm_mul_ps(x_yxxx,p1),
            _mm_mul_ps(x_zzyy,p2)),
            _mm_mul_ps(x_wwwz,p3)))));


    __m128 x_wzwy=_mm_castsi128_ps(_mm_shuffle_epi32(_mm_castps_si128(m.x.v),0x7B));
    //__m128 y_yxxx=_mm_castsi128_ps(_mm_shuffle_epi32(_mm_castps_si128(m.y.v),0x01));
    //__m128 x_yxxx=_mm_castsi128_ps(_mm_shuffle_epi32(_mm_castps_si128(m.x.v),0x01));
    __m128 y_wzwy=_mm_castsi128_ps(_mm_shuffle_epi32(_mm_castps_si128(m.y.v),0x7B));
    p1=_mm_sub_ps(_mm_mul_ps(x_wzwy,y_yxxx),_mm_mul_ps(x_yxxx,y_wzwy));



    __m128 x_ywxz=_mm_castsi128_ps(_mm_shuffle_epi32(_mm_castps_si128(m.x.v),0x8D));
    //__m128 y_zzyy=_mm_castsi128_ps(_mm_shuffle_epi32(_mm_castps_si128(m.y.v),0x5A));
    //__m128 x_zzyy=_mm_castsi128_ps(_mm_shuffle_epi32(_mm_castps_si128(m.x.v),0x5A));
    __m128 y_ywxz=_mm_castsi128_ps(_mm_shuffle_epi32(_mm_castps_si128(m.y.v),0x8D));
    p2=_mm_sub_ps(_mm_mul_ps(x_ywxz,y_zzyy),_mm_mul_ps(x_zzyy,y_ywxz));



    __m128 x_zxyx=_mm_castsi128_ps(_mm_shuffle_epi32(_mm_castps_si128(m.x.v),0x13));
    //__m128 y_wwwz=_mm_castsi128_ps(_mm_shuffle_epi32(_mm_castps_si128(m.y.v),0xBF));
    //__m128 x_wwwz=_mm_castsi128_ps(_mm_shuffle_epi32(_mm_castps_si128(m.x.v),0xBF));
    __m128 y_zxyx=_mm_castsi128_ps(_mm_shuffle_epi32(_mm_castps_si128(m.y.v),0x13));
    p3=_mm_sub_ps(_mm_mul_ps(x_zxyx,y_wwwz),_mm_mul_ps(x_wwwz,y_zxyx));


    inv.z.v=_mm_add_ps(_mm_add_ps(
        _mm_mul_ps(w_zwyz,p1), // w_zwyz
        _mm_mul_ps(w_wxwx,p2)),
        _mm_mul_ps(w_yzxy,p3));

    inv.w.v=_mm_castsi128_ps(_mm_xor_si128(_mm_set1_epi32(0x80000000),_mm_castps_si128(
        _mm_add_ps(_mm_add_ps(
            _mm_mul_ps(z_zwyz,p1), // w_zwyz
            _mm_mul_ps(z_wxwx,p2)),
            _mm_mul_ps(z_yzxy,p3)))));

    __m128 inv_det=_mm_dp_ps(m.x.v,inv.x.v,0xFF);
    assert(_mm_cvtss_f32(inv_det)!=0.0f);///matrix is not invertible
    inv_det=_mm_div_ps(_mm_set1_ps(1.0f),inv_det);

    inv.x.v=_mm_mul_ps(inv.x.v,inv_det);
    inv.y.v=_mm_mul_ps(inv.y.v,inv_det);
    inv.z.v=_mm_mul_ps(inv.z.v,inv_det);
    inv.w.v=_mm_mul_ps(inv.w.v,inv_det);

    ///
    return m44f_transpose_(inv);
}

static inline mat44f m44f_inv_new(mat44f m)
{
    mat44f inv;

    __m128 p1=_mm_sub_ps(_mm_mul_ps(_mm_castsi128_ps(_mm_shuffle_epi32(_mm_castps_si128(m.z.v),0x9E)),_mm_castsi128_ps(_mm_shuffle_epi32(_mm_castps_si128(m.w.v),0x7B))),
                         _mm_mul_ps(_mm_castsi128_ps(_mm_shuffle_epi32(_mm_castps_si128(m.z.v),0x7B)),_mm_castsi128_ps(_mm_shuffle_epi32(_mm_castps_si128(m.w.v),0x9E))));
    __m128 p2=_mm_sub_ps(_mm_mul_ps(_mm_castsi128_ps(_mm_shuffle_epi32(_mm_castps_si128(m.z.v),0x33)),_mm_castsi128_ps(_mm_shuffle_epi32(_mm_castps_si128(m.w.v),0x8D))),
                         _mm_mul_ps(_mm_castsi128_ps(_mm_shuffle_epi32(_mm_castps_si128(m.z.v),0x8D)),_mm_castsi128_ps(_mm_shuffle_epi32(_mm_castps_si128(m.w.v),0x33))));
    __m128 p3=_mm_sub_ps(_mm_mul_ps(_mm_castsi128_ps(_mm_shuffle_epi32(_mm_castps_si128(m.z.v),0x49)),_mm_castsi128_ps(_mm_shuffle_epi32(_mm_castps_si128(m.w.v),0x12))),
                         _mm_mul_ps(_mm_castsi128_ps(_mm_shuffle_epi32(_mm_castps_si128(m.z.v),0x12)),_mm_castsi128_ps(_mm_shuffle_epi32(_mm_castps_si128(m.w.v),0x49))));

    inv.x.v=_mm_add_ps(_mm_add_ps(
        _mm_mul_ps(_mm_castsi128_ps(_mm_shuffle_epi32(_mm_castps_si128(m.y.v),0x01)),p1),   // yxxx 0001
        _mm_mul_ps(_mm_castsi128_ps(_mm_shuffle_epi32(_mm_castps_si128(m.y.v),0x5A)),p2)),  // zzyy 1122
        _mm_mul_ps(_mm_castsi128_ps(_mm_shuffle_epi32(_mm_castps_si128(m.y.v),0xBF)),p3));  // wwwz 2333

    __m128 x_yxxx=_mm_castsi128_ps(_mm_shuffle_epi32(_mm_castps_si128(m.x.v),0x01));
    __m128 x_zzyy=_mm_castsi128_ps(_mm_shuffle_epi32(_mm_castps_si128(m.x.v),0x5A));
    __m128 x_wwwz=_mm_castsi128_ps(_mm_shuffle_epi32(_mm_castps_si128(m.x.v),0xBF));

    ///can just negate this whole thing! (using same guts)
    ///_mm_xor_si128 < this, flip sign bits
    inv.y.v=_mm_castsi128_ps(_mm_xor_si128(_mm_set1_epi32(0x80000000),_mm_castps_si128(
        _mm_add_ps(_mm_add_ps(_mm_mul_ps(x_yxxx,p1),_mm_mul_ps(x_zzyy,p2)),_mm_mul_ps(x_wwwz,p3)))));

///possible to re-arrange following too

    inv.z.v=_mm_add_ps(_mm_add_ps(
        _mm_mul_ps(x_yxxx,_mm_sub_ps(
                _mm_mul_ps(_mm_castsi128_ps(_mm_shuffle_epi32(_mm_castps_si128(m.y.v),0x9E)),_mm_castsi128_ps(_mm_shuffle_epi32(_mm_castps_si128(m.w.v),0x7B))),
                _mm_mul_ps(_mm_castsi128_ps(_mm_shuffle_epi32(_mm_castps_si128(m.y.v),0x7B)),_mm_castsi128_ps(_mm_shuffle_epi32(_mm_castps_si128(m.w.v),0x9E))))),
        _mm_mul_ps(x_zzyy,_mm_sub_ps(
                _mm_mul_ps(_mm_castsi128_ps(_mm_shuffle_epi32(_mm_castps_si128(m.y.v),0x33)),_mm_castsi128_ps(_mm_shuffle_epi32(_mm_castps_si128(m.w.v),0x8D))),
                _mm_mul_ps(_mm_castsi128_ps(_mm_shuffle_epi32(_mm_castps_si128(m.y.v),0x8D)),_mm_castsi128_ps(_mm_shuffle_epi32(_mm_castps_si128(m.w.v),0x33)))))),
        _mm_mul_ps(x_wwwz,
            _mm_sub_ps(
                _mm_mul_ps(_mm_castsi128_ps(_mm_shuffle_epi32(_mm_castps_si128(m.y.v),0x49)),_mm_castsi128_ps(_mm_shuffle_epi32(_mm_castps_si128(m.w.v),0x12))),
                _mm_mul_ps(_mm_castsi128_ps(_mm_shuffle_epi32(_mm_castps_si128(m.y.v),0x12)),_mm_castsi128_ps(_mm_shuffle_epi32(_mm_castps_si128(m.w.v),0x49))))));


    inv.w.v=_mm_add_ps(_mm_add_ps(
        _mm_mul_ps(x_yxxx,_mm_sub_ps(
                _mm_mul_ps(_mm_castsi128_ps(_mm_shuffle_epi32(_mm_castps_si128(m.y.v),0x7B)),_mm_castsi128_ps(_mm_shuffle_epi32(_mm_castps_si128(m.z.v),0x9E))),
                _mm_mul_ps(_mm_castsi128_ps(_mm_shuffle_epi32(_mm_castps_si128(m.y.v),0x9E)),_mm_castsi128_ps(_mm_shuffle_epi32(_mm_castps_si128(m.z.v),0x7B))))),
        _mm_mul_ps(x_zzyy,_mm_sub_ps(
                _mm_mul_ps(_mm_castsi128_ps(_mm_shuffle_epi32(_mm_castps_si128(m.y.v),0x8D)),_mm_castsi128_ps(_mm_shuffle_epi32(_mm_castps_si128(m.z.v),0x33))),
                _mm_mul_ps(_mm_castsi128_ps(_mm_shuffle_epi32(_mm_castps_si128(m.y.v),0x33)),_mm_castsi128_ps(_mm_shuffle_epi32(_mm_castps_si128(m.z.v),0x8D)))))),
        _mm_mul_ps(x_wwwz,_mm_sub_ps(
                _mm_mul_ps(_mm_castsi128_ps(_mm_shuffle_epi32(_mm_castps_si128(m.y.v),0x12)),_mm_castsi128_ps(_mm_shuffle_epi32(_mm_castps_si128(m.z.v),0x49))),
                _mm_mul_ps(_mm_castsi128_ps(_mm_shuffle_epi32(_mm_castps_si128(m.y.v),0x49)),_mm_castsi128_ps(_mm_shuffle_epi32(_mm_castps_si128(m.z.v),0x12))))));


    __m128 inv_det=_mm_dp_ps(m.x.v,inv.x.v,0xFF);
    assert(_mm_cvtss_f32(inv_det)!=0.0f);///matrix is not invertible
    inv_det=_mm_div_ps(_mm_set1_ps(1.0f),inv_det);

    inv.x.v=_mm_mul_ps(inv.x.v,inv_det);
    inv.y.v=_mm_mul_ps(inv.y.v,inv_det);
    inv.z.v=_mm_mul_ps(inv.z.v,inv_det);
    inv.w.v=_mm_mul_ps(inv.w.v,inv_det);

    ///
    return m44f_transpose_(inv);
}

//static inline mat44f m44f_inv_new(mat44f m)
//{
//    mat44f inv;
//
//    inv.x.v=_mm_add_ps(_mm_add_ps(
//        _mm_mul_ps(_mm_castsi128_ps(_mm_shuffle_epi32(_mm_castps_si128(m.y.v),0x01)),       // yxxx 0001
//            _mm_sub_ps(
//                _mm_mul_ps(
//                    _mm_castsi128_ps(_mm_shuffle_epi32(_mm_castps_si128(m.z.v),0x9E)),      // zwyz 2132
//                    _mm_castsi128_ps(_mm_shuffle_epi32(_mm_castps_si128(m.w.v),0x7B))),     // wzwy 1323
//                _mm_mul_ps(
//                    _mm_castsi128_ps(_mm_shuffle_epi32(_mm_castps_si128(m.z.v),0x7B)),      // wzwy 1323
//                    _mm_castsi128_ps(_mm_shuffle_epi32(_mm_castps_si128(m.w.v),0x9E))))),   // zwyz 2132
//            //==================================================
//        _mm_mul_ps(_mm_castsi128_ps(_mm_shuffle_epi32(_mm_castps_si128(m.y.v),0x5A)),       // zzyy 1122
//            _mm_sub_ps(
//                _mm_mul_ps(
//                    _mm_castsi128_ps(_mm_shuffle_epi32(_mm_castps_si128(m.z.v),0x33)),      // wxwx 0303
//                    _mm_castsi128_ps(_mm_shuffle_epi32(_mm_castps_si128(m.w.v),0x8D))),     // ywxz 2031
//                _mm_mul_ps(
//                    _mm_castsi128_ps(_mm_shuffle_epi32(_mm_castps_si128(m.z.v),0x8D)),      // ywxz 2031
//                    _mm_castsi128_ps(_mm_shuffle_epi32(_mm_castps_si128(m.w.v),0x33)))))),  // wxwx 0303
//            //==================================================
//        _mm_mul_ps(_mm_castsi128_ps(_mm_shuffle_epi32(_mm_castps_si128(m.y.v),0xBF)),       // wwwz 2333
//            _mm_sub_ps(
//                _mm_mul_ps(
//                    _mm_castsi128_ps(_mm_shuffle_epi32(_mm_castps_si128(m.z.v),0x49)),      // yzxy 1021
//                    _mm_castsi128_ps(_mm_shuffle_epi32(_mm_castps_si128(m.w.v),0x12))),     // zxyx 0102
//                _mm_mul_ps(
//                    _mm_castsi128_ps(_mm_shuffle_epi32(_mm_castps_si128(m.z.v),0x12)),      // zxyx 0102
//                    _mm_castsi128_ps(_mm_shuffle_epi32(_mm_castps_si128(m.w.v),0x49))))));  // yzxy 1021
//
//    inv.y.v=_mm_add_ps(_mm_add_ps(
//        _mm_mul_ps(_mm_castsi128_ps(_mm_shuffle_epi32(_mm_castps_si128(m.x.v),0x01)),
//            _mm_sub_ps(
//                _mm_mul_ps(
//                    _mm_castsi128_ps(_mm_shuffle_epi32(_mm_castps_si128(m.z.v),0x7B)),
//                    _mm_castsi128_ps(_mm_shuffle_epi32(_mm_castps_si128(m.w.v),0x9E))),
//                _mm_mul_ps(
//                    _mm_castsi128_ps(_mm_shuffle_epi32(_mm_castps_si128(m.z.v),0x9E)),
//                    _mm_castsi128_ps(_mm_shuffle_epi32(_mm_castps_si128(m.w.v),0x7B))))),
//            //==================================================
//        _mm_mul_ps(_mm_castsi128_ps(_mm_shuffle_epi32(_mm_castps_si128(m.x.v),0x5A)),
//            _mm_sub_ps(
//                _mm_mul_ps(
//                    _mm_castsi128_ps(_mm_shuffle_epi32(_mm_castps_si128(m.z.v),0x8D)),
//                    _mm_castsi128_ps(_mm_shuffle_epi32(_mm_castps_si128(m.w.v),0x33))),
//                _mm_mul_ps(
//                    _mm_castsi128_ps(_mm_shuffle_epi32(_mm_castps_si128(m.z.v),0x33)),
//                    _mm_castsi128_ps(_mm_shuffle_epi32(_mm_castps_si128(m.w.v),0x8D)))))),
//            //==================================================
//        _mm_mul_ps(_mm_castsi128_ps(_mm_shuffle_epi32(_mm_castps_si128(m.x.v),0xBF)),
//            _mm_sub_ps(
//                _mm_mul_ps(
//                    _mm_castsi128_ps(_mm_shuffle_epi32(_mm_castps_si128(m.z.v),0x12)),
//                    _mm_castsi128_ps(_mm_shuffle_epi32(_mm_castps_si128(m.w.v),0x49))),
//                _mm_mul_ps(
//                    _mm_castsi128_ps(_mm_shuffle_epi32(_mm_castps_si128(m.z.v),0x49)),
//                    _mm_castsi128_ps(_mm_shuffle_epi32(_mm_castps_si128(m.w.v),0x12))))));
//
//    inv.z.v=_mm_add_ps(_mm_add_ps(
//        _mm_mul_ps(_mm_castsi128_ps(_mm_shuffle_epi32(_mm_castps_si128(m.x.v),0x01)),
//            _mm_sub_ps(
//                _mm_mul_ps(
//                    _mm_castsi128_ps(_mm_shuffle_epi32(_mm_castps_si128(m.y.v),0x9E)),
//                    _mm_castsi128_ps(_mm_shuffle_epi32(_mm_castps_si128(m.w.v),0x7B))),
//                _mm_mul_ps(
//                    _mm_castsi128_ps(_mm_shuffle_epi32(_mm_castps_si128(m.y.v),0x7B)),
//                    _mm_castsi128_ps(_mm_shuffle_epi32(_mm_castps_si128(m.w.v),0x9E))))),
//            //==================================================
//        _mm_mul_ps(_mm_castsi128_ps(_mm_shuffle_epi32(_mm_castps_si128(m.x.v),0x5A)),
//            _mm_sub_ps(
//                _mm_mul_ps(
//                    _mm_castsi128_ps(_mm_shuffle_epi32(_mm_castps_si128(m.y.v),0x33)),
//                    _mm_castsi128_ps(_mm_shuffle_epi32(_mm_castps_si128(m.w.v),0x8D))),
//                _mm_mul_ps(
//                    _mm_castsi128_ps(_mm_shuffle_epi32(_mm_castps_si128(m.y.v),0x8D)),
//                    _mm_castsi128_ps(_mm_shuffle_epi32(_mm_castps_si128(m.w.v),0x33)))))),
//            //==================================================
//        _mm_mul_ps(_mm_castsi128_ps(_mm_shuffle_epi32(_mm_castps_si128(m.x.v),0xBF)),
//            _mm_sub_ps(
//                _mm_mul_ps(
//                    _mm_castsi128_ps(_mm_shuffle_epi32(_mm_castps_si128(m.y.v),0x49)),
//                    _mm_castsi128_ps(_mm_shuffle_epi32(_mm_castps_si128(m.w.v),0x12))),
//                _mm_mul_ps(
//                    _mm_castsi128_ps(_mm_shuffle_epi32(_mm_castps_si128(m.y.v),0x12)),
//                    _mm_castsi128_ps(_mm_shuffle_epi32(_mm_castps_si128(m.w.v),0x49))))));
//
//
//    inv.w.v=_mm_add_ps(_mm_add_ps(
//        _mm_mul_ps(_mm_castsi128_ps(_mm_shuffle_epi32(_mm_castps_si128(m.x.v),0x01)),
//            _mm_sub_ps(
//                _mm_mul_ps(
//                    _mm_castsi128_ps(_mm_shuffle_epi32(_mm_castps_si128(m.y.v),0x7B)),
//                    _mm_castsi128_ps(_mm_shuffle_epi32(_mm_castps_si128(m.z.v),0x9E))),
//                _mm_mul_ps(
//                    _mm_castsi128_ps(_mm_shuffle_epi32(_mm_castps_si128(m.y.v),0x9E)),
//                    _mm_castsi128_ps(_mm_shuffle_epi32(_mm_castps_si128(m.z.v),0x7B))))),
//            //==================================================
//        _mm_mul_ps(_mm_castsi128_ps(_mm_shuffle_epi32(_mm_castps_si128(m.x.v),0x5A)),
//            _mm_sub_ps(
//                _mm_mul_ps(
//                    _mm_castsi128_ps(_mm_shuffle_epi32(_mm_castps_si128(m.y.v),0x8D)),
//                    _mm_castsi128_ps(_mm_shuffle_epi32(_mm_castps_si128(m.z.v),0x33))),
//                _mm_mul_ps(
//                    _mm_castsi128_ps(_mm_shuffle_epi32(_mm_castps_si128(m.y.v),0x33)),
//                    _mm_castsi128_ps(_mm_shuffle_epi32(_mm_castps_si128(m.z.v),0x8D)))))),
//            //==================================================
//        _mm_mul_ps(_mm_castsi128_ps(_mm_shuffle_epi32(_mm_castps_si128(m.x.v),0xBF)),
//            _mm_sub_ps(
//                _mm_mul_ps(
//                    _mm_castsi128_ps(_mm_shuffle_epi32(_mm_castps_si128(m.y.v),0x12)),
//                    _mm_castsi128_ps(_mm_shuffle_epi32(_mm_castps_si128(m.z.v),0x49))),
//                _mm_mul_ps(
//                    _mm_castsi128_ps(_mm_shuffle_epi32(_mm_castps_si128(m.y.v),0x49)),
//                    _mm_castsi128_ps(_mm_shuffle_epi32(_mm_castps_si128(m.z.v),0x12))))));
//
//
//    __m128 inv_det=_mm_dp_ps(m.x.v,inv.x.v,0xFF);
//    assert(_mm_cvtss_f32(inv_det)!=0.0f);///matrix is not invertible
//    inv_det=_mm_div_ps(_mm_set1_ps(1.0f),inv_det);
//
//    inv.x.v=_mm_mul_ps(inv.x.v,inv_det);
//    inv.y.v=_mm_mul_ps(inv.y.v,inv_det);
//    inv.z.v=_mm_mul_ps(inv.z.v,inv_det);
//    inv.w.v=_mm_mul_ps(inv.w.v,inv_det);
//
//    ///
//    return m44f_transpose(inv);
//}
static inline mat44f m44f_inv_D(mat44f m)
{
    mat44f inv;
    /// 3 uses
    __m128 z_zwyz=_mm_castsi128_ps(_mm_shuffle_epi32(_mm_castps_si128(m.z.v),0x9E));
    __m128 w_wzwy=_mm_castsi128_ps(_mm_shuffle_epi32(_mm_castps_si128(m.w.v),0x7B));
    __m128 z_wzwy=_mm_castsi128_ps(_mm_shuffle_epi32(_mm_castps_si128(m.z.v),0x7B));
    __m128 w_zwyz=_mm_castsi128_ps(_mm_shuffle_epi32(_mm_castps_si128(m.w.v),0x9E));
    __m128 z_wxwx=_mm_castsi128_ps(_mm_shuffle_epi32(_mm_castps_si128(m.z.v),0x33));
    __m128 w_ywxz=_mm_castsi128_ps(_mm_shuffle_epi32(_mm_castps_si128(m.w.v),0x8D));
    __m128 z_ywxz=_mm_castsi128_ps(_mm_shuffle_epi32(_mm_castps_si128(m.z.v),0x8D));
    __m128 w_wxwx=_mm_castsi128_ps(_mm_shuffle_epi32(_mm_castps_si128(m.w.v),0x33));
    __m128 z_yzxy=_mm_castsi128_ps(_mm_shuffle_epi32(_mm_castps_si128(m.z.v),0x49));
    __m128 w_zxyx=_mm_castsi128_ps(_mm_shuffle_epi32(_mm_castps_si128(m.w.v),0x12));
    __m128 z_zxyx=_mm_castsi128_ps(_mm_shuffle_epi32(_mm_castps_si128(m.z.v),0x12));
    __m128 w_yzxy=_mm_castsi128_ps(_mm_shuffle_epi32(_mm_castps_si128(m.w.v),0x49));

//    __m128 x_yxxx=_mm_castsi128_ps(_mm_shuffle_epi32(_mm_castps_si128(m.x.v),0x01));
//    __m128 x_zzyy=_mm_castsi128_ps(_mm_shuffle_epi32(_mm_castps_si128(m.x.v),0x5A));
//    __m128 x_wwwz=_mm_castsi128_ps(_mm_shuffle_epi32(_mm_castps_si128(m.x.v),0xBF));
//    ///2 uses...
//    __m128 y_zwyz=_mm_castsi128_ps(_mm_shuffle_epi32(_mm_castps_si128(m.y.v),0x9E));
//    __m128 y_wzwy=_mm_castsi128_ps(_mm_shuffle_epi32(_mm_castps_si128(m.y.v),0x7B));
//    __m128 y_wxwx=_mm_castsi128_ps(_mm_shuffle_epi32(_mm_castps_si128(m.y.v),0x33));
//    __m128 y_ywxz=_mm_castsi128_ps(_mm_shuffle_epi32(_mm_castps_si128(m.y.v),0x8D));
//    __m128 y_yzxy=_mm_castsi128_ps(_mm_shuffle_epi32(_mm_castps_si128(m.y.v),0x49));
//    __m128 y_zxyx=_mm_castsi128_ps(_mm_shuffle_epi32(_mm_castps_si128(m.y.v),0x12));


    inv.x.v=_mm_add_ps(_mm_add_ps(
        _mm_mul_ps(_mm_castsi128_ps(_mm_shuffle_epi32(_mm_castps_si128(m.y.v),0x01)),       // yxxx 0001
            _mm_sub_ps(_mm_mul_ps(z_zwyz,w_wzwy),_mm_mul_ps(z_wzwy,w_zwyz))),
        _mm_mul_ps(_mm_castsi128_ps(_mm_shuffle_epi32(_mm_castps_si128(m.y.v),0x5A)),       // zzyy 1122
            _mm_sub_ps(_mm_mul_ps(z_wxwx,w_ywxz),_mm_mul_ps(z_ywxz,w_wxwx)))),
        _mm_mul_ps(_mm_castsi128_ps(_mm_shuffle_epi32(_mm_castps_si128(m.y.v),0xBF)),       // wwwz 2333
            _mm_sub_ps(_mm_mul_ps(z_yzxy,w_zxyx),_mm_mul_ps(z_zxyx,w_yzxy))));


    __m128 x_yxxx=_mm_castsi128_ps(_mm_shuffle_epi32(_mm_castps_si128(m.x.v),0x01));
    __m128 x_zzyy=_mm_castsi128_ps(_mm_shuffle_epi32(_mm_castps_si128(m.x.v),0x5A));
    __m128 x_wwwz=_mm_castsi128_ps(_mm_shuffle_epi32(_mm_castps_si128(m.x.v),0xBF));
    ///2 uses...
    __m128 y_zwyz=_mm_castsi128_ps(_mm_shuffle_epi32(_mm_castps_si128(m.y.v),0x9E));
    __m128 y_wzwy=_mm_castsi128_ps(_mm_shuffle_epi32(_mm_castps_si128(m.y.v),0x7B));
    __m128 y_wxwx=_mm_castsi128_ps(_mm_shuffle_epi32(_mm_castps_si128(m.y.v),0x33));
    __m128 y_ywxz=_mm_castsi128_ps(_mm_shuffle_epi32(_mm_castps_si128(m.y.v),0x8D));
    __m128 y_yzxy=_mm_castsi128_ps(_mm_shuffle_epi32(_mm_castps_si128(m.y.v),0x49));
    __m128 y_zxyx=_mm_castsi128_ps(_mm_shuffle_epi32(_mm_castps_si128(m.y.v),0x12));

    inv.y.v=_mm_add_ps(_mm_add_ps(
        _mm_mul_ps(x_yxxx,_mm_sub_ps(_mm_mul_ps(z_wzwy,w_zwyz),_mm_mul_ps(z_zwyz,w_wzwy))),
        _mm_mul_ps(x_zzyy,_mm_sub_ps(_mm_mul_ps(z_ywxz,w_wxwx),_mm_mul_ps(z_wxwx,w_ywxz)))),
        _mm_mul_ps(x_wwwz,_mm_sub_ps(_mm_mul_ps(z_zxyx,w_yzxy),_mm_mul_ps(z_yzxy,w_zxyx))));

    inv.z.v=_mm_add_ps(_mm_add_ps(
        _mm_mul_ps(x_yxxx,_mm_sub_ps(_mm_mul_ps(y_zwyz,w_wzwy),_mm_mul_ps(y_wzwy,w_zwyz))),
        _mm_mul_ps(x_zzyy,_mm_sub_ps(_mm_mul_ps(y_wxwx,w_ywxz),_mm_mul_ps(y_ywxz,w_wxwx)))),
        _mm_mul_ps(x_wwwz,_mm_sub_ps(_mm_mul_ps(y_yzxy,w_zxyx),_mm_mul_ps(y_zxyx,w_yzxy))));


    inv.w.v=_mm_add_ps(_mm_add_ps(
        _mm_mul_ps(x_yxxx,_mm_sub_ps(_mm_mul_ps(y_wzwy,z_zwyz),_mm_mul_ps(y_zwyz,z_wzwy))),
        _mm_mul_ps(x_zzyy,_mm_sub_ps(_mm_mul_ps(y_ywxz,z_wxwx),_mm_mul_ps(y_wxwx,z_ywxz)))),
        _mm_mul_ps(x_wwwz,_mm_sub_ps(_mm_mul_ps(y_zxyx,z_yzxy),_mm_mul_ps(y_yzxy,z_zxyx))));


    __m128 inv_det=_mm_dp_ps(m.x.v,inv.x.v,0xFF);
    assert(_mm_cvtss_f32(inv_det)!=0.0f);///matrix is not invertible
    inv_det=_mm_div_ps(_mm_set1_ps(1.0f),inv_det);

    inv.x.v=_mm_mul_ps(inv.x.v,inv_det);
    inv.y.v=_mm_mul_ps(inv.y.v,inv_det);
    inv.z.v=_mm_mul_ps(inv.z.v,inv_det);
    inv.w.v=_mm_mul_ps(inv.w.v,inv_det);

    ///
    return m44f_transpose_(inv);
}
