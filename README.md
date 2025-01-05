Work in Progress

The goal is to have bidirectional communication between two Wemos D1 Minis (using a third as a WiFi access point). Each of the two main devices would have an LCD and a button matrix to input characters (using double press to have every character available).

See Project Demo - Edited.mov inside the Media including Demo folder for a brief video demonstration. Note: the 3dp keyboard is not shown in the video. Yes, the video was edited to make it fit under github's file size requirements.

Notes:
- Unidirectional communication is pretty good
- 3d printed case for button matrix with keycaps (letter in a separate color)
- Button matrix uses a voltage ladder to use less pins on the Wemos
- Using I2C LCDs
