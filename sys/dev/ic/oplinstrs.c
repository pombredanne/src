/*	$OpenBSD: oplinstrs.c,v 1.1.6.1 2001/07/04 10:41:02 niklas Exp $	*/

#include <sys/param.h>
#include <sys/systm.h>
#include <sys/errno.h>
#include <sys/ioctl.h>
#include <sys/syslog.h>
#include <sys/device.h>
#include <sys/select.h>

#include <machine/bus.h>

#include <dev/midivar.h>
#include <dev/midisynvar.h>

#include <dev/ic/oplreg.h>
#include <dev/ic/oplvar.h>

/*
 * Operator settings for the OPL2 and OPL3 FM synths.
 * These tables are indexed by the General MIDI instrument number.
 */
const struct opl_operators opl3_instrs[OPL_NINSTR] = {
{ 1, { 0x23, 0x01, 0x15, 0x00, 0xfd, 0x84, 0x7c, 0xf5, 0x03, 0x00, 0x10, 
       0x03, 0x01, 0x5d, 0x00, 0xf2, 0xf4, 0x35, 0xf5, 0x00, 0x04, 0x11, } }, 
{ 1, { 0x03, 0x01, 0x7d, 0x00, 0xf3, 0xf3, 0xf5, 0xf5, 0x00, 0x04, 0x10,
       0x03, 0x01, 0x55, 0x00, 0xf2, 0xf2, 0xf5, 0xf5, 0x00, 0x04, 0x11, } }, 
{ 1, { 0x03, 0x01, 0x4d, 0x00, 0xfb, 0xf3, 0x55, 0xc5, 0x04, 0x04, 0x20,
       0x03, 0x01, 0xce, 0x00, 0xf3, 0xf3, 0x55, 0xd5, 0x00, 0x04, 0x21, } }, 
{ 1, { 0x03, 0x01, 0xd7, 0x00, 0xf3, 0xf2, 0xf5, 0xf5, 0x04, 0x04, 0x20,
       0x03, 0x01, 0xd5, 0x00, 0xf3, 0xf3, 0xf5, 0xf5, 0x00, 0x04, 0x21, } }, 
{ 1, { 0x16, 0x01, 0x57, 0x00, 0xff, 0xf3, 0x8a, 0x66, 0x00, 0x00, 0x2c,
       0x0b, 0x81, 0x2b, 0x00, 0xe5, 0xe2, 0xb5, 0x86, 0x00, 0x00, 0x21, } }, 
{ 1, { 0x0a, 0x01, 0x5f, 0x00, 0xf4, 0xf4, 0x87, 0x86, 0x00, 0x00, 0x10,
       0x03, 0x01, 0xcc, 0x00, 0xf2, 0xe2, 0x86, 0x86, 0x00, 0x00, 0x11, } }, 
{ 1, { 0x22, 0x01, 0xd1, 0x80, 0xe3, 0xf3, 0x79, 0xa7, 0x04, 0x04, 0x34,
       0x22, 0x01, 0x96, 0x00, 0xf2, 0xf3, 0x79, 0xa7, 0x04, 0x04, 0x31, } }, 
{ 1, { 0x22, 0x01, 0x9c, 0x00, 0xf4, 0xf3, 0x19, 0xa6, 0x05, 0x05, 0x10,
       0x22, 0x01, 0x97, 0x00, 0xf4, 0xf3, 0x19, 0xe6, 0x04, 0x04, 0x11, } }, 
{ 1, { 0x1b, 0x19, 0x5a, 0x00, 0xf4, 0xe3, 0xf6, 0xa2, 0x00, 0x00, 0x30,
       0x1a, 0x11, 0x57, 0x00, 0xf3, 0xf2, 0xb2, 0xa3, 0x00, 0x00, 0x31, } }, 
{ 1, { 0x1b, 0x17, 0x5a, 0x00, 0xd6, 0xf3, 0x53, 0x53, 0x00, 0x00, 0x20,
       0x1a, 0x11, 0x49, 0x00, 0xd1, 0xf3, 0x52, 0x52, 0x00, 0x00, 0x21, } }, 
{ 1, { 0x1b, 0x14, 0x5a, 0x00, 0xf6, 0x63, 0xf6, 0xa3, 0x00, 0x00, 0x30,
       0x1b, 0x11, 0x52, 0x00, 0xd3, 0xf2, 0xb2, 0xa3, 0x00, 0x00, 0x31, } }, 
{ 1, { 0x8b, 0x84, 0x5a, 0x00, 0xd7, 0xf1, 0x15, 0x95, 0x00, 0x00, 0x30,
       0x9b, 0x81, 0x49, 0x00, 0xdc, 0xd2, 0x25, 0xb5, 0x00, 0x00, 0x31, } }, 
{ 1, { 0x08, 0x01, 0xc0, 0x00, 0xfd, 0xf6, 0x56, 0x68, 0x00, 0x00, 0x20,
       0x95, 0x81, 0x40, 0x00, 0xfe, 0xf0, 0x27, 0x05, 0x00, 0x00, 0x21, } }, 
{ 1, { 0x08, 0x03, 0xc0, 0x00, 0xfa, 0xf8, 0x56, 0x66, 0x00, 0x00, 0x30,
       0x97, 0x81, 0x40, 0x00, 0xdf, 0xf8, 0x26, 0x06, 0x00, 0x00, 0x31, } }, 
{ 1, { 0x03, 0x00, 0x95, 0x80, 0xf3, 0xf3, 0x33, 0x23, 0x00, 0x00, 0x30,
       0x03, 0x00, 0x8c, 0x80, 0xf4, 0xf3, 0x23, 0x03, 0x00, 0x00, 0x31, } }, 
{ 1, { 0x01, 0x00, 0x8b, 0x00, 0xa7, 0x94, 0xf9, 0x36, 0x06, 0x05, 0x30,
       0x02, 0x81, 0x18, 0x80, 0xf6, 0xf4, 0x79, 0x55, 0x04, 0x04, 0x31, } }, 
{ 1, { 0x24, 0x24, 0x40, 0x00, 0xfd, 0xfb, 0xfd, 0x0e, 0x00, 0x00, 0x20,
       0x20, 0x20, 0x95, 0x00, 0xfb, 0xf6, 0x0e, 0x0e, 0x00, 0x00, 0x21, } }, 
{ 1, { 0x04, 0x24, 0x17, 0x00, 0xfe, 0xf7, 0xb7, 0x67, 0x00, 0x00, 0x20,
       0x20, 0xa0, 0x14, 0x00, 0xf9, 0xf6, 0x06, 0x07, 0x00, 0x00, 0x21, } }, 
{ 1, { 0xa2, 0x20, 0x1e, 0x00, 0xb9, 0xa9, 0x36, 0x0e, 0x00, 0x00, 0x30,
       0xb4, 0x20, 0x1a, 0x00, 0x79, 0x77, 0x51, 0x4c, 0x04, 0x00, 0x31, } }, 
{ 1, { 0x25, 0xa1, 0x23, 0x00, 0x74, 0x60, 0x11, 0x05, 0x00, 0x00, 0x12,
       0xb0, 0xb0, 0xd3, 0x00, 0x74, 0x60, 0x12, 0x05, 0x04, 0x01, 0x11, } }, 
{ 1, { 0x22, 0x21, 0x24, 0x40, 0x76, 0x76, 0x06, 0x06, 0x04, 0x04, 0x10,
       0x20, 0x21, 0x21, 0x00, 0x76, 0x66, 0x06, 0x06, 0x00, 0x02, 0x11, } }, 
{ 1, { 0x22, 0x21, 0x1b, 0x00, 0x76, 0x76, 0x07, 0x07, 0x00, 0x04, 0x30,
       0x21, 0x21, 0x19, 0x00, 0x76, 0x66, 0x07, 0x07, 0x00, 0x02, 0x31, } }, 
{ 1, { 0x21, 0x21, 0x20, 0x00, 0x93, 0x73, 0x09, 0x09, 0x04, 0x04, 0x30,
       0x21, 0x21, 0x27, 0x00, 0x76, 0x76, 0x09, 0x09, 0x04, 0x04, 0x31, } }, 
{ 1, { 0x21, 0x21, 0x1e, 0x00, 0x76, 0x66, 0x09, 0x19, 0x04, 0x05, 0x30,
       0x21, 0x21, 0x22, 0x00, 0x76, 0x66, 0x29, 0x19, 0x04, 0x04, 0x31, } }, 
{ 1, { 0x1b, 0x11, 0x4c, 0x00, 0xfe, 0xf2, 0xfe, 0xe3, 0x04, 0x00, 0x10,
       0x31, 0x11, 0x4c, 0x00, 0xf2, 0xf2, 0x45, 0xf4, 0x04, 0x00, 0x11, } }, 
{ 1, { 0x30, 0x05, 0x00, 0x0b, 0xf2, 0xfe, 0x49, 0x43, 0x02, 0x01, 0x21,
       0x01, 0x00, 0x00, 0x00, 0xf2, 0xf3, 0xe7, 0x9d, 0x01, 0x02, 0x21, } }, 
{ 1, { 0x34, 0x04, 0x00, 0x16, 0xf2, 0xfd, 0x49, 0x43, 0x00, 0x00, 0x31,
       0x01, 0x01, 0x00, 0x00, 0xf2, 0xf3, 0xe7, 0x9d, 0x00, 0x00, 0x31, } }, 
{ 1, { 0x03, 0x23, 0x00, 0x00, 0xfb, 0xfc, 0x49, 0x64, 0x06, 0x00, 0x19,
       0x01, 0x01, 0x00, 0x00, 0xf4, 0xf6, 0xc4, 0xc4, 0x02, 0x06, 0x11, } }, 
{ 1, { 0x01, 0x01, 0x06, 0x00, 0xf6, 0xf3, 0x27, 0xe7, 0x05, 0x01, 0x30,
       0x02, 0x01, 0x05, 0x00, 0xfa, 0x65, 0x96, 0x9b, 0x04, 0x04, 0x31, } }, 
{ 1, { 0x21, 0x21, 0xc1, 0x40, 0xf2, 0xf3, 0x13, 0x16, 0x00, 0x04, 0x30,
       0x21, 0x21, 0x03, 0x00, 0x97, 0x96, 0xae, 0x26, 0x03, 0x05, 0x31, } }, 
{ 1, { 0x21, 0x21, 0xc1, 0x00, 0xf2, 0xf2, 0x16, 0x16, 0x00, 0x04, 0x30,
       0x21, 0x20, 0x03, 0x00, 0xc7, 0x90, 0x46, 0x16, 0x03, 0x03, 0x31, } }, 
{ 1, { 0x0b, 0x01, 0x40, 0x40, 0x65, 0xf2, 0x1a, 0x3e, 0x00, 0x03, 0x2c,
       0x05, 0x0a, 0x00, 0x40, 0xe2, 0x54, 0x1a, 0x1a, 0x02, 0x01, 0x21, } }, 
{ 0, { 0x21, 0x21, 0x12, 0x00, 0xe4, 0xd3, 0x15, 0xa6, 0x00, 0x00, 0x30, } }, 
{ 1, { 0x21, 0x01, 0x00, 0x0b, 0xf2, 0xf2, 0x49, 0x97, 0x04, 0x05, 0x31,
       0x01, 0x01, 0x00, 0x00, 0xf6, 0xf3, 0xe7, 0x9d, 0x01, 0x04, 0x31, } }, 
{ 1, { 0x21, 0x01, 0x00, 0x08, 0xf2, 0xf2, 0x49, 0x97, 0x04, 0x05, 0x11,
       0x01, 0x01, 0x00, 0x00, 0xf6, 0xf3, 0xe7, 0x9d, 0x01, 0x04, 0x11, } }, 
{ 1, { 0x01, 0x01, 0xc0, 0x08, 0xf2, 0xc4, 0x6a, 0x97, 0x00, 0x00, 0x39,
       0x01, 0x01, 0x00, 0x00, 0x91, 0xf2, 0xe7, 0x6d, 0x00, 0x00, 0x31, } }, 
{ 1, { 0x01, 0x0b, 0x00, 0x08, 0xfd, 0xfa, 0x49, 0x87, 0x06, 0x04, 0x11,
       0x01, 0x01, 0x00, 0x00, 0xf3, 0xf3, 0xb7, 0x9d, 0x04, 0x04, 0x11, } }, 
{ 0, { 0x26, 0x21, 0x24, 0x00, 0xf6, 0xf2, 0x87, 0xf7, 0x04, 0x04, 0x30, } }, 
{ 1, { 0x21, 0x01, 0x08, 0x40, 0xf5, 0xf2, 0x47, 0xf7, 0x00, 0x00, 0x20,
       0x21, 0x01, 0x07, 0x40, 0xf5, 0xf2, 0x47, 0xf7, 0x00, 0x00, 0x29, } }, 
{ 1, { 0xc1, 0x01, 0x00, 0x21, 0xf2, 0xf2, 0x48, 0x97, 0x00, 0x00, 0x21,
       0x01, 0x01, 0x0a, 0x00, 0xf2, 0xf2, 0x97, 0x77, 0x00, 0x00, 0x20, } }, 
{ 1, { 0x22, 0x21, 0xca, 0x00, 0xb4, 0x44, 0x26, 0x06, 0x04, 0x04, 0x3c,
       0x22, 0x01, 0xcc, 0x00, 0x89, 0x5d, 0x16, 0x06, 0x04, 0x04, 0x31, } }, 
{ 1, { 0x24, 0x21, 0xcb, 0x00, 0x88, 0x66, 0x26, 0x16, 0x04, 0x04, 0x3c,
       0x34, 0x21, 0xc4, 0x00, 0x86, 0x66, 0x66, 0x06, 0x04, 0x04, 0x31, } }, 
{ 1, { 0x22, 0x01, 0xcb, 0x00, 0x72, 0x5f, 0x46, 0x86, 0x04, 0x04, 0x36,
       0x22, 0x21, 0xcd, 0x00, 0x76, 0x54, 0x36, 0x06, 0x04, 0x04, 0x31, } }, 
{ 1, { 0x22, 0x01, 0xff, 0x00, 0x86, 0x6f, 0x06, 0x08, 0x04, 0x04, 0x36,
       0x22, 0x21, 0xd2, 0x00, 0x74, 0x54, 0x16, 0x06, 0x04, 0x04, 0x31, } }, 
{ 1, { 0x60, 0x61, 0x00, 0x09, 0x68, 0x53, 0x04, 0x45, 0x02, 0x03, 0x11,
       0x61, 0x60, 0x00, 0x00, 0x54, 0x78, 0x54, 0x35, 0x01, 0x03, 0x11, } }, 
{ 1, { 0x00, 0x00, 0x07, 0x00, 0xf9, 0xf6, 0xf1, 0x34, 0x03, 0x02, 0x16,
       0x00, 0x00, 0x0c, 0x00, 0x89, 0xb5, 0xf1, 0x34, 0x03, 0x02, 0x11, } }, 
{ 1, { 0x12, 0x01, 0xd0, 0x00, 0xb5, 0xc3, 0x23, 0xe3, 0x04, 0x00, 0x30,
       0x12, 0x11, 0xd9, 0x00, 0xe5, 0xa2, 0x15, 0xf3, 0x04, 0x00, 0x31, } }, 
{ 1, { 0x11, 0x10, 0xd0, 0x00, 0xe4, 0xd3, 0xe4, 0xf4, 0x01, 0x00, 0x30,
       0x14, 0x10, 0x14, 0x00, 0xf6, 0xf3, 0xf4, 0xf4, 0x02, 0x02, 0x31, } }, 
{ 1, { 0x21, 0x21, 0x1b, 0xc0, 0xa9, 0x63, 0x23, 0x25, 0x00, 0x04, 0x3e,
       0x21, 0x21, 0x22, 0x40, 0xa9, 0x43, 0x23, 0x24, 0x00, 0x04, 0x31, } }, 
{ 1, { 0x21, 0x21, 0x1f, 0xc0, 0xa9, 0x33, 0x23, 0x15, 0x00, 0x04, 0x3e,
       0x21, 0x21, 0x23, 0x80, 0x60, 0x33, 0x23, 0x25, 0x00, 0x04, 0x31, } }, 
{ 1, { 0x20, 0x01, 0x00, 0x0c, 0x52, 0x50, 0xb6, 0x15, 0x02, 0x01, 0x31,
       0x21, 0x20, 0x00, 0x40, 0x44, 0x41, 0x04, 0x27, 0x01, 0x02, 0x31, } }, 
{ 1, { 0x22, 0x01, 0x40, 0xc2, 0x51, 0x31, 0xa4, 0xf4, 0x04, 0x01, 0x11,
       0x21, 0x21, 0x00, 0x40, 0x70, 0x34, 0xf4, 0x74, 0x01, 0x04, 0x11, } }, 
{ 1, { 0x21, 0x21, 0x10, 0x00, 0x61, 0x66, 0x04, 0x45, 0x00, 0x04, 0x3a,
       0x20, 0x21, 0x1c, 0x00, 0x63, 0x63, 0x05, 0x04, 0x00, 0x00, 0x31, } }, 
{ 1, { 0x21, 0x21, 0x16, 0x00, 0xa6, 0x77, 0x66, 0x06, 0x00, 0x00, 0x10,
       0x20, 0x21, 0xd8, 0x00, 0x66, 0x68, 0x36, 0x06, 0x00, 0x00, 0x11, } }, 
{ 1, { 0x21, 0x21, 0x12, 0x00, 0x75, 0x67, 0x66, 0x06, 0x00, 0x00, 0x10,
       0x21, 0x21, 0x2d, 0x00, 0x62, 0x68, 0x36, 0x06, 0x00, 0x00, 0x11, } }, 
{ 1, { 0x02, 0x01, 0x0e, 0x00, 0x76, 0x64, 0x34, 0x35, 0x00, 0x04, 0x10,
       0x01, 0x00, 0x2d, 0x00, 0x66, 0x63, 0x35, 0x04, 0x04, 0x00, 0x11, } }, 
{ 1, { 0x21, 0x21, 0x23, 0x00, 0x72, 0x72, 0x9b, 0x3b, 0x01, 0x04, 0x30,
       0x21, 0x21, 0x23, 0x00, 0x82, 0x92, 0x9b, 0x3b, 0x01, 0x04, 0x31, } }, 
{ 1, { 0x21, 0x21, 0x23, 0x00, 0x63, 0x62, 0x9b, 0x3b, 0x01, 0x04, 0x30,
       0x21, 0x21, 0x21, 0x00, 0x73, 0x82, 0x9b, 0x3b, 0x01, 0x04, 0x31, } }, 
{ 1, { 0x01, 0x01, 0x14, 0x00, 0x62, 0xe6, 0x0b, 0x2b, 0x05, 0x05, 0x26,
       0x21, 0x22, 0x13, 0x00, 0x55, 0x72, 0x1b, 0x2b, 0x04, 0x00, 0x21, } }, 
{ 1, { 0x01, 0x21, 0x11, 0x00, 0x60, 0x62, 0xfb, 0x7b, 0x05, 0x00, 0x10,
       0x01, 0x21, 0x10, 0x00, 0x60, 0x62, 0x3b, 0x7b, 0x05, 0x00, 0x11, } }, 
{ 0, { 0x21, 0x21, 0x23, 0x00, 0x41, 0x81, 0x94, 0xf6, 0x00, 0x00, 0x2e, } }, 
{ 1, { 0x22, 0x61, 0x4c, 0x40, 0x75, 0xf1, 0x37, 0xb7, 0x01, 0x05, 0x20,
       0x21, 0x20, 0x4d, 0x00, 0x56, 0x72, 0x07, 0x17, 0x05, 0x02, 0x21, } }, 
{ 1, { 0x21, 0x21, 0x04, 0x00, 0x82, 0x71, 0xcb, 0x2b, 0x01, 0x01, 0x26,
       0x21, 0x21, 0x11, 0x00, 0x75, 0x80, 0x8b, 0x3b, 0x00, 0x00, 0x21, } }, 
{ 1, { 0x21, 0x21, 0x83, 0x40, 0x84, 0xa2, 0x3b, 0x3b, 0x01, 0x00, 0x26,
       0x21, 0x21, 0x0e, 0x00, 0x74, 0x73, 0x7b, 0x3b, 0x00, 0x00, 0x21, } }, 
{ 1, { 0x01, 0x02, 0x15, 0x00, 0x70, 0x70, 0xe7, 0xe8, 0x05, 0x00, 0x10,
       0x01, 0x00, 0x42, 0x00, 0x70, 0x70, 0xf8, 0xe8, 0x07, 0x02, 0x11, } }, 
{ 1, { 0x01, 0x01, 0x13, 0x00, 0x70, 0x70, 0xe9, 0xe9, 0x05, 0x00, 0x10,
       0x01, 0x40, 0x47, 0x40, 0x70, 0x70, 0xe9, 0xe9, 0x07, 0x02, 0x11, } }, 
{ 1, { 0x01, 0x01, 0x15, 0x00, 0x70, 0x70, 0xeb, 0xea, 0x05, 0x00, 0x10,
       0x01, 0x40, 0x09, 0x00, 0x70, 0x70, 0xeb, 0xeb, 0x07, 0x02, 0x11, } }, 
{ 1, { 0x01, 0x04, 0x11, 0x00, 0xa4, 0x67, 0xbb, 0x6a, 0x00, 0x00, 0x20,
       0x01, 0x02, 0x0c, 0x00, 0xd0, 0x80, 0xeb, 0xeb, 0x01, 0x02, 0x21, } }, 
{ 1, { 0x11, 0x34, 0x17, 0x00, 0x70, 0x61, 0x29, 0x29, 0x00, 0x00, 0x30,
       0x31, 0x34, 0xff, 0x40, 0x60, 0x60, 0x09, 0x09, 0x00, 0x02, 0x31, } }, 
{ 1, { 0x01, 0x24, 0x11, 0x00, 0xf0, 0x80, 0x29, 0x09, 0x00, 0x00, 0x10,
       0x31, 0x24, 0xda, 0x00, 0x80, 0x80, 0x09, 0x09, 0x00, 0x00, 0x11, } }, 
{ 1, { 0x01, 0x24, 0x13, 0x00, 0x60, 0x80, 0x22, 0x09, 0x00, 0x00, 0x10,
       0x31, 0x24, 0x86, 0x00, 0x80, 0x80, 0x05, 0x09, 0x00, 0x00, 0x11, } }, 
{ 1, { 0x32, 0x31, 0x1d, 0x00, 0xa2, 0x51, 0x16, 0x26, 0x04, 0x00, 0x3a,
       0x32, 0x31, 0x45, 0x00, 0xfe, 0x55, 0x52, 0x15, 0x04, 0x00, 0x31, } }, 
{ 1, { 0x24, 0xa2, 0x20, 0x00, 0x67, 0x65, 0xf6, 0x06, 0x00, 0x00, 0x10,
       0x24, 0xa2, 0x1c, 0x00, 0x7f, 0x67, 0x27, 0x06, 0x00, 0x00, 0x11, } }, 
{ 1, { 0x22, 0xa1, 0x26, 0x00, 0x65, 0x67, 0x66, 0x06, 0x00, 0x00, 0x10,
       0x02, 0x21, 0x2a, 0x00, 0xa2, 0x6f, 0x36, 0x06, 0x00, 0x00, 0x11, } }, 
{ 1, { 0x21, 0x31, 0x11, 0x00, 0x85, 0x68, 0x66, 0x06, 0x00, 0x00, 0x10,
       0x32, 0x31, 0x19, 0x00, 0xd5, 0x68, 0x39, 0x06, 0x00, 0x00, 0x11, } }, 
{ 1, { 0x20, 0xa0, 0x00, 0x00, 0x77, 0x77, 0x08, 0x58, 0x00, 0x00, 0x2e,
       0x24, 0x21, 0xd3, 0x00, 0x76, 0x76, 0xa9, 0x08, 0x04, 0x00, 0x21, } }, 
{ 1, { 0x2b, 0x26, 0x12, 0x00, 0x73, 0x67, 0x68, 0x97, 0x00, 0x00, 0x1e,
       0x22, 0x21, 0x14, 0x00, 0x66, 0x60, 0x46, 0xb6, 0x00, 0x00, 0x11, } }, 
{ 1, { 0x61, 0xa1, 0x16, 0x00, 0x67, 0x77, 0x56, 0x06, 0x02, 0x00, 0x30,
       0x21, 0x01, 0x15, 0x00, 0xd7, 0x40, 0x66, 0xb6, 0x02, 0x00, 0x31, } }, 
{ 1, { 0x61, 0xa1, 0x31, 0x00, 0xc4, 0x50, 0x56, 0xb6, 0x02, 0x00, 0x20,
       0x61, 0x81, 0x30, 0x00, 0xc4, 0x50, 0x66, 0xb6, 0x01, 0x00, 0x21, } }, 
{ 1, { 0x31, 0x31, 0x2b, 0x00, 0x57, 0x68, 0x46, 0x06, 0x04, 0x00, 0x30,
       0x32, 0x31, 0x30, 0x00, 0x47, 0x68, 0x69, 0x06, 0x04, 0x00, 0x31, } }, 
{ 1, { 0x21, 0x21, 0x00, 0x0d, 0xd4, 0xc2, 0x1b, 0x26, 0x04, 0x06, 0x21,
       0x21, 0x21, 0x00, 0x00, 0xc6, 0xd0, 0x19, 0x27, 0x04, 0x06, 0x21, } }, 
{ 1, { 0x21, 0x20, 0x11, 0x00, 0xd4, 0xc2, 0x1b, 0x36, 0x04, 0x03, 0x10,
       0x21, 0x21, 0x10, 0x00, 0xc9, 0xd0, 0x49, 0x37, 0x04, 0x01, 0x11, } }, 
{ 1, { 0x22, 0xa1, 0x0e, 0x00, 0x76, 0x67, 0x66, 0x06, 0x00, 0x00, 0x26,
       0x22, 0x21, 0x1e, 0x00, 0x75, 0x75, 0x06, 0x06, 0x02, 0x00, 0x21, } }, 
{ 1, { 0xd1, 0xa0, 0x00, 0x00, 0xb7, 0x87, 0x66, 0x06, 0x02, 0x02, 0x20,
       0x01, 0x60, 0x14, 0x00, 0x75, 0xb5, 0x66, 0x06, 0x02, 0x02, 0x21, } }, 
{ 1, { 0x30, 0x22, 0x0c, 0x00, 0xf4, 0x62, 0xa0, 0x0e, 0x00, 0x00, 0x10,
       0x20, 0x21, 0x14, 0x00, 0x77, 0x90, 0x4e, 0x1c, 0x00, 0x00, 0x11, } }, 
{ 1, { 0x20, 0x32, 0x1a, 0x00, 0x76, 0x47, 0x64, 0x05, 0x00, 0x00, 0x16,
       0x31, 0x21, 0x1b, 0x00, 0x75, 0x75, 0x03, 0x05, 0x00, 0x00, 0x11, } }, 
{ 1, { 0x03, 0x06, 0x00, 0x3f, 0xf2, 0xf0, 0xa6, 0xfb, 0x00, 0x00, 0x17,
       0x01, 0x00, 0x15, 0x00, 0xf0, 0xf3, 0xf8, 0xc8, 0x00, 0x00, 0x10, } }, 
{ 1, { 0x02, 0x21, 0x0b, 0x00, 0xf3, 0xf1, 0xfb, 0x2b, 0x01, 0x01, 0x16,
       0x01, 0x21, 0xe1, 0x00, 0xf3, 0xf0, 0xfb, 0x2b, 0x05, 0x00, 0x11, } }, 
{ 1, { 0x12, 0x12, 0xc0, 0x00, 0xf2, 0xe2, 0x23, 0x55, 0x00, 0x00, 0x10,
       0x31, 0x30, 0x1c, 0x00, 0x69, 0x52, 0x33, 0x15, 0x02, 0x02, 0x11, } }, 
{ 1, { 0x01, 0x31, 0x18, 0x00, 0x21, 0x22, 0x53, 0x55, 0x00, 0x00, 0x30,
       0x11, 0x31, 0xd4, 0x00, 0x21, 0x21, 0x53, 0x45, 0x01, 0x00, 0x31, } }, 
{ 1, { 0x20, 0x01, 0x00, 0x00, 0xc3, 0xf0, 0xf4, 0xf5, 0x02, 0x00, 0x10,
       0x01, 0x21, 0x08, 0x00, 0x32, 0x20, 0x14, 0x04, 0x05, 0x00, 0x11, } }, 
{ 0, { 0x30, 0x20, 0xd1, 0x00, 0xa2, 0x63, 0x40, 0x48, 0x00, 0x00, 0x20, } }, 
{ 1, { 0x06, 0x21, 0x08, 0x00, 0xf0, 0x45, 0xe4, 0x36, 0x00, 0x00, 0x30,
       0x01, 0x21, 0x46, 0x00, 0xe5, 0x40, 0x34, 0x05, 0x04, 0x00, 0x31, } }, 
{ 1, { 0x21, 0x21, 0x40, 0x00, 0x63, 0x30, 0x83, 0x13, 0x00, 0x00, 0x10,
       0x21, 0x21, 0x17, 0x00, 0x63, 0x40, 0xa3, 0x13, 0x05, 0x04, 0x11, } }, 
{ 1, { 0x31, 0x30, 0x00, 0xc4, 0x60, 0x64, 0x15, 0xf8, 0x00, 0x02, 0x35,
       0x31, 0x31, 0x00, 0x00, 0xb0, 0xd0, 0xc5, 0x45, 0x00, 0x00, 0x31, } }, 
{ 1, { 0x31, 0x12, 0x00, 0x8c, 0x44, 0x12, 0xf4, 0xb3, 0x00, 0x00, 0x25,
       0x10, 0x30, 0x00, 0x00, 0x30, 0x74, 0xb4, 0x04, 0x01, 0x01, 0x21, } }, 
{ 1, { 0x08, 0x83, 0xc0, 0x00, 0xfb, 0xf5, 0x66, 0x68, 0x00, 0x00, 0x10,
       0x21, 0x21, 0x14, 0x00, 0xf0, 0xde, 0x25, 0x05, 0x00, 0x00, 0x11, } }, 
{ 1, { 0x08, 0x01, 0x01, 0x00, 0x33, 0x31, 0xf2, 0xf2, 0x00, 0x00, 0x20,
       0x01, 0x01, 0x04, 0x00, 0x31, 0x32, 0xf2, 0xf2, 0x01, 0x00, 0x21, } }, 
{ 1, { 0x17, 0x91, 0xc0, 0x00, 0xf1, 0xf1, 0x31, 0x22, 0x00, 0x00, 0x18,
       0x95, 0x91, 0x9b, 0x00, 0xf1, 0xf1, 0x21, 0x23, 0x00, 0x00, 0x11, } }, 
{ 1, { 0x01, 0x17, 0x00, 0x00, 0xd2, 0xd8, 0x24, 0x34, 0x00, 0x00, 0x20,
       0x01, 0x20, 0x17, 0x00, 0xea, 0xe1, 0x34, 0x14, 0x02, 0x02, 0x21, } }, 
{ 1, { 0x06, 0x01, 0x08, 0x00, 0xf0, 0xf5, 0xe4, 0x34, 0x00, 0x00, 0x30,
       0x01, 0xa0, 0x44, 0x00, 0xe5, 0xe0, 0x34, 0x44, 0x02, 0x02, 0x31, } }, 
{ 1, { 0x04, 0x01, 0x02, 0x00, 0x11, 0x21, 0x32, 0x22, 0x00, 0x00, 0x18,
       0x01, 0x01, 0x41, 0x00, 0x11, 0x11, 0x24, 0x23, 0x00, 0x00, 0x11, } }, 
{ 1, { 0x05, 0x01, 0xc0, 0x00, 0xf8, 0xe1, 0xd3, 0x03, 0x00, 0x00, 0x20,
       0x21, 0x21, 0x10, 0x00, 0x69, 0x52, 0x33, 0x13, 0x01, 0x00, 0x21, } }, 
{ 1, { 0x02, 0x21, 0x00, 0x00, 0x83, 0x61, 0xf3, 0x23, 0x01, 0x00, 0x16,
       0x01, 0x21, 0x0b, 0x00, 0x73, 0x52, 0xf3, 0x23, 0x05, 0x00, 0x11, } }, 
{ 1, { 0x25, 0x01, 0x12, 0x00, 0xf7, 0xf8, 0x44, 0x04, 0x04, 0x04, 0x30,
       0xa7, 0x01, 0x17, 0x00, 0x80, 0xf3, 0x64, 0x74, 0x04, 0x04, 0x31, } }, 
{ 1, { 0x01, 0x05, 0x00, 0x25, 0xf4, 0xf8, 0xa6, 0xfb, 0x04, 0x00, 0x35,
       0x01, 0x02, 0x10, 0x00, 0xf4, 0xf6, 0xfb, 0xc7, 0x00, 0x04, 0x30, } }, 
{ 1, { 0x04, 0x07, 0x09, 0x00, 0xfa, 0xf8, 0xa6, 0xfb, 0x00, 0x00, 0x24,
       0x01, 0x01, 0x16, 0x00, 0xf3, 0xf3, 0x95, 0xc5, 0x04, 0x04, 0x21, } }, 
{ 1, { 0x01, 0x05, 0x00, 0x25, 0xf4, 0xfa, 0xa4, 0xfb, 0x04, 0x04, 0x25,
       0x01, 0x01, 0x18, 0x00, 0xf4, 0xf3, 0xf5, 0xc5, 0x04, 0x04, 0x20, } }, 
{ 1, { 0x09, 0x03, 0x00, 0x00, 0xf8, 0xf8, 0x6e, 0x6e, 0x04, 0x04, 0x20,
       0x01, 0x86, 0x00, 0x00, 0xf4, 0xf7, 0x16, 0xea, 0x00, 0x00, 0x21, } }, 
{ 1, { 0x11, 0x32, 0x0c, 0x00, 0xd0, 0x80, 0xf3, 0x05, 0x00, 0x00, 0x30,
       0x31, 0x24, 0xc9, 0x00, 0x40, 0x50, 0x09, 0x09, 0x00, 0x04, 0x31, } }, 
{ 1, { 0x24, 0x20, 0xc9, 0x00, 0xaa, 0x95, 0x58, 0x08, 0x04, 0x03, 0x30,
       0x24, 0x20, 0xc9, 0x00, 0xc7, 0x72, 0x88, 0x28, 0x05, 0x03, 0x31, } }, 
{ 1, { 0x11, 0x32, 0x0c, 0x00, 0xd0, 0x80, 0xf3, 0x05, 0x00, 0x00, 0x10,
       0x31, 0x24, 0xcc, 0x00, 0x60, 0x60, 0x06, 0x06, 0x00, 0x04, 0x11, } }, 
{ 1, { 0x16, 0x19, 0x4f, 0x00, 0xf6, 0xf3, 0x53, 0x53, 0x00, 0x00, 0x30,
       0x15, 0x14, 0x41, 0x00, 0xf1, 0xf3, 0x52, 0x52, 0x06, 0x06, 0x31, } }, 
{ 1, { 0x17, 0x12, 0x00, 0x00, 0xfa, 0xf5, 0x57, 0x58, 0x00, 0x00, 0x10,
       0x14, 0x12, 0x40, 0x00, 0xf7, 0xf5, 0x52, 0x59, 0x06, 0x06, 0x11, } }, 
{ 1, { 0x08, 0x01, 0x1f, 0x00, 0xc6, 0x82, 0x37, 0x95, 0x00, 0x00, 0x36,
       0x05, 0x11, 0x0f, 0x00, 0x65, 0x52, 0x55, 0x75, 0x00, 0x00, 0x31, } }, 
{ 1, { 0x02, 0x01, 0x09, 0x00, 0xfc, 0xf7, 0xd6, 0xf7, 0x02, 0x00, 0x20,
       0x07, 0x03, 0x3f, 0xc0, 0xfa, 0xf5, 0x4e, 0xfe, 0x00, 0x00, 0x21, } }, 
{ 1, { 0x1a, 0x30, 0x00, 0x00, 0xfb, 0xf3, 0x57, 0x54, 0x00, 0x01, 0x11,
       0x11, 0x10, 0x40, 0x00, 0xc5, 0xc5, 0x52, 0x57, 0x01, 0x00, 0x11, } }, 
{ 1, { 0x12, 0x30, 0x01, 0x00, 0xfb, 0xf3, 0xa7, 0x53, 0x00, 0x00, 0x30,
       0x10, 0x10, 0xff, 0x00, 0xf4, 0xc4, 0x52, 0x57, 0x00, 0x00, 0x31, } }, 
{ 1, { 0x2b, 0x0b, 0x00, 0x00, 0xff, 0xf7, 0x0e, 0xfe, 0x00, 0x00, 0x3e,
       0x00, 0x20, 0xc0, 0x00, 0xf6, 0xff, 0xfe, 0x0c, 0x02, 0x00, 0x31, } }, 
{ 1, { 0x2a, 0x0b, 0x00, 0x00, 0xff, 0x2f, 0x0e, 0x0e, 0x04, 0x04, 0x2e,
       0x28, 0x09, 0x00, 0x00, 0xf9, 0x29, 0x0e, 0x0e, 0x04, 0x04, 0x21, } }, 
{ 1, { 0x0b, 0x0b, 0x00, 0x00, 0x64, 0x77, 0xf8, 0xf8, 0x04, 0x04, 0x1e,
       0x0b, 0x09, 0x10, 0x00, 0x56, 0x59, 0xf8, 0xe8, 0x04, 0x04, 0x11, } }, 
{ 1, { 0x2b, 0x25, 0x05, 0x00, 0x79, 0x77, 0x05, 0xa5, 0x00, 0x00, 0x2e,
       0x26, 0x01, 0x10, 0x00, 0x66, 0x76, 0xb9, 0xde, 0x04, 0x00, 0x21, } }, 
{ 0, { 0x30, 0x10, 0x00, 0x00, 0x11, 0x11, 0x00, 0x32, 0x00, 0x03, 0x3e, } }, 
{ 0, { 0x0a, 0x09, 0x00, 0x00, 0x59, 0x69, 0xfe, 0xfe, 0x00, 0x00, 0x21, } }, 
{ 1, { 0x0a, 0x02, 0x0b, 0x00, 0xe5, 0xe4, 0xf5, 0xf5, 0x05, 0x00, 0x30,
       0x08, 0x02, 0x43, 0x00, 0xf5, 0xf4, 0xf5, 0xf5, 0x03, 0x00, 0x31, } }, 
{ 1, { 0x0b, 0x04, 0x00, 0x80, 0x82, 0x77, 0xfa, 0xfa, 0x00, 0x05, 0x2e,
       0x20, 0x20, 0x11, 0x00, 0xf7, 0xfa, 0xfa, 0xfb, 0x00, 0x00, 0x21, } }, 
{ 1, { 0x2b, 0x27, 0x00, 0x00, 0xf0, 0x23, 0x01, 0x13, 0x00, 0x05, 0x3e,
       0x20, 0x20, 0x7f, 0x00, 0xfb, 0xf4, 0xe5, 0xf5, 0x00, 0x00, 0x31, } }, 
{ 1, { 0x0b, 0x01, 0x40, 0xc0, 0xf5, 0xf7, 0x67, 0x64, 0x00, 0x00, 0x1e,
       0x07, 0x01, 0x3f, 0x00, 0xf7, 0xfa, 0x34, 0x9e, 0x04, 0x04, 0x11, } }, 
{ 0, { 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x10, } }, 
{ 0, { 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x30, } }, 
{ 0, { 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x10, } }, 
{ 0, { 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x10, } }, 
{ 0, { 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x30, } }, 
{ 0, { 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x10, } }, 
{ 0, { 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x30, } }, 
{ 0, { 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x10, } }, 
{ 0, { 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x30, } }, 
{ 0, { 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x30, } }, 
{ 0, { 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x20, } }, 
{ 0, { 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x20, } }, 
{ 0, { 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x10, } }, 
{ 0, { 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x20, } }, 
{ 0, { 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x10, } }, 
{ 0, { 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x10, } }, 
{ 0, { 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x10, } }, 
{ 0, { 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x30, } }, 
{ 0, { 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x10, } }, 
{ 0, { 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x20, } }, 
{ 0, { 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x10, } }, 
{ 0, { 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x30, } }, 
{ 0, { 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x30, } }, 
{ 0, { 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x10, } }, 
{ 0, { 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x20, } }, 
{ 0, { 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x10, } }, 
{ 0, { 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x10, } }, 
{ 0, { 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x20, } }, 
{ 0, { 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x10, } }, 
{ 0, { 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x10, } }, 
{ 0, { 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x10, } }, 
{ 0, { 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x20, } }, 
{ 0, { 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x10, } }, 
{ 0, { 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x30, } }, 
{ 0, { 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x30, } }, 
{ 0, { 0x52, 0x11, 0x1b, 0x00, 0x68, 0xd6, 0x1c, 0x43, 0x05, 0x00, 0x2a, } }, 
{ 0, { 0x40, 0x01, 0x18, 0x00, 0x68, 0xd6, 0x19, 0x46, 0x02, 0x00, 0x1e, } }, 
{ 0, { 0x02, 0x04, 0x07, 0x00, 0xfa, 0xda, 0xbb, 0xbe, 0x00, 0x00, 0x38, } }, 
{ 1, { 0x12, 0x01, 0xc0, 0x00, 0xf3, 0xf6, 0xf0, 0xf6, 0x07, 0x00, 0x3e,
       0x10, 0x10, 0x0c, 0x00, 0xf0, 0x91, 0x00, 0x05, 0x03, 0x06, 0x30, } }, 
{ 0, { 0x01, 0x05, 0x07, 0x00, 0x9a, 0x9a, 0xb6, 0xbd, 0x02, 0x00, 0x3e, } }, 
{ 1, { 0x17, 0x01, 0xc0, 0x00, 0xf3, 0xf8, 0xf0, 0xb7, 0x00, 0x00, 0x1e,
       0x10, 0x10, 0x0c, 0x00, 0xf0, 0xd1, 0x60, 0x75, 0x00, 0x06, 0x10, } }, 
{ 0, { 0x01, 0x01, 0x0d, 0x00, 0xe8, 0xa5, 0xec, 0xf7, 0x00, 0x00, 0x16, } }, 
{ 0, { 0x2f, 0x31, 0x07, 0x00, 0x73, 0xc4, 0xc2, 0xb4, 0x00, 0x00, 0x2a, } }, 
{ 0, { 0x01, 0x01, 0x0d, 0x00, 0xe8, 0xa5, 0xec, 0xf7, 0x01, 0x00, 0x36, } }, 
{ 0, { 0x13, 0x10, 0xcd, 0x80, 0xf5, 0xd5, 0x92, 0xd4, 0x01, 0x00, 0x1a, } }, 
{ 0, { 0x01, 0x01, 0x0d, 0x00, 0xe8, 0xa5, 0xec, 0xf5, 0x00, 0x00, 0x16, } }, 
{ 0, { 0x3c, 0x31, 0x90, 0x00, 0x73, 0xc4, 0xb1, 0xf3, 0x00, 0x00, 0x2a, } }, 
};

