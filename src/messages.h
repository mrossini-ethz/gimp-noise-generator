/* 
 * Noise generator GIMP plugin
 * 
 * messages.h
 * Copyright 2008 by Marco Rossini
 * 
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License version 2+
 * as published by the Free Software Foundation.
 */

#ifndef __MESSAGES_H__
#define __MESSAGES_H__

#define TT_POISSON _("Poisson noise (also called shot noise) arises due to the counting of quantum events. Light is captured in packets (photons) that produces a counting statistic which is described by the poisson distribution.")
#define TT_POISSON_PHOTON _("Determines the number of photons required to increase the pixel value by one. The higher the value the less noisy the result and the longer the processing time.")
#define TT_POISSON_APPROX _("Approximates the Poisson distribution by the normal distribution for high values. This reduces processing speed significantly.")
#define TT_GAUSS _("Gaussian noise is standard distributed noise which is independent from pixel value. Gaussian noise normally arises due to thermal fluctuations in sensors, amplifiers, etc.")
#define TT_GAUSS_STDEV _("Determines the standard deviation of the normal distibution in pixel values. The larger the value the more noise is apparent.")
#define TT_UNIFORM _("Uniform noise has a box distribution and is independent from pixel value. Uniform noise may be produced by quantisation errors when digitizing.")
#define TT_UNIFORM_WIDTH _("The width of the box distribution. The larger the value the more noise is apparent.")
#define TT_UNIFORM_STDEV _("The calculated standard deviation of the uniform distribution.")
#define TT_LAPLACE _("Laplace noise is distributed by the Laplace distribution, which is also called the double exponential distribution.")
#define TT_LAPLACE_SCALE _("Determines the scale parameter of the Laplace distribution. The higher the value the more noise.")
#define TT_LAPLACE_STDEV _("The calculated standard deviation of the laplace distribution.")
#define TT_LORENTZ _("The Lorentz distribution describes resonances and is a (so called) fat-tail distribution, because away from the resonance it goes to zero very slowly. It does not have either mean value nor standard deviation. Lorentz noise produces salt-and-pepper noise.")
#define TT_LORENTZ_GAMMA _("Determines the scale parameter of the Lorentz distribution which is the half width half maximum (HWHM) of the distribution.")
#define TT_LUMINANCE _("Adds noise to the luminance (YCbCr) channel only. This also reduces processing time.")
#define TT_GAMMA _("Converts from sRGB colour space to linear RGB before applying the noise, then back again. This is more physical and increases the noise in dark regions.")
#define TT_RESET _("Resets to the default values.")

#endif /* __MESSAGES_H__ */
