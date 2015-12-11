@TexCoord = varying <2 x float> zeroinitializer
@Tex = uniform sampler2D zeroinitializer
@diffuse = varying <4 x float> zeroinitializer
@ambient = varying <4 x float> zeroinitializer
@normal = varying <3 x float> zeroinitializer
@lightDir = varying <3 x float> zeroinitializer
@halfVector = varying <3 x float> zeroinitializer
@specular_light = uniform <4 x float> zeroinitializer
@specular_material = uniform <4 x float> zeroinitializer
@specularExp = uniform float zeroinitializer
@shaderr = uniform i32 zeroinitializer

define void @main() {
entry:
	%start = call <4 x float> @OpenGLES.Start()
	%0 = load i32* @shaderr.u.0.1, align 16
	%1 = icmp eq i32 %0, 1
	;br i1 %1, label %L1.T, label %L1.F
;L1.T:
	%2 = load sampler2D* @Tex0.u.0.1, align 16
	;%1 = call <4 x float> %texture2D(sampler2D %Tex, <2 x float> %TexCoord)
	;%gl_FragColor = call <4 x float> @OpenGLES.ENDFrag.1(<4 x float> %1)
	;br label %L1.E
;L1.F:
	;%3 = shufflevector <4 x float> %2, <4 x float> undef, <4 x i32> <i32 0, i32 1, i32 2, i32 3>
	;%temp_normal = call <3 x float> @OpenGLES.Normalize(<3 x float> %temp_normal)
	;%4 = extractelement <3 x float> %temp_normal, i32 0
	;%gl_FragColor = insertelement <4 x float> %gl_FragColor, float %4, i32 0
	;%5 = extractelement <3 x float> %temp_normal, i32 1
	;%gl_FragColor = insertelement <4 x float> %gl_FragColor, float %5, i32 1
	;%6 = extractelement <3 x float> %temp_normal, i32 2
	;%gl_FragColor = insertelement <4 x float> %gl_FragColor, float %6, i32 2
;L1.E;:
	;%8 = shufflevector <4 x float> %7, <4 x float> undef, <4 x i32> <i32 0, i32 1, i32 2, i32 3>
	;%n = call <3 x float> @OpenGLES.Normalize(<3 x float> %normal)
	;%9 = call float @OpenGLES.Dot(<3 x float> %n, <3 x float> %lightDir)
	;%NdotL = call float @OpenGLES.Max(float %9, float 0x0000000000000000)
	;%10 = icmp sgt, float %NdotL, 0x0000000000000000
	;br i1 %10, label %L2.T, label %L2.F
;L2.T:
	;%halfV = call <3 x float> @OpenGLES.Normalize(<3 x float> %halfVector)
	;%11 = call float @OpenGLES.Dot(<3 x float> %n, <3 x float> %halfV)
	;%NdotHV = call float @OpenGLES.Max(float %11, float 0x0000000000000000)
	%3 = load float* @specularExp.u.0.1, align 16
	;%12 = call float @OpenGLES.Pow(float %NdotHV, float @specularExp)
	%4 = load float* @specular_material.u.0.1, align 16
	;%13 = call <4 x float> @OpenGLES.VectorTimesScalar(float %12, <4 x float> %specular_material)
	%5 = load float* @specular_light.u.0.1, align 16
	;%14 = mul <4 x float> %13, @specular_light
	;%15 = add <4 x float> %color_light, %14
	;%17 = shufflevector <4 x float> %16, <4 x float> undef, <4 x i32> <i32 0, i32 1, i32 2, i32 3>
	;%18 = call <4 x float> @OpenGLES.VectorTimesScalar(float %NdotL, <4 x float> %diffuse)
	;%19 = add <4 x float> %color_light, %18
	;%21 = shufflevector <4 x float> %20, <4 x float> undef, <4 x i32> <i32 0, i32 1, i32 2, i32 3>
;L2.F:
;L2.E:
	;%23 = shufflevector <4 x float> %22, <4 x float> undef, <4 x i32> <i32 0, i32 1, i32 2, i32 3>
	;%24 = add <4 x float> %gl_FragColor, %color_light
	;%25 = shufflevector <4 x float> %24, <4 x float> undef, <4 x i32> <i32 0, i32 1, i32 2, i32 3>
	;%gl_FragColor = call <4 x float> @OpenGLES.ENDFrag.1(<4 x float> %25)

br label %return
return:
	ret void
}