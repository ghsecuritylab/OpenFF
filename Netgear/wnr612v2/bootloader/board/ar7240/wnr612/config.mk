ifneq ($(RAM_VERSION),1)
# ROM version
TEXT_BASE = 0x9f000000
else
# SDRAM version
TEXT_BASE = 0x81000000
endif