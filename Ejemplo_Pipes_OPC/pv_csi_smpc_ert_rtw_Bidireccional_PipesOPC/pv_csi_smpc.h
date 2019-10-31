/*
 * File: pv_csi_smpc.h
 *
 * Code generated for Simulink model 'pv_csi_smpc'.
 *
 * Model version                  : 1.12
 * Simulink Coder version         : 8.14 (R2018a) 06-Feb-2018
 * C/C++ source code generated on : Thu Apr 11 08:32:16 2019
 *
 * Target selection: ert.tlc
 * Embedded hardware selection: Intel->x86-64 (Windows64)
 * Code generation objectives: Unspecified
 * Validation result: Not run
 */

#ifndef RTW_HEADER_pv_csi_smpc_h_
#define RTW_HEADER_pv_csi_smpc_h_
#include <math.h>
#include <string.h>
#ifndef pv_csi_smpc_COMMON_INCLUDES_
# define pv_csi_smpc_COMMON_INCLUDES_
#include "rtwtypes.h"
#include "rtw_continuous.h"
#include "rtw_solver.h"
#endif                                 /* pv_csi_smpc_COMMON_INCLUDES_ */

#include "pv_csi_smpc_types.h"
#include "rtGetInf.h"
#include "rt_nonfinite.h"

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
  real_T Delay90zV;                    /* '<S1>/Delay90(z)V' */
  real_T Gain1;                        /* '<Root>/Gain1' */
  real_T Sum4;                         /* '<Root>/Sum4' */
  real_T Delay90zI;                    /* '<S1>/Delay90(z)I' */
  real_T Sum7;                         /* '<Root>/Sum7' */
  real_T p;                            /* '<S1>/PQ_singlePhase' */
  real_T q;                            /* '<S1>/PQ_singlePhase' */
} B_pv_csi_smpc_T;

/* Block states (default storage) for system '<Root>' */
typedef struct {
  real_T Delay90zV_states;             /* '<S1>/Delay90(z)V' */
  real_T PRz_states[2];                /* '<S1>/PR(z)' */
  real_T Delay90zI_states;             /* '<S1>/Delay90(z)I' */
  real_T Delay90zV_tmp;                /* '<S1>/Delay90(z)V' */
  real_T PRz_tmp;                      /* '<S1>/PR(z)' */
  real_T Delay90zI_tmp;                /* '<S1>/Delay90(z)I' */
  real_T TimeStampA;                   /* '<Root>/Derivative1' */
  real_T LastUAtTimeA;                 /* '<Root>/Derivative1' */
  real_T TimeStampB;                   /* '<Root>/Derivative1' */
  real_T LastUAtTimeB;                 /* '<Root>/Derivative1' */
} DW_pv_csi_smpc_T;

/* Continuous states (default storage) */
typedef struct {
  real_T Integrator_CSTATE;            /* '<Root>/Integrator' */
  real_T VSIlineZ_CSTATE;              /* '<Root>/VSI  line Z' */
  real_T Hw2_CSTATE[4];                /* '<S1>/Hw2' */
  real_T Hw1_CSTATE[4];                /* '<S1>/Hw1' */
  real_T Hw6_CSTATE[4];                /* '<S1>/Hw6' */
} X_pv_csi_smpc_T;

/* State derivatives (default storage) */
typedef struct {
  real_T Integrator_CSTATE;            /* '<Root>/Integrator' */
  real_T VSIlineZ_CSTATE;              /* '<Root>/VSI  line Z' */
  real_T Hw2_CSTATE[4];                /* '<S1>/Hw2' */
  real_T Hw1_CSTATE[4];                /* '<S1>/Hw1' */
  real_T Hw6_CSTATE[4];                /* '<S1>/Hw6' */
} XDot_pv_csi_smpc_T;

/* State disabled  */
typedef struct {
  boolean_T Integrator_CSTATE;         /* '<Root>/Integrator' */
  boolean_T VSIlineZ_CSTATE;           /* '<Root>/VSI  line Z' */
  boolean_T Hw2_CSTATE[4];             /* '<S1>/Hw2' */
  boolean_T Hw1_CSTATE[4];             /* '<S1>/Hw1' */
  boolean_T Hw6_CSTATE[4];             /* '<S1>/Hw6' */
} XDis_pv_csi_smpc_T;

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
} ConstP_pv_csi_smpc_T;

