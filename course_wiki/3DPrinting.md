Design for 3d printing  

3d printing, or fused filament fabrication, has recently become an inexpensive and relatively reliable form of rapid prototyping (I use "rapid" here because it would take a few days to make these types of parts on a milling machine - on a printer it takes hours, so the "rapid" is relative).  

We have several 3d printers from [Monoprice](https://www.monoprice.com/Product?p_id=15710) available in the Mechatronics Lab, as well as some Ultimakers in the Corner Makery, NxR, and MSR labs. They all have similar performance, and print in PLA (which smells less and is less likely to warp than ABS).  

Not every part can be 3d printed. For instance, consider trying to print the capital letter 'T'. If you were to try to print it as drawn, the first few layers would collapse. Instead it would be better to print it upside down. If you were to print a 'V', it should work as long as the angle is less than 45 degrees, depending on the printer. Sometimes you cannot orient your part to be printed without an overhang, so support structure is added. You can do this in your CAD design, or have the printer software add it automatically.  

The printers we use have a single nozzle, so they cannot print a dissolvable support structure. Instead, software is used to generate a support of a different density that should be easy to remove. We use [Simplify3D](https://www.simplify3d.com/) in the Mechatronics lab. Instructions are posted above the printers.  

The printing process is slow. You part is "sliced" into 0.2mm tall 2d parts, and each layer is printed on top of the one below it. Sometimes a "raft" is added to give your part more surface area with the plate. Some plates are covered with sacrificial blue painters tape for the right amount of adhesion (try not to waste the tape, it is expensive), while others use a heated bed.  

The printer software gives you the option of printing hollow parts - you choose the wall thickness and internal density. The strength scales linearly with density (about 5%) and our parts usually do not need to be that strong, so consider printing at 15% density to save time.  

PLA does not glue very well, so plan on attaching parts with screws, snaps, or geometry.  

The print tends to be bigger than you planned, by about 0.02" in every direction (so your part gets bigger, and holes get smaller).  

Back to the [Schedule](https://github.com/ndm736/ME433_2019/wiki/Schedule) 