; Arguments
; RDI: uint8_t *pixels
; RSI: int width
; RDX: int height
; XMM0: double escapeRadius
; XMM1: double cReal
; XMM2: double cImag
; XMM3: double centerReal
; XMM4: double centerImag
; XMM5: double zoom


	section .text
	global generateJuliaSet

generateJuliaSet:
	; Prologue
	push	rbp
	mov		rbp, rsp
	push	rbx
	push	r12

	; r8 = row = height-1
	mov		r8, rdx
	sub		r8, 1
rowLoop:
	; r9 = col = width - 1
	mov		r9, rsi
	sub		r9, 1
colLoop:
	; xmm6 = zReal
	cvtsi2sd	xmm6, r9	; zReal = col
	subsd		xmm6, xmm3	; zReal = col - centerReal
	mulsd		xmm6, xmm0	; zReal = (col - centerReal) * escapeRadius
	addsd		xmm6, xmm6 	; zReal = (col - centerReal) * escapeRadius * 2
	cvtsi2sd	xmm8, rsi	; xmm8 = width
	mulsd		xmm8, xmm5	; xmm8 = (width * zoom)
	divsd		xmm6, xmm8	; zReal = (col - centerReal) * escapeRadius * 2 / (width * zoom)

	; xmm7 = zImag
	cvtsi2sd	xmm7, r8	; zImag = row
	subsd		xmm7, xmm4	; zImag = row - centerImag
	mulsd		xmm7, xmm0 	; zImag = (row - centerImag) * escapeRadius
	addsd		xmm7, xmm7	; zImag = (row - centerImag) * escapeRadius * 2
	cvtsi2sd	xmm8, rdx	; xmm8 = height
	mulsd		xmm8, xmm5 	; xmm8 = (height * zoom)
	divsd		xmm7, xmm8 	; (row - centerImag) * escapeRadius * 2 / (height * zoom)

	; rax = iteration = 0
	mov		rax, 0

pixelWhileLoop:
	;
	;	Check loop conditions
	;

	; break if iteration == maxIteration == 128 (second condition)
	cmp		rax, 128
	je		pixelWhileLoopEnd

	; xmm8 = tmp1 = zReal * zReal + zImag * zImag
	movsd	xmm8, xmm6		; tmp1 = zReal
	mulsd	xmm8, xmm8		; tmp1 = zReal * zReal
	movsd	xmm9, xmm7		; xmm9 = zImag
	mulsd	xmm9, xmm9		; xmm9 = zImag * zImag
	addsd	xmm8, xmm9		; tmp1 = zReal * zReal + zImag * zImag

	; xmm9 = tmp2 = escapeRadius * escapeRadius
	movsd	xmm9, xmm0		; tmp2 = escapeRadius
	mulsd	xmm9, xmm9		; tmp2 = escapeRadius * escapeRadius

	; break if (zReal * zReal + zImag * zImag) >= (escapeRadius * escapeRadius) (fisrt condition)
	comisd	xmm8, xmm9
	jae		pixelWhileLoopEnd

	;
	; Loop body
	;

	; xmm8 = tmpReal = zReal * zReal - zImag * zImag
	movsd	xmm8, xmm6		; xmm8 = zReal
	mulsd	xmm8, xmm8		; xmm8 = zReal * zReal
	movsd	xmm9, xmm7		; xmm9 = zImag
	mulsd	xmm9, xmm9		; xmm9 = zImag * zImag
	subsd	xmm8, xmm9		; xmm8 = tmpReal = zReal * zReal - zImag * zImag

	; xmm7 = zImag = 2 * zReal * zImag + cImag
	addsd	xmm7, xmm7		; xmm7 = 2 * zImag
	mulsd	xmm7, xmm6		; xmm7 = 2 * zReal * zImag
	addsd	xmm7, xmm2		; xmm7 = zImag = 2 * zReal * zImag + cImag

	; xmm6 = zReal = tmpReal + cReal
	movsd	xmm6, xmm8
	addsd	xmm6, xmm1

	; iteration++
	inc		rax

	jmp		pixelWhileLoop	; closeLoop
pixelWhileLoopEnd:

	; rcx = uint8_t r = ((maxIteration - iteration) * 255) / maxIteration;
	mov		rcx, 128		; rcx = maxIteration
	sub		rcx, rax		; rcx = maxIteration - iteration
	imul	rcx, 255		; rcx = (maxIteration - iteration) * 255
	sar		rcx, 7			; rcx = (maxIteration - iteration) * 255 / maxIteration (maxIteration == 128 == 2^7)

	; rbx = pixelIdx = 3 * (row * width + col)
	mov		rbx, r8			; rbx = row
	imul	rbx, rsi		; rbx = row * width
	add		rbx, r9			; rbx = row * width + col
	mov		r12, rbx		; r12 = 1 * (row * width + col)
	shl		rbx, 1			; rbx = 2 * (row * width + col)
	add		rbx, r12		; rbx = 3 * (row * width + col)

	mov 	[rdi + rbx], cl				; *(pixels + pixelIdx) = r
	mov		byte [rdi + rbx + 1], 0		; *(pixels + pixelIdx + 1) = 0
	mov		byte [rdi + rbx + 2], 0		; *(pixels + pixelIdx + 2) = 0


	; repeat while col >= 0
	dec		r9
	jnz		colLoop
colLoopEnd:

	; repeat while row >= 0
	dec		r8
	jnz		rowLoop
rowLoopEnd:

	; Epilogue
	pop		r12
	pop		rbx
	mov		rsp, rbp
	pop		rbp
	ret