# ray-fabrik
Forwards and Backwards Reaching Inverse Kinematics (FABRIK) implemented in raylib/C. It uses raymath-module's Vector2 math to calculate the algorithm.
![Diagram](/img/diagram.png)
This diagram shows an example of the fabrik algorithm, forward loop in blue, backward in red. 
- Move end point to target
- Draw a vector from target to start point
- Set joint's end point the correct distance away on the new vector line.
- Set target position to the new point, move on to next joint.
- Once at root, repeat but in backwards order starting in root.
Multiple iterations will converge to a close answer.
