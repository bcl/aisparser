%module aisparser
int pos2ddd( long, long, double *OUTPUT, double *OUTPUT);
int pos2dmm( long, long, short *OUTPUT, double *OUTPUT, short *OUTPUT, double *OUTPUT);

%{
/* Includes the header in the wrapper code */
#include "../../c/src/portable.h"
#include "../../c/src/nmea.h"
#include "../../c/src/sixbit.h"
#include "../../c/src/vdm_parse.h"
#include "../../c/src/seaway.h"
#include "../../c/src/imo.h"
#include "../../c/src/access.h"
%}

 
/* Parse the header file to generate wrappers */
%include "../../c/src/portable.h"
%include "../../c/src/nmea.h"
%include "../../c/src/sixbit.h"
%include "../../c/src/vdm_parse.h"
%include "../../c/src/seaway.h"
%include "../../c/src/imo.h"
%include "../../c/src/access.h"