/**
 *
 * In adelphos security is managed using trust levels.
 *
 *
 * A person of a given level can see and modify items of all
 * the levels before him.
 *
 * Only the administrator can have super powers.
 *
 *
 *
 *
 */

#ifndef _security_h
#define _security_h

enum ESecurityClearance
{
	E_SEC_GUEST,
	E_SEC_LOGGED,
	E_SEC_ADMIN
};


/* 
 * The rules of visibility in adelphos are these:
 *
 * In L0 all the objects are visibile, there are no secrets.
 *
 * In L0 I have a trust level, which is the equity.
 *
 * */


#endif
