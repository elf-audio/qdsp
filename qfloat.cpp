//
//  qfloat.cpp
//  Qsynth
//
//  Created by Marek on 3/15/15.
//
//

#include "qfloat.h"

#include <math.h>
#include <stdio.h>
#include <string.h>

qfloat QPI = QFLOAT(PI);
qfloat Q2PI = QFLOAT(PI*2.f);
qfloat QTWO_PI_OVER_SR = QFLOAT(PI*2.f/44100.f);
qfloat QFLOAT_PHASE_INC = QFLOAT(QFLOAT_SINLUT_SIZE / (M_PI*2.f));
qfloat Q05 = QFLOAT(0.5);

qfloat QFLOAT(float inp) {
	float temp = inp * qfloat_1;
	return (qfloat) temp;
}
float Q_FLOAT(qfloat fl) {
	return (float) fl / qfloat_1;
}


int Q_INT(qfloat inp) {
	return inp >> Q_SHIFT;
}


qfloat qmul(qfloat a, qfloat b) {
	
	int64_t m = (int64_t) a * b;
	
//	I think this does rounding
//	#define K   (1 << (Q-1))
//	m += K;
	
	
	qfloat r = m >> Q_SHIFT;
	
	return r;
}

qfloat qdiv(qfloat a, qfloat b) {
	int64_t d = (int64_t) a << Q_SHIFT;
//	I think this does rounding
//	d += b / 2;
	return d / b;
}


// this is pretty rough, uses the expansion on the wikipedia page
// for the exponential function
qfloat qexp(qfloat inp) {
	qfloat out = qfloat_1 + inp;

	qfloat x = inp;
	qfloat div = qfloat_1;
	
	for(int i = 2; i < 6; i++) {
		x = qmul(x, inp);
		// I'M NOT SURE THERE SHOULD BE QFLOAT(i) IN THE BELOW LINE
		// IS IT POSSIBLE TO DO SOMETHING FASTER?
		div = qmul(div, QFLOAT(i));
		out += qdiv(x, div);
	}
	return out;
}


qfloat qfloat_sinLUT[QFLOAT_SINLUT_SIZE];
qfloat qfloat_sawLUT[QFLOAT_SINLUT_SIZE];

#define SIN_LUT 1
#define SAW_LUT 2

void qfloat_createLUT(qfloat *lut, int type) {
	float phaseInc = (M_PI*2.0) / (float)QFLOAT_SINLUT_SIZE;
	float phase = 0;
	
	for(int i = 0; i < QFLOAT_SINLUT_SIZE; i++) {
		if(type==SIN_LUT) {
			lut[i] = QFLOAT(sin(phase));
		} else if(type==SAW_LUT) {
			float val = 0;
			for(float j = 1; j < 100.f; j++) {
				val += sin(phase * j) / j;
			}
			lut[i] = QFLOAT(val*0.6); // 0.6 to scale it
			
			

		}
		
		phase += phaseInc;
	}
}

qfloat qfloat_sin(qfloat phase) {
	qfloat pos = qmul(phase, QFLOAT_PHASE_INC);
	return qfloat_sinLUT[Q_INT(pos)];
}

qfloat qfloat_saw(qfloat phase) {
	qfloat pos = qmul(phase, QFLOAT_PHASE_INC);
	return qfloat_sawLUT[Q_INT(pos)];
}


qfloat qfloat_interpLUT(qfloat *lut, qfloat phase) {
	
	// work out offset for phase
	
	qfloat pos = qmul(phase, QFLOAT_PHASE_INC);
	
	unsigned int i = Q_INT(pos);
	unsigned int j = i + 1;
	
	if(j>=QFLOAT_SINLUT_SIZE) {
		j = 0;
	}
	
	qfloat m = pos - qfloat_from_int(i);
	
	
	qfloat m1 = qfloat_1 - m;
	
	return qmul(lut[i], m1) + qmul(lut[j], m);
}



qfloat qfloat_sin_interp(qfloat phase) {
	return qfloat_interpLUT(qfloat_sinLUT, phase);
	
}


qfloat qfloat_saw_interp(qfloat phase) {
	return qfloat_interpLUT(qfloat_sawLUT, phase);
	
}






void qinit() {
	qfloat_createLUT(qfloat_sinLUT, SIN_LUT);
	qfloat_createLUT(qfloat_sawLUT, SAW_LUT);
}












const char *qfloat_to_binary_string(qfloat inp) {
	static char b[37];
	
	b[0] = '\0';
	
	unsigned char *data = (unsigned char *)(&inp);
	int z;
	for(int i = 3; i >= 0; i--) {
		
	
		for (z = 128; z > 0; z >>= 1) {
			strcat(b, ((data[i] & z) == z) ? "1" : "0");
		}
		strcat(b, " ");
	}
	
	return b;
}



// not massively sure this works
int next = 0;
qfloat qranduf() {
	next = next * 1103515245 + 12345;
	qfloat rr = (next/65536) % 32768;
	rr %= qfloat_1 - qfloat_m1;
	rr += qfloat_m1;
	
	
	return rr;
}

qfloat qfloat_to_uint16(qfloat out) {
	float samp = Q_FLOAT(out);
	return samp*8000.0;
}