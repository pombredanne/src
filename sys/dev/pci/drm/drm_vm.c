/*-
 * Copyright 2003 Eric Anholt
 * All Rights Reserved.
 *
 * Permission is hereby granted, free of charge, to any person obtaining a
 * copy of this software and associated documentation files (the "Software"),
 * to deal in the Software without restriction, including without limitation
 * the rights to use, copy, modify, merge, publish, distribute, sublicense,
 * and/or sell copies of the Software, and to permit persons to whom the
 * Software is furnished to do so, subject to the following conditions:
 * 
 * The above copyright notice and this permission notice (including the next
 * paragraph) shall be included in all copies or substantial portions of the
 * Software.
 * 
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 * FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT.  IN NO EVENT SHALL
 * ERIC ANHOLT BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER
 * IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN
 * CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.
 */

/** @file drm_vm.c
 * Support code for mmaping of DRM maps.
 */

#include "drmP.h"
#include "drm.h"

#if defined(__FreeBSD__) && __FreeBSD_version >= 500102
int
drm_mmap(struct cdev *kdev, vm_offset_t offset, vm_paddr_t *paddr,
    int prot)
#elif defined(__FreeBSD__)
int
drm_mmap(dev_t kdev, vm_offset_t offset, int prot)
#elif defined(__NetBSD__) || defined(__OpenBSD__)
paddr_t
drm_mmap(dev_t kdev, off_t offset, int prot)
#endif
{
	drm_device_t *dev = drm_get_device_from_kdev(kdev);
	drm_local_map_t *map;
	drm_file_t *priv;
	drm_map_type_t type;
#ifdef __FreeBSD__
	vm_paddr_t phys;
#else
	paddr_t phys;
#endif

	DRM_LOCK();
#ifdef __OpenBSD__
	priv = drm_find_file_by_minor(dev, minor(kdev));
#else
	priv = drm_find_file_by_proc(dev, DRM_CURPROC);
#endif
	DRM_UNLOCK();
	if (priv == NULL) {
		DRM_ERROR("can't find authenticator\n");
		return EINVAL;
	}

	if (!priv->authenticated)
		return EACCES;

	if (dev->dma && offset >= 0 && offset < ptoa(dev->dma->page_count)) {
		drm_device_dma_t *dma = dev->dma;

		DRM_SPINLOCK(&dev->dma_lock);

		if (dma->pagelist != NULL) {
			unsigned long page = offset >> PAGE_SHIFT;
			unsigned long phys = dma->pagelist[page];

#if defined(__FreeBSD__) && __FreeBSD_version >= 500102
			*paddr = phys;
			DRM_SPINUNLOCK(&dev->dma_lock);
			return 0;
#else
			DRM_SPINUNLOCK(&dev->dma_lock);
			return atop(phys);
#endif
		} else {
			DRM_SPINUNLOCK(&dev->dma_lock);
			return -1;
		}
	}

	/*
	 * A sequential search of a linked list is
 	 * fine here because: 1) there will only be
	 * about 5-10 entries in the list and, 2) a
	 * DRI client only has to do this mapping
	 * once, so it doesn't have to be optimized
	 * for performance, even if the list was a
	 * bit longer.
	 */
	DRM_LOCK();
	TAILQ_FOREACH(map, &dev->maplist, link) {
		if (offset >= map->offset && offset < map->offset + map->size)
			break;
	}

	if (map == NULL) {
		DRM_UNLOCK();
		DRM_DEBUG("can't find map\n");
		return -1;
	}
#ifdef __OpenBSD__
	if (((map->flags&_DRM_RESTRICTED) && priv->master == 0)) {
#else
	if (((map->flags&_DRM_RESTRICTED) && !DRM_SUSER(DRM_CURPROC))) {
#endif
		DRM_UNLOCK();
		DRM_DEBUG("restricted map\n");
		return -1;
	}
	type = map->type;
	DRM_UNLOCK();

	switch (type) {
	case _DRM_FRAME_BUFFER:
	case _DRM_REGISTERS:
	case _DRM_AGP:
		phys = offset;
		break;
#ifdef __FreeBSD__
	case _DRM_CONSISTENT:
		phys = vtophys((char *)map->handle + (offset - map->offset));
		break;
	case _DRM_SCATTER_GATHER:
	case _DRM_SHM:
		phys = vtophys(offset);
		break;
#else
	/* XXX unify all the bus_dmamem_mmap bits */
	case _DRM_SCATTER_GATHER:
		return bus_dmamem_mmap(dev->pa.pa_dmat, dev->sg->mem->sg_segs,
		    dev->sg->mem->sg_nsegs, offset - dev->sg->handle, prot,
		    BUS_DMA_NOWAIT);
	case _DRM_SHM:
	case _DRM_CONSISTENT:
		return bus_dmamem_mmap(dev->pa.pa_dmat, &map->dmah->seg, 1,
		    offset - map->offset, prot, BUS_DMA_NOWAIT);
#endif
	default:
		DRM_ERROR("bad map type %d\n", type);
		return -1;	/* This should never happen. */
	}

#if defined(__FreeBSD__) && __FreeBSD_version >= 500102
	*paddr = phys;
	return 0;
#else
	return atop(phys);
#endif
}

