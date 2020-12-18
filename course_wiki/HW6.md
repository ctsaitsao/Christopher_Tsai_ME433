HW6

* Create an algorithm to take an ascii character, like 'H' (also represented by decimal 72), and draw it so that the top left corner is at any pixel location, using `LCD_drawPixel` to write the ASCII matrix contents to the LCD. For example, the first byte in the image is 0x7f, or 0b01111111. So the pixel at (x,y) should be off, (x,y+1) should be on, (x,y+2) should be on, and so on. The second byte is 0x08, or 0b00001000, so the pixel at (x+1,y) should be off (and so on) and the pixel at (x+1,y+1) should be off. Do this for every byte in ASCII for the character your are displaying. Be sure to check if the pixel location exists before trying to write to it (if the start location is (235,325) you will run out of room in both the x and y directions). 
* When you have written a function that can draw one character to the LCD, make a char array with sprintf(), and call your function in a loop to display every character in the string.   
* Also write a function that can draw a progress bar, using a start position and a length.
* Make a new folder called HW6 in your git folder and add your code. Make a video of the LCD showing "Hello world %d!" starting at (28,32), and below it a progress bar with the length of the displayed integer. Increment the integer from 0 to 100 at 10Hz. Show a live frames-per-second reading on your screen.  
* Just for reference, how many characters can be displayed on the screen at the same time? How long does it take to fill an entire row with characters?   

Hints:  
* The character array `char message[10]; sprintf(message,"hello");` contains 6 characters ('h' 'e' 'l' 'l' 'o' '/0') where '/0' is decimal 0. So, if you loop through message, you know to stop when you get to a '/0': `int i = 0; while(message[i]){ display_character(array[i]); i++;}`  
* The characters in ASCII start at 0x20 in the ascii table, so if you know the character you want to display, its location in ASCII is its decimal value minus 0x20.  

Back to the [Schedule](https://github.com/ndm736/ME433_2019/wiki/Schedule)