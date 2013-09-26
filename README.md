Keep-Your-Edge-Level-Editor
===========================

Level editor for Keep Your Edge

# Commands
[ and ] scroll the page
+ and - change the size of the object
s saves the current pattern to pattern.xml in the data directory
backspace will delete the last object placed

# Nifty
If you drag and drop a pattern XML file into the level editor window, it'll load that file.

# .level file
The level file contains all the sets of patterns you want for a level. It'll cycle through them in order.

```
<name>Square Test</name>
<author>Clay Ewing</author>
<complete>20</complete>
<speed>-1</speed>
<multiplier>1.01</multiplier>
<file>small.xml</file>
```
