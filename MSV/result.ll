; ModuleID = '_MSVOutput.cpp'
target datalayout = "e-m:x-p:32:32-i64:64-f80:32-n8:16:32-S32"
target triple = "i686-pc-windows-msvc"

$"\01??_C@_09MMEBDMLC@state?5?$CFd?3?$AA@" = comdat any

$"\01??_C@_05HGEFLAMF@i?$DN?$CFf?5?$AA@" = comdat any

$"\01??_C@_05EKDHDMFE@p?$DN?$CFp?5?$AA@" = comdat any

$"\01??_C@_01EEMJAFIK@?6?$AA@" = comdat any

@"\01?$$state_num@@3HA" = global i32 0, align 4
@"\01??_C@_09MMEBDMLC@state?5?$CFd?3?$AA@" = linkonce_odr unnamed_addr constant [10 x i8] c"state %d:\00", comdat, align 1
@"\01??_C@_05HGEFLAMF@i?$DN?$CFf?5?$AA@" = linkonce_odr unnamed_addr constant [6 x i8] c"i=%f \00", comdat, align 1
@"\01??_C@_05EKDHDMFE@p?$DN?$CFp?5?$AA@" = linkonce_odr unnamed_addr constant [6 x i8] c"p=%p \00", comdat, align 1
@"\01??_C@_01EEMJAFIK@?6?$AA@" = linkonce_odr unnamed_addr constant [2 x i8] c"\0A\00", comdat, align 1

define i32 @main() #0 {
  %1 = alloca i32, align 4
  %i = alloca float, align 4
  %p = alloca float*, align 4
  store i32 0, i32* %1
  store float* %i, float** %p, align 4
  %2 = load i32, i32* @"\01?$$state_num@@3HA", align 4
  %3 = call i32 (i8*, ...)* @printf(i8* getelementptr inbounds ([10 x i8], [10 x i8]* @"\01??_C@_09MMEBDMLC@state?5?$CFd?3?$AA@", i32 0, i32 0), i32 %2)
  %4 = load float, float* %i, align 4
  %5 = fpext float %4 to double
  %6 = call i32 (i8*, ...)* @printf(i8* getelementptr inbounds ([6 x i8], [6 x i8]* @"\01??_C@_05HGEFLAMF@i?$DN?$CFf?5?$AA@", i32 0, i32 0), double %5)
  %7 = load float*, float** %p, align 4
  %8 = call i32 (i8*, ...)* @printf(i8* getelementptr inbounds ([6 x i8], [6 x i8]* @"\01??_C@_05EKDHDMFE@p?$DN?$CFp?5?$AA@", i32 0, i32 0), float* %7)
  %9 = call i32 (i8*, ...)* @printf(i8* getelementptr inbounds ([2 x i8], [2 x i8]* @"\01??_C@_01EEMJAFIK@?6?$AA@", i32 0, i32 0))
  %10 = load i32, i32* @"\01?$$state_num@@3HA", align 4
  %11 = add nsw i32 %10, 1
  store i32 %11, i32* @"\01?$$state_num@@3HA", align 4
  %12 = load i32, i32* @"\01?$$state_num@@3HA", align 4
  %13 = call i32 (i8*, ...)* @printf(i8* getelementptr inbounds ([10 x i8], [10 x i8]* @"\01??_C@_09MMEBDMLC@state?5?$CFd?3?$AA@", i32 0, i32 0), i32 %12)
  %14 = call i32 (i8*, ...)* @printf(i8* getelementptr inbounds ([2 x i8], [2 x i8]* @"\01??_C@_01EEMJAFIK@?6?$AA@", i32 0, i32 0))
  %15 = load i32, i32* @"\01?$$state_num@@3HA", align 4
  %16 = add nsw i32 %15, 1
  store i32 %16, i32* @"\01?$$state_num@@3HA", align 4
  ret i32 0
}

declare i32 @printf(i8*, ...) #0

attributes #0 = { "less-precise-fpmad"="false" "no-frame-pointer-elim"="true" "no-frame-pointer-elim-non-leaf" "no-infs-fp-math"="false" "no-nans-fp-math"="false" "stack-protector-buffer-size"="8" "target-cpu"="pentium4" "unsafe-fp-math"="false" "use-soft-float"="false" }

!llvm.ident = !{!0}

!0 = !{!"clang version 3.7.0 (trunk)"}
