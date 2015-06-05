/* 
  command.cpp - esp8266 configuration class

  Copyright (c) 2014 Luc Lebosse. All rights reserved.
 
  This library is free software; you can redistribute it and/or
  modify it under the terms of the GNU Lesser General Public
  License as published by the Free Software Foundation; either
  version 2.1 of the License, or (at your option) any later version.

  This library is distributed in the hope that it will be useful,
  but WITHOUT ANY WARRANTY; without even the implied warranty of
  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
  Lesser General Public License for more details.

  You should have received a copy of the GNU Lesser General Public
  License along with this library; if not, write to the Free Software
  Foundation, Inc., 51 Franklin St, Fifth Floor, Boston, MA  02110-1301  USA
*/

#include "command.h"

String COMMAND::buffer_serial;
String COMMAND::buffer_tcp;

void COMMAND::execute_command(int cmd,String cmd_params)
{
	//manage parameters
	
	switch(cmd)
    {
		case 800:
			Serial.println("\nCommand received");
		break;
		//default:

	}
}

void COMMAND::check_command(String buffer)
{
	String ESP_Command;
	//look for ESP command
	//is there a first part ?
	int ESPpos = buffer.indexOf("[ESP");
	if (ESPpos>-1)
		{//is there the second part?
		int ESPpos2 = buffer.indexOf("]",ESPpos);	
		if (ESPpos2>-1)
			{	//Split in command and parameters
				String cmd_part1=buffer.substring(ESPpos+4,ESPpos2);
				String cmd_part2="";
				//is there space for parameters?
				if (ESPpos2<buffer.length())
					{
					cmd_part2=buffer.substring(ESPpos2+1);
					}
				//if command is a valid number then execute command
				if(atoi(cmd_part1.c_str())!=0)execute_command(atoi(cmd_part1.c_str()),cmd_part2);
				//if not is not a valid [ESPXXX] command
			}
		}
	//TODO look for response from printer not using [ESPXXX] format
}

//read a buffer in an array
void COMMAND::read_buffer_serial(uint8_t *b, size_t len)
{
for (long i; i< len;i++)
	{
	read_buffer_serial(*b);
	*b++;
	}
}

//read buffer as char
void COMMAND::read_buffer_tcp(uint8_t b)
{
static bool previous_was_char=false;
//to ensure it is continuous string, no char separated by binaries
if (!previous_was_char)buffer_tcp="";
//it is a char so add it to buffer
if (isPrintable(b))
	{
	previous_was_char=true;
	buffer_tcp+=char(b);
	}
else 
	{
	previous_was_char=false; //next call will reset the buffer
	}
//this is not printable but end of command check if need to handle it
if (b==13 ||b==10)
	{//Minimum is something like M10 so 3 char
	if (buffer_tcp.length()>3)
		check_command(buffer_tcp);
	}
}

//read buffer as char
void COMMAND::read_buffer_serial(uint8_t b)
{
static bool previous_was_char=false;
//to ensure it is continuous string, no char separated by binaries
if (!previous_was_char)buffer_serial="";
//it is a char so add it to buffer
if (isPrintable(b))
	{
	previous_was_char=true;
	buffer_serial+=char(b);
	}
else 
	{
	previous_was_char=false; //next call will reset the buffer
	}
//this is not printable but end of command check if need to handle it
if (b==13 ||b==10)
	{//Minimum is something like M10 so 3 char
	if (buffer_serial.length()>3)
		check_command(buffer_serial);
	}
}
