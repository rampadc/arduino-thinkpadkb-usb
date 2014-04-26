# Introduction
Using an Arduino Micro as a USB adapter for ThinkPad T60 keyboard with TrackPoint support. 
Supports different keyboards from the ThinkPad family that shares the same receptacle as T60.

Photo: Modular approach that splits the circuit into the microcontroller side and receptacle + supporting ICs side.
![](/doc/AdapterTest.jpg)

Part list:
+ 1x	Arduino Micro
+ 16x	0805 10k resistors
+ 1x	MCP23S17-E/SO
+ 1x	CD74HC4017EE4

# Files

Library: TrackPointExtended.h, TrackPointExtended.cpp, MCP23S17_InputsOnly.h, MCP23S17_InputsOnly.cpp

Example: MicroThinkPadAdapter.ino

# License

   Copyright 2014 CONG NGUYEN

   Licensed under the Apache License, Version 2.0 (the "License");
   you may not use this file except in compliance with the License.
   You may obtain a copy of the License at

       http://www.apache.org/licenses/LICENSE-2.0

   Unless required by applicable law or agreed to in writing, software
   distributed under the License is distributed on an "AS IS" BASIS,
   WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
   See the License for the specific language governing permissions and
   limitations under the License.