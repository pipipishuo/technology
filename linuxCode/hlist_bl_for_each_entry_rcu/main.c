#include<stdio.h>
#define hlist_bl_for_each_entry_rcu(tpos, pos, head, member)		\
	for (pos = hlist_bl_first_rcu(head);				\
		pos &&							\
		({ tpos = hlist_bl_entry(pos, typeof(*tpos), member); 1; }); \
		pos = rcu_dereference_raw(pos->next))
	
int main(){
	hlist_bl_for_each_entry_rcu(dentry, node, b, d_hash);
	
	
	return 0;
}
