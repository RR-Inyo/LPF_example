/*
  dFilt.hpp
  My digital filter header file
  (c) 2021 @RR_Inyo
  Released under the MIT lisence
  https://opensource.org/licenses/mit-license.php
*/

#ifndef _DFILT_
#define _DFILT_

// First-order low-pass filter class
class FirstOrderLPF {
  public:
    FirstOrderLPF(float Ts, float omega_n);
    ~FirstOrderLPF(void);
    float apply(float u);

  private:
    float a1, b0, b1;
    float y_old, u_old;
};

// Second-order low-pass filter class
class SecondOrderLPF {
  public:
    SecondOrderLPF(float Ts, float zeta, float omega_n);
    ~SecondOrderLPF(void);
    float apply(float u);

  private:
    float m, n;
    float a1, a2, b0, b1, b2;
    float y_old[2], u_old[2];
};

#endif
