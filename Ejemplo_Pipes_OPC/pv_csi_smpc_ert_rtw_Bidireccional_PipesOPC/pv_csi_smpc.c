/*
 * File: pv_csi_smpc.c
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

#include "pv_csi_smpc.h"
#include "pv_csi_smpc_private.h"

/* Block signals (default storage) */
B_pv_csi_smpc_T pv_csi_smpc_B;

/* Continuous states */
X_pv_csi_smpc_T pv_csi_smpc_X;

/* Block states (default storage) */
DW_pv_csi_smpc_T pv_csi_smpc_DW;

/* External inputs (root inport signals with default storage) */
ExtU_pv_csi_smpc_T pv_csi_smpc_U;

/* External outputs (root outports fed by signals with default storage) */
ExtY_pv_csi_smpc_T pv_csi_smpc_Y;

/* Real-time model */
RT_MODEL_pv_csi_smpc_T pv_csi_smpc_M_;
RT_MODEL_pv_csi_smpc_T *const pv_csi_smpc_M = &pv_csi_smpc_M_;

//Funciones creadas por el programador set y get
//-------Funciones para asignar entradas-------------
void set_Idc_PV(double ipv){
	//printf("La accion 1 es: %3.2f \n",V_bio);
	pv_csi_smpc_U.Ipv=ipv;
	//printf("El voltaje es: %3.2f \n",BIO_csi_U.V_bio);
	}
	
void set_Vload(double Vload){
	pv_csi_smpc_U.Vload=Vload;
}

void set_Pref(double pr){
	//printf("La accion 1 es: %3.2f \n",V_bio);
	pv_csi_smpc_U.Prefd=pr;
	//printf("El voltaje es: %3.2f \n",BIO_csi_U.V_bio);
	}
	
void set_Qref(double qr){
	pv_csi_smpc_U.Qrefd=qr;
}

//-----Funciones para obtener valores de las salidas-------
	
double get_I_pv(void){
	return pv_csi_smpc_Y.i3;
	 
}

double get_SOC(void){
	return pv_csi_smpc_Y.soc;
	 
}

double get_duty_cycle(void) {
    return pv_csi_smpc_Y.duty_cycle; 
}

double get_Pm(void){
	return pv_csi_smpc_Y.pdie_meas;
	 
}

double get_Qm(void){
	return pv_csi_smpc_Y.qdie_meas;
	 
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
  int_T nXc = 14;
  rtsiSetSimTimeStep(si,MINOR_TIME_STEP);

  /* Save the state values at time t in y, we'll use x as ynew. */
  (void) memcpy(y, x,
                (uint_T)nXc*sizeof(real_T));

  /* Assumes that rtsiSetT and ModelOutputs are up-to-date */
  /* f0 = f(t,y) */
  rtsiSetdX(si, f0);
  pv_csi_smpc_derivatives();

  /* f1 = f(t + (h/2), y + (h/2)*f0) */
  temp = 0.5 * h;
  for (i = 0; i < nXc; i++) {
    x[i] = y[i] + (temp*f0[i]);
  }

  rtsiSetT(si, t + temp);
  rtsiSetdX(si, f1);
  pv_csi_smpc_step();
  pv_csi_smpc_derivatives();

  /* f2 = f(t + (h/2), y + (h/2)*f1) */
  for (i = 0; i < nXc; i++) {
    x[i] = y[i] + (temp*f1[i]);
  }

  rtsiSetdX(si, f2);
  pv_csi_smpc_step();
  pv_csi_smpc_derivatives();

  /* f3 = f(t + h, y + h*f2) */
  for (i = 0; i < nXc; i++) {
    x[i] = y[i] + (h*f2[i]);
  }

  rtsiSetT(si, tnew);
  rtsiSetdX(si, f3);
  pv_csi_smpc_step();
  pv_csi_smpc_derivatives();

  /* tnew = t + h
     ynew = y + (h/6)*(f0 + 2*f1 + 2*f2 + 2*f3) */
  temp = h / 6.0;
  for (i = 0; i < nXc; i++) {
    x[i] = y[i] + temp*(f0[i] + 2.0*f1[i] + 2.0*f2[i] + f3[i]);
  }

  rtsiSetSimTimeStep(si,MAJOR_TIME_STEP);
}

