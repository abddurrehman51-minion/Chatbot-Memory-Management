#ifndef COMPACTION_H
#define COMPACTION_H
#include "history.h"

Message **compaction_build_array(History *h);
Node *compaction_build_list(Message **arr, int count);
void compaction_free_old_storage(History *h);

int compaction_replace_storage(History *h,
                               Message **arr,
                               Node *list);

int compaction_run(History *h);
#endif
