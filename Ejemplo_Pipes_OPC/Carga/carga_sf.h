/*
 * File: carga_sf.h
 *
 * Code generated for Simulink model 'carga_sf'.
 *
 * Model version                  : 1.66
 * Simulink Coder version         : 8.12 (R2017a) 16-Feb-2017
 * C/C++ source code generated on : Wed Aug 15 11:49:54 2018
 *
 * Target selection: ert.tlc
 * Embedded hardware selection: Intel->x86-64 (Windows64)
 * Code generation objectives: Unspecified
 * Validation result: Not run
 */

#ifndef RTW_HEADER_carga_sf_h_
#define RTW_HEADER_carga_sf_h_
#include <float.h>
#include <math.h>
#include <string.h>
#ifndef carga_sf_COMMON_INCLUDES_
# define carga_sf_COMMON_INCLUDES_
#include "rtwtypes.h"
#include "rtw_continuous.h"
#include "rtw_solver.h"
#endif                                 /* carga_sf_COMMON_INCLUDES_ */

#include "carga_sf_types.h"
#include "rt_defines.h"
#include "rt_nonfinite.h"
#include "rtGetInf.h"

/* Macros for accessing real-time model data structure */
#ifndef rtmGetErrorStatus
# define rtmGetErrorStatus(rtm)        ((rtm)->errorStatus)
#endif

#ifndef rtmSetErrorStatus
# define rtmSetErrorStatus(rtm, val)   ((rtm)->errorStatus = (val))
#endif

#ifndef rtmGetStopRequested
# define rtmGetStopRequested(rtm)      ((rtm)->Timing.stopRequestedFlag)
#endif

#ifndef rtmSetStopRequested
# define rtmSetStopRequested(rtm, val) ((rtm)->Timing.stopRequestedFlag = (val))
#endif

#ifndef rtmGetStopRequestedPtr
# define rtmGetStopRequestedPtr(rtm)   (&((rtm)->Timing.stopRequestedFlag))
#endif

#ifndef rtmGetT
# define rtmGetT(rtm)                  (rtmGetTPtr((rtm))[0])
#endif

/* Block signals (auto storage) */
typedef struct {
  real_T Memory;                       /* '<S7>/Memory' */
  real_T Switch;                       /* '<S7>/Switch' */
  real_T Memory_p;                     /* '<S6>/Memory' */
  real_T Switch_g;                     /* '<S6>/Switch' */
  real_T Memory_m;                     /* '<S11>/Memory' */
  real_T Switch_f;                     /* '<S11>/Switch' */
  real_T Memory_b;                     /* '<S10>/Memory' */
  real_T Switch_d;                     /* '<S10>/Switch' */
  real_T Product;                      /* '<S2>/Product' */
  real_T Product1;                     /* '<S2>/Product1' */
  real_T Sum6;                         /* '<Root>/Sum6' */
  real_T Product_m;                    /* '<S3>/Product' */
  real_T Product1_c;                   /* '<S3>/Product1' */
} B_carga_sf_T;

/* Block states (auto storage) for system '<Root>' */
typedef struct {
  real_T Memory_PreviousInput;         /* '<S7>/Memory' */
  real_T Memory_PreviousInput_p;       /* '<S6>/Memory' */
  real_T Memory_PreviousInput_a;       /* '<S11>/Memory' */
  real_T Memory_PreviousInput_pa;      /* '<S10>/Memory' */
  struct {
    real_T modelTStart;
    real_T TUbufferArea[16384];
  } TransportDelay_RWORK;              /* '<S7>/Transport Delay' */

  struct {
    real_T modelTStart;
    real_T TUbufferArea[16384];
  } TransportDelay_RWORK_d;            /* '<S6>/Transport Delay' */

  struct {
    real_T modelTStart;
    real_T TUbufferArea[16384];
  } TransportDelay_RWORK_h;            /* '<S11>/Transport Delay' */

  struct {
    real_T modelTStart;
    real_T TUbufferArea[16384];
  } TransportDelay_RWORK_dx;           /* '<S10>/Transport Delay' */

  struct {
    void *TUbufferPtrs[2];
  } TransportDelay_PWORK;              /* '<S7>/Transport Delay' */

  struct {
    void *TUbufferPtrs[2];
  } TransportDelay_PWORK_j;            /* '<S6>/Transport Delay' */

  struct {
    void *TUbufferPtrs[2];
  } TransportDelay_PWORK_p;            /* '<S11>/Transport Delay' */

  struct {
    void *TUbufferPtrs[2];
  } TransportDelay_PWORK_o;            /* '<S10>/Transport Delay' */

  struct {
    int_T Tail;
    int_T Head;
    int_T Last;
    int_T CircularBufSize;
  } TransportDelay_IWORK;              /* '<S7>/Transport Delay' */

  struct {
    int_T Tail;
    int_T Head;
    int_T Last;
    int_T CircularBufSize;
  } TransportDelay_IWORK_a;            /* '<S6>/Transport Delay' */

  struct {
    int_T Tail;
    int_T Head;
    int_T Last;
    int_T CircularBufSize;
  } TransportDelay_IWORK_n;            /* '<S11>/Transport Delay' */

  struct {
    int_T Tail;
    int_T Head;
    int_T Last;
    int_T CircularBufSize;
  } TransportDelay_IWORK_av;           /* '<S10>/Transport Delay' */
} DW_carga_sf_T;

/* Continuous states (auto storage) */
typedef struct {
  real_T integrator_CSTATE;            /* '<S7>/integrator' */
  real_T integrator_CSTATE_n;          /* '<S6>/integrator' */
  real_T integrator_CSTATE_f;          /* '<S11>/integrator' */
  real_T integrator_CSTATE_e;          /* '<S10>/integrator' */
  real_T TransferFcn3_CSTATE[2];       /* '<Root>/Transfer Fcn3' */
} X_carga_sf_T;

