/*
 * File: carga_sf.c
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

#include "carga_sf.h"
#include "carga_sf_private.h"

/* Block signals (auto storage) */
B_carga_sf_T carga_sf_B;

/* Continuous states */
X_carga_sf_T carga_sf_X;

/* Block states (auto storage) */
DW_carga_sf_T carga_sf_DW;

/* External inputs (root inport signals with auto storage) */
ExtU_carga_sf_T carga_sf_U;

/* External outputs (root outports fed by signals with auto storage) */
ExtY_carga_sf_T carga_sf_Y;

//-------Funciones para asignar entradas-------------
void set_i1(double i1){
	carga_sf_U.i1=i1;
}	
void set_i2(double i2){
	carga_sf_U.i2=i2;
}
void set_i3(double i3){
	carga_sf_U.i3=i3;
}
//-----Funciones para obtener valores de las salidas-------
double get_Pm(void){
	//printf("La potencia P medida es: %3.2f \n",carga_sf_Y.Pm);
	return carga_sf_Y.Pm;
}

double get_Qm(void){
	return carga_sf_Y.Qm;
}

double get_Vload(void){
	return carga_sf_Y.Vload;
}

/* Real-time model */
RT_MODEL_carga_sf_T carga_sf_M_;
RT_MODEL_carga_sf_T *const carga_sf_M = &carga_sf_M_;

/*
 * Time delay interpolation routine
 *
 * The linear interpolation is performed using the formula:
 *
 *          (t2 - tMinusDelay)         (tMinusDelay - t1)
 * u(t)  =  ----------------- * u1  +  ------------------- * u2
 *              (t2 - t1)                  (t2 - t1)
 */
