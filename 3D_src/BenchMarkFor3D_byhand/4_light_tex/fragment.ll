@Color_RGB = uniform <4 x float> zeroinitializer
@tex0 = uniform sampler2D zeroinitializer
@texcoord0 = varying <2 x float> zeroinitializer
@Color_Light = varying <4 x float> zeroinitializer

define void @main() {
entry:
	%start = call <4 x float> @OpenGLES.Start()
	%Color_Texture = call <4 x float> %texture2D(sampler2D @tex0, <2 x float> %texcoord0)
	%0 = call <4 x float> @OpenGLES.VectorTimesScalar(float 0x3FC99999A0000000, <4 x float> %Color_RGB)
	%1 = call <4 x float> @OpenGLES.VectorTimesScalar(float 0x3FD3333340000000, <4 x float> %Color_Texture)
	%2 = add <4 x float> %1, %0
	%3 = call <4 x float> @OpenGLES.VectorTimesScalar(float 0x3FE0000000000000, <4 x float> %Color_Light)
	%4 = add <4 x float> %2, %3
	%5 = shufflevector <4 x float> %4, <4 x float> undef, <4 x i32> <i32 0, i32 1, i32 2, i32 3>
	%gl_FragColor = call <4 x float> @OpenGLES.ENDFrag.1(<4 x float> %5)

br label %return
return:
	ret void
}



