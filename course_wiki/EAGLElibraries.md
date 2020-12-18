You can't add a part in EAGLE if it doesn't exist in a library. There are many libraries included in EAGLE, you can see them in the Control Panel window by opening the "Libraries" tree structure. EAGLE libraries have the file type .lbr, and can be specific to a manufacturer, or a generic part type (like resistor or LED). They can have many parts inside, or just one.

It is usually a good idea to make a library before making your .sch, so that if you share your EAGLE design with someone else, you can give them a library that contains all of the parts in your design. If your design uses a part from a library that you do not share, then the person receiving your file may not be able to edit errors in the part they do not have.

To make a library, right click your project and say New->Library. The library window will open up. This window has 3 panes: Device, Package, and Symbol.

A Symbol is what appears in the schematic. A component will have only one Symbol.

A Package is the physical footprint of the component. A component could have several Packages (like a DIP version and a surface mount version, or different sizes of surface mount).

A Device is the link between the Symbol and the Package, which connects the pins from the Symbol (like VDD, VSS, VOUT, etc) to the leg of the chip (like pin 1, pin 2, etc). The Device is what you add to your .sch file, where you specify which specific Package will also be added to the .brd file. (You can always change which Package to put in the .brd file from the .sch file, but try to do it before you start working on the .brd file because it might mess up the copper in the .brd.)

The first thing to do when making a library is to go steal all of the parts that already exist. While your library is open in the library window, go back to the Control Panel window and open the Libraries tree structure. Find the "resistor" library, open its tree structure, and right-click on "R-US_" and say "Copy to Library." This will copy the R-US_ Symbol, all of the R-US_ Packages, and the R-US_ Device to your library. Also copy C-US, CPOL-US, LED from the led library, +3V3, +5V, and GND from the supply1 library. 

Some parts, like the PIC32, don't exist, but the Package, a 10mm 44-pin TQFP, do exist, saving time so you don't have to remake (and potentially screw up) the Package. 

Some parts exist in libraries that other people have made, and we might as well go grab those too. [Sparkfun](https://github.com/sparkfun/SparkFun-Eagle-Libraries) has some nice parts. Download the zip file from their git page, unzip the folder, and [put them in a folder and add the folder to Eagle](https://www.autodesk.com/products/eagle/blog/library-basics-install-use-sparkfun-adafruit-libraries-autodesk-eagle/). Grab the USB device from the Sparkfun- (something) library, as well as the (linear holes) parts (these are 0.1 inch spaced holes, convenient for header pins).

The last part we need is the voltage regulator. It does not exist, but the Package does. Go the the microchip library, open the Packages tree, and copy the TO92 package.

Now it is time to start making parts. In the Library window, click the "Show table of contents" button (looks like an open book). This will show you all of the parts you copied. Click the "Symbol" button (looks like an op-amp), and in the "New" area, type the name of the component you want to make, in this case the voltage regulator L4931, and say yes to create a new symbol. In this window, you will create the Symbol for the voltage regulator that will appear in your .sch. Click the "Draw a Pin" button (red line with a box) and add three pins. The circle part of the pin is where you make net connections, so have those face out. Voltage regulators typically have the VIN on the left, GND facing down, and VOUT on the right. Note the plus sign in the middle of the page, that should be the middle of your Symbol (its also where you will click on the Symbol to move it around in the sch).

After dropping the pins, use the "Name" tool to rename the pins. Give them names like "1.GND", "2.VIN", so that you will be able to remember that pin 1 is GND, etc.

Move the pins so that the names to no overlap.

Use the "Wire" tool to draw a box around the pins, leaving the circles hanging out, to define your part.

Use the "Text" tool to drop a text with ">NAME" and ">VALUE" at the top of your box. EAGLE will autopopulate your .sch file, replacing >NAME with L4931 and >VALUE with U1 (or some number) if you use these special names.

The symbol is done! Go back to the Table of Contents. We already copied the Package for the L4931, its the TO92. Open the TO92 Package.

Use the "Info" button to identify which green hole is 1, 2, and 3. This is the top down view. Does it match the datasheet?

Now it is time to create the Device, and link the TO92 Package to the L4931 Symbol. Click the "Device" button (looks like two op-amps), and in the "New" area type L4931, and click yes to make a new Device. On the left of this window, click the "Add" button, and select your L4931 symbol, and drop it on the plus sign in the middle of the pane on the left. 

Then click the "New" button at the bottom of the pane on the right, and select the TO92 Package. 

Then click the "Connect" button, and carefully, using the datasheet, Connect the Pin name from the Symbol to the correct Pad number. When you are done, click OK.

Save your library and open your .sch file. To be able to use your library, go to Library->Use..., and select the library you created.

If you ever change your library, when you go back to your .sch or .brd, go to Library->Update..., and select your library, and the changes will be applied.

Now, go ahead and make the rest of your parts!

Back to the [Schedule](https://github.com/ndm736/ME433_2019/wiki/Schedule)