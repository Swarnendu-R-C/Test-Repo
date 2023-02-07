cpp = meson.get_compiler('cpp')

# Checking dependency external library

libsystemd = dependency('libsystemd', version : '>=221')

sdbusplus_dep = dependency('sdbusplus', required: false)
if sdbusplus_dep.found()
    sdbusplusplus_prog = find_program('sdbus++')
    sdbuspp_gen_meson_prog = find_program('sdbus++-gen-meson')
else
    sdbusplus_proj = subproject('sdbusplus', required: true)
    sdbusplus_dep = sdbusplus_proj.get_variable('sdbusplus_dep')
    sdbusplusplus_prog = sdbusplus_proj.get_variable('sdbusplusplus_prog')
    sdbuspp_gen_meson_prog = sdbusplus_proj.get_variable(
        'sdbuspp_gen_meson_prog'
    )
endif
sdeventplus_dep = dependency('sdeventplus')
phosphor_dbus_interfaces_dep = dependency(
    'phosphor-dbus-interfaces',
    fallback: [
        'phosphor-dbus-interfaces',
        'phosphor_dbus_interfaces_dep'
    ],
)
phosphor_debug_collector_test_dep = [
    libsystemd,
    sdbusplus_dep,
    sdeventplus_dep,
    phosphor_dbus_interfaces_dep
]

executable(
    'phosphor_debug_collector_test',
    'Test-pdbgc.cpp',
    dependencies : phosphor_debug_collector_test_dep,
    install : true,
)