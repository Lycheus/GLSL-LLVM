@Position_temp  = global <4 x float> zeroinitializer
@gtf_Color = attribute <4 x float> zeroinitializer
@gtf_Vertex = attribute <4 x float> zeroinitializer
@gtf_ModelViewProjectionMatrix = uniform [4 x <4 x float>] zeroinitializer
@color = varying <4 x float> zeroinitializer
@temp_var0  = global <4 x float> zeroinitializer

define void @main() {
entry:
	%start = call <4 x float> @OpenGLES.Start()
	%0 = call <4 x float> @OpenGLES.LoadAttribute4.gtf_Color.0()
	%1 = shufflevector <4 x float> %0, <4 x float> undef, <4 x i32> <i32 0, i32 1, i32 2, i32 3>
	%2= getelementptr inbounds [4 x <4 x float>]* @gtf_ModelViewProjectionMatrix.u.0.4, i32 0, i32 0
	%3= load <4 x float>* %2
	%4= getelementptr inbounds [4 x <4 x float>]* @gtf_ModelViewProjectionMatrix.u.0.4, i32 0, i32 1
	%5= load <4 x float>* %4
	%6= getelementptr inbounds [4 x <4 x float>]* @gtf_ModelViewProjectionMatrix.u.0.4, i32 0, i32 2
	%7= load <4 x float>* %6
	%8= getelementptr inbounds [4 x <4 x float>]* @gtf_ModelViewProjectionMatrix.u.0.4, i32 0, i32 3
	%9= load <4 x float>* %8
	%10 = call <4 x float> @OpenGLES.ChangeMatrix4.1(<4 x float> %9, <4 x float> %7, <4 x float> %5, <4 x float> %3)
	%11 = call <4 x float> @OpenGLES.ChangeMatrix4.2(<4 x float> %9, <4 x float> %7, <4 x float> %5, <4 x float> %3)
	%12 = call <4 x float> @OpenGLES.ChangeMatrix4.3(<4 x float> %9, <4 x float> %7, <4 x float> %5, <4 x float> %3)
	%13 = call <4 x float> @OpenGLES.ChangeMatrix4.4(<4 x float> %9, <4 x float> %7, <4 x float> %5, <4 x float> %3)
	%14 = call <4 x float> @OpenGLES.LoadAttribute4.gtf_Vertex.0()
	%15 = call <4 x float> @OpenGLES.CallRegister()
	%16 = call <4 x float> @OpenGLES.VectorTimesMatrix4.6(<4 x float> %14, <4 x float> %13, <4 x float> %12, <4 x float> %11, <4 x float> %10, <4 x float> %15)
	%17 = extractelement <4 x float> %16, i32 3
	%18 = fmul float %17, 0x4024000000000000
	%19 = insertelement <4 x float> %16, float %18, i32 3
	%20 = load float* @gl_PointSize, align 4
	%21= fmul float %20, 0x0000000000000000
	%22= fadd float %21, 0x3FF0000000000000
	store float %22, float* @gl_PointSize
	%23 = shufflevector <4 x float> %1, <4 x float> undef, <4 x i32> <i32 0, i32 1, i32 2, i32 3>
	%24 = call <4 x float> @OpenGLES.VaryingPut4.color.1(<4 x float> %23)
	%25 = shufflevector <4 x float> %19, <4 x float> undef, <4 x i32> <i32 0, i32 1, i32 2, i32 3>
	%26 = call <4 x float> @OpenGLES.CallRegister()
	%27 = call <4 x float> @OpenGLES.CallRegister()
	%28 = call <4 x float> @OpenGLES.END.3(<4 x float> %25, <4 x float> %26, <4 x float> %27)

br label %return
return:
	ret void
}
add_metadata



