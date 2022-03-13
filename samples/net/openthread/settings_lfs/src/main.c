/*
 * SPDX-License-Identifier: Apache-2.0
 */

#include <fs/fs.h>
#include <fs/littlefs.h>
#include <net/openthread.h>
#include <openthread/platform/settings.h>

#include <sys/reboot.h>
#include <logging/log_ctrl.h>
#include <logging/log.h>
LOG_MODULE_REGISTER(app_fs);

FS_LITTLEFS_DECLARE_DEFAULT_CONFIG(storage);
static struct fs_mount_t lfs_storage_mnt = {
	.type = FS_LITTLEFS,
	.fs_data = &storage,
	.storage_dev = (void *)FLASH_AREA_ID(storage),
	.mnt_point = "/settings",
};

void mount_fs(void)
{
	int rc;

	rc = fs_mount(&lfs_storage_mnt);
	if (rc < 0) {
		LOG_ERR("FAIL: mount id %u at %s: %d",
		       (unsigned int)lfs_storage_mnt.storage_dev, lfs_storage_mnt.mnt_point,
		       rc);
		return;
	}
}

/*
 * Usage:
 * On child device:
 * west build --board ubx_evkninab3_nrf52840 -- -DCONFIG_OPENTHREAD_MTD=y \
 *			-DCONFIG_OPENTHREAD_MTD_SED=y
 * On router device:
 * west build --board ubx_evkninab3_nrf52840 -- -DCONFIG_OPENTHREAD_FTD=y \
 *			-DCONFIG_SETTINGS_FS=y
 */
void main(void)
{
#if CONFIG_SETTINGS_FS
	struct openthread_context *ot_context = openthread_get_default_context();
	otInstance *ot_instance = ot_context->instance;

    /* manual mount as we are not using automount */
	mount_fs();

	/* running again, as the first one failed */
	otPlatSettingsInit(ot_instance);
#endif
}

void k_sys_fatal_error_handler(unsigned int reason, const z_arch_esf_t *esf)
{
	LOG_PANIC();
	LOG_ERR("Restarting system");

	sys_reboot(SYS_REBOOT_WARM);
}
