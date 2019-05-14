/*
 * File: pv.c
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

#include "pv.h"
#include "pv_private.h"

/* Block signals (default storage) */
B_pv_T pv_B;

/* Continuous states */
X_pv_T pv_X;

/* External inputs (root inport signals with default storage) */
ExtU_pv_T pv_U;

/* External outputs (root outports fed by signals with default storage) */
ExtY_pv_T pv_Y;

/* Real-time model */
RT_MODEL_pv_T pv_M_;
RT_MODEL_pv_T *const pv_M = &pv_M_;

//-------Funciones para asignar entradas-------------

void set_Idc_PV(double ipv){
  //printf("La accion 1 es: %3.2f \n",V_bio);
  pv_U.Ipv=ipv;
  //printf("El voltaje es: %3.2f \n",BIO_csi_U.V_bio);
}

void set_Iinv_PV(double iinv){
  //printf("La accion 1 es: %3.2f \n",V_bio);
  pv_U.Iinv=iinv;
  //printf("El voltaje es: %3.2f \n",BIO_csi_U.V_bio);
}


//-----Funciones para obtener valores de las salidas-------

double get_Vdc(void){
	return pv_Y.Vdc;

}

double get_SOC(void){
	return pv_Y.soc;

}

real_T look1_binlxpw(real_T u0, const real_T bp0[], const real_T table[],
                     uint32_T maxIndex)
{
  real_T frac;
  uint32_T iRght;
  uint32_T iLeft;
  uint32_T bpIdx;

  /* Lookup 1-D
     Search method: 'binary'
     Use previous index: 'off'
     Interpolation method: 'Linear point-slope'
     Extrapolation method: 'Linear'
     Use last breakpoint for index at or above upper limit: 'off'
     Remove protection against out-of-range input in generated code: 'off'
   */
  /* Prelookup - Index and Fraction
     Index Search method: 'binary'
     Extrapolation method: 'Linear'
     Use previous index: 'off'
     Use last breakpoint for index at or above upper limit: 'off'
     Remove protection against out-of-range input in generated code: 'off'
   */
  if (u0 <= bp0[0U]) {
    iLeft = 0U;
    frac = (u0 - bp0[0U]) / (bp0[1U] - bp0[0U]);
  } else if (u0 < bp0[maxIndex]) {
    /* Binary Search */
    bpIdx = maxIndex >> 1U;
    iLeft = 0U;
    iRght = maxIndex;
    while (iRght - iLeft > 1U) {
      if (u0 < bp0[bpIdx]) {
        iRght = bpIdx;
      } else {
        iLeft = bpIdx;
      }

      bpIdx = (iRght + iLeft) >> 1U;
    }

    frac = (u0 - bp0[iLeft]) / (bp0[iLeft + 1U] - bp0[iLeft]);
  } else {
    iLeft = maxIndex - 1U;
    frac = (u0 - bp0[maxIndex - 1U]) / (bp0[maxIndex] - bp0[maxIndex - 1U]);
  }

  /* Interpolation 1-D
     Interpolation method: 'Linear point-slope'
     Use last breakpoint for index at or above upper limit: 'off'
     Overflow mode: 'portable wrapping'
   */
  return (table[iLeft + 1U] - table[iLeft]) * frac + table[iLeft];
}

/*
 * This function updates continuous states using the ODE4 fixed-step
 * solver algorithm
 */
