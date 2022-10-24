################################################################################
# Automatically-generated file. Do not edit!
################################################################################

SHELL = cmd.exe

# Each subdirectory must supply rules for building sources it contributes
aic3204_loop_linein.obj: ../aic3204_loop_linein.c $(GEN_OPTS) | $(GEN_FILES) $(GEN_MISC_FILES)
	@echo 'Building file: "$<"'
	@echo 'Invoking: C5500 Compiler'
	"" --include_path="/include" --include_path="C:/Users/raiel/Downloads/ezdsp5502_BSL_RevC/ezdsp5502_v1/C55xxCSL/include" --include_path="C:/Users/raiel/Downloads/ezdsp5502_BSL_RevC/ezdsp5502_v1/include" --symdebug:dwarf --diag_warning=225 --memory_model=large --ptrdiff_size=32 --silicon_version=5502 --preproc_with_compile --preproc_dependency="$(basename $(<F)).d_raw" $(GEN_OPTS__FLAG) "$<"
	@echo 'Finished building: "$<"'
	@echo ' '

%.obj: ../%.c $(GEN_OPTS) | $(GEN_FILES) $(GEN_MISC_FILES)
	@echo 'Building file: "$<"'
	@echo 'Invoking: C5500 Compiler'
	"" --include_path="/include" --include_path="C:/Users/raiel/Downloads/ezdsp5502_BSL_RevC/ezdsp5502_v1/C55xxCSL/include" --include_path="C:/Users/raiel/Downloads/ezdsp5502_BSL_RevC/ezdsp5502_v1/include" --symdebug:dwarf --diag_warning=225 --memory_model=large --ptrdiff_size=32 --silicon_version=5502 --preproc_with_compile --preproc_dependency="$(basename $(<F)).d_raw" $(GEN_OPTS__FLAG) "$<"
	@echo 'Finished building: "$<"'
	@echo ' '

aic3204_tone_headphone.obj: ../aic3204_tone_headphone.c $(GEN_OPTS) | $(GEN_FILES) $(GEN_MISC_FILES)
	@echo 'Building file: "$<"'
	@echo 'Invoking: C5500 Compiler'
	"" --include_path="/include" --include_path="C:/Users/raiel/Downloads/ezdsp5502_BSL_RevC/ezdsp5502_v1/C55xxCSL/include" --include_path="C:/Users/raiel/Downloads/ezdsp5502_BSL_RevC/ezdsp5502_v1/include" --symdebug:dwarf --diag_warning=225 --memory_model=large --ptrdiff_size=32 --silicon_version=5502 --preproc_with_compile --preproc_dependency="$(basename $(<F)).d_raw" $(GEN_OPTS__FLAG) "$<"
	@echo 'Finished building: "$<"'
	@echo ' '


