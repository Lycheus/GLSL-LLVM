@Color_Light = varying <4 x float> zeroinitializer
@Tex = uniform sampler2D zeroinitializer
@TexCoord = varying <2 x float> zeroinitializer
@normal = varying <3 x float> zeroinitializer
@shaderr = uniform i32 zeroinitializer

define void @main() {
entry:
	%start = call <4 x float> @OpenGLES.Start()
	%0 = icmp eq, i32 @shaderr, 1
	br i1 %0, label %L1.T, label %L1.F
L1.T:
	%1 = call <4 x float> %texture2D(sampler2D @Tex, <2 x float> %TexCoord)
	%gl_FragColor = call <4 x float> @OpenGLES.ENDFrag.1(<4 x float> %1)
	br label %L1.E
L1.F:
	%3 = shufflevector <4 x float> %2, <4 x float> undef, <4 x i32> <i32 0, i32 1, i32 2, i32 3>
	%temp_normal = call <3 x float> @OpenGLES.Normalize(<3 x float> %temp_normal)
	%4 = extractelement <3 x float> %temp_normal, i32 0
	%gl_FragColor = insertelement <4 x float> %gl_FragColor, float %4, i32 0
	%5 = extractelement <3 x float> %temp_normal, i32 1
	%gl_FragColor = insertelement <4 x float> %gl_FragColor, float %5, i32 1
	%6 = extractelement <3 x float> %temp_normal, i32 2
	%gl_FragColor = insertelement <4 x float> %gl_FragColor, float %6, i32 2
L1.E:
	%7 = add <4 x float> %gl_FragColor, @Color_Light
	%8 = shufflevector <4 x float> %7, <4 x float> undef, <4 x i32> <i32 0, i32 1, i32 2, i32 3>
	%gl_FragColor = call <4 x float> @OpenGLES.ENDFrag.1(<4 x float> %8)

br label %return
return:
	ret void
}