real_T rt_TDelayInterpolate(
  real_T tMinusDelay,                  /* tMinusDelay = currentSimTime - delay */
  real_T tStart,
  real_T *tBuf,
  real_T *uBuf,
  int_T bufSz,
  int_T *lastIdx,
  int_T oldestIdx,
  int_T newIdx,
  real_T initOutput,
  boolean_T discrete,
  boolean_T minorStepAndTAtLastMajorOutput)
{
  int_T i;
  real_T yout, t1, t2, u1, u2;

  /*
   * If there is only one data point in the buffer, this data point must be
   * the t= 0 and tMinusDelay > t0, it ask for something unknown. The best
   * guess if initial output as well
   */
  if ((newIdx == 0) && (oldestIdx ==0 ) && (tMinusDelay > tStart))
    return initOutput;

  /*
   * If tMinusDelay is less than zero, should output initial value
   */
  if (tMinusDelay <= tStart)
    return initOutput;

  /* For fixed buffer extrapolation:
   * if tMinusDelay is small than the time at oldestIdx, if discrete, output
   * tailptr value,  else use tailptr and tailptr+1 value to extrapolate
   * It is also for fixed buffer. Note: The same condition can happen for transport delay block where
   * use tStart and and t[tail] other than using t[tail] and t[tail+1].
   * See below
   */
  if ((tMinusDelay <= tBuf[oldestIdx] ) ) {
    if (discrete) {
      return(uBuf[oldestIdx]);
    } else {
      int_T tempIdx= oldestIdx + 1;
      if (oldestIdx == bufSz-1)
        tempIdx = 0;
      t1= tBuf[oldestIdx];
      t2= tBuf[tempIdx];
      u1= uBuf[oldestIdx];
      u2= uBuf[tempIdx];
      if (t2 == t1) {
        if (tMinusDelay >= t2) {
          yout = u2;
        } else {
          yout = u1;
        }
      } else {
        real_T f1 = (t2-tMinusDelay) / (t2-t1);
        real_T f2 = 1.0 - f1;

        /*
         * Use Lagrange's interpolation formula.  Exact outputs at t1, t2.
         */
        yout = f1*u1 + f2*u2;
      }

      return yout;
    }
  }

  /*
   * When block does not have direct feedthrough, we use the table of
   * values to extrapolate off the end of the table for delays that are less
   * than 0 (less then step size).  This is not completely accurate.  The
   * chain of events is as follows for a given time t.  Major output - look
   * in table.  Update - add entry to table.  Now, if we call the output at
   * time t again, there is a new entry in the table. For very small delays,
   * this means that we will have a different answer from the previous call
   * to the output fcn at the same time t.  The following code prevents this
   * from happening.
   */
  if (minorStepAndTAtLastMajorOutput) {
    /* pretend that the new entry has not been added to table */
    if (newIdx != 0) {
      if (*lastIdx == newIdx) {
        (*lastIdx)--;
      }

      newIdx--;
    } else {
      if (*lastIdx == newIdx) {
        *lastIdx = bufSz-1;
      }

      newIdx = bufSz - 1;
    }
  }

  i = *lastIdx;
  if (tBuf[i] < tMinusDelay) {
    /* Look forward starting at last index */
    while (tBuf[i] < tMinusDelay) {
      /* May occur if the delay is less than step-size - extrapolate */
      if (i == newIdx)
        break;
      i = ( i < (bufSz-1) ) ? (i+1) : 0;/* move through buffer */
    }
  } else {
    /*
     * Look backwards starting at last index which can happen when the
     * delay time increases.
     */
    while (tBuf[i] >= tMinusDelay) {
      /*
       * Due to the entry condition at top of function, we
       * should never hit the end.
       */
      i = (i > 0) ? i-1 : (bufSz-1);   /* move through buffer */
    }

    i = ( i < (bufSz-1) ) ? (i+1) : 0;
  }

  *lastIdx = i;
  if (discrete) {
    /*
     * tempEps = 128 * eps;
     * localEps = max(tempEps, tempEps*fabs(tBuf[i]))/2;
     */
    double tempEps = (DBL_EPSILON) * 128.0;
    double localEps = tempEps * fabs(tBuf[i]);
    if (tempEps > localEps) {
      localEps = tempEps;
    }

    localEps = localEps / 2.0;
    if (tMinusDelay >= (tBuf[i] - localEps)) {
      yout = uBuf[i];
    } else {
      if (i == 0) {
        yout = uBuf[bufSz-1];
      } else {
        yout = uBuf[i-1];
      }
    }
  } else {
    if (i == 0) {
      t1 = tBuf[bufSz-1];
      u1 = uBuf[bufSz-1];
    } else {
      t1 = tBuf[i-1];
      u1 = uBuf[i-1];
    }

    t2 = tBuf[i];
    u2 = uBuf[i];
    if (t2 == t1) {
      if (tMinusDelay >= t2) {
        yout = u2;
      } else {
        yout = u1;
      }
    } else {
      real_T f1 = (t2-tMinusDelay) / (t2-t1);
      real_T f2 = 1.0 - f1;

      /*
       * Use Lagrange's interpolation formula.  Exact outputs at t1, t2.
       */
      yout = f1*u1 + f2*u2;
    }
  }

  return(yout);
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
  int_T nXc = 6;
  rtsiSetSimTimeStep(si,MINOR_TIME_STEP);

  /* Save the state values at time t in y, we'll use x as ynew. */
  (void) memcpy(y, x,
                (uint_T)nXc*sizeof(real_T));

  /* Assumes that rtsiSetT and ModelOutputs are up-to-date */
  /* f0 = f(t,y) */
  rtsiSetdX(si, f0);
  carga_sf_derivatives();

  /* f1 = f(t + (h/2), y + (h/2)*f0) */
  temp = 0.5 * h;
  for (i = 0; i < nXc; i++) {
    x[i] = y[i] + (temp*f0[i]);
  }

  rtsiSetT(si, t + temp);
  rtsiSetdX(si, f1);
  carga_sf_step();
  carga_sf_derivatives();

  /* f2 = f(t + (h/2), y + (h/2)*f1) */
  for (i = 0; i < nXc; i++) {
    x[i] = y[i] + (temp*f1[i]);
  }

  rtsiSetdX(si, f2);
  carga_sf_step();
  carga_sf_derivatives();

  /* f3 = f(t + h, y + h*f2) */
  for (i = 0; i < nXc; i++) {
    x[i] = y[i] + (h*f2[i]);
  }

  rtsiSetT(si, tnew);
  rtsiSetdX(si, f3);
  carga_sf_step();
  carga_sf_derivatives();

  /* tnew = t + h
     ynew = y + (h/6)*(f0 + 2*f1 + 2*f2 + 2*f3) */
  temp = h / 6.0;
  for (i = 0; i < nXc; i++) {
    x[i] = y[i] + temp*(f0[i] + 2.0*f1[i] + 2.0*f2[i] + f3[i]);
  }

  rtsiSetSimTimeStep(si,MAJOR_TIME_STEP);
}

real_T rt_hypotd_snf(real_T u0, real_T u1)
{
  real_T y;
  real_T a;
  a = fabs(u0);
  y = fabs(u1);
  if (a < y) {
    a /= y;
    y *= sqrt(a * a + 1.0);
  } else if (a > y) {
    y /= a;
    y = sqrt(y * y + 1.0) * a;
  } else {
    if (!rtIsNaN(y)) {
      y = a * 1.4142135623730951;
    }
  }

  return y;
}

real_T rt_atan2d_snf(real_T u0, real_T u1)
{
  real_T y;
  int32_T u0_0;
  int32_T u1_0;
  if (rtIsNaN(u0) || rtIsNaN(u1)) {
    y = (rtNaN);
  } else if (rtIsInf(u0) && rtIsInf(u1)) {
    if (u0 > 0.0) {
      u0_0 = 1;
    } else {
      u0_0 = -1;
    }

    if (u1 > 0.0) {
      u1_0 = 1;
    } else {
      u1_0 = -1;
    }

    y = atan2(u0_0, u1_0);
  } else if (u1 == 0.0) {
    if (u0 > 0.0) {
      y = RT_PI / 2.0;
    } else if (u0 < 0.0) {
      y = -(RT_PI / 2.0);
    } else {
      y = 0.0;
    }
  } else {
    y = atan2(u0, u1);
  }

  return y;
}

