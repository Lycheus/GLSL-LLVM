	.file	"vertexbyhand.bc"
	.text
	.globl	main
	.align	2
	.type	main,@function
main:                                   # @main
.Leh_func_begin0:
# BB#0:                                 # %entry
	ST r0, 4(1)
	NOP 9
	ST r1, -16(1)
	NOP 9
	ADDI r1, r1, -16
	NOP 9
.Ltmp0:
	ST r30, 8(1)
	NOP 9
	BL OpenGLES.Start
	NOP 9
	BL OpenGLES.ConstructVec4.vec4
	NOP 9
	ADDI r3, r0, 0
	NOP 9
.LBB0_1:                                # %L1.C
                                        # =>This Inner Loop Header: Depth=1
	BNE 3, 3, .LBB0_1, <1>
	NOP 9
# BB#2:                                 # %return
	LD r30, 8(1)
	NOP 9
	ADDI r1, r1, 16
	NOP 9
	LD r0, 4(1)
	NOP 9
	RET 
	NOP 9
.Ltmp1:
	.size	main, .Ltmp1-main
.Leh_func_end0:

	.type	wave_Vertex,@object     # @wave_Vertex
	.section	.bss,"aw",@nobits
	.align	4
wave_Vertex:
	.space	16
	.size	wave_Vertex, 16

	.type	time,@object            # @time
	.align	2
time:
	.long	0                       # float 0.000000e+00
	.size	time, 4

	.type	ami,@object             # @ami
	.align	4
ami:
	.space	28
	.size	ami, 28

	.type	wl,@object              # @wl
	.align	4
wl:
	.space	28
	.size	wl, 28

	.type	qq,@object              # @qq
	.align	4
qq:
	.space	28
	.size	qq, 28

	.type	dirx,@object            # @dirx
	.align	4
dirx:
	.space	28
	.size	dirx, 28

	.type	diry,@object            # @diry
	.align	4
diry:
	.space	28
	.size	diry, 28

	.type	s,@object               # @s
	.align	2
s:
	.long	0                       # float 0.000000e+00
	.size	s, 4

	.type	facing,@object          # @facing
	.align	2
facing:
	.long	0                       # float 0.000000e+00
	.size	facing, 4

	.type	lightPosition,@object   # @lightPosition
	.align	4
lightPosition:
	.space	16
	.size	lightPosition, 16

	.type	lightDir,@object        # @lightDir
	.align	4
lightDir:
	.space	16
	.size	lightDir, 16

	.type	ecPosition,@object      # @ecPosition
	.align	4
ecPosition:
	.space	16
	.size	ecPosition, 16

	.type	Intensity,@object       # @Intensity
	.align	2
Intensity:
	.long	0                       # float 0.000000e+00
	.size	Intensity, 4

	.type	viewVec,@object         # @viewVec
	.align	4
viewVec:
	.space	16
	.size	viewVec, 16

	.type	normal,@object          # @normal
	.align	4
normal:
	.space	16
	.size	normal, 16

	.section	.eh_frame,"aw",@progbits
.LEH_frame0:
.Lsection_eh_frame0:
.Leh_frame_common0:
.Lset0 = .Leh_frame_common_end0-.Leh_frame_common_begin0 # Length of Common Information Entry
	.long	.Lset0
.Leh_frame_common_begin0:
	.long	0                       # CIE Identifier Tag
	.byte	1                       # DW_CIE_VERSION
	.byte	0                       # CIE Augmentation
	.uleb128	1               # CIE Code Alignment Factor
	.sleb128	-4              # CIE Data Alignment Factor
	.byte	65                      # CIE Return Address Column
	.byte	12                      # DW_CFA_def_cfa
	.uleb128	1               # Register
	.uleb128	0               # Offset
	.align	2
.Leh_frame_common_end0:
.Lmain.eh:
.Lset1 = .Leh_frame_end0-.Leh_frame_begin0 # Length of Frame Information Entry
	.long	.Lset1
.Leh_frame_begin0:
.Lset2 = .Leh_frame_begin0-.Leh_frame_common0 # FDE CIE offset
	.long	.Lset2
	.long	.Leh_func_begin0        # FDE initial location
.Lset3 = .Leh_func_end0-.Leh_func_begin0 # FDE address range
	.long	.Lset3
	.uleb128	0               # Augmentation size
	.byte	4                       # DW_CFA_advance_loc4
.Lset4 = .Ltmp0-.Leh_func_begin0
	.long	.Lset4
	.byte	14                      # DW_CFA_def_cfa_offset
	.uleb128	16              # Offset
	.byte	17                      # DW_CFA_offset_extended_sf
	.uleb128	65              # Reg
	.sleb128	-1              # Offset
	.byte	158                     # DW_CFA_offset + Reg (30)
	.uleb128	2               # Offset
	.align	2
.Leh_frame_end0:

	.weak	OpenGLES.Start
	.weak	OpenGLES.ConstructVec4.vec4

