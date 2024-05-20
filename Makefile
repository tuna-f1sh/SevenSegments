USER_LIB_PATH = lib
OBJDIR = bin
TARGET = $(firstword $(wildcard *.ino))
FQBN = arduino:samd:mzero_pro_bl

# just to trigger changes to target bin
CSRC := $(wildcard *.c)
CPPSRC := $(wildcard *.cpp)
INCSRC := $(wildcard *.h)

ACLI = arduino-cli
UPLOAD_PORT ?= $(wildcard /dev/tty.usbmodem*)
CLI_FLAGS = --verbose --warnings default --fqbn ${FQBN}

default: ${OBJDIR}/${TARGET}.bin

${OBJDIR}/%.bin: ${TARGET} ${CSRC} ${CPPSRC} ${INCSRC} Makefile | ${OBJDIR}
	${ACLI} compile ${CLI_FLAGS} --libraries ${USER_LIB_PATH} --output-dir ${OBJDIR}

upload: ${OBJDIR}/${TARGET}.bin
	${ACLI} upload --fqbn ${FQBN} --port ${UPLOAD_PORT} --verbose

clean:
	${ACLI} compile --fqbn ${FQBN} --clean --libraries ${USER_LIB_PATH} --output-dir ${OBJDIR}

monitor:
	${ACLI} monitor --port ${UPLOAD_PORT}

tio:
	tio ${UPLOAD_PORT} -b 115200 -e --map ONLCRNL
