
; TODO: 
;   NEED TO READ FAT!
;        - how large is it anyway?
;           9 x 512 = 0x1200
;   Understand fat - how files are laid out
;   Read full file from fat
;   Jum to it
;   Have that file print a string
;

; Stage 0 bootloader
; Wiktor Lukasik (wiktor@lukasik.org)
;
; ==============================================================================
; MEMORY MAP
; ------------------------------------------------------------------------------
; 0x7E00    - BOOT SECTOR END
; 0x7C00    - BOOT SECTOR BEGIN
; 0x7C00    - FAT12 ROOT TABLE END
; 0x6000    - FAT12 ROOT TABLE BEGIN
; 0x5000    - REAL-MODE STACK TOP
; 0x4000    - REAL-MODE STACK BOTTOM
; 0x1900    - END FAT
; 0x0700    - START FAT
; 0x0700    - FLOPPY SECTOR END
; 0x0500    - FLOPPY SECTOR BEGIN
;
; ==============================================================================
; HELPFUL LINKS:
; ------------------------------------------------------------------------------
; int 13h interrupt list - http://en.wikipedia.org/wiki/INT_13H
;
; ==============================================================================
; DESCRIPTION
; ------------------------------------------------------------------------------
; Boot sector is 512 bytes, which is really tiny and not enought for much more than
; code to read a larger binary from floppy. So, we need a multi stage boot loading 
; process. Let's call this stage stage0.
;
; stage0's purpose is to read the list of files from a fat12 partition on a floppy
; and find binary file belonging to stage1. The very last thing in stage0 is to 
; load stage1 into memory and jump into it.
;
; stage1 will attempt to perform some setup so we can boot into kernel proper.
;
; WORKFLOW for stage0:
; 1. Set stack: top=0x5000, size=0x1000 (0x4000-0x5000)
; 1. Load fat12 root table at 0x6000-0x7C00 (size = bytes per root entry * root entries = 32 * 224 = 7168 = 0x1c00)
; 2. Iterate through list of files and find stage1
; 3. Load stage1 (TODO: where?)
; 4. Jump into stage1

org 0x7c00
bits	16

bytesPerRootEntry   equ 0x20
rootDirSegmentStart equ 0x50
stackSegmentStart   equ 0x500
stackSize           equ 0x1d00
stage1SegmentStart  equ 0x210
stage1sector        equ 0x0

; First three bytes is a jump instruction over
; BPB to the actuall code
start:	jmp loader

; Now we need to fill space up to 11th byte
TIMES 0Bh-$+start DB 0
; Optionally, this is where OEM name sits...
;bsOEM			DB "dasBOT  "

; Define some info for the diskette and fat12
bpbBytesPerSector: 	    DW 512
bpbSectorsPerCluster: 	DB 1
bpbReservedSectors: 	DW 1
bpbNumberOfFATs: 	    DB 2
bpbRootEntries: 	    DW 224
bpbTotalSectors: 	    DW 2880
bpbMedia: 	            DB 0xF0
bpbSectorsPerFAT: 	    DW 9
bpbSectorsPerTrack: 	DW 18
bpbHeadsPerCylinder: 	DW 2
bpbHiddenSectors: 	    DD 0
bpbTotalSectorsBig:     DD 0
bsDriveNumber: 	        DB 0
bsUnused: 	            DB 0
bsExtBootSignature: 	DB 0x29
bsSerialNumber:         DD 0x10203040
bsVolumeLabel: 	        DB "VOSBOOTDISK"
bsFileSystem: 	        DB "FAT12   "
fileName		        DB "...........", 0
stage1Name              DB "STAGE1  BIN", 0
stage1Found             DB "F", 0
stage1NotFound          DB "NF", 0
booting                 DB "B", 0
section .text

;%include 'stage0/util.s'

; Compares two file names, one from root table, other one from fileName
filename_compare:
    push    ebp
    mov     ebp, esp

