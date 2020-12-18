The first part of designing a printed circuit board (PCB) is defining which components go on the board, and how they connect to one another. The schematic (.sch) file stores this information graphically.

After opening EAGLE, create a new project (inside of your repo of course, probably in a folder named HW2), and inside of the project make a new schematic.

To add a component, click the Add Component button (it looks like an op amp). A window will pop up and allow you to look through many libraries to find to part you want to add. The built-in libraries do not have every component, so we will cover making components in a custom library in a bit. For now, find a resistor in the "resistor" library. For some reason there are resistors, capacitors, and inductors in the "resistor" library. There are also US and EU versions of the components (they draw them weird in Europe). Go into R-US_. Notice that all the resistors have the same schematic, but there are many different packages, or physical footprint layouts. Pick the 1/4W through hole resistor (R-US_0207/10), and add it to your schematic. Obviously it is easy to pick the wrong sized part, so later you will print (on paper) a 1:1 version of your design, and put an actual resistor on top of the image to double check that it will fit.

Also go and find a 3mm LED (in the "led" library, under the LED part, called LED3MM), and the symbols for 3.3V and GND (in the "supply1" library).

To connect the LED to 3.3V, use the "Net" tool (green line), click on the line end of the 3.3V symbol, then click on the positive end of the LED. **Don't use the "Wire" tool (a black line), as it is meant to draw lines, not electrical connections.**

Connect the other side of the LED to the resistor, and the resistor to GND.

Note that if you switch to the "Move" tool (looks like a "plus" with arrows), and click a component and move it, the green electrical connections move with it. If the connections stay, they were not properly attached.

To move multiple parts at one, you must first select all of the parts with the "Group" tool (looks like an hashed square), then switch to the move tool, and simultaneously right-click the group while holding alt to move all of the parts at once.

Nets can get quite complicated. Nets that cross, without dots at the intersection, are not connected. To avoid crossing nets and getting confused, you can name a net, and any other net with the same name will be connected (without drawing the line between them), making the schematic much more readable. Try it out - delete the net between the LED and resistor. Draw a net from each, to no where. Use the "Label" tool on each dangling net to identify the net, then use the "Name" tool on each label, calling them LEDconnect. When you name the second, a pop-up will ask if you meant to connect the nets. Say yes, and the connection is made.

Now you are ready to layout the PCB!


Back to the [Schedule](https://github.com/ndm736/ME433_2019/wiki/Schedule)