/* Model step function */
void pv_csi_smpc_step(void)
{
  if (rtmIsMajorTimeStep(pv_csi_smpc_M)) {
    /* set solver stop time */
    rtsiSetSolverStopTime(&pv_csi_smpc_M->solverInfo,
                          ((pv_csi_smpc_M->Timing.clockTick0+1)*
      pv_csi_smpc_M->Timing.stepSize0));
  }                                    /* end MajorTimeStep */

  /* Update absolute time of base rate at minor time step */
  if (rtmIsMinorTimeStep(pv_csi_smpc_M)) {
    pv_csi_smpc_M->Timing.t[0] = rtsiGetT(&pv_csi_smpc_M->solverInfo);
  }

  {
    real_T *lastU;
    real_T det;
    real_T VSIlineZ;

    /* Integrator: '<Root>/Integrator' */
    /* Limited  Integrator  */
    if (pv_csi_smpc_X.Integrator_CSTATE >= 1.0) {
      pv_csi_smpc_X.Integrator_CSTATE = 1.0;
    } else {
      if (pv_csi_smpc_X.Integrator_CSTATE <= 0.0) {
        pv_csi_smpc_X.Integrator_CSTATE = 0.0;
      }
    }

    /* MATLAB Function: '<Root>/MATLAB Function SOC ' incorporates:
     *  Integrator: '<Root>/Integrator'
     */
    pv_csi_smpc_Y.soc = -0.03536 * pv_csi_smpc_X.Integrator_CSTATE + 100.0;

    /* TransferFcn: '<Root>/VSI  line Z' */
    VSIlineZ = 33.333333333333336 * pv_csi_smpc_X.VSIlineZ_CSTATE;
    if (rtmIsMajorTimeStep(pv_csi_smpc_M)) {
      /* DiscreteTransferFcn: '<S1>/Delay90(z)V' incorporates:
       *  Inport: '<Root>/Vload'
       */
      pv_csi_smpc_DW.Delay90zV_tmp = pv_csi_smpc_U.Vload - -0.96364 *
        pv_csi_smpc_DW.Delay90zV_states;
      pv_csi_smpc_B.Delay90zV = -1.0364 * pv_csi_smpc_DW.Delay90zV_states +
        pv_csi_smpc_DW.Delay90zV_tmp;
    }

    /* MATLAB Function: '<S1>/PQ2Iab_PH1' incorporates:
     *  Inport: '<Root>/Prefd'
     *  Inport: '<Root>/Qrefd'
     *  Inport: '<Root>/Vload'
     */
    det = pv_csi_smpc_U.Vload * pv_csi_smpc_U.Vload + pv_csi_smpc_B.Delay90zV *
      pv_csi_smpc_B.Delay90zV;
    if (fabs(det) <= 4.94065645841247E-321) {
      det = (rtInf);
    } else {
      det = 1.0 / det;
    }

    det = det * pv_csi_smpc_U.Vload * (2.0 * pv_csi_smpc_U.Prefd) + det *
      pv_csi_smpc_B.Delay90zV * (2.0 * pv_csi_smpc_U.Qrefd);
    if (det > 1000.0) {
      det = 1000.0;
    }

    if (det < -1000.0) {
      det = -1000.0;
    }

    if (rtIsNaN(det)) {
      det = 0.0;
    }

    if (rtmIsMajorTimeStep(pv_csi_smpc_M)) {
      /* DiscreteTransferFcn: '<S1>/PR(z)' incorporates:
       *  MATLAB Function: '<S1>/PQ2Iab_PH1'
       *  Sum: '<S1>/Sum'
       */
      pv_csi_smpc_DW.PRz_tmp = ((det - VSIlineZ) - -1.9966 *
        pv_csi_smpc_DW.PRz_states[0]) - 0.998 * pv_csi_smpc_DW.PRz_states[1];
      det = (0.0201 * pv_csi_smpc_DW.PRz_tmp + -0.039932 *
             pv_csi_smpc_DW.PRz_states[0]) + 0.01986 *
        pv_csi_smpc_DW.PRz_states[1];

      /* Gain: '<Root>/Gain1' incorporates:
       *  DiscreteTransferFcn: '<S1>/PR(z)'
       */
      pv_csi_smpc_B.Gain1 = 2.0 * det;

      /* Outport: '<Root>/duty_cycle' */
      pv_csi_smpc_Y.duty_cycle = det;

      /* DiscreteTransferFcn: '<S1>/Delay90(z)I' */
      pv_csi_smpc_DW.Delay90zI_tmp = VSIlineZ - -0.96364 *
        pv_csi_smpc_DW.Delay90zI_states;
      pv_csi_smpc_B.Delay90zI = -1.0364 * pv_csi_smpc_DW.Delay90zI_states +
        pv_csi_smpc_DW.Delay90zI_tmp;
    }

    /* Sum: '<Root>/Sum4' incorporates:
     *  Gain: '<Root>/Gain4'
     *  Inport: '<Root>/Vload'
     *  Lookup_n-D: '<Root>/1-D Lookup Table2'
     *  Product: '<Root>/Product2'
     */
    pv_csi_smpc_B.Sum4 = 2.0 * look1_binlxpw(pv_csi_smpc_Y.soc,
      pv_csi_smpc_ConstP.uDLookupTable2_bp01Data,
      pv_csi_smpc_ConstP.uDLookupTable2_tableData, 8U) * pv_csi_smpc_B.Gain1 -
      pv_csi_smpc_U.Vload;

    /* MATLAB Function: '<S1>/PQ_singlePhase' incorporates:
     *  Inport: '<Root>/Vload'
     */
    det = pv_csi_smpc_U.Vload * VSIlineZ + pv_csi_smpc_B.Delay90zV *
      pv_csi_smpc_B.Delay90zI;
    if (det > 200000.0) {
      det = 200000.0;
    }

    if (det < -200000.0) {
      det = -0.002;
    }

    pv_csi_smpc_B.p = det / 2.0;
    det = pv_csi_smpc_B.Delay90zV * VSIlineZ - pv_csi_smpc_U.Vload *
      pv_csi_smpc_B.Delay90zI;
    if (det > 200000.0) {
      det = 200000.0;
    }

    if (det < -200000.0) {
      det = -200000.0;
    }

    pv_csi_smpc_B.q = det / 2.0;

    /* End of MATLAB Function: '<S1>/PQ_singlePhase' */

    /* Sum: '<Root>/Sum7' incorporates:
     *  Inport: '<Root>/Ipv'
     */
    pv_csi_smpc_B.Sum7 = VSIlineZ + pv_csi_smpc_U.Ipv;

    /* Derivative: '<Root>/Derivative1' incorporates:
     *  Inport: '<Root>/Vload'
     */
    if ((pv_csi_smpc_DW.TimeStampA >= pv_csi_smpc_M->Timing.t[0]) &&
        (pv_csi_smpc_DW.TimeStampB >= pv_csi_smpc_M->Timing.t[0])) {
      det = 0.0;
    } else {
      det = pv_csi_smpc_DW.TimeStampA;
      lastU = &pv_csi_smpc_DW.LastUAtTimeA;
      if (pv_csi_smpc_DW.TimeStampA < pv_csi_smpc_DW.TimeStampB) {
        if (pv_csi_smpc_DW.TimeStampB < pv_csi_smpc_M->Timing.t[0]) {
          det = pv_csi_smpc_DW.TimeStampB;
          lastU = &pv_csi_smpc_DW.LastUAtTimeB;
        }
      } else {
        if (pv_csi_smpc_DW.TimeStampA >= pv_csi_smpc_M->Timing.t[0]) {
          det = pv_csi_smpc_DW.TimeStampB;
          lastU = &pv_csi_smpc_DW.LastUAtTimeB;
        }
      }

      det = (pv_csi_smpc_U.Vload - *lastU) / (pv_csi_smpc_M->Timing.t[0] - det);
    }

    /* End of Derivative: '<Root>/Derivative1' */

    /* Outport: '<Root>/i3' incorporates:
     *  Gain: '<Root>/Gain2'
     *  Sum: '<Root>/Sum5'
     */
    pv_csi_smpc_Y.i3 = VSIlineZ - 0.00061 * det;

    /* Outport: '<Root>/qdie_meas' incorporates:
     *  StateSpace: '<S1>/Hw2'
     */
    pv_csi_smpc_Y.qdie_meas = 0.94444980822600488 * pv_csi_smpc_X.Hw2_CSTATE[3];

    /* Outport: '<Root>/pdie_meas' incorporates:
     *  StateSpace: '<S1>/Hw1'
     */
    pv_csi_smpc_Y.pdie_meas = 0.94444980822600488 * pv_csi_smpc_X.Hw1_CSTATE[3];
  }

  if (rtmIsMajorTimeStep(pv_csi_smpc_M)) {
    real_T *lastU;
    if (rtmIsMajorTimeStep(pv_csi_smpc_M)) {
      /* Update for DiscreteTransferFcn: '<S1>/Delay90(z)V' */
      pv_csi_smpc_DW.Delay90zV_states = pv_csi_smpc_DW.Delay90zV_tmp;

      /* Update for DiscreteTransferFcn: '<S1>/PR(z)' */
      pv_csi_smpc_DW.PRz_states[1] = pv_csi_smpc_DW.PRz_states[0];
      pv_csi_smpc_DW.PRz_states[0] = pv_csi_smpc_DW.PRz_tmp;

      /* Update for DiscreteTransferFcn: '<S1>/Delay90(z)I' */
      pv_csi_smpc_DW.Delay90zI_states = pv_csi_smpc_DW.Delay90zI_tmp;
    }

    /* Update for Derivative: '<Root>/Derivative1' incorporates:
     *  Inport: '<Root>/Vload'
     */
    if (pv_csi_smpc_DW.TimeStampA == (rtInf)) {
      pv_csi_smpc_DW.TimeStampA = pv_csi_smpc_M->Timing.t[0];
      lastU = &pv_csi_smpc_DW.LastUAtTimeA;
    } else if (pv_csi_smpc_DW.TimeStampB == (rtInf)) {
      pv_csi_smpc_DW.TimeStampB = pv_csi_smpc_M->Timing.t[0];
      lastU = &pv_csi_smpc_DW.LastUAtTimeB;
    } else if (pv_csi_smpc_DW.TimeStampA < pv_csi_smpc_DW.TimeStampB) {
      pv_csi_smpc_DW.TimeStampA = pv_csi_smpc_M->Timing.t[0];
      lastU = &pv_csi_smpc_DW.LastUAtTimeA;
    } else {
      pv_csi_smpc_DW.TimeStampB = pv_csi_smpc_M->Timing.t[0];
      lastU = &pv_csi_smpc_DW.LastUAtTimeB;
    }

    *lastU = pv_csi_smpc_U.Vload;

    /* End of Update for Derivative: '<Root>/Derivative1' */
  }                                    /* end MajorTimeStep */

  if (rtmIsMajorTimeStep(pv_csi_smpc_M)) {
    rt_ertODEUpdateContinuousStates(&pv_csi_smpc_M->solverInfo);

    /* Update absolute time for base rate */
    /* The "clockTick0" counts the number of times the code of this task has
     * been executed. The absolute time is the multiplication of "clockTick0"
     * and "Timing.stepSize0". Size of "clockTick0" ensures timer will not
     * overflow during the application lifespan selected.
     */
    ++pv_csi_smpc_M->Timing.clockTick0;
    pv_csi_smpc_M->Timing.t[0] = rtsiGetSolverStopTime
      (&pv_csi_smpc_M->solverInfo);

    {
      /* Update absolute timer for sample time: [0.0001s, 0.0s] */
      /* The "clockTick1" counts the number of times the code of this task has
       * been executed. The resolution of this integer timer is 0.0001, which is the step size
       * of the task. Size of "clockTick1" ensures timer will not overflow during the
       * application lifespan selected.
       */
      pv_csi_smpc_M->Timing.clockTick1++;
    }
  }                                    /* end MajorTimeStep */
}

