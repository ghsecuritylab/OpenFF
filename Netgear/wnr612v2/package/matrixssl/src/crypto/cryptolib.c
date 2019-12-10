/*
 *	cryptolib.c
 *	Release $Name: MATRIXSSL-3-1-2-OPEN $
 *
 *	Utilities and crypto library control
 */
/*
 *	Copyright (c) PeerSec Networks, 2002-2010. All Rights Reserved.
 *	The latest version of this code is available at http://www.matrixssl.org
 *
 *	This software is open source; you can redistribute it and/or modify
 *	it under the terms of the GNU General Public License as published by
 *	the Free Software Foundation; either version 2 of the License, or
 *	(at your option) any later version.
 *
 *	This General Public License does NOT permit incorporating this software 
 *	into proprietary programs.  If you are unable to comply with the GPL, a 
 *	commercial license for this software may be purchased from PeerSec Networks
 *	at http://www.peersec.com
 *	
 *	This program is distributed in WITHOUT ANY WARRANTY; without even the 
 *	implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. 
 *	See the GNU General Public License for more details.
 *	
 *	You should have received a copy of the GNU General Public License
 *	along with this program; if not, write to the Free Software
 *	Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA  02111-1307  USA
 *	http://www.gnu.org/copyleft/gpl.html
 */
/******************************************************************************/

#include "cryptoApi.h"

/******************************************************************************/
/*
	Utility functions
*/
/******************************************************************************/
#ifdef USE_BURN_STACK
void psBurnStack(uint32 len)
{
	unsigned char buf[32];
	
	memset(buf, 0x0, sizeof(buf));
	if (len > (uint32)sizeof(buf)) {
		psBurnStack(len - sizeof(buf));
	}
}
#endif /* USE_BURN_STACK */

/******************************************************************************/