;    push    PRINTNL
;    mov     si, fileName
;    call    printn
;    add     sp, 2

    ; Compare strings in DS:SI and ES:DI
    mov     cx, 11          ; Compare 11 chars - needed by repe
    xor     ax, ax          
    mov     ds, ax          ; no need for values in segments ds and es
    mov     es, ax
    mov     si, fileName    ; set source and destination for string comparison
    mov     di, stage1Name
    repe    cmpsb           ; compare strings

    jnz      .not_equal
    
 ;   push    PRINTNL         ; found file...
 ;   mov     si, stage1Found
 ;   call    printn
 ;   add     sp, 2
   
    mov     word[stage1sector], 4321
    jmp     .return

.not_equal:                 ; did not find file
    ;push    PRINTNL
    ;mov     si, stage1NotFound
    ;call    printn
    ;add     sp, 2

.return:
    pop     ebp
    ret

root_loading:
; GET SIZE OF ROOT DIRECTORY (in # of sectors, store in CX)
; size of root = (0x20 (32) bytes per root entry * number of root entries) / 512 bytes in a sector 
; size of root = (32 * 224) / 512 = 14 sectors
.size_root:
    xor     ax, ax  
    xor     cx, cx
    xor     dx, dx
    mov     ax, bytesPerRootEntry
    mul     word [bpbRootEntries]
    div     word [bpbBytesPerSector]
    xchg    ax, cx  ; CX should be 14, as per FAT12 docs   


; GET STARTING POINT OF ROOT DIRECTORY (store in AX)
; start of root = number of fats * sectors per fat + number of reserved sectors
; start of root = 2 * 9 + 1 = 19th sector (0x2600)
.start_root:
    xor     ax, ax
    mov     al, byte [bpbNumberOfFATs]
    mul     word[bpbSectorsPerFAT]
    add     ax, word [bpbReservedSectors]

; LOAD SECTOR OF ROOT DIRECTORY (at ES:BX=0x6000)
; End will be at 0x7C00, which is where code starts
.load_root:
    mov     bx, rootDirSegmentStart
    mov     es, bx
    xor     bx, bx   
.load_root_loop:
    push    ax
    push    bx
    push    cx
    call    lbatochs

    mov     ah, 0x02 ; read inst
    mov     al, 0x01 ; sectors to read
    mov     ch, [absoluteTrack]; cylinder
    mov     cl, [absoluteSector]; sector
    mov     dh, [absoluteHead]; head
    mov     dl, 0; drive
    int     0x13
    pop     cx
    pop     bx
    pop     ax
    add     bx, WORD [bpbBytesPerSector] 
    inc     ax
    loop    .load_root_loop
    ret 

lbatochs:
    xor     dx, dx                              ; prepare dx:ax for operation
    div     WORD [bpbSectorsPerTrack]           ; calculate
    inc     dl                                  ; adjust for sector 0
    mov     BYTE [absoluteSector], dl
    xor     dx, dx                              ; prepare dx:ax for operation
    div     WORD [bpbHeadsPerCylinder]          ; calculate
    mov     BYTE [absoluteHead], dl
    mov     BYTE [absoluteTrack], al
    ret

; ENTRY POINT
loader:

; SET-UP STACK
.stack_setup:
    cli                 ; disable interrupts
	mov	    ax, stackSegmentStart   ; set stack segment to 0x5000 (16 * 0x500)
	mov	    ss, ax
	mov	    ax, stackSize  ; stack size = 0x1000
	mov	    sp, ax
	mov	    bp, sp      ; not sure we need to set-up bp, but do it anyway

	xor	    ax, ax      ; zero out data and extra segments
	mov	    ds, ax
	mov	    es, ax
    mov     fs, ax
    mov     gs, ax
    sti                 ; set interrupts

    ; Find second stage
    ; Load fat root
    call    root_loading
    call    LOAD_FAT

; ITERATE OVER FILES IN FAT12
    mov     cx, 16      ; Assume there is up to 16 files. TODO: fix this
.printNextFileName:
    ; Copy file name
    push    ds          ; save ds and es as we will use them to copy string
    push    es
    push    cx

    ; si = starting address of root directory entry
    mov     ax, cx      ; a = counter
    dec     ax          ; a--
    mov     bx, 0x20    ; b = 32
    mul     bx          ; a = a * b
    mov     WORD[curFile], ax ; TODO: THIS HAS NO EFFECT, BUT WORKS ELSEWHERE!!!!
    mov     si, ax      ; set start point for string copy

    ; copy cluster 
    mov     bx, ax
    add     bx, 0x1A
    mov     WORD[curCluster], bx

    xor     cx, cx
    mov     cx, 11      ; each file name is up to 11 chars long
    mov     ax, rootDirSegmentStart 
    mov     ds, ax      ; set data segment to point to root entry segment - this is source segment for string copy


    xor     ax, ax
    mov     es, ax      ; es is destination segment for string copy - zero it out
    mov     di, fileName ; instead use di (destination index) alone

    ;push    di          ; push & pop di so we can keep track of where current file starts
    rep movsb           ; copy string (cx characters from ds:si to es:di)  
    ;pop     di

    pop     cx          ; most recently cx was used to count number of chars in filename - now restore cx from stack that indicates number of files left
    pop     es          ; restore ds and es
    pop     ds

    ; Print file name, which is stored ad location of fileName
    cmp     byte[fileName], 0x00 ; skip file if name starts with 0x00 - from fat 12 specs
    je      .skip
    cmp     byte[fileName], 0xe5 ; skip file if name starts with 0xe5 - from fat 12 specs
    je      .skip
    ;mov     si, fileName    ; use printn subroutine to print file name
    ;push    PRINTNL         ; ...and put new line after each file name 
    ;call    printn  
    ;add     sp, 2 


; CHECK IF CORRECT FILE
    pusha
    call    filename_compare
    popa
    .skip:
    loop    .printNextFileName

    cmp word[stage1sector], 4321
    jnz jump_to_loaded_code

print_booting: 
;    mov si, booting
;    push PRINTNL
;    call printn
;    add sp, 2
      
    mov ax, word[stage1sector]
    mov ax, 34
    call lbatochs 
    xor bx, bx
    mov es, bx
    mov bx, 0x7e00

    mov ah, 0x02
    mov al, 0x1
    mov ch, [absoluteTrack]
    mov cl, [absoluteSector]
    mov dh, [absoluteHead]
    mov dl, 0
    int 0x13
    mov ax, 789
; SET-UP BUFFER FOR FLOPPY DATA
;.read_sector_dest_setup:
;	mov	    ax, stage1SegmentStart  ; Floppy data will be read into  es:bx
;	mov	    es, ax
;	xor	    bx, bx

; READ SECTOR FROM FLOPPY
;.read_sector:
;	mov	ah, 0x02	    ; Read sector, ah=2,al=num sectors to read,ch=cylinder,cl=sector,dh=head,dl=drive,es:bx=destination buffer
;                        ; results: CF set on error,ah=return code,al=sector read count
;	mov	al, 0x01
;	mov	ch, 0
;	mov	cl, 2
;	mov	dh, 0
;	mov	dl, 0
;	int	0x13
;    jc .reset           ; If error occurred, reset floppy and try reading again

jump_to_loaded_code:    ; Jump to more code
   ; push    WORD 0x0050
   ; push    WORD 0x0000
   ; retf
   jmp 0x7e00
; RESET FLOPPY DRIVE    
reset_floppy:			        ; Reset floppy before reading, ag=dl=0
	mov	    ah, 0
    mov	    dl, 0
	int	    0x13
	jc	    reset_floppy        ; CF is set on error
    ret


; eh, this is broken
LOAD_FAT:
    pusha
    call    reset_floppy
    popa

    pusha

    xor     ax, ax
    mov     ax, 2
    call    lbatochs

    xor     bx, bx
    mov     es, bx
    mov     bx, 0x2100
    mov     ah, 0x02 ; read inst
    mov     al, 0x09 ; sectors to read
    mov     ch, [absoluteTrack]; cylinder
    mov     cl, 2; [absoluteSector]; sector
    mov     dh, [absoluteHead]; head
    mov     dl, 0; drive
    int     0x13
 
    popa
    ret    

stop:
	cli
	hlt

curFile        dw 0x00
curCluster     db 0x00
absoluteSector db 0x00
absoluteHead   db 0x00
absoluteTrack  db 0x00
times	510	- ($-$$) db 0

dw 0xAA55
