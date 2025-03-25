#include "alloc.h"
#include "lock.h"
char LOCKNAME[32];

#define FALSE 0
#define TRUE  1

#define FLAG_ENTRY_SIZE 1

typedef struct {
	volatile char locked; /* FALSE or TRUE. */
//	char padding[63];
	char padding[1023];
} clh_node_t;
//} __attribute__ ((aligned(64))) clh_node_t;

struct lock_struct {
	clh_node_t *tail;
};

/**
 * These are GCC's magic. Per thread variables.
 * They are initialized to 0 ( = NULL).
 **/
__thread clh_node_t *myNode;
__thread clh_node_t *myPred;

lock_t *lock_init(int nthreads)
{
	strcpy(LOCKNAME,"clh-queue");
	lock_t *lock;
	clh_node_t *tail;
	int i;

	XMALLOC(lock, 1);
	XMALLOC(tail, 1);

	lock->tail = tail;
	lock->tail->locked = FALSE;

	return lock;
}

void lock_free(lock_t *lock)
{
	lock_t *l = lock;
	XFREE(l);
}

void lock_acquire(lock_t *lock)
{
	lock_t *l = lock;

	if (!myNode)
		XMALLOC(myNode, 1);

	myNode->locked = TRUE;
	myPred = __sync_lock_test_and_set(&l->tail, myNode);

	while (myPred->locked == TRUE)
		/* do nothing */ ;
}

/*
Key Concepts:

    Thread-Local Nodes (myNode and myPred):
        Each thread has two thread-local variables:
            myNode: Points to the thread's own node in the queue.
            myPred: Points to the predecessor's node in the queue.
        These are thread-local to ensure threads do not interfere with one another.

    Steps in Lock Acquisition:
        If this thread's node (myNode) hasn’t been allocated yet, allocate it dynamically using XMALLOC.
        Set the locked flag in the thread's node (myNode) to TRUE, signaling that the thread wants the lock.
        Atomically add this thread’s node to the queue by swapping (__sync_lock_test_and_set) the lock’s tail pointer with myNode.
            myPred now holds the previous value of lock->tail, which is the predecessor node.
        Spin (busy-wait) on the predecessor's locked flag (myPred->locked):
            Wait until the predecessor thread releases the lock by setting its locked flag to FALSE.
*/

void lock_release(lock_t *lock)
{
	myNode->locked = FALSE;
	myNode = myPred;
}
