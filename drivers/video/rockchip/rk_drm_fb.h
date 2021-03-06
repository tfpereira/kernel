#include <linux/rk_fb.h>

#define RK_DRM_WIN_MASK  0x7fff
#define RK_DRM_CMD_MASK  0x7fff

#define RK_DRM_CALLBACK_VSYNC     0x1
#define RK_DRM_CALLBACK_HOTPLUG   0x2

#define RK_DRM_SCREEN_SET 	1<<0
#define	RK_DRM_SCREEN_BLANK 	1<<1
#define	RK_DRM_WIN_COMMIT     	1<<2
#define RK_DRM_DISPLAY_COMMIT 	1<<3
#define RK_DRM_SCREEN_SET 	1<<0
struct rk_drm_display;
enum drm_screen_type {
	RK_DRM_PRIMARY_SCREEN = 0,
	RK_DRM_EXTEND_SCREEN,
	RK_DRM_MAX_SCREEN_NUM,
};
struct rk_win_data {
	unsigned int		win_id;
	enum data_format 	format;
	int			xpos;
	int			ypos;
	unsigned int		xact;
	unsigned int		yact;
	unsigned int		xsize;
	unsigned int		ysize;
	unsigned int 		xvir;
	bool 			alpha_en;

	dma_addr_t		yrgb_addr;
	dma_addr_t		uv_addr;
	
	bool			enabled;
};

struct rk_drm_screen_ops {
	int (*setenable)(struct rk_drm_display *, int enable);
	int (*setmode)(struct rk_drm_display *, struct fb_videomode *mode);
	int (*getmodelist)(struct rk_drm_display *, struct list_head *modelist);
	int (*getedid)(struct rk_drm_display *,int block, unsigned char *buff);
};

struct rk_drm_display {
	/***** hardware define *****/ 
	enum drm_screen_type screen_type;

	struct list_head *modelist;
	int num_videomode;
	int best_mode;
	bool is_connected;
	int num_win;

	struct rk_drm_screen_ops screen_ops;
	/***** user fill info  *****/
	struct fb_videomode *mode;
	bool enable;
	bool screen_status;
	struct rk_win_data win[RK30_MAX_LAYER_SUPPORT]; 
	void (*event_call_back)(struct rk_drm_display *drm_disp,int win_id,int event);
};


struct rk_drm_screen_private {
	atomic_t wait_vsync_done;
	wait_queue_head_t wait_vsync_queue;
	
	struct rk_drm_display drm_disp;
	struct rk_drm_screen_ops screen_ops;
	struct rk_lcdc_driver *lcdc_dev_drv;
};
struct rk_drm_private {
	struct rk_drm_screen_private screen_priv[RK_DRM_MAX_SCREEN_NUM];
	int num_screen;

	int disp_mode;
};

extern struct rk_drm_display *rk_drm_get_diplay(int screen_type);
extern int rk_drm_disp_handle(struct rk_drm_display *drm_disp,unsigned int cmd_id,unsigned int argv);