/* Model step function */
void carga_sf_step(void)
{
  /* local block i/o variables */
  real_T rtb_integrator;
  real_T rtb_TransportDelay;
  real_T rtb_integrator_p;
  real_T rtb_TransportDelay_i;
  real_T rtb_integrator_g;
  real_T rtb_TransportDelay_h;
  real_T rtb_integrator_h;
  real_T rtb_TransportDelay_c;
  real_T rtb_ComplextoMagnitudeAngle_o1;
  real_T rtb_ComplextoMagnitudeAngle_o2;
  if (rtmIsMajorTimeStep(carga_sf_M)) {
    /* set solver stop time */
    rtsiSetSolverStopTime(&carga_sf_M->solverInfo,
                          ((carga_sf_M->Timing.clockTick0+1)*
      carga_sf_M->Timing.stepSize0));
  }                                    /* end MajorTimeStep */

  /* Update absolute time of base rate at minor time step */
  if (rtmIsMinorTimeStep(carga_sf_M)) {
    carga_sf_M->Timing.t[0] = rtsiGetT(&carga_sf_M->solverInfo);
  }

  /* Integrator: '<S7>/integrator' */
  rtb_integrator = carga_sf_X.integrator_CSTATE;

  /* TransportDelay: '<S7>/Transport Delay' */
  {
    real_T **uBuffer = (real_T**)&carga_sf_DW.TransportDelay_PWORK.TUbufferPtrs
      [0];
    real_T **tBuffer = (real_T**)&carga_sf_DW.TransportDelay_PWORK.TUbufferPtrs
      [1];
    real_T simTime = carga_sf_M->Timing.t[0];
    real_T tMinusDelay = simTime - (0.016666666666666666);
    rtb_TransportDelay = rt_TDelayInterpolate(
      tMinusDelay,
      0.0,
      *tBuffer,
      *uBuffer,
      carga_sf_DW.TransportDelay_IWORK.CircularBufSize,
      &carga_sf_DW.TransportDelay_IWORK.Last,
      carga_sf_DW.TransportDelay_IWORK.Tail,
      carga_sf_DW.TransportDelay_IWORK.Head,
      0.0,
      0,
      0);
  }

  if (rtmIsMajorTimeStep(carga_sf_M)) {
    /* Memory: '<S7>/Memory' */
    carga_sf_B.Memory = carga_sf_DW.Memory_PreviousInput;
  }

  /* Switch: '<S7>/Switch' incorporates:
   *  Clock: '<S7>/Clock'
   *  Constant: '<S7>/K1'
   *  Gain: '<S7>/Gain'
   *  RelationalOperator: '<S7>/Relational Operator'
   *  Sum: '<S7>/Sum'
   */
  if (carga_sf_M->Timing.t[0] >= 0.016666666666666666) {
    carga_sf_B.Switch = (rtb_integrator - rtb_TransportDelay) * 60.0;
  } else {
    carga_sf_B.Switch = carga_sf_B.Memory;
  }

  /* End of Switch: '<S7>/Switch' */

  /* Integrator: '<S6>/integrator' */
  rtb_integrator_p = carga_sf_X.integrator_CSTATE_n;

  /* TransportDelay: '<S6>/Transport Delay' */
  {
    real_T **uBuffer = (real_T**)
      &carga_sf_DW.TransportDelay_PWORK_j.TUbufferPtrs[0];
    real_T **tBuffer = (real_T**)
      &carga_sf_DW.TransportDelay_PWORK_j.TUbufferPtrs[1];
    real_T simTime = carga_sf_M->Timing.t[0];
    real_T tMinusDelay = simTime - (0.016666666666666666);
    rtb_TransportDelay_i = rt_TDelayInterpolate(
      tMinusDelay,
      0.0,
      *tBuffer,
      *uBuffer,
      carga_sf_DW.TransportDelay_IWORK_a.CircularBufSize,
      &carga_sf_DW.TransportDelay_IWORK_a.Last,
      carga_sf_DW.TransportDelay_IWORK_a.Tail,
      carga_sf_DW.TransportDelay_IWORK_a.Head,
      0.0,
      0,
      0);
  }

  if (rtmIsMajorTimeStep(carga_sf_M)) {
    /* Memory: '<S6>/Memory' */
    carga_sf_B.Memory_p = carga_sf_DW.Memory_PreviousInput_p;
  }

  /* Switch: '<S6>/Switch' incorporates:
   *  Clock: '<S6>/Clock'
   *  Constant: '<S6>/K1'
   *  Gain: '<S6>/Gain'
   *  RelationalOperator: '<S6>/Relational Operator'
   *  Sum: '<S6>/Sum'
   */
  if (carga_sf_M->Timing.t[0] >= 0.016666666666666666) {
    carga_sf_B.Switch_g = (rtb_integrator_p - rtb_TransportDelay_i) * 60.0;
  } else {
    carga_sf_B.Switch_g = carga_sf_B.Memory_p;
  }

  /* End of Switch: '<S6>/Switch' */

  /* Integrator: '<S11>/integrator' */
  rtb_integrator_g = carga_sf_X.integrator_CSTATE_f;

  /* TransportDelay: '<S11>/Transport Delay' */
  {
    real_T **uBuffer = (real_T**)
      &carga_sf_DW.TransportDelay_PWORK_p.TUbufferPtrs[0];
    real_T **tBuffer = (real_T**)
      &carga_sf_DW.TransportDelay_PWORK_p.TUbufferPtrs[1];
    real_T simTime = carga_sf_M->Timing.t[0];
    real_T tMinusDelay = simTime - (0.016666666666666666);
    rtb_TransportDelay_h = rt_TDelayInterpolate(
      tMinusDelay,
      0.0,
      *tBuffer,
      *uBuffer,
      carga_sf_DW.TransportDelay_IWORK_n.CircularBufSize,
      &carga_sf_DW.TransportDelay_IWORK_n.Last,
      carga_sf_DW.TransportDelay_IWORK_n.Tail,
      carga_sf_DW.TransportDelay_IWORK_n.Head,
      0.0,
      0,
      0);
  }

  if (rtmIsMajorTimeStep(carga_sf_M)) {
    /* Memory: '<S11>/Memory' */
    carga_sf_B.Memory_m = carga_sf_DW.Memory_PreviousInput_a;
  }

  /* Switch: '<S11>/Switch' incorporates:
   *  Clock: '<S11>/Clock'
   *  Constant: '<S11>/K1'
   *  Gain: '<S11>/Gain'
   *  RelationalOperator: '<S11>/Relational Operator'
   *  Sum: '<S11>/Sum'
   */
  if (carga_sf_M->Timing.t[0] >= 0.016666666666666666) {
    carga_sf_B.Switch_f = (rtb_integrator_g - rtb_TransportDelay_h) * 60.0;
  } else {
    carga_sf_B.Switch_f = carga_sf_B.Memory_m;
  }

  /* End of Switch: '<S11>/Switch' */

  /* Integrator: '<S10>/integrator' */
  rtb_integrator_h = carga_sf_X.integrator_CSTATE_e;

  /* TransportDelay: '<S10>/Transport Delay' */
  {
    real_T **uBuffer = (real_T**)
      &carga_sf_DW.TransportDelay_PWORK_o.TUbufferPtrs[0];
    real_T **tBuffer = (real_T**)
      &carga_sf_DW.TransportDelay_PWORK_o.TUbufferPtrs[1];
    real_T simTime = carga_sf_M->Timing.t[0];
    real_T tMinusDelay = simTime - (0.016666666666666666);
    rtb_TransportDelay_c = rt_TDelayInterpolate(
      tMinusDelay,
      0.0,
      *tBuffer,
      *uBuffer,
      carga_sf_DW.TransportDelay_IWORK_av.CircularBufSize,
      &carga_sf_DW.TransportDelay_IWORK_av.Last,
      carga_sf_DW.TransportDelay_IWORK_av.Tail,
      carga_sf_DW.TransportDelay_IWORK_av.Head,
      0.0,
      0,
      0);
  }

  if (rtmIsMajorTimeStep(carga_sf_M)) {
    /* Memory: '<S10>/Memory' */
    carga_sf_B.Memory_b = carga_sf_DW.Memory_PreviousInput_pa;
  }

  /* Switch: '<S10>/Switch' incorporates:
   *  Clock: '<S10>/Clock'
   *  Constant: '<S10>/K1'
   *  Gain: '<S10>/Gain'
   *  RelationalOperator: '<S10>/Relational Operator'
   *  Sum: '<S10>/Sum'
   */
  if (carga_sf_M->Timing.t[0] >= 0.016666666666666666) {
    carga_sf_B.Switch_d = (rtb_integrator_h - rtb_TransportDelay_c) * 60.0;
  } else {
    carga_sf_B.Switch_d = carga_sf_B.Memory_b;
  }

  /* End of Switch: '<S10>/Switch' */

  /* Gain: '<S1>/Gain1' incorporates:
   *  ComplexToMagnitudeAngle: '<S2>/Complex to Magnitude-Angle'
   *  ComplexToMagnitudeAngle: '<S3>/Complex to Magnitude-Angle'
   *  Product: '<S1>/Product'
   *  RealImagToComplex: '<S2>/Real-Imag to Complex'
   *  RealImagToComplex: '<S3>/Real-Imag to Complex'
   */
  rtb_ComplextoMagnitudeAngle_o1 = rt_hypotd_snf(carga_sf_B.Switch,
    carga_sf_B.Switch_g) * rt_hypotd_snf(carga_sf_B.Switch_f,
    carga_sf_B.Switch_d) * 0.5;

  /* Gain: '<S1>/Deg->Rad' incorporates:
   *  ComplexToMagnitudeAngle: '<S2>/Complex to Magnitude-Angle'
   *  ComplexToMagnitudeAngle: '<S3>/Complex to Magnitude-Angle'
   *  Gain: '<S2>/Rad->Deg.'
   *  Gain: '<S3>/Rad->Deg.'
   *  RealImagToComplex: '<S2>/Real-Imag to Complex'
   *  RealImagToComplex: '<S3>/Real-Imag to Complex'
   *  Sum: '<S1>/Sum'
   */
  rtb_ComplextoMagnitudeAngle_o2 = (57.295779513082323 * rt_atan2d_snf
    (carga_sf_B.Switch_g, carga_sf_B.Switch) - 57.295779513082323 *
    rt_atan2d_snf(carga_sf_B.Switch_d, carga_sf_B.Switch_f)) *
    0.017453292519943295;

  /* Outport: '<Root>/Pm' incorporates:
   *  Product: '<S1>/Product1'
   *  Trigonometry: '<S1>/Trigonometric Function1'
   */
  carga_sf_Y.Pm = rtb_ComplextoMagnitudeAngle_o1 * cos
    (rtb_ComplextoMagnitudeAngle_o2);

  /* Outport: '<Root>/Qm' incorporates:
   *  Product: '<S1>/Product3'
   *  Trigonometry: '<S1>/Trigonometric Function2'
   */
  carga_sf_Y.Qm = rtb_ComplextoMagnitudeAngle_o1 * sin
    (rtb_ComplextoMagnitudeAngle_o2);

  /* TransferFcn: '<Root>/Transfer Fcn3' */
  rtb_ComplextoMagnitudeAngle_o2 = 376.7897513187641 *
    carga_sf_X.TransferFcn3_CSTATE[0] + 0.0 * carga_sf_X.TransferFcn3_CSTATE[1];

  /* Outport: '<Root>/Vload' */
  carga_sf_Y.Vload = rtb_ComplextoMagnitudeAngle_o2;

  /* Product: '<S2>/Product' incorporates:
   *  Sin: '<S2>/sin(wt)'
   */
  carga_sf_B.Product = sin(376.99111843077515 * carga_sf_M->Timing.t[0]) * 2.0 *
    rtb_ComplextoMagnitudeAngle_o2;

  /* Product: '<S2>/Product1' incorporates:
   *  Sin: '<S2>/cos(wt)'
   */
  carga_sf_B.Product1 = sin(376.99111843077515 * carga_sf_M->Timing.t[0] +
    1.5707963267948966) * 2.0 * rtb_ComplextoMagnitudeAngle_o2;

  /* Sum: '<Root>/Sum6' incorporates:
   *  Inport: '<Root>/i1'
   *  Inport: '<Root>/i2'
   *  Inport: '<Root>/i3'
   */
  carga_sf_B.Sum6 = (carga_sf_U.i1 + carga_sf_U.i2) + carga_sf_U.i3;

  /* Product: '<S3>/Product' incorporates:
   *  Sin: '<S3>/sin(wt)'
   */
  carga_sf_B.Product_m = sin(376.99111843077515 * carga_sf_M->Timing.t[0]) * 2.0
    * carga_sf_B.Sum6;

  /* Product: '<S3>/Product1' incorporates:
   *  Sin: '<S3>/cos(wt)'
   */
  carga_sf_B.Product1_c = sin(376.99111843077515 * carga_sf_M->Timing.t[0] +
    1.5707963267948966) * 2.0 * carga_sf_B.Sum6;
  if (rtmIsMajorTimeStep(carga_sf_M)) {
    /* Update for TransportDelay: '<S7>/Transport Delay' */
    {
      real_T **uBuffer = (real_T**)
        &carga_sf_DW.TransportDelay_PWORK.TUbufferPtrs[0];
      real_T **tBuffer = (real_T**)
        &carga_sf_DW.TransportDelay_PWORK.TUbufferPtrs[1];
      real_T simTime = carga_sf_M->Timing.t[0];
      carga_sf_DW.TransportDelay_IWORK.Head =
        ((carga_sf_DW.TransportDelay_IWORK.Head <
          (carga_sf_DW.TransportDelay_IWORK.CircularBufSize-1)) ?
         (carga_sf_DW.TransportDelay_IWORK.Head+1) : 0);
      if (carga_sf_DW.TransportDelay_IWORK.Head ==
          carga_sf_DW.TransportDelay_IWORK.Tail) {
        carga_sf_DW.TransportDelay_IWORK.Tail =
          ((carga_sf_DW.TransportDelay_IWORK.Tail <
            (carga_sf_DW.TransportDelay_IWORK.CircularBufSize-1)) ?
           (carga_sf_DW.TransportDelay_IWORK.Tail+1) : 0);
      }

      (*tBuffer)[carga_sf_DW.TransportDelay_IWORK.Head] = simTime;
      (*uBuffer)[carga_sf_DW.TransportDelay_IWORK.Head] = rtb_integrator;
    }

    if (rtmIsMajorTimeStep(carga_sf_M)) {
      /* Update for Memory: '<S7>/Memory' */
      carga_sf_DW.Memory_PreviousInput = carga_sf_B.Switch;
    }

    /* Update for TransportDelay: '<S6>/Transport Delay' */
    {
      real_T **uBuffer = (real_T**)
        &carga_sf_DW.TransportDelay_PWORK_j.TUbufferPtrs[0];
      real_T **tBuffer = (real_T**)
        &carga_sf_DW.TransportDelay_PWORK_j.TUbufferPtrs[1];
      real_T simTime = carga_sf_M->Timing.t[0];
      carga_sf_DW.TransportDelay_IWORK_a.Head =
        ((carga_sf_DW.TransportDelay_IWORK_a.Head <
          (carga_sf_DW.TransportDelay_IWORK_a.CircularBufSize-1)) ?
         (carga_sf_DW.TransportDelay_IWORK_a.Head+1) : 0);
      if (carga_sf_DW.TransportDelay_IWORK_a.Head ==
          carga_sf_DW.TransportDelay_IWORK_a.Tail) {
        carga_sf_DW.TransportDelay_IWORK_a.Tail =
          ((carga_sf_DW.TransportDelay_IWORK_a.Tail <
            (carga_sf_DW.TransportDelay_IWORK_a.CircularBufSize-1)) ?
           (carga_sf_DW.TransportDelay_IWORK_a.Tail+1) : 0);
      }

      (*tBuffer)[carga_sf_DW.TransportDelay_IWORK_a.Head] = simTime;
      (*uBuffer)[carga_sf_DW.TransportDelay_IWORK_a.Head] = rtb_integrator_p;
    }

    if (rtmIsMajorTimeStep(carga_sf_M)) {
      /* Update for Memory: '<S6>/Memory' */
      carga_sf_DW.Memory_PreviousInput_p = carga_sf_B.Switch_g;
    }

    /* Update for TransportDelay: '<S11>/Transport Delay' */
    {
      real_T **uBuffer = (real_T**)
        &carga_sf_DW.TransportDelay_PWORK_p.TUbufferPtrs[0];
      real_T **tBuffer = (real_T**)
        &carga_sf_DW.TransportDelay_PWORK_p.TUbufferPtrs[1];
      real_T simTime = carga_sf_M->Timing.t[0];
      carga_sf_DW.TransportDelay_IWORK_n.Head =
        ((carga_sf_DW.TransportDelay_IWORK_n.Head <
          (carga_sf_DW.TransportDelay_IWORK_n.CircularBufSize-1)) ?
         (carga_sf_DW.TransportDelay_IWORK_n.Head+1) : 0);
      if (carga_sf_DW.TransportDelay_IWORK_n.Head ==
          carga_sf_DW.TransportDelay_IWORK_n.Tail) {
        carga_sf_DW.TransportDelay_IWORK_n.Tail =
          ((carga_sf_DW.TransportDelay_IWORK_n.Tail <
            (carga_sf_DW.TransportDelay_IWORK_n.CircularBufSize-1)) ?
           (carga_sf_DW.TransportDelay_IWORK_n.Tail+1) : 0);
      }

      (*tBuffer)[carga_sf_DW.TransportDelay_IWORK_n.Head] = simTime;
      (*uBuffer)[carga_sf_DW.TransportDelay_IWORK_n.Head] = rtb_integrator_g;
    }

    if (rtmIsMajorTimeStep(carga_sf_M)) {
      /* Update for Memory: '<S11>/Memory' */
      carga_sf_DW.Memory_PreviousInput_a = carga_sf_B.Switch_f;
    }

    /* Update for TransportDelay: '<S10>/Transport Delay' */
    {
      real_T **uBuffer = (real_T**)
        &carga_sf_DW.TransportDelay_PWORK_o.TUbufferPtrs[0];
      real_T **tBuffer = (real_T**)
        &carga_sf_DW.TransportDelay_PWORK_o.TUbufferPtrs[1];
      real_T simTime = carga_sf_M->Timing.t[0];
      carga_sf_DW.TransportDelay_IWORK_av.Head =
        ((carga_sf_DW.TransportDelay_IWORK_av.Head <
          (carga_sf_DW.TransportDelay_IWORK_av.CircularBufSize-1)) ?
         (carga_sf_DW.TransportDelay_IWORK_av.Head+1) : 0);
      if (carga_sf_DW.TransportDelay_IWORK_av.Head ==
          carga_sf_DW.TransportDelay_IWORK_av.Tail) {
        carga_sf_DW.TransportDelay_IWORK_av.Tail =
          ((carga_sf_DW.TransportDelay_IWORK_av.Tail <
            (carga_sf_DW.TransportDelay_IWORK_av.CircularBufSize-1)) ?
           (carga_sf_DW.TransportDelay_IWORK_av.Tail+1) : 0);
      }

      (*tBuffer)[carga_sf_DW.TransportDelay_IWORK_av.Head] = simTime;
      (*uBuffer)[carga_sf_DW.TransportDelay_IWORK_av.Head] = rtb_integrator_h;
    }

    if (rtmIsMajorTimeStep(carga_sf_M)) {
      /* Update for Memory: '<S10>/Memory' */
      carga_sf_DW.Memory_PreviousInput_pa = carga_sf_B.Switch_d;
    }
  }                                    /* end MajorTimeStep */

  if (rtmIsMajorTimeStep(carga_sf_M)) {
    rt_ertODEUpdateContinuousStates(&carga_sf_M->solverInfo);

    /* Update absolute time for base rate */
    /* The "clockTick0" counts the number of times the code of this task has
     * been executed. The absolute time is the multiplication of "clockTick0"
     * and "Timing.stepSize0". Size of "clockTick0" ensures timer will not
     * overflow during the application lifespan selected.
     */
    ++carga_sf_M->Timing.clockTick0;
    carga_sf_M->Timing.t[0] = rtsiGetSolverStopTime(&carga_sf_M->solverInfo);

    {
      /* Update absolute timer for sample time: [0.0001s, 0.0s] */
      /* The "clockTick1" counts the number of times the code of this task has
       * been executed. The resolution of this integer timer is 0.0001, which is the step size
       * of the task. Size of "clockTick1" ensures timer will not overflow during the
       * application lifespan selected.
       */
      carga_sf_M->Timing.clockTick1++;
    }
  }                                    /* end MajorTimeStep */
}

