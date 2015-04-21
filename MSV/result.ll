; ModuleID = '_MSVOutput.cpp'
target datalayout = "e-m:x-p:32:32-i64:64-f80:32-n8:16:32-S32"
target triple = "i686-pc-windows-msvc"

$"\01??_C@_09MMEBDMLC@state?5?$CFd?3?$AA@" = comdat any

$"\01??_C@_07KGDPDBII@sum?$DN?$CFd?5?$AA@" = comdat any

$"\01??_C@_01EEMJAFIK@?6?$AA@" = comdat any

$"\01??_C@_05OOFACGLN@m?$DN?$CFd?5?$AA@" = comdat any

$"\01??_C@_05LNELLFJF@x?$DN?$CFd?5?$AA@" = comdat any

@"\01?$$state_num@@3HA" = global i32 0, align 4
@"\01??_C@_09MMEBDMLC@state?5?$CFd?3?$AA@" = linkonce_odr unnamed_addr constant [10 x i8] c"state %d:\00", comdat, align 1
@"\01??_C@_07KGDPDBII@sum?$DN?$CFd?5?$AA@" = linkonce_odr unnamed_addr constant [8 x i8] c"sum=%d \00", comdat, align 1
@"\01??_C@_01EEMJAFIK@?6?$AA@" = linkonce_odr unnamed_addr constant [2 x i8] c"\0A\00", comdat, align 1
@"\01??_C@_05OOFACGLN@m?$DN?$CFd?5?$AA@" = linkonce_odr unnamed_addr constant [6 x i8] c"m=%d \00", comdat, align 1
@"\01??_C@_05LNELLFJF@x?$DN?$CFd?5?$AA@" = linkonce_odr unnamed_addr constant [6 x i8] c"x=%d \00", comdat, align 1