const struct opl_operators opl2_instrs[OPL_NINSTR] = {
{ 0, { 0x01, 0x10, 0x1b, 0x00, 0xc3, 0x92, 0x23, 0x62, 0x02, 0x01, 0x1e, } }, 
{ 0, { 0x03, 0x01, 0x7d, 0x00, 0xf3, 0xf3, 0xf5, 0xf5, 0x00, 0x04, 0x30, } }, 
{ 0, { 0x03, 0x01, 0x61, 0x00, 0xfb, 0xf5, 0xf5, 0xf5, 0x04, 0x04, 0x30, } }, 
{ 0, { 0x03, 0x01, 0xd7, 0x00, 0xf3, 0xf2, 0xf5, 0xf5, 0x04, 0x04, 0x20, } }, 
{ 0, { 0x35, 0x12, 0xd4, 0x00, 0x94, 0x92, 0x91, 0x62, 0x02, 0x00, 0x2e, } }, 
{ 0, { 0x11, 0x10, 0x00, 0x00, 0xe1, 0x92, 0x63, 0x72, 0x02, 0x00, 0x17, } }, 
{ 0, { 0x22, 0x01, 0x8a, 0x00, 0xe3, 0xf4, 0x79, 0xa8, 0x05, 0x04, 0x24, } }, 
{ 0, { 0x22, 0x01, 0x9c, 0x00, 0xf4, 0xf3, 0x18, 0x55, 0x05, 0x05, 0x30, } }, 
{ 0, { 0x02, 0x01, 0x00, 0x00, 0x93, 0xb6, 0x74, 0x45, 0x00, 0x00, 0x25, } }, 
{ 0, { 0x1b, 0x17, 0x5a, 0x00, 0xd6, 0xf3, 0x53, 0x53, 0x00, 0x00, 0x20, } }, 
{ 0, { 0x1b, 0x14, 0x5a, 0x00, 0xf6, 0x63, 0xf6, 0x73, 0x00, 0x00, 0x20, } }, 
{ 0, { 0x8b, 0x84, 0x5a, 0x00, 0xd8, 0xf4, 0x16, 0xe5, 0x00, 0x00, 0x30, } }, 
{ 0, { 0x08, 0x01, 0xc0, 0x00, 0xfd, 0xf6, 0x56, 0x68, 0x00, 0x00, 0x30, } }, 
{ 0, { 0x08, 0x03, 0xc0, 0x00, 0xf9, 0xf7, 0x56, 0x68, 0x00, 0x00, 0x30, } }, 
{ 0, { 0x35, 0x11, 0xcd, 0x00, 0xf2, 0xf2, 0xf1, 0xf2, 0x00, 0x00, 0x34, } }, 
{ 0, { 0x18, 0x02, 0x9a, 0x00, 0xb3, 0xf0, 0x8a, 0xd3, 0x01, 0x01, 0x1c, } }, 
{ 0, { 0x24, 0x61, 0xd4, 0x00, 0x81, 0x61, 0x06, 0xa6, 0x03, 0x00, 0x12, } }, 
{ 0, { 0xa3, 0x21, 0x95, 0x00, 0x81, 0xd1, 0xf9, 0x0d, 0x01, 0x00, 0x12, } }, 
{ 0, { 0x25, 0x61, 0xd2, 0x00, 0x81, 0xd1, 0x06, 0x35, 0x00, 0x00, 0x3e, } }, 
{ 0, { 0x27, 0x21, 0x23, 0x00, 0x74, 0x60, 0x11, 0x05, 0x00, 0x00, 0x22, } }, 
{ 0, { 0x22, 0x21, 0x24, 0x00, 0x76, 0x76, 0x06, 0x06, 0x04, 0x04, 0x30, } }, 
{ 0, { 0x22, 0x21, 0x1b, 0x00, 0x66, 0x66, 0x05, 0x06, 0x00, 0x04, 0x30, } }, 
{ 0, { 0x22, 0xb2, 0x15, 0x00, 0x41, 0x31, 0x04, 0x04, 0x03, 0x01, 0x22, } }, 
{ 0, { 0x21, 0x21, 0x1e, 0x00, 0x76, 0x66, 0x04, 0x17, 0x04, 0x05, 0x10, } }, 
{ 0, { 0x01, 0x11, 0xc0, 0x00, 0xb5, 0xd2, 0x52, 0x72, 0x01, 0x00, 0x20, } }, 
{ 0, { 0x23, 0x11, 0x8a, 0x00, 0x85, 0xd2, 0x54, 0x73, 0x01, 0x00, 0x28, } }, 
{ 0, { 0x04, 0x01, 0x12, 0x00, 0xfd, 0xf2, 0x43, 0xa6, 0x00, 0x00, 0x20, } }, 
{ 0, { 0x23, 0x01, 0x00, 0x00, 0xfc, 0xf4, 0x67, 0xc6, 0x03, 0x02, 0x20, } }, 
{ 0, { 0x01, 0x01, 0x06, 0x00, 0xf6, 0xf6, 0x27, 0x25, 0x04, 0x01, 0x10, } }, 
{ 0, { 0x21, 0x21, 0xc1, 0x00, 0xf2, 0xf2, 0x13, 0x16, 0x00, 0x04, 0x30, } }, 
{ 0, { 0x21, 0x21, 0xc1, 0x00, 0xf2, 0xf2, 0x14, 0x15, 0x00, 0x04, 0x10, } }, 
{ 0, { 0x0b, 0x01, 0x40, 0x00, 0x65, 0xf2, 0x1a, 0x3e, 0x00, 0x03, 0x3c, } }, 
{ 0, { 0x01, 0x01, 0x0f, 0x00, 0xd8, 0x82, 0x96, 0x66, 0x00, 0x00, 0x10, } }, 
{ 0, { 0x01, 0x01, 0x0b, 0x00, 0xf2, 0xf6, 0x97, 0xe6, 0x04, 0x01, 0x20, } }, 
{ 0, { 0x01, 0x01, 0x04, 0x00, 0xf2, 0xf6, 0x95, 0xe6, 0x04, 0x01, 0x10, } }, 
{ 0, { 0x01, 0x01, 0x48, 0x00, 0xc7, 0x91, 0x96, 0xe6, 0x00, 0x00, 0x30, } }, 
{ 0, { 0x0b, 0x01, 0x00, 0x00, 0xfa, 0xf3, 0x86, 0xb6, 0x06, 0x01, 0x10, } }, 
{ 0, { 0x0b, 0x01, 0x00, 0x00, 0xfa, 0xf3, 0x87, 0xb7, 0x02, 0x01, 0x10, } }, 
{ 0, { 0xc1, 0x01, 0x00, 0x00, 0xe5, 0xf5, 0x65, 0x95, 0x00, 0x00, 0x21, } }, 
{ 0, { 0xc1, 0x01, 0x00, 0x00, 0xf2, 0xf2, 0x45, 0x96, 0x00, 0x00, 0x21, } }, 
{ 0, { 0x62, 0x61, 0xca, 0x00, 0xb4, 0x44, 0x25, 0x06, 0x04, 0x04, 0x3e, } }, 
{ 0, { 0x24, 0x21, 0xcb, 0x00, 0x88, 0x66, 0x24, 0x15, 0x04, 0x04, 0x1e, } }, 
{ 0, { 0x24, 0x21, 0xd3, 0x00, 0x88, 0x66, 0x24, 0x16, 0x04, 0x04, 0x1e, } }, 
{ 0, { 0x71, 0x61, 0x23, 0x00, 0x51, 0x52, 0x43, 0x15, 0x00, 0x00, 0x1e, } }, 
{ 0, { 0xa2, 0xa1, 0xcb, 0x00, 0x84, 0x51, 0x13, 0x55, 0x00, 0x02, 0x2a, } }, 
{ 0, { 0xf1, 0xe2, 0x00, 0x80, 0xf5, 0xfd, 0xa8, 0xac, 0x00, 0x00, 0x37, } }, 
{ 0, { 0x22, 0x02, 0x00, 0x00, 0xd4, 0xc4, 0x34, 0x26, 0x01, 0x00, 0x29, } }, 
{ 0, { 0x10, 0x10, 0x55, 0x00, 0x30, 0xb0, 0x09, 0x04, 0x03, 0x03, 0x2a, } }, 
{ 0, { 0x62, 0xa2, 0x8b, 0x40, 0x71, 0x43, 0x33, 0xe5, 0x01, 0x00, 0x24, } }, 
{ 0, { 0x61, 0x21, 0x8b, 0x40, 0x71, 0x43, 0x33, 0xe5, 0x01, 0x00, 0x14, } }, 
{ 0, { 0x61, 0x22, 0x8b, 0x40, 0x71, 0x43, 0x33, 0xe5, 0x01, 0x00, 0x26, } }, 
{ 0, { 0x01, 0x21, 0x13, 0x00, 0x51, 0x44, 0x15, 0x04, 0x02, 0x01, 0x10, } }, 
{ 0, { 0xf3, 0xc1, 0x95, 0x00, 0x32, 0x43, 0x40, 0x46, 0x02, 0x01, 0x3e, } }, 
{ 0, { 0xf3, 0xc1, 0x51, 0x00, 0xa2, 0x62, 0xa4, 0x84, 0x00, 0x01, 0x1e, } }, 
{ 0, { 0xe0, 0xe3, 0x4d, 0x00, 0x52, 0x65, 0x51, 0x35, 0x02, 0x00, 0x36, } }, 
{ 0, { 0x20, 0x00, 0x07, 0x00, 0xf7, 0x96, 0xb5, 0x46, 0x00, 0x00, 0x20, } }, 
{ 0, { 0x21, 0xa1, 0x1c, 0x00, 0x53, 0x52, 0x14, 0x35, 0x00, 0x00, 0x1c, } }, 
{ 0, { 0x21, 0xa2, 0x1a, 0x80, 0x53, 0x52, 0x19, 0x39, 0x00, 0x00, 0x2a, } }, 
{ 0, { 0x21, 0x21, 0x21, 0x00, 0x63, 0x85, 0x8c, 0x2c, 0x00, 0x00, 0x3c, } }, 
{ 0, { 0xe1, 0xe3, 0x98, 0x00, 0x71, 0x81, 0xa5, 0x96, 0x01, 0x00, 0x1c, } }, 
{ 0, { 0x31, 0x21, 0x55, 0x00, 0x53, 0xaa, 0x54, 0x15, 0x00, 0x00, 0x3e, } }, 
{ 0, { 0x21, 0xa2, 0x1b, 0x80, 0x53, 0x52, 0x15, 0x36, 0x00, 0x00, 0x3a, } }, 
{ 0, { 0x21, 0x21, 0x20, 0x00, 0x83, 0x81, 0x75, 0x86, 0x05, 0x04, 0x1e, } }, 
{ 0, { 0x21, 0x21, 0x8e, 0x80, 0x9b, 0x90, 0x25, 0x05, 0x00, 0x00, 0x18, } }, 
{ 0, { 0x01, 0x02, 0x15, 0x00, 0x70, 0x70, 0xe5, 0xe6, 0x05, 0x00, 0x20, } }, 
{ 0, { 0x01, 0x01, 0x14, 0x00, 0x80, 0x70, 0xe5, 0xe6, 0x05, 0x00, 0x30, } }, 
{ 0, { 0x01, 0x01, 0x12, 0x00, 0x80, 0x70, 0xe5, 0xe6, 0x05, 0x00, 0x30, } }, 
{ 0, { 0x01, 0xa3, 0x91, 0x00, 0x51, 0x52, 0x57, 0x77, 0x01, 0x00, 0x2a, } }, 
{ 0, { 0x71, 0x64, 0xc9, 0x00, 0x6e, 0x8b, 0x13, 0x07, 0x00, 0x01, 0x12, } }, 
{ 0, { 0xa1, 0xa3, 0x25, 0x00, 0x71, 0x82, 0xa5, 0x97, 0x00, 0x00, 0x10, } }, 
{ 0, { 0x01, 0x24, 0x13, 0x00, 0xf0, 0x80, 0x22, 0x07, 0x00, 0x00, 0x20, } }, 
{ 0, { 0x32, 0x31, 0x1d, 0x00, 0xa2, 0x51, 0x14, 0x24, 0x04, 0x00, 0x3a, } }, 
{ 0, { 0x21, 0xa1, 0x20, 0x00, 0x77, 0x65, 0x43, 0x06, 0x00, 0x00, 0x20, } }, 
{ 0, { 0x01, 0xa1, 0xa4, 0x00, 0x6a, 0x63, 0x64, 0x66, 0x02, 0x00, 0x20, } }, 
{ 0, { 0x22, 0x21, 0xac, 0x00, 0x67, 0x65, 0x89, 0x28, 0x00, 0x00, 0x2e, } }, 
{ 0, { 0xe3, 0xa1, 0xe9, 0x00, 0x6d, 0x65, 0x8d, 0x29, 0x00, 0x00, 0x1e, } }, 
{ 0, { 0x20, 0x26, 0x10, 0x00, 0xd8, 0x68, 0x68, 0x97, 0x00, 0x00, 0x3e, } }, 
{ 0, { 0x22, 0x21, 0x6c, 0x00, 0x47, 0x65, 0x0f, 0x2a, 0x00, 0x00, 0x1e, } }, 
{ 0, { 0xe1, 0xe2, 0xff, 0x00, 0x70, 0x62, 0x0c, 0xfb, 0x00, 0x00, 0x3e, } }, 
{ 0, { 0x31, 0x31, 0x2b, 0x00, 0x57, 0x68, 0x46, 0x06, 0x04, 0x00, 0x20, } }, 
{ 0, { 0x91, 0x13, 0x97, 0x80, 0x2a, 0x42, 0x02, 0xf3, 0x00, 0x00, 0x20, } }, 
{ 0, { 0x00, 0xa2, 0x91, 0x00, 0x51, 0x52, 0x53, 0x76, 0x01, 0x00, 0x3e, } }, 
{ 0, { 0x23, 0x21, 0xd8, 0x00, 0x50, 0x72, 0x00, 0xf8, 0x03, 0x00, 0x1e, } }, 
{ 0, { 0xd1, 0xa0, 0x00, 0x00, 0xb7, 0x87, 0x66, 0x06, 0x02, 0x02, 0x10, } }, 
{ 0, { 0x00, 0x15, 0x4f, 0x00, 0x71, 0x52, 0x53, 0x74, 0x00, 0x00, 0x26, } }, 
{ 0, { 0xe0, 0xe1, 0xc5, 0x00, 0x52, 0x65, 0xa1, 0x35, 0x00, 0x00, 0x36, } }, 
{ 0, { 0xb0, 0xb1, 0x00, 0x00, 0xd5, 0x26, 0x13, 0x13, 0x01, 0x00, 0x35, } }, 
{ 0, { 0x21, 0xa1, 0x1d, 0x80, 0x53, 0x52, 0x14, 0x35, 0x00, 0x00, 0x2a, } }, 
{ 0, { 0x21, 0xa1, 0x1b, 0x00, 0x51, 0x82, 0x15, 0x45, 0x01, 0x00, 0x10, } }, 
{ 0, { 0xe1, 0xa1, 0xa3, 0x00, 0x6e, 0x65, 0x8f, 0x06, 0x01, 0x00, 0x2e, } }, 
{ 0, { 0x20, 0x01, 0x00, 0x00, 0xc3, 0xf0, 0xf4, 0xf5, 0x02, 0x00, 0x10, } }, 
{ 0, { 0xf1, 0xc2, 0x51, 0x00, 0xa2, 0x63, 0x40, 0x46, 0x00, 0x00, 0x3e, } }, 
{ 0, { 0xe1, 0xa2, 0x4d, 0x00, 0x71, 0x43, 0x33, 0xe5, 0x01, 0x00, 0x16, } }, 
{ 0, { 0x21, 0x21, 0x40, 0x00, 0x63, 0x30, 0x83, 0x15, 0x00, 0x00, 0x10, } }, 
{ 0, { 0x24, 0x21, 0x24, 0x00, 0x67, 0x65, 0x8f, 0x2a, 0x02, 0x00, 0x3e, } }, 
{ 0, { 0x24, 0x21, 0x15, 0x00, 0x27, 0x65, 0x88, 0x26, 0x02, 0x00, 0x1e, } }, 
{ 0, { 0x08, 0x83, 0xc0, 0x00, 0xfb, 0xf5, 0x66, 0x66, 0x00, 0x00, 0x10, } }, 
{ 0, { 0xf4, 0xe1, 0x54, 0x00, 0x25, 0xf0, 0xbd, 0x47, 0x00, 0x01, 0x10, } }, 
{ 0, { 0xb4, 0xa1, 0x0a, 0x00, 0x67, 0x65, 0x88, 0x2a, 0x02, 0x00, 0x20, } }, 
{ 0, { 0x23, 0x21, 0xdb, 0x00, 0x47, 0x65, 0x8f, 0x2a, 0x02, 0x00, 0x1e, } }, 
{ 0, { 0x24, 0x21, 0x22, 0x00, 0x47, 0x65, 0x8f, 0x2a, 0x02, 0x00, 0x1e, } }, 
{ 0, { 0xe6, 0x30, 0x0b, 0x00, 0x25, 0xf0, 0xb5, 0x45, 0x00, 0x00, 0x28, } }, 
{ 0, { 0xe3, 0x70, 0x50, 0x80, 0x25, 0xf0, 0xb0, 0x45, 0x00, 0x00, 0x30, } }, 
{ 0, { 0xe6, 0xe0, 0x15, 0x00, 0x70, 0x62, 0x07, 0xf7, 0x00, 0x00, 0x2e, } }, 
{ 0, { 0x25, 0x01, 0x12, 0x00, 0xf7, 0xf8, 0x43, 0x05, 0x04, 0x04, 0x30, } }, 
{ 0, { 0x01, 0x00, 0x00, 0x00, 0xf4, 0xf8, 0xa6, 0x35, 0x04, 0x00, 0x35, } }, 
{ 0, { 0x04, 0x07, 0x09, 0x00, 0xfa, 0xf8, 0xa6, 0xfb, 0x00, 0x00, 0x14, } }, 
{ 0, { 0x00, 0x01, 0x13, 0x00, 0xf2, 0xf4, 0x13, 0xf5, 0x00, 0x00, 0x18, } }, 
{ 0, { 0x00, 0x01, 0x0f, 0x00, 0xf2, 0xf4, 0x13, 0xf5, 0x00, 0x00, 0x30, } }, 
{ 0, { 0x11, 0x32, 0x0c, 0x00, 0xd0, 0x80, 0xf2, 0x05, 0x00, 0x00, 0x10, } }, 
{ 0, { 0x10, 0x21, 0x43, 0x40, 0x9b, 0x69, 0x30, 0x0e, 0x03, 0x05, 0x30, } }, 
{ 0, { 0x11, 0x32, 0x0c, 0x00, 0xd0, 0x80, 0xf3, 0x05, 0x00, 0x00, 0x20, } }, 
{ 0, { 0x16, 0x17, 0x4f, 0x00, 0xf6, 0xf3, 0x53, 0x53, 0x00, 0x00, 0x20, } }, 
{ 0, { 0x17, 0x12, 0x00, 0x00, 0xfa, 0xf5, 0x57, 0x58, 0x00, 0x00, 0x30, } }, 
{ 0, { 0x00, 0x00, 0x0d, 0x00, 0xa8, 0xd6, 0x4b, 0x4a, 0x00, 0x01, 0x28, } }, 
{ 0, { 0x32, 0x10, 0x4a, 0x00, 0xf8, 0xf5, 0xff, 0x7f, 0x00, 0x00, 0x3e, } }, 
{ 0, { 0x00, 0x00, 0x0d, 0x00, 0xa8, 0xd6, 0x4c, 0x4f, 0x00, 0x00, 0x10, } }, 
{ 0, { 0x12, 0x30, 0x01, 0x00, 0xfb, 0xf3, 0xa7, 0x53, 0x00, 0x00, 0x10, } }, 
{ 0, { 0x2b, 0x0b, 0x00, 0x00, 0xff, 0xf7, 0x0e, 0xfe, 0x00, 0x00, 0x3e, } }, 
{ 0, { 0x01, 0x10, 0x07, 0x00, 0xf5, 0x54, 0xb5, 0x2f, 0x00, 0x00, 0x26, } }, 
{ 0, { 0x00, 0x18, 0x80, 0x00, 0xb3, 0xaf, 0x3f, 0xf0, 0x00, 0x00, 0x1a, } }, 
{ 0, { 0x60, 0xef, 0x40, 0x00, 0xb1, 0xb2, 0x73, 0x9b, 0x02, 0x03, 0x3e, } }, 
{ 0, { 0xaf, 0xb0, 0x40, 0x00, 0xa1, 0x52, 0x53, 0x73, 0x00, 0x00, 0x3e, } }, 
{ 0, { 0x00, 0x07, 0x2e, 0x00, 0xf0, 0x5f, 0xf0, 0xf0, 0x00, 0x00, 0x1e, } }, 
{ 0, { 0xe5, 0x70, 0x13, 0x00, 0x70, 0x62, 0x0f, 0xff, 0x00, 0x00, 0x30, } }, 
{ 0, { 0x00, 0x10, 0x00, 0x00, 0x68, 0xa5, 0xcf, 0x6b, 0x02, 0x00, 0x2e, } }, 
{ 0, { 0x2b, 0x27, 0x00, 0x00, 0x82, 0x77, 0x0a, 0x0a, 0x00, 0x05, 0x2e, } }, 
{ 0, { 0x00, 0x00, 0x0b, 0x00, 0xfa, 0xf6, 0x6f, 0x8f, 0x00, 0x00, 0x30, } }, 
{ 0, { 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x10, } }, 
{ 0, { 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x30, } }, 
{ 0, { 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x10, } }, 
{ 0, { 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x20, } }, 
{ 0, { 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x20, } }, 
{ 0, { 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x20, } }, 
{ 0, { 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x30, } }, 
{ 0, { 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x30, } }, 
{ 0, { 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x20, } }, 
{ 0, { 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x20, } }, 
{ 0, { 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x10, } }, 
{ 0, { 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x20, } }, 
{ 0, { 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x10, } }, 
{ 0, { 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x10, } }, 
{ 0, { 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x20, } }, 
{ 0, { 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x30, } }, 
{ 0, { 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x20, } }, 
{ 0, { 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x10, } }, 
{ 0, { 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x30, } }, 
{ 0, { 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x10, } }, 
{ 0, { 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x20, } }, 
{ 0, { 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x20, } }, 
{ 0, { 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x30, } }, 
{ 0, { 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x10, } }, 
{ 0, { 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x30, } }, 
{ 0, { 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x20, } }, 
{ 0, { 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x30, } }, 
{ 0, { 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x30, } }, 
{ 0, { 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x10, } }, 
{ 0, { 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x30, } }, 
{ 0, { 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x10, } }, 
{ 0, { 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x20, } }, 
{ 0, { 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x10, } }, 
{ 0, { 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x10, } }, 
{ 0, { 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x20, } }, 
{ 0, { 0x42, 0x01, 0x1b, 0x00, 0x68, 0xd6, 0x1c, 0x45, 0x03, 0x00, 0x2e, } }, 
{ 0, { 0x40, 0x01, 0x18, 0x00, 0x68, 0xd6, 0x19, 0x46, 0x02, 0x00, 0x2e, } }, 
{ 0, { 0x02, 0x04, 0x07, 0x00, 0xfa, 0xda, 0xbb, 0xbe, 0x00, 0x00, 0x38, } }, 
{ 0, { 0x17, 0x00, 0x00, 0x00, 0xf3, 0xf8, 0xf0, 0xb7, 0x02, 0x00, 0x1e, } }, 
{ 0, { 0x01, 0x05, 0x07, 0x00, 0x9a, 0x9a, 0xb6, 0xb9, 0x02, 0x00, 0x3e, } }, 
{ 0, { 0x17, 0x03, 0x00, 0x00, 0xf1, 0xf7, 0xf0, 0xb7, 0x02, 0x00, 0x1e, } }, 
{ 0, { 0x01, 0x01, 0x0d, 0x00, 0xe8, 0xa5, 0xec, 0xf7, 0x00, 0x00, 0x16, } }, 
{ 0, { 0x2f, 0x31, 0x07, 0x00, 0x73, 0xc4, 0xc2, 0xb4, 0x00, 0x00, 0x3a, } }, 
{ 0, { 0x01, 0x01, 0x0d, 0x00, 0xe8, 0xa5, 0xec, 0xf7, 0x01, 0x00, 0x36, } }, 
{ 0, { 0x13, 0x10, 0xcd, 0x80, 0xf5, 0xd5, 0x92, 0xd4, 0x01, 0x00, 0x2a, } }, 
{ 0, { 0x01, 0x01, 0x0d, 0x00, 0xe8, 0xa5, 0xec, 0xf5, 0x00, 0x00, 0x16, } }, 
{ 0, { 0x3c, 0x31, 0x0a, 0x40, 0x73, 0xc4, 0xb1, 0xf3, 0x00, 0x00, 0x1a, } }, 
};