/* Derivatives for root system: '<Root>' */
void carga_sf_derivatives(void)
{
  XDot_carga_sf_T *_rtXdot;
  _rtXdot = ((XDot_carga_sf_T *) carga_sf_M->derivs);

  /* Derivatives for Integrator: '<S7>/integrator' */
  _rtXdot->integrator_CSTATE = carga_sf_B.Product;

  /* Derivatives for Integrator: '<S6>/integrator' */
  _rtXdot->integrator_CSTATE_n = carga_sf_B.Product1;

  /* Derivatives for Integrator: '<S11>/integrator' */
  _rtXdot->integrator_CSTATE_f = carga_sf_B.Product_m;

  /* Derivatives for Integrator: '<S10>/integrator' */
  _rtXdot->integrator_CSTATE_e = carga_sf_B.Product1_c;

  /* Derivatives for TransferFcn: '<Root>/Transfer Fcn3' */
  _rtXdot->TransferFcn3_CSTATE[0] = 0.0;
  _rtXdot->TransferFcn3_CSTATE[0] += -75.357950263752812 *
    carga_sf_X.TransferFcn3_CSTATE[0];
  _rtXdot->TransferFcn3_CSTATE[1] = 0.0;
  _rtXdot->TransferFcn3_CSTATE[0] += -72.459567561300787 *
    carga_sf_X.TransferFcn3_CSTATE[1];
  _rtXdot->TransferFcn3_CSTATE[1] += carga_sf_X.TransferFcn3_CSTATE[0];
  _rtXdot->TransferFcn3_CSTATE[0] += carga_sf_B.Sum6;
}

