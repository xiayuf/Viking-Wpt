/*
* [R] O Y A L   [H] A C K I N G   [T] O O L Z
*
* Coded by R15
*
* This program is free software; you can redistribute it and/or modify
* it under the terms of the GNU General Public License as published by
* the Free Software Foundation; either version 2 of the License, or
* (at your option) any later version.
*
* This program is distributed in the hope that it will be useful,
* but WITHOUT ANY WARRANTY; without even the implied warranty of
* MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
* GNU General Public License for more details.
* 
* You should have received a copy of the GNU General Public License
* along with this program; if not, write to the Free Software
* Foundation, Inc., 675 Mass Ave, Cambridge, MA 02139, USA.
* 
* GNU license doesn't cover Engine directory.
* Content of Engine directory is copyrighted (c) 1999, 2000, by Valve LLC
* and it is licensed under Valve's proprietary license (see original HL SDK).
*/

#include	<windows.h>
#include	<stdio.h>
#define	MSL	1024

//
// TinyStrings functions by Plastique:
//

//---------------------------------------------------------
int	TinyStrings_CountWords(char * cmd_arg)
{
char	* j;
int		count = 0;

		if (!cmd_arg) return 0;

		j = cmd_arg;

		while (*j) {
			
			// Skip spaces
			while (*j && isspace(*j)) j++;

			if (!*j) break;

			count++;
			while (*j && !isspace(*j)) j++;
		}
		return count;
}

//---------------------------------------------------------
char *	TinyStrings_GetWord(char * temp,char * cmd_arg, int number)
{
char	* j, *i;
int		count = 0;
         
		if (!cmd_arg) return NULL;
        memset(temp,0,MSL);
		j = cmd_arg;

		if (number >= TinyStrings_CountWords(cmd_arg)) return "";

		i = temp;
		*i = 0;

		while (*j) {
			// Skip spaces
			while (*j && isspace(*j)) j++;

			if (count == number) {
				while (*j && !isspace(*j)) *(i++) = *(j++);
				*i = 0;
				return temp;
			}

			count++;
			while (*j && !isspace(*j)) j++;
		}
		return "";
}
//---------------------------------------------------------
char *	TinyStrings_GetWordPointer(char * cmd_arg, int number)
{
char	* j;
int		count = 0;

		if (!cmd_arg) return NULL;

		j = cmd_arg;

		if (number >= TinyStrings_CountWords(cmd_arg)) return "";

		while (*j) {
			// Skip spaces
			while (*j && isspace(*j)) j++;

			if (count == number) return j;

			count++;
			while (*j && !isspace(*j)) j++;
		}
		return "";
}
