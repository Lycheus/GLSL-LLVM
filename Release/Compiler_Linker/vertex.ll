@Position_temp  = global <4 x float> zeroinitializer
@gtf_Color = attribute <4 x float> zeroinitializer
@gtf_Vertex = attribute <4 x float> zeroinitializer
@gtf_ModelViewProjectionMatrix = uniform [4 x <4 x float>] zeroinitializer
@color = varying <4 x float> zeroinitializer
@temp_var0  = global <4 x float> zeroinitializer

define void @main() {
entry:
	%start = call <4 x float> @OpenGLES.Start(), !dbg !4
	%0 = call <4 x float> @OpenGLES.LoadAttribute4.gtf_Color.0(), !dbg !4
	%1 = shufflevector <4 x float> %0, <4 x float> undef, <4 x i32> <i32 0, i32 1, i32 2, i32 3>, !dbg !4
	%2= getelementptr inbounds [4 x <4 x float>]* @gtf_ModelViewProjectionMatrix.u.0.4, i32 0, i32 0, !dbg !4
	%3= load <4 x float>* %2, !dbg !4
	%4= getelementptr inbounds [4 x <4 x float>]* @gtf_ModelViewProjectionMatrix.u.0.4, i32 0, i32 1, !dbg !4
	%5= load <4 x float>* %4, !dbg !4
	%6= getelementptr inbounds [4 x <4 x float>]* @gtf_ModelViewProjectionMatrix.u.0.4, i32 0, i32 2, !dbg !4
	%7= load <4 x float>* %6, !dbg !4
	%8= getelementptr inbounds [4 x <4 x float>]* @gtf_ModelViewProjectionMatrix.u.0.4, i32 0, i32 3, !dbg !4
	%9= load <4 x float>* %8, !dbg !4
	%10 = call <4 x float> @OpenGLES.ChangeMatrix4.1(<4 x float> %9, <4 x float> %7, <4 x float> %5, <4 x float> %3), !dbg !4
	%11 = call <4 x float> @OpenGLES.ChangeMatrix4.2(<4 x float> %9, <4 x float> %7, <4 x float> %5, <4 x float> %3), !dbg !4
	%12 = call <4 x float> @OpenGLES.ChangeMatrix4.3(<4 x float> %9, <4 x float> %7, <4 x float> %5, <4 x float> %3), !dbg !4
	%13 = call <4 x float> @OpenGLES.ChangeMatrix4.4(<4 x float> %9, <4 x float> %7, <4 x float> %5, <4 x float> %3), !dbg !4
	%14 = call <4 x float> @OpenGLES.LoadAttribute4.gtf_Vertex.0(), !dbg !4
	%15 = call <4 x float> @OpenGLES.CallRegister(), !dbg !4
	%16 = call <4 x float> @OpenGLES.VectorTimesMatrix4.6(<4 x float> %14, <4 x float> %13, <4 x float> %12, <4 x float> %11, <4 x float> %10, <4 x float> %15), !dbg !4
	%17 = extractelement <4 x float> %16, i32 3, !dbg !4
	%18 = fmul float %17, 0x4024000000000000
	%19 = insertelement <4 x float> %16, float %18, i32 3, !dbg !4
	%20 = load float* @gl_PointSize, align 4
	%21= fmul float %20, 0x0000000000000000
	%22= fadd float %21, 0x3FF0000000000000
	store float %22, float* @gl_PointSize
	%23 = shufflevector <4 x float> %1, <4 x float> undef, <4 x i32> <i32 0, i32 1, i32 2, i32 3>, !dbg !4
	%24 = call <4 x float> @OpenGLES.VaryingPut4.color.1(<4 x float> %23), !dbg !4
	%25 = shufflevector <4 x float> %19, <4 x float> undef, <4 x i32> <i32 0, i32 1, i32 2, i32 3>, !dbg !4
	%26 = call <4 x float> @OpenGLES.CallRegister(), !dbg !4
	%27 = call <4 x float> @OpenGLES.CallRegister(), !dbg !4
	%28 = call <4 x float> @OpenGLES.END.3(<4 x float> %25, <4 x float> %26, <4 x float> %27), !dbg !4

br label %return
return:
	ret void
}
!1 = metadata !{i32 458763, metadata !2}; [DW_TAG_lexical_block ]
!2 = metadata !{i32 458798, i32 0, metadata !3, metadata !"vector", metadata !"vector", metadata !"vector", metadata !3, i32 1, null, i1 false, i1 true}; [DW_TAG_subprogram ]
!3 = metadata !{i32 458769, i32 0, i32 12, metadata !"size", metadata !"", metadata !"", i1 true, i1 false, metadata !"", i32 0}; [DW_TAG_compile_unit ]
!4 = metadata !{i32 4, null, metadata !2, null}
!5 = metadata !{i32 3, null, metadata !2, null}