define i32 @main() #0 {
  %1 = alloca i32, align 4
  %sum = alloca i32, align 4
  %m = alloca i32, align 4
  %x = alloca i32, align 4
  store i32 0, i32* %1
  store i32 0, i32* %sum, align 4
  %2 = load i32, i32* @"\01?$$state_num@@3HA", align 4
  %3 = call i32 (i8*, ...)* @printf(i8* getelementptr inbounds ([10 x i8], [10 x i8]* @"\01??_C@_09MMEBDMLC@state?5?$CFd?3?$AA@", i32 0, i32 0), i32 %2)
  %4 = load i32, i32* %sum, align 4
  %5 = call i32 (i8*, ...)* @printf(i8* getelementptr inbounds ([8 x i8], [8 x i8]* @"\01??_C@_07KGDPDBII@sum?$DN?$CFd?5?$AA@", i32 0, i32 0), i32 %4)
  %6 = call i32 (i8*, ...)* @printf(i8* getelementptr inbounds ([2 x i8], [2 x i8]* @"\01??_C@_01EEMJAFIK@?6?$AA@", i32 0, i32 0))
  %7 = load i32, i32* @"\01?$$state_num@@3HA", align 4
  %8 = add nsw i32 %7, 1
  store i32 %8, i32* @"\01?$$state_num@@3HA", align 4
  %9 = load i32, i32* %sum, align 4
  store i32 %9, i32* %m, align 4
  %10 = load i32, i32* @"\01?$$state_num@@3HA", align 4
  %11 = call i32 (i8*, ...)* @printf(i8* getelementptr inbounds ([10 x i8], [10 x i8]* @"\01??_C@_09MMEBDMLC@state?5?$CFd?3?$AA@", i32 0, i32 0), i32 %10)
  %12 = load i32, i32* %sum, align 4
  %13 = call i32 (i8*, ...)* @printf(i8* getelementptr inbounds ([8 x i8], [8 x i8]* @"\01??_C@_07KGDPDBII@sum?$DN?$CFd?5?$AA@", i32 0, i32 0), i32 %12)
  %14 = load i32, i32* %m, align 4
  %15 = call i32 (i8*, ...)* @printf(i8* getelementptr inbounds ([6 x i8], [6 x i8]* @"\01??_C@_05OOFACGLN@m?$DN?$CFd?5?$AA@", i32 0, i32 0), i32 %14)
  %16 = call i32 (i8*, ...)* @printf(i8* getelementptr inbounds ([2 x i8], [2 x i8]* @"\01??_C@_01EEMJAFIK@?6?$AA@", i32 0, i32 0))
  %17 = load i32, i32* @"\01?$$state_num@@3HA", align 4
  %18 = add nsw i32 %17, 1
  store i32 %18, i32* @"\01?$$state_num@@3HA", align 4
  %19 = load i32, i32* %sum, align 4
  %20 = add nsw i32 %19, 2
  store i32 %20, i32* %x, align 4
  %21 = load i32, i32* @"\01?$$state_num@@3HA", align 4
  %22 = call i32 (i8*, ...)* @printf(i8* getelementptr inbounds ([10 x i8], [10 x i8]* @"\01??_C@_09MMEBDMLC@state?5?$CFd?3?$AA@", i32 0, i32 0), i32 %21)
  %23 = load i32, i32* %sum, align 4
  %24 = call i32 (i8*, ...)* @printf(i8* getelementptr inbounds ([8 x i8], [8 x i8]* @"\01??_C@_07KGDPDBII@sum?$DN?$CFd?5?$AA@", i32 0, i32 0), i32 %23)
  %25 = load i32, i32* %x, align 4
  %26 = call i32 (i8*, ...)* @printf(i8* getelementptr inbounds ([6 x i8], [6 x i8]* @"\01??_C@_05LNELLFJF@x?$DN?$CFd?5?$AA@", i32 0, i32 0), i32 %25)
  %27 = call i32 (i8*, ...)* @printf(i8* getelementptr inbounds ([2 x i8], [2 x i8]* @"\01??_C@_01EEMJAFIK@?6?$AA@", i32 0, i32 0))
  %28 = load i32, i32* @"\01?$$state_num@@3HA", align 4
  %29 = add nsw i32 %28, 1
  store i32 %29, i32* @"\01?$$state_num@@3HA", align 4
  %30 = load i32, i32* @"\01?$$state_num@@3HA", align 4
  %31 = call i32 (i8*, ...)* @printf(i8* getelementptr inbounds ([10 x i8], [10 x i8]* @"\01??_C@_09MMEBDMLC@state?5?$CFd?3?$AA@", i32 0, i32 0), i32 %30)
  %32 = load i32, i32* %sum, align 4
  %33 = call i32 (i8*, ...)* @printf(i8* getelementptr inbounds ([8 x i8], [8 x i8]* @"\01??_C@_07KGDPDBII@sum?$DN?$CFd?5?$AA@", i32 0, i32 0), i32 %32)
  %34 = load i32, i32* %x, align 4
  %35 = call i32 (i8*, ...)* @printf(i8* getelementptr inbounds ([6 x i8], [6 x i8]* @"\01??_C@_05LNELLFJF@x?$DN?$CFd?5?$AA@", i32 0, i32 0), i32 %34)
  %36 = call i32 (i8*, ...)* @printf(i8* getelementptr inbounds ([2 x i8], [2 x i8]* @"\01??_C@_01EEMJAFIK@?6?$AA@", i32 0, i32 0))
  %37 = load i32, i32* @"\01?$$state_num@@3HA", align 4
  %38 = add nsw i32 %37, 1
  store i32 %38, i32* @"\01?$$state_num@@3HA", align 4
  ret i32 0
}

declare i32 @printf(i8*, ...) #0

attributes #0 = { "less-precise-fpmad"="false" "no-frame-pointer-elim"="true" "no-frame-pointer-elim-non-leaf" "no-infs-fp-math"="false" "no-nans-fp-math"="false" "stack-protector-buffer-size"="8" "target-cpu"="pentium4" "unsafe-fp-math"="false" "use-soft-float"="false" }

!llvm.ident = !{!0}

!0 = !{!"clang version 3.7.0 (trunk)"}