/* State derivatives (auto storage) */
typedef struct {
  real_T integrator_CSTATE;            /* '<S7>/integrator' */
  real_T integrator_CSTATE_n;          /* '<S6>/integrator' */
  real_T integrator_CSTATE_f;          /* '<S11>/integrator' */
  real_T integrator_CSTATE_e;          /* '<S10>/integrator' */
  real_T TransferFcn3_CSTATE[2];       /* '<Root>/Transfer Fcn3' */
} XDot_carga_sf_T;

/* State disabled  */
typedef struct {
  boolean_T integrator_CSTATE;         /* '<S7>/integrator' */
  boolean_T integrator_CSTATE_n;       /* '<S6>/integrator' */
  boolean_T integrator_CSTATE_f;       /* '<S11>/integrator' */
  boolean_T integrator_CSTATE_e;       /* '<S10>/integrator' */
  boolean_T TransferFcn3_CSTATE[2];    /* '<Root>/Transfer Fcn3' */
} XDis_carga_sf_T;

#ifndef ODE4_INTG
#define ODE4_INTG

/* ODE4 Integration Data */
typedef struct {
  real_T *y;                           /* output */
  real_T *f[4];                        /* derivatives */
} ODE4_IntgData;

#endif

/* External inputs (root inport signals with auto storage) */
typedef struct {
  real_T i1;                           /* '<Root>/i1' */
  real_T i2;                           /* '<Root>/i2' */
  real_T i3;                           /* '<Root>/i3' */
} ExtU_carga_sf_T;

/* External outputs (root outports fed by signals with auto storage) */
typedef struct {
  real_T Pm;                           /* '<Root>/Pm' */
  real_T Qm;                           /* '<Root>/Qm' */
  real_T Vload;                        /* '<Root>/Vload' */
} ExtY_carga_sf_T;

/* Real-time Model Data Structure */
struct tag_RTM_carga_sf_T {
  const char_T *errorStatus;
  RTWSolverInfo solverInfo;
  X_carga_sf_T *contStates;
  int_T *periodicContStateIndices;
  real_T *periodicContStateRanges;
  real_T *derivs;
  boolean_T *contStateDisabled;
  boolean_T zCCacheNeedsReset;
  boolean_T derivCacheNeedsReset;
  boolean_T blkStateChange;
  real_T odeY[6];
  real_T odeF[4][6];
  ODE4_IntgData intgData;

  /*
   * Sizes:
   * The following substructure contains sizes information
   * for many of the model attributes such as inputs, outputs,
   * dwork, sample times, etc.
   */
  struct {
    int_T numContStates;
    int_T numPeriodicContStates;
    int_T numSampTimes;
  } Sizes;

  /*
   * Timing:
   * The following substructure contains information regarding
   * the timing information for the model.
   */
  struct {
    uint32_T clockTick0;
    time_T stepSize0;
    uint32_T clockTick1;
    SimTimeStep simTimeStep;
    boolean_T stopRequestedFlag;
    time_T *t;
    time_T tArray[2];
  } Timing;
};

/* Block signals (auto storage) */
extern B_carga_sf_T carga_sf_B;

/* Continuous states (auto storage) */
extern X_carga_sf_T carga_sf_X;

/* Block states (auto storage) */
extern DW_carga_sf_T carga_sf_DW;

/* External inputs (root inport signals with auto storage) */
extern ExtU_carga_sf_T carga_sf_U;

/* External outputs (root outports fed by signals with auto storage) */
extern ExtY_carga_sf_T carga_sf_Y;

/* Model entry point functions */
extern void carga_sf_initialize(void);
extern void carga_sf_step(void);
extern void carga_sf_terminate(void);

//-------Funciones para asignar entradas-------------
extern void set_i1(double);
extern void set_i2(double);
extern void set_i3(double);
//-----Funciones para obtener valores de las salidas-------
extern double get_Pm(void);
extern double get_Qm(void);
extern double get_Vload(void);

/* Real-time Model object */
extern RT_MODEL_carga_sf_T *const carga_sf_M;

/*-
 * The generated code includes comments that allow you to trace directly
 * back to the appropriate location in the model.  The basic format
 * is <system>/block_name, where system is the system number (uniquely
 * assigned by Simulink) and block_name is the name of the block.
 *
 * Use the MATLAB hilite_system command to trace the generated code back
 * to the model.  For example,
 *
 * hilite_system('<S3>')    - opens system 3
 * hilite_system('<S3>/Kp') - opens and selects block Kp which resides in S3
 *
 * Here is the system hierarchy for this model
 *
 * '<Root>' : 'carga_sf'
 * '<S1>'   : 'carga_sf/Power'
 * '<S2>'   : 'carga_sf/Power/Fourier'
 * '<S3>'   : 'carga_sf/Power/Fourier1'
 * '<S4>'   : 'carga_sf/Power/Fourier/Mean'
 * '<S5>'   : 'carga_sf/Power/Fourier/Mean value1'
 * '<S6>'   : 'carga_sf/Power/Fourier/Mean/Model'
 * '<S7>'   : 'carga_sf/Power/Fourier/Mean value1/Model'
 * '<S8>'   : 'carga_sf/Power/Fourier1/Mean'
 * '<S9>'   : 'carga_sf/Power/Fourier1/Mean value1'
 * '<S10>'  : 'carga_sf/Power/Fourier1/Mean/Model'
 * '<S11>'  : 'carga_sf/Power/Fourier1/Mean value1/Model'
 */
#endif                                 /* RTW_HEADER_carga_sf_h_ */

/*
 * File trailer for generated code.
 *
 * [EOF]
 */
