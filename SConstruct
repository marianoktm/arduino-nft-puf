# COMPILAZIONE ENROLLER


en_env = Environment()

en_env.Append(CPPPATH = ['enrollment', 'common', 'pc_common', 'cipher', 'dm_hash'])
en_env.Append(CCFLAGS = '-g -Wall -fdiagnostics-color=always')

en_env.VariantDir(variant_dir = 'build/enrollment', src_dir = 'enrollment', duplicate = 0)
en_env.VariantDir(variant_dir = 'build/enrollment/common', src_dir = 'common', duplicate = 0)
en_env.VariantDir(variant_dir = 'build/enrollment/dm_hash', src_dir = 'dm_hash', duplicate = 0)
en_env.VariantDir(variant_dir = 'build/enrollment/cipher', src_dir = 'cipher', duplicate = 0)
en_env.VariantDir(variant_dir = 'build/enrollment/pc_common', src_dir = 'pc_common', duplicate = 0)

src_enrollment = Glob('build/enrollment/*.c')
src_enrollment.append(Glob('build/enrollment/common/*.c'))
src_enrollment.append(Glob('build/enrollment/pc_common/*.c'))
src_enrollment.append(Glob('build/enrollment/dm_hash/*[!main].c'))
src_enrollment.append(Glob('build/enrollment/cipher/*[!main].c'))

build_enroller = en_env.Program(target = 'build/enrollment/enroller', source = [src_enrollment])
en_env.Alias('enroller', build_enroller)

# COMPILAZIONE BOOTLOADER


bl_env = Environment(
    CC = 'avr-gcc',
    AS = 'avr-as',
    OBJCOPY = 'avr-objcopy',
    PROGSUFFIX='.elf',
    SIZE = 'avr-size',
    OBJDUMP='avr-objdump')

bl_env.Append(CPPPATH = ['bootloader', 'common', 'bootloader/cipher', 'dm_hash'])
bl_env.Append(CCFLAGS = '-g -Wall -Os -fdiagnostics-color=always')
bl_env.Append(CCFLAGS = '-mmcu=atmega328p')
bl_env.Append(CCFLAGS = '-DF_CPU=16000000UL')
bl_env.Append(CCFLAGS = '-DPLAT_ARDUINO')
bl_env.Append(CCFLAGS = '-DAVR')

bl_env.Append(LINKFLAGS = '-g -Wall -Os -fdiagnostics-color=always')
bl_env.Append(LINKFLAGS = '-mmcu=atmega328p')
bl_env.Append(LINKFLAGS = '-Wl,-T./bootloader/linker_script.xn')

bl_env.VariantDir(variant_dir = 'build/bootloader', src_dir = 'bootloader', duplicate = 0)
bl_env.VariantDir(variant_dir = 'build/bootloader/common', src_dir = 'common', duplicate = 0)
bl_env.VariantDir(variant_dir = 'build/bootloader/dm_hash', src_dir = 'dm_hash', duplicate = 0)
bl_env.VariantDir(variant_dir = 'build/bootloader/cipher', src_dir = 'bootloader/cipher', duplicate = 0)

src_bootloader = Glob('build/bootloader/*.c')
# src_bootloader.append(Glob("build/bootloader/*.s")) solo per libreria flash
src_bootloader.append(Glob('build/bootloader/common/*.c'))
src_bootloader.append(Glob('build/bootloader/dm_hash/*[!main].c'))
src_bootloader.append(Glob('build/bootloader/cipher/*.c'))


build_bootloader = [

    bl_env.Program(target='build/bootloader/bootloader', source = [src_bootloader]),

    # Create hex binary file.
    bl_env.Command('build/bootloader/bootloader.hex', 'build/bootloader/bootloader.elf', bl_env['OBJCOPY'] + ' -O ihex $SOURCE $TARGET'),

    # Create disassembled.
    bl_env.Command('build//bootloader/bootloader.lst', 'build/bootloader/bootloader.elf', bl_env['OBJDUMP'] + ' -h -S $SOURCE > $TARGET'),

    # Compute memory usage.
    bl_env.Command(None, 'build/bootloader/bootloader.elf', bl_env['SIZE'] + ' -C --mcu=atmega328p $SOURCE')
]

bl_env.Alias('bootloader', build_bootloader)


# COMPILAZIONE CIFRARIO

sp_env = Environment()

sp_env.Append(CPPPATH = ['cipher'])
sp_env.Append(CCFLAGS = '-g -Wall -fdiagnostics-color=always')
sp_env.Append(CCFLAGS = '-DPC')
sp_env.Append(CCFLAGS = '-DDEBUG')

sp_env.VariantDir(variant_dir = 'build/cipher', src_dir = 'cipher', duplicate = 0)

src_cipher = Glob('build/cipher/*.c')

build_cipher = sp_env.Program(target = 'build/cipher/cipher_test', source = [src_cipher])
sp_env.Alias('cipher_test', build_cipher)

# COMPILAZIONE DEMO HASH FUNCTION DAVIES–MEYER

hp_env = Environment()

hp_env.Append(CPPPATH = ['dm_hash', 'pc_common', 'cipher', 'common'])
hp_env.Append(CCFLAGS = '-g -Wall -fdiagnostics-color=always')
hp_env.Append(CCFLAGS = '-DPC')
hp_env.Append(CCFLAGS = '-DDEBUG')

hp_env.VariantDir(variant_dir = 'build/dm_hash', src_dir = 'dm_hash', duplicate = 0)
hp_env.VariantDir(variant_dir = 'build/dm_hash/pc_common', src_dir = 'pc_common', duplicate = 0)
hp_env.VariantDir(variant_dir = 'build/dm_hash/cipher', src_dir = 'cipher', duplicate = 0)

src_dm_hash = Glob('build/dm_hash/*.c')
src_dm_hash.append(Glob('build/dm_hash/pc_common/*.c'))
src_dm_hash.append(Glob('build/dm_hash/cipher/*[!main].c'))

build_dm_hash = hp_env.Program(target = 'build/dm_hash/dm_hash', source = [src_dm_hash])
hp_env.Alias('dm_hash', build_dm_hash)