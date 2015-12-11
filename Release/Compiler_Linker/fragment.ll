@FragColor_temp  = global <4 x float> zeroinitializer
@color = varying <4 x float> zeroinitializer

define float @lerp(float %a, float %b, float %s) {
entry:
	%s_addr = alloca float
	%b_addr = alloca float
	%a_addr = alloca float
	store float %a, float* %a_addr
	store float %b, float* %b_addr
	store float %s, float* %s_addr
	%0 = load float* %a_addr, align 4
	%1 = load float* %b_addr, align 4
	%2 = fsub float %1, %0
	%3 = load float* %s_addr, align 4
	%4 = fmul float %3, %2
	%5 = fadd float %0, %4
	ret float %5
}


define void @main() {
entry:
	%start = call <4 x float> @OpenGLES.Start(), !dbg !4
	%sin_c = alloca float
	%weight = alloca float
	%arr0 = alloca i32
	%arrVal = alloca float
	%c = alloca float
	%sinValues = alloca [17 x float]
	%0 = getelementptr inbounds[17 x float]* %sinValues , i32 0 , i32 0
	%1 = load float* %0, align 4
	%2= fmul float %1, 0x0000000000000000
	%3= fadd float %2, 0x0000000000000000
	store float %3, float* %0
	%4 = getelementptr inbounds[17 x float]* %sinValues , i32 0 , i32 1
	%5 = load float* %4, align 4
	%6= fmul float %5, 0x0000000000000000
	%7= fadd float %6, 0x3FD87DE0E0000000
	store float %7, float* %4
	%8 = getelementptr inbounds[17 x float]* %sinValues , i32 0 , i32 2
	%9 = load float* %8, align 4
	%10= fmul float %9, 0x0000000000000000
	%11= fadd float %10, 0x3FE6A09EE0000000
	store float %11, float* %8
	%12 = getelementptr inbounds[17 x float]* %sinValues , i32 0 , i32 3
	%13 = load float* %12, align 4
	%14= fmul float %13, 0x0000000000000000
	%15= fadd float %14, 0x3FED906CC0000000
	store float %15, float* %12
	%16 = getelementptr inbounds[17 x float]* %sinValues , i32 0 , i32 4
	%17 = load float* %16, align 4
	%18= fmul float %17, 0x0000000000000000
	%19= fadd float %18, 0x3FF0000000000000
	store float %19, float* %16
	%20 = getelementptr inbounds[17 x float]* %sinValues , i32 0 , i32 5
	%21 = load float* %20, align 4
	%22= fmul float %21, 0x0000000000000000
	%23= fadd float %22, 0x3FED906CC0000000
	store float %23, float* %20
	%24 = getelementptr inbounds[17 x float]* %sinValues , i32 0 , i32 6
	%25 = load float* %24, align 4
	%26= fmul float %25, 0x0000000000000000
	%27= fadd float %26, 0x3FE6A09EE0000000
	store float %27, float* %24
	%28 = getelementptr inbounds[17 x float]* %sinValues , i32 0 , i32 7
	%29 = load float* %28, align 4
	%30= fmul float %29, 0x0000000000000000
	%31= fadd float %30, 0x3FD87DE0E0000000
	store float %31, float* %28
	%32 = getelementptr inbounds[17 x float]* %sinValues , i32 0 , i32 8
	%33 = load float* %32, align 4
	%34= fmul float %33, 0x0000000000000000
	%35= fadd float %34, 0x0000000000000000
	store float %35, float* %32
	%36 = getelementptr inbounds[17 x float]* %sinValues , i32 0 , i32 9
	%37 = load float* %36, align 4
	%38= fmul float %37, 0x0000000000000000
	%39= fadd float %38, 0xBFD87DE0E0000000
	store float %39, float* %36
	%40 = getelementptr inbounds[17 x float]* %sinValues , i32 0 , i32 10
	%41 = load float* %40, align 4
	%42= fmul float %41, 0x0000000000000000
	%43= fadd float %42, 0xBFE6A09EE0000000
	store float %43, float* %40
	%44 = getelementptr inbounds[17 x float]* %sinValues , i32 0 , i32 11
	%45 = load float* %44, align 4
	%46= fmul float %45, 0x0000000000000000
	%47= fadd float %46, 0xBFED906CC0000000
	store float %47, float* %44
	%48 = getelementptr inbounds[17 x float]* %sinValues , i32 0 , i32 12
	%49 = load float* %48, align 4
	%50= fmul float %49, 0x0000000000000000
	%51= fadd float %50, 0xBFF0000000000000
	store float %51, float* %48
	%52 = getelementptr inbounds[17 x float]* %sinValues , i32 0 , i32 13
	%53 = load float* %52, align 4
	%54= fmul float %53, 0x0000000000000000
	%55= fadd float %54, 0xBFED906AC0000000
	store float %55, float* %52
	%56 = getelementptr inbounds[17 x float]* %sinValues , i32 0 , i32 14
	%57 = load float* %56, align 4
	%58= fmul float %57, 0x0000000000000000
	%59= fadd float %58, 0xBFE6A09EE0000000
	store float %59, float* %56
	%60 = getelementptr inbounds[17 x float]* %sinValues , i32 0 , i32 15
	%61 = load float* %60, align 4
	%62= fmul float %61, 0x0000000000000000
	%63= fadd float %62, 0xBFD87DE0E0000000
	store float %63, float* %60
	%64 = getelementptr inbounds[17 x float]* %sinValues , i32 0 , i32 16
	%65 = load float* %64, align 4
	%66= fmul float %65, 0x0000000000000000
	%67= fadd float %66, 0x0000000000000000
	store float %67, float* %64
	%68 = call <4 x float> @OpenGLES.LoadVarying4.color.0(), !dbg !4
	%69 = extractelement <4 x float> %68, i32 0, !dbg !4
	%70 = fmul float %69, 0x400921FB60000000
	%71 = fmul float %70, 0x4000000000000000
	store float %71, float* %c
	%72 = load float* %c, align 4
	%73 = fmul float 0x40045F3060000000, %72
	store float %73, float* %arrVal
	%74 = load float* %arrVal, align 4
	%75 = call float @OpenGLES.Floor.f(float %74)
	%76 = call i32 @OpenGLES.Convert.fptosi(float %75)
	%77 = call float @OpenGLES.Floor.f(float %74)
	%78 = fsub float %74, %77
	store float %78, float* %weight
	store float 0x0000000000000000, float* %sin_c
	%79 = icmp eq i32 %76, 0
	store i32 %76, i32* %arr0
	br i1 %79, label %L1.T, label %L1.F
L1.T:
	%80 = getelementptr inbounds [17 x float]* %sinValues, i32 0, i32 1
	%81 = load float* %80, align 4
	%82 = getelementptr inbounds [17 x float]* %sinValues, i32 0, i32 0
	%83 = load float* %82, align 4
	%84 = load float* %weight, align 4
	%85 = call float @lerp(float %83, float %81, float %84)
	store float %85, float* %sin_c
	br label %L1.E
L1.F:
	%86 = load i32* %arr0, align 4
	%87 = icmp eq i32 %86, 1
	br i1 %87, label %L2.T, label %L2.F
L2.T:
	%88 = getelementptr inbounds [17 x float]* %sinValues, i32 0, i32 2
	%89 = load float* %88, align 4
	%90 = getelementptr inbounds [17 x float]* %sinValues, i32 0, i32 1
	%91 = load float* %90, align 4
	%92 = load float* %weight, align 4
	%93 = call float @lerp(float %91, float %89, float %92)
	store float %93, float* %sin_c
	br label %L2.E
L2.F:
	%94 = load i32* %arr0, align 4
	%95 = icmp eq i32 %94, 2
	br i1 %95, label %L3.T, label %L3.F
L3.T:
	%96 = getelementptr inbounds [17 x float]* %sinValues, i32 0, i32 3
	%97 = load float* %96, align 4
	%98 = getelementptr inbounds [17 x float]* %sinValues, i32 0, i32 2
	%99 = load float* %98, align 4
	%100 = load float* %weight, align 4
	%101 = call float @lerp(float %99, float %97, float %100)
	store float %101, float* %sin_c
	br label %L3.E
L3.F:
	%102 = load i32* %arr0, align 4
	%103 = icmp eq i32 %102, 3
	br i1 %103, label %L4.T, label %L4.F
L4.T:
	%104 = getelementptr inbounds [17 x float]* %sinValues, i32 0, i32 4
	%105 = load float* %104, align 4
	%106 = getelementptr inbounds [17 x float]* %sinValues, i32 0, i32 3
	%107 = load float* %106, align 4
	%108 = load float* %weight, align 4
	%109 = call float @lerp(float %107, float %105, float %108)
	store float %109, float* %sin_c
	br label %L4.E
L4.F:
	%110 = load i32* %arr0, align 4
	%111 = icmp eq i32 %110, 4
	br i1 %111, label %L5.T, label %L5.F
L5.T:
	%112 = getelementptr inbounds [17 x float]* %sinValues, i32 0, i32 5
	%113 = load float* %112, align 4
	%114 = getelementptr inbounds [17 x float]* %sinValues, i32 0, i32 4
	%115 = load float* %114, align 4
	%116 = load float* %weight, align 4
	%117 = call float @lerp(float %115, float %113, float %116)
	store float %117, float* %sin_c
	br label %L5.E
L5.F:
	%118 = load i32* %arr0, align 4
	%119 = icmp eq i32 %118, 5
	br i1 %119, label %L6.T, label %L6.F
L6.T:
	%120 = getelementptr inbounds [17 x float]* %sinValues, i32 0, i32 6
	%121 = load float* %120, align 4
	%122 = getelementptr inbounds [17 x float]* %sinValues, i32 0, i32 5
	%123 = load float* %122, align 4
	%124 = load float* %weight, align 4
	%125 = call float @lerp(float %123, float %121, float %124)
	store float %125, float* %sin_c
	br label %L6.E
L6.F:
	%126 = load i32* %arr0, align 4
	%127 = icmp eq i32 %126, 6
	br i1 %127, label %L7.T, label %L7.F
L7.T:
	%128 = getelementptr inbounds [17 x float]* %sinValues, i32 0, i32 7
	%129 = load float* %128, align 4
	%130 = getelementptr inbounds [17 x float]* %sinValues, i32 0, i32 6
	%131 = load float* %130, align 4
	%132 = load float* %weight, align 4
	%133 = call float @lerp(float %131, float %129, float %132)
	store float %133, float* %sin_c
	br label %L7.E
L7.F:
	%134 = load i32* %arr0, align 4
	%135 = icmp eq i32 %134, 7
	br i1 %135, label %L8.T, label %L8.F
L8.T:
	%136 = getelementptr inbounds [17 x float]* %sinValues, i32 0, i32 8
	%137 = load float* %136, align 4
	%138 = getelementptr inbounds [17 x float]* %sinValues, i32 0, i32 7
	%139 = load float* %138, align 4
	%140 = load float* %weight, align 4
	%141 = call float @lerp(float %139, float %137, float %140)
	store float %141, float* %sin_c
	br label %L8.E
L8.F:
	%142 = load i32* %arr0, align 4
	%143 = icmp eq i32 %142, 8
	br i1 %143, label %L9.T, label %L9.F
L9.T:
	%144 = getelementptr inbounds [17 x float]* %sinValues, i32 0, i32 9
	%145 = load float* %144, align 4
	%146 = getelementptr inbounds [17 x float]* %sinValues, i32 0, i32 8
	%147 = load float* %146, align 4
	%148 = load float* %weight, align 4
	%149 = call float @lerp(float %147, float %145, float %148)
	store float %149, float* %sin_c
	br label %L9.E
L9.F:
	%150 = load i32* %arr0, align 4
	%151 = icmp eq i32 %150, 9
	br i1 %151, label %L10.T, label %L10.F
L10.T:
	%152 = getelementptr inbounds [17 x float]* %sinValues, i32 0, i32 10
	%153 = load float* %152, align 4
	%154 = getelementptr inbounds [17 x float]* %sinValues, i32 0, i32 9
	%155 = load float* %154, align 4
	%156 = load float* %weight, align 4
	%157 = call float @lerp(float %155, float %153, float %156)
	store float %157, float* %sin_c
	br label %L10.E
L10.F:
	%158 = load i32* %arr0, align 4
	%159 = icmp eq i32 %158, 10
	br i1 %159, label %L11.T, label %L11.F
L11.T:
	%160 = getelementptr inbounds [17 x float]* %sinValues, i32 0, i32 11
	%161 = load float* %160, align 4
	%162 = getelementptr inbounds [17 x float]* %sinValues, i32 0, i32 10
	%163 = load float* %162, align 4
	%164 = load float* %weight, align 4
	%165 = call float @lerp(float %163, float %161, float %164)
	store float %165, float* %sin_c
	br label %L11.E
L11.F:
	%166 = load i32* %arr0, align 4
	%167 = icmp eq i32 %166, 11
	br i1 %167, label %L12.T, label %L12.F
L12.T:
	%168 = getelementptr inbounds [17 x float]* %sinValues, i32 0, i32 12
	%169 = load float* %168, align 4
	%170 = getelementptr inbounds [17 x float]* %sinValues, i32 0, i32 11
	%171 = load float* %170, align 4
	%172 = load float* %weight, align 4
	%173 = call float @lerp(float %171, float %169, float %172)
	store float %173, float* %sin_c
	br label %L12.E
L12.F:
	%174 = load i32* %arr0, align 4
	%175 = icmp eq i32 %174, 12
	br i1 %175, label %L13.T, label %L13.F
L13.T:
	%176 = getelementptr inbounds [17 x float]* %sinValues, i32 0, i32 13
	%177 = load float* %176, align 4
	%178 = getelementptr inbounds [17 x float]* %sinValues, i32 0, i32 12
	%179 = load float* %178, align 4
	%180 = load float* %weight, align 4
	%181 = call float @lerp(float %179, float %177, float %180)
	store float %181, float* %sin_c
	br label %L13.E
L13.F:
	%182 = load i32* %arr0, align 4
	%183 = icmp eq i32 %182, 13
	br i1 %183, label %L14.T, label %L14.F
L14.T:
	%184 = getelementptr inbounds [17 x float]* %sinValues, i32 0, i32 14
	%185 = load float* %184, align 4
	%186 = getelementptr inbounds [17 x float]* %sinValues, i32 0, i32 13
	%187 = load float* %186, align 4
	%188 = load float* %weight, align 4
	%189 = call float @lerp(float %187, float %185, float %188)
	store float %189, float* %sin_c
	br label %L14.E
L14.F:
	%190 = load i32* %arr0, align 4
	%191 = icmp eq i32 %190, 14
	br i1 %191, label %L15.T, label %L15.F
L15.T:
	%192 = getelementptr inbounds [17 x float]* %sinValues, i32 0, i32 15
	%193 = load float* %192, align 4
	%194 = getelementptr inbounds [17 x float]* %sinValues, i32 0, i32 14
	%195 = load float* %194, align 4
	%196 = load float* %weight, align 4
	%197 = call float @lerp(float %195, float %193, float %196)
	store float %197, float* %sin_c
	br label %L15.E
L15.F:
	%198 = load i32* %arr0, align 4
	%199 = icmp eq i32 %198, 15
	br i1 %199, label %L16.T, label %L16.F
L16.T:
	%200 = getelementptr inbounds [17 x float]* %sinValues, i32 0, i32 16
	%201 = load float* %200, align 4
	%202 = getelementptr inbounds [17 x float]* %sinValues, i32 0, i32 15
	%203 = load float* %202, align 4
	%204 = load float* %weight, align 4
	%205 = call float @lerp(float %203, float %201, float %204)
	store float %205, float* %sin_c
	br label %L16.E
L16.F:
	%206 = load i32* %arr0, align 4
	%207 = icmp eq i32 %206, 16
	br i1 %207, label %L17.T, label %L17.E
L17.T:
	%208 = getelementptr inbounds [17 x float]* %sinValues, i32 0, i32 16
	%209 = load float* %208, align 4
	store float %209, float* %sin_c
	br label %L17.E
L17.E:
	br label %L16.E
L16.E:
	br label %L15.E
L15.E:
	br label %L14.E
L14.E:
	br label %L13.E
L13.E:
	br label %L12.E
L12.E:
	br label %L11.E
L11.E:
	br label %L10.E
L10.E:
	br label %L9.E
L9.E:
	br label %L8.E
L8.E:
	br label %L7.E
L7.E:
	br label %L6.E
L6.E:
	br label %L5.E
L5.E:
	br label %L4.E
L4.E:
	br label %L3.E
L3.E:
	br label %L2.E
L2.E:
	br label %L1.E
L1.E:
	%210 = load float* %sin_c, align 4
	%211 = fmul float %210, 0x3FE0000000000000
	%212 = fadd float %211, 0x3FE0000000000000
	%213 = call <4 x float> @OpenGLES.ConstructVec4.f.f.f.f(float %212, float 0x0000000000000000, float 0x0000000000000000, float 0x3FF0000000000000), !dbg !4
	%214 = shufflevector <4 x float> %213, <4 x float> undef, <4 x i32> <i32 0, i32 1, i32 2, i32 3>, !dbg !4
	%215 = call <4 x float> @OpenGLES.ENDFrag.1(<4 x float> %214), !dbg !4

br label %return
return:
	ret void
}
!1 = metadata !{i32 458763, metadata !2}; [DW_TAG_lexical_block ]
!2 = metadata !{i32 458798, i32 0, metadata !3, metadata !"vector", metadata !"vector", metadata !"vector", metadata !3, i32 1, null, i1 false, i1 true}; [DW_TAG_subprogram ]
!3 = metadata !{i32 458769, i32 0, i32 12, metadata !"size", metadata !"", metadata !"", i1 true, i1 false, metadata !"", i32 0}; [DW_TAG_compile_unit ]
!4 = metadata !{i32 4, null, metadata !2, null}
!5 = metadata !{i32 3, null, metadata !2, null}