/* Derivatives for root system: '<Root>' */
void pv_csi_smpc_derivatives(void)
{
  boolean_T lsat;
  boolean_T usat;
  XDot_pv_csi_smpc_T *_rtXdot;
  _rtXdot = ((XDot_pv_csi_smpc_T *) pv_csi_smpc_M->derivs);

  /* Derivatives for Integrator: '<Root>/Integrator' */
  lsat = (pv_csi_smpc_X.Integrator_CSTATE <= 0.0);
  usat = (pv_csi_smpc_X.Integrator_CSTATE >= 1.0);
  if (((!lsat) && (!usat)) || (lsat && (pv_csi_smpc_B.Sum7 > 0.0)) || (usat &&
       (pv_csi_smpc_B.Sum7 < 0.0))) {
    _rtXdot->Integrator_CSTATE = pv_csi_smpc_B.Sum7;
  } else {
    /* in saturation */
    _rtXdot->Integrator_CSTATE = 0.0;
  }

  /* End of Derivatives for Integrator: '<Root>/Integrator' */

  /* Derivatives for TransferFcn: '<Root>/VSI  line Z' */
  _rtXdot->VSIlineZ_CSTATE = 0.0;
  _rtXdot->VSIlineZ_CSTATE += -0.03333333333333334 *
    pv_csi_smpc_X.VSIlineZ_CSTATE;
  _rtXdot->VSIlineZ_CSTATE += pv_csi_smpc_B.Sum4;

  /* Derivatives for StateSpace: '<S1>/Hw2' */
  _rtXdot->Hw2_CSTATE[0] = 0.0;

  /* Derivatives for StateSpace: '<S1>/Hw1' */
  _rtXdot->Hw1_CSTATE[0] = 0.0;

  /* Derivatives for StateSpace: '<S1>/Hw6' */
  _rtXdot->Hw6_CSTATE[0] = 0.0;

  /* Derivatives for StateSpace: '<S1>/Hw2' */
  _rtXdot->Hw2_CSTATE[1] = 0.0;

  /* Derivatives for StateSpace: '<S1>/Hw1' */
  _rtXdot->Hw1_CSTATE[1] = 0.0;

  /* Derivatives for StateSpace: '<S1>/Hw6' */
  _rtXdot->Hw6_CSTATE[1] = 0.0;

  /* Derivatives for StateSpace: '<S1>/Hw2' */
  _rtXdot->Hw2_CSTATE[2] = 0.0;

  /* Derivatives for StateSpace: '<S1>/Hw1' */
  _rtXdot->Hw1_CSTATE[2] = 0.0;

  /* Derivatives for StateSpace: '<S1>/Hw6' */
  _rtXdot->Hw6_CSTATE[2] = 0.0;

  /* Derivatives for StateSpace: '<S1>/Hw2' */
  _rtXdot->Hw2_CSTATE[3] = 0.0;

  /* Derivatives for StateSpace: '<S1>/Hw1' */
  _rtXdot->Hw1_CSTATE[3] = 0.0;

  /* Derivatives for StateSpace: '<S1>/Hw6' */
  _rtXdot->Hw6_CSTATE[3] = 0.0;

  /* Derivatives for StateSpace: '<S1>/Hw2' */
  _rtXdot->Hw2_CSTATE[0] += -341.08603071128289 * pv_csi_smpc_X.Hw2_CSTATE[0];
  _rtXdot->Hw2_CSTATE[0] += -178.02459475242631 * pv_csi_smpc_X.Hw2_CSTATE[1];
  _rtXdot->Hw2_CSTATE[1] += 178.0245947524264 * pv_csi_smpc_X.Hw2_CSTATE[0];
  _rtXdot->Hw2_CSTATE[2] += 199.58239979893239 * pv_csi_smpc_X.Hw2_CSTATE[1];
  _rtXdot->Hw2_CSTATE[2] += -247.76277465378331 * pv_csi_smpc_X.Hw2_CSTATE[2];
  _rtXdot->Hw2_CSTATE[2] += -199.58239979893239 * pv_csi_smpc_X.Hw2_CSTATE[3];
  _rtXdot->Hw2_CSTATE[3] += 199.58239979893244 * pv_csi_smpc_X.Hw2_CSTATE[2];
  _rtXdot->Hw2_CSTATE[0] += 188.49555921538757 * pv_csi_smpc_B.q;

  /* Derivatives for StateSpace: '<S1>/Hw1' */
  _rtXdot->Hw1_CSTATE[0] += -341.08603071128289 * pv_csi_smpc_X.Hw1_CSTATE[0];
  _rtXdot->Hw1_CSTATE[0] += -178.02459475242631 * pv_csi_smpc_X.Hw1_CSTATE[1];
  _rtXdot->Hw1_CSTATE[1] += 178.0245947524264 * pv_csi_smpc_X.Hw1_CSTATE[0];
  _rtXdot->Hw1_CSTATE[2] += 199.58239979893239 * pv_csi_smpc_X.Hw1_CSTATE[1];
  _rtXdot->Hw1_CSTATE[2] += -247.76277465378331 * pv_csi_smpc_X.Hw1_CSTATE[2];
  _rtXdot->Hw1_CSTATE[2] += -199.58239979893239 * pv_csi_smpc_X.Hw1_CSTATE[3];
  _rtXdot->Hw1_CSTATE[3] += 199.58239979893244 * pv_csi_smpc_X.Hw1_CSTATE[2];
  _rtXdot->Hw1_CSTATE[0] += 188.49555921538757 * pv_csi_smpc_B.p;

  /* Derivatives for StateSpace: '<S1>/Hw6' */
  _rtXdot->Hw6_CSTATE[0] += -341.08603071128289 * pv_csi_smpc_X.Hw6_CSTATE[0];
  _rtXdot->Hw6_CSTATE[0] += -178.02459475242631 * pv_csi_smpc_X.Hw6_CSTATE[1];
  _rtXdot->Hw6_CSTATE[1] += 178.0245947524264 * pv_csi_smpc_X.Hw6_CSTATE[0];
  _rtXdot->Hw6_CSTATE[2] += 199.58239979893239 * pv_csi_smpc_X.Hw6_CSTATE[1];
  _rtXdot->Hw6_CSTATE[2] += -247.76277465378331 * pv_csi_smpc_X.Hw6_CSTATE[2];
  _rtXdot->Hw6_CSTATE[2] += -199.58239979893239 * pv_csi_smpc_X.Hw6_CSTATE[3];
  _rtXdot->Hw6_CSTATE[3] += 199.58239979893244 * pv_csi_smpc_X.Hw6_CSTATE[2];
}