/* External inputs (root inport signals with default storage) */
typedef struct {
  real_T Ipv;                          /* '<Root>/Ipv' */
  real_T Vload;                        /* '<Root>/Vload' */
  real_T Prefd;                        /* '<Root>/Prefd' */
  real_T Qrefd;                        /* '<Root>/Qrefd' */
} ExtU_pv_csi_smpc_T;

/* External outputs (root outports fed by signals with default storage) */
typedef struct {
  real_T soc;                          /* '<Root>/soc' */
  real_T i3;                           /* '<Root>/i3' */
  real_T duty_cycle;                   /* '<Root>/duty_cycle' */
  real_T qdie_meas;                    /* '<Root>/qdie_meas' */
  real_T pdie_meas;                    /* '<Root>/pdie_meas' */
} ExtY_pv_csi_smpc_T;

/* Real-time Model Data Structure */
struct tag_RTM_pv_csi_smpc_T {
  const char_T *errorStatus;
  RTWSolverInfo solverInfo;
  X_pv_csi_smpc_T *contStates;
  int_T *periodicContStateIndices;
  real_T *periodicContStateRanges;
  real_T *derivs;
  boolean_T *contStateDisabled;
  boolean_T zCCacheNeedsReset;
  boolean_T derivCacheNeedsReset;
  boolean_T CTOutputIncnstWithState;
  real_T odeY[14];
  real_T odeF[4][14];
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
extern B_pv_csi_smpc_T pv_csi_smpc_B;

/* Continuous states (default storage) */
extern X_pv_csi_smpc_T pv_csi_smpc_X;

/* Block states (default storage) */
extern DW_pv_csi_smpc_T pv_csi_smpc_DW;

/* External inputs (root inport signals with default storage) */
extern ExtU_pv_csi_smpc_T pv_csi_smpc_U;

/* External outputs (root outports fed by signals with default storage) */
extern ExtY_pv_csi_smpc_T pv_csi_smpc_Y;

/* Constant parameters (default storage) */
extern const ConstP_pv_csi_smpc_T pv_csi_smpc_ConstP;

/* Model entry point functions */
extern void pv_csi_smpc_initialize(void);
extern void pv_csi_smpc_step(void);
extern void pv_csi_smpc_terminate(void);

//-------Funciones para asignar entradas-------------
extern void set_Idc_PV(double);
extern void set_Vload(double);
extern void set_Pref(double);
extern void set_Qref(double);
//-----Funciones para obtener valores de las salidas-------
extern double get_I_pv(void);
extern double get_SOC(void);
extern double get_duty_cycle(void);
extern double get_Pm(void);
extern double get_Qm(void);

/* Real-time Model object */
extern RT_MODEL_pv_csi_smpc_T *const pv_csi_smpc_M;

/*-
 * These blocks were eliminated from the model due to optimizations:
 *
 * Block '<S1>/Scope' : Unused code path elimination
 * Block '<S1>/Scope1' : Unused code path elimination
 */

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
 * '<Root>' : 'pv_csi_smpc'
 * '<S1>'   : 'pv_csi_smpc/CSI_Control_BIO1'
 * '<S2>'   : 'pv_csi_smpc/MATLAB Function SOC '
 * '<S3>'   : 'pv_csi_smpc/CSI_Control_BIO1/PQ2Iab_PH1'
 * '<S4>'   : 'pv_csi_smpc/CSI_Control_BIO1/PQ_singlePhase'
 * '<S5>'   : 'pv_csi_smpc/CSI_Control_BIO1/PR Controller_0_ref 1'
 * '<S6>'   : 'pv_csi_smpc/CSI_Control_BIO1/PR Controller_0_ref 1/Steper6'
 */
#endif                                 /* RTW_HEADER_pv_csi_smpc_h_ */

/*
 * File trailer for generated code.
 *
 * [EOF]
 */
