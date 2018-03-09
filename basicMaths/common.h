#pragma once
#define FORCE_INLINE	__forceinline

#define PI	((float)3.141592654f)
#define PI2	((float)6.283185307f)
#define PI_DIV_2	((float)1.570796327f)
#define PI_DIV_4	((float)0.318309886f)

typedef int FIX16;
typedef int *FIXP16_PTR;

#define FIXP16_SHIFT	16
#define FIXP16_MAG		65536
#define FIXP16_DP_MASK	0x0000ffff;
#define FIXP16_WP_MASK	0xffff0000;
#define FIXP16_ROUND_UP	0x00008000;

#define EPSILON_E4		(1E-4)
#define	EPSILON_E5		(1E-5)
#define EPSILON_E6		(1E-6)

#define MAX(a, b)	(((a) > (b)) ? (a) : (b))
#define MIN(a, b)	(((a) < (b)) ? (a) : (b))

#define SWAP(a, b, t) { t=a; a=b; b=t; }

#define DEG_TO_RAD(ang)		((ang)*PI/180.0f)
#define RAD_TO_DEG(rads)	((rads)*180.0f/PI)

#define RAND_RANGE(x, y)	((x) + (rand()%((y) - (x) + 1)))

FORCE_INLINE float I_fsel( float comparand, float valGE, float valLT )
{
	return comparand >= 0 ? valGE : valLT;
}

#define MAX_PRINT_MSG	1024
void Com_Printf( const char* fmt, ... );

#define SET_BIT(va, b) ( va |= b )
#define RESET_BIT(va, b) ( va ^= b )
