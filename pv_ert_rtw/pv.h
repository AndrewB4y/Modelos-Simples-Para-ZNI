/*
 * File: pv.h
 *
 * Code generated for Simulink model 'pv'.
 *
 * Model version                  : 1.17
 * Simulink Coder version         : 8.14 (R2018a) 06-Feb-2018
 * C/C++ source code generated on : Mon Mar  4 15:44:02 2019
 *
 * Target selection: ert.tlc
 * Embedded hardware selection: Intel->x86-64 (Windows64)
 * Code generation objectives: Unspecified
 * Validation result: Not run
 */

#ifndef RTW_HEADER_pv_h_
#define RTW_HEADER_pv_h_
#include <string.h>
#ifndef pv_COMMON_INCLUDES_
# define pv_COMMON_INCLUDES_
#include "rtwtypes.h"
#include "rtw_continuous.h"
#include "rtw_solver.h"
#endif                                 /* pv_COMMON_INCLUDES_ */

#include "pv_types.h"

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
  real_T Sum7;                         /* '<Root>/Sum7' */
} B_pv_T;

/* Continuous states (default storage) */
typedef struct {
  real_T Integrator_CSTATE;            /* '<Root>/Integrator' */
} X_pv_T;

/* State derivatives (default storage) */
typedef struct {
  real_T Integrator_CSTATE;            /* '<Root>/Integrator' */
} XDot_pv_T;

/* State disabled  */
typedef struct {
  boolean_T Integrator_CSTATE;         /* '<Root>/Integrator' */
} XDis_pv_T;

#ifndef ODE4_INTG
#define ODE4_INTG

/* ODE4 Integration Data */
typedef struct {
  real_T *y;                           /* output */
  real_T *f[4];                        /* derivatives */
} ODE4_IntgData;

#endif

/* Constant parameters (default storage) */
typedef struct {
  /* Expression: [0 10 200 500 800 1000 1140 1150 1200]
   * Referenced by: '<Root>/1-D Lookup Table2'
   */
  real_T uDLookupTable2_tableData[9];

  /* Expression: [0 10 20 30 40 50 70 80 100]
   * Referenced by: '<Root>/1-D Lookup Table2'
   */
  real_T uDLookupTable2_bp01Data[9];
} ConstP_pv_T;

/* External inputs (root inport signals with default storage) */
typedef struct {
  real_T Ipv;                          /* '<Root>/Ipv' */
  real_T Iinv;                         /* '<Root>/Iinv' */
} ExtU_pv_T;

/* External outputs (root outports fed by signals with default storage) */
typedef struct {
  real_T soc;                          /* '<Root>/soc' */
  real_T Vdc;                          /* '<Root>/Vdc' */
} ExtY_pv_T;

/* Real-time Model Data Structure */
struct tag_RTM_pv_T {
  const char_T *errorStatus;
  RTWSolverInfo solverInfo;
  X_pv_T *contStates;
  int_T *periodicContStateIndices;
  real_T *periodicContStateRanges;
  real_T *derivs;
  boolean_T *contStateDisabled;
  boolean_T zCCacheNeedsReset;
  boolean_T derivCacheNeedsReset;
  boolean_T CTOutputIncnstWithState;
  real_T odeY[1];
  real_T odeF[4][1];
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
extern B_pv_T pv_B;

/* Continuous states (default storage) */
extern X_pv_T pv_X;

/* External inputs (root inport signals with default storage) */
extern ExtU_pv_T pv_U;

/* External outputs (root outports fed by signals with default storage) */
extern ExtY_pv_T pv_Y;

/* Constant parameters (default storage) */
extern const ConstP_pv_T pv_ConstP;

/* Model entry point functions */
extern void pv_initialize(void);
extern void pv_step(void);
extern void pv_terminate(void);

/*____Funciones Get y Set ________*/

extern void set_Idc_PV(double);
extern void set_Iinv_PV(double);
extern double get_Vdc(void);
extern double get_SOC(void);

/* Real-time Model object */
extern RT_MODEL_pv_T *const pv_M;



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
 * '<Root>' : 'pv'
 * '<S1>'   : 'pv/MATLAB Function SOC '
 */
#endif                                 /* RTW_HEADER_pv_h_ */

/*
 * File trailer for generated code.
 *
 * [EOF]
 */