static void rt_ertODEUpdateContinuousStates(RTWSolverInfo *si )
{
  time_T t = rtsiGetT(si);
  time_T tnew = rtsiGetSolverStopTime(si);
  time_T h = rtsiGetStepSize(si);
  real_T *x = rtsiGetContStates(si);
  ODE4_IntgData *id = (ODE4_IntgData *)rtsiGetSolverData(si);
  real_T *y = id->y;
  real_T *f0 = id->f[0];
  real_T *f1 = id->f[1];
  real_T *f2 = id->f[2];
  real_T *f3 = id->f[3];
  real_T temp;
  int_T i;
  int_T nXc = 1;
  rtsiSetSimTimeStep(si,MINOR_TIME_STEP);

  /* Save the state values at time t in y, we'll use x as ynew. */
  (void) memcpy(y, x,
                (uint_T)nXc*sizeof(real_T));

  /* Assumes that rtsiSetT and ModelOutputs are up-to-date */
  /* f0 = f(t,y) */
  rtsiSetdX(si, f0);
  pv_derivatives();

  /* f1 = f(t + (h/2), y + (h/2)*f0) */
  temp = 0.5 * h;
  for (i = 0; i < nXc; i++) {
    x[i] = y[i] + (temp*f0[i]);
  }

  rtsiSetT(si, t + temp);
  rtsiSetdX(si, f1);
  pv_step();
  pv_derivatives();

  /* f2 = f(t + (h/2), y + (h/2)*f1) */
  for (i = 0; i < nXc; i++) {
    x[i] = y[i] + (temp*f1[i]);
  }

  rtsiSetdX(si, f2);
  pv_step();
  pv_derivatives();

  /* f3 = f(t + h, y + h*f2) */
  for (i = 0; i < nXc; i++) {
    x[i] = y[i] + (h*f2[i]);
  }

  rtsiSetT(si, tnew);
  rtsiSetdX(si, f3);
  pv_step();
  pv_derivatives();

  /* tnew = t + h
     ynew = y + (h/6)*(f0 + 2*f1 + 2*f2 + 2*f3) */
  temp = h / 6.0;
  for (i = 0; i < nXc; i++) {
    x[i] = y[i] + temp*(f0[i] + 2.0*f1[i] + 2.0*f2[i] + f3[i]);
  }

  rtsiSetSimTimeStep(si,MAJOR_TIME_STEP);
}

/* Model step function */
void pv_step(void)
{
  if (rtmIsMajorTimeStep(pv_M)) {
    /* set solver stop time */
    rtsiSetSolverStopTime(&pv_M->solverInfo,((pv_M->Timing.clockTick0+1)*
      pv_M->Timing.stepSize0));
  }                                    /* end MajorTimeStep */

  /* Update absolute time of base rate at minor time step */
  if (rtmIsMinorTimeStep(pv_M)) {
    pv_M->Timing.t[0] = rtsiGetT(&pv_M->solverInfo);
  }

  /* Integrator: '<Root>/Integrator' */
  /* Limited  Integrator  */
  if (pv_X.Integrator_CSTATE >= 1.0) {
    pv_X.Integrator_CSTATE = 1.0;
  } else {
    if (pv_X.Integrator_CSTATE <= 0.0) {
      pv_X.Integrator_CSTATE = 0.0;
    }
  }

  /* MATLAB Function: '<Root>/MATLAB Function SOC ' incorporates:
   *  Integrator: '<Root>/Integrator'
   */
  pv_Y.soc = -0.03536 * pv_X.Integrator_CSTATE + 100.0;

  /* Outport: '<Root>/Vdc' incorporates:
   *  Gain: '<Root>/Gain4'
   *  Lookup_n-D: '<Root>/1-D Lookup Table2'
   */
  pv_Y.Vdc = 2.0 * look1_binlxpw(pv_Y.soc, pv_ConstP.uDLookupTable2_bp01Data,
    pv_ConstP.uDLookupTable2_tableData, 8U);

  /* Sum: '<Root>/Sum7' incorporates:
   *  Inport: '<Root>/Iinv'
   *  Inport: '<Root>/Ipv'
   */
  pv_B.Sum7 = pv_U.Iinv + pv_U.Ipv;
  if (rtmIsMajorTimeStep(pv_M)) {
    rt_ertODEUpdateContinuousStates(&pv_M->solverInfo);

    /* Update absolute time for base rate */
    /* The "clockTick0" counts the number of times the code of this task has
     * been executed. The absolute time is the multiplication of "clockTick0"
     * and "Timing.stepSize0". Size of "clockTick0" ensures timer will not
     * overflow during the application lifespan selected.
     */
    ++pv_M->Timing.clockTick0;
    pv_M->Timing.t[0] = rtsiGetSolverStopTime(&pv_M->solverInfo);

    {
      /* Update absolute timer for sample time: [0.0001s, 0.0s] */
      /* The "clockTick1" counts the number of times the code of this task has
       * been executed. The resolution of this integer timer is 0.0001, which is the step size
       * of the task. Size of "clockTick1" ensures timer will not overflow during the
       * application lifespan selected.
       */
      pv_M->Timing.clockTick1++;
    }
  }                                    /* end MajorTimeStep */
}

