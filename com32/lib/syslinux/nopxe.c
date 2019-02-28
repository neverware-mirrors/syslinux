/*
 * Dummy implementation of the PXE API, for non-PXE cores
 */

#include <syslinux/pxe_api.h>

int __weak pxe_call(int func, void *ptr)
{
    (void)func;
    (void)ptr;

    return PXENV_STATUS_UNSUPPORTED;
}

void __weak unload_pxe(uint16_t flags)
{
    (void)flags;
}

uint32_t __weak pxe_dns(const char *str)
{
    (void)str;
    return 0;			/* Not found */
}

uint32_t __weak SendCookies;
uint8_t  __weak KeepPXE;

void __weak http_bake_cookies(void)
{
}
