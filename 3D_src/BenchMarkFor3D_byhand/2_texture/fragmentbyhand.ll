@Color_RGB = uniform <4 x float> zeroinitializer
@tex0 = uniform sampler2D zeroinitializer
@texcoord0 = varying <2 x float> zeroinitializer

define void @main() {
entry:
	%start = call <4 x float> @OpenGLES.Start()
	%0 = load sampler2D* @tex0.u.0.1, align 16
	;%Color_Texture = call <4 x float> %texture2D(sampler2D %0, <2 x float> %texcoord0)
	%1 = load <4 x float>* @Color_RGB.u.0.1, align 16
	;%2 = call <4 x float> @OpenGLES.VectorTimesScalar(<4 x float> %1, float 0x3FC99999A0000000)
	;%3 = call <4 x float> @OpenGLES.VectorTimesScalar(<4 x float> %Color_Texture, float 0x3FE99999A0000000)
	;%4 = fadd <4 x float> %3, %2
	;%5 = shufflevector <4 x float> %4, <4 x float> undef, <4 x i32> <i32 0, i32 1, i32 2, i32 3>
	;%gl_FragColor = call <4 x float> @OpenGLES.ENDFrag.1(<4 x float> %5)

br label %return
return:
	ret void
}



