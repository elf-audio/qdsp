//
//  qdsp.cpp
//  Qsynth
//
//  Created by Marek on 3/16/15.
//
//

#include "qdsp.h"


qfloat qlfo(qfloat val) {
	return qmul(qfloat_1 + val, Q05);
}
