# Test-Repo
It is a test repository which will be used for mock testing of various OpenBMC tools

## To build the repo/code using OpenBMC SDK do the following
```
    1) unset LD_LIBRARY_PATH

    2) . /your path to installed SDKs/TestSDK/1030-ghe/image/environment-setup-armv7ahf-vfpv4d16-openbmc-linux-gnueabi

    3) meson <builddir>

    4) ninja -C builddir
```

## To clean
`ninja -C <builddir> clean`
