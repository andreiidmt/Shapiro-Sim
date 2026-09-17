# Shapiro-Sim
A simulator/lab application developed in C++ where the user can play around and see the effects of Shapiro Time Dilation.

NOTE: If you only wish to use the application and do not want to look at the source code, click here: https://github.com/andreiidmt/Shapiro-Sim/releases/tag/v1.0

# Concept
This application was made to showcase the effects of Shapiro Time Dilation, the fourth classical test of general relativity which measures the extra travel time it takes for a light or radar signal to pass near a massive object like the Sun.
As per Albert Einstein's theory, massive objects bend the curvature of space-time. As such, light traveling past these massive objects will suffer a delay as the length of their path can increase. This test DOES NOT work for neutrinos or gravitational waves.

# Application review

The application features a very simple yet modern interface: on the left can be found the dashboard, where the user can change parameters and see how those affect the final analytical results; on the right we can find a graphical representation portraying said parameters(please note that the visual representation is not to scale, and the massive object sits at the center of the screen at all times; this was made for the ease of computations).

<img width="1917" height="1110" alt="image" src="https://github.com/user-attachments/assets/b795b410-e1de-4b46-81b3-71000aae7f13" />

IMPORTANT: While the original Shapiro Time Delay test(which was only proposed in 1964) did NOT take into account black holes, I have decided to use a non-rotating Schwarzschild black hole to portray that light can be captured and therefore, never arrive at its destination. In today's age, however, theoretical physicists use the Shapiro delay equations to calculate how light bends and delays near different types of black holes (like spinning Kerr black holes or charged Reissner-Nordström holes) to see if we can tell them apart by their light echoes.

# Controls

LMB + Drag - Drag the camera around.
MWS Up - Zoom in.
MWS Out - Zoom out.



