# *Trice*  Version 1.0 Specification (Draft)

<!-- vscode-markdown-toc -->
* 1. [Preface](#Preface)
* 2. [Compatibility](#Compatibility)
* 3. [Framing](#Framing)
* 4. [*Trice* ID list `til.json`](#TriceIDlisttil.json)
* 5. [*Trice* location information file `li.json`](#Tricelocationinformationfileli.json)
* 6. [TREX (*Trice* extendable) encoding](#TREXTriceextendableencoding)
	* 6.1. [Symbols](#Symbols)
	* 6.2. [Main stream logs](#Mainstreamlogs)
		* 6.2.1. [*Trice* format](#Triceformat)
		* 6.2.2. [COBS encoding](#COBSencoding)
	* 6.3. [Extended *Trices* as future option](#ExtendedTricesasfutureoption)
	* 6.4. [Unknown user data](#Unknownuserdata)
* 7. [Changelog](#Changelog)

<!-- vscode-markdown-toc-config
	numbering=true
	autoSave=true
	/vscode-markdown-toc-config -->
<!-- /vscode-markdown-toc -->

##  1. <a name='Preface'></a>Preface

The with name "COBS" branded [current (2022-03) *Trice* encoding](./TriceMessagesEncoding.md) is not optimal concerning the generated data amount:
* See discussion [#253 Save trice COBS encoded data on target and view it later on PC](https://github.com/rokath/trice/discussions/253).
* The location information is transmitted as 16 bit file ID plus 16 bit line number. It is possible to generate during `trice update` an additional file `li.json` containing the location information for each *Trice* ID avoiding the additional 4 bytes this way. But this could cause assignment issues, when the same *Trice* ID is used at different locations (see [https://github.com/rokath/trice/discussions/264](https://github.com/rokath/trice/discussions/264)). But it is possible to drop the option `trice u -sharedIDs`.
* The 32-bit COBS package descriptor is overkill for allowing user data.
* The additional padding bytes to achieve 32 bit sizes are not needed. The user could add them by himself if really needed.
* The 4 timestamp bytes in front of each *Trice* demand the COBS package descriptor. The timestamp should go inside the *Trice* message and be optionally smaller.

##  2. <a name='Compatibility'></a>Compatibility

* The *Trice* v0.48.0 user syntax will remain mainly unchanged. The letter case of the ID codes the target timestamp size. (see below)
* The as "COBS" branded legacy [v0.48.0 *Trice* encoding](.TriceMessageEncoding.md) will stay unchanged as an option for compatibility. But it will not be the default encoding anymore. To use newer **trice** tool versions with legacy projects the CLI switch `-encoding LegacyCOBS` needs to be used.
* The option `-sharedIDs` will be further available but depreciated to avoid location assignment issues.
* Legacy projects which used the option `-sharedIDs` will still work even with a `li.json` file. A several times used ID will get an assignment of one of the locations.
* The issue [#242 Add target context option](https://github.com/rokath/trice/issues/242) could get the label "wontfix". When a task ID is needed, it could be also a data value in such cases.
* The same user source files usable with the legacy *Trice* COBS encoding and the proposed additional [TREX](#TREXTriceextendableencoding) encoding. The `#define TRICE_FILE Id(n)` is ignored when TREX is used.

##  3. <a name='Framing'></a>Framing

Framing is will be done by with [TCOBS](./TCOBSSpecification.md) for data reduction. For robustness each *Trice* gets its own TCOBS package. User data are in separate TCOBS packages encoded. When *Trices* are accumulated in a double half buffer, their separation in TCOBS packages is possible until the first extended *Trice*. Because of the generally unknown extended *Trice* length from this point all following Trices in this half buffer need to go in one TCOBS package (including optional padding bytes) what is ok. The only disadvantage with this is, that in case of a data disruption several *Trice* messages get lost. 

##  4. <a name='TriceIDlisttil.json'></a>*Trice* ID list `til.json`

This file integrates all firmware variants and versions as v0.48.0 does. For the implementation of the optional *Trice* extensions (see below), a `til.json` format extension is needed because several files are unhandy. Both `til.json` formats will be accepted in the future.

##  5. <a name='Tricelocationinformationfileli.json'></a>*Trice* location information file `li.json`

With [TREX](#TREXTriceextendableencoding) encoding the location information needs no transmission anymore but goes not into the `til.json` file. In the field the location information is normally useless and easy outdated. The software developer is the one, mostly interested in the location information. So, if the `li.json` is generated and therefore available, the **trice** tool automatically displays file name and line number. When the firmware left the developer table, only the file `til.json` is of interest. The **trice** tool will silently not display location information, if the `li.json` file is not found. For in-field logging, the usage of option `-showID string` could be used. This allows later an easy location of the relevant source code. Also the planned `-binaryLogfile` option is possible. See [issue #267 Add `-binaryLogfile` option](https://github.com/rokath/trice/issues/267). It allows a replay of the logs and the developer can provide the right version of the `li.json` file.

##  6. <a name='TREXTriceextendableencoding'></a>TREX (*Trice* extendable) encoding

###  6.1. <a name='Symbols'></a>Symbols

* `i` = ID bit
* `I` = `iiiiiiii`
* `n` = number bit
* `s` = selector bit
* `N` = `snnnnnnnn`
* `c` = cycle counter bit
* `C` = s==0 ? `cccccccc` : `nnnnnnnn`
* `t` = timestamp bit
* `T` = `tttttttt`
* `d` = data bit
* `D` = `dddddddd`

###  6.2. <a name='Mainstreamlogs'></a>Main stream logs

All main stream logs share the same 14 bit ID space allowing 1-16383 IDs.

* `11iiiiii I N C  T T T T ...` 14 bit ID, *Trice* format with 32-bit timestamp: `TRICE( ID(n), "...", ...), ...`
* `10iiiiii I N C  T T ...`     14 bit ID, *Trice* format with 16-bit timestamp: `TRICE( Id(n), "...", ...), ...`
* `01iiiiii I N C  ...`         14 bit ID, *Trice* format without     timestamp: `TRICE( id(n), "...", ...), ...`
* The update switch `-timeStamp 32` defaults new ID´s to `ID`.
* The update switch `-timeStamp 16` defaults new ID´s to `Id`.
* The update switch `-timeStamp 0`  defaults new ID´s to `id`.
* The update switch `-timeStamp to32` converts all `id` and `Id` to `ID`.
* The update switch `-timeStamp to16` converts all `id` and `ID` to `Id`.
* The update switch `-timeStamp to0`  converts all `ID` and `Id` to `id`.
* The log switch `-ttsf` is the same as `-ttsf32`.
* There is a new log switch `ttsf16` for the 16 bit timestamps. 

####  6.2.1. <a name='Triceformat'></a>*Trice* format

* Optional data bytes start after optional timestamp.
* N is not u32 count anymore, it is data byte count (without header, without timestamp).
* N > 127 (s==1) tells `N C` is replaced by `1nnnnnnn nnnnnnnn`, allowing 32767 bytes.
  * C is incremented with each *Trice* but not transmitted when:
    * N > 127
    * extended *Trice* without C

####  6.2.2. <a name='COBSencoding'></a>COBS encoding

* Inside double buffer each trice starts at a u32 boundary.
* There are 1-3 padding bytes possible after each *Trice*.
* The COBS encoding drops the padding bytes using N and encodes each *Trice* separately. This minizmizes data loss in case of disruptions for example caused by reset.

###  6.3. <a name='ExtendedTricesasfutureoption'></a>Extended *Trices* as future option

If for special cases, the main stream encoding is not sufficient, the user can add its own encoding.

* `00...` sub-options `TRICEX0`, `TRICEX1`, `TRICEX2`, `TRICEX3`
  * `-ex0 pos -ex1 pos -ex2 pos -ex3 pos`  Select position in extendable table for TRICEXn, 4 codings selectable in one shot.
  * The table is creatable and extendable on demand.
  * For each line an appropriate target and host code needs to be done.
  * Then the target configuration must match the CLI switches.
  * Table example:
    |Position | Encoding                          | Remarks                                       |
    | -   | -                                     | -                                             |
    | pos | `00nniiii I D D`                      | 12 bit ID, no timestamp, 1x 16 bit data       |
    | pos | `00nniiii I D D`                      | 12 bit ID, no timestamp, 2x 8 bit data        |
    | pos | `00nniiii I D D D D`                  | 12 bit ID, no timestamp, 2x 16 bit data       |
    | pos | `00nniiii I T T  D D`                 | 12 bit ID, 16 bit timestamp,1x16 bit data     |
    | pos | `00nniiii I T T  D D D D`             | 12 bit ID, 16 bit timestamp, 1x32 bit data    |
    | pos | `00nniidd dddddddd`                   | 2 bit ID & 1x 10 bit data                     |
    |   6 | `00nndddd dddddddd`                   | no ID, 12 bit data as a 5 and a 7 bit value   |
    | pos | `00nndddd`                            | no ID, 4x 1 bit data                          |
    | pos | `00nniiii I`                          | 12 bit ID, no data                            |
    | pos | `00nniiii D D`                        | 4 bit ID, 1x 16 bit data                      |
    | pos | `00nniiii I D D`                      | 12 bit ID, 2x 8 bit data                      |
    | pos | `00nniiii I T T`                      | 12 bit ID, 16 bit timestamp, no data          |
    | pos | `00nniiii D D T`                      | ...                                           |
    |  13 | `00nniiii C T T  D D D D`             | 8 bit cyle counter, 16 bit timestamp, a float |
    | pos | `00nniiii C D D  T T T T`             | ...                                           |
    | pos | `00nniiii I D D  T`                   | ...                                           |
    | pos | `00nniiii tttttttt ttttdddd dddddddd` | 4 bit ID, 12 bit timestamp, 12 bit data       |
    | ... | `00nn...`                             | ...                                           |
  * Examples:
    * `-ex0 13` means TRICEX0 = `0000iiii C T T  D D D D`.
      * Usage: `TRICEX0( "result %f\n", aFloat(v));`
    * `-ex2 6`  means TRICEX2 = `0010dddd dddddddd`.
      * Usage: `TRICEX2( "point %x,%d\n", a, b);`
  * *Trice* extensions without cycle counter are counted as well.
  * Each TRICEXn has its own ID space.

###  6.4. <a name='Unknownuserdata'></a>Unknown user data

* Unknown user data are possible as part of the *Trice* extensions.
  * Without the `-ex0` switch, `0000...` packages are ignored as unknown user data.
  * Without the `-ex1` switch, `0001...` packages are ignored as unknwno user data.
  * Without the `-ex2` switch, `0010...` packages are ignored as unknown user data.
  * Without the `-ex3` switch, `0011...` packages are ignored as unknown user data.
* So, if *Trice* extensions not used, all `00...` packages are ignored as unknown user data.
* Unknown user data have an unknown length. Therefore they cannot share a COBS packet with *Trices*.
* Unknown user data packets do not affect the cycle counter. The can have their own cycle counter.
##  7. <a name='Changelog'></a>Changelog

| Date        | Version | Comment |
| -           | -       | - |
| 2022-MAR-15 |  0.0.0  | Initial Draft |
| 2022-MAR-15 |  0.1.0  | Minor corrections applied. |
| 2022-MAR-15 |  0.2.0  | Sigil byte encoding clarified. |
| 2022-MAR-15 |  0.3.0  | Forward versus backward COBS encoding discussion inserted. |
| 2022-MAR-15 |  0.4.0  | Forward versus backward COBS encoding reworked. Disruption detection added. |
| 2022-MAR-15 |  0.5.0  | Minor corrections |
| 2022-MAR-16 |  0.6.0  | TCOBS prime number comment added, simplified |
| 2022-MAR-17 |  0.7.0  | TCOBS move into a separate [TCOBS Specification](./TCOBSSpecification.md), Framing more detailed. |
 
- [*Trice*  Version 1.0 Specification (Draft)](#trice--version-10-specification-draft)
  - [1. <a name='Preface'></a>Preface](#1-preface)
  - [2. <a name='Compatibility'></a>Compatibility](#2-compatibility)
  - [3. <a name='Framing'></a>Framing](#3-framing)
  - [4. <a name='TriceIDlisttil.json'></a>*Trice* ID list `til.json`](#4-trice-id-list-tiljson)
  - [5. <a name='Tricelocationinformationfileli.json'></a>*Trice* location information file `li.json`](#5-trice-location-information-file-lijson)
  - [6. <a name='TREXTriceextendableencoding'></a>TREX (*Trice* extendable) encoding](#6-trex-trice-extendable-encoding)
    - [6.1. <a name='Symbols'></a>Symbols](#61-symbols)
    - [6.2. <a name='Mainstreamlogs'></a>Main stream logs](#62-main-stream-logs)
      - [6.2.1. <a name='Triceformat'></a>*Trice* format](#621-trice-format)
      - [6.2.2. <a name='COBSencoding'></a>COBS encoding](#622-cobs-encoding)
    - [6.3. <a name='ExtendedTricesasfutureoption'></a>Extended *Trices* as future option](#63-extended-trices-as-future-option)
    - [6.4. <a name='Unknownuserdata'></a>Unknown user data](#64-unknown-user-data)
  - [7. <a name='Changelog'></a>Changelog](#7-changelog)