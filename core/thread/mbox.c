/*
 * mbox.c
 *
 * Simple thread mailbox interface
 */

#include "thread.h"
#include "mbox.h"
#include <errno.h>

void mbox_init(struct mailbox *mbox, size_t size)
{
    sem_init(&mbox->prod_sem, size); /* All slots empty */
    sem_init(&mbox->cons_sem, 0);    /* No slots full */
    sem_init(&mbox->head_sem, 1);    /* Head mutex */
    sem_init(&mbox->tail_sem, 1);    /* Tail mutex */

    mbox->wrap = &mbox->data[size];
    mbox->head = &mbox->data[0];
    mbox->tail = &mbox->data[0];
};

int mbox_post(struct mailbox *mbox, void *msg, jiffies_t timeout)
{
    if (sem_down(&mbox->prod_sem, timeout) == (jiffies_t)-1)
	return ENOMEM;
    sem_down(&mbox->head_sem, 0);

    *mbox->head = msg;
    mbox->head++;
    if (mbox->head == mbox->wrap)
	mbox->head = &mbox->data[0];

    sem_up(&mbox->head_sem);
    sem_up(&mbox->cons_sem);
    return 0;
}

jiffies_t mbox_fetch(struct mailbox *mbox, void **msg, jiffies_t timeout)
{
    jiffies_t t;

    t = sem_down(&mbox->cons_sem, timeout);
    if (t == (jiffies_t)-1)
	return -1;
    t += sem_down(&mbox->tail_sem, 0);

    if (msg)
	*msg = *mbox->tail;
    mbox->tail++;
    if (mbox->tail == mbox->wrap)
	mbox->tail = &mbox->data[0];

    sem_up(&mbox->tail_sem);
    sem_up(&mbox->prod_sem);
    return t;
}
