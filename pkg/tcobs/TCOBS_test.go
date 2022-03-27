package src

import (
	"math/rand"
	"testing"

	"github.com/tj/assert"
)

type testTable []struct {
	dec []byte
	enc []byte
}

var testData = testTable{
	{[]byte{}, []byte{}},

	{[]byte{0}, []byte{0x20}},
	{[]byte{0, 0xAA}, []byte{0x20, 0xAA, 0xA1}},
	{[]byte{0, 0xFF}, []byte{0x20, 0xFF, 0xA1}},

	{[]byte{0, 0}, []byte{0x40}},
	{[]byte{0, 0, 0xAA}, []byte{0x40, 0xAA, 0xA1}},
	{[]byte{0, 0, 0xFF}, []byte{0x40, 0xFF, 0xA1}},

	{[]byte{0, 0, 0}, []byte{0x60}},
	{[]byte{0, 0, 0, 0xAA}, []byte{0x60, 0xAA, 0xA1}},
	{[]byte{0, 0, 0, 0xFF}, []byte{0x60, 0xFF, 0xA1}},

	{[]byte{0, 0, 0, 0}, []byte{0x60, 0x20}},

	{[]byte{0, 0, 0, 0, 0}, []byte{0x60, 0x40}},
	{[]byte{0, 0, 0, 0, 0, 0xFF}, []byte{0x60, 0x40, 0xFF, 0xA1}},
	{[]byte{0, 0, 0, 0, 0, 0xAA}, []byte{0x60, 0x40, 0xAA, 0xA1}},

	{[]byte{0, 0, 0, 0, 0, 0, 0xFF}, []byte{0x60, 0x60, 0xFF, 0xA1}},
	{[]byte{0, 0, 0, 0, 0, 0xAA, 0xFF, 0xFF, 0xAA}, []byte{0x60, 0x40, 0xAA, 0xC1, 0xAA, 0xA1}},

	{[]byte{0xAA, 0xAA}, []byte{0xAA, 0xAA, 0xA2}},
	{[]byte{0xAA, 0xBB}, []byte{0xAA, 0xBB, 0xA2}},
	{[]byte{0xFF}, []byte{0xFF, 0xA1}},
	{[]byte{0xFF, 0xFF}, []byte{0xC0}},

	{[]byte{0xFF}, []byte{0xFF, 0xA1}},
	{[]byte{0xFF, 0xAA}, []byte{0xFF, 0xAA, 0xA2}},
	{[]byte{0xFF, 0xFF}, []byte{0xC0}},
	{[]byte{0xFF, 0xFF, 0xAA}, []byte{0xC0, 0xAA, 0xA1}},
	{[]byte{0xFF, 0xFF, 0xFF}, []byte{0xE0}},
	{[]byte{0xFF, 0xFF, 0xFF, 0xAA}, []byte{0xE0, 0xAA, 0xA1}},
	{[]byte{0xFF, 0xFF, 0xFF, 0xFF}, []byte{0x80}},
	{[]byte{0xFF, 0xFF, 0xFF, 0xFF, 0xFF}, []byte{0x80, 0xFF, 0xA1}},
	{[]byte{0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xAA}, []byte{0x80, 0xFF, 0xAA, 0xA2}},
	{[]byte{0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF}, []byte{0x80, 0xC0}},
	{[]byte{0xFF, 0xFF, 0xFF, 0xFF, 0xAA, 0xFF, 0xFF}, []byte{0x80, 0xAA, 0xC1}},

	// ok ^

	// ok? v

	{[]byte{0xAA, 0x00}, []byte{0xAA, 0x21}},
	{[]byte{0xAA, 0xAA, 0x00}, []byte{0xAA, 0xAA, 0x22}},

	{[]byte{0xAA}, []byte{0xAA, 0xA1}},
	{[]byte{0xAA, 0xAA}, []byte{0xAA, 0xAA, 0xA2}},
	{[]byte{0xAA, 0xAA, 0xAA}, []byte{0xAA, 0x11}},

	{[]byte{0xAA, 0xAA, 0xAA, 0xAA, 0xAA}, []byte{0xAA, 0x19}},

	{[]byte{0xAA, 0xAA, 0xAA, 0xAA, 0xAA, 0xAA}, []byte{0xAA, 0x19, 0xAA, 0xA1}},
	{[]byte{0xAA, 0xAA, 0xAA, 0xAA, 0xAA, 0xAA, 0xAA}, []byte{0xAA, 0x19, 0xAA, 0xAA, 0xA2}},
}

func TestTCOBSEncode(t *testing.T) {
	for _, k := range testData {
		enc := make([]byte, 100)
		n := TCOBSEncodeC(enc, k.dec)
		enc = enc[:n]
		assert.Equal(t, k.enc, enc)
	}
}

func TestTCOBSDecode(t *testing.T) {
	b := make([]byte, 100)
	for _, k := range testData {
		n, e := TCOBSDecode(b, k.enc) // func TCOBSDecode(d, in []byte) (n int, e error) {
		assert.True(t, e == nil)
		assert.True(t, n <= len(b))
		dec := b[len(b)-n:]
		assert.Equal(t, k.dec, dec)
	}
}

func TestTCOBSDecoder(t *testing.T) {
	i := []byte{1, 2, 3, 0, 4, 5, 0, 6}
	before, after, err := TCOBSDecoder(i)
	assert.True(t, err == nil)
	assert.Equal(t, i[:3], before)
	assert.Equal(t, i[4:], after)

	before, after, err = TCOBSDecoder(after)
	assert.True(t, err == nil)
	assert.Equal(t, i[4:6], before)
	assert.Equal(t, i[7:], after)

	before, after, err = TCOBSDecoder(after)
	assert.True(t, err == nil)
	var exp []uint8
	assert.Equal(t, exp, before)
	assert.Equal(t, i[7:], after)
}

func TestEncodeDecode(t *testing.T) {
	max := 32768
	length := rand.Intn(max)
	datBuf := make([]byte, max)
	encBuf := make([]byte, 2*max) // max 1 + (1+1/32)*len) ~= 1.04 * len
	decBuf := make([]byte, 2*max) // max 1 + (1+1/32)*len) ~= 1.04 * len
	for i := 0; i < length; i++ {
		b := uint8(rand.Intn(3)) - 1 // Oxff, 0, 1 2
		datBuf[i] = b
	}
	dat := datBuf[:length]
	n := TCOBSEncodeC(encBuf, dat)
	enc := encBuf[:n]
	n, e := TCOBSDecode(decBuf, enc)
	assert.True(t, e == nil)
	dec := decBuf[len(decBuf)-n:]
	assert.Equal(t, dat, dec)
}
