//
//  qfloat.hpp
//  Qsynth
//
//  Created by Marek on 3/15/15.
//
//

#pragma once

#include <stdint.h>

#ifndef PI
#	define PI 3.141592653589793
#endif

#ifndef TWO_PI
#	define TWO_PI 6.28318530717959
#endif


typedef int32_t qfloat;


// max is 128
//static const qfloat qfloat_1 = 0x01000000; /*!< fix16_t value of 1 */
//#define Q_SHIFT 24

// max is 2048
static const qfloat qfloat_1 = 0x00100000; /*!< fix16_t value of 1 */
static const qfloat qfloat_m1 = -qfloat_1;
#define Q_SHIFT 20

//static const qfloat qfloat_1 = 0x00010000; /*!< fix16_t value of 1 */
//#define Q_SHIFT 16

#define QFLOAT_SINLUT_SIZE 128


qfloat QFLOAT(float inp);
float Q_FLOAT(qfloat fl);
int Q_INT(qfloat inp);

qfloat qmul(qfloat a, qfloat b);
qfloat qdiv(qfloat a, qfloat b);

// returns a value between -1 and 1 (as a qfloat)
qfloat qranduf();

static inline qfloat qfloat_from_int(int a)     { return a * qfloat_1; }


qfloat qfloat_sin(qfloat phase);
qfloat qfloat_sin_interp(qfloat phase);

void qinit();


extern qfloat QPI;
extern qfloat Q2PI;
extern qfloat QTWO_PI_OVER_SR;

qfloat qexp(qfloat inp);


const char *qfloat_to_binary_string(qfloat inp);