/* Model initialize function */
void pv_csi_smpc_initialize(void)
{
  /* Registration code */

  /* initialize non-finites */
  rt_InitInfAndNaN(sizeof(real_T));

  /* initialize real-time model */
  (void) memset((void *)pv_csi_smpc_M, 0,
                sizeof(RT_MODEL_pv_csi_smpc_T));

  {
    /* Setup solver object */
    rtsiSetSimTimeStepPtr(&pv_csi_smpc_M->solverInfo,
                          &pv_csi_smpc_M->Timing.simTimeStep);
    rtsiSetTPtr(&pv_csi_smpc_M->solverInfo, &rtmGetTPtr(pv_csi_smpc_M));
    rtsiSetStepSizePtr(&pv_csi_smpc_M->solverInfo,
                       &pv_csi_smpc_M->Timing.stepSize0);
    rtsiSetdXPtr(&pv_csi_smpc_M->solverInfo, &pv_csi_smpc_M->derivs);
    rtsiSetContStatesPtr(&pv_csi_smpc_M->solverInfo, (real_T **)
                         &pv_csi_smpc_M->contStates);
    rtsiSetNumContStatesPtr(&pv_csi_smpc_M->solverInfo,
      &pv_csi_smpc_M->Sizes.numContStates);
    rtsiSetNumPeriodicContStatesPtr(&pv_csi_smpc_M->solverInfo,
      &pv_csi_smpc_M->Sizes.numPeriodicContStates);
    rtsiSetPeriodicContStateIndicesPtr(&pv_csi_smpc_M->solverInfo,
      &pv_csi_smpc_M->periodicContStateIndices);
    rtsiSetPeriodicContStateRangesPtr(&pv_csi_smpc_M->solverInfo,
      &pv_csi_smpc_M->periodicContStateRanges);
    rtsiSetErrorStatusPtr(&pv_csi_smpc_M->solverInfo, (&rtmGetErrorStatus
      (pv_csi_smpc_M)));
    rtsiSetRTModelPtr(&pv_csi_smpc_M->solverInfo, pv_csi_smpc_M);
  }

  rtsiSetSimTimeStep(&pv_csi_smpc_M->solverInfo, MAJOR_TIME_STEP);
  pv_csi_smpc_M->intgData.y = pv_csi_smpc_M->odeY;
  pv_csi_smpc_M->intgData.f[0] = pv_csi_smpc_M->odeF[0];
  pv_csi_smpc_M->intgData.f[1] = pv_csi_smpc_M->odeF[1];
  pv_csi_smpc_M->intgData.f[2] = pv_csi_smpc_M->odeF[2];
  pv_csi_smpc_M->intgData.f[3] = pv_csi_smpc_M->odeF[3];
  pv_csi_smpc_M->contStates = ((X_pv_csi_smpc_T *) &pv_csi_smpc_X);
  rtsiSetSolverData(&pv_csi_smpc_M->solverInfo, (void *)&pv_csi_smpc_M->intgData);
  rtsiSetSolverName(&pv_csi_smpc_M->solverInfo,"ode4");
  rtmSetTPtr(pv_csi_smpc_M, &pv_csi_smpc_M->Timing.tArray[0]);
  pv_csi_smpc_M->Timing.stepSize0 = 0.0001;

  /* block I/O */
  (void) memset(((void *) &pv_csi_smpc_B), 0,
                sizeof(B_pv_csi_smpc_T));

  /* states (continuous) */
  {
    (void) memset((void *)&pv_csi_smpc_X, 0,
                  sizeof(X_pv_csi_smpc_T));
  }

  /* states (dwork) */
  (void) memset((void *)&pv_csi_smpc_DW, 0,
                sizeof(DW_pv_csi_smpc_T));

  /* external inputs */
  (void)memset((void *)&pv_csi_smpc_U, 0, sizeof(ExtU_pv_csi_smpc_T));

  /* external outputs */
  (void) memset((void *)&pv_csi_smpc_Y, 0,
                sizeof(ExtY_pv_csi_smpc_T));

  /* InitializeConditions for Integrator: '<Root>/Integrator' */
  pv_csi_smpc_X.Integrator_CSTATE = 0.0;

  /* InitializeConditions for TransferFcn: '<Root>/VSI  line Z' */
  pv_csi_smpc_X.VSIlineZ_CSTATE = 0.0;

  /* InitializeConditions for Derivative: '<Root>/Derivative1' */
  pv_csi_smpc_DW.TimeStampA = (rtInf);
  pv_csi_smpc_DW.TimeStampB = (rtInf);

  /* InitializeConditions for StateSpace: '<S1>/Hw2' */
  pv_csi_smpc_X.Hw2_CSTATE[0] = 0.0;

  /* InitializeConditions for StateSpace: '<S1>/Hw1' */
  pv_csi_smpc_X.Hw1_CSTATE[0] = 0.0;

  /* InitializeConditions for StateSpace: '<S1>/Hw6' */
  pv_csi_smpc_X.Hw6_CSTATE[0] = 0.0;

  /* InitializeConditions for StateSpace: '<S1>/Hw2' */
  pv_csi_smpc_X.Hw2_CSTATE[1] = 0.0;

  /* InitializeConditions for StateSpace: '<S1>/Hw1' */
  pv_csi_smpc_X.Hw1_CSTATE[1] = 0.0;

  /* InitializeConditions for StateSpace: '<S1>/Hw6' */
  pv_csi_smpc_X.Hw6_CSTATE[1] = 0.0;

  /* InitializeConditions for StateSpace: '<S1>/Hw2' */
  pv_csi_smpc_X.Hw2_CSTATE[2] = 0.0;

  /* InitializeConditions for StateSpace: '<S1>/Hw1' */
  pv_csi_smpc_X.Hw1_CSTATE[2] = 0.0;

  /* InitializeConditions for StateSpace: '<S1>/Hw6' */
  pv_csi_smpc_X.Hw6_CSTATE[2] = 0.0;

  /* InitializeConditions for StateSpace: '<S1>/Hw2' */
  pv_csi_smpc_X.Hw2_CSTATE[3] = 0.0;

  /* InitializeConditions for StateSpace: '<S1>/Hw1' */
  pv_csi_smpc_X.Hw1_CSTATE[3] = 0.0;

  /* InitializeConditions for StateSpace: '<S1>/Hw6' */
  pv_csi_smpc_X.Hw6_CSTATE[3] = 0.0;
}

/* Model terminate function */
void pv_csi_smpc_terminate(void)
{
  /* (no terminate code required) */
}

/*
 * File trailer for generated code.
 *
 * [EOF]
 */
