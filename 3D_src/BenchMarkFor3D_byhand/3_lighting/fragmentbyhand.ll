@Color_RGB = uniform <4 x float> zeroinitializer
@Color_Light = varying <4 x float> zeroinitializer

define void @main() {
entry:
	%start = call <4 x float> @OpenGLES.Start()
	%0 = load <4 x float>* @Color_RGB.u.0.1, align 16
	;%1 = fadd <4 x float> %0, @Color_Light
	;%2 = fdiv <4 x float> %1, 0x4000000000000000
	;%3 = shufflevector <4 x float> %2, <4 x float> undef, <4 x i32> <i32 0, i32 1, i32 2, i32 3>
	;%gl_FragColor = call <4 x float> @OpenGLES.ENDFrag.1(<4 x float> %3)

br label %return
return:
	ret void
}



