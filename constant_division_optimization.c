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

/// not bothering with includes or allowing platform support

static inline uint32_t cvm_po2_lt(uint32_t v){ return __bsrd(v-1); }

static inline int32_t test_div_1(int32_t n,int32_t d)
{
    ///assert(n!=0x80000000 && d!=0x80000000);
    uint32_t s;
    int32_t ad=abs(d);
    s=cvm_po2_lt(ad);
    s+=32;
    int64_t m=((((int64_t)1)<<s)/ad)+1;
    //printf("%d %lx / %lx = %lx : * %lx = %lx => %lx %zu\n",s,(((int64_t)1)<<s),(uint64_t)ad,m,n,m*n,m*n>>s,sizeof(ad));
    m*=n;
    int32_t sb=d>>31;
    sb|=1;
    return ((m>>s)+(n<0))*sb;
}

static inline int32_t test_div_2(int32_t n,int32_t d)
{
    ///assert(n!=0x80000000 && d!=0x80000000);
    uint32_t s;
    //d+=d==0x80000000;
    int32_t ad=abs(d);
    s=cvm_po2_lt(ad);
    s+=32;
    int64_t m=((((int64_t)1)<<s)/ad)+1;
    //printf("%d %lx / %lx = %lx : * %lx = %lx => %lx %zu\n",s,(((int64_t)1)<<s),(uint64_t)ad,m,n,m*n,m*n>>s,sizeof(ad));
    m*=n;
    int32_t sb=(d>>31)|1;///sign bit
    n=(n>>31)&1;///can be done in single op, as intrinsics has specialised ops for right shift ignoring type (always fill 0)
    return ((m>>s)+n)*sb;
    ///sign bit can be handled w/o multiplication, abuse 2's compliment (sub 1 and swap top bit if result needs to be negated)
}

static inline int32_t test_div_3(int32_t n,int32_t d)
{
    int32_t s,sm,c;
    //assert(d!=0x80000000);///failure case!
    //d+=d==0x80000000;/// can be used to "fix" division by INT_MIN in all but 1 case: (INT_min+1)/INT_MIN --> 1 rather than 0
    int64_t ad=abs((int64_t)d);
    s=cvm_po2_lt(ad);
    s+=32;
    int64_t m=((((int64_t)1)<<s)/ad)+1;
    //if(m>2147483649)printf("%lld %d %lld\n",m,s,ad);
    //printf("%d %lx / %lx = %lx : * %lx = %lx => %lx %zu\n",s,(((int64_t)1)<<s),(uint64_t)ad,m,n,m*n,m*n>>s,sizeof(ad));
    m*=n;
    sm=d>>31;///divisor sign mask (all bits)
    c=(((uint32_t)n)>>31) - (sm&1);
    return ((m>>s)+c)^sm;///xor and sub replaces multiplication of sign
    ///sign bit can be handled w/o multiplication, abuse 2's compliment (sub 1 and swap top bit if result needs to be negated)
}

///all above variants (unacceptably) generate multipliers greater than 2^31
/// pretty confident this one is it, just works
static inline int32_t test_div_4(int32_t n,int32_t d)
{
    ///will produce undefined behaviour when (d==1 or d==-1) and n==INT_MIN , these cases COULD be handled by returning n or n*d in those cases, but i don't see such support as worth the performance hit
    /// ESPECIALLY when INT_MIN/-1 is an invalid op anyway! (so really only real op is unsupported)
    int32_t sh,sm,de;
    int64_t ad=labs((int64_t)d);
    sh=cvm_po2_lt(ad);
    int64_t m=((((int64_t)1)<<(sh|0x20))/ad)-(((int64_t)1)<<32)+1;///subtract 2^32 (then add n later before right shifting 32) to ensure multiplier stays withing s32 range
//    if((m<-2147483648)||(m>2147483647))printf("%lld %d %d %lld\n",m,s,d,ad);
//    assert(m>=-2147483648);
//    assert(m<= 2147483647);
    sm=d>>31;///divisor sign mask (all bits)
    de=(((uint32_t)n)>>31) - (sm&1);
    m*=n;
//    int64_t tt=(m>>32)+n;
//    if((tt<-2147483648)||(tt>2147483646))printf("%lld %lld %d %d\n",m,tt,d,sh);
//    assert(tt>=-2147483648);
//    assert(tt<= 2147483647);
    return ((((m>>32)+n)>>sh)+de)^sm;///xor and sub replaces multiplication of sign
    ///sign bit can be handled w/o multiplication, abuse 2's compliment (sub 1 and swap top bit if result needs to be negated)
}

static inline int32_t test_div(int32_t n,int32_t d)
{
    ///will produce undefined behaviour when (d==1 or d==-1) and n==INT_MIN , these cases COULD be handled by returning n or n*d in those cases, but i don't see such support as worth the performance hit
    /// ESPECIALLY when INT_MIN/-1 is an invalid op anyway! (so really only real op is unsupported)
    int32_t sh,sm,de;
    int64_t ad=labs((int64_t)d);
    sh=cvm_po2_lt(ad);
    int64_t m=((((int64_t)1)<<(sh|0x20))/ad)-(((int64_t)1)<<32)+1;///subtract 2^32 (then add n later before right shifting 32) to ensure multiplier stays withing s32 range
    sm=d>>31;///divisor sign mask (all bits)
    de=(((uint32_t)n)>>31) + sm;//sm is just -1 when sign bit is 1 and 0 otherwise (and we want to subtract 1 here when sign is negative)
    m*=n;
    return ((((m>>32)+n)>>sh)+de)^sm;///xor and sub replaces multiplication of sign
    ///sign bit can be handled w/o multiplication, abuse 2's compliment (sub 1 and swap top bit if result needs to be negated)
}
