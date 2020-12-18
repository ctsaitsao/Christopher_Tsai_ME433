When your .brd looks complete, generate the Gerber files using the CAM generator. Gerber files come in a few flavors, so we need to make sure we generate the right type.

We are using SeeedStudio to manufacture our boards. Seeed provides [guidelines](http://support.seeedstudio.com/knowledgebase/articles/422482-fusion-pcb-pcba-order-submission-guidelines) listing the file type they need:
* Top Layer: pcbname.GTL
* Top Solder Mask: pcbname.GTS
* Top Silkscreen: pcbname.GTO
* Bottom Layer: pcbname.GBL
* Bottom Solder Mask﻿: pcbname.GBS
* Bottom silkscreen: pcbname.GBO
* Board Outline:pcbname.GML/GKO
* Drills: pcbname.TXT

Seeed provides a file that EAGLE uses to generate these files, Seeed_Gerber_Generater_2-layer.cam. Copy it from git and put it in your project folder.

In EAGLE, select File->Open->Job, and select the Seeed cam file. Note that there are 8 tabs - each tab will generate a file. Within each tab is a list of layers that will be placed into the file. The Top layer should have the top copper (red layer). The Bottom layer should have the bottom copper (blue layer) and so on. 

The only layers you might want to change are the silkscreen layers. The component names and values are stored in the tNames and tValues (or bNames and bValues) layers, and are not included in the silkscreen files by default. You usually want those, so select them in the tabs.

Click Ok, and the files will be generated and added to your project folder. Combine them in a zip file.

View your zip file using [Mayhew Labs](http://mayhewlabs.com/webGerber/) or loading it into [Seeed](https://www.seeedstudio.com/fusion_pcb.html) to double check that your gerber files match your design. Sometimes the silkscreen layer gets stretched. If text moved off the edge of your board, change your .brd file so that when it gets stretched it doesn't fall off.


Back to the [Schedule](https://github.com/ndm736/ME433_2019/wiki/Schedule)