#ifndef _SGE_RESOURCE_QUOTA_SCHEDD_H
#define __SGE_RESOURCE_QUOTA_SCHEDD_H
/*___INFO__MARK_BEGIN__*/
/*************************************************************************
 * 
 *  The Contents of this file are made available subject to the terms of
 *  the Sun Industry Standards Source License Version 1.2
 * 
 *  Sun Microsystems Inc., March, 2001
 * 
 * 
 *  Sun Industry Standards Source License Version 1.2
 *  =================================================
 *  The contents of this file are subject to the Sun Industry Standards
 *  Source License Version 1.2 (the "License"); You may not use this file
 *  except in compliance with the License. You may obtain a copy of the
 *  License at http://gridengine.sunsource.net/Gridengine_SISSL_license.html
 * 
 *  Software provided under this License is provided on an "AS IS" basis,
 *  WITHOUT WARRANTY OF ANY KIND, EITHER EXPRESSED OR IMPLIED, INCLUDING,
 *  WITHOUT LIMITATION, WARRANTIES THAT THE SOFTWARE IS FREE OF DEFECTS,
 *  MERCHANTABLE, FIT FOR A PARTICULAR PURPOSE, OR NON-INFRINGING.
 *  See the License for the specific provisions governing your rights and
 *  obligations concerning the Software.
 * 
 *   The Initial Developer of the Original Code is: Sun Microsystems, Inc.
 * 
 *   Copyright: 2001 by Sun Microsystems, Inc.
 * 
 *   All Rights Reserved.
 * 
 ************************************************************************/
/*___INFO__MARK_END__*/

#include "sgeobj/sge_resource_quota.h"

bool rqs_set_dynamical_limit(lListElem *limit, lListElem *global_host, lListElem *exec_host, lList *centry);

int debit_job_from_rqs(lListElem *job, lList *granted, lList *rqs_list, lListElem *pe, lList *centry_list, lList *acl_list, lList *hgrp_list);

void rqs_expand_cqueues(const lListElem *rule, lList **skip_cqueue_list);
void rqs_expand_hosts(const lListElem *rule, lList **skip_host_list, const lList *host_list, lList *hgrp_list);

bool is_cqueue_global(const lListElem *rule);
bool is_host_global(const lListElem *rule);

bool is_cqueue_expand(const lListElem *rule);
bool is_host_expand(const lListElem *rule);

#endif /* __SGE_RESOURCE_QUOTA_SCHEDD_H*/