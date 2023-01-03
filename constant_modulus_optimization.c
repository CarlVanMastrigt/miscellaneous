/** 

Following excerpt from licence copied here because i want to STRESS this part

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
SOFTWARE.
*/

static inline int32_t test_mod_1(int32_t n,int32_t d)
{
    int32_t sh,sm,de;
    int64_t ad=labs((int64_t)d);
    int64_t m=((((int64_t)1)<<(0x20))/ad)-((((int64_t)1)<<(0x20))-1);///subtract 2^32 (then add n later before right shifting 32) to ensure multiplier stays withing s32 range
    //sm=d>>31;///divisor sign mask (all bits)
    //de=(((uint32_t)n)>>31) + sm;
    m*=n;
    m&=(((int64_t)1)<<(0x20))-1;
    m*=d;
    return (m>>32);///xor and sub replaces multiplication of sign
//    return ((((m>>32)+n)>>sh)+de)^sm;///xor and sub replaces multiplication of sign
    ///sign bit can be handled w/o multiplication, abuse 2's compliment (sub 1 and swap top bit if result needs to be negated)
}

static inline int32_t test_mod_2(int32_t n,int32_t d)
{
    ///will produce undefined behaviour when (d==1 or d==-1) and n==INT_MIN , these cases COULD be handled by returning n or n*d in those cases, but i don't see such support as worth the performance hit
    /// ESPECIALLY when INT_MIN/-1 is an invalid op anyway! (so really only real op is unsupported)
    int32_t sh,smd,smn,de;
    int64_t ad=labs((int64_t)d);
    sh=cvm_po2_lt(ad);
    int64_t m=((((int64_t)1)<<(0x20|sh))/ad)-((((int64_t)1)<<(0x20|sh))-1);///subtract 2^32 (then add n later before right shifting 32) to ensure multiplier stays withing s32 range
    smn=n>>31;///divisor sign mask (all bits)
    smd=d>>31;///divisor sign mask (all bits)
    //de=(((uint32_t)n)>>31) + sm;
    assert((((int64_t)0x7FFFFFFFFFFFFFFF)/labs(d)) > labs(n));

    m*=n;
    m&=(((int64_t)1)<<(0x20|sh))-1;
    m*=d;
//    return m>>(0x20|sh);///xor and sub replaces multiplication of sign
    return (((m>>(0x20|sh))-(smn&(d)))^smd)-smn;///xor and sub replaces multiplication of sign
//    return (((m>>(0x20|sh))-(smn&(d)))^smd);///xor and sub replaces multiplication of sign
///this approav
}

static inline int32_t test_mod_3(int32_t n,int32_t d)
{
    int32_t sh,smd,smn,de;
    int64_t ad=labs((int64_t)d);
    sh=cvm_po2_gte(ad);
//    int64_t m=((((int64_t)1)<<(0x1F+sh))/ad);//-((((int64_t)1)<<(0x1F+sh))-1);
    int64_t m=((((int64_t)1)<<(0x20+sh)))/ad;// -((((int64_t)1)<<(0x20+sh))-1);
//    smn=n>>31;///divisor sign mask (all bits)
//    smd=d>>31;///divisor sign mask (all bits)

    m*=n;
    m+=(((int64_t)1)<<(0x20))-1;
    m&=(((int64_t)1)<<(0x20+sh))-1;
    m>>=sh;
    m*=d;
    return (m>>0x20);
}

///positive n only
static inline int32_t test_mod_pos(int32_t n,int32_t d)
{
    int32_t sh,smd,de;
    int64_t smn=n;
    smn>>=63;
    uint64_t ad=labs((int64_t)d);
    sh=cvm_po2_gte(ad);
//    sh=cvm_po2_lt(ad);
//    int64_t m=((((int64_t)1)<<(0x1F+sh))/ad);//-((((int64_t)1)<<(0x1F+sh))-1);
    uint64_t tmp=((((uint64_t)1)<<(0x20+sh)))/ad;// -((((int64_t)1)<<(0x20+sh))-1);
    smn=n>>31;///numerator sign mask (all bits)
//    smd=d>>31;///divisor sign mask (all bits)
    int64_t m=tmp;

    m*=n;
    m+=(((int64_t)1)<<(0x20))-1;
    m&=(((int64_t)1)<<(0x20+sh))-1;

    m>>=sh;
    m*=d;

    //m^=smn;

//    return (m>>0x20);
    return (m>>0x20);
}

static inline int32_t test_mod_4(int32_t n,int32_t d)
{
    int32_t sh,smd,de,smn;
    uint64_t ad=labs((int64_t)d);
    sh=cvm_po2_gte(ad);
//    sh=cvm_po2_lt(ad);
//    int64_t m=((((int64_t)1)<<(0x1F+sh))/ad);//-((((int64_t)1)<<(0x1F+sh))-1);
    uint64_t tmp=((((uint64_t)1)<<(0x20+sh)))/ad;// -((((int64_t)1)<<(0x20+sh))-1);
    smn=n>>31;///numerator sign mask (all bits)
//    smd=d>>31;///divisor sign mask (all bits)
    int64_t m=tmp;

    m*=(n^smn)-smn;
//    m*=labs(n);
    m+=(((int64_t)1)<<(0x20))-1;
    m&=(((int64_t)1)<<(0x20+sh))-1;

    m>>=sh;
    m*=d;

    //m^=smn;
    m=((m>>0x20)^smn)-smn;

//    return (m>>0x20)*smn;
}

static inline int32_t test_mod(int32_t n,int32_t d)
{
	///there's almost certainly a way to optimise this further...
    int32_t sh,smd,de,smn;
    uint64_t ad=labs((int64_t)d);
    sh=cvm_po2_gte(ad);
//    sh=cvm_po2_lt(ad);
//    int64_t m=((((int64_t)1)<<(0x1F+sh))/ad);//-((((int64_t)1)<<(0x1F+sh))-1);
    uint64_t tmp=((((uint64_t)1)<<(0x20+sh)))/ad;// -((((int64_t)1)<<(0x20+sh))-1);
    smn=n>>31;///numerator sign mask (all bits)
    smd=d>>31;///divisor sign mask (all bits)
    int64_t m=tmp;

    m*=((n^smn)-smn);

    m+=(((int64_t)1)<<(0x20))-1;
    m&=(((int64_t)1)<<(0x20+sh))-1;

    m>>=sh;
    m*=d;

    m=((m>>0x20)^smn^smd)-smn;
}
