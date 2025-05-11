# Syscalls Only to 2KB of RAM
Using Syscalls to write to the SSD1306 screen, no buffers.<br/>
This is for an ATmega328PB, so a pretty simple circuit board.<br/>
The I2C and Interrupts handler are written in AVR assembly<br/>

Note: syscalls i refer to as commands (because that is basically what the i2c_write is).<br/>
This is using syscalls or i2c_write commands to write to different memory sections<br/>
e.g., the SSD1306 screen, which uses commands to write to its screen in the memory,<br/>
instead of using giant buffers like a 1024-byte buffer, which would take up 50% of the<br/>
available RAM (We have 2KBs of RAM). There is also a character and text writing function<br/>
which is also written using syscalls.

If you enter the main.c you will also see me writing pixels. That's because I connected a<br/>
playstation controller (only the knob, an external component) via jumper cables.<br/>

Pretty fun project, and the method of writing pixels via commands instead of wasting RAM<br/>
was told to me by my teacher, who programmed phone hardware in the earlier days.

Have a wonderful day.