/* Model initialize function */
void carga_sf_initialize(void)
{
  /* Registration code */

  /* initialize non-finites */
  rt_InitInfAndNaN(sizeof(real_T));

  /* initialize real-time model */
  (void) memset((void *)carga_sf_M, 0,
                sizeof(RT_MODEL_carga_sf_T));

  {
    /* Setup solver object */
    rtsiSetSimTimeStepPtr(&carga_sf_M->solverInfo,
                          &carga_sf_M->Timing.simTimeStep);
    rtsiSetTPtr(&carga_sf_M->solverInfo, &rtmGetTPtr(carga_sf_M));
    rtsiSetStepSizePtr(&carga_sf_M->solverInfo, &carga_sf_M->Timing.stepSize0);
    rtsiSetdXPtr(&carga_sf_M->solverInfo, &carga_sf_M->derivs);
    rtsiSetContStatesPtr(&carga_sf_M->solverInfo, (real_T **)
                         &carga_sf_M->contStates);
    rtsiSetNumContStatesPtr(&carga_sf_M->solverInfo,
      &carga_sf_M->Sizes.numContStates);
    rtsiSetNumPeriodicContStatesPtr(&carga_sf_M->solverInfo,
      &carga_sf_M->Sizes.numPeriodicContStates);
    rtsiSetPeriodicContStateIndicesPtr(&carga_sf_M->solverInfo,
      &carga_sf_M->periodicContStateIndices);
    rtsiSetPeriodicContStateRangesPtr(&carga_sf_M->solverInfo,
      &carga_sf_M->periodicContStateRanges);
    rtsiSetErrorStatusPtr(&carga_sf_M->solverInfo, (&rtmGetErrorStatus
      (carga_sf_M)));
    rtsiSetRTModelPtr(&carga_sf_M->solverInfo, carga_sf_M);
  }

  rtsiSetSimTimeStep(&carga_sf_M->solverInfo, MAJOR_TIME_STEP);
  carga_sf_M->intgData.y = carga_sf_M->odeY;
  carga_sf_M->intgData.f[0] = carga_sf_M->odeF[0];
  carga_sf_M->intgData.f[1] = carga_sf_M->odeF[1];
  carga_sf_M->intgData.f[2] = carga_sf_M->odeF[2];
  carga_sf_M->intgData.f[3] = carga_sf_M->odeF[3];
  carga_sf_M->contStates = ((X_carga_sf_T *) &carga_sf_X);
  rtsiSetSolverData(&carga_sf_M->solverInfo, (void *)&carga_sf_M->intgData);
  rtsiSetSolverName(&carga_sf_M->solverInfo,"ode4");
  rtmSetTPtr(carga_sf_M, &carga_sf_M->Timing.tArray[0]);
  carga_sf_M->Timing.stepSize0 = 0.0001;

  /* block I/O */
  (void) memset(((void *) &carga_sf_B), 0,
                sizeof(B_carga_sf_T));

  /* states (continuous) */
  {
    (void) memset((void *)&carga_sf_X, 0,
                  sizeof(X_carga_sf_T));
  }

  /* states (dwork) */
  (void) memset((void *)&carga_sf_DW, 0,
                sizeof(DW_carga_sf_T));

  /* external inputs */
  (void)memset((void *)&carga_sf_U, 0, sizeof(ExtU_carga_sf_T));

  /* external outputs */
  (void) memset((void *)&carga_sf_Y, 0,
                sizeof(ExtY_carga_sf_T));

  /* Start for TransportDelay: '<S7>/Transport Delay' */
  {
    real_T *pBuffer = &carga_sf_DW.TransportDelay_RWORK.TUbufferArea[0];
    carga_sf_DW.TransportDelay_IWORK.Tail = 0;
    carga_sf_DW.TransportDelay_IWORK.Head = 0;
    carga_sf_DW.TransportDelay_IWORK.Last = 0;
    carga_sf_DW.TransportDelay_IWORK.CircularBufSize = 8192;
    pBuffer[0] = 0.0;
    pBuffer[8192] = carga_sf_M->Timing.t[0];
    carga_sf_DW.TransportDelay_PWORK.TUbufferPtrs[0] = (void *) &pBuffer[0];
    carga_sf_DW.TransportDelay_PWORK.TUbufferPtrs[1] = (void *) &pBuffer[8192];
  }

  /* Start for TransportDelay: '<S6>/Transport Delay' */
  {
    real_T *pBuffer = &carga_sf_DW.TransportDelay_RWORK_d.TUbufferArea[0];
    carga_sf_DW.TransportDelay_IWORK_a.Tail = 0;
    carga_sf_DW.TransportDelay_IWORK_a.Head = 0;
    carga_sf_DW.TransportDelay_IWORK_a.Last = 0;
    carga_sf_DW.TransportDelay_IWORK_a.CircularBufSize = 8192;
    pBuffer[0] = 0.0;
    pBuffer[8192] = carga_sf_M->Timing.t[0];
    carga_sf_DW.TransportDelay_PWORK_j.TUbufferPtrs[0] = (void *) &pBuffer[0];
    carga_sf_DW.TransportDelay_PWORK_j.TUbufferPtrs[1] = (void *) &pBuffer[8192];
  }

  /* Start for TransportDelay: '<S11>/Transport Delay' */
  {
    real_T *pBuffer = &carga_sf_DW.TransportDelay_RWORK_h.TUbufferArea[0];
    carga_sf_DW.TransportDelay_IWORK_n.Tail = 0;
    carga_sf_DW.TransportDelay_IWORK_n.Head = 0;
    carga_sf_DW.TransportDelay_IWORK_n.Last = 0;
    carga_sf_DW.TransportDelay_IWORK_n.CircularBufSize = 8192;
    pBuffer[0] = 0.0;
    pBuffer[8192] = carga_sf_M->Timing.t[0];
    carga_sf_DW.TransportDelay_PWORK_p.TUbufferPtrs[0] = (void *) &pBuffer[0];
    carga_sf_DW.TransportDelay_PWORK_p.TUbufferPtrs[1] = (void *) &pBuffer[8192];
  }

  /* Start for TransportDelay: '<S10>/Transport Delay' */
  {
    real_T *pBuffer = &carga_sf_DW.TransportDelay_RWORK_dx.TUbufferArea[0];
    carga_sf_DW.TransportDelay_IWORK_av.Tail = 0;
    carga_sf_DW.TransportDelay_IWORK_av.Head = 0;
    carga_sf_DW.TransportDelay_IWORK_av.Last = 0;
    carga_sf_DW.TransportDelay_IWORK_av.CircularBufSize = 8192;
    pBuffer[0] = 0.0;
    pBuffer[8192] = carga_sf_M->Timing.t[0];
    carga_sf_DW.TransportDelay_PWORK_o.TUbufferPtrs[0] = (void *) &pBuffer[0];
    carga_sf_DW.TransportDelay_PWORK_o.TUbufferPtrs[1] = (void *) &pBuffer[8192];
  }

  /* InitializeConditions for Integrator: '<S7>/integrator' */
  carga_sf_X.integrator_CSTATE = 0.0;

  /* InitializeConditions for Memory: '<S7>/Memory' */
  carga_sf_DW.Memory_PreviousInput = 1.0;

  /* InitializeConditions for Integrator: '<S6>/integrator' */
  carga_sf_X.integrator_CSTATE_n = 0.0;

  /* InitializeConditions for Integrator: '<S11>/integrator' */
  carga_sf_X.integrator_CSTATE_f = 0.0;

  /* InitializeConditions for Memory: '<S11>/Memory' */
  carga_sf_DW.Memory_PreviousInput_a = 1.0;

  /* InitializeConditions for Integrator: '<S10>/integrator' */
  carga_sf_X.integrator_CSTATE_e = 0.0;

  /* InitializeConditions for TransferFcn: '<Root>/Transfer Fcn3' */
  carga_sf_X.TransferFcn3_CSTATE[0] = 0.0;
  carga_sf_X.TransferFcn3_CSTATE[1] = 0.0;
}

/* Model terminate function */
void carga_sf_terminate(void)
{
  /* (no terminate code required) */
}

/*
 * File trailer for generated code.
 *
 * [EOF]
 */
