/*	$OpenBSD: rtld_machine.c,v 1.44 2010/05/02 04:57:01 guenther Exp $ */

/*
 * Copyright (c) 1999 Dale Rahn
 * Copyright (c) 2001 Niklas Hallqvist
 * Copyright (c) 2001 Artur Grabowski
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions
 * are met:
 * 1. Redistributions of source code must retain the above copyright
 *    notice, this list of conditions and the following disclaimer.
 * 2. Redistributions in binary form must reproduce the above copyright
 *    notice, this list of conditions and the following disclaimer in the
 *    documentation and/or other materials provided with the distribution.
 *
 * THIS SOFTWARE IS PROVIDED BY THE AUTHOR ``AS IS'' AND ANY EXPRESS
 * OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED
 * WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE
 * ARE DISCLAIMED.  IN NO EVENT SHALL THE AUTHOR BE LIABLE FOR ANY
 * DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL
 * DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS
 * OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION)
 * HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT
 * LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY
 * OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF
 * SUCH DAMAGE.
 *
 */

#define _DYN_LOADER

#include <sys/types.h>
#include <sys/cdefs.h>
#include <sys/mman.h>
#include <sys/exec.h>

#include <nlist.h>
#include <link.h>
#include <signal.h>

#include "syscall.h"
#include "archdep.h"
#include "resolve.h"

int
_dl_md_reloc(elf_object_t *object, int rel, int relasz)
{
	long	i;
	long	numrela;
	int	fails = 0;
	Elf64_Addr loff;
	Elf64_Rela  *relas;
	struct load_list *llist;

	loff = object->obj_base;
	numrela = object->Dyn.info[relasz] / sizeof(Elf64_Rela);
	relas = (Elf64_Rela *)(object->Dyn.info[rel]);

	if (relas == NULL)
		return(0);

	/*
	 * unprotect some segments if we need it.
	 * XXX - we unprotect way to much. only the text can have cow
	 * relocations.
	 */
	if ((object->dyn.textrel == 1) && (rel == DT_REL || rel == DT_RELA)) {
		for (llist = object->load_list; llist != NULL; llist = llist->next) {
			if (!(llist->prot & PROT_WRITE)) {
				_dl_mprotect(llist->start, llist->size,
				    llist->prot|PROT_WRITE);
			}
		}
	}

	for (i = 0; i < numrela; i++, relas++) {
		Elf64_Addr *r_addr;
		Elf64_Addr ooff;
		const Elf64_Sym *sym, *this;
		const char *symn;

		r_addr = (Elf64_Addr *)(relas->r_offset + loff);

		if (ELF64_R_SYM(relas->r_info) == 0xffffffff)
			continue;


		sym = object->dyn.symtab;
		sym += ELF64_R_SYM(relas->r_info);
		symn = object->dyn.strtab + sym->st_name;

		this = NULL;
		switch (ELF64_R_TYPE(relas->r_info)) {
		case R_TYPE(REFQUAD):
			ooff =  _dl_find_symbol_bysym(object,
			    ELF64_R_SYM(relas->r_info), &this,
			    SYM_SEARCH_ALL|SYM_WARNNOTFOUND|SYM_NOTPLT,
			    sym, NULL);
			if (this == NULL)
				goto resolve_failed;
			*r_addr += ooff + this->st_value + relas->r_addend;
			break;
		case R_TYPE(RELATIVE):
			/*
			 * There is a lot of unaligned RELATIVE
			 * relocs generated by gcc in the exception handlers.
			 */
			if ((((Elf_Addr) r_addr) & 0x7) != 0) {
				Elf_Addr tmp;
#if 0
_dl_printf("unaligned RELATIVE: %p type: %d %s 0x%lx -> 0x%lx\n", r_addr,
    ELF_R_TYPE(relas->r_info), object->load_name, *r_addr, *r_addr+loff);
#endif
				_dl_bcopy(r_addr, &tmp, sizeof(Elf_Addr));
				tmp += loff;
				_dl_bcopy(&tmp, r_addr, sizeof(Elf_Addr));
			} else
				*r_addr += loff;
			break;
		case R_TYPE(JMP_SLOT):
			ooff = _dl_find_symbol(symn, &this,
			    SYM_SEARCH_ALL|SYM_WARNNOTFOUND|SYM_PLT,
			    sym, object, NULL);
			if (this == NULL)
				goto resolve_failed;
			*r_addr = ooff + this->st_value + relas->r_addend;
			break;
		case R_TYPE(GLOB_DAT):
			ooff =  _dl_find_symbol_bysym(object,
			    ELF64_R_SYM(relas->r_info), &this,
			    SYM_SEARCH_ALL|SYM_WARNNOTFOUND|SYM_NOTPLT,
			    sym, NULL);
			if (this == NULL)
				goto resolve_failed;
			*r_addr = ooff + this->st_value + relas->r_addend;
			break;
		case R_TYPE(NONE):
			break;
		default:
			_dl_printf("%s:"
			    " %s: unsupported relocation '%s' %d at %lx\n",
			    _dl_progname, object->load_name, symn,
			    ELF64_R_TYPE(relas->r_info), r_addr );
			_dl_exit(1);
		}
		continue;
resolve_failed:
		if (ELF_ST_BIND(sym->st_info) != STB_WEAK)
			fails++;
	}
	__asm __volatile("imb" : : : "memory");

	/* reprotect the unprotected segments */
	if ((object->dyn.textrel == 1) && (rel == DT_REL || rel == DT_RELA)) {
		for (llist = object->load_list; llist != NULL; llist = llist->next) {
			if (!(llist->prot & PROT_WRITE))
				_dl_mprotect(llist->start, llist->size,
				    llist->prot);
		}
	}
	return (fails);
}

