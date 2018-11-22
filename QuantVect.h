#pragma once


// Here are the three implementations for our vectors
// QuantVect<N> is x87-based, parametrized on the vector size

#pragma inline_depth(255)

// here are the generic x87 vector operations unrolled using the 
// template recursion trick
template<int N> struct QuantVectOp{
	__forceinline	 static float Distance(const float *pfa, const float *pfb) 
	{
		float fd = *pfa - *pfb;
		return fd * fd + QuantVectOp<N-1>::Distance(pfa+1, pfb+1);
	}
	__forceinline  static void Adjust(float *pfCur, const float *pfGoal, float fAlpha) 
	{
		*pfCur = *pfCur + fAlpha * (*pfGoal - *pfCur);
		QuantVectOp<N-1>::Adjust(pfCur+1, pfGoal+1, fAlpha);
	}
	__forceinline  static void Add(float *pfa, const float *pfb) 
	{
		*pfa += *pfb;
		QuantVectOp<N-1>::Add(pfa+1, pfb+1);
	} 
	__forceinline  static void Sub(float *pfa, const float *pfb) 
	{
		*pfa -= *pfb;
		QuantVectOp<N-1>::Sub(pfa+1, pfb+1);
	}
	__forceinline  static void Mul(float *pf, const float f)
	{
		*pf *= f;
		QuantVectOp<N-1>::Mul(pf+1, f);
	}
};

// terminating the recursion...
template<     > struct QuantVectOp<0>{
	__forceinline static float Distance(const float *pfa, const float *pfb) 
	{ return 0; }
	__forceinline static void Adjust(float *pfCur, const float *pfGoal, float fAlpha) 
	{	}
	__forceinline static void Add(float *pfa, const float *pfb) 
	{	}
	__forceinline static void Sub(float *pfa, const float *pfb) 
	{	}
	__forceinline static void Mul(float *pf, const float f)
	{ }
};

// the plain old x87 vector, parametrized on vector size
template<int N>
struct QuantVect {
	float data[N];
	QuantVect() 
	{	
	
	}
	QuantVect(float f) {
		for(int i=0; i<N; i++) {
			data[i] = (float)f;
		}
	}
	QuantVect(int n) {
		for(int i=0; i<N;	i++) {
			data[i] = (float)n;
		}
	}

	QuantVect<N>& operator+=(const QuantVect<N>& other) {
		QuantVectOp<N>::Add(&data[0], &other.data[0]);
		return *this;
	}
	QuantVect<N>& operator-=(const QuantVect<N>& other) {
		QuantVectOp<N>::Sub(&data[0], &other.data[0]);
		return *this;
	}
	QuantVect<N>& operator*=(const float f) {
		QuantVectOp<N>::Mul(&data[0], f);
		return *this;
	}

	BOOL operator >(QuantVect<N>& p_other) {
		for(int i= 0; i < N; ++i)
		{
			if(data[i] == p_other.data[i])
			{
				if(i == N)
					return FALSE;
			}
			else if(data[i] > p_other.data[i])
				return TRUE;

			return FALSE;
		}
	}
};

template<int N>
__forceinline void Adjust(QuantVect<N>& cur, const QuantVect<N>& goal, float fAlpha)
{
	QuantVectOp<N>::Adjust(&cur.data[0], &goal.data[0], fAlpha);
}

template<int N>
__forceinline float Distance(const QuantVect<N>& a, const QuantVect<N>& b)
{
	int n = N;
	float fDist = QuantVectOp<N>::Distance(&a.data[0], &b.data[0]);
	return fDist;
}

// just for debugging purposes...
template<int N>
void Print(QuantVect<N>& v)
{
	for(int i=0; i<N; i++) {
		printf("%1.2f ", v.data[i]);
	}
	printf("\n");
}

typedef QuantVect<3>   TQuant_Palette; // the actual dimensionality we'll use
typedef QuantVect<45>   TQuant_Screen; // the actual dimensionality we'll use
