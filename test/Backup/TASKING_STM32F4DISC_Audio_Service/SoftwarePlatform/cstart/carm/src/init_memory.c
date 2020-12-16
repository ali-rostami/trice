/* ------------------------------------------------------------
**
**  Copyright (c) 2013-2015 Altium Limited
**
**  This software is the proprietary, copyrighted property of
**  Altium Ltd. All Right Reserved.
**
**  SVN revision information:
**  $Rev: 14907 $:
**  $Date: 2015-01-19 13:30:51 +0100 (Mon, 19 Jan 2015) $:
**
**  Dummy implementation for __init_memory(). This stub
**  function will only be used if there no other (user)
**  implementation of __init_memory().
**
** ------------------------------------------------------------
*/

#pragma weak __init_memory

extern void __init_memory ( void )
{
}