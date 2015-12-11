#### BEGIN COMPILER 0 INFO LOG ####
@Position_temp  = highp global <4 x float>
@gtf_Color = highp attribute <4 x float>
@gtf_Vertex = highp attribute <4 x float>
@gtf_ModelViewProjectionMatrix = highp uniform [4 x <4 x float>]
@color = highp varying <4 x float>
@temp_var0  = highp global <4 x float>

define highp void @main:void() {
	%0 = call <4 x float> @OpenGLES.LoadAttribute4.gtf_Color.0()
	%1 = shufflevector <4 x float> %0, <4 x float> undef, <4 x i32> <i32 0, i32 1, i32 2, i32 3>
	%2= getelementptr inbounds highp [4 x <4 x float>]* @gtf_ModelViewProjectionMatrix.u.0.4, i32 0, i32 0
	%3= load highp <4 x float>* %2
	%4= getelementptr inbounds highp [4 x <4 x float>]* @gtf_ModelViewProjectionMatrix.u.0.4, i32 0, i32 1
	%5= load highp <4 x float>* %4
	%6= getelementptr inbounds highp [4 x <4 x float>]* @gtf_ModelViewProjectionMatrix.u.0.4, i32 0, i32 2
	%7= load highp <4 x float>* %6
	%8= getelementptr inbounds highp [4 x <4 x float>]* @gtf_ModelViewProjectionMatrix.u.0.4, i32 0, i32 3
	%9= load highp <4 x float>* %8
	%10 = call highp <4 x float> @OpenGLES.ChangeMatrix4.1(highp <4 x float> %9, highp <4 x float> %7, highp <4 x float> %5, highp <4 x float> %3)
	%11 = call highp <4 x float> @OpenGLES.ChangeMatrix4.2(highp <4 x float> %9, highp <4 x float> %7, highp <4 x float> %5, highp <4 x float> %3)
	%12 = call highp <4 x float> @OpenGLES.ChangeMatrix4.3(highp <4 x float> %9, highp <4 x float> %7, highp <4 x float> %5, highp <4 x float> %3)
	%13 = call highp <4 x float> @OpenGLES.ChangeMatrix4.4(highp <4 x float> %9, highp <4 x float> %7, highp <4 x float> %5, highp <4 x float> %3)
	%14 = call <4 x float> @OpenGLES.LoadAttribute4.gtf_Vertex.0()
	%15 = call highp <4 x float> @OpenGLES.CallRegister()
	%16 = call highp <4 x float> @OpenGLES.VectorTimesMatrix4.6(highp <4 x float> %14, highp <4 x float> %13, highp <4 x float> %12, highp <4 x float> %11, highp <4 x float> %10, highp <4 x float> %15)
	%17 = extractelement highp <4 x float> %16, i32 3
	%18 = fmul highp float %17, 0x4024000000000000
	%19 = insertelement highp <4 x float> %16, highp float %18, i32 3
	%20 = load highp float* @gl_PointSize, align 4
	%21= fmul highp float %20, 0x0000000000000000
	%22= fadd highp float %21, 0x3FF0000000000000
	store highp float %22, highp float* @gl_PointSize
	%23 = shufflevector <4 x float> %1, <4 x float> undef, <4 x i32> <i32 0, i32 1, i32 2, i32 3>
	%24 = call highp <4 x float> @OpenGLES.VaryingPut4.color.1(highp <4 x float> %23)
	%25 = shufflevector <4 x float> %19, <4 x float> undef, <4 x i32> <i32 0, i32 1, i32 2, i32 3>
	%26 = call highp <4 x float> @OpenGLES.CallRegister()
	%27 = call highp <4 x float> @OpenGLES.CallRegister()
	%28 = call highp <4 x float> @OpenGLES.END.3(highp <4 x float> %25, highp <4 x float> %26, highp <4 x float> %27)
main function end
}
add_metadata


#### END COMPILER 0 INFO LOG ####
#### BEGIN LINKER INFO LOG ####

#### END LINKER INFO LOG ####
