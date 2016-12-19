#include <string.h>
#include "system.h"
#include "genesis.h"

system_type detect_system_type(uint8_t *rom, long filesize)
{
	if (filesize >= 0x104 && !memcmp("SEGA", rom + 0x100, 4)) {
		//TODO: Differentiate between vanilla Genesis and Sega CD/32X games
		return SYSTEM_GENESIS;
	}
	//TODO: Detect SMS and Jaguar ROMs here
	
	//More certain checks failed, look for a valid 68K reset vector
	if (filesize >= 8) {
		uint32_t reset = rom[4] << 24 | rom[5] << 16 | rom[6] << 8 | rom[7];
		if (!(reset & 1) && reset < filesize) {
			//we have a valid looking reset vector, assume it's a Genesis ROM
			return SYSTEM_GENESIS;
		}
	}
	return SYSTEM_UNKNOWN;
}

system_header *alloc_config_system(system_type stype, void *rom, uint32_t rom_size, void *lock_on, uint32_t lock_on_size, uint32_t opts, uint8_t force_region, rom_info *info_out)
{
	switch (stype)
	{
	case SYSTEM_GENESIS:
		return &(alloc_config_genesis(rom, rom_size, lock_on, lock_on_size, opts, force_region, info_out))->header;
	default:
		return NULL;
	}
}