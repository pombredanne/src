/*	$OpenBSD: uvm_pager.c,v 1.28.2.3 2002/06/11 03:33:04 art Exp $	*/
/*	$NetBSD: uvm_pager.c,v 1.54 2001/11/10 07:37:00 lukem Exp $	*/

/*
 *
 * Copyright (c) 1997 Charles D. Cranor and Washington University.
 * All rights reserved.
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions
 * are met:
 * 1. Redistributions of source code must retain the above copyright
 *    notice, this list of conditions and the following disclaimer.
 * 2. Redistributions in binary form must reproduce the above copyright
 *    notice, this list of conditions and the following disclaimer in the
 *    documentation and/or other materials provided with the distribution.
 * 3. All advertising materials mentioning features or use of this software
 *    must display the following acknowledgement:
 *      This product includes software developed by Charles D. Cranor and
 *      Washington University.
 * 4. The name of the author may not be used to endorse or promote products
 *    derived from this software without specific prior written permission.
 *
 * THIS SOFTWARE IS PROVIDED BY THE AUTHOR ``AS IS'' AND ANY EXPRESS OR
 * IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED WARRANTIES
 * OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE DISCLAIMED.
 * IN NO EVENT SHALL THE AUTHOR BE LIABLE FOR ANY DIRECT, INDIRECT,
 * INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT
 * NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE,
 * DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY
 * THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
 * (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF
 * THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 *
 * from: Id: uvm_pager.c,v 1.1.2.23 1998/02/02 20:38:06 chuck Exp
 */

/*
 * uvm_pager.c: generic functions used to assist the pagers.
 */

#define UVM_PAGER
#include <sys/param.h>
#include <sys/systm.h>
#include <sys/proc.h>
#include <sys/malloc.h>
#include <sys/pool.h>
#include <sys/vnode.h>
#include <sys/buf.h>

#include <uvm/uvm.h>

struct pool *uvm_aiobuf_pool;

/*
 * list of uvm pagers in the system
 */

extern struct uvm_pagerops uvm_deviceops;
extern struct uvm_pagerops uvm_vnodeops;
extern struct uvm_pagerops ubc_pager;

struct uvm_pagerops *uvmpagerops[] = {
	&aobj_pager,
	&uvm_deviceops,
	&uvm_vnodeops,
	&ubc_pager,
};

/*
 * the pager map: provides KVA for I/O
 */

struct vm_map *pager_map;		/* XXX */
struct simplelock pager_map_wanted_lock;
boolean_t pager_map_wanted;	/* locked by pager map */
static vaddr_t emergva;
static boolean_t emerginuse;

/*
 * uvm_pager_init: init pagers (at boot time)
 */

void
uvm_pager_init()
{
	int lcv;
	vaddr_t sva, eva;

	/*
	 * init pager map
	 */

	sva = 0;
	pager_map = uvm_km_suballoc(kernel_map, &sva, &eva, PAGER_MAP_SIZE, 0,
	    FALSE, NULL);
	simple_lock_init(&pager_map_wanted_lock);
	pager_map_wanted = FALSE;
	emergva = uvm_km_valloc(kernel_map, MAXBSIZE);
	emerginuse = FALSE;

	/*
	 * init ASYNC I/O queue
	 */

	TAILQ_INIT(&uvm.aio_done);

	/*
	 * call pager init functions
	 */
	for (lcv = 0 ; lcv < sizeof(uvmpagerops)/sizeof(struct uvm_pagerops *);
	    lcv++) {
		if (uvmpagerops[lcv]->pgo_init)
			uvmpagerops[lcv]->pgo_init();
	}
}

/*
 * uvm_pagermapin: map pages into KVA (pager_map) for I/O that needs mappings
 *
 * we basically just map in a blank map entry to reserve the space in the
 * map and then use pmap_enter() to put the mappings in by hand.
 */

