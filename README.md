# An M5Stack Arduino IDE example for simple digital low-pass filters

## Purpose
This is my practice to implement a constant-period timer interruptiun as well as IIR digital filters.

## Usage
When run, the screen shows a yellow line on the top and a cyan line on the bottom. Actually another one, the pink line is overlapped and obscured by the cyan line. The yellow line corresponds to the input signal whearas the pink and cyan lines correspond to the output signals.

<img src="M5-LPFs.jpg" width=400>

Pushing Button A makes the input signal jump from zero to unity. In other words, you can make a unit step by pushing Button A. The pink and cyan lines on the bottom show the responses of the first- and second-order low-pass filters, respectively.

## Implementation
The low-pass filters are implemented as C++ classes.