/* Derivatives for root system: '<Root>' */
void pv_derivatives(void)
{
  boolean_T lsat;
  boolean_T usat;
  XDot_pv_T *_rtXdot;
  _rtXdot = ((XDot_pv_T *) pv_M->derivs);

  /* Derivatives for Integrator: '<Root>/Integrator' */
  lsat = (pv_X.Integrator_CSTATE <= 0.0);
  usat = (pv_X.Integrator_CSTATE >= 1.0);
  if (((!lsat) && (!usat)) || (lsat && (pv_B.Sum7 > 0.0)) || (usat && (pv_B.Sum7
        < 0.0))) {
    _rtXdot->Integrator_CSTATE = pv_B.Sum7;
  } else {
    /* in saturation */
    _rtXdot->Integrator_CSTATE = 0.0;
  }

  /* End of Derivatives for Integrator: '<Root>/Integrator' */
}

/* Model initialize function */
void pv_initialize(void)
{
  /* Registration code */

  /* initialize real-time model */
  (void) memset((void *)pv_M, 0,
                sizeof(RT_MODEL_pv_T));

  {
    /* Setup solver object */
    rtsiSetSimTimeStepPtr(&pv_M->solverInfo, &pv_M->Timing.simTimeStep);
    rtsiSetTPtr(&pv_M->solverInfo, &rtmGetTPtr(pv_M));
    rtsiSetStepSizePtr(&pv_M->solverInfo, &pv_M->Timing.stepSize0);
    rtsiSetdXPtr(&pv_M->solverInfo, &pv_M->derivs);
    rtsiSetContStatesPtr(&pv_M->solverInfo, (real_T **) &pv_M->contStates);
    rtsiSetNumContStatesPtr(&pv_M->solverInfo, &pv_M->Sizes.numContStates);
    rtsiSetNumPeriodicContStatesPtr(&pv_M->solverInfo,
      &pv_M->Sizes.numPeriodicContStates);
    rtsiSetPeriodicContStateIndicesPtr(&pv_M->solverInfo,
      &pv_M->periodicContStateIndices);
    rtsiSetPeriodicContStateRangesPtr(&pv_M->solverInfo,
      &pv_M->periodicContStateRanges);
    rtsiSetErrorStatusPtr(&pv_M->solverInfo, (&rtmGetErrorStatus(pv_M)));
    rtsiSetRTModelPtr(&pv_M->solverInfo, pv_M);
  }

  rtsiSetSimTimeStep(&pv_M->solverInfo, MAJOR_TIME_STEP);
  pv_M->intgData.y = pv_M->odeY;
  pv_M->intgData.f[0] = pv_M->odeF[0];
  pv_M->intgData.f[1] = pv_M->odeF[1];
  pv_M->intgData.f[2] = pv_M->odeF[2];
  pv_M->intgData.f[3] = pv_M->odeF[3];
  pv_M->contStates = ((X_pv_T *) &pv_X);
  rtsiSetSolverData(&pv_M->solverInfo, (void *)&pv_M->intgData);
  rtsiSetSolverName(&pv_M->solverInfo,"ode4");
  rtmSetTPtr(pv_M, &pv_M->Timing.tArray[0]);
  pv_M->Timing.stepSize0 = 0.0001;

  /* block I/O */
  (void) memset(((void *) &pv_B), 0,
                sizeof(B_pv_T));

  /* states (continuous) */
  {
    (void) memset((void *)&pv_X, 0,
                  sizeof(X_pv_T));
  }

  /* external inputs */
  (void)memset((void *)&pv_U, 0, sizeof(ExtU_pv_T));

  /* external outputs */
  (void) memset((void *)&pv_Y, 0,
                sizeof(ExtY_pv_T));

  /* InitializeConditions for Integrator: '<Root>/Integrator' */
  pv_X.Integrator_CSTATE = 0.0;
}

/* Model terminate function */
void pv_terminate(void)
{
  /* (no terminate code required) */
}

/*
 * File trailer for generated code.
 *
 * [EOF]
 */