vaddr_t
uvm_pagermapin(pps, npages, flags)
	struct vm_page **pps;
	int npages;
	int flags;
{
	vsize_t size;
	vaddr_t kva;
	vaddr_t cva;
	struct vm_page *pp;
	vm_prot_t prot;
	UVMHIST_FUNC("uvm_pagermapin"); UVMHIST_CALLED(maphist);

	UVMHIST_LOG(maphist,"(pps=0x%x, npages=%d)", pps, npages,0,0);

	/*
	 * compute protection.  outgoing I/O only needs read
	 * access to the page, whereas incoming needs read/write.
	 */

	prot = VM_PROT_READ;
	if (flags & UVMPAGER_MAPIN_READ)
		prot |= VM_PROT_WRITE;

ReStart:
	size = npages << PAGE_SHIFT;
	kva = 0;			/* let system choose VA */

	if (uvm_map(pager_map, &kva, size, NULL,
	      UVM_UNKNOWN_OFFSET, 0, UVM_FLAG_NOMERGE) != 0) {
		if (curproc == uvm.pagedaemon_proc) {
			simple_lock(&pager_map_wanted_lock);
			if (emerginuse) {
				UVM_UNLOCK_AND_WAIT(&emergva,
				    &pager_map_wanted_lock, FALSE,
				    "emergva", 0);
				goto ReStart;
			}
			emerginuse = TRUE;
			simple_unlock(&pager_map_wanted_lock);
			kva = emergva;
			KASSERT(npages <= MAXBSIZE >> PAGE_SHIFT);
			goto enter;
		}
		if ((flags & UVMPAGER_MAPIN_WAITOK) == 0) {
			UVMHIST_LOG(maphist,"<- NOWAIT failed", 0,0,0,0);
			return(0);
		}
		simple_lock(&pager_map_wanted_lock);
		pager_map_wanted = TRUE;
		UVMHIST_LOG(maphist, "  SLEEPING on pager_map",0,0,0,0);
		UVM_UNLOCK_AND_WAIT(pager_map, &pager_map_wanted_lock, FALSE,
		    "pager_map", 0);
		goto ReStart;
	}

enter:
	/* got it */
	for (cva = kva ; size != 0 ; size -= PAGE_SIZE, cva += PAGE_SIZE) {
		pp = *pps++;
		KASSERT(pp);
		KASSERT(pp->flags & PG_BUSY);
		pmap_kenter_pa(cva, VM_PAGE_TO_PHYS(pp), prot);
	}
	pmap_update(vm_map_pmap(pager_map));

	UVMHIST_LOG(maphist, "<- done (KVA=0x%x)", kva,0,0,0);
	return(kva);
}

/*
 * uvm_pagermapout: remove pager_map mapping
 *
 * we remove our mappings by hand and then remove the mapping (waking
 * up anyone wanting space).
 */

void
uvm_pagermapout(kva, npages)
	vaddr_t kva;
	int npages;
{
	vsize_t size = npages << PAGE_SHIFT;
	struct vm_map_entry *entries;
	UVMHIST_FUNC("uvm_pagermapout"); UVMHIST_CALLED(maphist);

	UVMHIST_LOG(maphist, " (kva=0x%x, npages=%d)", kva, npages,0,0);

	/*
	 * duplicate uvm_unmap, but add in pager_map_wanted handling.
	 */

	pmap_kremove(kva, npages << PAGE_SHIFT);
	if (kva == emergva) {
		simple_lock(&pager_map_wanted_lock);
		emerginuse = FALSE;
		wakeup(&emergva);
		simple_unlock(&pager_map_wanted_lock);
		return;
	}

	vm_map_lock(pager_map);
	uvm_unmap_remove(pager_map, kva, kva + size, &entries);
	simple_lock(&pager_map_wanted_lock);
	if (pager_map_wanted) {
		pager_map_wanted = FALSE;
		wakeup(pager_map);
	}
	simple_unlock(&pager_map_wanted_lock);
	vm_map_unlock(pager_map);
	if (entries)
		uvm_unmap_detach(entries, 0);
	pmap_update(pmap_kernel());
	UVMHIST_LOG(maphist,"<- done",0,0,0,0);
}

/*
 * interrupt-context iodone handler for nested i/o bufs.
 *
 * => must be at splbio().
 */

void
uvm_aio_biodone1(bp)
	struct buf *bp;
{
	struct buf *mbp = bp->b_private;

	splassert(IPL_BIO);

	KASSERT(mbp != bp);
	if (bp->b_flags & B_ERROR) {
		mbp->b_flags |= B_ERROR;
		mbp->b_error = bp->b_error;
	}
	mbp->b_resid -= bp->b_bcount;
	pool_put(&bufpool, bp);
	if (mbp->b_resid == 0) {
		biodone(mbp);
	}
}

/*
 * interrupt-context iodone handler for single-buf i/os
 * or the top-level buf of a nested-buf i/o.
 *
 * => must be at splbio().
 */

void
uvm_aio_biodone(bp)
	struct buf *bp;
{
	splassert(IPL_BIO);

	/* reset b_iodone for when this is a single-buf i/o. */
	bp->b_iodone = uvm_aio_aiodone;

	simple_lock(&uvm.aiodoned_lock);	/* locks uvm.aio_done */
	TAILQ_INSERT_TAIL(&uvm.aio_done, bp, b_freelist);
	wakeup(&uvm.aiodoned);
	simple_unlock(&uvm.aiodoned_lock);
}

/*
 * uvm_aio_aiodone: do iodone processing for async i/os.
 * this should be called in thread context, not interrupt context.
 */

void
uvm_aio_aiodone(bp)
	struct buf *bp;
{
	int npages = bp->b_bufsize >> PAGE_SHIFT;
	struct vm_page *pg, *pgs[npages];
	struct uvm_object *uobj;
	struct simplelock *slock;
	int i, error, swslot;
	boolean_t write, swap, pageout;
	UVMHIST_FUNC("uvm_aio_aiodone"); UVMHIST_CALLED(ubchist);
	UVMHIST_LOG(ubchist, "bp %p", bp, 0,0,0);

	splassert(IPL_BIO);

