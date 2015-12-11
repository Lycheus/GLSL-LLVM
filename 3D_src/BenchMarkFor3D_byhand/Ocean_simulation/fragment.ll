@facing = varying float zeroinitializer
@lightDir = varying <3 x float> zeroinitializer
@ecPosition = varying <3 x float> zeroinitializer
@surfaceColor = uniform <3 x float> zeroinitializer
@BumpDensity = uniform float zeroinitializer
@BumpSize = uniform float zeroinitializer
@SpecularFactor = uniform float zeroinitializer
@waterTexture = uniform sampler2D zeroinitializer
@Intensity = varying float zeroinitializer
@viewVec = varying <3 x float> zeroinitializer
@normal = varying <3 x float> zeroinitializer

define void @main() {
entry:
	%start = call <4 x float> @OpenGLES.Start()
	%1 = shufflevector <4 x float> %0, <4 x float> undef, <4 x i32> <i32 0, i32 1, i32 2, i32 3>
	%3 = shufflevector <4 x float> %2, <4 x float> undef, <4 x i32> <i32 0, i32 1, i32 2, i32 3>
	%eDotN = call float @OpenGLES.Dot(<3 x float> %viewVec, <3 x float> %normal)
	%4 = load <3 x float>* @normal.v, align 12
	%5 = call <3 x float> @OpenGLES.VectorTimesScalar(<3 x float> %4, float %eDotN)
	%6 = call <3 x float> @OpenGLES.VectorTimesScalar(<3 x float> %5, float 0x4000000000000000)
	%7 = sub <3 x float> %6, @viewVec
	%9 = shufflevector <4 x float> %8, <4 x float> undef, <4 x i32> <i32 0, i32 1, i32 2, i32 3>
	%10 = call <2 x float> @OpenGLES.ConstructVec2.vec3(<3 x float> %vEyeReflected)
	%reflection = call <4 x float> @texture2D(sampler2D @waterTexture, <2 x float> %10)
	%11 = sub float 0x3FF0000000000000, %eDotN
	%12 = extractelement <4 x float> %reflection, i32 1
	%13 = mul float %12, %11
	%Fresnel = or float %13, 0
	%14 = mul float %eDotN, 0x3FE0000000000000
	%diffuse = call <4 x float> @OpenGLES.VectorTimesScalar(float %14, <4 x float> %watercolor)
	%15 = mul float %eDotN, 0x3FF3333340000000
	%16 = call <4 x float> @OpenGLES.VectorTimesScalar(float %15, <4 x float> %reflection)
	%17 = sub <4 x float> %16, %diffuse
	%18 = call <4 x float> @OpenGLES.VectorTimesScalar(<4 x float> %17, float %Fresnel)
	%19 = add <4 x float> %diffuse, %18
	%20 = call <4 x float> @OpenGLES.VectorTimesScalar(float 0x3FD3333340000000, <4 x float> %reflection)
	%21 = add <4 x float> %19, %20
	%color = call <4 x float> @OpenGLES.Clamp(<4 x float> %21, float 0x0000000000000000, float 0x3FF0000000000000)
	%22 = extractelement <4 x float> %reflection, i32 1
	%23 = call float @OpenGLES.Pow(float %22, float 0x4020000000000000)
	%24 = mul float %eDotN, %23
	%specular = call <4 x float> @OpenGLES.VectorTimesScalar(float %24, <4 x float> %waterhighcolor)

br label %return
return:
	ret void
}



