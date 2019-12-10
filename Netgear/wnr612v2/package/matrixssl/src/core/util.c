/*
 *	util.c
 *	Release $Name: MATRIXSSL-3-1-2-OPEN $
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

#include "coreApi.h"

/******************************************************************************/
/*
	Creates a simple linked list from a given stream and separator char
	
	Memory info:
	Callers do not have to free 'items' on function failure.
*/
int32 psParseList(psPool_t *pool, char *list, const char separator,
		psList_t **items)
{
	psList_t	*litems, *start, *prev;
	uint32		itemLen, listLen;
	char		*tmp;

	*items = NULL;
	
	listLen = (int32)strlen(list) + 1;
	if (listLen == 1) {
		return PS_ARG_FAIL;
	}
	start = litems = psMalloc(pool, sizeof(psList_t));
	if (litems == NULL) {
		return PS_MEM_FAIL;
	}
	memset(litems, 0, sizeof(psList_t));

	while (listLen > 0) {
		itemLen = 0;
		tmp = list;
		if (litems == NULL) {
			litems = psMalloc(pool, sizeof(psList_t));
			if (litems == NULL) {
				psFreeList(start);
				return PS_MEM_FAIL;
			}
			memset(litems, 0, sizeof(psList_t));
			prev->next = litems;
			
		}
		while (*list != separator && *list != '\0') {
			itemLen++;
			listLen--;
			list++;
		}
		litems->item = psMalloc(pool, itemLen + 1);
		if (litems->item == NULL) {
			psFreeList(start);
			return PS_MEM_FAIL;
		}
		litems->len = itemLen;
		memset(litems->item, 0x0, itemLen + 1);
		memcpy(litems->item, tmp, itemLen);
		list++;
		listLen--;
		prev = litems;
		litems = litems->next;
	}
	*items = start;
	return PS_SUCCESS;
}

void psFreeList(psList_t *list)
{
	psList_t	*next, *current;

	if (list == NULL) {
		return;
	}
	current = list;
	while (current) {
		next = current->next;
		if (current->item) {
			psFree(current->item);
		}
		psFree(current);
		current = next;
	}	
}

/******************************************************************************/