	error = (bp->b_flags & B_ERROR) ? (bp->b_error ? bp->b_error : EIO) : 0;
	write = (bp->b_flags & B_READ) == 0;
	/* XXXUBC B_NOCACHE is for swap pager, should be done differently */
	if (write && !(bp->b_flags & B_NOCACHE) && bioops.io_pageiodone) {
		(*bioops.io_pageiodone)(bp);
	}

	uobj = NULL;
	for (i = 0; i < npages; i++) {
		pgs[i] = uvm_pageratop((vaddr_t)bp->b_data + (i << PAGE_SHIFT));
		UVMHIST_LOG(ubchist, "pgs[%d] = %p", i, pgs[i],0,0);
	}
	uvm_pagermapout((vaddr_t)bp->b_data, npages);
#ifdef UVM_SWAP_ENCRYPT
	/*
	 * XXX - assumes that we only get ASYNC writes. used to be above.
	 */
	if (pgs[0]->pqflags & PQ_ENCRYPT) {
		uvm_swap_freepages(pgs, npages);
		goto freed;
	}
#endif /* UVM_SWAP_ENCRYPT */

	swslot = 0;
	slock = NULL;
	swap = (pgs[0]->pqflags & PQ_SWAPBACKED) != 0;
	pageout = (pgs[0]->flags & PG_PAGEOUT) != 0;
	if (!swap) {
		uobj = pgs[0]->uobject;
		slock = &uobj->vmobjlock;
		simple_lock(slock);
		uvm_lock_pageq();
	} else if (error) {
		pg = pgs[0];
		if (pg->pqflags & PQ_ANON) {
			swslot = pg->uanon->an_swslot;
		} else {
			swslot = uao_find_swslot(pg->uobject, pg->offset);
		}
		KASSERT(swslot);
	}
	for (i = 0; i < npages; i++) {
		pg = pgs[i];
		KASSERT(swap || pg->uobject == uobj);
		KASSERT(pageout ^ ((pg->flags & PG_PAGEOUT) == 0));
		UVMHIST_LOG(ubchist, "pg %p", pg, 0,0,0);

		/*
		 * for swap i/os, lock each page's object (or anon)
		 * individually since each page may need a different lock.
		 */

		if (swap) {
			if (pg->pqflags & PQ_ANON) {
				slock = &pg->uanon->an_lock;
			} else {
				slock = &pg->uobject->vmobjlock;
			}
			simple_lock(slock);
			uvm_lock_pageq();
		}

		/*
		 * process errors.  for reads, just mark the page to be freed.
		 * for writes, if the error was ENOMEM, we assume this was
		 * a transient failure so we mark the page dirty so that
		 * we'll try to write it again later.  for all other write
		 * errors, we assume the error is permanent, thus the data
		 * in the page is lost.  bummer.
		 */

		if (error) {
			if (!write) {
				pg->flags |= PG_RELEASED;
				continue;
			} else if (error == ENOMEM) {
				if (pg->flags & PG_PAGEOUT) {
					pg->flags &= ~PG_PAGEOUT;
					uvmexp.paging--;
				}
				pg->flags &= ~PG_CLEAN;
				uvm_pageactivate(pg);
			}
		}

		/*
		 * if the page is PG_FAKE, this must have been a read to
		 * initialize the page.  clear PG_FAKE and activate the page.
		 * we must also clear the pmap "modified" flag since it may
		 * still be set from the page's previous identity.
		 */

		if (pg->flags & PG_FAKE) {
			KASSERT(!write);
			pg->flags &= ~PG_FAKE;
			uvm_pageactivate(pg);
			pmap_clear_modify(pg);
		}

		/*
		 * do accounting for pagedaemon i/o and arrange to free
		 * the pages instead of just unbusying them.
		 */

		if (pg->flags & PG_PAGEOUT) {
			pg->flags &= ~PG_PAGEOUT;
			uvmexp.paging--;
			pg->flags |= PG_RELEASED;
		}

		/*
		 * for swap pages, unlock everything for this page now.
		 */

		if (swap) {
			uvm_page_unbusy(&pg, 1);
			uvm_unlock_pageq();
			simple_unlock(slock);
		}
	}
	if (!swap) {
		uvm_page_unbusy(pgs, npages);
		uvm_unlock_pageq();
		simple_unlock(slock);
	} else {
		KASSERT(write);
		KASSERT(pageout);

		/* these pages are now only in swap. */
		simple_lock(&uvm.swap_data_lock);
		KASSERT(uvmexp.swpgonly + npages <= uvmexp.swpginuse);
		uvmexp.swpgonly += npages;
		simple_unlock(&uvm.swap_data_lock);
		if (error) {
			uvm_swap_markbad(swslot, npages);
		}
	}

#ifdef UVM_SWAP_ENCRYPT
freed:
#endif
	if (bp->b_vp != NULL) {
		if (write && (bp->b_flags & B_AGE) != 0) {
			vwakeup(bp->b_vp);
		}
	}
	(void) buf_cleanout(bp);
	pool_put(&bufpool, bp);
}