/*
 * Resolve a symbol at run-time.
 */
Elf_Addr
_dl_bind(elf_object_t *object, int reloff)
{
	Elf_RelA *rela;
	Elf_Addr *addr, ooff;
	const Elf_Sym *sym, *this;
	const char *symn;
	sigset_t savedmask;

	rela = (Elf_RelA *)(object->Dyn.info[DT_JMPREL] + reloff);

	addr = (Elf_Addr *)(object->obj_base + rela->r_offset);
	if (object->plt_size != 0 && !(*addr >=  object->plt_start &&
	    *addr < (object->plt_start + object->plt_size ))) {
		/* something is broken, relocation has already occurred */
#if 0
		DL_DEB(("*addr doesn't point into plt %p obj %s\n", 
		    *addr, object->load_name));
#endif
		return *addr;
	}

	sym = object->dyn.symtab;
	sym += ELF64_R_SYM(rela->r_info);
	symn = object->dyn.strtab + sym->st_name;

	this = NULL;
	ooff = _dl_find_symbol(symn, &this,
	    SYM_SEARCH_ALL|SYM_WARNNOTFOUND|SYM_PLT, sym,
	    object, NULL);
	if (this == NULL) {
		_dl_printf("lazy binding failed!\n");
		*((int *)0) = 0;	/* XXX */
	}

	/* if PLT is protected, allow the write */
	if (object->plt_size != 0) {
		_dl_thread_bind_lock(0, &savedmask);
		_dl_mprotect(addr, sizeof(Elf_Addr),
		    PROT_READ|PROT_WRITE);
	}

	*addr = ooff + this->st_value + rela->r_addend;

	/* if PLT is (to be protected, change back to RO/X  */
	if (object->plt_size != 0) {
		_dl_mprotect(addr, sizeof(Elf_Addr),
		    PROT_READ);
		_dl_thread_bind_lock(1, &savedmask);
	}

	return *addr;
}

/*
 *	Relocate the Global Offset Table (GOT).
 */
