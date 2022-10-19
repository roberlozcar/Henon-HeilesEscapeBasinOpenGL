# Henon-Heiles

## Introduction

The Henon-Heiles system is a non-linear dynamic equations system, which is used to model a star orbiting a galaxy center. This system shows three escapes and it also shows chaos. So, the escape basin of the system is a topic of study in non-linear dynamic field.

The escape basin is a plot in which each axis represents a value of an initial condition, so that, its points represent a set of the initial condition and their colors represent each escape possibility. In this case, we assign energy to the system and we reduce the dimension of the initial conditions set. We represent the two-dimensional position of the star and associate red, green, and blue with each escape; the white points represent no escape.

The system has no closed-form solution so we had to implement a four-order Runge-Kutta method to solve it numerically. This implementation is coded in the compute shader as well as the color buffer, which will be drawn in the others shaders as a texture. Furthermore, we implemented an option to save the buffer as an image.

For more information about this system and the importance of its escape basin see, for example, "Nieto, A. R., Seoane, J. M., & Sanjuán, M. A. (2022). Noise activates escapes in closed Hamiltonian systems. Communications in Nonlinear Science and Numerical Simulation, 105, 106074".

## Options

- You can see the basin with animation (if you press 'y' in the console) or as an image ('n').
- You can choose the energy and the energy step of the animation in the console input.
- The parameter of the solver can be change too in the shader: the step, "h", and the maximum iteration, "maxsteps".

## Installation

Build with CMake and move the bin dlls to the same folder as the compiled executable. 

You may have to move the shaders folder to the parent folder of the executable.

## Requeriments

This project needs glm, glew, glut and freeimage. The headers, libs and dlls for win32 are included in this repository.

## Questions

Write an email to r.lozano93@gmail.com.
