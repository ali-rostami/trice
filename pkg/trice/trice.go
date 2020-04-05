// Copyright 2020 basti@blackoutcloud.de
//                Thomas.Hoehenleitner [at] seerose.net
// Use of this source code is governed by a license that can be found in the LICENSE file.

package trice

import (
	"bufio"
	"crypto/sha1"
	"errors"
	"fmt"
	"os"
	"runtime"
	"strings"
	"sync"

	"github.com/rokath/trice/pkg/disp"
	"github.com/rokath/trice/pkg/id"
	"github.com/rokath/trice/pkg/lgf"
	"github.com/rokath/trice/pkg/receiver"
	"golang.org/x/crypto/xtea"
)

// local config values
var (
	// Password is the key one needs to derypt trice logs if enncrypted
	Password string

	// ShowPassword, if set, allows to see the encryption passphrase
	ShowPassword bool
)

// ScLog is the subcommand log and connects to COM port and displays traces
func ScLog() error {
	lgf.Enable()
	defer lgf.Disable()

	return DoReceive()
}

// scReceive is the subcommand remoteDisplay and acts as client connecting to the displayServer
func ScReceive(sv bool) error {
	var wg sync.WaitGroup

	if true == sv {
		disp.StartServer()
	}
	wg.Add(1)

	err := disp.Connect()
	if nil != err {
		return err
	}
	var result int64

	/*err = pRPC.Call("Server.Adder", [2]int64{10, 20}, &result)
	if err != nil {
		fmt.Println(err)
	} else {
		fmt.Println("Server.Adder(10,20) =", result)
	}*/
	s := []string{"att:\n\n\nnew connection....\n\n\n"}
	err = disp.PtrRpc.Call("Server.Visualize", s, &result)
	if err != nil {
		fmt.Println(err)
	} else {
		fmt.Println("len is", result)
	}
	keyboardInput()
	DoReceive() // does not return
	wg.Wait()
	fmt.Println("...done")
	return nil
}

// DoReceive connects to COM port and displays traces
func DoReceive() error {
	if "none" != id.FnJSON {
		// setup ip list
		err := id.List.Read(id.FnJSON)
		if nil != err {
			fmt.Println("ID list " + id.FnJSON + " not found, exit")
			return nil
		}
	}

	var err error
	receiver.Cipher, receiver.Crypto, err = createCipher()
	if nil != err {
		return err
	}

	/* TODO: Introduce new command line option for choosing between
	   1) Serial receiver(port name, baudrate, parity bit etc. )
	   2) TCP receiver (IP, port, Protocol (i.e JSON,XML))
	   3) HTTP/Websocket receiver (may be the simplest form in Golang)
	*/

	fmt.Println("id list file", id.FnJSON, "with", len(id.List), "items")
	return receiver.DoSerial()
}

// keyboardInput expects user input from terminal
func keyboardInput() { // https://tutorialedge.net/golang/reading-console-input-golang/
	reader := bufio.NewReader(os.Stdin)
	fmt.Println("Simple Shell")
	fmt.Println("------------")

	go func() {
		for {
			fmt.Print("-> ")
			text, _ := reader.ReadString('\n')
			// convert CRLF to LF
			e := "\n"
			if runtime.GOOS == "windows" {
				e = "\r\n"
			}
			text = strings.Replace(text, e, "", -1) // Linux "\n" !

			switch text {
			case "q", "quit":
				os.Exit(0)
			case "h", "help":
				fmt.Println("h|help    - this text")
				fmt.Println("q|quit    - end program")
			default:
				fmt.Printf("Unknown command '%s' - use 'help'\n", text)
			}
		}
	}() // https://stackoverflow.com/questions/16008604/why-add-after-closure-body-in-golang
}

// createCipher prepares decryption, with password "none" the encryption flag is set false, otherwise true
func createCipher() (*xtea.Cipher, bool, error) {
	h := sha1.New() // https://gobyexample.com/sha1-hashes
	h.Write([]byte(Password))
	key := h.Sum(nil)
	key = key[:16] // only first 16 bytes needed as key

	c, err := xtea.NewCipher(key)
	if err != nil {
		return nil, false, errors.New("NewCipher returned error")
	}
	var e bool
	if "none" != Password {
		e = true
		if true == ShowPassword {
			fmt.Printf("% 20x is XTEA encryption key\n", key)
		}
	} else if true == ShowPassword {
		fmt.Printf("no encryption\n")
	}
	return c, e, nil
}
