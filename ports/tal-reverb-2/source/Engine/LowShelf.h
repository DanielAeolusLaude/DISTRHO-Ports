/*-----------------------------------------------------------------------------

Kunz Patrick 30.04.2007

A one pole low pass filter.

-----------------------------------------------------------------------------*/

#if !defined(__LowShelf_h)
#define __LowShelf_h

#include "Math.h"

class LowShelf 
{
public:
	int filterDecibel;

	float sampleRate;

	float a0, a1, a2;
	float b0, b1, b2;

	float x0, x1, x2;
	float     y1, y2;

	float outSample;

	float a, w0, q, s, alpha;
	float cosValue, sqrtValue;

	float dBgain, freq;

	float random;

	LowShelf(float sampleRate, int filterDecibel) 
	{
		this->sampleRate = sampleRate;
		this->filterDecibel = filterDecibel;

		a0 = a1 = a2 = 0.0f;
		b0 = b1 = b2 = 0.0f;

		x0 = x1 = x2 = 0.0f;
		y1 = y2 = 0.0f;

		q = dBgain = freq = 0.0f;

		s = 1.0f;
	}

	// gain [0..1], q[0..1], freq[0..44100]
	inline void tick(float *inSample, float freq, float q, float gain) 
	{
		gain = filterDecibel - (1.0f - gain) * filterDecibel * 2.0f;
		calcCoefficients(freq, q, gain);
		
		outSample = b0*x0 + b1*x1 + b2*x2 - a1*y1 - a2*y2;
		outSample = b0*x0 + b1*x1 + b2*x2 - a1*y1 - a2*y2;
		updateHistory(*inSample, outSample);
		*inSample = outSample;
	}

	inline void calcCoefficients(float freq, float q, float dBgain)
	{
		if (this->q != q || this->dBgain != dBgain || this->freq != freq)
		{
			this->dBgain = dBgain;
			this->q = q;
			this->freq = freq;
			w0 = 2.0f * 3.141592653589793f * freq / sampleRate;
	
			a = sqrtf(pow(10, dBgain/20.0f));
			alpha = sinf(w0)/(2.0f*q);
			q = 1.0f / sqrt((a + 1.0f/a)*(1.0f/s - 1.0f) + 2.0f);
	
			cosValue = cosf(w0);
			sqrtValue = sqrtf(a);
		}	
		b0 =         a * ((a + 1.0f) - (a - 1.0f) * cosValue + 2.0f * sqrtValue * alpha );
		b1 =  2.0f * a * ((a - 1.0f) - (a + 1.0f) * cosValue                           );
		b2 =         a * ((a + 1.0f) - (a - 1.0f) * cosValue - 2.0f * sqrtValue * alpha );
		a0 =              (a + 1.0f) + (a - 1.0f) * cosValue + 2.0f * sqrtValue * alpha;
		a1 =    - 2.0f * ((a - 1.0f) + (a + 1.0f) * cosValue                           );
		a2 =              (a + 1.0f) + (a - 1.0f) * cosValue - 2.0f * sqrtValue * alpha;

		a0 = 1.0f/a0;

		b0 *= a0;
		b1 *= a0;
		b2 *= a0;
		a1 *= a0;
		a2 *= a0;
	}

	inline void updateHistory(float inSample, float outSample)
	{
		x0 = saturate(x1, -0.08f);
		x1 = saturate(x2, 0.05f);
		x2 = saturate(inSample, 0.01f);

		y2 = y1;
		y1 = outSample;
	}

	inline float saturate(float x, float variation)
	{
		return x - 0.002f * (x + variation) * x * x;
	}
};

#endif
