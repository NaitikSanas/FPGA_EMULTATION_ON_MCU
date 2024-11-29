
set(COMPONENT_SRCS "fpga_emulation.c"
)
# Fix cmake build

idf_component_register(SRCS "${COMPONENT_SRCS}" 
                        INCLUDE_DIRS ".")
