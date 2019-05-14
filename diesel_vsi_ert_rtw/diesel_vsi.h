/*
 * File: diesel_vsi.h
 *
 * Code generated for Simulink model 'diesel_vsi'.
 *
 * Model version                  : 1.25
 * Simulink Coder version         : 8.14 (R2018a) 06-Feb-2018
 * C/C++ source code generated on : Tue May 14 08:17:43 2019
 *
 * Target selection: ert.tlc
 * Embedded hardware selection: Intel->x86-64 (Windows64)
 * Code generation objectives: Unspecified
 * Validation result: Not run
 */

#ifndef RTW_HEADER_diesel_vsi_h_
#define RTW_HEADER_diesel_vsi_h_
#include <float.h>
#include <math.h>
#include <string.h>
#ifndef diesel_vsi_COMMON_INCLUDES_
# define diesel_vsi_COMMON_INCLUDES_
#include "rtwtypes.h"
#include "rtw_continuous.h"
#include "rtw_solver.h"
#endif                                 /* diesel_vsi_COMMON_INCLUDES_ */

#include "diesel_vsi_types.h"

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

#ifndef rtmGetTPtr
# define rtmGetTPtr(rtm)               ((rtm)->Timing.t)
#endif

/* Block signals (default storage) */
typedef struct {
  real_T OutputDCVoltage;              /* '<Root>/Sum8' */
  real_T FilterCoefficient;            /* '<S2>/Filter Coefficient' */
  real_T Sum;                          /* '<S2>/Sum' */
  real_T IntegralGain;                 /* '<S2>/Integral Gain' */
  real_T Sum2;                         /* '<S1>/Sum2' */
} B_diesel_vsi_T;

/* Block states (default storage) for system '<Root>' */
typedef struct {
  struct {
    real_T modelTStart;
    real_T TUbufferArea[2048];
  } MotordeCombustin1_RWORK;           /* '<S1>/Motor de Combusti�n1' */

  struct {
    void *TUbufferPtrs[2];
  } MotordeCombustin1_PWORK;           /* '<S1>/Motor de Combusti�n1' */

  struct {
    int_T Tail;
    int_T Head;
    int_T Last;
    int_T CircularBufSize;
  } MotordeCombustin1_IWORK;           /* '<S1>/Motor de Combusti�n1' */
} DW_diesel_vsi_T;

/* Continuous states (default storage) */
typedef struct {
  real_T Actuador1_CSTATE;             /* '<S1>/Actuador1' */
  real_T TransferFcn5_CSTATE;          /* '<Root>/Transfer Fcn5' */
  real_T Dinmicaeneleje1_CSTATE;       /* '<S1>/Din�mica en el eje1' */
  real_T Filter_CSTATE;                /* '<S2>/Filter' */
  real_T Integrator_CSTATE;            /* '<S2>/Integrator' */
} X_diesel_vsi_T;

/* State derivatives (default storage) */
typedef struct {
  real_T Actuador1_CSTATE;             /* '<S1>/Actuador1' */
  real_T TransferFcn5_CSTATE;          /* '<Root>/Transfer Fcn5' */
  real_T Dinmicaeneleje1_CSTATE;       /* '<S1>/Din�mica en el eje1' */
  real_T Filter_CSTATE;                /* '<S2>/Filter' */
  real_T Integrator_CSTATE;            /* '<S2>/Integrator' */
} XDot_diesel_vsi_T;

/* State disabled  */
typedef struct {
  boolean_T Actuador1_CSTATE;          /* '<S1>/Actuador1' */
  boolean_T TransferFcn5_CSTATE;       /* '<Root>/Transfer Fcn5' */
  boolean_T Dinmicaeneleje1_CSTATE;    /* '<S1>/Din�mica en el eje1' */
  boolean_T Filter_CSTATE;             /* '<S2>/Filter' */
  boolean_T Integrator_CSTATE;         /* '<S2>/Integrator' */
} XDis_diesel_vsi_T;

#ifndef ODE4_INTG
#define ODE4_INTG

/* ODE4 Integration Data */
typedef struct {
  real_T *y;                           /* output */
  real_T *f[4];                        /* derivatives */
} ODE4_IntgData;

#endif

/* External inputs (root inport signals with default storage) */
typedef struct {
  real_T Vload;                        /* '<Root>/Vload' */
  real_T Vref;                         /* '<Root>/Vref' */
  real_T par;                          /* '<Root>/par' */
  real_T I2;                           /* '<Root>/I2' */
} ExtU_diesel_vsi_T;

/* External outputs (root outports fed by signals with default storage) */
typedef struct {
  real_T flujo;                        /* '<Root>/flujo' */
  real_T vdc;                          /* '<Root>/vdc' */
} ExtY_diesel_vsi_T;

/* Real-time Model Data Structure */
struct tag_RTM_diesel_vsi_T {
  const char_T *errorStatus;
  RTWSolverInfo solverInfo;
  X_diesel_vsi_T *contStates;
  int_T *periodicContStateIndices;
  real_T *periodicContStateRanges;
  real_T *derivs;
  boolean_T *contStateDisabled;
  boolean_T zCCacheNeedsReset;
  boolean_T derivCacheNeedsReset;
  boolean_T CTOutputIncnstWithState;
  real_T odeY[5];
  real_T odeF[4][5];
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

/* Block signals (default storage) */
extern B_diesel_vsi_T diesel_vsi_B;

/* Continuous states (default storage) */
extern X_diesel_vsi_T diesel_vsi_X;

/* Block states (default storage) */
extern DW_diesel_vsi_T diesel_vsi_DW;

/* External inputs (root inport signals with default storage) */
extern ExtU_diesel_vsi_T diesel_vsi_U;

/* External outputs (root outports fed by signals with default storage) */
extern ExtY_diesel_vsi_T diesel_vsi_Y;

/* Model entry point functions */
extern void diesel_vsi_initialize(void);
extern void diesel_vsi_step(void);
extern void diesel_vsi_terminate(void);

/*___________Funciones Get y Set_________*/
extern void set_Vload(double);
extern void set_par(double);
extern void set_Vref(double);
extern void set_Iinv(double);
extern double get_flujo(void);
extern double get_vdc(void);
/* Real-time Model object */
extern RT_MODEL_diesel_vsi_T *const diesel_vsi_M;



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
 * '<Root>' : 'diesel_vsi'
 * '<S1>'   : 'diesel_vsi/Diesel Generator3'
 * '<S2>'   : 'diesel_vsi/PID Controller3'
 */
#endif                                 /* RTW_HEADER_diesel_vsi_h_ */

/*
 * File trailer for generated code.
 *
 * [EOF]
 */
