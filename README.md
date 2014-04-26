# Introduction
Using an Arduino Micro as a USB adapter for ThinkPad T60 keyboard with TrackPoint support. 
Supports different keyboards from the ThinkPad family that shares the same receptacle as T60.

Photo: Modular approach that splits the circuit into the microcontroller side and receptacle + supporting ICs.
![](/doc/AdapterTest.jpg)

# Files

Library: TrackPointExtended.h, TrackPointExtended.cpp, MCP23S17_InputsOnly.h, MCP23S17_InputsOnly.cpp

Example: MicroThinkPadAdapter.ino

# License
Copyright (c) 2014 Cong Nguyen

Permission is hereby granted, free of charge, to any person obtaining a copy
of this software and associated documentation files (the "Software"), to deal
in the Software without restriction, including without limitation the rights
to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
copies of the Software, and to permit persons to whom the Software is
furnished to do so, subject to the following conditions:

The above copyright notice and this permission notice shall be included in
all copies or substantial portions of the Software.

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
THE SOFTWARE.
