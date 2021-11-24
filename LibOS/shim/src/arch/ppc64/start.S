/* This is the canonical entry point, usually the first thing in the text
   segment.
 */
	.text
	.globl shim_start
	.type shim_start,@function

shim_start:
	.cfi_startproc
0:	addis	%r2, %r12, (.TOC. - 0b)@ha
	addi	%r2, %r2, (.TOC. - 0b)@l

	mflr	%r0
	std	%r0, 16(%r1)
	std	%r31, -8(%r1)

	mr	%r6, %r3	// pointer to cookies
	ld	%r3, 0(%r6)	// argc
	addi	%r4, %r6, 8	// argv

	slwi	%r5, %r3, 3	// argc * 8
	add	%r5, %r5, %r4	// envp = argv + argc * 8
	addi	%r5, %r5, 8

	stdu	%r1, -48(%r1)	// keep 16-byte stack alignment

	mr	%r31, %r1	// save our stack pointer

	bl	shim_init
	nop

	addi	%r1, %r31, 48	// restore original stack
	ld	%r31, -8(%r1)
	ld	%r0, 16(%r1)
	mtlr	%r0
	blr

	.cfi_endproc
