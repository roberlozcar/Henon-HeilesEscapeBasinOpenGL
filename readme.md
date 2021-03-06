# Henon-Heiles

## Introduction

The Henon-Heiles system is a non-linear dynamic equations system, which is used to model a star orbiting a galaxy center. This system shows three escape and, furthermore, it also shows chaos. So that, the escape basin of the system is a topic of study in non-linear dynamic field.

The escape basin is an plot in which each axis represents a value of an initial condition, so that, its points represents a set of initial condition and their colors represent each escape possibility. In this case we assign an energy to the system and we reduce the dimension of the initial conditions set. We represent the two dimensional position of the star and associate red, green and blue to each escape; the white points represent no escape.

The system has no closed-form solution so that we implemented a four order Runge-Kutta method to solve it numerically. This implementation is coded in the compute shader as well as the color buffer, that will be drawn in the others shaders as a texture. Furthermore, we implemented an option to save the buffer as an image.

For more information about this system and the importance of its escape basin see, for example, "Nieto, A. R., Seoane, J. M., & Sanju?n, M. A. (2022). Noise activates escapes in closed Hamiltonian systems. Communications in Nonlinear Science and Numerical Simulation, 105, 106074".

## Options

- The energy can be changed in the variable in the compute shader, so that it will not be necessary to recompile the project to change it.
- The parameter of the solver can be change too in the shader: the step, "h", and the maximum iteration, "maxsteps".

## Requeriments

This project needs glm, glew, glut and freeimage. The headers, libs and dlls for win32 are included in this repository.

## Questions

Write an email to r.lozano93@gmail.com.