int
_dl_md_reloc_got(elf_object_t *object, int lazy)
{
	int	fails = 0;
	Elf_Addr *pltgot;
	extern void _dl_bind_start(void);	/* XXX */
	Elf_Addr ooff;
	Elf_Addr plt_addr;
	const Elf_Sym *this;


	pltgot = (Elf_Addr *)object->Dyn.info[DT_PLTGOT];

	object->got_addr = NULL;
	object->got_size = 0;
	this = NULL;
	ooff = _dl_find_symbol("__got_start", &this,
	    SYM_SEARCH_OBJ|SYM_NOWARNNOTFOUND|SYM_PLT, NULL, object, NULL);
	if (this != NULL)
		object->got_addr = ooff + this->st_value;

	this = NULL;
	ooff = _dl_find_symbol("__got_end", &this,
	    SYM_SEARCH_OBJ|SYM_NOWARNNOTFOUND|SYM_PLT, NULL, object, NULL);
	if (this != NULL)
		object->got_size = ooff + this->st_value  - object->got_addr;

	plt_addr = NULL;
	object->plt_size = 0;
	this = NULL;
	ooff = _dl_find_symbol("__plt_start", &this,
	    SYM_SEARCH_OBJ|SYM_NOWARNNOTFOUND|SYM_PLT, NULL, object, NULL);
	if (this != NULL)
		plt_addr = ooff + this->st_value;

	this = NULL;
	ooff = _dl_find_symbol("__plt_end", &this,
	    SYM_SEARCH_OBJ|SYM_NOWARNNOTFOUND|SYM_PLT, NULL, object, NULL);
	if (this != NULL)
		object->plt_size = ooff + this->st_value  - plt_addr;

	if (object->got_addr == NULL)
		object->got_start = NULL;
	else {
		object->got_start = ELF_TRUNC(object->got_addr, _dl_pagesz);
		object->got_size += object->got_addr - object->got_start;
		object->got_size = ELF_ROUND(object->got_size, _dl_pagesz);
	}
	if (plt_addr == NULL)
		object->plt_start = NULL;
	else {
		object->plt_start = ELF_TRUNC(plt_addr, _dl_pagesz);
		object->plt_size += plt_addr - object->plt_start;
		object->plt_size = ELF_ROUND(object->plt_size, _dl_pagesz);
	}

	if (object->obj_type == OBJTYPE_LDR || !lazy || pltgot == NULL) {
		fails = _dl_md_reloc(object, DT_JMPREL, DT_PLTRELSZ);
	} else {
		if (object->obj_base != 0) {
			int i, size;
			Elf_Addr *addr;
			Elf_RelA *rela;

			size = object->Dyn.info[DT_PLTRELSZ] /
			    sizeof(Elf_RelA);
			rela = (Elf_RelA *)(object->Dyn.info[DT_JMPREL]);

			for (i = 0; i < size; i++) {
				addr = (Elf_Addr *)(object->obj_base +
				    rela[i].r_offset);
				*addr += object->obj_base;
			}
		}
	}
	if (pltgot != NULL) {
		pltgot[2] = (Elf_Addr)_dl_bind_start;
		pltgot[3] = (Elf_Addr)object;
	}
	if (object->got_size != 0)
		_dl_mprotect((void*)object->got_start, object->got_size,
		    PROT_READ);
	if (object->plt_size != 0)
		_dl_mprotect((void*)object->plt_start, object->plt_size,
		    PROT_READ|PROT_EXEC);

	return (fails);
}

/* relocate the GOT early */

void	_reloc_alpha_got(Elf_Dyn *dynp, Elf_Addr relocbase);

void
_reloc_alpha_got(Elf_Dyn *dynp, Elf_Addr relocbase)
{
	const Elf_RelA *rela = 0, *relalim;
	Elf_Addr relasz = 0;
	Elf_Addr *where;

	for (; dynp->d_tag != DT_NULL; dynp++) {
		switch (dynp->d_tag) {
		case DT_RELA:
			rela = (const Elf_RelA *)(relocbase + dynp->d_un.d_ptr);
			break;
		case DT_RELASZ:
			relasz = dynp->d_un.d_val;
			break;
		}
	}
	relalim = (const Elf_RelA *)((caddr_t)rela + relasz);
	for (; rela < relalim; rela++) {
		where = (Elf_Addr *)(relocbase + rela->r_offset);
		/* XXX For some reason I see a few GLOB_DAT relocs here. */
		*where += (Elf_Addr)relocbase;
	}
}