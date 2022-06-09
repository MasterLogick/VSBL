cmake_minimum_required(VERSION 3.23)
project(VSBL LANGUAGES CXX ASM)

set(CMAKE_CXX_STANDARD 20)

set(CMAKE_CXX_FLAGS "-O2 -static -m64 -ffreestanding -fno-PIC -fno-PIE -Wall -Wextra -Werror -fno-exceptions -fno-rtti -nostartfiles -nodefaultlibs -nostdinc++ -mno-red-zone -mno-mmx -mno-sse -mno-sse2 -include ${PROJECT_SOURCE_DIR}/src/placement_new.h")
set(CMAKE_CXX_COMPILER g++)
set(CMAKE_ASM_COMPILER yasm)
set(CMAKE_ASM_FLAGS "-f elf64")
set(CMAKE_LINKER ld)
set(CMAKE_EXE_LINKER_FLAGS "-T ${CMAKE_SOURCE_DIR}/linker.ld -nostdlib -O2 -m elf_x86_64 -M")
set(CMAKE_ASM_COMPILE_OBJECT "<CMAKE_ASM_COMPILER> <DEFINES> <INCLUDES> ${CMAKE_ASM_FLAGS} -o <OBJECT> <SOURCE>")
set(CMAKE_CXX_LINK_EXECUTABLE "<CMAKE_LINKER> <CMAKE_CXX_LINK_FLAGS> <LINK_FLAGS> <OBJECTS> -o <TARGET> <LINK_LIBRARIES>")
set(CMAKE_CXX_COMPILE_OBJECT "<CMAKE_CXX_COMPILER> <DEFINES> <INCLUDES> ${CMAKE_CXX_FLAGS} -o <OBJECT> -c <SOURCE>")

file(GLOB_RECURSE SRC "src/**")

set(CMAKE_EXECUTABLE_SUFFIX .elf)
add_executable(${CMAKE_PROJECT_NAME} ${SRC})
target_include_directories(${CMAKE_PROJECT_NAME} PUBLIC src)

set(KERNEL_PADDING 512)
add_custom_command(TARGET ${CMAKE_PROJECT_NAME} POST_BUILD
        COMMAND sh ${CMAKE_SOURCE_DIR}/extract_image.sh $<TARGET_FILE:${CMAKE_PROJECT_NAME}> $<TARGET_FILE:${CMAKE_PROJECT_NAME}>.bin ${KERNEL_PADDING}
        VERBATIM)

set(VBox_Machine_Name Test_VM)
set(VBox_Machine_Vdi ${CMAKE_SOURCE_DIR}/${VBox_Machine_Name}/${VBox_Machine_Name}.vdi)
add_custom_target(${CMAKE_PROJECT_NAME}_VBox_Update
        COMMAND vboxmanage storageattach ${VBox_Machine_Name} --storagectl SATA --port 0 --medium none
        COMMAND vboxmanage closemedium ${VBox_Machine_Vdi} --delete
        COMMAND vboxmanage convertfromraw $<TARGET_FILE:${CMAKE_PROJECT_NAME}>.bin ${VBox_Machine_Vdi} --format VDI
        COMMAND vboxmanage storageattach ${VBox_Machine_Name} --storagectl SATA --port 0 --type hdd --medium ${VBox_Machine_Vdi}
        DEPENDS ${CMAKE_PROJECT_NAME}
        VERBATIM)