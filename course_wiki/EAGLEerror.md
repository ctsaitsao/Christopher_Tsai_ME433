EAGLE can do automatic error checking on both schematics and boards. We will not worry about automatically checking for errors in the schematic (that's on you), but the manufacturer's tolerances will determine how close traces are allowed to be on the board. Common tolerances are 6mils. Traces less wide than 6mils might not be continuous, and copper traces with less than a 6mil gap between them might have a short.

Our manufacturer provides a tolerance file for EAGLE to use: SeeedStudio_2layer_DRU_no_angle_20140221.dru. Download it from git and add it to your project folder. You can also get if from [Seeed](http://support.seeedstudio.com/knowledgebase/articles/422482-fusion-pcb-pcba-order-submission-guidelines)

In the .brd, click the DRC button (Design Rule Check). Click the Load button in the bottom right, and select the Seeed .dru file. Click the Check button, and EAGLE will highlight traces that accidentally touch or are too close. Those are important errors to fix. EAGLE will also highlight potential problems, like silkscreen going over holes or pads, which you can ignore.

Back to the [Schedule](https://github.com/ndm736/ME433_2019/wiki/Schedule)