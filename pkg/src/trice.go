// Package src is a helper for testing the target C-code.
// Each C function gets a Go wrapper which ist tested in appropriate test functions.
// For some reason inside the trice_test.go an 'import "C"' is not possible.
package src

// #include <stdint.h>
// #include "trice_test.h"
// #include "trice.h"
// #cgo CFLAGS: -g -Wall -flto
import "C"
import (
	"unsafe"
)

func setTriceBuffer(o []byte) {
	out := (*C.uint8_t)(unsafe.Pointer(&o[0]))
	C.SetTriceBuffer(out)
}

type triceTestFunction struct {
	tfn func() int
	exp []byte
}

var triceTests = []triceTestFunction{
	{func() int { return int(C.T1()) }, []byte{0x02, 0x03, 0x01, 0x01, 0x02, 0x13, 0x0f, 0x38, 0xcb, 0x11, 0x11, 0x11, 0x11, 0xc0, 0x01, 0x83, 0xe5, 0xff, 0xff, 0xff, 0xff, 0x00, 0x00, 0x00}},
	{func() int { return int(C.T1()) }, []byte{0x02, 0x03, 0x01, 0x01, 0x02, 0x13, 0x0f, 0x38, 0xcb, 0x11, 0x11, 0x11, 0x11, 0xc0, 0x01, 0x83, 0xe5, 0xff, 0xff, 0xff, 0xff, 0x00, 0x00, 0x00}},
	{func() int { return int(C.T1()) }, []byte{0x02, 0x03, 0x01, 0x01, 0x02, 0x13, 0x0f, 0x38, 0xcb, 0x11, 0x11, 0x11, 0x11, 0xc0, 0x01, 0x83, 0xe5, 0xff, 0xff, 0xff, 0xff, 0x00, 0x00, 0x00}},
	{func() int { return int(C.T2()) }, []byte{0x2, 0x3, 0x1, 0x1, 0x2, 0x14, 0xc, 0x38, 0xcb, 0x11, 0x11, 0x11, 0x11, 0xc0, 0x4, 0xd7, 0xcb, 0x1, 0x1, 0x1, 0x1, 0x1, 0x1, 0x1, 0x2, 0x2, 0x1, 0x1, 0x1, 0x1, 0x1, 0x1, 0x1, 0x0, 0x0, 0x0}},
}
